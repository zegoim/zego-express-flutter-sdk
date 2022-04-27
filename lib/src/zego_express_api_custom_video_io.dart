
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineCustomVideoIO on ZegoExpressEngine {

  /// Enables or disables custom video capture (for the specified channel).
  ///
  /// Available since: 1.9.0
  /// Description: If the value of enable is true, the video collection function is enabled. If the value of enable is false, the video collection function is disabled.
  /// Use case: The App developed by the developer uses the beauty SDK of a third-party beauty manufacturer to broadcast non-camera collected data.
  /// Default value: When this function is not called, custom video collection is disabled by default.
  /// When to call: After [createEngine], call [startPreview], [startPublishingStream], [createRealTimeSequentialDataManager], and call [logoutRoom] to modify the configuration.
  /// Caution: Custom video rendering can be used in conjunction with custom video capture, but when both are enabled, the local capture frame callback for custom video rendering will no longer be triggered, and the developer should directly capture the captured video frame from the custom video capture source.
  /// Related callbacks: When developers to open a custom collection, by calling [setCustomVideoCaptureHandler] can be set up to receive a custom collection start-stop event notification.
  ///
  /// - [enable] enable or disable
  /// - [config] custom video capture config
  /// - [channel] publish channel
  Future<void> enableCustomVideoCapture(bool enable, {ZegoCustomVideoCaptureConfig? config, ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enableCustomVideoCapture(enable, config: config, channel: channel);
  }

}
