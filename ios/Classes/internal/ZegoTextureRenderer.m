//
//  ZegoTextureRenderer.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoTextureRenderer.h"
#import "ZegoLog.h"

@interface ZegoTextureRenderer ()

@property (nonatomic, weak) id<FlutterTextureRegistry> registry;

@end

@implementation ZegoTextureRenderer {
    CVPixelBufferRef _lastPixelBuffer;
}

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry size:(CGSize)size {
    self = [super init];
    if (self) {
        _registry = registry;
        _textureID = [registry registerTexture:self];
    }

    ZGLog(@"[ZegoTextureRenderer] [init] renderer:%p", self);

    return self;
}

- (void)destroy {
    @synchronized (self) {
        // Release GPU Resource
        [self.registry unregisterTexture:_textureID];
    }

    ZGLog(@"[ZegoTextureRenderer] [release] renderer:%p", self);
}

- (void)updateSrcFrameBuffer:(CVPixelBufferRef)buffer {
    @synchronized (self) {
        if (_lastPixelBuffer) {
            CVPixelBufferRelease(_lastPixelBuffer);
        }
        _lastPixelBuffer = buffer;
        CVPixelBufferRetain(_lastPixelBuffer);
        [self.registry textureFrameAvailable:self.textureID];
    }
}

#pragma mark - FlutterTexture Delegate

- (CVPixelBufferRef)copyPixelBuffer {
    @synchronized (self) {
        if (_lastPixelBuffer) {
            CVPixelBufferRetain(_lastPixelBuffer);
        }
        return _lastPixelBuffer;
    }
}

- (void)onTextureUnregistered:(NSObject<FlutterTexture> *)texture {
    ZGLog(@"[ZegoTextureRenderer] [unregistered] renderer:%p", texture);
    @synchronized (self) {
        if (_lastPixelBuffer) {
            CVPixelBufferRelease(_lastPixelBuffer);
        }
    }
}

@end
