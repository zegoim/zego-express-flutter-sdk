import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {
  /// Whether to enable acoustic echo cancellation (AEC).
  ///
  /// Available since: 1.1.0
  /// Description: Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
  /// Use case: When you need to reduce the echo to improve the call quality and user experience, you can turn on this feature.
  /// When to call: It needs to be called after [createEngine].
  /// Caution: The AEC function only supports the processing of sounds playbacked through the SDK, such as sounds played by the playing stream, media player, audio effect player, etc. Before this function is called, the SDK automatically determines whether to use AEC. Once this function is called, the SDK does not automatically determine whether to use AEC.
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
  /// Description: When [enableAEC] is used to turn on echo cancellation, it is only turned on when the speaker is used for mobile terminal equipment. Call this function if you need to turn echo cancellation on or off when using the headset.
  /// Use case: It is common when the mobile device is connected to a external sound card as the audio output source. In order to eliminate the echo in this case, you need to call this function to turn on the echo cancellation.
  /// Default value: Android is off by default and iOS is on by default.
  /// When to call: It needs to be called after [createEngine], before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager].
  /// Caution: Turning on echo cancellation will increase the ear return delay. On the iOS platform, the SDK cannot distinguish between the headset and the external sound card. If you use this function to turn off the system echo cancellation when using the headset, the sound played by the external sound card will be collected when the user accesses the external sound card.
  /// Restrictions: None.
  /// Related APIs: When the headset is not used, you can set whether the SDK turns on echo cancellation through [enableAEC].
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
  /// When to call: It needs to be called after [createEngine].
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
  /// When to call: It needs to be called after [createEngine].
  /// Caution: Before this function is called, the SDK automatically determines whether to use AGC. Once this function is called, the SDK does not automatically determine whether to use AGC.
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
  /// When to call: It needs to be called after [createEngine].
  /// Related APIs: This function has a better suppression effect on continuous noise (such as the sound of rain, white noise). If you need to turn on transient noise suppression, please use [enableTransientANS]. And the noise suppression mode can be set by [setANSMode].
  /// Caution: Before this function is called, the SDK automatically determines whether to use ANS. Once this function is called, the SDK does not automatically determine whether to use ANS.
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
  /// When to call: It needs to be called after [createEngine].
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
  /// Default value: When this function is not called, the default automatic noise suppression (ANS) mode is [Medium].
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: The value set by this function is valid only after the noise suppression function is turned on.
  ///
  /// - [mode] Audio Noise Suppression mode
  Future<void> setANSMode(ZegoANSMode mode) async {
    return await ZegoExpressImpl.instance.setANSMode(mode);
  }

  /// Enables or disables speech enhancement.
  ///
  /// Available since: 3.3.0.
  /// Description: Enable the speech enhancement function.
  /// Use case: In the KTV external playback scene, due to 3A damage and distance, the voice is weak or empty or unclear, so it is necessary to enhance the external playback experience through the voice enhancement technology.
  /// Default value: When this function is not called, this is disabled by default.
  /// When to call: It needs to be called after [createEngine] and it Supports dynamic change.
  /// Caution: 1. If this interface is called repeatedly, the settings of the last call are used;
  ///  2. After call [DestroyEngine], the Settings fail.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable speech enhancement, true: enable, false: disable
  /// - [level] Enhancement level; The value range is [0, 10]
  Future<void> enableSpeechEnhance(bool enable, int level) async {
    return await ZegoExpressImpl.instance.enableSpeechEnhance(enable, level);
  }

  /// Enable the Effects beauty environment.
  ///
  /// Available since: 2.16.0
  /// Description: Enable the Effects beauty environment. The SDK uses the specified video frame data type for transmission. The Windows platform only supports video frame raw data, the Apple platform only supports CVPixelBuffer, and the Android platform only supports texture2d.
  /// Use cases: It is often used in scenes such as video calls and live broadcasts.
  /// Default value: When this function is not called, the beauty environment is not activated by default.
  /// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
  /// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
  /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] https://doc-zh.zego.im/article/9556 for best results.
  /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  Future<void> startEffectsEnv() async {
    return await ZegoExpressImpl.instance.startEffectsEnv();
  }

  /// Disable the Effects beauty environment.
  ///
  /// Available since: 2.16.0
  /// Description: Disable the Effects beauty environment.
  /// Use cases: It is often used in scenes such as video calls and live broadcasts.
  /// When to call: Must be called before calling [startPreview], [startPublishingStream]. If you need to modify the configuration, please call [logoutRoom] to log out of the room first, otherwise it will not take effect.
  /// Related APIs: [enableEffectsBeauty] switch beauty, [setEffectsBeautyParam] set beauty parameters.
  /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI ​​Effects SDK [ZegoEffects] for best results.
  /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  Future<void> stopEffectsEnv() async {
    return await ZegoExpressImpl.instance.stopEffectsEnv();
  }

  /// Enables or disables the beauty effect.
  ///
  /// Available since: 2.16.0
  /// Description: Support basic beauty functions, including whiten, rosy, smooth, and sharpen.
  /// Use cases: It is often used in scenes such as video calls and live broadcasts.
  /// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
  /// Default value: When this function is not called, the beauty effect is not enabled by default.
  /// Related APIs: You can call the [setBeautifyOption] function to adjust the beauty parameters.
  /// Caution: This beauty function is the basic function. If it does not meet the expectations of the developer, you can use the custom video pre-processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] docking and constructing the AI Effects SDK [ZegoEffects] for best results.
  /// Restrictions: If this function is used on the Android platform, it only supports 5.0 and above, SDK version 21 and above. Calling in publishing or preview is invalid when using the web platform.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] Whether to enable the beauty effect, true is enabled; false is disabled, and the default is false.
  Future<void> enableEffectsBeauty(bool enable) async {
    return await ZegoExpressImpl.instance.enableEffectsBeauty(enable);
  }

  /// Set beautify param.
  ///
  /// Available since: 2.16.0
  /// Description: Set the beauty parameters, including whiten, rosy, smooth, and sharpen.
  /// Use cases: It is often used in scenes such as video calls and live broadcasts.
  /// When to call: You must call [startEffectsEnv] to enable the beauty environment before calling this function.
  /// Related APIs: You can call [enableEffectsBeauty] to turn on or off the beauty function.
  /// Restrictions: This function only supports Android system 5.0 and above, Android SDK version 21 and above. Calling in publishing or preview is invalid when using the web platform.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [param] Beauty option param.
  Future<void> setEffectsBeautyParam(ZegoEffectsBeautyParam param) async {
    return await ZegoExpressImpl.instance.setEffectsBeautyParam(param);
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
    return await ZegoExpressImpl.instance
        .setAudioEqualizerGain(bandIndex, bandGain);
  }

  /// Setting up the voice changer via preset enumeration.
  ///
  /// Available since: 1.17.0
  /// Description: Call this function to use preset voice changer effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Related APIs:
  /// If you need advanced voice changer effect, please use [setVoiceChangerParam].
  /// The effect of using this function together with [setReverbPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
  /// Using ANDROID/ETHEREAL preset voice changer effect will modify reverberation or reverberation echo parameters. Calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect after use these preset voice changer effect.
  /// If you need advanced reverb/echo/electronic effects/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setElectronicEffects], [setVoiceChangerParam] together.
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
  /// Related APIs:
  /// If you need advanced reverb effect, please use [setReverbAdvancedParam].
  /// The effect of using this function together with [setVoiceChangerPreset] may be different from what is expected. If you need to use it at the same time, it is recommended to enable the voice changer first, and then enable the reverb.
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
  /// Related APIs: If you need advanced reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
  ///
  /// - [param] The reverberation echo parameter.
  Future<void> setReverbEchoParam(ZegoReverbEchoParam param) async {
    return await ZegoExpressImpl.instance.setReverbEchoParam(param);
  }

  /// Enable or disable the virtual stereo effect when publishing stream.
  ///
  /// Available since: 1.10.0; Note: Starting from 2.15.0, the angle parameter supports setting -1 to present a all round virtual stereo effect.
  /// Description: Call this function to enable / disable the virtual stereo effect when publishing stream.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Caution: You need to set up a dual channel with [setAudioConfig] for the virtual stereo to take effect.
  ///
  /// - [enable] true to turn on the virtual stereo, false to turn off the virtual stereo.
  /// - [angle] The angle of the sound source in virtual stereo in the range of -1 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively. In particular, when set to -1, it is all round virtual stereo effects.
  Future<void> enableVirtualStereo(bool enable, int angle) async {
    return await ZegoExpressImpl.instance.enableVirtualStereo(enable, angle);
  }

  /// Enable or disable the virtual stereo effect when playing stream.
  ///
  /// Available since: 2.8.0
  /// Description: Call this function to enable/disable the virtual stereo effect when playing stream.
  /// Use cases: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It needs to be called after [createEngine].
  /// Caution: It can dynamically switch and set angle parameters before and after playing stream. After stopping playing stream, it will automatically reset and disable.
  ///
  /// - [enable] true to turn on the virtual stereo, false to turn off the virtual stereo.
  /// - [angle] The angle of the sound source in virtual stereo in the range of 0 ~ 360, with 90 being directly in front, 0 / 180 / 270 corresponding to the rightmost and leftmost respectively.
  /// - [streamID] Stream ID.
  Future<void> enablePlayStreamVirtualStereo(
      bool enable, int angle, String streamID) async {
    return await ZegoExpressImpl.instance
        .enablePlayStreamVirtualStereo(enable, angle, streamID);
  }

  /// Turn on or off the electronic sound effect.
  ///
  /// Available since: 2.13.0
  /// Description: Call this function to turn on or off the electronic sound effect.
  /// Use cases: Often used in live broadcasting, voice chatroom and sung unaccompanied scenes.
  /// Default value: When this function is not called, the electronic sound effect is not enabled by default.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Caution: When the mode parameter is Harmonic Minor, the tonal parameter does not take effect.
  /// Related APIs: Common electronic sound effect configurations can be set via [setVoiceChangerPreset].
  ///
  /// - [enable] true to turn on the electronic sound effect, false to turn off the electronic sound effect.
  /// - [mode] Mode of Electronic Effects reference.
  /// - [tonal] The starting pitch of an electric tone in a given mode, representing 12 semitones in one octave of the sound, in the range [0, 11].
  Future<void> setElectronicEffects(
      bool enable, ZegoElectronicEffectsMode mode, int tonal) async {
    return await ZegoExpressImpl.instance
        .setElectronicEffects(enable, mode, tonal);
  }

  /// Enable color enhancement.
  ///
  /// Available since: 3.11.0
  /// Description: Call this function to enable or disable color enhancement.
  /// Use cases: Commonly used in video calling, live streaming, and similar scenarios.
  /// Default value: When this function is not called, color enhancement is not enabled by default.
  /// When to call: It needs to be called after [createEngine].
  ///
  /// - [enable] Whether to enable, true: enable, false: disable
  /// - [params] Color enhancement parameters.
  /// - [channel] Publish stream channel.
  Future<void> enableColorEnhancement(
      bool enable, ZegoColorEnhancementParams params,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableColorEnhancement(enable, params, channel: channel);
  }
}
