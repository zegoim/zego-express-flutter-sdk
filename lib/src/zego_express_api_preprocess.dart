
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {

  /// Enables or disables Acoustic Echo Cancellation (AEC).
  ///
  /// Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream] or [startPreview] to take effect.
  ///
  /// - [enable] Whether to enable echo cancellation, true: enable echo cancellation, false: disable echo cancellation
  Future<void> enableAEC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAEC(enable);
  }

  /// Sets the Acoustic Echo Cancellation (AEC) mode.
  ///
  /// Switch different echo cancellation modes to control the extent to which echo data is eliminated.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream] or [startPreview] to take effect.
  ///
  /// - [mode] Echo cancellation mode
  Future<void> setAECMode(ZegoAECMode mode) async {
    return await ZegoExpressImpl.instance.setAECMode(mode);
  }

  /// Enables or disables Automatic Gain Control (AGC).
  ///
  /// When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream] or [startPreview] to take effect.
  ///
  /// - [enable] Whether to enable automatic gain control, true: enable AGC, false: disable AGC
  Future<void> enableAGC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAGC(enable);
  }

  /// Enables or disables Automatic Noise Suppression (ANS).
  ///
  /// Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream] or [startPreview] to take effect.
  ///
  /// - [enable] Whether to enable noise suppression, true: enable AGC, false: disable AGC
  Future<void> enableANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableANS(enable);
  }

  /// Sets the Automatic Noise Suppression (ANS) mode.
  ///
  /// Default is medium mode
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream] or [startPreview] to take effect.
  ///
  /// - [mode] Audio Noise Suppression mode
  Future<void> setANSMode(ZegoANSMode mode) async {
    return await ZegoExpressImpl.instance.setANSMode(mode);
  }

  /// Enables or disables the beauty features (for the specified channel).
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

  /// Sets up the beauty parameters (for the specified channel).
  ///
  /// Developer need to call [enableBeautify] API first to enable the beautify function before calling this API
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this api is no longer valid.
  ///
  /// - [option] Beauty configuration options
  /// - [channel] Publishing stream channel
  Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
  }

  /// Set the sound equalizer (EQ)
  ///
  /// - [bandIndex] The value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
  /// - [bandGain] The value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
  Future<void> setAudioEqualizerGain(int bandIndex, double bandGain) async {
    return await ZegoExpressImpl.instance.setAudioEqualizerGain(bandIndex, bandGain);
  }

  /// Sets up the voice changer parameters.
  ///
  /// sound change effect only works on capture sounds.
  ///
  /// - [param] Voice changer parameters
  Future<void> setVoiceChangerParam(ZegoVoiceChangerParam param) async {
    return await ZegoExpressImpl.instance.setVoiceChangerParam(param);
  }

  /// Sets up the reverberation parameters.
  ///
  /// Different values set dynamically after a successful publishing will take effect, When all of the parameters is set to 0, the reverb is turned off.
  ///
  /// - [param] Reverb parameter
  Future<void> setReverbParam(ZegoReverbParam param) async {
    return await ZegoExpressImpl.instance.setReverbParam(param);
  }

  /// Enables the virtual stereo feature.
  ///
  /// Note: You need to set up a dual channel setAudioConfig for the virtual stereo to take effect!
  ///
  /// - [enable] true to turn on the virtual stereo, false to turn off the virtual stereo
  /// - [angle] angle of the sound source in the virtual stereo, ranging from 0 to 180, with 90 being the front, and 0 and 180 being respectively Corresponds to rightmost and leftmost, usually use 90.
  Future<void> enableVirtualStereo(bool enable, int angle) async {
    return await ZegoExpressImpl.instance.enableVirtualStereo(enable, angle);
  }

}
