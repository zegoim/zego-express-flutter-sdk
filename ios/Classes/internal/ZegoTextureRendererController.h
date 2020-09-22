//
//  ZegoTextureRendererController.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#ifndef ZegoTextureRenderController_h
#define ZegoTextureRenderController_h

#import <Foundation/Foundation.h>
#import <ZegoExpressEngine/ZegoExpressEngine.h>
#import "ZegoTextureRenderer.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoTextureRendererController : NSObject <ZegoCustomVideoRenderHandler>

@property (nonatomic, assign) BOOL isUseFrontCam;

+ (instancetype)sharedInstance;


#pragma mark - Dart Texture Render Utils Operation

/// Called when dart invoke `createTextureRenderer`
/// @return textureID
- (int64_t)createTextureRenderer:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height;

/// Called when dart invoke `updateTextureRendererSize`
- (BOOL)updateTextureRenderer:(int64_t)textureID viewWidth:(int)width viewHeight:(int)height;

/// Called when dart invoke `destroyTextureRenderer`
- (BOOL)destroyTextureRenderer:(int64_t)textureID;



#pragma mark - Dart Express Engine API Operation

/// Called when dart invoke `createEngine`
- (void)initController;

/// Called when dart invoke `destroyEngine`
- (void)uninitController;



/// Called when dart invoke `startPreview`
- (BOOL)addCapturedRenderer:(int64_t)textureID key:(NSNumber *)channel;

/// Called when dart invoke `stopPreview`
- (void)removeCapturedRenderer:(NSNumber *)channel;

/// Called when dart invoke `startPlayingStream`
- (BOOL)addRemoteRenderer:(int64_t)textureID key:(NSString *)streamID;

/// Called when dart invoke `stopPlayingStream`
- (void)removeRemoteRenderer:(NSString *)streamID;



/// For video preview/play
- (void)startRendering;

/// For video preview/play
- (void)stopRendering;


- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer param:(ZegoVideoFrameParam *)param flipMode:(ZegoVideoFlipMode)flipMode channel:(ZegoPublishChannel)channel;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoTextureRenderController_h */
