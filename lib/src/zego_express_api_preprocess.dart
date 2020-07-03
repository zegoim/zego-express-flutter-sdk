
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {

  /// Enables or disables Acoustic Echo Cancellation (AEC).
  ///
  /// Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio. It needs to be set before starting the publish, and the setting is invalid after the start of the publish.
  ///
  /// - [enable] Whether to enable echo cancellation, true: enable echo cancellation, false: disable echo cancellation
  Future<void> enableAEC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAEC(enable);
  }

  /// Sets the Acoustic Echo Cancellation (AEC) mode.
  ///
  /// Switch different echo cancellation modes to control the extent to which echo data is eliminated. Need to be set before starting the publish.
  ///
  /// - [mode] Echo cancellation mode
  Future<void> setAECMode(ZegoAECMode mode) async {
    return await ZegoExpressImpl.instance.setAECMode(mode);
  }

  /// Enables or disables Automatic Gain Control (AGC).
  ///
  /// When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent. Need to be set before starting the publish.
  ///
  /// - [enable] Whether to enable automatic gain control, true: enable AGC, false: disable AGC
  Future<void> enableAGC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAGC(enable);
  }

  /// Enables or disables Automatic Noise Suppression (ANS).
  ///
  /// Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer. Need to be set before starting the publish.
  ///
  /// - [enable] Whether to enable noise suppression, true: enable AGC, false: disable AGC
  Future<void> enableANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableANS(enable);
  }

  /// Enables or disables the beauty features for the specified channel.
  ///
  /// The current beauty function is simple and may not meet the developer's expectations, it is recommended to use [enableCustomVideoCapture] function to connect to a third party professional beauty SDK to get the best results.
  /// The [setBeautifyOption] API can be called to adjust the beauty parameters after the beauty function is enabled.
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this api is no longer valid.
  ///
  /// - [featureBitmask] Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
  /// - [channel] Publishing stream channel
  Future<void> enableBeautify(int featureBitmask, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.enableBeautify(featureBitmask, channel: channel);
  }

  /// Set beauty parameters
  ///
  /// Developer need to call [enableBeautify] API first to enable the beautify function before calling this API
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this api is no longer valid.
  ///
  /// - [option] Beauty configuration options
  /// - [channel] Publishing stream channel
  Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
  }

}
