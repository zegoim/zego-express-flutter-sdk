//
//  ZegoExpressEngineEventHandler.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/4/9.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <Flutter/Flutter.h>
#import <ZegoExpressEngine/ZegoExpressEngine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngineEventHandler : NSObject<ZegoEventHandler, ZegoMediaPlayerEventHandler, ZegoAudioEffectPlayerEventHandler, ZegoDataRecordEventHandler>

+ (instancetype)sharedInstance;

@property (nonatomic, strong, nullable) FlutterEventSink eventSink;

@end

NS_ASSUME_NONNULL_END
