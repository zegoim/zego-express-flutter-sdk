
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {

  /// Enables or disables acoustic echo cancellation (AEC).
  ///
  /// Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [enable] Whether to enable echo cancellation, true: enable, false: disable
  Future<void> enableAEC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAEC(enable);
  }

  /// Sets the acoustic echo cancellation (AEC) mode.
  ///
  /// Switch different echo cancellation modes to control the extent to which echo data is eliminated.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [mode] Echo cancellation mode
  Future<void> setAECMode(ZegoAECMode mode) async {
    return await ZegoExpressImpl.instance.setAECMode(mode);
  }

  /// Enables or disables automatic gain control (AGC).
  ///
  /// When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [enable] Whether to enable automatic gain control, true: enable, false: disable
  Future<void> enableAGC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAGC(enable);
  }

  /// Enables or disables active noise suppression (ANS, aka ANC).
  ///
  /// Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer.
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [enable] Whether to enable noise suppression, true: enable, false: disable
  Future<void> enableANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableANS(enable);
  }

  /// Enables or disables transient noise suppression.
  ///
  /// Suppress transient noises such as keyboard and desk knocks
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [enable] Whether to enable transient noise suppression, true: enable, false: disable
  Future<void> enableTransientANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableTransientANS(enable);
  }

  /// Sets the automatic noise suppression (ANS) mode.
  ///
  /// Default is medium mode
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [mode] Audio Noise Suppression mode
  Future<void> setANSMode(ZegoANSMode mode) async {
    return await ZegoExpressImpl.instance.setANSMode(mode);
  }

  /// Enables or disables the beauty features (for the specified channel).
  ///
  /// The current beauty function is simple and may not meet the developer's expectations, it is recommended to use [enableCustomVideoCapture] function to connect to a third party professional beauty SDK to get the best results.
  /// The [setBeautifyOption] function can be called to adjust the beauty parameters after the beauty function is enabled.
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
  ///
  /// - [featureBitmask] Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
  /// - [channel] Publishing stream channel
  Future<void> enableBeautify(int featureBitmask, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.enableBeautify(featureBitmask, channel: channel);
  }

  /// Sets up the beauty parameters (for the specified channel).
  ///
  /// Developer need to call [enableBeautify] function first to enable the beautify function before calling this function
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
  ///
  /// - [option] Beauty configuration options
  /// - [channel] Publishing stream channel
  Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
  }

  /// Set the sound equalizer (EQ).
  ///
  /// - [bandIndex] Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
  /// - [bandGain] Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
  Future<void> setAudioEqualizerGain(int bandIndex, double bandGain) async {
    return await ZegoExpressImpl.instance.setAudioEqualizerGain(bandIndex, bandGain);
  }

  /// Setting up the voice changer via preset enumeration.
  ///
  /// Voice changer effect is only effective for the captured sound.
  /// This function is an encapsulated version of [setVoiceChangerParam], which provides some preset values. If you need to configure the voice changer effects, please use [setVoiceChangerParam]
  /// This function is mutually exclusive with [setReverbPreset]. If used at the same time, it will produce undefined effects.
  /// Some enumerated preset will modify the parameters of reverberation or reverberation echo, so after calling this function, calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect.
  /// If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [preset] The voice changer preset enumeration
  Future<void> setVoiceChangerPreset(ZegoVoiceChangerPreset preset) async {
    return await ZegoExpressImpl.instance.setVoiceChangerPreset(preset);
  }

  /// Setting up the specific voice changer parameters.
  ///
  /// Voice changer effect is only effective for the captured sound.
  /// This function is an advanced version of [setVoiceChangerPreset], you can configure the voice changer effect by yourself.
  /// If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] Voice changer parameters
  Future<void> setVoiceChangerParam(ZegoVoiceChangerParam param) async {
    return await ZegoExpressImpl.instance.setVoiceChangerParam(param);
  }

  /// Setting up the reverberation via preset enumeration.
  ///
  /// Support dynamic settings when publishing stream.
  /// This function is a encapsulated version of [setReverbAdvancedParam], which provides some preset values. If you need to configure the reverb, please use [setReverbAdvancedParam]
  /// This function is mutually exclusive with [setVoiceChangerPreset]. If used at the same time, it will produce undefined effects.
  /// If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [preset] The reverberation preset enumeration
  Future<void> setReverbPreset(ZegoReverbPreset preset) async {
    return await ZegoExpressImpl.instance.setReverbPreset(preset);
  }

  /// Setting up the specific reverberation parameters.
  ///
  /// Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
  /// This function is an advanced version of [setReverbPreset], you can configure the reverb effect by yourself.
  /// If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] Reverb advanced parameter
  Future<void> setReverbAdvancedParam(ZegoReverbAdvancedParam param) async {
    return await ZegoExpressImpl.instance.setReverbAdvancedParam(param);
  }

  /// Setting up the specific reverberation echo parameters.
  ///
  /// This function can be used with voice changer and reverb to achieve a variety of custom sound effects
  /// If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] The reverberation echo parameter
  Future<void> setReverbEchoParam(ZegoReverbEchoParam param) async {
    return await ZegoExpressImpl.instance.setReverbEchoParam(param);
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
