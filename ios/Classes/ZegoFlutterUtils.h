//
//  ZegoFlutterUtils.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/2/14.
//

#ifndef ZegoFlutterUtils_h
#define ZegoFlutterUtils_h

#import <Foundation/Foundation.h>

@interface ZegoFlutterUtils : NSObject

+ (bool)numberToBoolValue:(NSNumber *)number;

+ (int)numberToIntValue:(NSNumber *)number;

+ (unsigned int)numberToUintValue:(NSNumber *)number;

+ (unsigned long)numberToULongValue:(NSNumber *)number;

+ (float)numberToFolatValue:(NSNumber *)number;

+ (double)numberToDoubleValue:(NSNumber *)number;

@end

#endif /* ZegoFlutterUtils_h */
