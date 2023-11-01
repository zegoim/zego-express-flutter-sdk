//
//  ZegoTextureRendererController.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoTextureRenderController_h
#define ZegoTextureRenderController_h

#import "ZegoTextureRenderer.h"
#import <Foundation/Foundation.h>
#import <ZegoExpressEngine/ZegoExpressEngine.h>
#import "ZegoCustomVideoRenderManager.h"
#import "ZegoMediaPlayerVideoManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoTextureRendererController : NSObject <ZegoCustomVideoRenderHandler, ZegoMediaPlayerVideoHandler>

+ (instancetype)sharedInstance;

#pragma mark - Dart Texture Render Utils Operation

/// Called when dart invoke `createTextureRenderer`
/// @return textureID
- (int64_t)createTextureRenderer:(id<FlutterTextureRegistry>)registry
                       viewWidth:(int)width
                      viewHeight:(int)height;

/// Called when dart invoke `destroyTextureRenderer`
- (BOOL)destroyTextureRenderer:(int64_t)textureID;

#pragma mark - Dart Express Engine API Operation

/// Called when dart invoke `createEngine`
- (void)initControllerWithMessenger:(NSObject<FlutterBinaryMessenger> *)messenger;

/// Called when dart invoke `destroyEngine`
- (void)uninitController;

/// Called when dart invoke `startPreview`
- (BOOL)bindCapturedChannel:(NSNumber *)channel withTexture:(int64_t)textureID;

/// Called when dart invoke `stopPreview`
- (void)unbindCapturedChannel:(NSNumber *)channel;

/// Called when dart invoke `startPlayingStream`
- (BOOL)bindRemoteStreamId:(NSString *)streamId withTexture:(int64_t)textureID;

/// Called when dart invoke `stopPlayingStream`
- (void)unbindRemoteStreamId:(NSString *)streamId;

/// Called when dart invoke `mediaPlayerSetPlayerCanvas`
- (BOOL)bindMediaPlayerIndex:(NSNumber *)index withTexture:(int64_t)textureID;

/// Called when dart invoke `destroyMediaPlayer`
- (void)unbindMediaPlayerIndex:(NSNumber *)index;

/// Called when dart invoke `enableCustomVideoRender`
- (void)setCustomVideoRenderHandler: (id<ZegoFlutterCustomVideoRenderHandler> _Nullable) handler;

/// Called when dart invoke `MediaPlayer.enableVideoData`
- (void)setMediaPlayerVideoHandle: (id<ZegoFlutterMediaPlayerVideoHandler> _Nullable) handler;

/// Called when dart invoke `startPreview/startPlayingStream/updatePlayingCanvas`
- (BOOL)enableTextureAlpha:(BOOL) enable withTexture:(int64_t)textureID;

/// Called when dart invoke `setVideoSource`
- (void)setVideoSourceChannel:(NSNumber *)channel withSource:(ZegoVideoSourceType)sourceType;

- (void)resetMediaRenderFirstFrame:(NSNumber *)index;

- (void)resetAllRenderFirstFrame;

#pragma mark - For CustomVideoCaptureManager
- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                    param:(ZegoVideoFrameParam *)param
                                 flipMode:(ZegoVideoFlipMode)flipMode
                                  channel:(ZegoPublishChannel)channel;

- (void)sendScreenCapturedVideoFrameRawData:(const void *)data
                                 dataLength:(unsigned int)dataLength
                                      param:(ZegoVideoFrameParam *)param;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoTextureRenderController_h */
