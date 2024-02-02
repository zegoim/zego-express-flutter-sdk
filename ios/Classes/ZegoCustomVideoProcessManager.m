//
//  ZegoCustomVideoProcessManager.m
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#import "ZegoCustomVideoProcessManager.h"
#import "ZegoLog.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoCustomVideoProcessManager()<ZegoCustomVideoProcessHandler>

@property (nonatomic, weak) id<ZegoFlutterCustomVideoProcessHandler> handler;

@end

@implementation ZegoCustomVideoProcessManager

+ (instancetype)sharedInstance {
    static ZegoCustomVideoProcessManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoCustomVideoProcessManager alloc] init];
    });
    return instance;
}

-(void)setCustomVideoProcessHandler:(id<ZegoFlutterCustomVideoProcessHandler>)handler {
    
    self.handler = handler;
}


- (void)sendProcessedCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoProcessedCVPixelBuffer:buffer timestamp:timestamp channel:(ZegoPublishChannel)channel];
}

- (void)sendCustomVideoProcessedCVPixelBuffer:(CVPixelBufferRef)buffer
                                    timestamp:(CMTime)timestamp
                                        usage:(ZGFlutterProcessedDataUsageType)usage
                                      channel:(ZGFlutterPublishChannel)channel {
    [[ZegoExpressEngine sharedEngine] sendCustomVideoProcessedCVPixelBuffer:buffer 
                                                                  timestamp:timestamp 
                                                                      usage:(ZegoProcessedDataUsageType)usage 
                                                                    channel:(ZegoPublishChannel)channel];
}

# pragma mark ZegoCustomVideoProcessHandler
- (void)onStart:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoProcess] onStart");

    if([self.handler respondsToSelector:@selector(onStart:)]) {
        [self.handler onStart:(ZGFlutterPublishChannel)channel];
    }
}


- (void)onStop:(ZegoPublishChannel)channel {
    ZGLog(@"[CustomVideoProcess] onStop");
    
    if([self.handler respondsToSelector:@selector(onStop:)]) {
        [self.handler onStop:(ZGFlutterPublishChannel)channel];
    }
}

- (void)onCapturedUnprocessedCVPixelBuffer:(CVPixelBufferRef)buffer
                                 timestamp:(CMTime)timestamp
                                   channel:(ZegoPublishChannel)channel {
    if([self.handler respondsToSelector:@selector(onCapturedUnprocessedCVPixelBuffer:timestamp:channel:)]) {
        [self.handler onCapturedUnprocessedCVPixelBuffer:buffer timestamp:timestamp channel:(ZGFlutterPublishChannel)channel];
    }
}

@end
