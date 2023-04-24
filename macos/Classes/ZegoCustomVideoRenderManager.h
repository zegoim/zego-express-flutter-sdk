//
//  ZegoCustomVideoRenderManager.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoCustomVideoRenderManager_h
#define ZegoCustomVideoRenderManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterCustomVideoRenderHandler <NSObject>

@optional

/// When custom video rendering is enabled, the original video frame data collected by the local preview is called back.
///
/// Available since: 1.1.0
/// Description: When using custom video rendering, the SDK callbacks the original video frame data collected by the local preview, which is rendered by the developer.
/// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
/// When to Trigger: When the local preview is turned on, when the SDK collects the local preview video frame data.
/// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
///
/// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
/// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
/// @param param Video frame parameters.
/// @param flipMode video flip mode.
/// @param channel Publishing stream channel.
- (void)onCapturedVideoFrameRawData:(unsigned char *_Nonnull *_Nonnull)data
                         dataLength:(unsigned int *)dataLength
                              param:(ZGFlutterVideoFrameParam *)param
                           flipMode:(ZGFlutterVideoFlipMode)flipMode
                            channel:(ZGFlutterPublishChannel)channel;

/// When custom video rendering is enabled, the remote end pulls the original video frame data to call back, and distinguishes different streams by streamID.
///
/// Available since: 1.1.0
/// Description: When custom video rendering is enabled, the SDK calls back the remote end to pull the original video frame data, distinguishes different streams by streamID, and renders them by the developer.
/// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
/// When to Trigger: After starting to stream, when the SDK receives the video frame data of the remote stream.
/// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
///
/// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
/// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
/// @param param Video frame parameters.
/// @param streamID Stream ID.
- (void)onRemoteVideoFrameRawData:(unsigned char *_Nonnull *_Nonnull)data
                       dataLength:(unsigned int *)dataLength
                            param:(ZGFlutterVideoFrameParam *)param
                         streamID:(NSString *)streamID;

/// Custom video rendering local preview video frame CVPixelBuffer data callback.
///
/// Available since: 1.1.0
/// Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [CVPixelBuffer], this function will be called back with the local preview video frame CVPixelBufferRef data.
/// Use cases: Scenes that need to obtain video frame data captured by the SDK for special processing, such as cross-platform UI frameworks, game engines, etc..
/// When to Trigger: After calling [startPreview], the SDK will call back when the local preview video frame data is captured.
/// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
/// Platform differences: Only for iOS and macOS.
///
/// @param buffer Video frame data in CVPixelBufferRef
/// @param param Video frame param.
/// @param flipMode video flip mode.
/// @param channel Publishing stream channel.
- (void)onCapturedVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                    param:(ZGFlutterVideoFrameParam *)param
                                 flipMode:(ZGFlutterVideoFlipMode)flipMode
                                  channel:(ZGFlutterPublishChannel)channel;

/// Custom video rendering remote playing stream video frame CVPixelBuffer data callback.
///
/// Available since: 1.1.0
/// Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [CVPixelBuffer], this function will be called back with the remote playing stream video frame CVPixelBufferRef data.
/// Use cases: Scenes that need to obtain the video frame data of the SDK remote playing stream for special processing, such as using a cross-platform UI framework, game engine, etc.
/// When to Trigger: After calling [startPlayingStream], the SDK will call back when the remote streaming video frame data is received.
/// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
/// Platform differences: Only for iOS and macOS.
///
/// @param buffer Video frame data in CVPixelBufferRef
/// @param param Video frame param.
/// @param streamID Stream ID.
- (void)onRemoteVideoFrameCVPixelBuffer:(CVPixelBufferRef)buffer
                                  param:(ZGFlutterVideoFrameParam *)param
                               streamID:(NSString *)streamID;

/// Custom video rendering remote playing stream video frame undecoded data callback.
///
/// Available since: 1.10.0
/// Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [EncodedData], this function will be called back with the remote playing stream undecoded video frame data.
/// When to Trigger: After calling [startPlayingStream], the SDK will call back when the remote streaming video frame data is received.
/// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
///
/// @param data Encoded data of video frames.
/// @param dataLength Data length.
/// @param param Video frame parameters.
/// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
/// @param streamID Stream ID.
- (void)onRemoteVideoFrameEncodedData:(unsigned char *_Nonnull)data
                           dataLength:(unsigned int)dataLength
                                param:(ZGFlutterVideoEncodedFrameParam *)param
             referenceTimeMillisecond:(unsigned long long)referenceTimeMillisecond
                             streamID:(NSString *)streamID;

@end

@interface ZegoCustomVideoRenderManager : NSObject

/// Get the custom video capture manager instance
+ (instancetype)sharedInstance;

/// Sets up the event callback handler for custom video rendering.
///
/// Available since: 1.9.0
/// Description: Custom video rendering, where the developer is responsible for rendering the video data onto the UI components. With the change callback set, a callback that throws video data to the developer will be triggered when the developer calls startPreview, startPublishingStream, and startPlayingStream.
/// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
/// When to call: After [createEngine].
/// Related callbacks: [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param handler Custom video render handler
-(void)setCustomVideoRenderHandler:(id<ZegoFlutterCustomVideoRenderHandler>)handler;

-(id<ZegoFlutterCustomVideoRenderHandler>_Nullable)getCustomVideoRenderHandler;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoCustomVideoRenderManager_h */
