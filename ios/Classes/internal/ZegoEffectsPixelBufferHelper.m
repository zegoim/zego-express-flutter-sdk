//
//  ZegoEffectsPixelBufferHelper.m
//  ZegoEffects
//
//  Created by Patrick Fu on 2021/3/23.
//  Copyright © 2021 Zego. All rights reserved.
//

#import "ZegoEffectsPixelBufferHelper.h"

@implementation ZegoEffectsPixelBufferHelper

/** CVPixelBufferRef 转 CVOpenGL(ES)TextureRef */
+ (void)createTexture:(ZGCVOpenGLTextureRef *)textureRef
      fromPixelBuffer:(CVPixelBufferRef)pixelBuffer
                cache:(ZGCVOpenGLTextureCacheRef)textureCache {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    // 将 PixelBuffer 转成 OpenGL(ES) 的 Texture
#if TARGET_OS_IPHONE
    CVReturn res = CVOpenGLESTextureCacheCreateTextureFromImage(
        kCFAllocatorDefault, textureCache, pixelBuffer, NULL, GL_TEXTURE_2D, GL_RGBA,
        (GLsizei)CVPixelBufferGetWidth(pixelBuffer), (GLsizei)CVPixelBufferGetHeight(pixelBuffer),
        GL_BGRA,
        GL_UNSIGNED_BYTE, // UInt8_t
        0, textureRef);
#elif TARGET_OS_OSX
    CVReturn res = CVOpenGLTextureCacheCreateTextureFromImage(kCFAllocatorDefault, textureCache,
                                                              pixelBuffer, NULL, textureRef);
#endif

    // 错误设置上面的 internalFormat / format / type 参数都会导致 res 为 6683 ！
    // 出现 -6683 请检查上述参数！

    // 若传入的 buffer 为 nil，则会出现 -6661 错误

    if (res != kCVReturnSuccess) {
        return;
    }

    GLenum target = ZGCVOpenGLTextureGetTarget(*textureRef);
    GLuint textureID = ZGCVOpenGLTextureGetName(*textureRef);

    // 激活 Texture & 为 Texture 设定纹理参数
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, textureID); // CVOpenGLESTextureGetName 可以获得 texture id

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    

    // 重置 Context 的 GL_TEXTURE_2D，防止意外错误
    glBindTexture(GL_TEXTURE_2D, 0);

    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);

#pragma clang diagnostic pop
}

@end
