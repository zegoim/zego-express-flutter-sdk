//
//  ZegoFlutterUtils.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/2/14.
//

#import "ZegoFlutterUtils.h"

@implementation ZegoFlutterUtils

+ (bool)numberToBoolValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? false : [number boolValue];
}

+ (int)numberToIntValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number intValue];
}

+ (unsigned int)numberToUintValue:(NSNumber *)number {
   
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedIntValue];
}

+ (unsigned long)numberToULongValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0 : [number unsignedLongValue];
}

+ (float)numberToFolatValue:(NSNumber *)number {
    
    return [number isKindOfClass:[NSNull class]] ? 0.f : [number floatValue];
}

+ (double)numberToDoubleValue:(NSNumber *)number {
    return [number isKindOfClass:[NSNull class]] ? 0.0 : [number doubleValue];
}

@end
