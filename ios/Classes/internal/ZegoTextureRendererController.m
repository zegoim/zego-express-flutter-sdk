//
//  ZegoTextureRendererController.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRendererController.h"
#import "ZegoLog.h"

@interface ZegoTextureRendererController()

// BufferRenderers for caching, the renderer created will not be immediately added to the renderers dictionary, but the developer will need to explicitly bind the relationship
@property (strong) NSMutableDictionary<NSNumber *, ZegoTextureRenderer*> *allRenderers;

@property (strong) NSMutableDictionary<NSNumber *, ZegoTextureRenderer*> *capturedRenderers;
@property (strong) NSMutableDictionary<NSString *, ZegoTextureRenderer*> *remoteRenderers;
@property(readonly, nonatomic) CADisplayLink* displayLink;
@property (nonatomic, assign) BOOL isRendering;

@end

@implementation ZegoTextureRendererController

+ (instancetype)sharedInstance {
    static ZegoTextureRendererController *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoTextureRendererController alloc] init];
    });
    return instance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _capturedRenderers = [NSMutableDictionary dictionary];
        _remoteRenderers = [NSMutableDictionary dictionary];
        _allRenderers = [NSMutableDictionary dictionary];
        _isRendering = NO;
    }
    return self;
}

#pragma mark - Dart Texture Render Utils Operation

/// The following methods are only triggered by the dart `zego_texture_render_utils`

- (int64_t)createTextureRenderer:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height {
    
    ZegoTextureRenderer *renderer = [[ZegoTextureRenderer alloc] initWithTextureRegistry:registry viewWidth:width viewHeight:height];

    ZGLog(@"[createTextureRenderer] textureID:%ld, renderer:%p", (long)renderer.textureID, renderer);
    
    [self.allRenderers setObject:renderer forKey:@(renderer.textureID)];

    [self logCurrentRenderers];
    
    return renderer.textureID;
}

- (BOOL)updateTextureRenderer:(int64_t)textureID viewWidth:(int)width viewHeight:(int)height {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        ZGLog(@"[updateTextureRendererSize] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[updateTextureRendererSize] textureID:%ld, renderer:%p", (long)textureID, renderer);
    
    [renderer updateRenderSize:CGSizeMake(width, height)];

    [self logCurrentRenderers];

    return YES;
}

- (BOOL)destroyTextureRenderer:(int64_t)textureID {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        ZGLog(@"[destroyTextureRenderer] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[destroyTextureRenderer] textureID:%ld, renderer:%p", (long)textureID, renderer);
    
    [self.allRenderers removeObjectForKey:@(renderer.textureID)];
    
    // Release renderer
    [renderer destroy];

    [self logCurrentRenderers];

    return YES;
}

- (void)logCurrentRenderers {
    NSMutableString *desc = [[NSMutableString alloc] init];
    for (NSNumber *i in self.allRenderers) {
        ZegoTextureRenderer *eachRenderer = self.allRenderers[i];
        [desc appendFormat:@"[ID:%d|Rnd:%p] ", i.intValue, eachRenderer];
    }
    ZGLog(@"[ZegoTextureRendererController] currentRenderers: %@", desc);
}


#pragma mark - Dart Express Engine API Operation

/// The following methods are only triggered by the dart `zego_express_api`

- (void)initController {

    // Enable custom video render
    ZegoCustomVideoRenderConfig *renderConfig = [[ZegoCustomVideoRenderConfig alloc] init];
    renderConfig.frameFormatSeries = ZegoVideoFrameFormatSeriesRGB;
    renderConfig.bufferType = ZegoVideoBufferTypeCVPixelBuffer;
    [[ZegoExpressEngine sharedEngine] enableCustomVideoRender:YES config:renderConfig];

    // Set up custom video render handler
    [[ZegoExpressEngine sharedEngine] setCustomVideoRenderHandler:self];
}

- (void)uninitController {
    [self removeAllRenderer];
}

- (BOOL)addCapturedRenderer:(int64_t)textureID key:(NSNumber *)channel {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        ZGLog(@"[addCapturedRenderer] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[addCapturedRenderer] textureID:%ld, renderer:%p, channel:%d", (long)textureID, renderer, channel.intValue);
    
    [self.capturedRenderers setObject:renderer forKey:channel];

    [self logCurrentRenderers];

    return YES;
}

