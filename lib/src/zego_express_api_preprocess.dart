
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {

  /// Enables or disables acoustic echo cancellation (AEC).
  ///
  /// Available since: 1.1.0
  /// Description: Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
  /// Use case: When you need to reduce the echo to improve the call quality and user experience, you can turn on this feature.
  /// Default value: When this function is not called, echo cancellation is enabled by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Restrictions: None.
  /// Related APIs: Developers can use [enableHeadphoneAEC] to set whether to enable AEC when using headphones, and use [setAECMode] to set the echo cancellation mode.
  ///
  /// - [enable] Whether to enable echo cancellation, true: enable, false: disable
  Future<void> enableAEC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAEC(enable);
  }

  /// Whether to turn on acoustic echo cancellation (AEC) when using the headphone.
  ///
  /// Available since: 1.1.0
  /// Description: When [enableAEC] is used to turn on echo cancellation, it is only turned on when using speakers for mobile devices, and not turned on by default when using headphones. If you need to enable echo cancellation when using headphones, please call this function.
  /// Use case: It is common when the mobile device is connected to a bluetooth speaker as the audio output source. In order to eliminate the echo in this case, you need to call this function to turn on the echo cancellation.
  /// Default value: When this function is not called, this is disabled by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Restrictions: None.
  /// Related APIs: Developers need to use [enableAEC] to set whether the SDK enables echo cancellation before using this function.
  /// Platform differences: Only supports iOS and Android.
  ///
  /// - [enable] Whether to enable, true: enable, false: disable
  Future<void> enableHeadphoneAEC(bool enable) async {
    return await ZegoExpressImpl.instance.enableHeadphoneAEC(enable);
  }

  /// Sets the acoustic echo cancellation (AEC) mode.
  ///
  /// Available since: 1.1.0
  /// Description: When [enableAEC] is used to enable echo cancellation, this function can be used to switch between different echo cancellation modes to control the degree of echo cancellation.
  /// Use case: When the default echo cancellation effect does not meet expectations, this function can be used to adjust the echo cancellation mode.
  /// Default value: When this function is not called, the default echo cancellation mode is [Aggressive].
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Restrictions: The value set by this function is valid only after the echo cancellation function is turned on.
  ///
  /// - [mode] Echo cancellation mode
  Future<void> setAECMode(ZegoAECMode mode) async {
    return await ZegoExpressImpl.instance.setAECMode(mode);
  }

  /// Enables or disables automatic gain control (AGC).
  ///
  /// Available since: 1.1.0
  /// Description: After turning on this function, the SDK can automatically adjust the microphone volume to adapt to near and far sound pickups and keep the volume stable.
  /// Use case: When you need to ensure volume stability to improve call quality and user experience, you can turn on this feature.
  /// Default value: When this function is not called, AGC is enabled by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable automatic gain control, true: enable, false: disable
  Future<void> enableAGC(bool enable) async {
    return await ZegoExpressImpl.instance.enableAGC(enable);
  }

  /// Enables or disables active noise suppression (ANS, aka ANC).
  ///
  /// Available since: 1.1.0
  /// Description: Enable the noise suppression can reduce the noise in the audio data and make the human voice clearer.
  /// Use case: When you need to suppress noise to improve call quality and user experience, you can turn on this feature.
  /// Default value: When this function is not called, ANS is enabled by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Related APIs: This function will not suppress transient noise after it is turned on. If you need to turn on transient noise suppression, please use [enableTransientANS].
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable noise suppression, true: enable, false: disable
  Future<void> enableANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableANS(enable);
  }

  /// Enables or disables transient noise suppression.
  ///
  /// Available since: 1.17.0
  /// Description: Enable the transient noise suppression can suppress the noises such as keyboard and desk knocks.
  /// Use case: When you need to suppress transient noise to improve call quality and user experience, you can turn on this feature.
  /// Default value: When this function is not called, this is disabled by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Related APIs: This function will not suppress normal noise after it is turned on. If you need to turn on normal noise suppression, please use [enableANS].
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable transient noise suppression, true: enable, false: disable
  Future<void> enableTransientANS(bool enable) async {
    return await ZegoExpressImpl.instance.enableTransientANS(enable);
  }

  /// Sets the automatic noise suppression (ANS) mode.
  ///
  /// Available since: 1.1.0
  /// Description: When [enableANS] is used to enable noise suppression, this function can be used to switch between different noise suppression modes to control the degree of noise suppression.
  /// Use case: When the default noise suppression effect does not meet expectations, this function can be used to adjust the noise suppression mode.
  /// Default value: When this function is not called, the default echo cancellation mode is [Medium].
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer].
  /// Restrictions: The value set by this function is valid only after the noise suppression function is turned on.
  ///
  /// - [mode] Audio Noise Suppression mode
  Future<void> setANSMode(ZegoANSMode mode) async {
    return await ZegoExpressImpl.instance.setANSMode(mode);
  }

  /// Enables or disables audio mixing function.
  ///
  /// Available since: 1.9.0
  /// Description: After the audio mixing function is turned on, the SDK will mix the audio data prepared by the developer with the audio data collected by the SDK before publishing.
  /// Use case: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
  /// Default value: When this function is not called, this is disabled by default.
  /// When to call: It needs to be called after [createEngine].
  /// Related APIs: After enabling audio mixing, the developer also needs to call [setAudioMixingHandler] to set the audio mixing callback, so as to provide the SDK with the audio data that needs to be mixed in the [onAudioMixingCopyData] callback.
  /// Caution: After you start audio mixing, the SDK will play the mixed audio on the local (publisher side) by default. If you do not want to play it locally but only on the remote (player side), please call [muteLocalAudioMixing] to set the local audio mixing mute.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable audio mixting, true: enable, false: disable
  Future<void> enableAudioMixing(bool enable) async {
    return await ZegoExpressImpl.instance.enableAudioMixing(enable);
  }

  /// Mute or resume local playback of the mixing audio.
  ///
  /// Available since: 1.9.0
  /// Description: When calling this function to mute the local playback audio mixing, the local (publisher side) will not play the audio provided to the SDK through [onAudioMixingCopyData], but the remote (player side) can still playback it Mix.
  /// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, but the mixed audio only wants to be heard by the remote player, and does not want to be playback locally, you can use this function.
  /// Default value: When this function is not called, the default is not mute, which is false.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  ///
  /// - [mute] Whether to mute local audio mixting, true: mute, false: unmute
  Future<void> muteLocalAudioMixing(bool mute) async {
    return await ZegoExpressImpl.instance.muteLocalAudioMixing(mute);
  }

  /// Sets the audio mixing output volume for either local playback or the stream to published.
  ///
  /// Available since: 1.9.0
  /// Description: After enabling the audio mixing function through [enableAudioMixing], the developer can use this function to set the volume of the mixing audio to be mixed to SDK.
  /// Use case: When developers need to mix their own songs, sound effects or other audio data into the publishing stream, and want to adjust the volume of the mixing audio, you can use this function.
  /// When to call: It needs to be called after [createEngine].
  /// Caution: This function can individually set the mixing audio volume of the local playback or the remote playback.
  /// Restrictions: None.
  ///
  /// - [volume] The audio mixing volume, range from 0 to 200, 100 as default.
  /// - [type] Local playback volume / Remote playback volume
  Future<void> setAudioMixingVolume(int volume, ZegoVolumeType type) async {
    return await ZegoExpressImpl.instance.setAudioMixingVolume(volume, type);
  }

  /// Enables or disables the beauty features for the specified publish channel.
  ///
  /// Available since: 1.1.0
  /// Description: When developers do not have much need for beauty features, they can use this function to set some very simple beauty effects.
  /// When to call: It needs to be called after [createEngine].
  /// Default value: When this function is not called, the beauty feature is not enabled by default.
  /// Related APIs: After turning on the beauty features, you can call the [setBeautifyOption] function to adjust the beauty parameters.
  /// Caution: This beauty feature is very simple and may not meet the developer’s expectations. It is recommended to use the custom video processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] to connect the [ZegoEffects] AI SDK (https://doc-en.zego.im/article/9896) for best results.
  /// Restrictions: In the case of using the custom video capture function, since the developer has handle the video data capturing, the SDK is no longer responsible for the video data capturing, so this function is no longer valid.
  ///
  /// - [featureBitmask] Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
  /// - [channel] Publishing stream channel
  Future<void> enableBeautify(int featureBitmask, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enableBeautify(featureBitmask, channel: channel);
  }

  /// Set beautify option.
  ///
  /// Available since: 1.1.0
  /// Description: set beautify option for specified stream publish channel.
  /// Use cases: Often used in video call, live broadcasting.
  /// When to call: It needs to be called after [createEngine] and [enableBeautify].
  /// Restrictions: None.
  /// Caution: In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, call this function will not take effect.
  ///
  /// - [option] Beautify option.
  /// - [channel] stream publish channel.
  Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
  }

  /// Set the sound equalizer (EQ).
  ///
  /// Available since: 1.12.0
  /// Description: Call this function to set the sound equalizer adjust the tone.
  /// Use cases: Often used in voice chatroom, KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  ///
  /// - [bandIndex] Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
  /// - [bandGain] Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
  Future<void> setAudioEqualizerGain(int bandIndex, double bandGain) async {
    return await ZegoExpressImpl.instance.setAudioEqualizerGain(bandIndex, bandGain);
  }

  /// Setting up the voice changer via preset enumeration.
  ///
  /// Available since: 1.17.0
  /// Description: Call this function to use preset voice changer effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: Voice changer effect is only effective for SDK captured sound.
  /// Related APIs:
  /// If you need advanced voice changer effect, please use [setVoiceChangerParam].
  /// This function is mutually exclusive with [setReverbPreset]. If used at the same time, it will produce undefined effect.
  /// Using ANDROID/ETHEREAL preset voice changer effect will modify reverberation or reverberation echo parameters. Calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect after use these preset voice changer effect.
  /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [preset] The voice changer preset enumeration.
  Future<void> setVoiceChangerPreset(ZegoVoiceChangerPreset preset) async {
    return await ZegoExpressImpl.instance.setVoiceChangerPreset(preset);
  }

  /// Setting up the specific voice changer parameters.
  ///
  /// Available since: 1.10.0
  /// Description: Call this function to set custom voice changer effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: Voice changer effect is only effective for SDK captured sound.
  /// Related APIs:
  /// [setVoiceChangerPreset] provide a set of preset voice changer effects.
  /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] Voice changer parameters.
  Future<void> setVoiceChangerParam(ZegoVoiceChangerParam param) async {
    return await ZegoExpressImpl.instance.setVoiceChangerParam(param);
  }

  /// Setting up the reverberation via preset enumeration.
  ///
  /// Available since: 1.17.0
  /// Description: Call this function to set preset reverb effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine]. Support call this function to change preset reverb effect during publishing stream.
  /// Restrictions: Reverb effect is only effective for SDK captured sound.
  /// Related APIs:
  /// If you need advanced reverb effect, please use [setReverbAdvancedParam].
  /// This function is mutually exclusive with [setVoiceChangerPreset]. If used at the same time, it will produce undefined effects.
  /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [preset] The reverberation preset enumeration.
  Future<void> setReverbPreset(ZegoReverbPreset preset) async {
    return await ZegoExpressImpl.instance.setReverbPreset(preset);
  }

  /// Setting up the specific reverberation parameters.
  ///
  /// Available since: 1.10.0
  /// Description: Call this function to set preset reverb effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: Reverb effect is only effective for SDK captured sound.
  /// Caution: Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
  /// Related APIs:
  /// [setReverbPreset] provide a set of preset reverb effects.
  /// If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] Reverb advanced parameter.
  Future<void> setReverbAdvancedParam(ZegoReverbAdvancedParam param) async {
    return await ZegoExpressImpl.instance.setReverbAdvancedParam(param);
  }

  /// Setting up the specific reverberation echo parameters.
  ///
  /// Available since: 1.17.0
  /// Description: Call this function to set reverb echo effect. This function can be used with voice changer and reverb to achieve a variety of custom sound effects.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: Reverb echo effect is only effective for SDK captured sound.
  /// Related APIs: If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] The reverberation echo parameter.
  Future<void> setReverbEchoParam(ZegoReverbEchoParam param) async {
    return await ZegoExpressImpl.instance.setReverbEchoParam(param);
  }

  /// Enables the virtual stereo feature with publishing stream.
  ///
  /// Available since: 1.10.0
  /// Description: Call this function to enable/disable virtual stereo effect with publishing stream.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: Virtual stereo effect is only effective for SDK captured sound.
  /// Caution: You need to set up a dual channel with [setAudioConfig] for the virtual stereo to take effect.
  ///
  /// - [enable] true to turn on the virtual stereo, false to turn off the virtual stereo.
  /// - [angle] angle of the sound source in the virtual stereo, ranging from 0 to 180, with 90 being the front, and 0 and 180 being respectively Corresponds to rightmost and leftmost, usually use 90.
  Future<void> enableVirtualStereo(bool enable, int angle) async {
    return await ZegoExpressImpl.instance.enableVirtualStereo(enable, angle);
  }

}
