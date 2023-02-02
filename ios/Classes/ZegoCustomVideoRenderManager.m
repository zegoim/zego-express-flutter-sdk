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
        VideoFrameParam *videoFrameParam = [[VideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (VideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onCapturedVideoFrameRawData:data dataLength:dataLength param:videoFrameParam flipMode:(VideoFlipMode)flipMode channel:(PublishChannel)channel];
        
        free(videoFrameParam.strides);
    }
}

- (void)onRemoteVideoFrameRawData:(unsigned char *_Nonnull *_Nonnull)data
                       dataLength:(unsigned int *)dataLength
                            param:(ZegoVideoFrameParam *)param
                         streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameRawData:dataLength:param:streamID:)]) {
        VideoFrameParam *videoFrameParam = [[VideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (VideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onRemoteVideoFrameRawData:data dataLength:dataLength param:videoFrameParam streamID:streamID];
        
        free(videoFrameParam.strides);
    }
}

- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                    param:(ZegoVideoFrameParam *)param
                                 flipMode:(ZegoVideoFlipMode)flipMode
                                  channel:(ZegoPublishChannel)channel {
    if([self.handler respondsToSelector:@selector(onCapturedVideoFrameCVPixelBuffer:param:flipMode:channel:)]) {
        VideoFrameParam *videoFrameParam = [[VideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (VideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:videoFrameParam flipMode:(VideoFlipMode)flipMode channel:(PublishChannel)channel];
        
        free(videoFrameParam.strides);
    }
}

- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                  param:(ZegoVideoFrameParam *)param
                               streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameCVPixelBuffer:param:streamID:)]) {
        VideoFrameParam *videoFrameParam = [[VideoFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (VideoFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.strides = malloc(sizeof(int)*4);
        for (int i = 0; i < 4; i++) {
            videoFrameParam.strides[i] = param.strides[i];
        }
        
        [self.handler onRemoteVideoFrameCVPixelBuffer:buffer param:videoFrameParam streamID:streamID];
        
        free(videoFrameParam.strides);
    }
}

- (void)onRemoteVideoFrameEncodedData:(unsigned char *_Nonnull)data
                           dataLength:(unsigned int)dataLength
                                param:(ZegoVideoEncodedFrameParam *)param
             referenceTimeMillisecond:(unsigned long long)referenceTimeMillisecond
                             streamID:(NSString *)streamID {
    if([self.handler respondsToSelector:@selector(onRemoteVideoFrameEncodedData:dataLength:param:referenceTimeMillisecond:streamID:)]) {
        VideoEncodedFrameParam *videoFrameParam = [[VideoEncodedFrameParam alloc] init];
        videoFrameParam.size = param.size;
        videoFrameParam.format = (VideoEncodedFrameFormat)param.format;
        videoFrameParam.rotation = param.rotation;
        videoFrameParam.isKeyFrame = param.isKeyFrame;
        videoFrameParam.SEIData = param.SEIData;
        
        [self.handler onRemoteVideoFrameEncodedData:data dataLength:dataLength param:videoFrameParam referenceTimeMillisecond:referenceTimeMillisecond streamID:streamID];
        
    }
}

@end
