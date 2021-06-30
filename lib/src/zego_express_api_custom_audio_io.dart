import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

class ZegoExpressEngineCustomAudioIO {
  /// Enable audio data observering
  ///
  /// It will only be triggered after this function is called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be playing stream, and the sampling rate passed in by calling the [startAudioDataObserver] function does not support 8000Hz, 22050Hz, and 24000Hz.
  ///
  /// - [observerBitMask] The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
  /// - [param] param of audio frame
  Future<void> startAudioDataObserver(
      int observerBitMask, ZegoAudioFrameParam param) async {
    return await ZegoExpressImpl.instance
        .startAudioDataObserver(observerBitMask, param);
  }

  /// Disable audio data observering
  Future<void> stopAudioDataObserver() async {
    return await ZegoExpressImpl.instance.stopAudioDataObserver();
  }
}
