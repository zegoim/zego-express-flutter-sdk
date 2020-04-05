//
//  ZegoLog.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/3/25.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoLog.h"
#include "../../zego_defines.h"

@implementation ZegoLog

+ (void)logWithFile:(const char *)file
           function:(const char *)function
               line:(NSUInteger)line
             format:(NSString *)format, ... NS_FORMAT_FUNCTION(4,5) {
    if (format) {
        va_list args;
        va_start(args, format);
        [self logWithFile:file
                 function:function
                     line:line
                   format:format
                     args:args];
        va_end(args);
    }
}

+ (void)logWithFile:(const char *)file
           function:(const char *)function
               line:(NSUInteger)line
             format:(NSString *)format
               args:(va_list)argList {
    if (!format) {
        return;
    }
    NSString *msg = [[NSString alloc] initWithFormat:format arguments:argList];
    
    [ZegoLog log:[NSString stringWithFormat:@"[Flutter-Native] %s:%lu, %s, %@", file, (unsigned long)line, function, msg]];
}

+ (void)log:(NSString *)content {
//    zego_liveroom_custom_log(content.UTF8String);
    NSLog(@"%@", content);
}

@end
