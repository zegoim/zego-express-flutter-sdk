import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineCustomVideoIO on ZegoExpressEngine {
  /// Enables or disables custom video rendering.
  ///
  /// Available since: 1.9.0
  /// Description: When enable is `true`,video custom rendering is enabled; if the value of `false`, video custom rendering is disabled.
  /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
  /// Default value: Custom video rendering is turned off by default when this function is not called.
  /// When to call: After [createEngine], and before calling [startPreview], [startPublishingStream], [startPlayingStream], [createRealTimeSequentialDataManager]. The configuration can only be modified after the engine is stopped, that is, after [logoutRoom] is called.
  /// Caution: Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be called back, you should directly use the captured video frame from the custom video capture source.
  /// Related callbacks: Call [setCustomVideoRenderHandler] to set the callback to get video frame data. [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] enable or disable
  /// - [config] custom video render config
  Future<void> enableCustomVideoRender(
      bool enable, ZegoCustomVideoRenderConfig config) async {
    return await ZegoExpressImpl.instance
        .enableCustomVideoRender(enable, config);
  }

  /// Enables or disables custom video capture (for the specified channel).
  ///
  /// Available since: 1.9.0
  /// Description: If the value of enable is true, the video collection function is enabled. If the value of enable is false, the video collection function is disabled.
  /// Use case: The App developed by the developer uses the beauty SDK of a third-party beauty manufacturer to broadcast non-camera collected data.
  /// Default value: When this function is not called, custom video collection is disabled by default.
  /// When to call: After [createEngine], call [startPreview], [startPublishingStream], [createRealTimeSequentialDataManager], and call [logoutRoom] to modify the configuration.
  /// Caution: Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be triggered, and the developer should directly capture the captured video frame from the custom video capture source.
  /// Related callbacks: When developers to open a custom collection, by calling [setCustomVideoCaptureHandler] can be set up to receive a custom collection start-stop event notification.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] enable or disable
  /// - [config] custom video capture config
  /// - [channel] publish channel
  Future<void> enableCustomVideoCapture(bool enable,
      {ZegoCustomVideoCaptureConfig? config,
      ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableCustomVideoCapture(enable, config: config, channel: channel);
  }

  /// Enables or disables custom video processing, and support specifying the publish channel.
  ///
  /// Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
  /// Description: When the developer opens custom pre-processing, by calling [setCustomVideoProcessHandler] you can set the custom video pre-processing callback.
  /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
  /// Default value: Off by default
  /// When to call: Must be set before calling [startPreview], [startPublishingStream], [createRealTimeSequentialDataManager]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
  /// Restrictions: None.
  /// Related APIs: Call the [setCustomVideoProcessHandler] function to set the callback before custom video processing.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] enable or disable. Required: Yes.
  /// - [config] custom video processing configuration. Required: Yes.Caution: If NULL is passed, the platform default value is used.
  /// - [channel] Publishing stream channel.Required: No.Default value: Main publish channel.
  Future<void> enableCustomVideoProcessing(
      bool enable, ZegoCustomVideoProcessConfig config,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableCustomVideoProcessing(enable, config, channel: channel);
  }
}
