//
//  ZegoExpressEngineMediaPlayerEventHandler.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/5/1.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <Flutter/Flutter.h>
#import <ZegoExpressEngine/ZegoExpressEngine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngineMediaPlayerEventHandler : NSObject<ZegoMediaPlayerEventHandler>

- (instancetype)initWithSink:(FlutterEventSink)sink;

@end

NS_ASSUME_NONNULL_END
