//
//  ZegoPlatformView.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoPlatformViewRenderer_h
#define ZegoPlatformViewRenderer_h

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <Flutter/FlutterPlatformViews.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterPlatformViews.h>
#endif

#import <ZegoExpressEngine/ZegoExpressEngine.h>

NS_ASSUME_NONNULL_BEGIN

#if TARGET_OS_IPHONE
@interface ZegoPlatformView : NSObject<FlutterPlatformView>
#elif TARGET_OS_OSX
@interface ZegoPlatformView : NSObject
#endif

@property (nonatomic, strong, readonly) ZGView *view;

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoPlatformViewRenderer_h */
