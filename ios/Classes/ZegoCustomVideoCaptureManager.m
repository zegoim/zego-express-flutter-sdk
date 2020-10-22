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

@interface ZegoCustomVideoCaptureManager()<ZegoCustomVideoCaptureHandler>

@property (nonatomic, weak) id<ZegoFlutterCustomVideoCaptureHandler> handler;

@property (nonatomic, assign)ZegoVideoMirrorMode mirrorMode;
@property (nonatomic, strong)ZegoVideoFrameParam *videoParam;
@end

@implementation ZegoCustomVideoCaptureManager

+ (instancetype)sharedInstance {
    static ZegoCustomVideoCaptureManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoCustomVideoCaptureManager alloc] init];
    });
    return instance;
}

- (void)setCustomVideoCaptureHandler:(id<ZegoFlutterCustomVideoCaptureHandler>)handler {
    
    self.handler = handler;
}

- (void)setVideoMirrorMode:(int)mode channel:(int)channel{
    _mirrorMode = mode;
    if([ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:_mirrorMode channel:channel];
    }
}

- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(int)channel{
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCapturePixelBuffer:buffer timestamp:timestamp];
    // 使用 Texture 方式渲染时，还需要将数据传给 TextureRednerer
    if(![ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] onCapturedVideoFrameCVPixelBuffer:buffer param:self.videoParam flipMode:(_mirrorMode == ZegoVideoMirrorModeOnlyPreviewMirror || _mirrorMode == ZegoVideoMirrorModeBothMirror) channel:channel];
    }
}

- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp channel:(int)channel {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureTextureData:textureID size:size timestamp:timestamp channel:(ZegoPublishChannel)channel];
    // 使用 Texture 方式渲染时，此方法无法直接渲染
    // TODO: 考虑使用 gltexture -> cvpixelbuffer 的转换
}

# pragma mark ZegoCustomVideoCaptureHandler
- (void)onStart:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStart");

    if([self.handler respondsToSelector:@selector(onStart:)]) {
        [self.handler onStart:(int)channel];
    }
}


- (void)onStop:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStop");
    
    if([self.handler respondsToSelector:@selector(onStop:)]) {
        [self.handler onStop:(int)channel];
    }
}

@end
