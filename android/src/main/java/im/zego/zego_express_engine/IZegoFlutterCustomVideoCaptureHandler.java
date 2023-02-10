package im.zego.zego_express_engine;

public interface IZegoFlutterCustomVideoCaptureHandler {
    /**
     * The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
     * 
     * @param channel publish channel, It is consistent with Dart API
     */
    void onStart(ZGFlutterPublishChannel channel);

    /**
     * The callback triggered when SDK stops receiving captured video data.
     *
     * After receiving this callback, the developer is advised to set the saved client object to nil, otherwise the memory occupied by the client object will always exist
     * @param channel publish channel, It is consistent with Dart API
     */
    void onStop(ZGFlutterPublishChannel channel);

    /**
     * When network changes are detected during custom video capture, the developer is notified that traffic control is required, and the encoding configuration is adjusted according to the recommended parameters of the SDK.
     *
     * Available since: 1.14.0
     * Description: When using custom video capture, the SDK detects a network change, informs the developer that it needs to do flow control, and adjust the encoding configuration according to the recommended parameters of the SDK. In the case of custom collection and transmission of encoded data, the SDK cannot know the external encoding configuration, so the flow control operation needs to be completed by the developer. The SDK will notify the developer of the recommended value of the video configuration according to the current network situation, and the developer needs to modify the encoder configuration by himself to ensure the smoothness of video transmission.
     * Use cases: Live data collected by non-cameras. For example, local video file playback, screen sharing, game live broadcast, etc.
     * When to Trigger: When network status changes during the process of custom video capture and flow control is required.
     * Caution: Please do not perform time-consuming operations in this callback, such as reading and writing large files. If you need to perform time-consuming operations, please switch threads.
     * Related APIs: Call [setCustomVideoCaptureHandler] to set custom video capture callback.
     *
     * @param trafficControlInfo traffic control info.
     * @param channel Publishing stream channel.
     */
    void onEncodedDataTrafficControl(ZGFlutterTrafficControlInfo trafficControlInfo,
                                     ZGFlutterPublishChannel channel);
}
