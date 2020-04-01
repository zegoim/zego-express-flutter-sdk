//
//  ZegoPlatformViewRenderer.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoPlatformViewRenderer_h
#define ZegoPlatformViewRenderer_h

#import <Foundation/Foundation.h>
#import <Flutter/FlutterPlatformViews.h>

@interface ZegoPlatformViewRenderer : NSObject<FlutterPlatformView>

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID;

- (UIView *)getUIView;

@end

#endif /* ZegoPlatformViewRenderer_h */
