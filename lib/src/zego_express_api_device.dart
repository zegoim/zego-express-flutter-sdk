
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineDevice on ZegoExpressEngine {

  /// Whether to mute microphone input
  ///
  /// This api is used to control whether the collected audio data is used. When the microphone is turned off, the data is collected and discarded, and the microphone is still occupied.
  /// The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this api simply discards the collected data.
  /// If you really want SDK to give up occupy the microphone, you can call the [enableAudioCaptureDevice] interface.
  /// Developers who want to control whether to use microphone on the UI should use this interface to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice].
  ///
  /// - [mute] Whether to turn off the microphone, true: turn off microphone, false: turn on microphone. The default is true.
  Future<void> muteMicrophone(bool mute) async {
    return await ZegoExpressImpl.instance.muteMicrophone(mute);
  }

  /// Whether to mute speaker output
  ///
  /// After closing, all the SDK sounds will not play, including playing stream, mediaplayer, etc. But the SDK will still occupy the output device.
  ///
  /// - [mute] Whether to disable audio output to the device, true: disable audio output, false: enable audio output. The default value is false
  Future<void> muteSpeaker(bool mute) async {
    return await ZegoExpressImpl.instance.muteSpeaker(mute);
  }

  /// On/off audio capture device
  ///
  /// This api is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
  /// Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] interface is generally recommended.
  ///
  /// - [enable] Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await ZegoExpressImpl.instance.enableAudioCaptureDevice(enable);
  }

  /// Whether to use the built-in speaker to play sound
  ///
  /// When you choose not to use the built-in speaker to play sound, that is, set to false, the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
  ///
  /// - [enable] Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
  Future<void> setBuiltInSpeakerOn(bool enable) async {
    return await ZegoExpressImpl.instance.setBuiltInSpeakerOn(enable);
  }

  /// On/off camera. You can call this api to set params when publishing another streams
  ///
  /// This interface is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
  ///
  /// - [enable] Whether to turn on the camera, true: turn on camera, false: turn off camera
  /// - [channel] Publishing stream channel.
  Future<void> enableCamera(bool enable, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.enableCamera(enable, channel: channel);
  }

  /// Switch front and rear camera.You can call this api to set params when publishing another streams
  ///
  /// This interface is used to control the front or rear camera
  ///
  /// - [enable] Whether to use the front camera, true: use the front camera, false: use the the rear camera. The default value is true
  /// - [channel] Publishing stream channel.
  Future<void> useFrontCamera(bool enable, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.useFrontCamera(enable, channel: channel);
  }

  /// Start the sound level monitor
  ///
  /// After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
  /// Before entering the room, you can call [startPreview] with this api and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
  /// [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is 100 ms.
  Future<void> startSoundLevelMonitor() async {
    return await ZegoExpressImpl.instance.startSoundLevelMonitor();
  }

  /// Stop the sound level monitor
  ///
  /// After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
  Future<void> stopSoundLevelMonitor() async {
    return await ZegoExpressImpl.instance.stopSoundLevelMonitor();
  }

  /// Start the audio spectrum monitor
  ///
  /// After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
  /// [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is 100 ms.
  Future<void> startAudioSpectrumMonitor() async {
    return await ZegoExpressImpl.instance.startAudioSpectrumMonitor();
  }

  /// Stop the audio spectrum monitor
  ///
  /// After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
  Future<void> stopAudioSpectrumMonitor() async {
    return await ZegoExpressImpl.instance.stopAudioSpectrumMonitor();
  }

}
