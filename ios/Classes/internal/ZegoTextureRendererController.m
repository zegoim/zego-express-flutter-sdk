//
//  ZegoTextureRendererController.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRendererController.h"
#import "ZegoLog.h"

@interface ZegoTextureRendererController () <FlutterStreamHandler>

// BufferRenderers for caching, the renderer created will not be immediately added to the renderers dictionary, but the developer will need to explicitly bind the relationship
@property (strong) NSMutableDictionary<NSNumber *, ZegoTextureRenderer *> *renderers;

@property (strong) NSMutableDictionary<NSNumber *, NSNumber *> *capturedTextureIdMap;
@property (strong) NSMutableDictionary<NSString *, NSNumber *> *remoteTextureIdMap;
@property (strong) NSMutableDictionary<NSNumber *, NSNumber *> *mediaPlayerTextureIdMap;

@property (nonatomic, assign) BOOL isInited;

@property (nonatomic, strong) FlutterEventSink eventSink;

@property (nonatomic, assign) BOOL isAppEnterBackground;

@property (nonatomic, weak) id<ZegoFlutterCustomVideoRenderHandler> renderHandler;

@property (nonatomic, weak) id<ZegoFlutterMediaPlayerVideoHandler> mediaPlayerVideoHandler;

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
    self.isAppEnterBackground = NO;
    if (self) {
        _renderers = [NSMutableDictionary dictionary];
        _capturedTextureIdMap = [NSMutableDictionary dictionary];
        _remoteTextureIdMap = [NSMutableDictionary dictionary];
        _mediaPlayerTextureIdMap = [NSMutableDictionary dictionary];
        _renderHandler = nil;
        
#if TARGET_OS_IPHONE
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidEnterBackground:) name:UIApplicationDidEnterBackgroundNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillEnterForeground:) name:UIApplicationWillEnterForegroundNotification object:nil];
#endif
    }
    return self;
}

-(void)applicationDidEnterBackground:(NSNotification *)notification {
    @synchronized (self) {
        self.isAppEnterBackground = YES;
    }
}

-(void)applicationWillEnterForeground:(NSNotification *)notification {
    @synchronized (self) {
        self.isAppEnterBackground = NO;
    }
}

#pragma mark - Dart Texture Render Utils Operation

/// The following methods are only triggered by the dart `zego_texture_render_utils`

- (int64_t)createTextureRenderer:(id<FlutterTextureRegistry>)registry
                       viewWidth:(int)width
                      viewHeight:(int)height {

    ZegoTextureRenderer *renderer = [[ZegoTextureRenderer alloc] initWithTextureRegistry:registry size:CGSizeMake(width, height)];

    ZGLog(@"[createTextureRenderer] textureID:%ld, renderer:%p", (long)renderer.textureID,
          renderer);

    @synchronized (self) {
        [self.renderers setObject:renderer forKey:@(renderer.textureID)];
    }

    [self logCurrentRenderers];

    return renderer.textureID;
}

