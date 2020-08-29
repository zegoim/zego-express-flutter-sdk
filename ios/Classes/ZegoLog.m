//
//  ZegoLog.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/3/25.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoLog.h"
#import <objc/message.h>

static Class engineClass;
static SEL logSelector;
static const char *moduleName = "Flutter-Native";

@implementation ZegoLog

+ (void)logWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2) {
    if (!format) return;

    if (!engineClass || !logSelector) {
        engineClass = NSClassFromString(@"ZegoExpressEngine");
        logSelector = NSSelectorFromString(@"logNotice:module:");
    }

    va_list args;
    va_start(args, format);

    NSString *message = [[NSString alloc] initWithFormat:format arguments:args];
    ((void (*)(id, SEL, const char *, const char *))objc_msgSend)(engineClass, logSelector, [message UTF8String], moduleName);
    NSLog(@"[%s] %@", moduleName, message);

    va_end(args);
}

@end
