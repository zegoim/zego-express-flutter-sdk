import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'package:zego_express_engine/src/zego_express_api.dart';

import 'zego_express_defines.dart';
import 'zego_express_event_handler.dart';

class ZegoExpressImpl {
    /* Method Channel */
    static const MethodChannel _channel =
    const MethodChannel('plugins.zego.im/zego_express_engine');
    static const EventChannel _event =
    const EventChannel('plugins.zego.im/zego_express_event_handler');

    static ZegoExpressImpl _instance;

    static bool _enablePlatformView = false;

    //static Stream<dynamic> _receiverEvents;
    /// 用于接收native层事件流，开发者无需关注
    static StreamSubscription<dynamic> _streamSubscription;

    ZegoExpressImpl._internal();

    factory ZegoExpressImpl() {
        return _instance;
    }

    static ZegoExpressImpl get instance {
        return _instance;
    }

    static Future<String> getVersion() async {
        final String version = await _channel.invokeMethod('getSDKVersion');
        return version;
    }

    static bool get isUsePlatformView {
        return _enablePlatformView;
    }

    /// 设置是否使用 Platform View 渲染
    ///
    ///@param enable 是否使用，true 使用，false 不使用。默认为 false
    ///@discussion 必须在 [initSDK] 之前调用才会生效
    ///@discussion 参数为 true 时，使用 Platform View 渲染，参数为 false 时，使用 Texture 渲染
    ///@discussion 由于 flutter 团队 对 platform view 仍处于开发阶段，请开发者酌情使用
    static Future<void> enablePlatformView(bool enable) async {
        _enablePlatformView = enable;
        return await _channel.invokeMethod('enablePlatformView', {
            'enable': enable
        });
    }

    static Future<void> createEngine(int appID, String appSign, bool isTestEnv, ZegoScenario scenario) async {
        final int error = await _channel.invokeMethod('createEngine', {
            'appID': appID,
            'appSign': appSign,
            'isTestEnv': isTestEnv,
            'scenario': scenario.index
        });

        if(error == 0) {
            _instance = _instance ?? new ZegoExpressImpl._internal();
            _registerEventHandler();
        }
    }

    static Future<void> destroyEngine() async {
        await _channel.invokeMethod('destroyEngine');
        _unregisterEventHandler();
        if(_instance != null) {
            _instance = null;
        }
    }

    Future<void> uploadLog() async {
        return await _channel.invokeMethod('uploadLog');
    }

