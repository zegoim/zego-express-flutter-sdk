// ignore_for_file: library_names

/*
 * @Author: sunboylu
 * @Date: 2021-01-11 19:33:44
 * @LastEditors: sunboylu
 * @LastEditTime: 2021-09-27 20:51:31
 * @Description:
 */

@JS("ZegoFlutterEngine")
library ZegoFlutterEngine;

import 'package:js/js.dart';

@JS()
class ZegoFlutterEngine {
  external dynamic zegoEntity;
  external static ZegoFlutterEngine instance;
  external static destroyEngine();
  external static getVersion();
  external static createEngineWithProfile(dynamic profile);
  external static setEventHandler(Function handler);
  external loginRoom(String roomID, dynamic user, dynamic config, dynamic success, dynamic fail );
  external logoutRoom(String roomID);
  external setVideoConfig(dynamic config, int channel);
  external getVideoConfig(int channel);
  external startPreview(dynamic canvas, int channel);
  external stopPreview(int channel);
  external startPublishingStream(String streamID, int? channel);
  external stopPublishingStream(int channel);
  external startPlayingStream(String streamID, dynamic remoteVideo);
  external stopPlayingStream(String streamID);
  external sendBroadcastMessage(String roomID, String message);
  external sendBarrageMessage(String roomID, String message);
  external sendCustomCommand(String roomID, String message, List toUserList);
  external setSEIConfig(Map<dynamic, dynamic> config);
  external mutePublishStreamVideo(bool mute, int channel);
  external mutePublishStreamAudio(bool mute, int channel);
  external enableAEC(bool enable);
  external enableAGC(bool enable);
  external enableANS(bool enable);
  external enableCamera(bool enable, int channel);
  external renewToken(String roomID, String token);
  external mutePlayStreamAudio(String streamID, bool mute);
  external mutePlayStreamVideo(String streamID, bool mute);
  @JS('enumDevices')
  external enumDevices(dynamic success, dynamic fail);
  external getCameras(dynamic success, dynamic fail);
  external getMicrophones(dynamic success, dynamic fail);
  external getSpeakers(dynamic success, dynamic fail);
  external useVideoDevice(int channel, String deviceID);
  external useAudioDevice(int channel, String deviceID);
  external useAudioOutputDevice(dynamic media, String deviceID);
  external setEngineConfig(dynamic config);
  external setStreamExtraInfo(String extraInfo, int channel);
  external sendSEI(dynamic data, int dataLength, int channel);
  external sendReliableMessage(String roomID, String key, String value);
  external createMediaPlayer();
  external mediaPlayerSetPlayerCanvas(dynamic viewElem, Map<dynamic, dynamic> canvas, dynamic MediaPlayer, dynamic success, dynamic fail);
}

@JS()
@anonymous
class StreamListInfo {
  external factory StreamListInfo({
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
class ZegoWebVideoConfig {
  external factory ZegoWebVideoConfig(
      {int captureWidth, int captureHeight, int fps, int bitrate, int codecID});

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

@JS("MediaPlayer")
@anonymous
class MediaPlayer {
  external MediaPlayer instance;
  external enableRepeat(bool enable);
  external loadResource(String path, dynamic success, dynamic fail);
  external loadResourceFromMediaData(dynamic mediaData,int startPosition, dynamic success, dynamic fail);
  external start();
  external pause();
  external stop();
  external resume();
  external setPlaySpeed(double speed);
  external muteLocal(bool enable);
  external getTotalDuration();
  external setVolume(int volume );
  external destroy();
}