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
#import <Flutter/Flutter.h>
#import <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum
{
    ZegoRendererScaleAspectFit = 0,    ///< 等比缩放，可能有黑边
    ZegoRendererScaleAspectFill = 1,   ///< 等比缩放填充整View，可能有部分被裁减
    ZegoRendererScaleToFill = 2,       ///< 填充整个View
}ZegoRendererViewMode;

@interface ZegoTextureRenderer : NSObject <FlutterTexture>

@property (nonatomic, readonly) int64_t textureID;
@property (nonatomic) int viewWidth;
@property (nonatomic) int viewHeight;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height;
- (void)destroy;

- (void)setSrcFrameBuffer:(CVPixelBufferRef)srcFrameBuffer;
- (void)setVideoMode:(ZegoRendererViewMode)mode;
- (void)setUseMirrorEffect:(BOOL)isUse;
- (void)setBackgroundColor:(int)color;
- (void)updateRenderSize:(CGSize)size;

- (BOOL)isNewFrameAvailable;
- (void)notifyDrawNewFrame;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoTextureRenderer_h */
