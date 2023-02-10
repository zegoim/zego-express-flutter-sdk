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

- (void)setVideoMirrorMode:(int)mode channel:(PublishChannel)channel{
    _mirrorMode = mode;
    if([ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        if ([ZegoExpressEngine sharedEngine] != nil) {
            [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:_mirrorMode channel:(ZegoPublishChannel)channel];
        } else {
            
        }
    }
}

- (void)setFillMode:(ViewMode)mode channel:(PublishChannel)channel {
    if ([ZegoExpressEngine sharedEngine] != nil) {
        [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureFillMode:(ZegoViewMode)mode channel:(ZegoPublishChannel)channel];
    } else {
        
    }
}

- (void)setFlipMode:(VideoFlipMode)mode channel:(PublishChannel)channel {
    if ([ZegoExpressEngine sharedEngine] != nil) {
        [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureFlipMode:(ZegoVideoFlipMode)mode channel:(ZegoPublishChannel)channel];
    } else {
        
    }
}

- (void)setRotation:(int)rotation channel:(PublishChannel)channel {
    if ([ZegoExpressEngine sharedEngine] != nil) {
        [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureRotation:rotation channel:(ZegoPublishChannel)channel];
    } else {
        
    }
}

- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(PublishChannel)channel{
    if ([ZegoExpressEngine sharedEngine] != nil) {
        [[ZegoExpressEngine sharedEngine] sendCustomVideoCapturePixelBuffer:buffer timestamp:timestamp channel:(ZegoPublishChannel)channel];
    } else {
        
    }
    
    // 使用 Texture 方式渲染时，还需要将数据传给 TextureRednerer
    if(![ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        if (self.videoParam == nil) {
            self.videoParam = [[ZegoVideoFrameParam alloc] init];
        }
        self.videoParam.size = CGSizeMake(CVPixelBufferGetWidth(buffer), CVPixelBufferGetHeight(buffer));
        
        
        [[ZegoTextureRendererController sharedInstance] onCapturedVideoFrameCVPixelBuffer:buffer param:self.videoParam flipMode:(_mirrorMode == ZegoVideoMirrorModeOnlyPreviewMirror || _mirrorMode == ZegoVideoMirrorModeBothMirror) channel:(ZegoPublishChannel)channel];
    }
}

- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp channel:(PublishChannel)channel {
    if ([ZegoExpressEngine sharedEngine] != nil) {
        [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureTextureData:textureID size:size timestamp:timestamp channel:(ZegoPublishChannel)channel];
    } else {
        
    }
    
    // 使用 Texture 方式渲染时，此方法无法直接渲染
    // TODO: 考虑使用 gltexture -> cvpixelbuffer 的转换
}

- (void)sendEncodedData:(NSData *)data
                                   params:(VideoEncodedFrameParam *)params
                                timestamp:(CMTime)timestamp
                                  channel:(PublishChannel)channel {
    if ([ZegoExpressEngine sharedEngine] != nil) {
        ZegoVideoEncodedFrameParam *frameParam = [[ZegoVideoEncodedFrameParam alloc] init];
        frameParam.size = params.size;
        frameParam.format = (ZegoVideoEncodedFrameFormat)params.format;
        frameParam.rotation = params.rotation;
        frameParam.SEIData = params.SEIData;
        frameParam.isKeyFrame = params.isKeyFrame;
        [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureEncodedData:data params:frameParam timestamp:timestamp channel:(ZegoPublishChannel)channel];
    } else {
        
    }
}

# pragma mark ZegoCustomVideoCaptureHandler
- (void)onStart:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStart");

    if([self.handler respondsToSelector:@selector(onStart:)]) {
        [self.handler onStart:(PublishChannel)channel];
    }
}


- (void)onStop:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStop");
    
    if([self.handler respondsToSelector:@selector(onStop:)]) {
        [self.handler onStop:(PublishChannel)channel];
    }
}

- (void)onEncodedDataTrafficControl:(ZegoTrafficControlInfo *)trafficControlInfo
                            channel:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onEncodedDataTrafficControl");
    if([self.handler respondsToSelector:@selector(onEncodedDataTrafficControl:channel:)]) {
        TrafficControlInfo *info = [[TrafficControlInfo alloc] init];
        info.fps = trafficControlInfo.fps;
        info.bitrate = trafficControlInfo.bitrate;
        info.resolution = trafficControlInfo.resolution;
        [self.handler onEncodedDataTrafficControl:info channel:(PublishChannel)channel];
    }
}

@end
