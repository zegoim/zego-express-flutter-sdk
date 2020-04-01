//
//  ZegoPlatformViewRenderController.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoPlatformViewRenderController_h
#define ZegoPlatformViewRenderController_h

#import <Foundation/Foundation.h>
#import <Flutter/FlutterPlatformViews.h>
#import "ZegoPlatformViewRenderer.h"

@interface ZegoPlatformViewRenderController : NSObject<FlutterPlatformViewFactory>

+ (instancetype) sharedInstance;

// dart invole removeView 时调用
- (BOOL)removeView:(NSNumber *)viewID;

// startPreview/startPlayingStream 时获取view传入native
- (ZegoPlatformViewRenderer *)getRenderer:(NSNumber *) viewID;

@end

#endif /* ZegoPlatformViewRenderController_h */
