
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineRecord on ZegoExpressEngine {

  /// Starts to record locally captured audio or video and directly save the data to a file.
  ///
  /// The recording process should not logoutRoom, if the logoutRoom SDK internally ends the current recording voluntarily.
  /// Currently only one task can be recorded simultaneously.
  /// The call to this api is effective only if the call to [startPreview] or [startPublishingStream] is successful.
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
