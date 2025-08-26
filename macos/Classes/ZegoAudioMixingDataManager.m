//
//  ZegoAudioMixingDataManager.m
//  zego_express_engine
//
//  Created by zego on 2025/5/12.
//

#import "ZegoAudioMixingDataManager.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoAudioMixingDataManager () <ZegoAudioMixingHandler>

@property (nonatomic, strong) id<ZegoFlutterAudioMixingHandler> handler;

@end

@implementation ZegoAudioMixingDataManager

+ (instancetype)sharedInstance {
    static ZegoAudioMixingDataManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoAudioMixingDataManager alloc] init];
    });
    return instance;
}

- (void)setAudioMixingHandler:(id<ZegoFlutterAudioMixingHandler>)handler {
    self.handler = handler;
}

- (ZegoAudioMixingData *)onAudioMixingCopyData:(unsigned int)expectedDataLength {
    if (!self.handler) {
        return nil;
    }
    ZGFlutterAudioMixingData *data = [self.handler onAudioMixingCopyData:expectedDataLength];
    if (!data) {
        return nil;
    }
    ZegoAudioMixingData *result = [[ZegoAudioMixingData alloc] init];
    result.audioData = data.audioData;
    result.SEIData = data.SEIData;
    result.param = [[ZegoAudioFrameParam alloc] init];
    result.param.sampleRate = (ZegoAudioSampleRate)data.param.sampleRate;
    result.param.channel = (ZegoAudioChannel)data.param.channel;
    return result;
}

@end
