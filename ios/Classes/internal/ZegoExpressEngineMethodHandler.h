//
//  ZegoExpressEngineMethodHandler.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/5/8.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE
#import <Flutter/Flutter.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterMacOS.h>
#endif


NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngineMethodHandler : NSObject

@property (nonatomic, assign, readonly) BOOL enablePlatformView;

+ (instancetype)sharedInstance;

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result;

- (void)setRegistrar:(id<FlutterPluginRegistrar>)registrar eventSink:(FlutterEventSink)sink;

- (void)initApiCalledCallback;

- (void)unInit;

@end

NS_ASSUME_NONNULL_END
