//
//  ZegoTextureRenderer.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoTextureRenderer_h
#define ZegoTextureRenderer_h

#import <Foundation/Foundation.h>
#import <ZegoExpressEngine/ZegoExpressDefines.h>

#if TARGET_OS_IPHONE
#import <Flutter/Flutter.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterMacOS.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZegoTextureRenderer : NSObject <FlutterTexture>

@property (nonatomic, readonly) int64_t textureID;
@property (nonatomic) int viewWidth;
@property (nonatomic) int viewHeight;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                              viewWidth:(int)width
                             viewHeight:(int)height;
- (void)destroy;

- (void)setSrcFrameBuffer:(CVPixelBufferRef)srcFrameBuffer;
- (void)setViewMode:(ZegoViewMode)mode;
- (void)setUseMirrorEffect:(BOOL)isUse;
- (void)setBackgroundColor:(int)color;
- (void)updateRenderSize:(CGSize)size;

- (BOOL)isNewFrameAvailable;
- (void)notifyDrawNewFrame;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoTextureRenderer_h */
