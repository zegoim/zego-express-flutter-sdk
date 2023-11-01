//
//  ZegoExpressEngineEventHandler.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/4/9.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE
#import <Flutter/Flutter.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterMacOS.h>
#endif

#import <ZegoExpressEngine/ZegoExpressEngine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngineEventHandler : NSObject<ZegoEventHandler, ZegoMediaPlayerEventHandler, ZegoAudioEffectPlayerEventHandler, ZegoMediaDataPublisherEventHandler, ZegoDataRecordEventHandler, ZegoAudioDataHandler, ZegoRangeAudioEventHandler, ZegoCustomAudioProcessHandler, ZegoApiCalledEventHandler, ZegoRealTimeSequentialDataEventHandler, ZegoCopyrightedMusicEventHandler
#if TARGET_OS_OSX
    ,ZegoScreenCaptureSourceEventHandler
#endif
    ,ZegoAIVoiceChangerEventHandler
>

+ (instancetype)sharedInstance;

@property (nonatomic, strong, nullable) FlutterEventSink eventSink;

@end

NS_ASSUME_NONNULL_END
