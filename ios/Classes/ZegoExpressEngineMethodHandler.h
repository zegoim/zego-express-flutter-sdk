//
//  ZegoExpressEngineMethodHandler.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/5/8.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <Flutter/Flutter.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngineMethodHandler : NSObject

@property (nonatomic, assign, readonly) BOOL enablePlatformView;

+ (instancetype)sharedInstance;

@end

NS_ASSUME_NONNULL_END
