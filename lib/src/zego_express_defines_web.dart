/*
 * @Author: sunboylu
 * @Date: 2021-01-11 19:33:44
 * @LastEditors: sunboylu
 * @LastEditTime: 2021-09-27 20:51:31
 * @Description:
 */

@JS("ZegoFlutterEngine")
library ZegoFlutterEngine;

import 'dart:html';

import 'package:js/js.dart';

@JS()
class ZegoFlutterEngine {
  external dynamic zegoEntity;
  external static ZegoFlutterEngine instance;
  external static destroyEngine();
  external static getVersion();
  external static createEngineWithProfile(dynamic profile);
  external static setEventHandler(Function handler);
  external loginRoom(String roomID, dynamic user, dynamic config);
  external logoutRoom(String roomID);
  external setVideoConfig(dynamic config, int channel);
  external getVideoConfig(int channel);
  external startPreview(dynamic canvas, int channel);
  external stopPreview(int channel);
  external startPublishingStream(String streamID, int? channel);
  external stopPublishingStream(String streamID, int channel);
  external startPlayingStream(String streamID, dynamic remoteVideo);
  external stopPlayingStream(String streamID);
  external sendBroadcastMessage(String roomID, String message);
  external mutePublishStreamVideo(dynamic canvas, bool mute);
  external mutePublishStreamAudio(dynamic canvas, bool mute);
}

@JS()
@anonymous
class streamListInfo {
  external factory streamListInfo({
    String soundLevel,
    String streamID,
  });

  external String get userID;
  external String get userName;
}

@JS()
@anonymous
class Profile {
  external factory Profile({
    int appID,
    String server,
  });

  external int get appID;
  external String get server;
}

@JS()
@anonymous
class ZegoUserWeb {
  external factory ZegoUserWeb({
    String userID,
    String userName,
  });

  external String get userID;
  external String get userName;
}

@JS()
@anonymous
class ZegoRoomConfigWeb {
  external factory ZegoRoomConfigWeb({
    int maxMemberCount,
    bool isUserStatusNotify,
    String token,
  });

  external int get maxMemberCount;
  external bool get isUserStatusNotify;
  external String get token;
}
@JS()
@anonymous
class ZegoWebVideoConfig{
  external factory ZegoWebVideoConfig({
    int captureWidth,
    int captureHeight,
    int fps,
    int bitrate,
    int codecID
  });

  external int get captureWidth;
  external int get captureHeight;
  external int get fps;
  external int get bitrate;
  external int codecID;
}

enum ZegoWebVideoCodecID {
  Default,

  Vp8
}
