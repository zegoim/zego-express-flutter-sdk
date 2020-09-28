
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineDevice on ZegoExpressEngine {

  /// Mutes or unmutes the microphone.
  ///
  /// This api is used to control whether the collected audio data is used. When the microphone is muted (disabled), the data is collected and discarded, and the microphone is still occupied.
  /// The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this api simply discards the collected data.
  /// If you really want SDK to give up occupy the microphone, you can call the [enableAudioCaptureDevice] function.
  /// Developers who want to control whether to use microphone on the UI should use this function to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice].
  ///
  /// - [mute] Whether to mute (disable) the microphone, true: mute (disable) microphone, false: enable microphone. The default is false.
  Future<void> muteMicrophone(bool mute) async {
    return await ZegoExpressImpl.instance.muteMicrophone(mute);
  }

  /// Checks whether the microphone is muted.
  ///
  /// Can be used with [muteMicrophone], determine whether the microphone is muted.
  ///
  /// - Returns Whether the microphone is muted; true: the microphone is muted; false: the microphone is enable (not muted)
  Future<bool> isMicrophoneMuted() async {
    return await ZegoExpressImpl.instance.isMicrophoneMuted();
  }

  /// Mutes or unmutes the audio output speaker.
  ///
  /// After mute speaker, all the SDK sounds will not play, including playing stream, mediaplayer, etc. But the SDK will still occupy the output device.
  ///
  /// - [mute] Whether to mute (disable) speaker audio output, true: mute (disable) speaker audio output, false: enable speaker audio output. The default value is false
  Future<void> muteSpeaker(bool mute) async {
    return await ZegoExpressImpl.instance.muteSpeaker(mute);
  }

  /// Checks whether the audio output speaker is muted.
  ///
  /// Can be used with [muteSpeaker], determine whether the speaker audio output is muted.
  ///
  /// - Returns Whether the speaker is muted; true: the speaker is muted; false: the speaker is enable (not muted)
  Future<bool> isSpeakerMuted() async {
    return await ZegoExpressImpl.instance.isSpeakerMuted();
  }

  /// Enables or disables the audio capture device.
  ///
  /// This api is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
  /// Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] function is generally recommended.
  ///
  /// - [enable] Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await ZegoExpressImpl.instance.enableAudioCaptureDevice(enable);
  }

  /// Whether to use the built-in speaker to play audio.
  ///
  /// When you choose not to use the built-in speaker to play sound, that is, set to false, the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
  ///
  /// - [enable] Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
  Future<void> setBuiltInSpeakerOn(bool enable) async {
    return await ZegoExpressImpl.instance.setBuiltInSpeakerOn(enable);
  }

  /// Turns on/off the camera (for the specified channel).
  ///
  /// This function is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this api is no longer valid.
  ///
  /// - [enable] Whether to turn on the camera, true: turn on camera, false: turn off camera
  /// - [channel] Publishing stream channel
  Future<void> enableCamera(bool enable, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.enableCamera(enable, channel: channel);
  }

  /// Switches to the front or the rear camera (for the specified channel).
  ///
  /// This function is used to control the front or rear camera
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this api is no longer valid.
  ///
  /// - [enable] Whether to use the front camera, true: use the front camera, false: use the the rear camera. The default value is true
  /// - [channel] Publishing stream channel
  Future<void> useFrontCamera(bool enable, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.useFrontCamera(enable, channel: channel);
  }

  /// Starts sound level monitoring.
  ///
  /// After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
  /// Before entering the room, you can call [startPreview] with this api and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
  /// [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [millisecond] Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
  Future<void> startSoundLevelMonitor({int millisecond}) async {
    return await ZegoExpressImpl.instance.startSoundLevelMonitor(millisecond: millisecond);
  }

  /// Stops sound level monitoring.
  ///
  /// After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
  Future<void> stopSoundLevelMonitor() async {
    return await ZegoExpressImpl.instance.stopSoundLevelMonitor();
  }

  /// Starts audio spectrum monitoring.
  ///
  /// After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
  /// [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [millisecond] Monitoring time period of the audio spectrum, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
  Future<void> startAudioSpectrumMonitor({int millisecond}) async {
    return await ZegoExpressImpl.instance.startAudioSpectrumMonitor(millisecond: millisecond);
  }

  /// Stops audio spectrum monitoring.
  ///
  /// After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
  Future<void> stopAudioSpectrumMonitor() async {
    return await ZegoExpressImpl.instance.stopAudioSpectrumMonitor();
  }

  /// Enables or disables headphone monitoring.
  ///
  /// enable/disable headphone monitor, this setting takes effect when the headset is connected.
  ///
  /// - [enable] Whether to use headphone monitor, true: enable, false: disable
  Future<void> enableHeadphoneMonitor(bool enable) async {
    return await ZegoExpressImpl.instance.enableHeadphoneMonitor(enable);
  }

  /// Sets the headphone monitor volume.
  ///
  /// set headphone monitor volume, this setting takes effect when the headset is connected.
  ///
  /// - [volume] headphone monitor volume, range from 0 to 200, 100 as default
  Future<void> setHeadphoneMonitorVolume(int volume) async {
    return await ZegoExpressImpl.instance.setHeadphoneMonitorVolume(volume);
  }

}
