//
//  ZegoUtils.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/2/14.
//

#ifndef ZegoFlutterUtils_h
#define ZegoFlutterUtils_h

#import <Foundation/Foundation.h>

@interface ZegoUtils : NSObject

+ (BOOL)boolValue:(NSNumber *)number;

+ (int)intValue:(NSNumber *)number;

+ (unsigned int)unsignedIntValue:(NSNumber *)number;

+ (unsigned long)unsignedLongValue:(NSNumber *)number;

+ (float)folatValue:(NSNumber *)number;

+ (double)doubleValue:(NSNumber *)number;

@end

#endif /* ZegoFlutterUtils_h */
