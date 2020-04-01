//
//  ZegoTextureRenderController.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRenderController.h"

@interface ZegoTextureRenderController()

// 缓存用的BuffList,create出来的renderer不会立即被加入到renderers字典中,而是需要客户显式绑定关系
@property (strong) NSMutableArray<ZegoTextureRenderer*> *rendererList;

@property (strong) NSMutableDictionary<NSNumber *, ZegoTextureRenderer*> *captureRenderers;
@property (strong) NSMutableDictionary<NSString *, ZegoTextureRenderer*> *remoteRenderers;
@property(readonly, nonatomic) CADisplayLink* displayLink;
@property (nonatomic, assign) BOOL isRendering;

@end

@implementation ZegoTextureRenderController

+ (instancetype)sharedInstance
{
    static ZegoTextureRenderController *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoTextureRenderController alloc] init];
    });
    return instance;
}

- (instancetype)init
{
    if(self = [super init])
    {
        _captureRenderers = [[NSMutableDictionary alloc] init];
        _remoteRenderers = [[NSMutableDictionary alloc] init];
        _rendererList = [NSMutableArray array];
        _isRendering = NO;
    }
    
    return self;
}

- (void)initController
{
    ZegoExpressEngine *engine = [ZegoExpressEngine sharedEngine];
    if(engine)
    {
        [engine setCustomVideoRenderHandler:self];
    }
}

- (void)uninitController
{
    ZegoExpressEngine *engine = [ZegoExpressEngine sharedEngine];
    if(engine)
    {
        [engine setCustomVideoRenderHandler:nil];
    }
    
    [self removeAllRenderer];
}

- (int64_t)createRenderer:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height
{
    ZegoTextureRenderer *renderer = [[ZegoTextureRenderer alloc] initWithTextureRegistry:registry viewWidth:width viewHeight:height];
    [self.rendererList addObject:renderer];
    
    return renderer.textureID;
}

- (void)releaseRenderer:(int64_t)textureID
{
    ZegoTextureRenderer *renderer = [self getRendererFromList:textureID];
    
    if(renderer)
    {
        [self.rendererList removeObject:renderer];
        renderer = nil;
    }
}

- (BOOL)addCaptureRenderer:(int64_t)textureID ofKey:(NSNumber *)channel
{
    if([self isCaptureRendererExists:channel])
        return NO;
    
    ZegoTextureRenderer *renderer = [self getRendererFromList:textureID];
    
    if(renderer)
    {
        [self.captureRenderers setObject:renderer forKey:channel];
        [self.rendererList removeObject:renderer];
        return YES;
    }
    
    return NO;
}

- (BOOL)addRemoteRenderer:(int64_t)textureID ofKey:(NSString *)streamID
{
    if([self isRemoterendererExists:streamID])
        return NO;
    
    ZegoTextureRenderer *renderer = [self getRendererFromList:textureID];
    
    if(renderer)
    {
        [self.remoteRenderers setObject:renderer forKey:streamID];
        [self.rendererList removeObject:renderer];
        return YES;
    }
    
    return NO;
}

- (BOOL)removeCaptureRenderer:(NSNumber *)channel
{
    /*ZegoTextureRenderer *renderer = [self.captureRenderers objectForKey:channel];
    if(renderer)
    {
        [self.captureRenderers removeObjectForKey:channel];
    }*/
    
    [self.captureRenderers removeObjectForKey:channel];
    return YES;
}

- (BOOL)removeRemoteRenderer:(NSString *)streamID
{
    [self.remoteRenderers removeObjectForKey:streamID];
    return YES;
}

- (void)removeAllRenderer
{
    [self.captureRenderers removeAllObjects];
    [self.remoteRenderers removeAllObjects];
    [self.rendererList removeAllObjects];
}

- (ZegoTextureRenderer *)getRendererFromList:(int64_t)textureID
{
    ZegoTextureRenderer *renderer = nil;
    for(ZegoTextureRenderer *obj in self.rendererList)
    {
        if(obj.textureID == textureID)
        {
            renderer = obj;
            break;
        }
    }
    
    return renderer;
}

- (void)startRendering
{
    if(self.isRendering)
        return;
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self
                                               selector:@selector(onDisplayLink:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    // 30帧刷新，对于 120hz刷新率的ipad，则为60帧刷新
    _displayLink.frameInterval = 2;
    _displayLink.paused = NO;
    
    self.isRendering = YES;
}

- (void)stopRendering
{
    if(!self.isRendering)
        return;
    
    // TODO: 暂时先限定死只有在所有renderer都不存在时，停止渲染才成功
    if(self.captureRenderers.count == 0 && self.remoteRenderers == 0)
    {
        self.displayLink.paused = YES;
        [_displayLink invalidate];
        
        self.isRendering = NO;
    }
}

#pragma mark Private Methods
- (BOOL)isCaptureRendererExists:(NSNumber *)channel
{
    for(NSNumber *key in self.captureRenderers)
    {
        if([key isEqualToNumber:channel])
            return YES;
    }
    
    return NO;
}

- (BOOL)isRemoterendererExists:(NSString *)streamID
{
    for(NSString *key in self.remoteRenderers)
    {
        if([key isEqualToString:streamID])
            return YES;
    }
    
    return NO;
}

- (void)onDisplayLink:(CADisplayLink*)link
{
    // 本地渲染
    for(NSNumber *key in self.captureRenderers)
    {
        ZegoTextureRenderer *renderer = [self.captureRenderers objectForKey:key];
        if(!renderer || ![renderer isNewFrameAvailable])
            continue;
        
        [renderer notifyDrawNewFrame];
    }
    
    // 拉流渲染
    for(NSString *key in self.remoteRenderers)
    {
        ZegoTextureRenderer *renderer = [self.remoteRenderers objectForKey:key];
        if(!renderer || ![renderer isNewFrameAvailable])
            continue;
        
        [renderer notifyDrawNewFrame];
    }
}

#pragma mark Custom Render Delegate
/// Remote playing stream video frame raw data callback, you can differentiate different streams by streamID
///
/// @param buffer video data of CVPixelBuffer format
/// @param param Video frame param
/// @param flipMode video flip mode
/// @param channel Publishing stream channel.
- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param flipMode:(ZegoVideoFlipMode)flipMode channel:(ZegoPublishChannel)channel
{
    if(!self.isRendering)
    {
        return;
    }
    
    ZegoTextureRenderer *renderer = [self.captureRenderers objectForKey:@(channel)];
    if(renderer)
    {
        [renderer setUseMirrorEffect:flipMode == 1];
        [renderer setSrcFrameBuffer:buffer];
    }
}

/// Remote playing stream video frame CVPixelBuffer data callback, you can differentiate different streams by streamID
///
/// @param buffer video data of CVPixelBuffer format
/// @param param Video frame param
/// @param streamID Stream ID
- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param streamID:(NSString *)streamID
{
    if(!self.isRendering)
    {
        return;
    }
    
    ZegoTextureRenderer *renderer = [self.remoteRenderers objectForKey:streamID];
    if(renderer)
    {
        [renderer setSrcFrameBuffer:buffer];
    }
}

@end
