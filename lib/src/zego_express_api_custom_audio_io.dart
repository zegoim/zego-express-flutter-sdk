import 'dart:typed_data';
import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineCustomAudioIO on ZegoExpressEngine {
  /// Enable local collection and custom audio processing(before ear return).
  ///
  /// Available since: 1.13.0
  /// Description: Enable custom audio processing(before ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
  /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
  /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable local capture custom audio processing.
  /// - [config] Custom audio processing configuration.
  Future<void> enableCustomAudioCaptureProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await ZegoExpressImpl.instance
        .enableCustomAudioCaptureProcessing(enable, config);
  }

  /// Turn on local collection and custom audio processing (after ear return).
  ///
  /// Available since: 1.13.0
  /// Description: Enable custom audio processing(after ear return), developers can receive locally collected audio frames through [onProcessCapturedAudioData], and can modify the audio data.
  /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
  /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable local capture custom audio processing.
  /// - [config] Custom audio processing configuration.
  Future<void> enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await ZegoExpressImpl.instance
        .enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
            enable, config);
  }

  /// Enable feature of throwing audio aux frames which aligned with accompany.
  ///
  /// Available since: 2.22.0
  /// Description: Enable feature of throwing audio aux frames which aligned with accompany, and developers can receive the aligned audio aux frame through [onAlignedAudioAuxData].
  /// Use cases: In KTV scene, this function can be used if the user wants to record voice and accompaniment for free processing.
  /// When to call: It needs to be called after [createEngine] to be effective.
  /// Restrictions: None.
  /// Caution: When throwing onAlignedAudioAuxData audio aux frames is enabled, the streaming data of [startPublishingStream] does not contain audio aux frames.
  ///
  /// - [enable] Whether to enable the feature of throwing alignmented audio aux frames.
  /// - [param] param of audio frame. Currently supports 8k, 16k, 32k, 44.1k, 48k sampling rate, mono or stereo.
  Future<void> enableAlignedAudioAuxData(
      bool enable, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance
        .enableAlignedAudioAuxData(enable, param);
  }

  /// Enable custom audio processing for remote playing stream.
  ///
  /// Available since: 1.13.0
  /// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessRemoteAudioData], and can modify the audio data.
  /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing before pulling the remote audio data for rendering.
  /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable custom audio processing for remote playing stream.
  /// - [config] Custom audio processing configuration.
  Future<void> enableCustomAudioRemoteProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await ZegoExpressImpl.instance
        .enableCustomAudioRemoteProcessing(enable, config);
  }

  /// Enable custom audio processing for SDK playback audio data.
  ///
  /// Available since: 1.13.0
  /// Description: Enable remote streaming custom audio processing, developers can receive remote streaming audio frames through [onProcessRemoteAudioData], and can modify the audio data.
  /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after collecting audio data.
  /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable custom audio processing for SDK playback audio data.
  /// - [config] Custom audio processing configuration.
  Future<void> enableCustomAudioPlaybackProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await ZegoExpressImpl.instance
        .enableCustomAudioPlaybackProcessing(enable, config);
  }

  /// Enable audio data observering.
  ///
  /// Available since: 1.1.0
  /// Description: When custom audio processing is turned on, use this function to turn on audio data callback monitoring.
  /// Use cases: When develop need to monitor the original audio.
  /// When to call: After creating the engine.
  /// Restrictions: Audio monitoring is triggered only after this function has been called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be in the pull stream, and the incoming sampling rate of the [startAudioDataObserver] function is not supported at 8000Hz, 22050Hz, and 24000Hz.
  /// Caution: This api will start the media engine and occupy the microphone device.
  ///
  /// - [observerBitMask] The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
  /// - [param] param of audio frame.
  Future<void> startAudioDataObserver(
      int observerBitMask, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance
        .startAudioDataObserver(observerBitMask, param);
  }

  /// Disable audio data observering.
  ///
  /// Available since: 1.1.0
  /// Description: Disable audio data observering.
  /// Use cases: When develop need to monitor the original audio.
  /// When to call: After calling [startAudioDataObserver] to start audio data monitoring.
  Future<void> stopAudioDataObserver() async {
    return await ZegoExpressImpl.instance.stopAudioDataObserver();
  }

  /// Enables the custom audio I/O function (for the specified channel), support PCM, AAC format data.
  ///
  /// Available since: 1.10.0
  /// Description: Enable custom audio IO function, support PCM, AAC format data.
  /// Use cases: If the developer wants to implement special functions (such as voice change, bel canto, etc.) through custom processing after the audio data is collected or before the remote audio data is drawn for rendering.
  /// When to call: It needs to be called before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer], [createAudioEffectPlayer] and [createRealTimeSequentialDataManager] to be effective.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable custom audio IO, default is false.
  /// - [config] Custom audio IO config.
  /// - [channel] Specify the publish channel to enable custom audio IO.
  Future<void> enableCustomAudioIO(bool enable, ZegoCustomAudioConfig config,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableCustomAudioIO(enable, config, channel: channel);
  }

  /// Sends AAC audio data produced by custom audio capture to the SDK (for the specified channel).
  ///
  /// Available since: 2.20.0
  /// Description: Sends the captured audio AAC data to the SDK.
  /// Use cases: The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission.
  /// When to call: After [enableCustomAudioIO] and publishing stream successfully.
  /// Restrictions: None.
  /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
  ///
  /// - [data] AAC buffer data.
  /// - [dataLength] The total length of the buffer data.
  /// - [configLength] The length of AAC specific config (Note: The AAC encoded data length is 'encodedLength = dataLength - configLength').Value range: [0,64]
  /// - [referenceTimeMillisecond] The UNIX timestamp of this AAC audio frame in millisecond.
  /// - [samples] The number of samples for this AAC audio frame.Value range: [480,512,1024,1960,2048].
  /// - [param] The param of this AAC audio frame.
  /// - [channel] Publish channel for capturing audio frames.
  Future<void> sendCustomAudioCaptureAACData(
      Uint8List data,
      int dataLength,
      int configLength,
      int referenceTimeMillisecond,
      int samples,
      ZegoAudioFrameParam param,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.sendCustomAudioCaptureAACData(data,
        dataLength, configLength, referenceTimeMillisecond, samples, param,
        channel: channel);
  }

  /// Sends PCM audio data produced by custom audio capture to the SDK (for the specified channel).
  ///
  /// Available since: 1.10.0
  /// Description: Sends the captured audio PCM data to the SDK.
  /// Use cases: 1.The customer needs to obtain input after acquisition from the existing audio stream, audio file, or customized acquisition system, and hand it over to the SDK for transmission. 2.Customers have their own requirements for special sound processing for PCM input sources. After the sound processing, the input will be sent to the SDK for transmission.
  /// When to call: After [enableCustomAudioIO] and publishing stream successfully.
  /// Restrictions: None.
  /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the push stream [startPublishingStream].
  ///
  /// - [data] PCM buffer data.
  /// - [dataLength] The total length of the buffer data.
  /// - [param] The param of this PCM audio frame.
  /// - [channel] Publish channel for capturing audio frames.
  Future<void> sendCustomAudioCapturePCMData(
      Uint8List data, int dataLength, ZegoAudioFrameParam param,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.sendCustomAudioCapturePCMData(
        data, dataLength, param,
        channel: channel);
  }

  /// Fetches PCM audio data of the remote stream from the SDK for custom audio rendering.
  ///
  /// Available since: 1.10.0
  /// Description: Fetches PCM audio data of the remote stream from the SDK for custom audio rendering, it is recommended to use the system framework to periodically invoke this function to drive audio data rendering.
  /// Use cases: When developers have their own rendering requirements, such as special applications or processing and rendering of the original PCM data that are pulled, it is recommended to use the custom audio rendering function of the SDK.
  /// When to call: After [enableCustomAudioIO] and playing stream successfully.
  /// Restrictions: None.
  /// Related APIs: Enable the custom audio IO function [enableCustomAudioIO], and start the play stream [startPlayingStream].
  ///
  /// - [data] A block of memory for storing audio PCM data that requires user to manage the memory block's lifecycle, the SDK will copy the audio frame rendering data to this memory block.
  /// - [dataLength] The length of the audio data to be fetch this time (dataLength = duration * sampleRate * channels * 2(16 bit depth i.e. 2 Btye)).
  /// - [param] Specify the parameters of the fetched audio frame. sampleRate in ZegoAudioFrameParam must assignment
  Future<void> fetchCustomAudioRenderPCMData(
      Uint8List data, int dataLength, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance
        .fetchCustomAudioRenderPCMData(data, dataLength, param);
  }
}
