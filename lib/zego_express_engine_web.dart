// ignore_for_file: prefer_typing_uninitialized_variables

import 'dart:async';
import 'dart:convert';
// In order to *not* need this ignore, consider extracting the "web" version
// of your plugin as a separate package, instead of inlining it in the same
// package as the core of your plugin.
// ignore: avoid_web_libraries_in_flutter
import 'dart:html';
import 'dart:js';
import 'dart:js_util';

import 'package:flutter/services.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'package:zego_express_engine/src/impl/zego_express_impl.dart';
import 'package:zego_express_engine/src/zego_express_defines_web.dart';
import 'package:zego_express_engine/src/utils/web_api.dart';

import 'zego_express_engine.dart';

const MEDIA_PLAYER_NOT_EXIST = 1008001;
ZegoAudioFrameParam getZegoAudioOutputParam(dynamic data) {
  final channelCount = ZegoAudioChannel.values[getProperty(data, 'channel')];

  ZegoAudioSampleRate convertSampleRate(int sampleRate) {
    switch (sampleRate) {
      case 8000:
        return ZegoAudioSampleRate.SampleRate8K;
      case 16000:
        return ZegoAudioSampleRate.SampleRate16K;
      case 22050:
        return ZegoAudioSampleRate.SampleRate22K;
      case 24000:
        return ZegoAudioSampleRate.SampleRate24K;
      case 32000:
        return ZegoAudioSampleRate.SampleRate32K;
      case 44100:
        return ZegoAudioSampleRate.SampleRate44K;
      case 48000:
        return ZegoAudioSampleRate.SampleRate48K;
      default:
        return ZegoAudioSampleRate.Unknown;
    }
  }

  return ZegoAudioFrameParam(
      convertSampleRate(getProperty(data, 'sampleRate')), channelCount);
}

/// A web implementation of the ZegoExpressEngineWeb plugin.
class ZegoExpressEngineWeb {
  dynamic previewView;
  static final StreamController _evenController = StreamController();

  static final dynamic _mediaPlayers = {}; // 存储媒体流实例
  static final dynamic _mediaSources = {}; // 存储屏幕采集实例
  static int _index = 1;
  static void registerWith(Registrar registrar) {
    final MethodChannel channel = MethodChannel(
      'plugins.zego.im/zego_express_engine',
      const StandardMethodCodec(),
      registrar,
    );

    // ignore: unused_local_variable
    final eventChannel = PluginEventChannel(
        'plugins.zego.im/zego_express_event_handler',
        const StandardMethodCodec(),
        registrar);

    final pluginInstance = ZegoExpressEngineWeb();
    channel.setMethodCallHandler(pluginInstance.handleMethodCall);
    eventChannel.setController(ZegoExpressEngineWeb._evenController);

    _evenController.stream.listen((event) {
      _eventListener(event);
    });

    // 获取所有 <script> 标签
    List<Element> scripts = document.querySelectorAll('script');
    // 查找包含 'ZegoExpressWebFlutterWrapper' 的脚本
    final zegoScript = scripts.firstWhere(
      (script) =>
          script.attributes['src']?.contains('ZegoExpressWebFlutterWrapper') ??
          false,
      orElse: () => Element.div(),
    );
    if (zegoScript.tagName.toLowerCase() == 'div') {
      var element = ScriptElement()
        ..src =
            'assets/packages/zego_express_engine/assets/ZegoExpressWebFlutterWrapper.js'
        ..type = 'application/javascript';
      document.body!.append(element);
    }
  }

