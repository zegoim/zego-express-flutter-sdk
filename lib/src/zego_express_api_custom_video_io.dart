
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineCustomVideoIO on ZegoExpressEngine {

  /// Enables or disables custom video capture (for the specified channel).
  ///
  /// It must be set before the engine starts, that is, before calling [startPreview], [startPublishing]; and the configuration can be modified after the engine is stopped, that is, after calling [logoutRoom].
  /// When the developer starts the custom capture, it can be set to receive notification of the start and stop of the custom capture by calling [setCustomVideoCaptureHandler].
  ///
  /// - [enable] enable or disable
  /// - [config] custom video capture config
  /// - [channel] publish channel
  Future<void> enableCustomVideoCapture(bool enable, {ZegoCustomVideoCaptureConfig config, ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.enableCustomVideoCapture(enable, config: config, channel: channel);
  }

}