- (BOOL)destroyTextureRenderer:(int64_t)textureID {

    ZegoTextureRenderer *renderer = [self.renderers objectForKey:@(textureID)];

    if (!renderer) {
        ZGLog(@"[destroyTextureRenderer] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[destroyTextureRenderer] textureID:%ld, renderer:%p", (long)textureID, renderer);

    @synchronized (self) {
        [self.renderers removeObjectForKey:@(renderer.textureID)];
    }

    // Release renderer
    [renderer destroy];

    [self logCurrentRenderers];

    return YES;
}

- (void)logCurrentRenderers {
    NSMutableString *desc = [[NSMutableString alloc] init];
    for (NSNumber *i in self.renderers) {
        ZegoTextureRenderer *eachRenderer = self.renderers[i];
        [desc appendFormat:@"[ID:%d|Rnd:%p] ", i.intValue, eachRenderer];
    }
    ZGLog(@"[ZegoTextureRendererController] currentRenderers: %@", desc);
}

#pragma mark - Dart Express Engine API Operation

/// The following methods are only triggered by the dart `zego_express_api`

- (void)initControllerWithMessenger:(NSObject<FlutterBinaryMessenger> *)messenger {

    if (!self.isInited) {
        FlutterEventChannel *eventChannel = [FlutterEventChannel eventChannelWithName:@"plugins.zego.im/zego_texture_renderer_controller_event_handler" binaryMessenger:messenger];
        [eventChannel setStreamHandler:self];
        
        // Enable custom video render
        ZegoCustomVideoRenderConfig *renderConfig = [[ZegoCustomVideoRenderConfig alloc] init];
        renderConfig.frameFormatSeries = ZegoVideoFrameFormatSeriesRGB;
        renderConfig.bufferType = ZegoVideoBufferTypeCVPixelBuffer;
        [[ZegoExpressEngine sharedEngine] enableCustomVideoRender:YES config:renderConfig];

        // Set up custom video render handler
        [[ZegoExpressEngine sharedEngine] setCustomVideoRenderHandler:self];

        // To avoid call enableCustomVideoRender many times because of flutter hotreload
        self.isInited = YES;
    }
}

- (void)uninitController {
    @synchronized (self) {
        [self.renderers removeAllObjects];
        [self.capturedTextureIdMap removeAllObjects];
        [self.remoteTextureIdMap removeAllObjects];
    }
    self.isInited = NO;
}

- (BOOL)bindCapturedChannel:(NSNumber *)channel withTexture:(int64_t)textureID {

    ZegoTextureRenderer *renderer = [self.renderers objectForKey:@(textureID)];

    if (!renderer) {
        ZGLog(@"[bindCapturedChannel] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[bindCapturedChannel] textureID:%ld, renderer:%p, channel:%d",
          (long)textureID, renderer, channel.intValue);

    @synchronized (self) {
        [self.capturedTextureIdMap setObject:@(textureID) forKey:channel];
    }
    
    [self logCurrentRenderers];

    return YES;
}

- (void)unbindCapturedChannel:(NSNumber *)channel {
    ZGLog(@"[unbindCapturedChannel] channel:%d", channel.intValue);

    @synchronized (self) {
        [self.capturedTextureIdMap removeObjectForKey:channel];
    }

    [self logCurrentRenderers];
}

- (BOOL)bindRemoteStreamId:(NSString *)streamId withTexture:(int64_t)textureID {

    ZegoTextureRenderer *renderer = [self.renderers objectForKey:@(textureID)];

    if (!renderer) {
        ZGLog(@"[bindRemoteStreamId] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[bindRemoteStreamId] textureID:%ld, renderer:%p, streamId:%@",
          (long)textureID, renderer, streamId);

    @synchronized (self) {
        [self.remoteTextureIdMap setObject:@(textureID) forKey:streamId];
    }

    [self logCurrentRenderers];

    return YES;
}

- (void)unbindRemoteStreamId:(NSString *)streamId {
    ZGLog(@"[unbindRemoteStreamId] streamId:%@", streamId);

    @synchronized (self) {
        [self.remoteTextureIdMap removeObjectForKey:streamId];
    }

    [self logCurrentRenderers];
}

- (BOOL)bindMediaPlayerIndex:(NSNumber *)index withTexture:(int64_t)textureID {
    ZegoTextureRenderer *renderer = [self.renderers objectForKey:@(textureID)];

    if (!renderer) {
        ZGLog(@"[bindMediaPlayerIndex] renderer for textureID:%ld not exists", (long)textureID);
        [self logCurrentRenderers];
        return NO;
    }

    ZGLog(@"[bindMediaPlayerIndex] textureID:%ld, renderer:%p, index:%@",
          (long)textureID, renderer, index);

    @synchronized (self) {
        [self.mediaPlayerTextureIdMap setObject:@(textureID) forKey:index];
    }

    [self logCurrentRenderers];

    return YES;
}

- (void)unbindMediaPlayerIndex:(NSNumber *)index {
    ZGLog(@"[unbindMediaPlayerIndex] index:%@", index);

    @synchronized (self) {
        [self.mediaPlayerTextureIdMap removeObjectForKey:index];
    }

    [self logCurrentRenderers];
}

#pragma mark - Private Methods

- (void)updateRenderer:(ZegoTextureRenderer *)renderer
            withBuffer:(CVPixelBufferRef)buffer
                 param:(ZegoVideoFrameParam *)param
              flipMode:(ZegoVideoFlipMode)flipMode {
    
    if ((int)param.size.width != (int)renderer.imageSize.width ||
        (int)param.size.height != (int)renderer.imageSize.height ||
        param.rotation != renderer.rotation ||
        flipMode != renderer.flipMode) {
        NSDictionary *map = @{
            @"type": @"update",
            @"textureID": @(renderer.textureID),
            @"width": @(param.size.width),
            @"height": @(param.size.height),
            @"isMirror": @(flipMode == ZegoVideoFlipModeX ? 1 : 0),
            // TODO: Rotation & FlipModeY
        };
        self.eventSink(map);
    }
    
    renderer.imageSize = param.size;
    renderer.rotation = param.rotation;
    renderer.flipMode = flipMode;
    
    @synchronized (self) {
        if (!self.isAppEnterBackground) {
            [renderer updateSrcFrameBuffer:buffer];
        }
    }
}

#pragma mark - ZegoCustomVideoRenderHandler

- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                    param:(ZegoVideoFrameParam *)param
                                 flipMode:(ZegoVideoFlipMode)flipMode
                                  channel:(ZegoPublishChannel)channel {
    if ([self.renderHandler respondsToSelector:@selector(onCapturedVideoFrameCVPixelBuffer:param:flipMode:channel:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.renderHandler onCapturedVideoFrameCVPixelBuffer:buffer param:videoFrameParam flipMode:(ZGFlutterVideoFlipMode)flipMode channel:(ZGFlutterPublishChannel)channel];
        free(videoFrameParam.strides);
    }
    
    NSNumber *textureID = nil;
    ZegoTextureRenderer *renderer = nil;

    @synchronized (self) {
        textureID = [self.capturedTextureIdMap objectForKey:@(channel)];
        if (!textureID) {
            return;
        }
        renderer = [self.renderers objectForKey:textureID];
        if (!renderer) {
            return;
        }
    }
    
    [self updateRenderer:renderer withBuffer:buffer param:param flipMode:flipMode];
}

- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                  param:(ZegoVideoFrameParam *)param
                               streamID:(NSString *)streamID {
    if ([self.renderHandler respondsToSelector:@selector(onRemoteVideoFrameCVPixelBuffer:param:streamID:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.renderHandler onRemoteVideoFrameCVPixelBuffer:buffer param:videoFrameParam streamID:streamID];
        free(videoFrameParam.strides);
    }
    
    NSNumber *textureID = nil;
    ZegoTextureRenderer *renderer = nil;

    @synchronized (self) {
        textureID = [self.remoteTextureIdMap objectForKey:streamID];
        if (!textureID) {
            return;
        }
        renderer = [self.renderers objectForKey:textureID];
        if (!renderer) {
            return;
        }
    }
    
    [self updateRenderer:renderer withBuffer:buffer param:param flipMode:ZegoVideoFlipModeNone];
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer videoFramePixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param extraInfo:(NSDictionary *)extraInfo {
    NSNumber *textureID = nil;
    ZegoTextureRenderer *renderer = nil;

    if ([self.mediaPlayerVideoHandler respondsToSelector:@selector(mediaPlayer:videoFramePixelBuffer:param:extraInfo:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.mediaPlayerVideoHandler mediaPlayer:[mediaPlayer.index intValue] videoFramePixelBuffer:buffer param:videoFrameParam extraInfo:extraInfo];
        free(videoFrameParam.strides);
    }
    
    
    @synchronized (self) {
        textureID = [self.mediaPlayerTextureIdMap objectForKey:mediaPlayer.index];
        if (!textureID) {
            return;
        }
        renderer = [self.renderers objectForKey:textureID];
        if (!renderer) {
            return;
        }
    }
    
    [self updateRenderer:renderer withBuffer:buffer param:param flipMode:ZegoVideoFlipModeNone];
}

- (void)setCustomVideoRenderHandler: (id<ZegoFlutterCustomVideoRenderHandler> _Nullable) handler {
    self.renderHandler = handler;
}

-(void)setMediaPlayerVideoHandle: (id<ZegoFlutterMediaPlayerVideoHandler> _Nullable) handler {
    self.mediaPlayerVideoHandler = handler;
}

#pragma mark - FlutterStreamHandler

- (FlutterError * _Nullable)onListenWithArguments:(id _Nullable)arguments eventSink:(nonnull FlutterEventSink)events {
    self.eventSink = events;
    ZGLog(@"[ZegoTextureRendererController] [onListen] set eventSink: %p", _eventSink);
    return nil;
}

- (FlutterError * _Nullable)onCancelWithArguments:(id _Nullable)arguments {
    ZGLog(@"[ZegoTextureRendererController] [onCancel] set eventSink: %p to nil", _eventSink);
    self.eventSink = nil;
    return nil;
}

@end
