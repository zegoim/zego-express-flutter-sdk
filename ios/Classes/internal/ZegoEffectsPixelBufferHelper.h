//
//  ZegoEffectsPixelBufferHelper.h
//  ZegoEffects
//
//  Created by Patrick Fu on 2021/3/23.
//  Copyright © 2021 Zego. All rights reserved.
//

#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES3/glext.h>

#define ZGCVOpenGLTextureRef CVOpenGLESTextureRef
#define ZGCVOpenGLTextureCacheRef CVOpenGLESTextureCacheRef
#define ZGGLContext EAGLContext

#elif TARGET_OS_OSX

#import <OpenGL/gl.h>
#import <OpenGL/glext.h>

#define ZGCVOpenGLTextureRef CVOpenGLTextureRef
#define ZGCVOpenGLTextureCacheRef CVOpenGLTextureCacheRef
#define ZGGLContext NSOpenGLContext

#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZegoEffectsPixelBufferHelper : NSObject

+ (void)createTexture:(ZGCVOpenGLTextureRef _Nonnull *_Nonnull)textureRef
      fromPixelBuffer:(CVPixelBufferRef _Nonnull)pixelBuffer
                cache:(ZGCVOpenGLTextureCacheRef _Nonnull)textureCache;

@end

NS_ASSUME_NONNULL_END
