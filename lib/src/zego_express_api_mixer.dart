
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineMixer on ZegoExpressEngine {

  /// Starts a stream mixing task.
  ///
  /// Due to the performance considerations of the client device, ZegoExpressEngine's mix stream is to start the mixing stream task on the server side of the ZEGO audio and video cloud for mixing stream.
  /// After calling this api, SDK initiates a mixing stream request to the ZEGO audio and video cloud. The ZEGO audio and video cloud will find the current publishing stream and perform video layer blending according to the parameters of the mixing stream task requested by ZegoExpressEngine.
  /// When you need to update the mixing stream task, that is, the input stream list needs to be updated when the input stream increases or decreases, you can update the field of the [ZegoMixerTask] object inputList and call this api again to pass the same [ZegoMixerTask] object to update the mixing stream task.
  /// If an exception occurs when requesting to start the mixing stream task, for example, the most common mix input stream does not exist, it will be given from the callback error code. For specific error codes, please refer to the Error Codes https://doc-en.zego.im/en/308.html
  /// If an input stream does not exist in the middle, the mixing stream task will automatically retry playing the input stream for 90 seconds, and will not retry after 90 seconds.
  ///
  /// - [task] Mix stream task object
  /// - Returns Start mix stream task result callback notification
  Future<ZegoMixerStartResult> startMixerTask(ZegoMixerTask task) async {
    return await ZegoExpressImpl.instance.startMixerTask(task);
  }

  /// Stops a stream mixing task.
  ///
  /// Similar to [startMixerTask], after calling this api, SDK initiates a request to end the mixing stream task to the ZEGO audio and video cloud server.
  /// If you starts the next mixing stream task without stopping the previous mixing stream task, the previous mixing stream task will not stop automatically. The previous mixing stream task will not be stopped automatically until 90 seconds after the input stream of the previous mixing stream task does not exist.
  /// When using the mixing stream function of the ZEGO audio and video cloud service, you should pay attention to the start of the next mixing stream task, and should stop the previous mixing stream task, so as not to cause the anchor has started the next streaming task and mixing with other anchors, and the audience is still playing the output stream of the previous mixing stream task.
  ///
  /// - [task] Mix stream task object
  /// - Returns Stop mix stream task result callback notification
  Future<ZegoMixerStopResult> stopMixerTask(ZegoMixerTask task) async {
    return await ZegoExpressImpl.instance.stopMixerTask(task);
  }

}
