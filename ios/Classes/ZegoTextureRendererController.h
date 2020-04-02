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


@interface ZegoTextureRendererController : NSObject <ZegoCustomVideoRenderHandler>

@property (nonatomic, assign) BOOL isUseFrontCam;

+ (instancetype)sharedInstance;

/// Called when dart invoke `createEngine`
- (void)initController;

/// Called when dart invoke `destroyEngine`
- (void)uninitController;


/// Called when dart invoke `createTextureRenderer`
/// @return textureID
- (int64_t)createRenderer:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height;

/// Called when dart invoke `updateTextureRendererSize`
- (void)updateTextureRenderer:(int64_t)textureID viewWidth:(int)width viewHeight:(int)height;

/// Called when dart invoke `destroyTextureRenderer`
- (void)releaseRenderer:(int64_t)textureID;


/// Called when dart invoke `startPreview`
- (BOOL)addCapturedRenderer:(int64_t)textureID key:(NSNumber *)channel;

/// Called when dart invoke `startPlayingStream`
- (BOOL)addRemoteRenderer:(int64_t)textureID key:(NSString *)streamID;

/// Called when dart invoke `stopPreview`
- (BOOL)removeCapturedRenderer:(NSNumber *)channel;

/// Called when dart invoke `stopPlayingStream`
- (BOOL)removeRemoteRenderer:(NSString *)streamID;


- (void)startRendering;

- (void)stopRendering;

@end

#endif /* ZegoTextureRenderController_h */