    Future<void> setDebugVerbose(bool enable, ZegoLanguage language) async {
        return await _channel.invokeMethod('setDebugVerbose', {
            'enable': enable,
            'language': language.index
        });
    }

    Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config}) async {
        return await _channel.invokeMethod('loginRoom', {
            'roomID': roomID,
            'user': user.toMap(),
            'config': config.toMap(),
        });
    }

    Future<void> logoutRoom(String roomID) async {
        return await _channel.invokeMethod('logoutRoom', {
            'roomID': roomID
        });
    }

    Future<int> createRenderer(int width, int height) async {
        final int textureID = await _channel.invokeMethod('createRenderer', {
            'width': width,
            'height': height
        });

        return textureID;
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
    Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mode) async {
        return await _channel.invokeMethod('setVideoMirrorMode', {
            'mode': mode.index
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

    Future<void> setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) async {
        return await _channel.invokeMethod('setCapturePipelineScaleMode', {
            'mode': mode.index
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
        List<Map<String, dynamic>> objUserList;

        if(toUserList != null) {
            objUserList = new List(toUserList.length);
            for (var user in toUserList) {
                objUserList.add(user.toMap());
            }
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

//  static void Function(String roomID, ZegoRoomState state, int errorCode) onRoomStateUpdate;
//
//  static void Function(String roomID, ZegoUpdateType updateType, List<ZegoUser> userList) onRoomUserUpdate;

    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoStream> streamList) onRoomStreamUpdate;

    static void Function(String roomID, List<ZegoStream> streamList) onRoomStreamExtraInfoUpdate;

    static void Function(String streamID, ZegoPublisherState state, int errorCode) onPublisherStateUpdate;

    static void Function(String streamID, ZegoPublishStreamQuality quality) onPublisherQualityUpdate;

    static void Function(int event) onPublisherRecvFirstFrameEvent;

    static void Function(int width, int height) onPublisherVideoSizeChanged;

    static void Function(String streamID, List<ZegoStreamRelayCDNInfo> relayInfoList) onPublisherRelayCDNStateUpdate;

    static void Function(String streamID, ZegoPlayerState state, int errorCode) onPlayerStateUpdate;

    static void Function(String streamID, ZegoPlayStreamQuality quality) onPlayerQualityUpdate;

    static void Function(String streamID, ZegoPlayerMediaEvent event) onPlayerMediaEvent;

    static void Function(String streamID, int event) onPlayerRecvFirstFrameEvent;

    static void Function(String streamID, int width, int height) onPlayerVideoSizeChanged;

    static void Function(String streamID, Uint8List byteData) onPlayerRecvSEI;

    static void Function(double soundLevel) onCapturedSoundLevelUpdate;

    static void Function(Map<String, double> soundLevels) onRemoteSoundLevelUpdate;

    static void Function(List<double> audioSpectrum) onCapturedAudioSpectrumUpdate;

    static void Function(Map<String, List<double>> audioSpectrums) onRemoteAudioSpectrumUpdate;

    static void Function(String deviceName, int errorCode) onDeviceError;

    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteCameraStateUpdate;

    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteMicStateUpdate;

    static void Function(String roomID, List<ZegoMessageInfo> messageList) onRecvBroadcastMessage;

    static void Function(String roomID, String command, ZegoUser fromUser) onRecvCustomCommand;

    static void _registerEventHandler() async {
        _streamSubscription = _event.receiveBroadcastStream().listen(_eventListener);
    }

    static void _unregisterEventHandler() async {
        await _streamSubscription?.cancel();
        _streamSubscription = null;
    }

    static void _eventListener(dynamic data) {
        final Map<dynamic, dynamic> method = data;
        switch(method['name']) {
            case 'onRoomStateUpdate':
                if (ZegoExpressEngine.onRoomStateUpdate != null) {
                    ZegoExpressEngine.onRoomStateUpdate(method['roomID'], method['state'], method['errorCode']);
                }
                break;
            case 'onRoomUserUpdate':
                break;
            case 'onRoomStreamUpdate':
                break;
            case 'onRoomStreamExtraInfoUpdate':
                break;
            case 'onPublisherStateUpdate':
                break;
            case 'onPublisherQualityUpdate':
                break;
            case 'onPublisherRecvFirstFrameEvent':
                break;
            case 'onPublisherVideoSizeChanged':
                break;
            case 'onPublisherRelayCDNStateUpdate':
                break;
            case 'onPlayerStateUpdate':
                break;
            case 'onPlayerQualityUpdate':
                break;
            case 'onPlayerMediaEvent':
                break;
            case 'onPlayerRecvFirstFrameEvent':
                break;
            case 'onPlayerVideoSizeChanged':
                break;
            case 'onPlayerRecvSEI':
                break;
            case 'onCapturedSoundLevelUpdate':
                break;
            case 'onRemoteSoundLevelUpdate':
                break;
            case 'onCapturedAudioSpectrumUpdate':
                break;
            case 'onRemoteAudioSpectrumUpdate':
                break;
            case 'onDeviceError':
                break;
            case 'onRemoteCameraStateUpdate':
                break;
            case 'onRemoteCameraStateUpdate':
                break;
            case 'onRemoteMicStateUpdate':
                break;
            case 'onRecvBroadcastMessage':
                break;
            case 'onRecvCustomCommand':
                break;
            default:
                break;
        }

    }







}