- (void)removeCapturedRenderer:(NSNumber *)channel {
    ZegoTextureRenderer *renderer = [self.capturedRenderers objectForKey:channel];

    if (!renderer) {
        ZGLog(@"[removeCapturedRenderer] renderer for channel:%d not exists", channel.intValue);
        [self logCurrentRenderers];
        return;
    }

    ZGLog(@"[removeCapturedRenderer] channel:%d, renderer:%p", channel.intValue, renderer);

    [self.capturedRenderers removeObjectForKey:channel];

    [self logCurrentRenderers];
}

- (BOOL)addRemoteRenderer:(int64_t)textureID key:(NSString *)streamID {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        ZGLog(@"[addRemoteRenderer] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[addRemoteRenderer] textureID:%ld, renderer:%p, streamID:%@", (long)textureID, renderer, streamID);
    
    [self.remoteRenderers setObject:renderer forKey:streamID];

    [self logCurrentRenderers];

    return YES;
}

- (void)removeRemoteRenderer:(NSString *)streamID {
    ZegoTextureRenderer *renderer = [self.remoteRenderers objectForKey:streamID];

    if (!renderer) {
        ZGLog(@"[removeRemoteRenderer] renderer for streamID:%@ not exists", streamID);
        [self logCurrentRenderers];
        return;
    }

    ZGLog(@"[removeRemoteRenderer] streamID:%@, renderer:%p", streamID, renderer);

    [self.remoteRenderers removeObjectForKey:streamID];

    [self logCurrentRenderers];
}

- (void)startRendering {
    if (self.isRendering) {
        return;
    }
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onDisplayLink:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];


    if (@available(iOS 10.0, *)) {
        // Set the FPS to 30Hz
        _displayLink.preferredFramesPerSecond = 30;
    } else {
        // The FPS of iOS9 and below is 60Hz, so set triggered every two frames, that is, 30Hz
        _displayLink.frameInterval = 2;
    }

    _displayLink.paused = NO;
    
    self.isRendering = YES;
}

- (void)stopRendering {
    if (!self.isRendering) {
        return;
    }
    
    // TODO: 暂时先限定死只有在所有renderer都不存在时，停止渲染才成功
    if (self.capturedRenderers.count == 0 && self.remoteRenderers.count == 0) {
        self.displayLink.paused = YES;
        [_displayLink invalidate];
        
        self.isRendering = NO;
    }
}

#pragma mark - Private Methods

- (void)removeAllRenderer {
    [self.capturedRenderers removeAllObjects];
    [self.remoteRenderers removeAllObjects];
    [self.allRenderers removeAllObjects];
}

- (void)onDisplayLink:(CADisplayLink*)link {
    
    // Render local
    for (NSNumber *key in self.capturedRenderers) {
        ZegoTextureRenderer *renderer = [self.capturedRenderers objectForKey:key];
        if(!renderer || ![renderer isNewFrameAvailable])
            continue;
        
        [renderer notifyDrawNewFrame];
    }
    
    // Render remote
    for (NSString *key in self.remoteRenderers) {
        ZegoTextureRenderer *renderer = [self.remoteRenderers objectForKey:key];
        if(!renderer || ![renderer isNewFrameAvailable])
            continue;
        
        [renderer notifyDrawNewFrame];
    }
}


#pragma mark - ZegoCustomVideoRenderHandler

/// Remote playing stream video frame raw data callback, you can differentiate different streams by streamID
///
/// @param buffer video data of CVPixelBuffer format
/// @param param Video frame param
/// @param flipMode video flip mode
/// @param channel Publishing stream channel.
- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param flipMode:(ZegoVideoFlipMode)flipMode channel:(ZegoPublishChannel)channel {
    if (!self.isRendering) {
        return;
    }
    
    ZegoTextureRenderer *renderer = [self.capturedRenderers objectForKey:@(channel)];
    if (renderer) {
        [renderer setUseMirrorEffect:flipMode == 1];
        [renderer setSrcFrameBuffer:buffer];
    }
}

/// Remote playing stream video frame CVPixelBuffer data callback, you can differentiate different streams by streamID
///
/// @param buffer video data of CVPixelBuffer format
/// @param param Video frame param
/// @param streamID Stream ID
- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param streamID:(NSString *)streamID {
    if (!self.isRendering) {
        return;
    }
    
    ZegoTextureRenderer *renderer = [self.remoteRenderers objectForKey:streamID];
    if (renderer) {
        [renderer setSrcFrameBuffer:buffer];
    }
}

@end
