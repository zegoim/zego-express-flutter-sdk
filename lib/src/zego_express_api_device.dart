
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineDevice on ZegoExpressEngine {

  /// Mutes or unmutes the microphone.
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control whether to use the collected audio data. Mute (turn off the microphone) will use the muted data to replace the audio data collected by the device for streaming. At this time, the microphone device will still be occupied.
  /// Default value: The default is `false`, which means no muting.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: Developers who want to control whether to use microphone on the UI should use this function to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice]. You can use [isMicrophoneMuted] to check if the microphone is muted.
  ///
  /// - [mute] Whether to mute (disable) the microphone, `true`: mute (disable) microphone, `false`: enable microphone.
  Future<void> muteMicrophone(bool mute) async {
    return await ZegoExpressImpl.instance.muteMicrophone(mute);
  }

  /// Checks whether the microphone is muted.
  ///
  /// Available since: 1.1.0
  /// Description: Used to determine whether the microphone is set to mute.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: [muteMicrophone].
  ///
  /// - Returns Whether the microphone is muted; true: the microphone is muted; `false`: the microphone is enable (not muted).
  Future<bool> isMicrophoneMuted() async {
    return await ZegoExpressImpl.instance.isMicrophoneMuted();
  }

  /// Mutes or unmutes the audio output speaker.
  ///
  /// Available since: 1.1.0
  /// Description: After mute speaker, all the SDK sounds will not play, including playing stream, mediaplayer, etc. But the SDK will still occupy the output device.
  /// Default value: The default is `false`, which means no muting.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  ///
  /// - [mute] Whether to mute (disable) speaker audio output, `true`: mute (disable) speaker audio output, `false`: enable speaker audio output.
  Future<void> muteSpeaker(bool mute) async {
    return await ZegoExpressImpl.instance.muteSpeaker(mute);
  }

  /// Checks whether the audio output speaker is muted.
  ///
  /// Available since: 1.1.0
  /// Description: Used to determine whether the audio output is muted.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: [muteSpeaker].
  ///
  /// - Returns Whether the speaker is muted; `true`: the speaker is muted; `false`: the speaker is enable (not muted).
  Future<bool> isSpeakerMuted() async {
    return await ZegoExpressImpl.instance.isSpeakerMuted();
  }

  /// Gets a list of audio devices.
  ///
  /// Only supports desktop.
  ///
  /// - [deviceType] Audio device type
  /// - Returns Audo device List
  Future<List<ZegoDeviceInfo>> getAudioDeviceList(ZegoAudioDeviceType deviceType) async {
    return await ZegoExpressImpl.instance.getAudioDeviceList(deviceType);
  }

  /// Get the device ID of the default audio device.
  ///
  /// Only supports desktop.
  ///
  /// - [deviceType] Audio device type
  /// - Returns Default Audio device ID
  Future<String> getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) async {
    return await ZegoExpressImpl.instance.getDefaultAudioDeviceID(deviceType);
  }

  /// Chooses to use the specified audio device.
  ///
  /// Only supports desktop.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  Future<void> useAudioDevice(ZegoAudioDeviceType deviceType, String deviceID) async {
    return await ZegoExpressImpl.instance.useAudioDevice(deviceType, deviceID);
  }

  /// Get volume for the specified audio device.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  /// - Returns Device volume
  Future<int> getAudioDeviceVolume(ZegoAudioDeviceType deviceType, String deviceID) async {
    return await ZegoExpressImpl.instance.getAudioDeviceVolume(deviceType, deviceID);
  }

  /// Set volume for the specified audio device.
  ///
  /// The direct operating system device may fail due to system restrictions. Please use [setCaptureVolume] and [setPlayVolume] first to adjust the volume of publish and play streams.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  /// - [volume] Device volume
  Future<void> setAudioDeviceVolume(ZegoAudioDeviceType deviceType, String deviceID, int volume) async {
    return await ZegoExpressImpl.instance.setAudioDeviceVolume(deviceType, deviceID, volume);
  }

  /// Enables or disables the audio capture device.
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
  /// Use cases: When the user never needs to use the audio, you can call this function to close the audio collection.
  /// Default value: The default is `false`.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: Turning off or turning on the microphone on the hardware is a time-consuming operation, and there is a certain performance overhead when the user performs frequent operations. [muteMicrophone] is generally recommended.
  ///
  /// - [enable] Whether to enable the audio capture device, `true`: disable audio capture device, `false`: enable audio capture device.
  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await ZegoExpressImpl.instance.enableAudioCaptureDevice(enable);
  }

  /// get current audio route type.
  ///
  /// Available since: 1.1.0
  /// Description: Audio routing refers to the audio output device that an app uses to play audio, and common audio routes are: speakers, handsets, headphones, Bluetooth devices, and so on.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: Set audio route to speaker [setAudioRouteToSpeaker].
  Future<ZegoAudioRoute> getAudioRouteType() async {
    return await ZegoExpressImpl.instance.getAudioRouteType();
  }

  /// Whether to use the built-in speaker to play audio.
  ///
  /// Available since: 1.1.0
  /// Description: Whether to use the speaker to play audio, when you choose not to use the built-in speaker to play the sound, the SDK will select the audio output device with the highest current priority to play the sound according to the system schedule, and common audio routes are: handsets, headphones, Bluetooth devices, and so on.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: Get the current audio route [getAudioRouteType].
  ///
  /// - [defaultToSpeaker] Whether to use the built-in speaker to play sound, `true`: use the built-in speaker to play sound, `false`: use the highest priority audio output device scheduled by the current system to play sound
  Future<void> setAudioRouteToSpeaker(bool defaultToSpeaker) async {
    return await ZegoExpressImpl.instance.setAudioRouteToSpeaker(defaultToSpeaker);
  }

  /// Turns on/off the camera (for the specified channel).
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
  /// Default value: The default is `true` which means the camera is turned on.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Caution: In the case of using the custom video capture function [enableCustomVideoCapture], since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, but this function still affects whether to encode or not. Therefore, when developers use custom video capture, please ensure that the value of this function is `true`.
  ///
  /// - [enable] Whether to turn on the camera, `true`: turn on camera, `false`: turn off camera
  /// - [channel] Publishing stream channel
  Future<void> enableCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enableCamera(enable, channel: channel);
  }

  /// Switches to the front or the rear camera (for the specified channel).
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control the use of the front camera or the rear camera (only supported by Android and iOS).
  /// Default value: The default is `true` which means the front camera is used.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Caution: When the custom video capture function [enableCustomVideoCapture] is turned on, since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, and this function is no longer valid.
  ///
  /// - [enable] Whether to use the front camera, `true`: use the front camera, `false`: use the the rear camera.
  /// - [channel] Publishing stream channel
  Future<void> useFrontCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.useFrontCamera(enable, channel: channel);
  }

  /// Set the camera exposure compensation value.
  ///
  /// Available since: 2.10.0
  /// Description: Set the camera exposure compensation value.
  /// Use cases: User can call this function to set the camera exposure compensation value.
  /// When to call /Trigger: Called this function after calling [startPublishingStream] or [startPreview].
  /// Restrictions: None.
  /// Caution: The setting will be invalid when the camera is restarted.
  /// Platform differences: Only supports iOS and Android.
  ///
  /// - [value] Camera exposure, the value range is [-1,1], the default 0, -1 tends to darken, 1 tends to brighten.
  /// - [channel] Publishing stream channel
  Future<void> setCameraExposureCompensation(double value, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setCameraExposureCompensation(value, channel: channel);
  }

  /// Set the zoom factor of the camera. Every time the camera is restarted, the camera zoom factor will return to the initial value (1.0).
  ///
  /// Available since: 1.20.0
  /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: The settings will not take effect until the camera is started.
  ///
  /// - [factor] The zoom factor of the camera, the minimum value is 1.0, and the maximum value is the return value of [getCameraMaxZoomFactor].
  /// - [channel] Publishing stream channel
  Future<void> setCameraZoomFactor(double factor, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setCameraZoomFactor(factor, channel: channel);
  }

  /// Get the maximum zoom factor of the camera.
  ///
  /// Available since: 1.20.0
  /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
  /// When to call: This is only available after the camera has been successfully started, and can generally be called when the captured first frame is received [onPublisherCapturedVideoFirstFrame] callback.
  /// Restrictions: None.
  ///
  /// - [channel] Publishing stream channel
  /// - Returns The maximum zoom factor of the camera.
  Future<double> getCameraMaxZoomFactor({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.getCameraMaxZoomFactor(channel: channel);
  }

  /// Starts sound level monitoring. Support enable some advanced feature.
  ///
  /// Available since: 2.10.0
  /// Description: After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback. Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
  /// Use cases: During the publishing and playing process, determine who is talking on the wheat and do a UI presentation.
  /// When to call: After the engine is created [createEngine].
  /// Caution: [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [config] Configuration for starts the sound level monitor.
  Future<void> startSoundLevelMonitor({ZegoSoundLevelConfig? config}) async {
    return await ZegoExpressImpl.instance.startSoundLevelMonitor(config: config);
  }

  /// Stops sound level monitoring.
  ///
  /// Available since: 1.1.0
  /// Description: After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
  /// When to call: After the engine is created [createEngine].
  /// Related APIs: Soundwave monitoring can be initiated via [startSoundLevelMonitor].
  Future<void> stopSoundLevelMonitor() async {
    return await ZegoExpressImpl.instance.stopSoundLevelMonitor();
  }

  /// Starts audio spectrum monitoring. Support setting the listening interval.
  ///
  /// Available since: 1.15.0
  /// Description: After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
  /// Use cases: In the host K song scene, has been published or played under the premise that the host or audience to see the tone and volume change animation.
  /// When to call: After the engine is created [createEngine].
  /// Caution: [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [millisecond] Monitoring time period of the audio spectrum, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
  Future<void> startAudioSpectrumMonitor({int? millisecond}) async {
    return await ZegoExpressImpl.instance.startAudioSpectrumMonitor(millisecond: millisecond);
  }

  /// Stops audio spectrum monitoring.
  ///
  /// Available since: 1.1.0
  /// Description: After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
  /// When to call: After the engine is created [createEngine].
  /// Related APIs: Audio spectrum monitoring can be initiated via [startAudioSpectrumMonitor].
  Future<void> stopAudioSpectrumMonitor() async {
    return await ZegoExpressImpl.instance.stopAudioSpectrumMonitor();
  }

  /// Enables or disables headphone monitoring.
  ///
  /// Available since: 1.9.0
  /// Description: Enable/Disable headphone monitor, and users hear their own voices as they use the microphone to capture sounds.
  /// When to call: After the engine is created [createEngine].
  /// Default value: Disable.
  /// Caution: This setting does not actually take effect until both the headset and microphone are connected.
  ///
  /// - [enable] Whether to use headphone monitor, true: enable, false: disable
  Future<void> enableHeadphoneMonitor(bool enable) async {
    return await ZegoExpressImpl.instance.enableHeadphoneMonitor(enable);
  }

  /// Sets the headphone monitor volume.
  ///
  /// Available since: 1.9.0
  /// Description: set headphone monitor volume.
  /// When to call: After the engine is created [createEngine].
  /// Caution: This setting does not actually take effect until both the headset and microphone are connected.
  /// Related APIs: Enables or disables headphone monitoring via [enableHeadphoneMonitor].
  ///
  /// - [volume] headphone monitor volume, range from 0 to 200, 100 as default.
  Future<void> setHeadphoneMonitorVolume(int volume) async {
    return await ZegoExpressImpl.instance.setHeadphoneMonitorVolume(volume);
  }

}
