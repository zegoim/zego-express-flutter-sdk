//
//  ZegoLog.h
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/3/25.
//  Copyright © 2020 Zego. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZegoLog : NSObject

#define ZGLog(frmt, ...) \
[ZegoLog logWithFile:__FILE__ \
function:__PRETTY_FUNCTION__ \
line:__LINE__ \
format:(frmt), ## __VA_ARGS__]

+ (void)logWithFile:(const char *)file
           function:(const char *)function
               line:(NSUInteger)line
             format:(NSString *)format, ... NS_FORMAT_FUNCTION(4,5);

@end

NS_ASSUME_NONNULL_END
