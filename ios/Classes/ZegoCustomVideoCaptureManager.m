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

- (void)setVideoMirrorMode:(int)mode channel:(ZGFlutterPublishChannel)channel{
    _mirrorMode = mode;
    if([ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:_mirrorMode channel:(ZegoPublishChannel)channel];
    }
}

- (void)setFillMode:(ZGFlutterViewMode)mode channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureFillMode:(ZegoViewMode)mode channel:(ZegoPublishChannel)channel];
}

- (void)setFlipMode:(ZGFlutterVideoFlipMode)mode channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureFlipMode:(ZegoVideoFlipMode)mode channel:(ZegoPublishChannel)channel];
}

- (void)setRotation:(int)rotation channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureRotation:rotation channel:(ZegoPublishChannel)channel];
}

-(ZegoVideoFrameFormat)osTypeToZegoVideoFrameFormat:(OSType) type {
    ZegoVideoFrameFormat format = ZegoVideoFrameFormatUnknown;
    switch(type) {
        case kCVPixelFormatType_420YpCbCr8Planar:
        case kCVPixelFormatType_420YpCbCr8PlanarFullRange:
            format = ZegoVideoFrameFormatI420;
            break;
        case kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange:
        case kCVPixelFormatType_420YpCbCr8BiPlanarFullRange:
            format = ZegoVideoFrameFormatNV12;
            break;
        case kCVPixelFormatType_32BGRA:
            format = ZegoVideoFrameFormatBGRA32;
            break;
        case kCVPixelFormatType_32ARGB:
            format = ZegoVideoFrameFormatARGB32;
            break;
    }
    return format;
}

- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel{
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCapturePixelBuffer:buffer timestamp:timestamp channel:(ZegoPublishChannel)channel];
    
    // 使用 Texture 方式渲染时，还需要将数据传给 TextureRednerer
    if(![ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        if (self.videoParam == nil) {
            self.videoParam = [[ZegoVideoFrameParam alloc] init];
        }
        self.videoParam.size = CGSizeMake(CVPixelBufferGetWidth(buffer), CVPixelBufferGetHeight(buffer));
        self.videoParam.format = [self osTypeToZegoVideoFrameFormat: CVPixelBufferGetPixelFormatType(buffer)];
        int tempStrides[4] = {0};
        self.videoParam.strides = tempStrides;
        if (self.videoParam.format == ZegoVideoFrameFormatI420 || self.videoParam.format == ZegoVideoFrameFormatNV12) {
            for (int i = 0; i < CVPixelBufferGetPlaneCount(buffer); i++) {
                self.videoParam.strides[i] = (int)CVPixelBufferGetWidthOfPlane(buffer, i);
            }
        } else {
            self.videoParam.strides[0] = (int32_t)CVPixelBufferGetBytesPerRow(buffer);
        }
        
        [[ZegoTextureRendererController sharedInstance] onCapturedVideoFrameCVPixelBuffer:buffer param:self.videoParam flipMode:(_mirrorMode == ZegoVideoMirrorModeOnlyPreviewMirror || _mirrorMode == ZegoVideoMirrorModeBothMirror) channel:(ZegoPublishChannel)channel];
    }
}

- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureTextureData:textureID size:size timestamp:timestamp channel:(ZegoPublishChannel)channel];
    
    // 使用 Texture 方式渲染时，此方法无法直接渲染
    // TODO: 考虑使用 gltexture -> cvpixelbuffer 的转换
}

- (void)sendEncodedData:(NSData *)data
                                   params:(ZGFlutterVideoEncodedFrameParam *)params
                                timestamp:(CMTime)timestamp
                                  channel:(ZGFlutterPublishChannel)channel {
    ZegoVideoEncodedFrameParam *frameParam = [[ZegoVideoEncodedFrameParam alloc] init];
    frameParam.size = params.size;
    frameParam.format = (ZegoVideoEncodedFrameFormat)params.format;
    frameParam.rotation = params.rotation;
    frameParam.SEIData = params.SEIData;
    frameParam.isKeyFrame = params.isKeyFrame;
    [[ZegoExpressEngine sharedEngine] sendCustomVideoCaptureEncodedData:data params:frameParam timestamp:timestamp channel:(ZegoPublishChannel)channel];
}

# pragma mark ZegoCustomVideoCaptureHandler
- (void)onStart:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStart");

    if([self.handler respondsToSelector:@selector(onStart:)]) {
        [self.handler onStart:(ZGFlutterPublishChannel)channel];
    }
}


- (void)onStop:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onStop");
    
    if([self.handler respondsToSelector:@selector(onStop:)]) {
        [self.handler onStop:(ZGFlutterPublishChannel)channel];
    }
}

- (void)onEncodedDataTrafficControl:(ZegoTrafficControlInfo *)trafficControlInfo
                            channel:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoCapture] onEncodedDataTrafficControl");
    if([self.handler respondsToSelector:@selector(onEncodedDataTrafficControl:channel:)]) {
        ZGFlutterTrafficControlInfo *info = [[ZGFlutterTrafficControlInfo alloc] init];
        info.fps = trafficControlInfo.fps;
        info.bitrate = trafficControlInfo.bitrate;
        info.resolution = trafficControlInfo.resolution;
        [self.handler onEncodedDataTrafficControl:info channel:(ZGFlutterPublishChannel)channel];
    }
}

@end
