//
//  ZegoUtils.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/2/14.
//

#import "ZegoUtils.h"

@implementation ZegoUtils

+ (BOOL)boolValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? NO : [number boolValue];
}

+ (int)intValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number intValue];
}

+ (unsigned int)unsignedIntValue:(NSNumber *)number {
   
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedIntValue];
}

+ (long)longValue:(NSNumber *)number {
    return [number isKindOfClass:[NSNull class]] ? 0 : [number longValue];
}

+ (unsigned long)unsignedLongValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedLongValue];
}

+ (float)folatValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0.f : [number floatValue];
}

+ (double)doubleValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0.0 : [number doubleValue];
}

@end
