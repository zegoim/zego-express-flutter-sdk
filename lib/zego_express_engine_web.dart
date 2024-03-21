// ignore_for_file: prefer_typing_uninitialized_variables

import 'dart:async';
import 'dart:convert';
// In order to *not* need this ignore, consider extracting the "web" version
// of your plugin as a separate package, instead of inlining it in the same
// package as the core of your plugin.
// ignore: avoid_web_libraries_in_flutter
import 'dart:html';
import 'dart:js';

import 'package:flutter/services.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'package:zego_express_engine/src/zego_express_defines_web.dart';

import 'zego_express_engine.dart';

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

    var element = ScriptElement()
      ..src =
          'assets/packages/zego_express_engine/assets/ZegoExpressWebFlutterWrapper.js'
      ..type = 'application/javascript';
    document.body!.append(element);
  }

  /// Handles method calls over the MethodChannel of this plugin.
  /// Note: Check the "federated" architecture for a new way of doing this:
  /// https://flutter.dev/go/federated-plugins
  Future<dynamic> handleMethodCall(MethodCall call) async {
    // var args = <String, dynamic>{};
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
            allowInterop((String event, String data) {
          _evenController.add({'methodName': event, 'data': data});
        }));
        return createEngineWithProfile(call.arguments['profile']);
      case 'getVersion':
        return getVersion();
      case 'loginRoom':
        return loginRoom(call.arguments['roomID'], call.arguments['user'],
            call.arguments['config']);
      case 'logoutRoom':
        return logoutRoom(call.arguments['roomID']);
      case 'setVideoConfig':
        return setVideoConfig(
            call.arguments["config"], call.arguments["channel"]);
      case 'getVideoConfig':
        return getVideoConfig(call.arguments["channel"]);
      case 'enableAEC':
        return enableAEC(call.arguments["enable"]);
      case 'enableAGC':
        return enableAGC(call.arguments["enable"]);
      case 'enableANS':
        return enableANS(call.arguments["enable"]);
      case 'enableCamera':
        return enableCamera(
            call.arguments["enable"], call.arguments["channel"]);
      case 'useFrontCamera':
        return useFrontCamera(
            call.arguments["enable"], call.arguments["channel"]);
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
      case 'muteMicrophone':
        return muteMicrophone(call.arguments["mute"]);
      case 'isMicrophoneMuted':
        return isMicrophoneMuted();
      case 'startPublishingStream':
        return startPublishingStream(call.arguments["streamID"],
            call.arguments["config"], call.arguments["channel"]);
      case 'stopPublishingStream':
        return stopPublishingStream(call.arguments["channel"]);
      case 'startPlayingStream':
        return startPlayingStream(call.arguments["streamID"],
            call.arguments["canvas"], call.arguments["config"]);
      case 'stopPlayingStream':
        return stopPlayingStream(call.arguments["streamID"]);
      case 'destroyPlatformView':
        return destroyPlatformView(call.arguments["viewID"]);
      case 'destroyEngine':
        return destroyEngine();
      case 'sendBroadcastMessage':
        return sendBroadcastMessage(
            call.arguments["roomID"], call.arguments["message"]);
      case 'sendBarrageMessage':
        return sendBarrageMessage(
            call.arguments['roomID'], call.arguments['message']);
      case 'sendCustomCommand':
        return sendCustomCommand(call.arguments['roomID'],
            call.arguments['command'], call.arguments['toUserList']);
      case 'setRoomExtraInfo':
        return setRoomExtraInfo(call.arguments['roomID'], call.arguments['key'],
            call.arguments['value']);
      case 'setSEIConfig':
        return setSEIConfig(call.arguments['config']);
      case 'sendSEI':
        return sendSEI(call.arguments['data'], call.arguments['dataLength'],
            call.arguments['channel']);
      case 'renewToken':
        return renewToken(call.arguments['roomID'], call.arguments['token']);
      case 'mutePlayStreamAudio':
        return mutePlayStreamAudio(
            call.arguments['streamID'], call.arguments['mute']);
      case 'mutePlayStreamVideo':
        return mutePlayStreamVideo(
            call.arguments['streamID'], call.arguments['mute']);
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
      case 'setEngineConfig':
        return setEngineConfig(call.arguments['config']);
      case 'setStreamExtraInfo':
        return setStreamExtraInfo(
            call.arguments['extraInfo'], call.arguments['channel']);
      case 'setPluginVersion':
        return;
      case 'createMediaPlayer':
        return createMediaPlayer();
      case 'mediaPlayerSetPlayerCanvas':
        return mediaPlayerSetPlayerCanvas(
            call.arguments['index'], call.arguments['canvas']);
      case 'mediaPlayerLoadResource':
        return mediaPlayerLoadResource(
            call.arguments['index'], call.arguments['path']);
      case 'mediaPlayerLoadResourceFromMediaData':
        return mediaPlayerLoadResourceFromMediaData(call.arguments['index'],
            call.arguments['mediaData'], call.arguments['startPosition']);
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
      case 'setRoomScenario':
        return setRoomScenario(call.arguments['scenario']);
      case 'setRoomMode':
        return setRoomMode(call.arguments['mode']);
      case 'enableEffectsBeauty':
        return enableEffectsBeauty(call.arguments['enable']);
      case 'setEffectsBeautyParam':
        return setEffectsBeautyParam(call.arguments['param']);
      case 'startEffectsEnv':
        return;
      case 'getScreenCaptureSources':
        return;
      case 'createScreenCaptureSource':
        return createScreenCaptureSource(
            call.arguments['sourceId'], call.arguments['sourceType']);
      case 'destroyScreenCaptureSource':
        return destroyScreenCaptureSource(call.arguments['index']);
      case 'stopEffectsEnv':
        return stopEffectsEnv();
      case 'setVideoSource':
        return setVideoSource(call.arguments['source'],
            call.arguments['instanceID'], call.arguments['channel']);
      case 'setAudioSource':
        return setAudioSource(
            call.arguments['source'], call.arguments['channel']);
      case 'startCaptureScreenCaptureSource':
        return startCaptureScreen(
            call.arguments['index'], call.arguments['config']);
      case 'stopCaptureScreenCaptureSource':
        return stopCaptureScreen(call.arguments['index']);
      case 'startMixerTask':
        return startMixerTask(call.arguments);
      case 'stopMixerTask':
        return stopMixerTask(call.arguments["taskID"]);
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

        ZegoExpressEngine.onRoomOnlineUserCountUpdate!(
            map['roomID'], map['count']);
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
                false,
                ZegoVideoCodecID.values[quality['videoCodecID']],
                0,
                0,
                0));
        break;
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
                0,
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
      case "onRemoteMicStateUpdate":
        if (ZegoExpressEngine.onRemoteMicStateUpdate == null) return;

        final data = jsonDecode(map["data"]);
        var state = 10;
        if (data["state"] == 'OPEN') {
          state = 0;
        }
        ZegoExpressEngine.onRemoteMicStateUpdate!(
            data['streamID'], ZegoRemoteDeviceState.values[state]);
        break;
      case "onRemoteCameraStateUpdate":
        if (ZegoExpressEngine.onRemoteCameraStateUpdate == null) return;
        final data = jsonDecode(map["data"]);
        var state = 10;
        if (data["state"] == 'OPEN') {
          state = 0;
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
      case 'onPlayerVideoSizeChanged':
        if (ZegoExpressEngine.onPlayerVideoSizeChanged == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onPlayerVideoSizeChanged!(
            data['streamID'], data['width'], data['height']);

        break;
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
      case 'onPlayerRecvSEI':
        if (ZegoExpressEngine.onPlayerRecvSEI == null) return;
        final data = jsonDecode(map["data"]);
        var bytes = Uint8List.fromList(utf8.encode(data["data"]));
        ZegoExpressEngine.onPlayerRecvSEI!(data['streamID'], bytes);
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
      case 'onDebugError':
        if (ZegoExpressEngine.onDebugError == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onDebugError!(
            data['errorCode'], data['funcName'], data['info']);
        break;
      default:
        break;
    }
  }

  static getPublishChannel(int? channel) {
    if (channel != null) {
      return channel;
    }

    return 0;
  }

  static Future<void> createEngineWithProfile(dynamic profile) async {
    final appID = profile["appID"];
    const server = "wss://test.com";
    Profile engineProfile =
        Profile(appID: appID, server: server, scenario: profile['scenario']);

    ZegoFlutterEngine.createEngineWithProfile(engineProfile);
  }

  Future<void> destroyEngine() {
    ZegoFlutterEngine.destroyEngine();

    return Future.value();
  }

  Future<Map<dynamic, dynamic>> loginRoom(
      String roomID, dynamic user, dynamic config) async {
    ZegoUserWeb webUser =
        ZegoUserWeb(userID: user["userID"], userName: user["userName"]);
    ZegoRoomConfigWeb webConfig = ZegoRoomConfigWeb(
        maxMemberCount: config["maxMemberCount"],
        token: config["token"],
        isUserStatusNotify: config["isUserStatusNotify"]);
    var result;
    result = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.loginRoom(roomID, webUser, webConfig,
          completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    final map = {};
    map["errorCode"] = 0;
    map["extendedData"] = "{}";
    if (result == false) {
      map["errorCode"] = 1;
    }
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> logoutRoom(String? roomID) {
    roomID ??= "";
    ZegoFlutterEngine.instance.logoutRoom(roomID);
    final map = {};
    map["errorCode"] = 0;
    map["extendedData"] = "{}";

    return Future.value(map);
  }

  Future<void> setVideoConfig(dynamic config, int channel) {
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

  Future<Map<dynamic, dynamic>> getVideoConfig(int? channel) async {
    var config =
        ZegoFlutterEngine.instance.getVideoConfig(getPublishChannel(channel));
    var map = {};
    map['captureWidth'] = config.captureWidth;
    map['captureHeight'] = config.captureHeight;
    map['encodeWidth'] = config.captureWidth;
    map['encodeHeight'] = config.captureHeight;
    map['fps'] = config.fps;
    map['bitrate'] = config.bitrate;
    map['codecID'] = config.codecID;

    return Future.value(map);
  }

  Future<void> startPreview(dynamic canvas, int channel) async {
    previewView = document.getElementById("zego-view-${canvas["view"]}");
    previewView?.muted = true;
    ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));

    await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.startPreview(
          previewView,
          getPublishChannel(channel),
          false,
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

  Future<void> startPlayingStream(
      String streamID, dynamic canvas, dynamic config) {
    final playView = document.getElementById('zego-view-${canvas["view"]}');
    ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));
    ZegoFlutterEngine.instance
        .startPlayingStream(streamID, playView, "", jsonEncode(config));
    return Future.value();
  }

  Future<void> stopPlayingStream(String streamID) {
    ZegoFlutterEngine.instance.stopPlayingStream(streamID);
    return Future.value();
  }

  Future<Map<dynamic, dynamic>> sendBroadcastMessage(
      String roomID, String message) async {
    ZegoFlutterEngine.instance.sendBroadcastMessage(roomID, message);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;
    map["messageID"] = 0;

    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> sendBarrageMessage(
      String roomID, String message) async {
    await ZegoFlutterEngine.instance.sendBarrageMessage(roomID, message);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;
    map["messageID"] = '';

    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> sendCustomCommand(
      String roomID, String message, List toUserList) async {
    List useridList = [];
    for (var item in toUserList) {
      useridList.add(item["userID"]);
    }
    await ZegoFlutterEngine.instance
        .sendCustomCommand(roomID, message, useridList);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;
    map["messageID"] = 0;

    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> setRoomExtraInfo(
      String roomID, String key, String value) async {
    await ZegoFlutterEngine.instance.setRoomExtraInfo(roomID, key, value);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;

    return Future.value(map);
  }

  Future<void> setSEIConfig(Map<dynamic, dynamic> config) async {
    return await ZegoFlutterEngine.instance.setSEIConfig(config['type']);
  }

  Future<void> sendSEI(dynamic data, int dataLength, int channel) async {
    return await ZegoFlutterEngine.instance
        .sendSEI(const Utf8Decoder().convert(data), dataLength, channel);
  }

  Future<void> mutePublishStreamVideo(bool mute, int channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamVideo(mute, getPublishChannel(channel));
  }

  Future<void> mutePublishStreamAudio(bool mute, int channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamAudio(mute, getPublishChannel(channel));
  }

  Future<void> muteMicrophone(bool mute) async {
    return await ZegoFlutterEngine.instance.muteMicrophone(mute);
  }

  Future<void> isMicrophoneMuted() async {
    return await ZegoFlutterEngine.instance.isMicrophoneMuted();
  }

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

  Future<void> renewToken(String roomID, String token) async {
    return Future.value(ZegoFlutterEngine.instance.renewToken(roomID, token));
  }

  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    await ZegoFlutterEngine.instance.mutePlayStreamAudio(streamID, mute);
  }

  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    await ZegoFlutterEngine.instance.mutePlayStreamVideo(streamID, mute);
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

  Future<void> setEngineConfig(dynamic config) async {
    return await ZegoFlutterEngine.setEngineConfig(config);
  }

  Future<Map<dynamic, dynamic>> setStreamExtraInfo(
      String extraInfo, int channel) async {
    await ZegoFlutterEngine.instance.setStreamExtraInfo(extraInfo, channel);
    final map = {};
    map["errorCode"] = 0;
    return Future.value(map);
  }

  Future<int> createMediaPlayer() async {
    var instance = await ZegoFlutterEngine.instance.createMediaPlayer();
    var i = _index;
    var mediaPlayer = MediaPlayer();
    mediaPlayer.instance = instance;
    _mediaPlayers[i] = mediaPlayer;
    _index++;
    return i;
  }

  Future<void> mediaPlayerSetPlayerCanvas(
      int index, Map<dynamic, dynamic> canvas) async {
    var viewElem = document.getElementById("zego-view-${canvas["view"]}");
    if (_mediaPlayers[index] == null) {
      return Future.value();
    }
    ZegoFlutterEngine.instance.setStyleByCanvas(jsonEncode(canvas));
    await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.mediaPlayerSetPlayerCanvas(
          viewElem,
          canvas,
          _mediaPlayers[index].instance,
          completerMap["success"],
          completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return Future.value();
  }

  Future<Map<dynamic, dynamic>> mediaPlayerLoadResource(
      int index, String path) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = 1;
      return Future.value(map);
    }
    await (() {
      Map completerMap = createCompleter();
      _mediaPlayers[index]
          .instance
          .loadResource(path, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    map["errorCode"] = 0;
    return Future.value(map);
  }

  Future<Map<dynamic, dynamic>> mediaPlayerLoadResourceFromMediaData(
      int index, dynamic mediaData, int startPosition) async {
    final map = {};
    if (_mediaPlayers[index] == null) {
      map["errorCode"] = 1;
      return Future.value(map);
    }
    await (() {
      Map completerMap = createCompleter();
      _mediaPlayers[index].instance.loadResourceFromMediaData(mediaData,
          startPosition, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    map["errorCode"] = 0;
    return Future.value(map);
  }

  Future<void> mediaPlayerEnableRepeat(int index, bool enable) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.enableRepeat(enable);
  }

  Future<void> mediaPlayerStart(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.start();
  }

  Future<void> mediaPlayerPause(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.pause();
  }

  Future<void> mediaPlayerStop(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.stop();
  }

  Future<void> mediaPlayerResume(int index) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.resume();
  }

  Future<void> mediaPlayerSetPlaySpeed(int index, double speed) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.setPlaySpeed(speed);
  }

  Future<void> mediaPlayerMuteLocal(int index, bool enable) async {
    if (_mediaPlayers[index] == null) {
      return;
    }
    return _mediaPlayers[index].instance.muteLocal(enable);
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
    _mediaPlayers[index].instance.destroy();
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
    var duration = _mediaPlayers[index].instance.getTotalDuration();
    return Future.value(duration);
  }

  Future<void> setRoomScenario(int scenario) async {
    return ZegoFlutterEngine.instance.setRoomScenario(scenario);
  }

  Future<void> setRoomMode(int mode) async {
    return ZegoFlutterEngine.setRoomMode(mode);
  }

  Future<void> enableEffectsBeauty(bool enable) async {
    return ZegoFlutterEngine.instance.enableEffectsBeauty(enable);
  }

  Future<void> stopEffectsEnv() async {
    return ZegoFlutterEngine.instance.stopEffectsEnv();
  }

  Future<void> setEffectsBeautyParam(dynamic param) async {
    EffectsBeautyParam options = EffectsBeautyParam(
        whitenIntensity: param["whitenIntensity"],
        osyIntensity: param["osyIntensity"],
        smoothIntensity: param["smoothIntensity"],
        sharpenIntensity: param["sharpenIntensity"]);
    return ZegoFlutterEngine.instance.setEffectsBeautyParam(options);
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

  Future<int> setVideoSource(int source, int? instanceID, int? channel) async {
    return ZegoFlutterEngine.instance
        .setVideoSource(source, instanceID, getPublishChannel(channel));
  }

  Future<int> setAudioSource(int source, int? channel) async {
    return ZegoFlutterEngine.instance
        .setAudioSource(source, getPublishChannel(channel));
  }

  Future<void> startCaptureScreen(int index, dynamic config) async {
    return _mediaSources[index].instance.startCapture(config);
  }

  Future<void> stopCaptureScreen(int index) async {
    return _mediaSources[index].instance.stopCapture();
  }

  Future<Map<dynamic, dynamic>> startMixerTask(
      Map<dynamic, dynamic> config) async {
    config["userData"] = const Utf8Decoder().convert(config["userData"]);
    var data = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.startMixerTask(
          jsonEncode(config), completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return json.decode(data);
  }

  Future<Map<dynamic, dynamic>> stopMixerTask(String taskID) async {
    var data = await (() {
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance
          .stopMixerTask(taskID, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return json.decode(data);
  }
}
