import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';

import 'zego_express_defines.dart';

class ZegoExpressEngine {
  /* Method Channel */
  static const MethodChannel _channel =
      const MethodChannel('plugins.zego.im/zego_express_engine');
  static const EventChannel _event =
      const EventChannel('plugins.zego.im/zego_express_event_handler');

  static ZegoExpressEngine _instance;
  static Stream<dynamic> _receiverEvents;

  ZegoExpressEngine._internal();

  factory ZegoExpressEngine() {
    return _instance;
  }

  static ZegoExpressEngine get instance {
    return _instance;
  }

  static Future<String> get sdkVersion async {
    final String version = await _channel.invokeMethod('getSDKVersion');
    return version;
  }

  /// 设置是否使用 Platform View 渲染
  ///
  ///@param enable 是否使用，true 使用，false 不使用。默认为 false
  ///@discussion 必须在 [initSDK] 之前调用才会生效
  ///@discussion 参数为 true 时，使用 Platform View 渲染，参数为 false 时，使用 Texture 渲染
  ///@discussion 由于 flutter 团队 对 platform view 仍处于开发阶段，请开发者酌情使用
  static Future<void> enablePlatformView(bool enable) async {
    return await _channel.invokeMethod('enablePlatformView', {
      'enable': enable
    });
  }

  static Future<void> createEngine(int appID, String appSign, bool isTestEnv, int scenario) async {
    final int error = await _channel.invokeMethod('createEngine', {
      'appID': appID,
      'appSign': appSign,
      'isTestEnv': isTestEnv,
      'scenario': scenario
    });

    if(error == 0) {
      _instance = _instance ?? new ZegoExpressEngine._internal();
      _receiverEvents = _receiverEvents ?? _event.receiveBroadcastStream();
    }
  }

  static Future<void> destroyEngine() async {
    await _channel.invokeMethod('destroyEngine');
    if(_instance != null) {
      _instance = null;
    }

    if(_receiverEvents != null) {
      _receiverEvents = null;
    }
  }

  void registerEventHandler({
    Function(String roomID, int state, int errorCode) onRoomStateUpdate,
    Function(String roomID, int updateType, List<ZegoUser> userList) onRoomUserUpdate,
    Function(String roomID, int updateType, List<ZegoStream> streamList) onRoomStreamUpdate,
    Function(String roomID, List<ZegoStream> streamList) onRoomStreamExtraInfoUpdate,
    Function(String streamID, int state, int errorCode) onPublisherStateUpdate,
    Function(String streamID, ZegoPublishStreamQuality quality) onPublisherQualityUpdate,
    Function(int event) onPublisherRecvFirstFrameEvent,
    Function(int width, int height) onPublisherVideoSizeChanged,
    Function(String streamID, List<ZegoStreamRelayCDNInfo> relayInfoList) onPublisherRelayCDNStateUpdate,
    Function(String streamID, int state, int errorCode) onPlayerStateUpdate,
    Function(String streamID, ZegoPlayStreamQuality quality) onPlayerQualityUpdate,
    Function(String streamID, int event) onPlayerMediaEvent,
    Function(String streamID, int event) onPlayerRecvFirstFrameEvent,
    Function(String streamID, int width, int height) onPlayerVideoSizeChanged,
    Function(String streamID, Uint8List byteData) onPlayerRecvSEI,
    Function(double soundLevel) onCapturedSoundLevelUpdate,
    Function(Map<String, double> soundLevels) onRemoteSoundLevelUpdate,
    Function(List<double> audioSpectrum) onCapturedAudioSpectrumUpdate,
    Function(Map<String, List<double>> audioSpectrums) onRemoteAudioSpectrumUpdate,
    Function(String deviceName, int errorCode) onDeviceError,
    Function(String streamID, int state) onRemoteCameraStateUpdate,
    Function(String streamID, int state) onRemoteMicStateUpdate,
    Function(String roomID, List<ZegoMessageInfo> messageList) onRecvBroadcastMessage,
    Function(String roomID, String command, ZegoUser fromUser) onRecvCustomCommand
  }) {
    
  }

  void unregisterEventHandler() {

  }

  Future<void> uploadLog() async {
    return await _channel.invokeMethod('uploadLog');
  }

