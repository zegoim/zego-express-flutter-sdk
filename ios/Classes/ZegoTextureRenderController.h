//
//  ZegoTextureRenderController.h
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


@interface ZegoTextureRenderController : NSObject <ZegoCustomVideoRenderHandler>

@property (nonatomic, assign) BOOL isUseFrontCam;

+ (instancetype)sharedInstance;
// CreateEngine之后调用
- (void)initController;
// DestroyEngine之后调用
- (void)uninitController;

// dart invoke createRenderer 时调用
- (int64_t)createRenderer:(id<FlutterTextureRegistry>)registry viewWidth:(int)width viewHeight:(int)height;
// dart invoke destroyRenderer 时调用
- (void)releaseRenderer:(int64_t)textureID;


// startPreview之前调用
- (BOOL)addCaptureRenderer:(int64_t)textureID ofKey:(NSNumber *)channel;
// startPlayingStream之前调用
- (BOOL)addRemoteRenderer:(int64_t)textureID ofKey:(NSString *)streamID;

// stopPreview之后调用
- (BOOL)removeCaptureRenderer:(NSNumber *)channel;
// stopPlayingStream之后调用
- (BOOL)removeRemoteRenderer:(NSString *)streamID;


- (void)startRendering;
- (void)stopRendering;

@end

#endif /* ZegoTextureRenderController_h */