  /// Handles method calls over the MethodChannel of this plugin.
  /// Note: Check the "federated" architecture for a new way of doing this:
  /// https://flutter.dev/go/federated-plugins
  Future<dynamic> handleMethodCall(MethodCall call) async {
    // var args = <String, dynamic>{};
    //print("*### [sdk] metho ${call.method}");
    switch (call.method) {
      case 'createEngineWithProfile':
        if (call.arguments['profile'] != null &&
            call.arguments['profile']['appSign'] != null) {
          throw PlatformException(
            code: 'Unimplemented',
            details:
                'zego_rtc_engine for web doesn\'t support appSign, Please remove it',
          );
        }
        ZegoFlutterEngine.setEventHandler(
            allowInterop((String event, dynamic data) {
          _evenController.add({'methodName': event, 'data': data});
        }));
        return createEngineWithProfile(call.arguments['profile']);
      case 'destroyEngine':
        return destroyEngine();
      case 'setLogConfig':
        return presetLogConfig(call.arguments['config']);
      case 'getVersion':
        return getVersion();
      case 'setEngineConfig':
        return setEngineConfig(call.arguments['config']);
      case 'setPluginVersion':
        return;
      case 'setRoomScenario':
        return setRoomScenario(call.arguments['scenario']);
      case 'setRoomMode':
        return setRoomMode(call.arguments['mode']);
      case 'setGeoFence':
        return setGeoFence(call.arguments['type'], call.arguments['areaList']);
      case 'setCloudProxyConfig':
        return setCloudProxyConfig(call.arguments['proxyList'],
            call.arguments['token'], call.arguments['enable']);
      // case 'setLocalProxyConfig': // 参数跟native接口有差异，web 需要统一接入、logger 域名，Flutter对外接口需要改声明才能兼容。
      //   return setLocalProxyConfig(
      //       call.arguments['proxyList'], call.arguments['enable']);
      case 'callExperimentalAPI':
        return callExperimentalAPI(call.arguments['params']);
      //////////////////////////////房间相关//////////////////////////////////
      case 'loginRoom':
        return loginRoom(call.arguments['roomID'], call.arguments['user'],
            call.arguments['config']);
      case 'logoutRoom':
        return logoutRoom(call.arguments['roomID']);
      case 'renewToken':
        return renewToken(call.arguments['roomID'], call.arguments['token']);
      case 'setRoomExtraInfo':
        return setRoomExtraInfo(call.arguments['roomID'], call.arguments['key'],
            call.arguments['value']);
      case 'switchRoom':
        return switchRoom(call.arguments['fromRoomID'],
            call.arguments['toRoomID'], call.arguments['config']);
      //////////////////////////////推流相关//////////////////////////////////
      case 'startPublishingStream':
        return startPublishingStream(call.arguments["streamID"],
            call.arguments["config"], call.arguments["channel"]);
      case 'stopPublishingStream':
        return stopPublishingStream(call.arguments["channel"]);
      case 'setStreamExtraInfo':
        return setStreamExtraInfo(
            call.arguments['extraInfo'], call.arguments['channel']);
      case 'setVideoConfig':
        return setVideoConfig(
            call.arguments["config"], call.arguments["channel"]);
      case 'getVideoConfig':
        return getVideoConfig(call.arguments["channel"]);
      case 'setAudioConfig':
        return setAudioConfig(
            call.arguments["config"], call.arguments["channel"]);
      case 'getAudioConfig':
        return getAudioConfig(call.arguments["channel"]);
      case 'setVideoMirrorMode':
        return setVideoMirrorMode(
            call.arguments["mirrorMode"], call.arguments["channel"]);
      case 'startPreview':
        return startPreview(
            call.arguments['canvas'], call.arguments["channel"]);
      case 'stopPreview':
        return stopPreview(call.arguments["channel"]);
      case 'mutePublishStreamVideo':
        return mutePublishStreamVideo(
            call.arguments["mute"], call.arguments["channel"]);
      case 'mutePublishStreamAudio':
        return mutePublishStreamAudio(
            call.arguments["mute"], call.arguments["channel"]);
      case 'setStreamAlignmentProperty':
        return setStreamAlignmentProperty(
            call.arguments["alignment"], call.arguments["channel"]);
      case 'enableTrafficControl':
        return enableTrafficControl(
            call.arguments["enable"], call.arguments["channel"]);
      case 'setMinVideoBitrateForTrafficControl':
        return setMinVideoBitrateForTrafficControl(call.arguments["bitrate"],
            call.arguments["mode"], call.arguments["channel"]);
      case 'setTrafficControlFocusOn':
        return setTrafficControlFocusOn(
            call.arguments["mode"], call.arguments["channel"]);
      case 'muteMicrophone':
        return muteMicrophone(call.arguments["mute"]);
      case 'isMicrophoneMuted':
        return isMicrophoneMuted();
      case 'setSEIConfig':
        return setSEIConfig(call.arguments['config']);
      case 'sendSEI':
        return sendSEI(call.arguments['data'], call.arguments['dataLength'],
            call.arguments['channel']);
      case 'setLowlightEnhancement':
        return setLowlightEnhancement(
            call.arguments['mode'], call.arguments['channel']);
      case 'setVideoSource':
        return setVideoSource(call.arguments['source'],
            call.arguments['instanceID'], call.arguments['channel']);
      case 'setAudioSource':
        return setAudioSource(
            call.arguments['source'], call.arguments['channel']);
      case 'enableHardwareEncoder':
        return enableHardwareEncoder(call.arguments['enable']);
      case 'addPublishCdnUrl':
        return addPublishCdnUrl(
            call.arguments['streamID'], call.arguments['targetURL']);
      case 'removePublishCdnUrl':
        return removePublishCdnUrl(
            call.arguments['streamID'], call.arguments['targetURL']);
      //////////////////////////////拉流相关//////////////////////////////////
      case 'startPlayingStream':
        return startPlayingStream(call.arguments["streamID"],
            call.arguments["canvas"], call.arguments["config"]);
      case 'stopPlayingStream':
        return stopPlayingStream(call.arguments["streamID"]);
      case 'setPlayVolume':
        return setPlayVolume(
            call.arguments["streamID"], call.arguments["volume"]);
      case 'setAllPlayStreamVolume':
        return setAllPlayStreamVolume(call.arguments["volume"]);
      case 'mutePlayStreamAudio':
        return mutePlayStreamAudio(
            call.arguments['streamID'], call.arguments['mute']);
      case 'mutePlayStreamVideo':
        return mutePlayStreamVideo(
            call.arguments['streamID'], call.arguments['mute']);
      case 'muteAllPlayAudioStreams':
        return muteAllPlayAudioStreams(call.arguments['mute']);
      case 'muteAllPlayVideoStreams':
        return muteAllPlayVideoStreams(call.arguments['mute']);
      case 'updatePlayingCanvas':
        return updatePlayingCanvas(
            call.arguments["streamID"], call.arguments["canvas"]);
      // case 'enableHardwareDecoder':
      //   return enableHardwareDecoder(call.arguments["enable"]);
      //////////////////////////////消息相关//////////////////////////////////
      case 'sendBroadcastMessage':
        return sendBroadcastMessage(
            call.arguments["roomID"], call.arguments["message"]);
      case 'sendBarrageMessage':
        return sendBarrageMessage(
            call.arguments['roomID'], call.arguments['message']);
      case 'sendCustomCommand':
        return sendCustomCommand(call.arguments['roomID'],
            call.arguments['command'], call.arguments['toUserList']);
      case 'createRealTimeSequentialDataManager':
        return createRealTimeSequentialDataManager(call.arguments['roomID']);
      case 'destroyRealTimeSequentialDataManager':
        return destroyRealTimeSequentialDataManager(call.arguments['index']);
      case 'dataManagerStartBroadcasting':
        return startBroadcasting(
            call.arguments['index'], call.arguments['streamID']);
      case 'dataManagerStopBroadcasting':
        return stopBroadcasting(
            call.arguments['index'], call.arguments['streamID']);
      case 'dataManagerStartSubscribing':
        return startSubscribing(
            call.arguments['index'], call.arguments['streamID']);
      case 'dataManagerStopSubscribing':
        return stopSubscribing(
            call.arguments['index'], call.arguments['streamID']);
      case 'dataManagerSendRealTimeSequentialData':
        return sendRealTimeSequentialData(call.arguments['index'],
            call.arguments['data'], call.arguments['streamID']);
      //////////////////////////////混流相关//////////////////////////////////
      case 'startMixerTask':
        return startMixerTask(call.arguments);
      case 'stopMixerTask':
        return stopMixerTask(call.arguments["taskID"]);
      case 'startAutoMixerTask':
        return startAutoMixerTask(call.arguments);
      case 'stopAutoMixerTask':
        return stopAutoMixerTask(call.arguments);
      //////////////////////////////前处理相关//////////////////////////////////
      case 'setCaptureVolume':
        return setCaptureVolume(call.arguments["volume"]);
      case 'enableAEC':
        return enableAEC(call.arguments["enable"]);
      case 'enableAGC':
        return enableAGC(call.arguments["enable"]);
      case 'enableANS':
        return enableANS(call.arguments["enable"]);
      case 'startEffectsEnv':
        return startEffectsEnv();
      case 'stopEffectsEnv':
        return stopEffectsEnv();
      case 'enableEffectsBeauty':
        return enableEffectsBeauty(call.arguments['enable']);
      case 'setEffectsBeautyParam':
        return setEffectsBeautyParam(call.arguments['param']);
      // case 'setVoiceChangerPreset':
      //   return setVoiceChangerPreset(call.arguments['preset']);
      // case 'setVoiceChangerParam':
      //   return setVoiceChangerPreset(call.arguments['pitch']);
      // case 'setReverbPreset':
      //   return setReverbPreset(call.arguments['prese']);
      // case 'enableVirtualStereo':
      //   return enableVirtualStereo(
      //       call.arguments['enable'], call.arguments['angle']);
      //////////////////////////////设备管理相关/////////////////////////////////
      case 'enableCamera':
        return enableCamera(
            call.arguments["enable"], call.arguments["channel"]);
      case 'useFrontCamera':
        return useFrontCamera(
            call.arguments["enable"], call.arguments["channel"]);
      case 'destroyPlatformView':
        return destroyPlatformView(call.arguments["viewID"]);
      case 'getAudioDeviceList':
        return getAudioDeviceList(call.arguments['type']);
      case 'getVideoDeviceList':
        return getVideoDeviceList();
      case 'useVideoDevice':
        return useVideoDevice(
            call.arguments['deviceID'], call.arguments['channel']);
      case 'useAudioDevice':
        return useAudioDevice(
            call.arguments['type'], call.arguments['deviceID']);
      case 'useAudioOutputDevice':
        return useAudioOutputDevice(
            call.arguments['mediaID'], call.arguments['deviceID']);
      case 'startSoundLevelMonitor':
        return startSoundLevelMonitor(call.arguments['config']);
      case 'stopSoundLevelMonitor':
        return stopSoundLevelMonitor();
      case 'getScreenCaptureSources':
        return;
      case 'createScreenCaptureSource':
        return createScreenCaptureSource(
            call.arguments['sourceId'], call.arguments['sourceType']);
      case 'destroyScreenCaptureSource':
        return destroyScreenCaptureSource(call.arguments['index']);
      case 'startCaptureScreenCaptureSource':
        return startCaptureScreen(
            call.arguments['index'], call.arguments['config']);
      case 'stopCaptureScreenCaptureSource':
        return stopCaptureScreen(call.arguments['index']);
      //////////////////////////////播放器相关/////////////////////////////////
      case 'createMediaPlayer':
        return createMediaPlayer();
      case 'mediaPlayerSetPlayerCanvas':
        return mediaPlayerSetPlayerCanvas(
            call.arguments['index'], call.arguments['canvas']);
      case 'mediaPlayerLoadResource':
        return mediaPlayerLoadResource(
            call.arguments['index'], call.arguments['path']);
      case 'meidaPlayerLoadResourceWithPosition':
        return mediaPlayerLoadResource(call.arguments['index'],
            call.arguments['path'], call.arguments['startPosition']);
      case 'mediaPlayerLoadResourceFromMediaData':
        return mediaPlayerLoadResourceFromMediaData(call.arguments['index'],
            call.arguments['mediaData'], call.arguments['startPosition']);
      case 'mediaPlayerLoadCopyrightedMusicResourceWithPosition':
        return mediaPlayerLoadCopyrightedMusicResourceWithPosition(
            call.arguments['index'],
            call.arguments['resourceID'],
            call.arguments['startPosition']);
      case 'mediaPlayerSetAudioTrackIndex':
        return mediaPlayerSetAudioTrackIndex(
            call.arguments['index'], call.arguments['trackIndex']);
      case 'mediaPlayerEnableRepeat':
        return mediaPlayerEnableRepeat(
            call.arguments['index'], call.arguments['enable']);
      case 'mediaPlayerStart':
        return mediaPlayerStart(call.arguments['index']);
      case 'mediaPlayerPause':
        return mediaPlayerPause(call.arguments['index']);
      case 'mediaPlayerStop':
        return mediaPlayerStop(call.arguments['index']);
      case 'mediaPlayerResume':
        return mediaPlayerResume(call.arguments['index']);
      case 'mediaPlayerSetPlaySpeed':
        return mediaPlayerSetPlaySpeed(
            call.arguments['index'], call.arguments['speed']);
      case 'mediaPlayerMuteLocal':
        return mediaPlayerMuteLocal(
            call.arguments['index'], call.arguments['mute']);
      case 'mediaPlayerEnableAux':
        return mediaPlayerEnableAux(
            call.arguments['index'], call.arguments['enable']);
      case 'destroyMediaPlayer':
        return destroyMediaPlayer(call.arguments['index']);
      case 'mediaPlayerSetVolume':
        return mediaPlayerSetVolume(
            call.arguments['index'], call.arguments['volume']);
      case 'mediaPlayerGetTotalDuration':
        return mediaPlayerGetTotalDuration(call.arguments['index']);
      case 'mediaPlayerSeekTo':
        return mediaPlayerSeekTo(
            call.arguments['index'], call.arguments['millisecond']);
      //////////////////////////////版权音乐相关/////////////////////////////////
      case 'createCopyrightedMusic':
        return createCopyrightedMusic();
      case 'destroyCopyrightedMusic':
        return destroyCopyrightedMusic();
      case 'copyrightedMusicInitCopyrightedMusic':
        return copyrightedMusicInitCopyrightedMusic(call.arguments["config"]);
      case 'copyrightedMusicSendExtendedRequest':
        return copyrightedMusicSendExtendedRequest(
            call.arguments["command"], call.arguments["params"]);
      case 'copyrightedMusicRequestResource':
        return copyrightedMusicRequestResource(
            call.arguments["config"], call.arguments["type"]);
      case 'copyrightedMusicRequestResourceV2':
        return copyrightedMusicRequestResourceV2(call.arguments["config"]);
      case 'copyrightedMusicGetSharedResource':
        return copyrightedMusicGetSharedResource(
            call.arguments["config"], call.arguments["type"]);
      case 'copyrightedMusicGetSharedResourceV2':
        return copyrightedMusicGetSharedResourceV2(call.arguments["config"]);
      case 'copyrightedMusicGetLrcLyric':
        return copyrightedMusicGetLrcLyric(
            call.arguments["songID"], call.arguments["vendorID"]);
      case 'copyrightedMusicGetKrcLyricByToken':
        return copyrightedMusicGetKrcLyricByToken(call.arguments["krcToken"]);
      case 'copyrightedMusicDownload':
        return copyrightedMusicDownload(call.arguments["resourceID"]);
      case 'copyrightedMusicClearCache':
        return copyrightedMusicClearCache();
      case 'copyrightedMusicGetStandardPitch':
        return copyrightedMusicGetStandardPitch(call.arguments["resourceID"]);
      case 'copyrightedMusicGetCurrentPitch':
        return copyrightedMusicGetCurrentPitch(call.arguments["resourceID"]);
      case 'copyrightedMusicSetScoringLevel':
        return copyrightedMusicSetScoringLevel(call.arguments["level"]);
      case 'copyrightedMusicStartScore':
        return copyrightedMusicStartScore(call
            .arguments["resourceID"]); // ,call.arguments["pitchValueInterval"]
      case 'copyrightedMusicStopScore':
        return copyrightedMusicStopScore(call.arguments["resourceID"]);
      case 'copyrightedMusicPauseScore':
        return copyrightedMusicPauseScore(call.arguments["resourceID"]);
      case 'copyrightedMusicResumeScore':
        return copyrightedMusicResumeScore(call.arguments["resourceID"]);
      case 'copyrightedMusicGetPreviousScore':
        return copyrightedMusicGetPreviousScore(call.arguments["resourceID"]);
      case 'copyrightedMusicGetAverageScore':
        return copyrightedMusicGetAverageScore(call.arguments["resourceID"]);
      case 'copyrightedMusicGetTotalScore':
        return copyrightedMusicGetTotalScore(call.arguments["resourceID"]);
      case 'copyrightedMusicGetFullScore':
        return copyrightedMusicGetFullScore(call.arguments["resourceID"]);
      case 'takePlayStreamSnapshot':
        return takePlayStreamSnapshot(call.arguments['streamID']);
      case 'startAudioDataObserver':
        return startAudioDataObserver(
            call.arguments["observerBitMask"], call.arguments["param"]);
      case 'stopAudioDataObserver':
        return stopAudioDataObserver();
      default:
        throw PlatformException(
          code: 'Unimplemented',
          details:
              'zego_rtc_engine for web doesn\'t implement \'${call.method}\'',
        );
    }
  }

