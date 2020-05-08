//
//  ZegoExpressEngineMediaPlayerEventHandler.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/5/1.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoExpressEngineMediaPlayerEventHandler.h"
#import "ZegoLog.h"

@interface ZegoExpressEngineMediaPlayerEventHandler ()

@property (nonatomic, strong) FlutterEventSink eventSink;

@end

@implementation ZegoExpressEngineMediaPlayerEventHandler

- (instancetype)initWithSink:(FlutterEventSink)sink {
    self = [super init];
    if (self) {
        _eventSink = sink;
    }
    return self;
}

#pragma mark - Zego

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer stateUpdate:(ZegoMediaPlayerState)state errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"state: %d, errorCode: %d", (int)state, errorCode);

    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerStateUpdate",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"state": @(state),
            @"errorCode": @(errorCode)
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer networkEvent:(ZegoMediaPlayerNetworkEvent)networkEvent {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"networkEvent: %d", (int)networkEvent);

    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerNetworkEvent",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"networkEvent": @(networkEvent)
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer playingProgress:(unsigned long long)millisecond {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"millisecond: %llu", millisecond);

    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerPlayingProgress",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"millisecond": @(millisecond)
        });
    }
}

@end
