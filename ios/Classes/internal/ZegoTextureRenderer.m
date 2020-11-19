//
//  ZegoTextureRenderer.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRenderer.h"
#import "ZegoLog.h"

@interface ZegoTextureRenderer()

@property (nonatomic, weak) id<FlutterTextureRegistry> registry;
@property (nonatomic, strong)EAGLContext *context;
@property (nonatomic, strong) dispatch_semaphore_t lock;

@end

@implementation ZegoTextureRenderer
{
    CVPixelBufferRef m_pInputFrameBuffer;
    CVPixelBufferRef m_pRenderFrameBuffer;
    
    dispatch_queue_t  m_opengl_queue;
    GLfloat m_lstVertices[8];
    GLfloat m_lstTexCoord[8];
    CVOpenGLESTextureCacheRef m_pTexCache;
    GLuint m_hProgram;
    GLuint m_hVertexShader;
    GLuint m_hFragShader;
    GLuint m_framebuffer;
    int m_nFrameUniform;
    CVPixelBufferPoolRef m_buffer_pool;
    CVOpenGLESTextureRef m_output_texture;
    GLint m_position;
    GLint m_texcoord;
    ZegoViewMode m_view_mode;
    
    int m_img_width;
    int m_img_height;
    
    BOOL m_isUseMirror;
    BOOL m_config_changed;
    
    //当有新帧到达或者屏幕大小发生变化时该标志位才会置位YES
    BOOL m_isNewFrameAvailable;
}

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height {
    self = [super init];
    if (self) {
        _registry = registry;
        m_pRenderFrameBuffer = nil;
        m_output_texture = nil;
        m_view_mode = ZegoViewModeAspectFit;
        _textureID  = [registry registerTexture:self];
        m_opengl_queue = dispatch_queue_create([[NSString stringWithFormat:@"opengl_queue%lld", _textureID] UTF8String], NULL);
       _lock = dispatch_semaphore_create(1);
        _viewWidth = width;
        _viewHeight = height;
        
        m_isUseMirror = NO;
        
        m_img_width = 0;
        m_img_height = 0;
        m_config_changed = YES;
        
        m_isNewFrameAvailable = NO;
        
        [self createPixelBufferPool:&m_buffer_pool width:_viewWidth height:_viewHeight];
        
        __weak ZegoTextureRenderer *weak_ptr = self;
        dispatch_async(m_opengl_queue, ^{
            ZegoTextureRenderer *strong_ptr = weak_ptr;
            if(strong_ptr)
                [strong_ptr setupGL];
        });
    }

    ZGLog(@"[ZegoTextureRenderer] [init] renderer:%p", self);
    
    return self;
}

- (void)destroy {
    [self destroyPixelBufferPool:m_buffer_pool];
    
    dispatch_async(m_opengl_queue, ^{
        
        [EAGLContext setCurrentContext:self.context];
        
        if(self->m_pInputFrameBuffer){
            CVBufferRelease(self->m_pInputFrameBuffer);
            self->m_pInputFrameBuffer = nil;
        }
        
        if(self->m_pRenderFrameBuffer){
            CVBufferRelease(self->m_pRenderFrameBuffer);
            self->m_pRenderFrameBuffer = nil;
        }
        
        if(self->m_output_texture) {
            CFRelease(self->m_output_texture);
            self->m_output_texture = NULL;
        }
        
        if (self->m_pTexCache) {
            CFRelease(self->m_pTexCache);
            self->m_pTexCache = 0;
        }
        
        if(self->m_framebuffer) {
            glDeleteBuffers(1, &self->m_framebuffer);
        }
        
        if(self->m_hVertexShader) {
            glDeleteShader(self->m_hVertexShader);
        }
        
        if(self->m_hFragShader) {
            glDeleteShader(self->m_hFragShader);
        }
        
        if(self->m_hProgram) {
            glDeleteProgram(self->m_hProgram);
        }
    });
    
    // Release GPU Resource
    [self.registry unregisterTexture:_textureID];

    ZGLog(@"[ZegoTextureRenderer] [release] renderer:%p", self);
}


- (void)setSrcFrameBuffer:(CVPixelBufferRef)srcFrameBuffer {
    
    CVBufferRetain(srcFrameBuffer);
    
    dispatch_async(m_opengl_queue, ^{
        
        if(self->m_pInputFrameBuffer) {
            CVBufferRelease(self->m_pInputFrameBuffer);
        }
        self->m_pInputFrameBuffer = srcFrameBuffer;
        
    });
    
    // TODO: 检查该标志位的位置是否正确
    m_isNewFrameAvailable = YES;
}

