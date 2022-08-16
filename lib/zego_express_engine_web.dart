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
      case 'setPluginVersion':
        return;
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

  static void _eventListener(dynamic event) {
    final Map<dynamic, dynamic> map = event;
    switch (map["methodName"]) {
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

    ZegoFlutterEngine.instance.loginRoom(roomID, webUser, webConfig);

    final map = {};
    map["errorCode"] = 0;
    map["extendedData"] = "{}";

    return Future.value(map);
  }

  Future<void> logoutRoom(String roomID) {
    ZegoFlutterEngine.instance.logoutRoom(roomID);
    return Future.value();
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
    ZegoFlutterEngine.instance
        .startPreview(previewView, getPublishChannel(channel));

    return Future.value();
  }

  Future<void> stopPreview(int? channel) {
    ZegoFlutterEngine.instance.stopPreview(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPublishingStream(
      String streamID, dynamic config, int? channel) {
    ZegoFlutterEngine.instance
        .startPublishingStream(streamID, getPublishChannel(channel));
    return Future.value();
  }

  Future<void> stopPublishingStream(int? channel) {
    ZegoFlutterEngine.instance.stopPublishingStream(getPublishChannel(channel));
    return Future.value();
  }

  Future<void> startPlayingStream(String streamID, dynamic canvas) {
    final playView = document.getElementById("zego-view-${canvas["view"]}");
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
    ZegoFlutterEngine.instance.enableCamera(enable, getPublishChannel(channel));
  }
}
