
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineRecord on ZegoExpressEngine {

  /// Starts to record locally captured audio or video and directly save the data to a file.
  ///
  /// Currently only one task can be recorded simultaneously.
  /// This function needs to be called after the success of [startPreview] or [startPublishingStream] to be effective.
  /// Developers should not [stopPreview] or [stopPublishingStream] during recording, otherwise the SDK will end the current recording task.
  /// Developers will receive the [onCapturedDataRecordStateUpdate] and the [onCapturedDataRecordProgressUpdate] callback after start recording.
  ///
  /// - [config] Record config
  /// - [channel] Publishing stream channel
  Future<void> startRecordingCapturedData(ZegoDataRecordConfig config, {ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.startRecordingCapturedData(config, channel: channel);
  }

  /// Stops recording locally captured audio or video.
  ///
  /// - [channel] Publishing stream channel
  Future<void> stopRecordingCapturedData({ZegoPublishChannel channel}) async {
    return await ZegoExpressImpl.instance.stopRecordingCapturedData(channel: channel);
  }

}