  /// Returns a [String] containing the version of the platform.
  Future<String> getVersion() {
    return Future.value(ZegoFlutterEngine.getVersion());
  }

  static createCompleter() {
    Completer completer = Completer();
    return {
      'completer': completer,
      "success": allowInterop((res) {
        completer.complete(res);
      }),
      "fail": allowInterop((err) {
        completer.completeError(err);
      })
    };
  }

  static void _eventListener(dynamic event) {
    final Map<dynamic, dynamic> map = event;
    var methodName = map["methodName"];
    switch (methodName) {
      ////////////////////////////////////房间相关/////////////////////////////////////
      case "onRoomStateUpdate":
        if (ZegoExpressEngine.onRoomStateUpdate == null) return;

        final data = jsonDecode(map["data"]);
        var state, extendedData;

        switch (data["state"]) {
          case "DISCONNECTED":
            state = 0;
            break;
          case "CONNECTING":
            state = 1;
            break;
          case "CONNECTED":
            state = 2;
        }

        // if (data['extendedData'] == null || data['extendedData'] == "") {
        //   extendedData = {};
        // } else {
        //   extendedData = jsonDecode(map['extendedData']);
        // }
        extendedData = {};

        ZegoExpressEngine.onRoomStateUpdate!(
            data["roomID"],
            ZegoRoomState.values[state],
            data['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;
      case "onRoomStateChanged":
        if (ZegoExpressEngine.onRoomStateChanged == null) return;

        final data = jsonDecode(map["data"]);
        var state, extendedData;

        switch (data["reason"]) {
          case "LOGINING":
            state = 0;
            break;
          case "LOGINED":
            state = 1;
            break;
          case "LOGIN_FAILED":
            state = 2;
            break;
          case "RECONNECTING":
            state = 3;
            break;
          case "RECONNECTED":
            state = 4;
            break;
          case "RECONNECT_FAILED":
            state = 5;
            break;
          case "KICKOUT":
            state = 6;
            break;
          case "LOGOUT":
            state = 7;
            break;
          case "LOGOUT_FAILED":
            state = 8;
            break;
        }
        // if (data['extendedData'] == null || data['extendedData'] == "") {
        //   extendedData = {};
        // } else {
        //   extendedData = jsonDecode(map['extendedData']);
        // }
        extendedData = {};

        ZegoExpressEngine.onRoomStateChanged!(
            data["roomID"],
            ZegoRoomStateChangedReason.values[state],
            data['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;
      case "onRoomUserUpdate":
        if (ZegoExpressEngine.onRoomUserUpdate == null) return;

        var data = jsonDecode(map["data"]);
        var updateType;

        switch (data["updateType"]) {
          case "ADD":
            updateType = 0;
            break;
          case "DELETE":
            updateType = 1;
        }

        List<dynamic> userMapList = data["userList"];
        List<ZegoUser> userList = [];
        for (Map<dynamic, dynamic> userMap in userMapList) {
          ZegoUser user = ZegoUser(userMap['userID'], userMap['userName']);
          userList.add(user);
        }

        ZegoExpressEngine.onRoomUserUpdate!(
            data['roomID'], ZegoUpdateType.values[updateType], userList);
        break;
      case 'onRoomOnlineUserCountUpdate':
        if (ZegoExpressEngine.onRoomOnlineUserCountUpdate == null) return;

        var data = jsonDecode(map["data"]);
        ZegoExpressEngine.onRoomOnlineUserCountUpdate!(
            data['roomID'], data['count']);
        break;
      case "onRoomStreamUpdate":
        if (ZegoExpressEngine.onRoomStreamUpdate == null) return;

        var data, updateType, extendedData;

        data = jsonDecode(map["data"]);

        List<dynamic> streamMapList = data['streamList'];
        List<ZegoStream> streamList = [];
        for (Map<dynamic, dynamic> streamMap in streamMapList) {
          ZegoStream stream = ZegoStream(
              ZegoUser(
                  streamMap['user']['userID'], streamMap['user']['userName']),
              streamMap['streamID'],
              streamMap['extraInfo'] ?? "");
          streamList.add(stream);
        }
        extendedData = {};

        switch (data["updateType"]) {
          case "ADD":
            updateType = 0;
            break;
          case "DELETE":
            updateType = 1;
            break;
        }

        ZegoExpressEngine.onRoomStreamUpdate!(
            data['roomID'],
            ZegoUpdateType.values[updateType],
            streamList,
            Map<String, dynamic>.from(extendedData));
        break;
      case "onRoomTokenWillExpire":
        if (ZegoExpressEngine.onRoomTokenWillExpire == null) return;

        final data = jsonDecode(map["data"]);
        // 过期前30s 执行回调
        ZegoExpressEngine.onRoomTokenWillExpire!(
            data['roomID'], data['remainTimeInSecond']);
        break;
      case 'onRoomStreamExtraInfoUpdate':
        if (ZegoExpressEngine.onRoomStreamExtraInfoUpdate == null) return;
        final data = jsonDecode(map["data"]);
        List<dynamic> streamMapList = data['streamList'];
        List<ZegoStream> streamList = [];
        for (Map<dynamic, dynamic> streamMap in streamMapList) {
          ZegoStream stream = ZegoStream(
              ZegoUser(
                  streamMap['user']['userID'], streamMap['user']['userName']),
              streamMap['streamID'],
              streamMap['extraInfo'] ?? "");
          streamList.add(stream);
        }
        ZegoExpressEngine.onRoomStreamExtraInfoUpdate!(
            data['roomID'], streamList);
        break;
      case 'onRoomExtraInfoUpdate':
        if (ZegoExpressEngine.onRoomExtraInfoUpdate == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoRoomExtraInfo> roomExtraInfoList = [];
        for (Map<dynamic, dynamic> info in data['roomExtraInfoList']) {
          ZegoRoomExtraInfo msg = ZegoRoomExtraInfo(
              info['key'],
              info['value'],
              ZegoUser(
                  info['updateUser']['userID'], info['updateUser']['userName']),
              info['updateTime']);
          roomExtraInfoList.add(msg);
        }
        ZegoExpressEngine.onRoomExtraInfoUpdate!(
            data['roomID'], roomExtraInfoList);
        break;
      ////////////////////////////////////推流相关/////////////////////////////////////
      case "onPublisherStateUpdate":
        if (ZegoExpressEngine.onPublisherStateUpdate == null) return;

        var data, extendedData, state;
        data = jsonDecode(map["data"])["publisherState"];
        // if (data["extendedData"] == null || data["extendedData"] == "" ) {
        //   extendedData = {};
        // } else {
        //   extendedData = jsonDecode(data['extendedData']);
        // }
        extendedData = {};
        switch (data["state"]) {
          case "PUBLISHING":
            state = 2;
            break;
          case "NO_PUBLISH":
            state = 0;
            break;
          case "PUBLISH_REQUESTING":
            state = 1;
            break;
        }

        ZegoExpressEngine.onPublisherStateUpdate!(
            data['streamID'],
            ZegoPublisherState.values[state],
            data['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;
      case "onPublisherQualityUpdate":
        if (ZegoExpressEngine.onPublisherQualityUpdate == null) return;
        var data = jsonDecode(map["data"]);
        var quality = jsonDecode(data["quality"]);
        ZegoExpressEngine.onPublisherQualityUpdate!(
            data['streamID'],
            ZegoPublishStreamQuality(
                quality['videoCaptureFPS'],
                quality['videoEncodeFPS'],
                quality['videoSendFPS'],
                quality['videoKBPS'],
                quality['audioCaptureFPS'],
                quality['audioSendFPS'],
                quality['audioKBPS'],
                quality['rtt'],
                quality['packetLostRate'],
                ZegoStreamQualityLevel.values[quality['level']],
                quality['isHardwareEncode'],
                ZegoVideoCodecID.values[quality['videoCodecID']],
                0,
                0,
                0));
        break;
      ////////////////////////////////////拉流相关/////////////////////////////////////
      case "onPlayerStateUpdate":
        if (ZegoExpressEngine.onPlayerStateUpdate == null) return;

        var data, extendedData, state;
        data = jsonDecode(map["data"])["playerState"];

        extendedData = {};

        // if (data["extendedData"] == null || data["extendedData"] == "" ) {
        //   extendedData = {};
        // } else if (data["extendedData"]){
        //   extendedData = jsonDecode(data['extendedData']);
        // }

        switch (data["state"]) {
          case "PLAYING":
            state = 2;
            break;
          case "NO_PLAY":
            state = 0;
            break;
          case "PLAY_REQUESTING":
            state = 1;
            break;
        }

        ZegoExpressEngine.onPlayerStateUpdate!(
            data['streamID'],
            ZegoPlayerState.values[state],
            data['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;
      case "onPlayerQualityUpdate":
        if (ZegoExpressEngine.onPlayerQualityUpdate == null) return;
        var data = jsonDecode(map["data"]);
        var quality = jsonDecode(data["quality"]);
        ZegoExpressEngine.onPlayerQualityUpdate!(
            data['streamID'],
            ZegoPlayStreamQuality(
                quality['videoRecvFPS'],
                0,
                quality['videoFPS'],
                quality['videoRenderFPS'],
                quality['videoKBPS'],
                0,
                quality['audioRecvFPS'],
                0,
                0,
                0,
                quality['audioKBPS'],
                0,
                0,
                quality['rtt'],
                quality['packetLostRate'],
                quality['peerToPeerDelay'],
                quality['peerToPeerPacketLostRate'],
                ZegoStreamQualityLevel.values[quality['level']],
                quality['delay'],
                0,
                false,
                ZegoVideoCodecID.values[quality['videoCodecID']],
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0));
        break;
      case 'onPlayerVideoSizeChanged':
        if (ZegoExpressEngine.onPlayerVideoSizeChanged == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onPlayerVideoSizeChanged!(
            data['streamID'], data['width'], data['height']);

        break;
      case 'onPlayerRecvSEI':
        if (ZegoExpressEngine.onPlayerRecvSEI == null) return;
        final data = jsonDecode(map["data"]);
        var bytes = Uint8List.fromList(utf8.encode(data["data"]));
        ZegoExpressEngine.onPlayerRecvSEI!(data['streamID'], bytes);
        break;
      ////////////////////////////////////设备相关/////////////////////////////////////
      case "onRemoteMicStateUpdate":
        if (ZegoExpressEngine.onRemoteMicStateUpdate == null) return;

        final data = jsonDecode(map["data"]);
        var state = data["state"];
        if (state < 0 || state > 16) {
          state = 10;
        }
        ZegoExpressEngine.onRemoteMicStateUpdate!(
            data['streamID'], ZegoRemoteDeviceState.values[state]);
        break;
      case "onRemoteCameraStateUpdate":
        if (ZegoExpressEngine.onRemoteCameraStateUpdate == null) return;
        final data = jsonDecode(map["data"]);
        var state = data["state"];
        if (state < 0 || state > 16) {
          state = 10;
        }
        ZegoExpressEngine.onRemoteCameraStateUpdate!(
            data['streamID'], ZegoRemoteDeviceState.values[state]);
        break;
      case "onRemoteSoundLevelUpdate":
        if (ZegoExpressEngine.onRemoteSoundLevelUpdate == null) return;
        var soundLevels = <dynamic, dynamic>{};
        final data = jsonDecode(map["data"]);
        if (data['soundLevels'] != null) {
          soundLevels = data['soundLevels'];
        }
        ZegoExpressEngine
            .onRemoteSoundLevelUpdate!(Map<String, double>.from(soundLevels));
        break;
      case 'onCapturedSoundLevelUpdate':
        if (ZegoExpressEngine.onCapturedSoundLevelUpdate == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onCapturedSoundLevelUpdate!(data['soundLevel']);
        break;
      case 'onAudioDeviceStateChanged':
        if (ZegoExpressEngine.onAudioDeviceStateChanged == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onAudioDeviceStateChanged!(
            data['updateType'], data['deviceType'], data['deviceInfo']);
        break;
      // onAudioDeviceVolumeChanged
      case 'onVideoDeviceStateChanged':
        if (ZegoExpressEngine.onVideoDeviceStateChanged == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onVideoDeviceStateChanged!(
            data['updateType'], data['deviceInfo']);
        break;
      case 'onLocalDeviceExceptionOccurred':
        if (ZegoExpressEngine.onLocalDeviceExceptionOccurred == null) return;
        final data = jsonDecode(map["data"]);
        final exceptionType =
            ZegoDeviceExceptionType.values[data['exceptionType']];
        final deviceType = ZegoDeviceType.values[data['deviceType']];
        ZegoExpressEngine.onLocalDeviceExceptionOccurred!(
            exceptionType, deviceType, data['deviceID']);
        break;
      // onCapturedSoundLevelInfoUpdate
      // onRemoteSoundLevelInfoUpdate
      // onCapturedAudioSpectrumUpdate
      // onRemoteAudioSpectrumUpdate
      // onRemoteSpeakerStateUpdate
      // onAudioRouteChange
      // onAudioVADStateUpdate
      ////////////////////////////////////消息相关/////////////////////////////////////
      case 'onIMRecvBroadcastMessage':
        if (ZegoExpressEngine.onIMRecvBroadcastMessage == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoBroadcastMessageInfo> messageList = [];
        for (Map<dynamic, dynamic> info in data['messageList']) {
          ZegoBroadcastMessageInfo msg = ZegoBroadcastMessageInfo(
              info['message'],
              info['messageID'],
              info['sendTime'],
              ZegoUser(
                  info['fromUser']['userID'], info['fromUser']['userName']));
          messageList.add(msg);
        }
        ZegoExpressEngine.onIMRecvBroadcastMessage!(
            data['roomID'], messageList);
        break;
      case 'onIMRecvBarrageMessage':
        if (ZegoExpressEngine.onIMRecvBarrageMessage == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoBarrageMessageInfo> messageList = [];
        for (Map<dynamic, dynamic> info in data['messageList']) {
          ZegoBarrageMessageInfo msg = ZegoBarrageMessageInfo(
              info['message'],
              info['messageID'],
              info['sendTime'],
              ZegoUser(
                  info['fromUser']['userID'], info['fromUser']['userName']));
          messageList.add(msg);
        }
        ZegoExpressEngine.onIMRecvBarrageMessage!(data['roomID'], messageList);
        break;
      case 'onIMRecvCustomCommand':
        if (ZegoExpressEngine.onIMRecvCustomCommand == null) return;
        final data = jsonDecode(map["data"]);
        ZegoUser fromUser =
            ZegoUser(data['fromUser']['userID'], data['fromUser']['userName']);
        ZegoExpressEngine.onIMRecvCustomCommand!(
            data['roomID'], fromUser, data['command']);
        break;
      case 'onReceiveRealTimeSequentialData':
        if (ZegoExpressEngine.onReceiveRealTimeSequentialData == null) return;
        final data = jsonDecode(map["data"]);
        var bytes = Uint8List.fromList(utf8.encode(data["data"]));
        var mgr =
            ZegoExpressImpl.realTimeSequentialDataManagerMap[data['index']];
        if (mgr != null) {
          ZegoExpressEngine.onReceiveRealTimeSequentialData!(
              mgr, bytes, data['streamID']);
        }
        break;
      // 获取推拉流输出音频原始数据
      case 'onMixedAudioData':
        if (ZegoExpressEngine.onMixedAudioData == null) return;
        final data = map["data"];
        final param = getZegoAudioOutputParam(data);
        ZegoExpressEngine.onMixedAudioData!(
            Uint8List.view(getProperty(data, 'data')),
            getProperty(data, 'dataLength'),
            param);
        break;
      case 'onPlayerAudioData':
        if (ZegoExpressEngine.onPlayerAudioData == null) return;
        final data = map["data"];
        final param = getZegoAudioOutputParam(data);
        ZegoExpressEngine.onPlayerAudioData!(
            Uint8List.view(getProperty(data, 'data')),
            getProperty(data, 'dataLength'),
            param,
            getProperty(data, 'streamID'));
        break;
      ////////////////////////////////////其他/////////////////////////////////////
      case 'onDebugError':
        if (ZegoExpressEngine.onDebugError == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onDebugError!(
            data['errorCode'], data['funcName'], data['info']);
        break;
      //////////////////////////////混流//////////////////////////
      case 'onMixerSoundLevelUpdate':
        final data = parseJSON(map["data"]);
        final soundLevelsMap = getProperty(data, 'soundLevels');
        final keys = objectKeys(soundLevelsMap);
        final soundLevels = <int, double>{};
        for (int i = 0; i < keys.length; i++) {
          final key = keys[i];
          final numKey = int.parse(key);
          final numValue = getProperty(soundLevelsMap, key);
          soundLevels[numKey] = numValue;
        };
        ZegoExpressEngine.onMixerSoundLevelUpdate!(soundLevels);
        break;
      case 'onAutoMixerSoundLevelUpdate':
        final data = parseJSON(map["data"]);
        final soundLevelsMap = getProperty(data, 'soundLevels');
        final keys = objectKeys(soundLevelsMap);
        final soundLevels = <String, double>{};
        for (int i = 0; i < keys.length; i++) {
          final key = keys[i];
          final numKey = key;
          final numValue = getProperty(soundLevelsMap, key);
          soundLevels[numKey] = numValue;
        };
        ZegoExpressEngine.onAutoMixerSoundLevelUpdate!(soundLevels);
        break;
      case 'onMixerRelayCDNStateUpdate':
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onMixerRelayCDNStateUpdate!(
            data['taskID'], data['infoList']);
        break;
      //////////////////////////////媒体播放器//////////////////////////
      case "onMediaPlayerStateUpdate":
        if (ZegoExpressEngine.onMediaPlayerStateUpdate == null) return;
        final data = jsonDecode(map["data"]);
        final index = data["mediaPlayer"];
        if (index is int) {
          final player = ZegoExpressImpl.mediaPlayerMap[index];
          if (player != null) {
            ZegoMediaPlayerState state =
                ZegoMediaPlayerState.values[data["state"]];
            ZegoExpressEngine.onMediaPlayerStateUpdate!(
                player, state, data["errorCode"]);
          }
        }
        break;
      case "onMediaPlayerPlayingProgress":
        if (ZegoExpressEngine.onMediaPlayerPlayingProgress == null) return;
        final data = jsonDecode(map["data"]);
        final index = data["mediaPlayer"];
        if (index is int) {
          final player = ZegoExpressImpl.mediaPlayerMap[index];
          if (player != null) {
            final millisecond = data["millisecond"];
            ZegoExpressEngine.onMediaPlayerPlayingProgress!(
                player, millisecond);
          }
        }
        break;
      default:
        break;
    }
  }

  static getPublishChannel([int? channel = 0]) {
    return channel;
  }

  static Future<void> createEngineWithProfile(dynamic profile) async {
    final appID = profile["appID"];
    const server = "wss://test.com";
    Profile engineProfile =
        Profile(appID: appID, server: server, scenario: profile['scenario']);

    ZegoFlutterEngine.createEngineWithProfile(engineProfile, 'flutter');
  }

  static Future<void> presetLogConfig(dynamic config) async {
    final logLevel = config["logLevel"];
    LogConfig logConfig = LogConfig(logLevel: logLevel);
    ZegoFlutterEngine.presetLogConfig(logConfig);
  }

  Future<void> destroyEngine() {
    ZegoFlutterEngine.destroyEngine();

    return Future.value();
  }

  Future<void> setRoomScenario(int scenario) async {
    return ZegoFlutterEngine.instance.setRoomScenario(scenario);
  }

  Future<void> setRoomMode(int mode) async {
    return ZegoFlutterEngine.setRoomMode(mode);
  }

  Future<void> setGeoFence(int type, List<int> areaList) async {
    ZegoFlutterEngine.setGeoFence(type, areaList);
    return Future.value();
  }

  static Future<void> setLocalProxyConfig(
      List<Map> proxyList, bool enable) async {
    // fixme
    var proxy = proxyList[0];
    ZegoLocalProxyConfigWeb config =
        ZegoLocalProxyConfigWeb(accesshubProxy: proxy['hostName']);
    ZegoFlutterEngine.setLocalProxyConfig(config, enable);
    return Future.value();
  }

  static Future<void> setCloudProxyConfig(
      List<Map> proxyList, String token, bool enable) async {
    List<ZegoProxyInfoWeb> proxyListWeb = [];
    for (var item in proxyList) {
      if (item['port'] == 0) {
        ZegoProxyInfoWeb newItem = ZegoProxyInfoWeb(hostName: item['hostName']);
        proxyListWeb.add(newItem);
      } else {
        ZegoProxyInfoWeb newItem =
            ZegoProxyInfoWeb(hostName: item['hostName'], port: item['port']);
        proxyListWeb.add(newItem);
      }
    }
    ZegoFlutterEngine.setCloudProxyConfig(proxyListWeb, token, enable);
    return Future.value();
  }

  Future<String> callExperimentalAPI(dynamic params) async {
    //todo:
    return Future.value("");
  }

  /////////////////////////////房间相关////////////////////////////////////////
  Future<Map<dynamic, dynamic>> loginRoom(
      String roomID, dynamic user, dynamic config) async {
    ZegoUserWeb webUser =
        ZegoUserWeb(userID: user["userID"], userName: user["userName"]);
    ZegoRoomConfigWeb webConfig = ZegoRoomConfigWeb(
        maxMemberCount: config["maxMemberCount"],
        token: config["token"],
        isUserStatusNotify: config["isUserStatusNotify"]);

    final map = {};
    map["extendedData"] = "{}";
    try {
      final result = await (() {
        Map completerMap = createCompleter();
        ZegoFlutterEngine.instance.loginRoom(roomID, webUser, webConfig,
            completerMap["success"], completerMap["fail"]);
        return completerMap["completer"].future;
      })();
      map["errorCode"] = result;
    } catch (errorCode) {
      map["errorCode"] = errorCode;
    }
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> logoutRoom(String? roomID) async {
    final result = await (() {
      Completer c = Completer();
      ZegoFlutterEngine.instance.logoutRoom(allowInterop((errorCode, msg) {
        c.complete(errorCode);
      }), roomID);
      return c.future;
    })();

    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = result;
    map["extendedData"] = "{}";
    return Future.value(map);
  }

  Future<void> renewToken(String roomID, String token) async {
    return Future.value(ZegoFlutterEngine.instance.renewToken(roomID, token));
  }

  Future<void> switchRoom(
      String fromRoomID, String toRoomID, dynamic config) async {
    ZegoRoomConfigWeb webConfig = ZegoRoomConfigWeb(
        maxMemberCount: config["maxMemberCount"] ?? 0,
        token: config["token"] ?? "",
        isUserStatusNotify: config["isUserStatusNotify"] ?? false);
    ZegoFlutterEngine.instance.switchRoom(fromRoomID, toRoomID, webConfig);
    return Future.value();
  }

  Future<Map<dynamic, dynamic>> setRoomExtraInfo(
      String roomID, String key, String value) async {
    final Map<dynamic, dynamic> map = {};
    try {
      final result = await (() {
        Map completerMap = createCompleter();
        ZegoFlutterEngine.instance.setRoomExtraInfo(
            roomID, key, value, completerMap["success"], completerMap["fail"]);
        return completerMap["completer"].future;
      })();
      map["errorCode"] = result;
    } catch (errorCode) {
      map["errorCode"] = errorCode;
    }
    return Future.value(map);
  }

  /////////////////////////////推流相关////////////////////////////////////////
  Future<void> setVideoConfig(dynamic config, int? channel) {
    ZegoWebVideoConfig webVideoConfig = ZegoWebVideoConfig(
        encodeWidth: config["encodeWidth"],
        encodeHeight: config["encodeHeight"],
        fps: config["fps"],
        bitrate: config["bitrate"],
        codecID: config["codecID"]);

    ZegoFlutterEngine.instance
        .setVideoConfig(webVideoConfig, getPublishChannel(channel));
    return Future.value();
  }

  Future<dynamic> getVideoConfig(int? channel) async {
    var config =
        ZegoFlutterEngine.instance.getVideoConfig(getPublishChannel(channel));
    var map = {};
    if (config != null) {
      map['captureWidth'] = getProperty(config, 'captureWidth') ??
          getProperty(config, 'encodeWidth') ??
          0;
      map['captureHeight'] = getProperty(config, 'captureHeight') ??
          getProperty(config, 'encodeHeight') ??
          0;
      map['encodeWidth'] = getProperty(config, 'encodeHeight') ?? 0;
      map['encodeHeight'] = getProperty(config, 'encodeWidth') ?? 0;
      map['fps'] = getProperty(config, 'fps') ?? 0;
      map['bitrate'] = getProperty(config, 'bitrate') ?? 0;
      map['codecID'] = getProperty(config, 'codecID') ?? 0;
      final gop = getProperty(config, 'keyFrameInterval');
      if (gop != null) {
        map['keyFrameInterval'] = gop;
      }
    }
    return Future.value(map);
  }

  Future<void> setAudioConfig(dynamic config, int? channel) {
    ZegoWebAudioConfig aconfig = ZegoWebAudioConfig(
      bitrate: config["bitrate"],
      channel: config["channel"],
    );

    ZegoFlutterEngine.instance
        .setAudioConfig(aconfig, getPublishChannel(channel));
    return Future.value();
  }

  Future<Map<dynamic, dynamic>> getAudioConfig(int? channel) async {
    var config =
        ZegoFlutterEngine.instance.getAudioConfig(getPublishChannel(channel));
    var map = {};
    map['bitrate'] = config.bitrate;
    map['channel'] = config.channel;
    map['codecID'] = 6; // Low3(OPUS)

    return Future.value(map);
  }

  Future<void> setVideoMirrorMode(int mirrorMode, int? channel) async {
    ZegoFlutterEngine.instance
        .setVideoMirrorMode(mirrorMode, getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPreview(dynamic canvas, int channel) async {
    previewView = document.getElementById("zego-view-${canvas["view"]}");
    // previewView?.muted = true;
    // ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));

    await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.startPreview(
          previewView,
          getPublishChannel(channel),
          dartObjToJSON(canvas),
          completerMap["success"],
          completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return Future.value();
  }

  Future<void> stopPreview(int? channel) {
    ZegoFlutterEngine.instance.stopPreview(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPublishingStream(
      String streamID, dynamic config, int? channel) {
    String roomID = "";
    if (config["roomID"] != null) {
      roomID = config["roomID"];
    }
    PublishConfig options = PublishConfig(roomID: roomID);
    ZegoFlutterEngine.instance
        .startPublishingStream(streamID, getPublishChannel(channel), options);
    return Future.value();
  }

  Future<void> stopPublishingStream(int? channel) {
    ZegoFlutterEngine.instance.stopPublishingStream(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> setSEIConfig(Map<dynamic, dynamic> config) async {
    return await ZegoFlutterEngine.instance.setSEIConfig(config['type']);
  }

  Future<void> sendSEI(dynamic data, int dataLength, int channel) async {
    return await ZegoFlutterEngine.instance
        .sendSEI(const Utf8Decoder().convert(data), dataLength, channel);
  }

  Future<void> setCaptureVolume(int volume) async {
    return await ZegoFlutterEngine.instance.setCaptureVolume(volume);
  }

  Future<Map<dynamic, dynamic>> addPublishCdnUrl(
      String streamID, String targetURL,
      {int? timeout}) async {
    final map = {};
    try {
      final promise = callMethod(ZegoFlutterEngine.instance, 'addPublishCdnUrl',
          [streamID, targetURL]);
      final result = await promiseToFuture(promise);
      map["errorCode"] = getProperty(result, 'errorCode');
    } catch (error) {
      map["errorCode"] = getProperty(error, 'errorCode');
    }
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> removePublishCdnUrl(
      String streamID, String targetURL) async {
    final map = {};
    try {
      final promise = callMethod(ZegoFlutterEngine.instance,
          'removePublishCdnUrl', [streamID, targetURL]);
      final result = await promiseToFuture(promise);
      map["errorCode"] = getProperty(result, 'errorCode');
    } catch (error) {
      map["errorCode"] = getProperty(error, 'errorCode');
    }
    return Future.value(map);
  }

  Future<void> mutePublishStreamVideo(bool mute, int? channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamVideo(mute, getPublishChannel(channel));
  }

  Future<void> mutePublishStreamAudio(bool mute, int? channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamAudio(mute, getPublishChannel(channel));
  }

  Future<void> setStreamAlignmentProperty(int alignment, int? channel) async {
    return await ZegoFlutterEngine.instance
        .setStreamAlignmentProperty(alignment, getPublishChannel(channel));
  }

  Future<void> enableTrafficControl(bool enable, int? channel) async {
    return await ZegoFlutterEngine.instance
        .enableTrafficControl(enable, getPublishChannel(channel));
  }

  Future<void> setMinVideoBitrateForTrafficControl(
      int bitrate, int mode, int? channel) async {
    return await ZegoFlutterEngine.instance.setMinVideoBitrateForTrafficControl(
        bitrate, mode, getPublishChannel(channel));
  }

  // setMinVideoFpsForTrafficControl
  // setMinVideoResolutionForTrafficControl
  Future<void> setTrafficControlFocusOn(int mode, int? channel) async {
    return await ZegoFlutterEngine.instance
        .setTrafficControlFocusOn(mode, getPublishChannel(channel));
  }

  Future<void> muteMicrophone(bool mute) async {
    return await ZegoFlutterEngine.instance.muteMicrophone(mute);
  }

  Future<void> isMicrophoneMuted() async {
    return await ZegoFlutterEngine.instance.isMicrophoneMuted();
  }

  /////////////////////////////拉流相关////////////////////////////////////////
  Future<void> startPlayingStream(
      String streamID, dynamic canvas, dynamic config) {
    final playView = document.getElementById('zego-view-${canvas["view"]}');
    // ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));
    ZegoFlutterEngine.instance.startPlayingStream(
        streamID, playView, "", jsonEncode(config), dartObjToJSON(canvas));
    return Future.value();
  }

  Future<void> stopPlayingStream(String streamID) {
    ZegoFlutterEngine.instance.stopPlayingStream(streamID);
    return Future.value();
  }

  Future<void> setPlayVolume(String streamID, int volume) {
    ZegoFlutterEngine.instance.setPlayVolume(streamID, volume);
    return Future.value();
  }

  Future<void> setAllPlayStreamVolume(int volume) {
    ZegoFlutterEngine.instance.setAllPlayStreamVolume(volume);
    return Future.value();
  }

  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    await ZegoFlutterEngine.instance.mutePlayStreamAudio(streamID, mute);
  }

  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    await ZegoFlutterEngine.instance.mutePlayStreamVideo(streamID, mute);
  }

  Future<void> muteAllPlayAudioStreams(bool mute) async {
    await ZegoFlutterEngine.instance.muteAllPlayAudioStreams(mute);
  }

  Future<void> muteAllPlayVideoStreams(bool mute) async {
    await ZegoFlutterEngine.instance.muteAllPlayVideoStreams(mute);
  }

  Future<int> updatePlayingCanvas(String streamID, dynamic canvas) async {
    final view = document.getElementById('zego-view-${canvas["view"]}');
    return ZegoFlutterEngine.instance
        .updatePlayingCanvas(streamID, view, dartObjToJSON(canvas));
  }

  // Future<void> enableHardwareDecoder(bool enable) async {
  //   //ZegoFlutterEngine.instance.enableHardwareDecoder(enable);
  //   return Future.value();
  // }

  /////////////////////////////消息相关////////////////////////////////////////
  Future sendBroadcastMessageWithCallback(String roomID, String message) {
    Completer c = Completer();
    ZegoFlutterEngine.instance.sendBroadcastMessage(roomID, message,
        allowInterop((errorCode, messageID) {
      final Map<dynamic, dynamic> map = {};
      map["errorCode"] = errorCode;
      map["messageID"] = messageID;
      c.complete(map);
    }), allowInterop((errorCode) {
      c.completeError(errorCode);
    }));
    return c.future;
  }

  Future<Map<dynamic, dynamic>> sendBroadcastMessage(
      String roomID, String message) async {
    try {
      final Map<dynamic, dynamic> res =
          await sendBroadcastMessageWithCallback(roomID, message);
      return Future.value(res);
    } catch (errorCode) {
      final Map<dynamic, dynamic> map = {};
      map["messageID"] = 0; // fixme
      map["errorCode"] = errorCode;
      return Future.value(map);
    }
  }

  Future sendBarrageMessageWitchCallback(String roomID, String message) {
    Completer c = Completer();
    ZegoFlutterEngine.instance.sendBarrageMessage(roomID, message,
        allowInterop((errorCode, messageID) {
      final Map<dynamic, dynamic> map = {};
      map["errorCode"] = errorCode;
      map["messageID"] = messageID;
      c.complete(map);
    }), allowInterop((errorCode) {
      c.completeError(errorCode);
    }));
    return c.future;
  }

  Future<Map<dynamic, dynamic>> sendBarrageMessage(
      String roomID, String message) async {
    try {
      final Map<dynamic, dynamic> map =
          await sendBarrageMessageWitchCallback(roomID, message);
      return Future.value(map);
    } catch (errorCode) {
      final Map<dynamic, dynamic> map = {};
      map["messageID"] = ''; // fixme
      map["errorCode"] = errorCode;
      return Future.value(map);
    }
  }

  Future<Map<dynamic, dynamic>> sendCustomCommand(
      String roomID, String message, List toUserList) async {
    List useridList = [];
    for (var item in toUserList) {
      useridList.add(item["userID"]);
    }
    final Map<dynamic, dynamic> map = {};
    try {
      final result = await (() {
        Map completerMap = createCompleter();
        ZegoFlutterEngine.instance.sendCustomCommand(roomID, message,
            useridList, completerMap["success"], completerMap["fail"]);
        return completerMap["completer"].future;
      })();
      map["errorCode"] = result;
    } catch (e) {
      map["errorCode"] = e;
    }
    return Future.value(map);
  }

  Future<int> createRealTimeSequentialDataManager(String roomID) async {
    //print("*### [sdk] createRealTimeSequentialDataManager ${roomID}");
    final index = await ZegoFlutterEngine.instance
        .createRealTimeSequentialDataManager(roomID);
    return index;
  }

  Future<void> destroyRealTimeSequentialDataManager(int index) async {
    // print("*### [sdk] destroyRealTimeSequentialDataManager ${index}");
    await ZegoFlutterEngine.instance
        .destroyRealTimeSequentialDataManager(index);
  }

  Future<void> startBroadcasting(int index, String streamID) async {
    await ZegoFlutterEngine.instance.startBroadcasting(index, streamID);
  }

  Future<void> stopBroadcasting(int index, String streamID) async {
    await ZegoFlutterEngine.instance.stopBroadcasting(index, streamID);
  }

  Future<Map<dynamic, dynamic>> sendRealTimeSequentialData(
      int index, Uint8List data, String streamID) async {
    final Map<dynamic, dynamic> map = {};
    await ZegoFlutterEngine.instance.sendRealTimeSequentialData(
        index, streamID, const Utf8Decoder().convert(data));
    map["errorCode"] = 0;
    return Future.value(map);
  }

  Future<void> startSubscribing(int index, String streamID) async {
    await ZegoFlutterEngine.instance.startSubscribing(index, streamID);
  }

  Future<void> stopSubscribing(int index, String streamID) async {
    await ZegoFlutterEngine.instance.stopSubscribing(index, streamID);
  }

  /////////////////////////////设备相关////////////////////////////////////////
  Future<bool> destroyPlatformView(int viewID) {
    final media = document.getElementById("zego-view-$viewID");
    if (media != null) {
      media.remove();
    } else {
      return Future.value(false);
    }

    return Future.value(true);
  }

  Future<void> enableCamera(bool enable, int channel) async {
    return await ZegoFlutterEngine.instance
        .enableCamera(enable, getPublishChannel(channel));
  }

  Future<void> useFrontCamera(bool enable, int channel) async {
    return await ZegoFlutterEngine.instance
        .useFrontCamera(enable, getPublishChannel(channel));
  }

  // type 0 为input, 1 为output
  Future<List> getAudioDeviceList(int type) async {
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.getAudioDeviceList(
          type, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return formatDeviceList(jsonDecode(result));
  }

  static formatDeviceList(list) {
    var newList = [];
    if (list == null || list is! List) return newList;
    for (var item in list) {
      Map<String, dynamic> info = {};
      info["deviceID"] = item["deviceID"];
      info["deviceName"] = item["deviceName"];
      newList.add(info);
    }
    return newList;
  }

  Future<List> getVideoDeviceList() async {
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance
          .getVideoDeviceList(completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return formatDeviceList(jsonDecode(result));
  }

  Future<void> useVideoDevice(String deviceID, int channel) async {
    return await ZegoFlutterEngine.instance.useVideoDevice(deviceID, channel);
  }

  // type：input 0, output 1
  Future<void> useAudioDevice(int type, String deviceID) async {
    return ZegoFlutterEngine.instance.useAudioDevice(type, deviceID);
  }

  Future<void> useAudioOutputDevice(String viewID, String deviceID) async {
    return await ZegoFlutterEngine.instance.useAudioOutputDevice(
        document.querySelector('#zego-view-' + viewID), deviceID);
  }

  Future<void> startSoundLevelMonitor(dynamic config) async {
    return ZegoFlutterEngine.instance.startSoundLevelMonitor(config);
  }

  Future<void> stopSoundLevelMonitor() async {
    return ZegoFlutterEngine.instance.stopSoundLevelMonitor();
  }

  Future<void> setEngineConfig(dynamic config) async {
    final engineConfig = dartObjToJSON(config);
    return await ZegoFlutterEngine.setEngineConfig(engineConfig);
  }

  Future<Map<dynamic, dynamic>> setStreamExtraInfo(
      String extraInfo, int channel) async {
    await ZegoFlutterEngine.instance.setStreamExtraInfo(extraInfo, channel);
    final map = {};
    map["errorCode"] = 0;
    return Future.value(map);
  }

  /////////////////////////////前处理相关////////////////////////////////////////
  Future<void> enableAEC(bool enable) async {
    ZegoFlutterEngine.instance.enableAEC(enable);
    return Future.value();
  }

  Future<void> enableAGC(bool enable) async {
    ZegoFlutterEngine.instance.enableAGC(enable);
    return Future.value();
  }

  Future<void> enableANS(bool enable) async {
    ZegoFlutterEngine.instance.enableANS(enable);
    return Future.value();
  }

  Future<void> enableEffectsBeauty(bool enable) async {
    return ZegoFlutterEngine.instance.enableEffectsBeauty(enable);
  }

  Future<void> startEffectsEnv() async {
    return ZegoFlutterEngine.instance.startEffectsEnv();
  }

  Future<void> stopEffectsEnv() async {
    return ZegoFlutterEngine.instance.stopEffectsEnv();
  }

  Future<void> setEffectsBeautyParam(dynamic param) async {
    EffectsBeautyParam options = EffectsBeautyParam(
        whitenIntensity: param["whitenIntensity"],
        rosyIntensity: param["rosyIntensity"],
        smoothIntensity: param["smoothIntensity"],
        sharpenIntensity: param["sharpenIntensity"]);
    return ZegoFlutterEngine.instance.setEffectsBeautyParam(options);
  }

  // Future<void> setVoiceChangerPreset(int preset) async {}
  // Future<void> setVoiceChangerParam(int pitch) async {}
  // Future<void> setReverbPreset(int prese) async {}
  // Future<void> enableVirtualStereo(bool enable, int angle) async {}

  /////////////////////////////播放器相关////////////////////////////////////////
  Future<int> createMediaPlayer() async {
    var i = _index;
    var instance = await ZegoFlutterEngine.instance.createMediaPlayer(i);
    if (instance != null) {
      var mediaPlayer = MediaPlayer();
      mediaPlayer.instance = instance;
      _mediaPlayers[i] = mediaPlayer;
      _index++;
    } else {
      i = -1;
    }
    return i;
  }

  void mediaPlayerSetPlayerCanvas(int index, Map<dynamic, dynamic> canvas) {
    var viewElem = document.getElementById("zego-view-${canvas["view"]}");
    if (_mediaPlayers[index] == null) {
      return;
    }
    // ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));
    callMethod(_mediaPlayers[index].instance, 'setPlayerCanvas', [
      viewElem,
      dartObjToJSON(canvas),
    ]);
  }

  Future<Map<dynamic, dynamic>> mediaPlayerLoadResource(int index, String path,
      [int startPosition = 0]) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = MEDIA_PLAYER_NOT_EXIST;
      return Future.value(map);
    }

    final promise = callMethod(
        _mediaPlayers[index].instance, 'loadResource', [path, startPosition]);
    final result = await promiseToFuture(promise);
    map["errorCode"] = getProperty(result, 'errorCode');
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> mediaPlayerLoadResourceFromMediaData(
      int index, dynamic mediaData, int startPosition) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = MEDIA_PLAYER_NOT_EXIST;
      return Future.value(map);
    }
    final promise =
        callMethod(_mediaPlayers[index].instance, 'loadResourceFromMediaData', [
      mediaData,
      startPosition,
    ]);
    final result = await promiseToFuture(promise);
    map["errorCode"] = result;
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>>
      mediaPlayerLoadCopyrightedMusicResourceWithPosition(
          int index, String resourceID, int startPosition) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = MEDIA_PLAYER_NOT_EXIST;
      return Future.value(map);
    }

    final urls = _downloadUrlMap[resourceID];

    final promise = callMethod(
        _mediaPlayers[index].instance,
        'loadCopyrightedMusicResourceWithPosition',
        [resourceID, urls, 0, startPosition]);
    final result = await promiseToFuture(promise);
    map["errorCode"] = result;
    return Future.value(map);
  }

  mediaPlayerSetAudioTrackIndex(int index, int trackIndex) {
    if (_mediaPlayers[index] == null) {
      return;
    }
    callMethod(
        _mediaPlayers[index].instance, 'setAudioTrackIndex', [trackIndex]);
  }

  Future<void> mediaPlayerEnableRepeat(int index, bool enable) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'enableRepeat', [enable]);
  }

  Future<void> mediaPlayerStart(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'start', []);
  }

  Future<void> mediaPlayerPause(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'pause', []);
  }

  Future<void> mediaPlayerStop(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'stop', []);
  }

  Future<void> mediaPlayerResume(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'resume', []);
  }

  Future<void> mediaPlayerSetPlaySpeed(int index, double speed) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'setPlaySpeed', [speed]);
  }

  Future<void> mediaPlayerMuteLocal(int index, bool enable) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return callMethod(_mediaPlayers[index].instance, 'muteLocal', [enable]);
  }

  Future<void> mediaPlayerEnableAux(int index, bool enable) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    ZegoFlutterEngine.instance
        .mediaPlayerEnableAux(enable, _mediaPlayers[index].instance);
  }

  Future<void> destroyMediaPlayer(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    callMethod(_mediaPlayers[index].instance, 'destroy', []);
    _mediaPlayers.remove(index);
  }

  Future<void> mediaPlayerSetVolume(int index, int volume) {
    if (_mediaPlayers[index] == null) {
      return Future.value();
    }
    return Future.value(ZegoFlutterEngine.instance
        .mediaPlayerSetVolume(volume, _mediaPlayers[index].instance));
  }

  Future<int> mediaPlayerGetTotalDuration(int index) async {
    var duration =
        callMethod(_mediaPlayers[index].instance, 'getTotalDuration', []);
    return Future.value(duration);
  }

  Future<dynamic> mediaPlayerSeekTo(int index, int millisecond) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = MEDIA_PLAYER_NOT_EXIST;
      return Future.value(map);
    }
    var errorCode =
        callMethod(_mediaPlayers[index].instance, 'seekTo', [millisecond]);

    map['errorCode'] = errorCode;
    return map;
  }

  // Future<List<ZegoScreenCaptureSourceInfo>> getScreenCaptureSources() async{
  //   List<ZegoScreenCaptureSourceInfo> list = [];
  //   list.add(ZegoScreenCaptureSourceInfo(ZegoScreenCaptureSourceType.Screen, _index, "web", null, null));
  //   return list;
  // }
  Future<int> createScreenCaptureSource(int? sourceId, int? sourceType) async {
    sourceId ??= _index;
    var instance =
        await ZegoFlutterEngine.instance.createScreenCaptureSource(sourceId);
    var i = _index;
    var capureSource = ScreenCaptureSource();
    capureSource.instance = instance;
    _mediaSources[i] = capureSource;
    _index++;
    return i;
  }

  Future<void> destroyScreenCaptureSource(int index) async {
    if (_mediaSources[index] == null) return;
    _mediaSources[index].instance.stopCapture(index);
    _mediaSources.remove(index);
  }

  Future<void> setLowlightEnhancement(int mode, int? channel) async {
    ZegoFlutterEngine.instance
        .setLowlightEnhancement(mode, getPublishChannel(channel));
    return Future.value();
  }

  Future<int> setVideoSource(int source, int? instanceID, int? channel) async {
    return ZegoFlutterEngine.instance
        .setVideoSource(source, instanceID ?? 0, getPublishChannel(channel));
  }

  Future<int> setAudioSource(int source, int? channel) async {
    return ZegoFlutterEngine.instance
        .setAudioSource(source, getPublishChannel(channel));
  }

  Future<void> enableHardwareEncoder(bool enable) async {
    return ZegoFlutterEngine.instance.enableHardwareEncoder(enable);
  }

  Future<void> startCaptureScreen(int index, dynamic config) async {
    return _mediaSources[index].instance.startCapture(config);
  }

  Future<void> stopCaptureScreen(int index) async {
    return _mediaSources[index].instance.stopCapture();
  }

  ////////////////////////////////混流接口//////////////////////////////////////////
  Future<Map<dynamic, dynamic>> startMixerTask(
      Map<dynamic, dynamic> config) async {
    config["userData"] = const Utf8Decoder().convert(config["userData"]);
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance
          .startMixerTask(jsonEncode(config), completerMap["success"]);
      return completerMap["completer"].future;
    })();
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = result.errorCode;
    if (result.extendedData != null) {
      map['extendedData'] = result.extendedData;
    }
    return map;
  }

  Future<Map<dynamic, dynamic>> stopMixerTask(String taskID) async {
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.stopMixerTask(taskID, completerMap["success"]);
      return completerMap["completer"].future;
    })();

    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = result.errorCode;
    if (result.extendedData) {
      map['extendedData'] = result.extendedData;
    }
    return map;
  }

  Future<Map<dynamic, dynamic>> startAutoMixerTask(
      Map<dynamic, dynamic> config) async {
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance
          .startAutoMixerTask(jsonEncode(config), completerMap["success"]);
      return completerMap["completer"].future;
    })();

    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = result.errorCode;
    if (result.extendedData) {
      map['extendedData'] = result.extendedData;
    }
    return map;
  }

  Future<Map<dynamic, dynamic>> stopAutoMixerTask(
      Map<dynamic, dynamic> config) async {
    var result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance
          .stopAutoMixerTask(jsonEncode(config), completerMap["success"]);
      return completerMap["completer"].future;
    })();

    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = result.errorCode;
    if (result.extendedData) {
      map['extendedData'] = result.extendedData;
    }
    return map;
  }

  Future<dynamic> takePlayStreamSnapshot(String streamID) async {
    final result = ZegoFlutterEngine.instance.takePlayStreamSnapshot(streamID);
    // final result = await promiseToFuture(promise);

    final Map<dynamic, dynamic> map = {};
    final errorCode = getProperty(result, 'errorCode');
    map['errorCode'] = errorCode;

    map['image'] = null;
    final imageURL = getProperty(result, 'image') as String;

    if (imageURL.startsWith('data:image/')) {
      // 提取 Base64 数据
      final base64Data = imageURL.split(',')[1];
      // 解码 Base64 数据
      Uint8List bytes = base64.decode(base64Data);

      // 返回 MemoryImage
      map['image'] = bytes;
    }

    return map;
  }

  ////////////////////////////////版权音乐相关//////////////////////////////////////////
  Map<String, List<String>> _downloadUrlMap = {};
  Future<int> createCopyrightedMusic() async {
    ZegoFlutterEngine.copyMusic =
        ZegoFlutterEngine.instance.createCopyrightedMusic();
    if (ZegoFlutterEngine.copyMusic != null) {
      return 0;
    } else {
      return 1; // TODO: 错误码处理
    }
  }

  Future<void> destroyCopyrightedMusic() async {
    _downloadUrlMap = {};
    ZegoFlutterEngine.instance.destroyCopyrightedMusic();
    ZegoFlutterEngine.copyMusic = null;
  }

  Future<Map<dynamic, dynamic>> copyrightedMusicInitCopyrightedMusic(
      dynamic config) async {
    dynamic obj = dartObjToJSON(config);
    final promise = ZegoFlutterEngine.copyMusic?.initCopyrightedMusic(obj);
    int code = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = code;
    return map;
  }

  Future<Map<dynamic, dynamic>> copyrightedMusicSendExtendedRequest(
      String command, String params) async {
    final paramsJS = parseJSON(params);
    Object promise =
        ZegoFlutterEngine.copyMusic?.sendExtendedRequest(command, paramsJS);
    final result = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = getProperty(result, 'errorCode');
    map['command'] = command;
    final reqResult = getProperty(result, 'result');
    map['result'] = stringifyJSON(reqResult);
    return map;
  }

  Future<Map<dynamic, dynamic>> copyrightedMusicRequestResource(
      dynamic config, int type) async {
    final promise = ZegoFlutterEngine.copyMusic
        ?.requestResource(dartObjToJSON(config), type);
    final result = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = getProperty(result, 'errorCode');
    map['resource'] = stringifyJSON(getProperty(result, 'resource'));
    return map;
  }

  Future<Map<dynamic, dynamic>> copyrightedMusicRequestResourceV2(
      dynamic config) async {
    int resourceType = 0;
    if (config['resourceType'] != null) {
      resourceType = config['resourceType'];
    }
    return copyrightedMusicRequestResource(config, resourceType);
  }

  Future<dynamic> copyrightedMusicGetSharedResource(
      dynamic config, int type) async {
    final Map<dynamic, dynamic> map = {};
    try {
      final promise = ZegoFlutterEngine.copyMusic
          ?.getSharedResource(dartObjToJSON(config), type);
      final result = await promiseToFuture(promise);
      map['errorCode'] = getProperty(result, 'errorCode');
      map['resource'] = stringifyJSON(getProperty(result, 'resource'));
    } catch (e) {
      dynamic error = e;
      final errorCode = getProperty(error, 'errorCode');
      if (errorCode is int) {
        map['errorCode'] = errorCode;
      }
      throw error;
    }
    return map;
  }

  Future<dynamic> copyrightedMusicGetSharedResourceV2(dynamic config) {
    int resourceType = 0;
    if (config['resourceType'] != null) {
      resourceType = config['resourceType'];
    }
    return copyrightedMusicGetSharedResource(config, resourceType);
  }

  Future<dynamic> copyrightedMusicGetLrcLyric(
      String songID, int vendorID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getLrcLyric(songID, vendorID);
    final result = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = getProperty(result, 'errorCode');
    map['lyrics'] = stringifyJSON(getProperty(result, 'lyrics'));
    return map;
  }

  Future<dynamic> copyrightedMusicGetKrcLyricByToken(String krcToken) async {
    final promise = ZegoFlutterEngine.copyMusic?.getKrcLyricByToken(krcToken);
    final result = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = getProperty(result, 'errorCode');
    map['lyrics'] = stringifyJSON(getProperty(result, 'lyrics'));
    return map;
  }

  Future<dynamic> copyrightedMusicDownload(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.download(resourceID);
    final result = await promiseToFuture(promise);
    final errorCode = getProperty(result, 'errorCode');
    if (errorCode == 0) {
      _downloadUrlMap[resourceID] =
          List<String>.from(getProperty(result, 'urls'));
    }
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = errorCode;
    return map;
  }

  Future<void> copyrightedMusicClearCache() async {
    _downloadUrlMap = {};
    ZegoFlutterEngine.copyMusic?.clearCache();
  }

  // 打分相关接口
  Future<dynamic> copyrightedMusicGetStandardPitch(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getStandardPitch(resourceID);
    final result = await promiseToFuture(promise);
    final Map<dynamic, dynamic> map = {};
    map['errorCode'] = getProperty(result, 'errorCode');
    map['pitch'] = stringifyJSON(getProperty(result, 'pitch'));
    return map;
  }

  Future<dynamic> copyrightedMusicGetCurrentPitch(String resourceID) async {
    final promise =
        await ZegoFlutterEngine.copyMusic?.getCurrentPitch(resourceID);
    int value = await promiseToFuture(promise);
    return value;
  }

  Future<void> copyrightedMusicSetScoringLevel(int level) async {
    ZegoFlutterEngine.copyMusic?.setScoringLevel(level);
  }

  Future<int> copyrightedMusicStartScore(String resourceID) async {
    final promise = ZegoFlutterEngine.instance
        .copyrightedMusicStartScore(ZegoFlutterEngine.copyMusic, resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicStopScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.stopScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicPauseScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.pauseScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicResumeScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.resumeScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicGetPreviousScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getPreviousScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicGetAverageScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getAverageScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicGetTotalScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getTotalScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<int> copyrightedMusicGetFullScore(String resourceID) async {
    final promise = ZegoFlutterEngine.copyMusic?.getFullScore(resourceID);
    final result = await promiseToFuture(promise);
    return result;
  }

  Future<void> startAudioDataObserver(
      int observerBitMask, dynamic param) async {
    final param1 = dartObjToJSON(param);
    callMethod(ZegoFlutterEngine.instance, 'startAudioDataObserver',
        [observerBitMask, param1]);
  }

  Future<void> stopAudioDataObserver() async {
    callMethod(ZegoFlutterEngine.instance, 'stopAudioDataObserver', []);
  }
}