  Future<void> setDebugVerbose(bool enable, int language) async {
    return await _channel.invokeMethod('setDebugVerbose', {
      'enable': enable,
      'language': language
    });
  }

  Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config, String token}) async {
    return await _channel.invokeMethod('loginRoom', {
      'roomID': roomID,
      'user': user.toMap(),
      'config': config,
      'token': token
    });
  }

  Future<void> logoutRoom(String roomID) async {
    return await _channel.invokeMethod('logoutRoom', {
      'roomID': roomID
    });
  }

  Future<void> startPublishing(String streamID) async {
    return await _channel.invokeMethod('startPublishing', {
      'streamID': streamID
    });
  }

  Future<void> stopPublishing() async {
    return await _channel.invokeMethod('stopPublishing');
  }

  Future<ZegoStreamExtraInfoResult> setStreamExtraInfo(String extraInfo) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod('setStreamExtraInfo', {
      'extraInfo': extraInfo
    });

    return ZegoStreamExtraInfoResult(map['errorCode']);
  }

  Future<void> startPreview(int viewID) async {

  }

  Future<void> stopPreview(int viewID) async {

  }

  Future<void> setVideoConfig(ZegoVideoConfig config) async {
    return await _channel.invokeMethod('setVideoConfig', {
      'config': config.toMap()
    });
  }
  /// 参考 [ZegoVideoMirrorMode]
  Future<void> setVideoMirrorMode(int mode) async {
    return await _channel.invokeMethod('setVideoMirrorMode', {
      'mode': mode
    });
  }

  Future<void> setAppOrientation(DeviceOrientation orientation) async {
    return await _channel.invokeMethod('setAppOrientation', {
      'orientation': orientation.index
    });
  }

  Future<void> setAudioConfig(ZegoAudioConfig config) async {
    return await _channel.invokeMethod('setAudioConfig', {
      'config': config.toMap()
    });
  }

  Future<void> mutePublishStreamAudio(bool mute) async {
    return await _channel.invokeMethod('mutePublishStreamAudio', {
      'mute': mute
    });
  }

  Future<void> mutePublishStreamVideo(bool mute) async {
    return await _channel.invokeMethod('mutePublishStreamVideo', {
      'mute': mute
    });
  }

  Future<void> setCaptureVolume(int volume) async {
    return await _channel.invokeMethod('setCaptureVolume', {
      'volume': volume
    });
  }

  Future<ZegoUpdateCDNURLResult> addPublishCDNURL(String targetURL, String streamID) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod('addPublishCDNURL', {
      'targetURL': targetURL,
      'streamID': streamID
    });
    
    return ZegoUpdateCDNURLResult(map['errorCode']);
  }

  Future<ZegoUpdateCDNURLResult> removePublishCDNUrl(String targetURL, String streamID) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod('removePublishCDNUrl', {
      'targetURL': targetURL,
      'streamID': streamID
    });

    return ZegoUpdateCDNURLResult(map['errorCode']);
  }

  Future<void> setPublishWatermark(ZegoWatermark watermark, bool isPreviewVisible) async {
    return await _channel.invokeMethod('setPublishWatermark', {
      'watermark': watermark.toMap(),
      'isPreviewVisible': isPreviewVisible
    });
  }

  Future<void> sendSEI(Uint8List byteData) async {
    return await _channel.invokeMethod('sendSEI', {
      'byteData': byteData
    });
  }

  Future<void> enableHardwareEncoder(bool enable) async {
    return await _channel.invokeMethod('enableHardwareEncoder', {
      'enable': enable
    });
  }

  Future<void> setCapturePipelineScaleMode(int mode) async {
    return await _channel.invokeMethod('setCapturePipelineScaleMode', {
      'mode': mode
    });
  }

  Future<void> startPlayingStream(String streamID, int viewID) async {

  }

  Future<void> stopPlayingStream(String streamID) async {

  }

  Future<void> setPlayVolume(String streamID, int volume) async {
    return await _channel.invokeMethod('setPlayVolume', {
      'streamID': streamID,
      'volume': volume
    });
  }

  Future<void> mutePlayStreamAudio(bool mute) async {
    return await _channel.invokeMethod('mutePlayStreamAudio', {
      'mute': mute
    });
  }

  Future<void> mutePlayStreamVideo(bool mute) async {
    return await _channel.invokeMethod('mutePlayStreamVideo', {
      'mute': mute
    });
  }

  Future<void> enableHardwareDecoder(bool enable) async {
    return await _channel.invokeMethod('enableHardwareDecoder', {
      'enable': enable
    });
  }

  Future<void> enableCheckPoc(bool enable) async {
    return await _channel.invokeMethod('enableCheckPoc', {
      'enable': enable
    });
  }

  Future<void> enableAEC(bool enable) async {
    return await _channel.invokeMethod('enableAEC', {
      'enable': enable
    });
  }

  Future<void> setAECMode(int mode) async {
    return await _channel.invokeMethod('setAECMode', {
      'mode': mode
    });
  }

  Future<void> enableAGC(bool enable) async {
    return await _channel.invokeMethod('enableAGC', {
      'enable': enable
    });
  }

  Future<void> enableANS(bool enable) async {
    return await _channel.invokeMethod('enableANS', {
      'enable': enable
    });
  }

  Future<void> enableBeautify(int feature) async {
    return await _channel.invokeMethod('enableBeautify', {
      'feature': feature
    });
  }

  Future<void> setBeautifyOption(ZegoBeautifyOption option) async {
    return await _channel.invokeMethod('setBeautifyOption', {
      'option': option.toMap()
    });
  }

  Future<void> muteMicrophone(bool mute) async {
    return await _channel.invokeMethod('muteMicrophone', {
      'mute': mute
    });
  }

  Future<void> muteAudioOutput(bool mute) async {
    return await _channel.invokeMethod('muteAudioOutput', {
      'mute': mute
    });
  }

  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await _channel.invokeMethod('enableAudioCaptureDevice', {
      'enable': enable
    });
  }

  Future<void> enableCamera(bool enable) async {
    return await _channel.invokeMethod('enableCamera', {
      'enable': enable
    });
  }

  Future<void> useFrontCamera(bool enable) async {
    return await _channel.invokeMethod('useFrontCamera', {
      'enable': enable
    });
  }

  Future<void> startSoundLevelMonitor() async {
    return await _channel.invokeMethod('startSoundLevelMonitor');
  }

  Future<void> stopSoundLevelMonitor() async {
    return await _channel.invokeMethod('stopSoundLevelMonitor');
  }

  Future<void> startAudioSpectrumMonitor() async {
    return await _channel.invokeMethod('startAudioSpectrumMonitor');
  }

  Future<void> stopAudioSpectrumMonitor() async {
    return await _channel.invokeMethod('stopAudioSpectrumMonitor');
  }

  Future<ZegoBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod('sendBroadcastMessage', {
      'roomID': roomID,
      'message': message
    });

    return ZegoBroadcastMessageResult(map['errorCode']);
  }

  Future<ZegoCustomCommandResult> sendCustomCommand(String roomID, String command, {List<ZegoUser> toUserList}) async {
    List<Map<String, dynamic>> objUserList = [];
    for(var user in toUserList) {
      objUserList.add(user.toMap());
    }

    final Map<dynamic, dynamic> map = await _channel.invokeMethod('sendCustomCommand', {
      'roomID': roomID,
      'command': command,
      'toUserList': objUserList
    });

    return ZegoCustomCommandResult(map['errorCode']);
  }

  /*Future<void> startMixerTask(ZegoMixerTask task) async {

  }

  Future<void> stopMixerTask() async {

  }

  Future<void> createMediaPlayer() async {

  }

  Future<void> destroyMediaPlayer() async {

  }*/

  void Function(String roomID, int state, int errorCode) _onRoomStateUpdate;

  void Function(String roomID, int updateType, List<ZegoUser> userList) _onRoomUserUpdate;

  void Function(String roomID, int updateType, List<ZegoStream> streamList) _onRoomStreamUpdate;

  void Function(String roomID, List<ZegoStream> streamList) _onRoomStreamExtraInfoUpdate;

  void Function(String streamID, int state, int errorCode) _onPublisherStateUpdate;

  void Function(String streamID, ZegoPublishStreamQuality quality) _onPublisherQualityUpdate;

  void Function(int event) _onPublisherRecvFirstFrameEvent;

  void Function(int width, int height) _onPublisherVideoSizeChanged;

  void Function(String streamID, List<ZegoStreamRelayCDNInfo> relayInfoList) _onPublisherRelayCDNStateUpdate;

  void Function(String streamID, int state, int errorCode) _onPlayerStateUpdate;

  void Function(String streamID, ZegoPlayStreamQuality quality) _onPlayerQualityUpdate;

  void Function(String streamID, int event) _onPlayerMediaEvent;

  void Function(String streamID, int event) _onPlayerRecvFirstFrameEvent;

  void Function(String streamID, int width, int height) _onPlayerVideoSizeChanged;

  void Function(String streamID, Uint8List byteData) _onPlayerRecvSEI;

  void Function(double soundLevel) _onCapturedSoundLevelUpdate;

  void Function(Map<String, double> soundLevels) _onRemoteSoundLevelUpdate;

  void Function(List<double> audioSpectrum) _onCapturedAudioSpectrumUpdate;

  void Function(Map<String, List<double>> audioSpectrums) _onRemoteAudioSpectrumUpdate;

  void Function(String deviceName, int errorCode) _onDeviceError;

  void Function(String streamID, int state) _onRemoteCameraStateUpdate;

  void Function(String streamID, int state) _onRemoteMicStateUpdate;

  void Function(String roomID, List<ZegoMessageInfo> messageList) _onRecvBroadcastMessage;

  void Function(String roomID, String command, ZegoUser fromUser) _onRecvCustomCommand;









  
}