//
//  ZegoCustomVideoCaptureManager.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/9/11.
//

#ifndef ZegoCustomVideoCaptureManager_h
#define ZegoCustomVideoCaptureManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

@protocol ZegoFlutterCustomVideoCaptureHandler <NSObject>

@required

/// The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
///
/// @param channel publish channel, It is consistent with Dart API
- (void)onStart:(int)channel;

/// The callback triggered when SDK stops receiving captured video data.
///
/// After receiving this callback, the developer is advised to set the saved client object to nil, otherwise the memory occupied by the client object will always exist
/// @param channel publish channel, It is consistent with Dart API
- (void)onStop:(int)channel;

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
- (void)setVideoMirrorMode:(int)mode channel:(int)channel;

/// Sends the video frames (CVPixelBuffer) produced by custom video capture to the SDK.
///
/// This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
///
/// @param buffer Video frame data to send to the SDK
/// @param timestamp Timestamp of this video frame
/// @param channel publish channel, It is consistent with Dart API
- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp channel:(int)channel;

/// Sends the video frames (Texture Data) produced by custom video capture to the SDK.
///
/// This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
///
/// @param textureID texture ID
/// @param size Video frame width and height
/// @param timestamp Timestamp of this video frame
/// @param channel publish channel, It is consistent with Dart API
- (void)sendGLTextureData:(GLuint)textureID size:(CGSize)size timestamp:(CMTime)timestamp channel:(int)channel;

@end

#endif /* ZegoCustomVideoCaptureManager_h */
