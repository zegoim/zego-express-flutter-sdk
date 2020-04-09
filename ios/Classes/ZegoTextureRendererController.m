//
//  ZegoTextureRendererController.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRendererController.h"

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
    
    [self.allRenderers setObject:renderer forKey:@(renderer.textureID)];
    
    return renderer.textureID;
}

- (void)updateTextureRenderer:(int64_t)textureID viewWidth:(int)width viewHeight:(int)height {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        return;
    }
    
    [renderer updateRenderSize:CGSizeMake(width, height)];
}

- (void)destroyTextureRenderer:(int64_t)textureID {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        return;
    }
    
    [self.allRenderers removeObjectForKey:@(renderer.textureID)];
    
    // Release renderer
    [renderer destroy];
}


#pragma mark - Dart Express Engine API Operation

/// The following methods are only triggered by the dart `zego_express_api`

- (void)initController {
    // Set up custom video render handler
    [[ZegoExpressEngine sharedEngine] setCustomVideoRenderHandler:self];
}

- (void)uninitController {
    [self removeAllRenderer];
}

- (BOOL)addCapturedRenderer:(int64_t)textureID key:(NSNumber *)channel {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        return NO;
    }
    
    [self.capturedRenderers setObject:renderer forKey:channel];
    return YES;
}

- (void)removeCapturedRenderer:(NSNumber *)channel {
    [self.capturedRenderers removeObjectForKey:channel];
}

- (BOOL)addRemoteRenderer:(int64_t)textureID key:(NSString *)streamID {
    
    ZegoTextureRenderer *renderer = [self.allRenderers objectForKey:@(textureID)];
    
    if (!renderer) {
        return NO;
    }
    
    [self.remoteRenderers setObject:renderer forKey:streamID];
    return YES;
}

- (void)removeRemoteRenderer:(NSString *)streamID {
    [self.remoteRenderers removeObjectForKey:streamID];
}

- (void)startRendering {
    if (self.isRendering) {
        return;
    }
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self
                                               selector:@selector(onDisplayLink:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    // 30帧刷新，对于 120hz刷新率的ipad，则为60帧刷新
    _displayLink.frameInterval = 2;
    _displayLink.paused = NO;
    
    self.isRendering = YES;
}

- (void)stopRendering {
    if (!self.isRendering) {
        return;
    }
    
    // TODO: 暂时先限定死只有在所有renderer都不存在时，停止渲染才成功
    if (self.capturedRenderers.count == 0 && self.remoteRenderers == 0) {
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
