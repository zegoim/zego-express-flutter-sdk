//
//  ZegoAudioMixingDataManager.h
//  zego_express_engine
//
//  Created by zego on 2025/5/12.
//

#import <Foundation/Foundation.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterAudioMixingHandler <NSObject>

@required

- (ZGFlutterAudioMixingData *)onAudioMixingCopyData:(unsigned int)expectedDataLength;

@end

@interface ZegoAudioMixingDataManager : NSObject

+ (instancetype)sharedInstance;

- (void)setAudioMixingHandler:(id<ZegoFlutterAudioMixingHandler>)handler;

@end

NS_ASSUME_NONNULL_END
