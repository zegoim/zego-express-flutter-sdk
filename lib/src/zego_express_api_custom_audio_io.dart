
import 'dart:typed_data';
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineCustomAudioIO on ZegoExpressEngine {

  /// Enable audio data observering
  ///
  /// It will only be triggered after this function is called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be playing stream, and the sampling rate passed in by calling the [startAudioDataObserver] function does not support 8000Hz, 22050Hz, and 24000Hz.
  ///
  /// - [observerBitMask] The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
  /// - [param] param of audio frame
  Future<void> startAudioDataObserver(int observerBitMask, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance.startAudioDataObserver(observerBitMask, param);
  }

  /// Disable audio data observering
  Future<void> stopAudioDataObserver() async {
    return await ZegoExpressImpl.instance.stopAudioDataObserver();
  }

  /// Enables the custom audio I/O function (for the specified channel).
  ///
  /// It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
  ///
  /// - [enable] Whether to enable custom audio IO, default is false
  /// - [config] Custom audio IO config
  /// - [channel] Specify the publish channel to enable custom audio IO
  Future<void> enableCustomAudioIO(bool enable, ZegoCustomAudioConfig config, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enableCustomAudioIO(enable, config, channel: channel);
  }

  /// Sends AAC audio data produced by custom audio capture to the SDK (for the specified channel).
  ///
  /// - [data] AAC buffer data
  /// - [dataLength] The total length of the buffer data
  /// - [configLength] The length of AAC specific config (Note: The AAC encoded data length is 'encodedLength = dataLength - configLength')
  /// - [referenceTimeMillisecond] The UNIX timestamp of this AAC audio frame in millisecond.
  /// - [param] The param of this AAC audio frame
  /// - [channel] Publish channel for capturing audio frames
  Future<void> sendCustomAudioCaptureAACData(Uint8List data, int dataLength, int configLength, int referenceTimeMillisecond, ZegoAudioFrameParam param, ZegoPublishChannel channel) async {
    return await ZegoExpressImpl.instance.sendCustomAudioCaptureAACData(data, dataLength, configLength, referenceTimeMillisecond, param, channel);
  }

  /// Sends PCM audio data produced by custom audio capture to the SDK (for the specified channel).
  ///
  /// - [data] PCM buffer data
  /// - [dataLength] The total length of the buffer data
  /// - [param] The param of this PCM audio frame
  /// - [channel] Publish channel for capturing audio frames
  Future<void> sendCustomAudioCapturePCMData(Uint8List data, int dataLength, ZegoAudioFrameParam param, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.sendCustomAudioCapturePCMData(data, dataLength, param, channel: channel);
  }

  /// Fetches PCM audio data of the remote stream for custom audio rendering.
  ///
  /// It is recommended to use the system framework to periodically invoke this function to drive audio data rendering
  ///
  /// - [data] A block of memory for storing audio PCM data that requires user to manage the memory block's lifecycle, the SDK will copy the audio frame rendering data to this memory block
  /// - [dataLength] The length of the audio data to be fetch this time (dataLength = duration * sampleRate * channels * 2(16 bit depth i.e. 2 Btye))
  /// - [param] Specify the parameters of the fetched audio frame
  Future<void> fetchCustomAudioRenderPCMData(Uint8List data, int dataLength, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance.fetchCustomAudioRenderPCMData(data, dataLength, param);
  }

}