- (void)updateRenderSize:(CGSize)size {
    //切换大小时注意线程问题，保证线程同步
    
    __weak ZegoTextureRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoTextureRenderer *strong_ptr = weak_ptr;
        if(strong_ptr == nil)
            return;
        
        if(strong_ptr.viewWidth != size.width || strong_ptr.viewHeight != size.height) {
            strong_ptr.viewWidth = size.width;
            strong_ptr.viewHeight = size.height;
            
            [strong_ptr destroyPixelBufferPool:self->m_buffer_pool];
            [strong_ptr createPixelBufferPool:&self->m_buffer_pool width:strong_ptr.viewWidth height:strong_ptr.viewHeight];
            
            self->m_config_changed = YES;
            
        }
        
    });
    
    m_isNewFrameAvailable = YES;
    
}

- (void)setViewMode:(ZegoViewMode)mode {
    
    dispatch_async(m_opengl_queue, ^{
        if(self->m_view_mode == mode)
            return;
        self->m_view_mode = mode;
        
        self->m_config_changed = YES;
    });
}

- (void)setUseMirrorEffect:(BOOL)isUse {
    if (m_isUseMirror == isUse) {
        return;
    }
    
    __weak ZegoTextureRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoTextureRenderer *strong_ptr = weak_ptr;
        if(strong_ptr == nil)
            return;
        
        if(self->m_isUseMirror == isUse)
            return;
        
        self->m_isUseMirror = isUse;
        self->m_config_changed = YES;
    });
}

- (void)setBackgroundColor:(int)color {
    // TODO: 补充背景颜色
}

- (BOOL)isNewFrameAvailable {
    return m_isNewFrameAvailable;
}

- (void)notifyDrawNewFrame {
    [self.registry textureFrameAvailable:self.textureID];
}

- (void)setupGL {
    
    /*完全显示，view port坐标是[-1, 1]*/
    m_lstVertices[0] = -1.0;
    m_lstVertices[1] = -1.0;
    
    m_lstVertices[2] = 1.0;
    m_lstVertices[3] = -1.0;
    
    m_lstVertices[4] = -1.0;
    m_lstVertices[5] = 1.0;
    
    m_lstVertices[6] = 1.0;
    m_lstVertices[7] = 1.0;
    

    // * texture
    m_lstTexCoord[0] = 0.0;
    m_lstTexCoord[1] = 0.0;
    
    m_lstTexCoord[2] = 1.0;
    m_lstTexCoord[3] = 0.0;
    
    m_lstTexCoord[4] = 0.0;
    m_lstTexCoord[5] = 1.0;
    
    m_lstTexCoord[6] = 1.0;
    m_lstTexCoord[7] = 1.0;
    
   
    //初始化context
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(self.context == nil) {
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
        
    [EAGLContext setCurrentContext:self.context];
    
    // * disable depth test
    glDisable(GL_DEPTH_TEST);
    
    // * create our texture cache
    CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.context, NULL, &m_pTexCache);
    //CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.context, NULL, &m_pTexCache2);
    // * create our program
    m_hProgram = glCreateProgram();
    
    // * compile vertex shader
    const static char *strVertexShader = " \
    attribute vec4 position; \
    attribute mediump vec4 texcoord; \
    \
    varying mediump vec2 textureCoordinate; \
    \
    void main() \
    { \
    gl_Position = position; \
    textureCoordinate = texcoord.xy; \
    } \
    ";
    
    m_hVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_hVertexShader, 1, &strVertexShader, 0);
    glCompileShader(m_hVertexShader);
    GLint status;
    glGetShaderiv(m_hVertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        // * compile shader error
        // * to_do: handle error
        NSLog(@"OpenGL glCompileShader GL_VERTEX_SHADER error");
    }
    
    // * compile fragment shader
    const static char *strFragmentShader = " \
    varying highp vec2 textureCoordinate; \
    uniform sampler2D frame; \
    \
    void main() \
    { \
    gl_FragColor = texture2D(frame, textureCoordinate);\
    } \
    ";
    m_hFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_hFragShader, 1, &strFragmentShader, 0);
    glCompileShader(m_hFragShader);
    // * to_do: check status
    if (status != GL_TRUE) {
        // * compile shader error
        // * to_do: handle error
        NSLog(@"OpenGL glCompileShader GL_FRAGMENT_SHADER error");
    }
    
    // * attach shader to program
    glAttachShader(m_hProgram, m_hVertexShader);
    glAttachShader(m_hProgram, m_hFragShader);
    
    // * link program
    glLinkProgram(m_hProgram);
    glGetProgramiv(m_hProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        // * to_do: handle error
        NSLog(@"OpenGL glLinkProgram error");
    }
    
    // * use program
    glUseProgram(m_hProgram);
    
    // * get uniform index
    m_nFrameUniform = glGetUniformLocation(m_hProgram, "frame");
    
    /* 获取OpenGL为我们分配的两个attribute位置 */
    m_position = glGetAttribLocation(m_hProgram, "position");
    m_texcoord = glGetAttribLocation(m_hProgram, "texcoord");
    
    // * enable attributes
    glEnableVertexAttribArray(m_position);
    glEnableVertexAttribArray(m_texcoord);
    
    
    glGenFramebuffers(1, &m_framebuffer);
   
}

