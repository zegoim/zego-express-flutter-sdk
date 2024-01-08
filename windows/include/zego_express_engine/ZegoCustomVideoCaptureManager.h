#ifndef ZEGO_CUSTOM_VIDEO_CAPTURE_MANAGER_H_
#define ZEGO_CUSTOM_VIDEO_CAPTURE_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterCustomVideoCaptureHandler {
protected:
    virtual ~IZegoFlutterCustomVideoCaptureHandler() {}
public:
    /// Customize the notification of the start of video capture.
    ///
    /// Available since: 1.1.0
    /// Description: The SDK informs that the video frame is about to be collected, and the video frame data sent to the SDK is valid after receiving the callback.
    /// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
    /// When to Trigger: After calling [startPreview] or [startPublishingStream] successfully.
    /// Caution: The video frame data sent to the SDK after receiving the callback is valid.
    /// Related callbacks: Customize the end of capture notification [onCaptureStop].
    /// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
    ///
    /// @param channel Publishing stream channel.
    virtual void onStart(ZGFlutterPublishChannel channel) = 0;

    /// Customize the notification of the end of the collection.
    ///
    /// Available since: 1.1.0
    /// Description: The SDK informs that it is about to end the video frame capture.
    /// Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
    /// When to Trigger: After calling [stopPreview] or [stopPublishingStream] successfully.
    /// Caution: If you call [startPreview] and [startPublishingStream] to start preview and push stream at the same time after you start custom capture, you should call [stopPreview] and [stopPublishingStream] to stop the preview and push stream before triggering the callback.
    /// Related callbacks: Custom video capture start notification [onCaptureStart].
    /// Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
    ///
    /// @param channel Publishing stream channel.
    virtual void onStop(ZGFlutterPublishChannel channel) = 0;

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
    virtual void onEncodedDataTrafficControl(ZGFlutterTrafficControlInfo trafficControlInfo, ZGFlutterPublishChannel channel) = 0;
};

namespace ZEGO::EXPRESS {
    class IZegoCustomVideoCaptureHandler;
    class IZegoExpressEngine;
};
class ZegoCustomVideoCaptureHandler;

class FLUTTER_PLUGIN_EXPORT ZegoCustomVideoCaptureManager{
public:
    static std::shared_ptr<ZegoCustomVideoCaptureManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoCaptureHandler> getHandler();

    ZEGO::EXPRESS::IZegoExpressEngine *GetEngine();
    
    /// Sets the event callback handler for custom video capture.
    ///
    /// Available since: 1.9.0
    /// Description: Example Set the callback for custom video capture. That is, the developer is responsible for collecting video data, and sends the collected video data to the SDK for coding and pushing the video data to the ZEGO RTC server.
    /// Use case: This feature is typically used by developers using third-party beauty features or livestreaming of games.
    /// When to call: After [createEngine] .
    /// Caution: In the case of customized video collection, the SDK will not start the camera to collect video data, and developers need to collect video data from the video collection source. A callback notifying the developer that it can start sending video data is triggered when the developer calls [startPreview] to start pushing the stream to [startPublishingStream]. A callback notifying the developer that it can stop sending video data is triggered when [stopPreview] and [stopPublishingStream].
    /// Related callbacks: [onStart] Starts collection, and [onStop] stops collection.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video capture handler
    void setCustomVideoCaptureHandler(std::shared_ptr<IZegoFlutterCustomVideoCaptureHandler> handler);

    /// Sends the video frames (Raw Data) produced by custom video capture to the SDK (for the specified channel).
    ///
    /// Available since: 1.9.0
    /// Description: Sends customized raw video frame data to the SDK.
    /// When to call: After receiving the [onStart] notification, the developer starts the call after the collection logic starts and ends the call after the [onStop] notification.
    /// Caution: This interface must be called with [enableCustomVideoCapture] passing the parameter type RAW_DATA.
    /// Related APIs: [enableCustomVideoCapture], [setCustomVideoCaptureHandler].
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data video frame data
    /// @param dataLength video frame data length
    /// @param params video frame param
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel
    void
    sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength,
                                  ZGFlutterVideoFrameParam params,
                                  unsigned long long referenceTimeMillisecond,
                                  ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

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
    /// @param dataLength video frame data length
    /// @param params video frame param
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel
    void
    sendCustomVideoCaptureEncodedData(const unsigned char *data, unsigned int dataLength,
                                      ZGFlutterVideoEncodedFrameParam params,
                                      unsigned long long referenceTimeMillisecond,
                                      ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

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
    void
    setCustomVideoCaptureFillMode(ZGFlutterViewMode mode,
                                  ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

    /// Sets device state of custom video capture (for the specified channel).
    ///
    /// Available since: 2.15.0.
    /// Description: User can set the video device state of custom video capture (for the specified channel) through this function.
    /// When to call: After the callback [onStart] is received.
    /// Restrictions: None.
    /// Related Callbacks: The remote user that play stream of local user publishing stream can receive state update notification by monitoring [onRemoteCameraStateUpdate] callbacks.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param isEnable Whether the device is enable.
    /// @param state The device state.
    /// @param channel Publishing stream channel
    void
    setCustomVideoCaptureDeviceState(bool isEnable, ZGFlutterRemoteDeviceState state,
                                     ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

    /// Set the region of interest (ROI) for encoder of custom video capture (for the specified channel).
    ///
    /// Available since: 2.16.0.
    /// Description: You can set the video encoder's region of interest rectangle areas of custom video capture (for the specified channel) through this function.
    /// When to call: After the callback [onStart] is received.
    /// Restrictions: Currently, only certain video encoders support this function, please contact ZEGO technical support before using it.
    /// Caution: This function is currently an experimental feature, please contact ZEGO technical support before using it.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param rectList ROI rectangle area list, currently supports up to 6 areas.
    /// @param channel Publish channel
    void setCustomVideoCaptureRegionOfInterest(
        std::vector<ZGFlutterRoiRect> rectList,
        ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

private:
    friend class ZegoCustomVideoCaptureHandler;
    std::shared_ptr<IZegoFlutterCustomVideoCaptureHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoCaptureHandler> zegoHandler_ = nullptr;
};

#endif  // ZEGO_CUSTOM_VIDEO_CAPTURE_MANAGER_H_
