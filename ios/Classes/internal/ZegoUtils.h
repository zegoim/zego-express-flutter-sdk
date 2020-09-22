//
//  ZegoUtils.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/2/14.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoFlutterUtils_h
#define ZegoFlutterUtils_h

#import <Foundation/Foundation.h>

@interface ZegoUtils : NSObject

+ (BOOL)boolValue:(NSNumber *)number;

+ (int)intValue:(NSNumber *)number;

+ (unsigned int)unsignedIntValue:(NSNumber *)number;

+ (unsigned long)unsignedLongValue:(NSNumber *)number;

+ (unsigned long long)unsignedLongLongValue:(NSNumber *)number;

+ (long long)longLongValue:(NSNumber *)number;

+ (float)floatValue:(NSNumber *)number;

+ (double)doubleValue:(NSNumber *)number;

+ (BOOL)isNullObject:(id)object;

@end

#endif /* ZegoFlutterUtils_h */
