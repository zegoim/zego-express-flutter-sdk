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

  static dynamic MediaPlayers = {}; // 存储媒体流实例
  static int _index = 0;
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
      case 'startPublishingStream':
        return startPublishingStream(call.arguments["streamID"],
            call.arguments["config"], call.arguments["channel"]);
      case 'stopPublishingStream':
        return stopPublishingStream(call.arguments["channel"]);
      case 'startPlayingStream':
        return startPlayingStream(
            call.arguments["streamID"], call.arguments["canvas"]);
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
        return sendBarrageMessage(call.arguments['roomID'], call.arguments['message']);
      case 'sendCustomCommand':
        return sendCustomCommand(call.arguments['roomID'], call.arguments['command'], call.arguments['toUserList']);
      case 'setRoomExtraInfo':
        return setRoomExtraInfo(call.arguments['roomID'], call.arguments['key'], call.arguments['value']);
      case 'setSEIConfig':
        return setSEIConfig(call.arguments['config']);
      case 'sendSEI':
        return sendSEI(call.arguments['data'], call.arguments['dataLength'], call.arguments['channel']);
      case 'renewToken':
        return renewToken(call.arguments['roomID'], call.arguments['token']);
      case 'mutePlayStreamAudio':
        return mutePlayStreamAudio(call.arguments['streamID'], call.arguments['mute']);
      case 'mutePlayStreamVideo':
        return mutePlayStreamVideo(call.arguments['streamID'], call.arguments['mute']);
      case 'enumDevices':
        return enumDevices();
      case 'getCameras':
        return getCameras();
      case 'getMicrophones':
        return getMicrophones();
      case 'getSpeakers':
        return getSpeakers();
      case 'useVideoDevice':
        return useVideoDevice(call.arguments['type'], call.arguments['deviceID']);
      case 'useAudioDevice':
        return useAudioDevice(call.arguments['type'], call.arguments['deviceID']);
      case 'useAudioOutputDevice':
        return useAudioOutputDevice(call.arguments['mediaID'], call.arguments['deviceID']);
      case 'setEngineConfig':
        return setEngineConfig(call.arguments['config']);
      case 'setStreamExtraInfo':
        return setStreamExtraInfo(call.arguments['extraInfo'], call.arguments['channel']);
      case 'setPluginVersion':
        return;
      case 'createMediaPlayer':
        return createMediaPlayer();
      case 'mediaPlayerSetPlayerCanvas':
        return mediaPlayerSetPlayerCanvas(call.arguments['index'], call.arguments['canvas']);
      case 'mediaPlayerLoadResource':
        return mediaPlayerLoadResource(call.arguments['index'], call.arguments['path']);
      case 'mediaPlayerLoadResourceFromMediaData':
        return mediaPlayerLoadResourceFromMediaData(call.arguments['index'], call.arguments['mediaData'], call.arguments['startPosition']);
      case 'mediaPlayerEnableRepeat':
        return mediaPlayerEnableRepeat(call.arguments['index'], call.arguments['enable']);
      case 'mediaPlayerStart':
        return mediaPlayerStart(call.arguments['index']);
      case 'mediaPlayerPause':
        return mediaPlayerPause(call.arguments['index']);
      case 'mediaPlayerStop':
        return mediaPlayerStop(call.arguments['index']);
      case 'mediaPlayerResume':
        return mediaPlayerResume(call.arguments['index']);
      case 'mediaPlayerSetPlaySpeed':
        return mediaPlayerSetPlaySpeed(call.arguments['index'], call.arguments['speed']);
      case 'mediaPlayerMuteLocal':
        return mediaPlayerMuteLocal(call.arguments['index'], call.arguments['enable']);
      case 'destroyMediaPlayer':
        return destroyMediaPlayer(call.arguments['index']);
      case 'mediaPlayerSetVolume':
        return mediaPlayerSetVolume(call.arguments['index'], call.arguments['volume']);
      case 'mediaPlayerGetTotalDuration':
        return mediaPlayerGetTotalDuration(call.arguments['index']);
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
      "success": allowInterop((res){
        completer.complete(res);
      }),
      "fail":allowInterop((err){
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
        ZegoExpressEngine.onRoomTokenWillExpire!(data['roomID'],data['remainTimeInSecond']);
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

        ZegoExpressEngine.onPublisherQualityUpdate!(
            map['streamID'],
            ZegoPublishStreamQuality(
              map['quality']['videoCaptureFPS'],
              map['quality']['videoEncodeFPS'],
              map['quality']['videoSendFPS'],
              map['quality']['videoKBPS'],
              map['quality']['audioCaptureFPS'],
              map['quality']['audioSendFPS'],
              map['quality']['audioKBPS'],
              map['quality']['rtt'],
              map['quality']['packetLostRate'],
              ZegoStreamQualityLevel.values[map['quality']['level']],
              map['quality']['isHardwareEncode'],
              ZegoVideoCodecID.values[map['quality']['videoCodecID']],
              map['quality']['totalSendBytes'],
              map['quality']['audioSendBytes'],
              map['quality']['videoSendBytes'],
            ));
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

        ZegoExpressEngine.onPlayerQualityUpdate!(
            map['streamID'],
            ZegoPlayStreamQuality(
                map['quality']['videoRecvFPS'],
                map['quality']['videoDejitterFPS'],
                map['quality']['videoDecodeFPS'],
                map['quality']['videoRenderFPS'],
                map['quality']['videoKBPS'],
                map['quality']['videoBreakRate'],
                map['quality']['audioRecvFPS'],
                map['quality']['audioDejitterFPS'],
                map['quality']['audioDecodeFPS'],
                map['quality']['audioRenderFPS'],
                map['quality']['audioKBPS'],
                map['quality']['audioBreakRate'],
                map['quality']['mos'],
                map['quality']['rtt'],
                map['quality']['packetLostRate'],
                map['quality']['peerToPeerDelay'],
                map['quality']['peerToPeerPacketLostRate'],
                ZegoStreamQualityLevel.values[map['quality']['level']],
                map['quality']['delay'],
                map['quality']['avTimestampDiff'],
                map['quality']['isHardwareDecode'],
                ZegoVideoCodecID.values[map['quality']['videoCodecID']],
                map['quality']['totalRecvBytes'],
                map['quality']['audioRecvBytes'],
                map['quality']['videoRecvBytes']));
        break;
      case "onRemoteMicStateUpdate":
        if (ZegoExpressEngine.onRemoteMicStateUpdate == null) return;

        final data = jsonDecode(map["data"]);
        var  state = 10;
        if(data["state"] == 'OPEN') {
          state = 0;
        }
        ZegoExpressEngine.onRemoteMicStateUpdate!(
            data['streamID'], ZegoRemoteDeviceState.values[state]);;
        break;
      case "onRemoteCameraStateUpdate":
        if (ZegoExpressEngine.onRemoteCameraStateUpdate == null) return;
        final data = jsonDecode(map["data"]);
        var  state = 10;
        if(data["state"] == 'OPEN') {
          state = 0;
        }
        ZegoExpressEngine.onRemoteCameraStateUpdate!(
            data['streamID'], ZegoRemoteDeviceState.values[state]);;
        break;
      case "onRemoteSoundLevelUpdate":
        if(ZegoExpressEngine.onRemoteSoundLevelUpdate == null) return;
        var soundLevels = <dynamic, dynamic>{};
        final data = jsonDecode(map["data"]);
        if (data['soundLevels'] != null) {
          soundLevels = data['soundLevels'];
        }
        ZegoExpressEngine.onRemoteSoundLevelUpdate!(Map<String, double>.from(soundLevels));
        break;
      case 'onCapturedSoundLevelUpdate':
        if(ZegoExpressEngine.onCapturedSoundLevelUpdate == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onCapturedSoundLevelUpdate!(data['soundLevel']);break;
      case 'onPlayerVideoSizeChanged':
        if(ZegoExpressEngine.onPlayerVideoSizeChanged == null) return;
        final data = jsonDecode(map["data"]);
        ZegoExpressEngine.onPlayerVideoSizeChanged!(
            data['streamID'], data['width'], data['height']);
        
        break;
      case 'onIMRecvBroadcastMessage':
      if (ZegoExpressEngine.onIMRecvBroadcastMessage == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoBroadcastMessageInfo> messageList= [];
        for (Map<dynamic, dynamic> info in data['messageList']) {
          ZegoBroadcastMessageInfo msg = ZegoBroadcastMessageInfo(
              info['message'],
              info['messageID'],
              info['sendTime'],
              ZegoUser(info['fromUser']['userID'], info['fromUser']['userName']));
          messageList.add(msg);
        }
        ZegoExpressEngine.onIMRecvBroadcastMessage!(data['roomID'], messageList);
        break;
      case 'onIMRecvBarrageMessage':
        if (ZegoExpressEngine.onIMRecvBarrageMessage == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoBarrageMessageInfo> messageList= [];
        for (Map<dynamic, dynamic> info in data['messageList']) {
          ZegoBarrageMessageInfo msg = ZegoBarrageMessageInfo(
              info['message'],
              info['messageID'],
              info['sendTime'],
              ZegoUser(info['fromUser']['userID'], info['fromUser']['userName']));
          messageList.add(msg);
        }
        ZegoExpressEngine.onIMRecvBarrageMessage!(data['roomID'], messageList);
        break;
      case 'onIMRecvCustomCommand':
        if (ZegoExpressEngine.onIMRecvCustomCommand == null) return;
        final data = jsonDecode(map["data"]);

        ZegoUser fromUser = ZegoUser(data['fromUser']['userID'], data['fromUser']['userName']);
        ZegoExpressEngine.onIMRecvCustomCommand!(data['roomID'],fromUser, data['command']);
        break;
      case 'onRoomExtraInfoUpdate':
        if (ZegoExpressEngine.onRoomExtraInfoUpdate == null) return;
        final data = jsonDecode(map["data"]);
        List<ZegoRoomExtraInfo> roomExtraInfoList= [];
        for (Map<dynamic, dynamic> info in data['roomExtraInfoList']) {
          ZegoRoomExtraInfo msg = ZegoRoomExtraInfo(
              info['key'],
              info['value'],
              ZegoUser(info['updateUser']['userID'], info['updateUser']['userName']),
              info['updateTime']);
          roomExtraInfoList.add(msg);
        }
        ZegoExpressEngine.onRoomExtraInfoUpdate!(data['roomID'], roomExtraInfoList);
        break;
      case 'onPlayerRecvSEI':
        if (ZegoExpressEngine.onPlayerRecvSEI == null) return;

        ZegoExpressEngine.onPlayerRecvSEI!(map['streamID'], map['data']);
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
        ZegoExpressEngine.onRoomStreamExtraInfoUpdate!(data['roomID'], streamList);
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
    Profile engineProfile = Profile(appID: appID, server: server);

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
    
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance?.loginRoom(roomID, webUser, webConfig, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    final map = {};
    map["errorCode"] = 0;
    map["extendedData"] = "{}";
    if(result == false) {
      map["errorCode"] = 1;
    }
    return Future.value(map);
  }

  Future<void> logoutRoom(String roomID) {
    ZegoFlutterEngine.instance.logoutRoom(roomID);
    final map = {};
    map["errorCode"] = 0;
    map["extendedData"] = "{}";

    return Future.value(map);
  }

  Future<void> setVideoConfig(dynamic config, int channel) {
    ZegoWebVideoConfig webVideoConfig = ZegoWebVideoConfig(
        captureWidth: config["captureWidth"],
        captureHeight: config["captureHeight"],
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
    previewView.muted = true;
    setVideoMode(canvas["viewMode"], previewView);
    
    ZegoFlutterEngine.instance
        .startPreview(previewView, getPublishChannel(channel));

    return Future.value();
  }

  Future<void> stopPreview(int? channel) {
    ZegoFlutterEngine.instance.stopPreview(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPublishingStream(
      String streamID, dynamic? config, int? channel) {
    ZegoFlutterEngine.instance
        .startPublishingStream(streamID, getPublishChannel(channel));
    return Future.value();
  }

  Future<void> stopPublishingStream(int? channel) {
    ZegoFlutterEngine.instance.stopPublishingStream(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPlayingStream(String streamID, dynamic canvas) {
    final playView = document.getElementById('zego-view-${canvas["view"]}');
    setVideoMode(canvas["viewMode"], playView);
    ZegoFlutterEngine.instance.startPlayingStream(streamID, playView);
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

  Future<void> sendBarrageMessage(String roomID, String message) async {
    await ZegoFlutterEngine.instance.sendBarrageMessage(roomID, message);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;
    map["messageID"] = '';

    return Future.value(map);
  }
  Future<void> sendCustomCommand(String roomID, String message, List toUserList) async {
    await ZegoFlutterEngine.instance.sendCustomCommand(roomID, message, toUserList);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;
    map["messageID"] = 0;

    return Future.value(map);
  }
  Future<void> setRoomExtraInfo(String roomID, String key, String value) async {
    await ZegoFlutterEngine.instance.sendReliableMessage(roomID, key, value);
    final Map<dynamic, dynamic> map = {};
    map["errorCode"] = 0;

    return Future.value(map);
  }
  Future<void> setSEIConfig(Map<dynamic, dynamic> config) async {
    return await ZegoFlutterEngine.instance.setSEIConfig(config);
  }
  Future<void> sendSEI(dynamic data, int dataLength, int channel) async {
    return await ZegoFlutterEngine.instance.sendSEI(data, dataLength, channel);
  }

  Future<void> mutePublishStreamVideo(bool mute, int channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamVideo(mute, getPublishChannel(channel));
  }

  Future<void> mutePublishStreamAudio(bool mute, int channel) async {
    return await ZegoFlutterEngine.instance
        .mutePublishStreamAudio(mute, getPublishChannel(channel));
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
    return await ZegoFlutterEngine.instance.enableCamera(enable, getPublishChannel(channel));
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
  static formatDeviceList(list){
    List<Map<String, dynamic>> newList = [];
    if(list == null || !(list is List)) return newList;
    for(dynamic item in list) {
      newList.add({"deviceID": item["deviceID"], "deviceName": item["deviceName"]});
    }
    return newList;
  }
  Future<Map<dynamic, dynamic>> enumDevices() async {
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.enumDevices(completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    result = jsonDecode(result);
    Map map = {
      'cameras': formatDeviceList(result.cameras),
      'microphones': formatDeviceList(result.microphones),
      'speakers': formatDeviceList(result.speakers),
    };
    return map;
  }
  Future<List> getCameras() async {
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.getCameras(completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return jsonDecode(result);
  }
  Future<List> getMicrophones() async {
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.getMicrophones(completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return jsonDecode(result);
  }
  Future<List> getSpeakers() async {
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.getSpeakers(completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return jsonDecode(result);
  }
  Future<void> useVideoDevice(int type, String deviceID) async {
    return await ZegoFlutterEngine.instance.useVideoDevice(type, deviceID);
  }
  // type：input 0, output 1
  Future<void> useAudioDevice(int type, String deviceID) async {
    return ZegoFlutterEngine.instance.useAudioDevice(type, deviceID);
  }
  Future<void> useAudioOutputDevice(String mediaID, String deviceID) async {
    return await ZegoFlutterEngine.instance.useAudioOutputDevice(document.querySelector('#'+mediaID), deviceID);
  }
  Future<void> setEngineConfig(dynamic config) async {
    return await ZegoFlutterEngine.instance.setEngineConfig(config);
  }
  Future<void> setStreamExtraInfo(String extraInfo, int channel) async {
    await ZegoFlutterEngine.instance.setStreamExtraInfo(extraInfo, channel);
    final map = {};
    map["errorCode"] = 0;
    return Future.value(map);
  }
  Future<int> createMediaPlayer() async {
    var instance = await ZegoFlutterEngine.instance.createMediaPlayer();
    var i = _index;
    var mediaPlayer = new MediaPlayer();
    mediaPlayer.instance = instance;
    MediaPlayers[i] = mediaPlayer;
    _index++;
    return i;
  }
  Future<void> mediaPlayerSetPlayerCanvas(int index, Map<dynamic, dynamic> canvas) async{
    var viewElem = document.getElementById("zego-view-${canvas["view"]}");
    if(MediaPlayers[index] == null) {
      return Future.value();
    }
    setVideoMode(canvas["viewMode"], viewElem);
    var result = await ((){
      Map completerMap = createCompleter();
      ZegoFlutterEngine.instance.mediaPlayerSetPlayerCanvas(viewElem, canvas, MediaPlayers[index].instance, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    return Future.value();
  }
  Future<Map<dynamic, dynamic>> mediaPlayerLoadResource(int index, String path) async {
    final map = {};
    if(MediaPlayers[index] == null) {
      map["errorCode"] = 1;
      return Future.value(map);
    }
    var result = await ((){
      Map completerMap = createCompleter();
      MediaPlayers[index].instance.loadResource(path, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    map["errorCode"] = 0;
    return Future.value(map);
  }
  Future<Map<dynamic, dynamic>> mediaPlayerLoadResourceFromMediaData(int index, dynamic mediaData, int startPosition) async {
    final map = {};
    if(MediaPlayers[index] == null) {
      map["errorCode"] = 1;
      return Future.value(map);
    }
    var result = await ((){
      Map completerMap = createCompleter();
      MediaPlayers[index].instance.loadResourceFromMediaData(mediaData, startPosition, completerMap["success"], completerMap["fail"]);
      return completerMap["completer"].future;
    })();
    map["errorCode"] = 0;
    return Future.value(map);
  }
  
  Future<void> mediaPlayerEnableRepeat(int index, bool enable) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.enableRepeat(enable);
  }
  Future<void> mediaPlayerStart(int index) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.start();
  }
  Future<void> mediaPlayerPause(int index) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.pause();
  }
  Future<void> mediaPlayerStop(int index) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.stop();
  }
  Future<void> mediaPlayerResume(int index) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.resume();
  }
  Future<void> mediaPlayerSetPlaySpeed(int index, double speed) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.setPlaySpeed(speed);
  }
  Future<void> mediaPlayerMuteLocal(int index, bool enable) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    return MediaPlayers[index].instance.muteLocal(enable);
  }
  Future<void> destroyMediaPlayer(int index) async {
    if(MediaPlayers[index] == null) {
      return;
    }
    MediaPlayers[index].instance.destroy();
    MediaPlayers.remove(index);
  }

  Future<void> mediaPlayerSetVolume(int index, int volume) {
    if(MediaPlayers[index] == null) {
      return Future.value();
    }
    return Future.value(MediaPlayers[index].instance.setVolume(volume));
  }
  void setVideoMode(int mode, dynamic elem) {
    // AspectFit 0, AspectFill 1, ScaleToFill 2
    var fit = '';
    switch(mode) {
      case 0: fit = "contain";break;
      case 1: fit = "cover";break;
      case 2: fit = "fill";break;
      default: fit = "none";
    }
    elem.style.objectFit = fit;
  }
  Future<int> mediaPlayerGetTotalDuration(int index) async {
    var duration = MediaPlayers[index].instance.getTotalDuration();
    print("--------------${duration}");
    return Future.value(duration);
  }
}





