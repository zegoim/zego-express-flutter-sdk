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

#define ZGLog(format, ...) [ZegoLog logWithFormat:(format), ## __VA_ARGS__]

+ (void)logWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);

@end

NS_ASSUME_NONNULL_END
