//
//  ZegoCustomVideoRenderManager.m
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#import "ZegoCustomVideoRenderManager.h"
#import "ZegoLog.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoCustomVideoRenderManager()<ZegoCustomVideoRenderHandler>

@property (nonatomic, weak) id<ZegoFlutterCustomVideoRenderHandler> handler;

@end

@implementation ZegoCustomVideoRenderManager

+ (instancetype)sharedInstance {
    static ZegoCustomVideoRenderManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoCustomVideoRenderManager alloc] init];
    });
    return instance;
}

- (void)setCustomVideoRenderHandler:(id<ZegoFlutterCustomVideoRenderHandler>)handler {
    
    self.handler = handler;
}

-(id<ZegoFlutterCustomVideoRenderHandler>_Nullable)getCustomVideoRenderHandler {
    return self.handler;
}

# pragma mark ZegoCustomVideoRenderHandler
- (void)onCapturedVideoFrameRawData:(unsigned char *_Nonnull *_Nonnull)data
                         dataLength:(unsigned int *)dataLength
                              param:(ZegoVideoFrameParam *)param
                           flipMode:(ZegoVideoFlipMode)flipMode
                            channel:(ZegoPublishChannel)channel {
    if([self.handler respondsToSelector:@selector(onCapturedVideoFrameRawData:dataLength:param:flipMode:channel:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        int tempStrides[4] = {0};
        videoFrameParam.strides = tempStrides;
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onCapturedVideoFrameRawData:data dataLength:dataLength param:videoFrameParam flipMode:(ZGFlutterVideoFlipMode)flipMode channel:(ZGFlutterPublishChannel)channel];
    }
}

- (void)onRemoteVideoFrameRawData:(unsigned char *_Nonnull *_Nonnull)data
                       dataLength:(unsigned int *)dataLength
                            param:(ZegoVideoFrameParam *)param
                         streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameRawData:dataLength:param:streamID:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        int tempStrides[4] = {0};
        videoFrameParam.strides = tempStrides;
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onRemoteVideoFrameRawData:data dataLength:dataLength param:videoFrameParam streamID:streamID];
    }
}

- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                    param:(ZegoVideoFrameParam *)param
                                 flipMode:(ZegoVideoFlipMode)flipMode
                                  channel:(ZegoPublishChannel)channel {
    if([self.handler respondsToSelector:@selector(onCapturedVideoFrameCVPixelBuffer:param:flipMode:channel:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        int tempStrides[4] = {0};
        videoFrameParam.strides = tempStrides;
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:videoFrameParam flipMode:(ZGFlutterVideoFlipMode)flipMode channel:(ZGFlutterPublishChannel)channel];
    }
}

- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                  param:(ZegoVideoFrameParam *)param
                               streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameCVPixelBuffer:param:streamID:)]) {
        ZGFlutterVideoFrameParam *videoFrameParam = [[ZGFlutterVideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        int tempStrides[4] = {0};
        videoFrameParam.strides = tempStrides;
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onRemoteVideoFrameCVPixelBuffer:buffer param:videoFrameParam streamID:streamID];
    }
}

- (void)onRemoteVideoFrameEncodedData:(unsigned char *_Nonnull)data
                           dataLength:(unsigned int)dataLength
                                param:(ZegoVideoEncodedFrameParam *)param
             referenceTimeMillisecond:(unsigned long long)referenceTimeMillisecond
                             streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameEncodedData:dataLength:param:referenceTimeMillisecond:streamID:)]) {
        ZGFlutterVideoEncodedFrameParam *videoFrameParam = [[ZGFlutterVideoEncodedFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (ZGFlutterVideoEncodedFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.isKeyFrame = param.isKeyFrame;
        videoFrameParam.SEIData = param.SEIData;
        
        [self.handler onRemoteVideoFrameEncodedData:data dataLength:dataLength param:videoFrameParam referenceTimeMillisecond:referenceTimeMillisecond streamID:streamID];
        
    }
}

@end