- (void)setupVAO:(int)imageWidth height:(int)imageHeight {
    //重新对texcoord数组赋值，负责缩放以及镜像
    
    float fWidthScale = 1.0;
    float fHeightScale = 1.0;
    
    
    m_img_width = imageWidth;
    m_img_height = imageHeight;
    //NSLog(@"setup vao, textureID: %lld, img :(%d, %d), view :(%d, %d),  ", _textureID, imageWidth, imageHeight, self.viewWidth, self.viewHeight);
    
    //实际倍数
    float fViewRate = (float)self.viewWidth / (float)self.viewHeight;
    float fImageRate = (float)imageWidth / (float)imageHeight;
    
    if (m_view_mode == ZegoViewModeAspectFit) {
        // * blackbar
        if(fImageRate > fViewRate) {
            float actualHeight = self.viewWidth / fImageRate;
            fHeightScale = actualHeight / (float)self.viewHeight;
        }else {
            float actualWidth = self.viewHeight * fImageRate;
            fWidthScale = actualWidth / (float)self.viewWidth;
        }
    } else if(m_view_mode == ZegoViewModeAspectFill) {
        // * crop
        if(fImageRate > fViewRate) {
            float actualWidth = self.viewHeight * fImageRate;
            fWidthScale = actualWidth / (float)self.viewWidth;

        }else {
            float actualHeight = self.viewWidth / fImageRate;
            fHeightScale = actualHeight / (float)self.viewHeight;
            
        }
    } else if(m_view_mode == ZegoViewModeScaleToFill) {
        // * stretch
    }
    
    
    m_lstVertices[0] = -fWidthScale;
    m_lstVertices[1] = -fHeightScale;
    
    m_lstVertices[2] = fWidthScale;
    m_lstVertices[3] = -fHeightScale;
    
    m_lstVertices[4] = -fWidthScale;
    m_lstVertices[5] = fHeightScale;
    
    m_lstVertices[6] = fWidthScale;
    m_lstVertices[7] = fHeightScale;
    
    if(m_isUseMirror) {
        for (int i = 0; i < 4; i++) {
            m_lstVertices[2 * i] = - m_lstVertices[2 * i];
        }
    }
    
    
    m_config_changed = NO;
    // * enable attributes
}

