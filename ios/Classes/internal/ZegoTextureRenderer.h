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

@property (nonatomic, assign) CGSize imageSize;
@property (nonatomic, assign) int rotation;
@property (nonatomic, assign) ZegoVideoFlipMode flipMode;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry size:(CGSize)size;
- (void)destroy;

- (void)updateSrcFrameBuffer:(CVPixelBufferRef)buffer;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoTextureRenderer_h */
