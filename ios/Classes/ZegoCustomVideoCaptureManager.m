//
//  ZegoCustomVideoCaptureManager.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/9/11.
//

#import "ZegoCustomVideoCaptureManager.h"
#import "ZegoTextureRendererController.h"
#import "ZegoExpressEngineMethodHandler.h"
#import "ZegoLog.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoCustomVideoCaptureClient()

@property (nonatomic, assign)ZegoPublishChannel channel;
@property (nonatomic, assign)ZegoVideoMirrorMode mirrorMode;
@property (nonatomic, strong)ZegoVideoFrameParam *videoParam;

@end

@implementation ZegoCustomVideoCaptureClient

- (void)setVideoMirrorMode:(int)mode {
    _mirrorMode = mode;
    if([ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:_mirrorMode channel:_channel];
    }
}

- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCapturePixelBuffer:buffer timestamp:timestamp];
    // 使用 Texture 方式渲染时，还需要将数据传给 TextureRednerer
    if(![ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] onCapturedVideoFrameCVPixelBuffer:buffer param:self.videoParam flipMode:(_mirrorMode == ZegoVideoMirrorModeOnlyPreviewMirror || _mirrorMode == ZegoVideoMirrorModeBothMirror) channel:_channel];
    }
}

- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureTextureData:textureID size:size timestamp:timestamp];
    // 使用 Texture 方式渲染时，此方法无法直接渲染
    // TODO: 考虑使用 gltexture -> cvpixelbuffer 的转换
}

+ (instancetype)new {
    return nil;
}

- (instancetype)init {
    return nil;
}

- (instancetype)initWithChannel:(ZegoPublishChannel)channel{
    if(self = [super init]) {
        _channel = channel;
        _mirrorMode = ZegoVideoMirrorModeNoMirror;
        _videoParam = [[ZegoVideoFrameParam alloc] init];
    }
    
    return self;
}

- (void) dealloc {
    ZGLog(@"[CustomVideoCaptureClient] dealloc");
}

@end

@interface ZegoCustomVideoCaptureManager()

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoCustomVideoCaptureClient *>* clients;
@property (nonatomic, strong) NSMapTable<NSNumber *, id<ZegoCustomVideoCaptureDelegate>>* delegates;

@end

@implementation ZegoCustomVideoCaptureManager

- (instancetype)init {
    if(self = [super init]) {
        //_fps = 15;
        //_timers = [NSMutableDictionary dictionary];
        _clients = [NSMutableDictionary dictionary];
        _delegates = [NSMapTable strongToWeakObjectsMapTable];
    }
    return self;
}

+ (instancetype)sharedInstance {
    static ZegoCustomVideoCaptureManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoCustomVideoCaptureManager alloc] init];
    });
    return instance;
}

- (void)setCustomVideoCaptureDelegate:(id<ZegoCustomVideoCaptureDelegate>)delegate channel:(int)channel {
    [self.delegates setObject:delegate forKey:@(channel)];
}

# pragma mark ZegoCustomVideoCaptureHandler
- (void)onStart:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStart");
    
    ZegoCustomVideoCaptureClient *client = [[ZegoCustomVideoCaptureClient alloc] initWithChannel:channel];
    id<ZegoCustomVideoCaptureDelegate> delegate = [self.delegates objectForKey:@(channel)];
    [self.clients setObject:client forKey:@(channel)];
    
    if([delegate respondsToSelector:@selector(onStart:)]) {
        [delegate onStart:client];
    }
}


- (void)onStop:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStop");
    
    [self.clients removeObjectForKey:@(channel)];
    id<ZegoCustomVideoCaptureDelegate> delegate = [self.delegates objectForKey:@(channel)];
    if([delegate respondsToSelector:@selector(onStop)]) {
        [delegate onStop];
    }
}

@end
