import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

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
  /// Only supports Windows and Mac.
  ///
  /// - [deviceType] Audio device type
  /// - Returns Audo device List
  Future<List<ZegoDeviceInfo>> getAudioDeviceList(
      ZegoAudioDeviceType deviceType) async {
    return await ZegoExpressImpl.instance.getAudioDeviceList(deviceType);
  }

  /// Get the device ID of the default audio device.
  ///
  /// Only supports Windows and Mac.
  ///
  /// - [deviceType] Audio device type
  /// - Returns Default Audio device ID
  Future<String> getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) async {
    return await ZegoExpressImpl.instance.getDefaultAudioDeviceID(deviceType);
  }

  /// Chooses to use the specified audio device.
  ///
  /// Available since: 1.0.0
  /// Description: Chooses to use the specified audio device.
  /// When to call: After creating the engine [createEngine] and before call [startPublishingStream] or [startPlayingStream].
  /// Restrictions: Only supports Windows/macOS.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  Future<void> useAudioDevice(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await ZegoExpressImpl.instance.useAudioDevice(deviceType, deviceID);
  }


  Future<void> useVideoDevice(
      ZegoVideoDeviceType deviceType, String deviceID) async {
    return await ZegoExpressImpl.instance.useVideoDevice(deviceType, deviceID);
  }
  Future<void> useAudioOutputDevice(String mediaID, String deviceID) async {
    return await ZegoExpressImpl.instance.useAudioOutputDevice(mediaID, deviceID);
  }

  Future<Map<dynamic, dynamic>> enumDevices() async {
    return await ZegoExpressImpl.instance.enumDevices();
  }

  Future<List> getCameras() async {
    return await ZegoExpressImpl.instance.getCameras();
  }
  Future<List> getMicrophones() async {
    return await ZegoExpressImpl.instance.getMicrophones();
  }
  Future<List> getSpeakers() async {
    return await ZegoExpressImpl.instance.getSpeakers();
  }
  /// Get volume for the specified audio device.
  ///
  /// Get volume for the specified audio device. Only for Windows/macOS.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  /// - Returns Device volume
  Future<int> getAudioDeviceVolume(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await ZegoExpressImpl.instance
        .getAudioDeviceVolume(deviceType, deviceID);
  }

  /// Set volume for the specified audio device.
  ///
  /// Only for Windows/macOS. The direct operating system device may fail due to system restrictions. Please use [setCaptureVolume] and [setPlayVolume] first to adjust the volume of publish and play streams.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] ID of a device obtained by [getAudioDeviceList]
  /// - [volume] Device volume
  Future<void> setAudioDeviceVolume(
      ZegoAudioDeviceType deviceType, String deviceID, int volume) async {
    return await ZegoExpressImpl.instance
        .setAudioDeviceVolume(deviceType, deviceID, volume);
  }

  /// Enables or disables the audio capture device.
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control whether to use the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
  /// Use cases: When the user never needs to use the audio, you can call this function to close the audio collection.
  /// Default value: The default is `true`.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Related APIs: Turning off or turning on the microphone on the hardware is a time-consuming operation, and there is a certain performance overhead when the user performs frequent operations. [muteMicrophone] is generally recommended.
  ///
  /// - [enable] Whether to enable the audio capture device, `true`: enable audio capture device, `false`: disable audio capture device.
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
    return await ZegoExpressImpl.instance
        .setAudioRouteToSpeaker(defaultToSpeaker);
  }

  /// Turns on/off the camera (for the specified channel).
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control whether to start the capture of the camera. After the camera is turned off, the video capture will not be performed. At this time, there will be no video data for local preview and push streaming.
  /// Default value: The default is `true` which means the camera is turned on.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Caution: In the case of using the custom video capture function [enableCustomVideoCapture], since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, but this function still affects whether to encode or not. Therefore, when developers use custom video capture, please ensure that the value of this function is `true`.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] Whether to turn on the camera, `true`: turn on camera, `false`: turn off camera
  /// - [channel] Publishing stream channel
  Future<void> enableCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableCamera(enable, channel: channel);
  }

  /// Switches to the front or the rear camera (for the specified channel).
  ///
  /// Available since: 1.1.0
  /// Description: This function is used to control the use of the front camera or the rear camera (only supported by Android and iOS).
  /// Default value: The default is `true` which means the front camera is used.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: None.
  /// Caution: When the custom video capture function [enableCustomVideoCapture] is turned on, since the developer has taken over the video data capture, the SDK is no longer responsible for the video data capture, and this function is no longer valid.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] Whether to use the front camera, `true`: use the front camera, `false`: use the the rear camera.
  /// - [channel] Publishing stream channel
  Future<void> useFrontCamera(bool enable,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .useFrontCamera(enable, channel: channel);
  }

  /// Whether the camera supports focusing.
  ///
  /// Available since: 2.14.0
  /// Description: Whether the camera supports focusing.
  /// Trigger: Called after turn on preview [startPreivew].
  /// Caution: Need to start the camera successfully.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [channel] Publishing stream channel
  /// - Returns Whether to support focus, support is true, not support is false.
  Future<bool> isCameraFocusSupported({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .isCameraFocusSupported(channel: channel);
  }

  /// Set the camera focus mode.
  ///
  /// Available since: 2.14.0
  /// Description: Set the camera focus mode.
  /// Trigger: Called after turn on preview [startPreivew].
  /// Restrictions: Currently only supports iOS and Android platforms.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [mode] focus mode.
  /// - [channel] Publishing stream channel
  Future<void> setCameraFocusMode(ZegoCameraFocusMode mode,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraFocusMode(mode, channel: channel);
  }

  /// Set the focus point in the preview view.
  ///
  /// Available since: 2.14.0
  /// Description: Set the focus point in the preview view. (x, y) are the normalized coordinates in the preview view, that is, the ratio of the position of the focus point relative to the preview view and the width and height of the preview view. The upper left corner is (0, 0).
  /// Trigger: Called after turn on preview [startPreivew].
  /// Restrictions: Currently only supports iOS and Android platforms.
  /// Caution: Every time the camera restarts the acquisition, the settings will become invalid and need to be reset.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [x] Normalized X axis coordinate value, effective value [0,1].
  /// - [y] Normalized Y axis coordinate value, effective value [0,1].
  /// - [channel] Publishing stream channel
  Future<void> setCameraFocusPointInPreview(double x, double y,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraFocusPointInPreview(x, y, channel: channel);
  }

  /// Set the camera exposure mode.
  ///
  /// Available since: 2.14.0
  /// Description: Set the camera exposure mode.
  /// Trigger: Called after turn on preview [startPreivew].
  /// Restrictions: Currently only supports iOS and Android platforms.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [mode] Exposure mode.
  /// - [channel] Publishing stream channel
  Future<void> setCameraExposureMode(ZegoCameraExposureMode mode,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraExposureMode(mode, channel: channel);
  }

  /// Set the exposure point in the preview view.
  ///
  /// Available since: 2.14.0
  /// Description: Set the exposure point in the preview view. (x, y) are the normalized coordinates in the preview view, that is, the ratio of the position of the exposure point relative to the preview view and the width and height of the preview view. The upper left corner is (0, 0).
  /// Trigger: Called after turn on preview [startPreivew].
  /// Restrictions: Currently only supports iOS and Android platforms.
  /// Caution: Every time the camera restarts the acquisition, the settings will become invalid and need to be reset.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [x] Normalized X axis coordinate value, effective value [0,1].
  /// - [y] Normalized Y axis coordinate value, effective value [0,1].
  /// - [channel] Publishing stream channel
  Future<void> setCameraExposurePointInPreview(double x, double y,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraExposurePointInPreview(x, y, channel: channel);
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
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [value] Camera exposure, the value range is [-1,1], the default 0, -1 tends to darken, 1 tends to brighten.
  /// - [channel] Publishing stream channel
  Future<void> setCameraExposureCompensation(double value,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraExposureCompensation(value, channel: channel);
  }

  /// Set the zoom factor of the camera. Every time the camera is restarted, the camera zoom factor will return to the initial value (1.0).
  ///
  /// Available since: 1.20.0
  /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
  /// When to call: After creating the engine [createEngine].
  /// Restrictions: The settings will not take effect until the camera is started.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [factor] The zoom factor of the camera, the minimum value is 1.0, and the maximum value is the return value of [getCameraMaxZoomFactor].
  /// - [channel] Publishing stream channel
  Future<void> setCameraZoomFactor(double factor,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setCameraZoomFactor(factor, channel: channel);
  }

  /// Get the maximum zoom factor of the camera.
  ///
  /// Available since: 1.20.0
  /// Description: Set the camera zoom factor. Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
  /// When to call: This is only available after the camera has been successfully started, and can generally be called when the captured first frame is received [onPublisherCapturedVideoFirstFrame] callback.
  /// Restrictions: None.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [channel] Publishing stream channel
  /// - Returns The maximum zoom factor of the camera.
  Future<double> getCameraMaxZoomFactor({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .getCameraMaxZoomFactor(channel: channel);
  }

  /// Enable camera adaptive frame rate.
  ///
  /// Available since: 2.20.0
  /// Description: After enabling, the SDK matches the capture frame rate range supported by the camera according to the set frame rate range, and dynamically adjusts the capture frame rate of the camera according to the ambient brightness within this range to improve the screen brightness when the set frame rate is too high.
  /// Use cases: The frame rate set by the user on the streaming end is too high, and the ambient lighting is low, so the subject cannot be displayed or recognized normally. For example, live broadcast scenes with high exposure requirements.
  /// When to call: After creating the engine [createEngine], before the camera starts.
  /// Caution: Takes When calling [setVideoConfig] to set the frame rate lower than the expected minimum frame rate, the frame rate value set by [setVideoConfig] will be used. Due to the different hardware and algorithm strategies of different mobile phone manufacturers, the effect of this interface is different on different models or on the front and rear cameras of the same model.
  /// Related APIs: Through [setVideoConfig], you can set the camera capture frame rate and the encoder encoding frame rate.
  ///
  /// - [enable] Whether to enable camera adaptive frame rate. true means on, false means off.Off by default.
  /// - [minFPS] Desired minimum frame rate, 15 recommended. Unit: fps.
  /// - [maxFPS] Desired minimum frame rate, 25 recommended. Unit: fps.
  /// - [channel] Publishing stream channel.
  Future<void> enableCameraAdaptiveFPS(
      bool enable, int minFPS, int maxFPS, ZegoPublishChannel channel) async {
    return await ZegoExpressImpl.instance
        .enableCameraAdaptiveFPS(enable, minFPS, maxFPS, channel);
  }

  /// Starts sound level monitoring. Support enable some advanced feature.
  ///
  /// Available since: 2.10.0
  /// Description: After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback. Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
  /// Use cases: During the publishing and playing process, determine who is talking on the wheat and do a UI presentation.
  /// When to call: After the engine is created [createEngine].
  /// Caution:
  ///   1. [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
  ///   2. After the sound monitoring is started, even if the local audio capture is not started, [onCapturedSoundLevelUpdate] will have a callback, and the sound level is 0.
  ///
  /// - [config] Configuration for starts the sound level monitor.
  Future<void> startSoundLevelMonitor({ZegoSoundLevelConfig? config}) async {
    return await ZegoExpressImpl.instance
        .startSoundLevelMonitor(config: config);
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
    return await ZegoExpressImpl.instance
        .startAudioSpectrumMonitor(millisecond: millisecond);
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
  /// Caution:
  ///   1. This setting does not actually take effect until both the headset and microphone are connected.
  ///   2. The default is to return after acquisition and before pre-processing. If you need to return after pre-processing, please contact ZEGO technical support.
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
  /// - [volume] headphone monitor volume, range from 0 to 200, 60 as default.
  Future<void> setHeadphoneMonitorVolume(int volume) async {
    return await ZegoExpressImpl.instance.setHeadphoneMonitorVolume(volume);
  }

  /// Start audio VAD stable state monitoring, and the monitoring period can be set.
  ///
  /// Available: since 2.14.0
  /// Description: After monitoring is started, you can use the [onAudioVADStateUpdate] callback to receive the specified type of audio VAD callback.
  /// Use cases: For example, when you specify the type of collection and use the microphone to collect, you can check whether the host has continuous and stable voice input through this interface.
  /// When to call: Before publish stream, you can call [startPreview] with this function and combine it with [onAudioVADStateUpdate] callback to determine whether the audio device is working properly.
  /// Restrictions: [onAudioVADStateUpdate] The default callback notification period is 3 seconds.
  /// Related APIs: [stopAudioVADStableStateMonitor].
  ///
  /// - [type] audio VAD monitor type.
  /// - [millisecond] monitoring period
  Future<void> startAudioVADStableStateMonitor(
      ZegoAudioVADStableStateMonitorType type,
      {int? millisecond}) async {
    return await ZegoExpressImpl.instance
        .startAudioVADStableStateMonitor(type, millisecond: millisecond);
  }

  /// Stop audio VAD stable state monitoring.
  ///
  /// Available: since 2.14.0
  /// Description: After calling this interface, the specified type of [onAudioVADStateUpdate] callback can no longer be received.
  /// When to call: None.
  /// Restrictions: None.
  /// Related APIs: [startAudioVADStableStateMonitor].
  ///
  /// - [type] audio VAD monitor type.
  Future<void> stopAudioVADStableStateMonitor(
      ZegoAudioVADStableStateMonitorType type) async {
    return await ZegoExpressImpl.instance.stopAudioVADStableStateMonitor(type);
  }
}
