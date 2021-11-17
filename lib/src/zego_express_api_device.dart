
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineDevice on ZegoExpressEngine {

  /// Mutes or unmutes the microphone.
  ///
  /// This function is used to control whether the collected audio data is used. When the microphone is muted (disabled), the data is collected and discarded, and the microphone is still occupied.
  /// The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this function simply discards the collected data.
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
  /// This function is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
  /// Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] function is generally recommended.
  ///
  /// - [enable] Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await ZegoExpressImpl.instance.enableAudioCaptureDevice(enable);
  }

  /// get current audio route type
  ///
  /// Audio routing refers to the audio output device that an app uses to play audio, and common audio routes are: speakers, handsets, headphones, Bluetooth devices, and so on.
  Future<ZegoAudioRoute> getAudioRouteType() async {
    return await ZegoExpressImpl.instance.getAudioRouteType();
  }

  /// Whether to use the built-in speaker to play audio.
  ///
  /// Audio routing refers to the audio output device that an app uses to play audio, and common audio routes are: speakers, handsets, headphones, Bluetooth devices, and so on.
  /// When you choose not to use the built-in speaker to play sound, that is, set to [false], the SDK will select the currently highest priority audio output device to play the sound according to the system schedule.
  ///
  /// - [defaultToSpeaker] Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
  Future<void> setAudioRouteToSpeaker(bool defaultToSpeaker) async {
    return await ZegoExpressImpl.instance.setAudioRouteToSpeaker(defaultToSpeaker);
  }

  /// Turns on/off the camera (for the specified channel).
  ///
  /// This function is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
  /// In the case of using the custom video capture function, since the developer has taken over the capture of video data, the SDK is no longer responsible for the capture of video data, but this API will still affect the behavior of whether to encode or not. Therefore, when developers use custom video capture, please ensure that the value of this API is true
  ///
  /// - [enable] Whether to turn on the camera, true: turn on camera, false: turn off camera
  /// - [channel] Publishing stream channel
  Future<void> enableCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enableCamera(enable, channel: channel);
  }

  /// Switches to the front or the rear camera (for the specified channel).
  ///
  /// This function is used to control the front or rear camera
  /// In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
  ///
  /// - [enable] Whether to use the front camera, true: use the front camera, false: use the the rear camera. The default value is true
  /// - [channel] Publishing stream channel
  Future<void> useFrontCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.useFrontCamera(enable, channel: channel);
  }

  /// Available since: 2.10.0
  /// Description: Set the camera exposure compensation value.
  /// Trigger: Called after turn on preview [startPreivew].
  ///
  /// - [value] Camera exposure, the value range is [-1,1], -1 tends to darken, 1 tends to brighten
  /// - [channel] Publishing stream channel
  Future<void> setCameraExposureCompensation(double value, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setCameraExposureCompensation(value, channel: channel);
  }

  /// Set the camera zoom factor.
  ///
  /// Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
  ///
  /// - [factor] The zoom factor of the camera, the minimum value is 1.0, and the maximum value is the return value of [getCameraMaxZoomFactor].
  /// - [channel] Publishing stream channel
  Future<void> setCameraZoomFactor(double factor, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setCameraZoomFactor(factor, channel: channel);
  }

  /// Get the maximum zoom factor of the camera.
  ///
  /// This is only available after the camera has been successfully started, and can generally be called when the captured first frame is received, aka [onPublisherCapturedVideoFirstFrame] callback.
  ///
  /// - [channel] Publishing stream channel
  /// - Returns The maximum zoom factor of the camera.
  Future<double> getCameraMaxZoomFactor({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.getCameraMaxZoomFactor(channel: channel);
  }

  /// Starts sound level monitoring. Support enable some advanced feature.
  ///
  /// Available: since 2.10.0
  /// After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
  /// Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
  /// [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [config] Configuration for starts the sound level monitor.
  Future<void> startSoundLevelMonitor({ZegoSoundLevelConfig? config}) async {
    return await ZegoExpressImpl.instance.startSoundLevelMonitor(config: config);
  }

  /// Stops sound level monitoring.
  ///
  /// After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
  Future<void> stopSoundLevelMonitor() async {
    return await ZegoExpressImpl.instance.stopSoundLevelMonitor();
  }

  /// Starts audio spectrum monitoring. Support setting the listening interval.
  ///
  /// After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
  /// [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
  ///
  /// - [millisecond] Monitoring time period of the audio spectrum, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
  Future<void> startAudioSpectrumMonitor({int? millisecond}) async {
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
