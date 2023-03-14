//
//  ZegoCustomVideoProcessManager.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoCustomVideoProcessManager_h
#define ZegoCustomVideoProcessManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterCustomVideoProcessHandler <NSObject>

@optional

/// The SDK informs the developer that it is about to start custom video processing.
///
/// Available since: 2.2.0
/// Description: When the custom video pre-processing is turned on, the SDK informs the developer that the video pre-processing is about to start, and it is recommended to initialize other resources(E.g. Beauty SDK) in this callback.
/// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
/// When to Trigger: Open custom video pre-processing, after calling [startPreview] or [startPublishingStream] successfully.
/// Related callbacks: Custom video pre-processing end notification [onStop].
/// Related APIs: Call [setCustomVideoProcessHandler] function to set custom video pre-processing callback.
///
/// @param channel Publishing stream channel.
- (void)onStart:(ZGFlutterPublishChannel)channel;

/// The SDK informs the developer to stop custom video processing.
///
/// Available since: 2.2.0
/// Description: When the custom video pre-processing is turned on, the SDK informs the developer that the video pre-processing is about to end, and it is recommended to destroy other resources(E.g. Beauty SDK) in this callback.
/// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
/// When to Trigger: If you call [startPreview] to start the preview and [startPublishingStream] to start the push stream at the same time after you start the custom capture, you should call [stopPreview] to stop the preview and [stopPublishingStream] to stop the push stream before the callback will be triggered.
/// Related callbacks: Custom video pre-processing start notification [onStart].
/// Related APIs: Call [setCustomVideoProcessHandler] function to set custom video pre-processing callback.
///
/// @param channel Publishing stream channel.
- (void)onStop:(ZGFlutterPublishChannel)channel;

/// Call back when the original video data of type [CVPixelBuffer] is obtained.
///
/// Available since: 2.2.0
/// Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedCVPixelbuffer] to send the processed data back to the SDK, otherwise it will cause frame loss.
/// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
/// When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
/// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeCVPixelBuffer].
/// Platform differences: It only takes effect on the iOS/macOS platform.
///
/// @param buffer CVPixelBuffer type video frame data to be sent to the SDK.
/// @param timestamp Timestamp of this video frame.
/// @param channel Publishing stream channel.
- (void)onCapturedUnprocessedCVPixelBuffer:(CVPixelBufferRef)buffer
                                 timestamp:(CMTime)timestamp
                                   channel:(ZGFlutterPublishChannel)channel;

@end

@interface ZegoCustomVideoProcessManager : NSObject

/// Get the custom video capture manager instance
+ (instancetype)sharedInstance;

/// Sets up the event callback handler for custom video processing.
///
/// Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
/// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
/// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
/// When to call: Called before [startPreview], [startPublishingStream] and [createRealTimeSequentialDataManager], otherwise it may cause the timing of obtaining video data to be too slow.
/// Restrictions: None.
/// Related APIs: Call [enableCustomVideoProcessing] function to enable custom video pre-processing callback.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param handler Custom video process handler.Required: Yes.
- (void)setCustomVideoProcessHandler:(id<ZegoFlutterCustomVideoProcessHandler>)handler;


/// Send the [CVPixelBuffer] type video data after the custom video processing to the SDK (for the specified channel).
///
/// Available since: 2.2.0 (iOS native), 2.4.0 (macOS C++).
/// Description: When the custom video pre-processing is turned on, the [CVPixelBuffer] format video data after the custom video pre-processing is sent to the SDK, and other channels are supported.
/// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
/// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
/// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeCVPixelBuffer].
/// Platform differences: Only valid on Windows platform.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param buffer CVPixelBuffer type video frame data to be sent to the SDK.Required: Yes.
/// @param timestamp Timestamp of this video frame.Required: Yes.
- (void)sendProcessedCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoCustomVideoProcessManager_h */
