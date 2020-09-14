//
//  ZegoPlatformViewFactory.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoPlatformViewRenderController_h
#define ZegoPlatformViewRenderController_h

#import <Foundation/Foundation.h>
#import <Flutter/FlutterPlatformViews.h>
#import "ZegoPlatformView.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoPlatformViewFactory : NSObject<FlutterPlatformViewFactory>

+ (instancetype)sharedInstance;

/// Called when dart invoke `destroyPlatformView`
- (BOOL)destroyPlatformView:(NSNumber *)viewID;

/// Get PlatformView to pass to native when dart invoke `startPreview` or `startPlayingStream`
- (nullable ZegoPlatformView *)getPlatformView:(NSNumber *)viewID;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoPlatformViewRenderController_h */