- (void)processingData {
    
    [EAGLContext setCurrentContext:self.context];
    
    /* 销毁上一帧的texture缓存 */
    if(m_output_texture) {
        CFRelease(m_output_texture);
        m_output_texture = NULL;
    }
    
  
    if(m_pInputFrameBuffer == nil /*|| m_pProcessFrameBuffer == nil*/)
        return;

    int width = (int)CVPixelBufferGetWidth(m_pInputFrameBuffer);
    int height = (int)CVPixelBufferGetHeight(m_pInputFrameBuffer);

    /* 以image宽高、viewg宽高计算纹理贴图坐标 */
    if(m_config_changed || width != m_img_width || height != m_img_height)
        [self setupVAO:width height:height];

    CVPixelBufferRef processBuffer;
    CVReturn ret = CVPixelBufferPoolCreatePixelBuffer(nil, m_buffer_pool, &processBuffer);
    if(ret != kCVReturnSuccess)
        return;

    /* create input frame texture from sdk */
    CVOpenGLESTextureRef texture_input = NULL;
    [self createTexture:&texture_input FromPixelBuffer:m_pInputFrameBuffer];

    //create output frame texture to flutter
    [self createTexture:&m_output_texture FromPixelBuffer:processBuffer];

    if (texture_input && m_output_texture) {
        
        //Bind
        glBindTexture(CVOpenGLESTextureGetTarget(m_output_texture), CVOpenGLESTextureGetName(m_output_texture));
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, CVOpenGLESTextureGetName(m_output_texture), 0);
        
        glViewport(0, 0, self.viewWidth, self.viewHeight);
        
        // 黑色背景用于填充黑边
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(m_hProgram);
        
        
        glBindTexture(CVOpenGLESTextureGetTarget(texture_input), CVOpenGLESTextureGetName(texture_input));
        glUniform1i(m_nFrameUniform, 0);
        
        
        glEnableVertexAttribArray(m_position);
        glEnableVertexAttribArray(m_texcoord);
        
        glVertexAttribPointer(m_position, 2, GL_FLOAT, 0, 0, m_lstVertices);
        glVertexAttribPointer(m_texcoord, 2, GL_FLOAT, 0, 0, m_lstTexCoord);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();
        

        //std::lock_guard<std::mutex> lock(m_mutex);
        dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
        if(self->m_pRenderFrameBuffer)
            CVBufferRelease(self->m_pRenderFrameBuffer);
        
        self->m_pRenderFrameBuffer = processBuffer;
        dispatch_semaphore_signal(_lock);
        //CVBufferRetain(self->m_pRenderFrameBuffer);
        
        CFRelease(texture_input);
    }
}


#pragma mark - FlutterTexture Delegate
- (CVPixelBufferRef)copyPixelBuffer {
   
    __weak ZegoTextureRenderer *weak_ptr = self;
    dispatch_async(m_opengl_queue, ^{
        ZegoTextureRenderer *strong_ptr = weak_ptr;
        [strong_ptr processingData];
    });
    
    CVPixelBufferRef temp = nil;
    
    //std::lock_guard<std::mutex> lock(m_mutex);
    dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
    if (m_pRenderFrameBuffer) {
       temp = m_pRenderFrameBuffer;
        CVBufferRetain(temp);
    }
    dispatch_semaphore_signal(_lock);
    
    m_isNewFrameAvailable = NO;
    return temp;
}

- (void)onTextureUnregistered:(NSObject<FlutterTexture>*)texture {
    ZGLog(@"[ZegoTextureRenderer] [unregistered] renderer:%p", texture);
}

#pragma mark OpenGL Methods

- (void)createTexture:(CVOpenGLESTextureRef *)texture FromPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    
    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                                m_pTexCache,
                                                                pixelBuffer,
                                                                NULL,
                                                                GL_TEXTURE_2D,
                                                                GL_RGBA,
                                                                (GLsizei)width,
                                                                (GLsizei)height,
                                                                GL_BGRA,
                                                                GL_UNSIGNED_BYTE,
                                                                0,
                                                                texture);
    if(err != kCVReturnSuccess) {
        CFRelease(*texture);
        return;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(CVOpenGLESTextureGetTarget(*texture), CVOpenGLESTextureGetName(*texture));
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
}

- (void)createPixelBufferPool:(CVPixelBufferPoolRef *)pool width:(int)width height:(int)height {
    NSDictionary *pixelBufferAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithBool:YES], (id)kCVPixelBufferOpenGLCompatibilityKey,
                                           [NSNumber numberWithInt:width], (id)kCVPixelBufferWidthKey,
                                           [NSNumber numberWithInt:height], (id)kCVPixelBufferHeightKey,
                                           [NSDictionary dictionary], (id)kCVPixelBufferIOSurfacePropertiesKey,
                                           [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
                                           nil
                                           ];
    
    CFDictionaryRef ref = (__bridge CFDictionaryRef)pixelBufferAttributes;
    CVReturn ret = CVPixelBufferPoolCreate(nil, nil, ref, pool);
    if (ret != kCVReturnSuccess) {
        return;
    }
    
}

- (void)destroyPixelBufferPool:(CVPixelBufferPoolRef)pool {
    if(pool == nil)
        return;
    
    CVPixelBufferPoolFlushFlags flag = 0;
    CVPixelBufferPoolFlush(pool, flag);
    CFRelease(pool);
    pool = nil;
    
}


@end
