//
//  ZegoCustomVideoCaptureManager.h
//  zego_express_engine
//
//  Created by Patrick Fu on 2022/7/18.
//

#ifndef ZegoCustomVideoCaptureManager_h
#define ZegoCustomVideoCaptureManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterCustomVideoCaptureHandler <NSObject>

@required

/// The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
///
/// @param channel publish channel, It is consistent with Dart API
- (void)onStart:(ZGFlutterPublishChannel)channel;

/// The callback triggered when SDK stops receiving captured video data.
///
/// After receiving this callback, the developer is advised to set the saved client object to nil, otherwise the memory occupied by the client object will always exist
/// @param channel publish channel, It is consistent with Dart API
- (void)onStop:(ZGFlutterPublishChannel)channel;

/// When network changes are detected during custom video capture, the developer is notified that traffic control is required, and the encoding configuration is adjusted according to the recommended parameters of the SDK.
///
/// Available since: 1.14.0
/// Description: When using custom video capture, the SDK detects a network change, informs the developer that it needs to do flow control, and adjust the encoding configuration according to the recommended parameters of the SDK. In the case of custom collection and transmission of encoded data, the SDK cannot know the external encoding configuration, so the flow control operation needs to be completed by the developer. The SDK will notify the developer of the recommended value of the video configuration according to the current network situation, and the developer needs to modify the encoder configuration by himself to ensure the smoothness of video transmission.
/// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
/// When to Trigger: When network status changes during the process of custom video capture and flow control is required.
/// Caution: Please do not perform time-consuming operations in this callback, such as reading and writing large files. If you need to perform time-consuming operations, please switch threads.
/// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
///
/// @param trafficControlInfo traffic control info.
/// @param channel Publishing stream channel.
- (void)onEncodedDataTrafficControl:(ZGFlutterTrafficControlInfo *)trafficControlInfo
                            channel:(ZGFlutterPublishChannel)channel;

@end

@interface ZegoCustomVideoCaptureManager : NSObject

/// Get the custom video capture manager instance
+ (instancetype)sharedInstance;

/// Sets the event callback handler for custom video capture.
///
/// Developers need to pass in the callback object that implements [ZegoFlutterCustomVideoCaptureHandler] and open [enableCustomVideoCapture] in the Dart API to make the custom capture module take effect.
/// When the developer calls [startPreview]/[stopPreview] or [startPublishingStream]/[stopPublishingStream] in the Dart API, the SDK will notify the developer  the start/stop of the custom video capture, and the developer can receive [onStart] and [onStop] Start and stop the external input source after the notification.
/// This API call is set at any time before [enableCustomVideoCapture] is enabled in Dart
/// @param handler the callback object that implements the [ZegoFlutterCustomVideoCaptureHandler]
- (void)setCustomVideoCaptureHandler:(id<ZegoFlutterCustomVideoCaptureHandler>)handler;


/// Sets the video mirroring mode.
///
/// This function can be called to set whether the local preview video and the published video have mirror mode enabled.
///
/// @param mode Mirror mode for previewing or publishing the stream. 0: Only preview mirror. 1: Both preview and publish mirror. 2: No Mirror. 3. Only publish mirror. No mirror by default.
/// @param channel publish channel, It is consistent with Dart API
- (void)setVideoMirrorMode:(int)mode channel:(ZGFlutterPublishChannel)channel;

/// Sets the video fill mode of custom video capture (for the specified channel).
///
/// Available since: 1.10.0
/// Use cases: If the customized collection resolution and SDK coding resolution are inconsistent, THE SDK will process the video frame according to the fill mode and adjust it to the coding resolution.
/// Description: When the custom video collection function is enabled, you can use this interface to set the video zooming and filling mode.
/// When to call: Before [sendCustomVideoCaptureRawData], [sendCustomVideoCaptureTextureData], [sendCustomVideoCapturePixelBuffer], [sendCustomVideoCaptureEncodedData].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mode View mode
/// @param channel Publishing stream channel
- (void)setFillMode:(ZGFlutterViewMode)mode channel:(ZGFlutterPublishChannel)channel;

/// Sets the video flip mode of custom video capture (for the specified channel). This function takes effect only if the custom video buffer type is Texture2D.
///
/// Available since: 1.10.0
/// Description: When the custom video collection function is enabled, you can use this interface to set the video flipping mode.
/// When to call: Before [sendCustomVideoCaptureRawData], [sendCustomVideoCaptureTextureData], [sendCustomVideoCapturePixelBuffer], [sendCustomVideoCaptureEncodedData].
/// Caution: This function takes effect only if the custom collection type is Texture2D.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mode Flip mode
/// @param channel Publishing stream channel
- (void)setFlipMode:(ZGFlutterVideoFlipMode)mode channel:(ZGFlutterPublishChannel)channel;

/// Sets the video rotation of custom video capture (for the specified channel).
///
/// Available since: 2.14.0.
/// Description: You can set the video rotation of custom video capture (for the specified channel) through this function.
/// When to call: After the callback [onStart] is received.
/// Restrictions: This function takes effect when the custom acquisition type is CVPixelBuffer or Texture2D.
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param rotation Clockwise angle.
/// @param channel Publishing stream channel
- (void)setRotation:(int)rotation channel:(ZGFlutterPublishChannel)channel;

/// Sends the video frames (CVPixelBuffer) produced by custom video capture to the SDK.
///
/// This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
///
/// @param buffer Video frame data to send to the SDK
/// @param timestamp Timestamp of this video frame
/// @param channel publish channel, It is consistent with Dart API
- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel;

/// Sends the video frames (Texture Data) produced by custom video capture to the SDK.
///
/// This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
///
/// @param textureID texture ID
/// @param size Video frame width and height
/// @param timestamp Timestamp of this video frame
/// @param channel publish channel, It is consistent with Dart API
- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp channel:(ZGFlutterPublishChannel)channel;

/// Sends the video frames (Encoded Data) produced by custom video capture to the SDK (for the specified channel).
///
/// Available since: 1.9.0
/// Description: Send custom captured video frame type CVPixelBuffer data to the SDK.
/// When to call: After receiving the [onStart] notification, the developer starts the call after the collection logic starts and ends the call after the [onStop] notification.
/// Caution: It is recommended to make a GOP every 2s, and each I frame must carry SPS and PPS and be placed at the top.  Only I and P frames are accepted, B frames are NOT accepted. Calling this interface must be [enableCustomVideoCapture] with an argument of type ZegoVideoBufferTypeEncodedData.
/// Related APIs: [enableCustomVideoCapture], [setCustomVideoCaptureHandler].
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param data video frame encoded data
/// @param params video frame param
/// @param timestamp video frame reference time, UNIX timestamp.
/// @param channel Publishing stream channel
- (void)sendEncodedData:(NSData *)data
                                   params:(ZGFlutterVideoEncodedFrameParam *)params
                                timestamp:(CMTime)timestamp
                                  channel:(ZGFlutterPublishChannel)channel;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoCustomVideoCaptureManager_h */
