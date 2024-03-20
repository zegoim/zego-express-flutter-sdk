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
  external loginRoom(String roomID, dynamic user, dynamic config,
      dynamic success, dynamic fail);
  external logoutRoom(String roomID);
  external setVideoConfig(dynamic config, int channel);
  external getVideoConfig(int channel);
  external startPreview(dynamic canvas, int channel, bool isPublish,
      dynamic success, dynamic fail);
  external stopPreview(int channel);
  external startPublishingStream(String streamID, int? channel, dynamic config);
  external stopPublishingStream(int channel);
  external startPlayingStream(
      String streamID, dynamic remoteVideo, String s, dynamic config);
  external stopPlayingStream(String streamID);
  external sendBroadcastMessage(String roomID, String message);
  external sendBarrageMessage(String roomID, String message);
  external sendCustomCommand(String roomID, String message, List toUserList);
  external setSEIConfig(int config);
  external mutePublishStreamVideo(bool mute, int channel);
  external mutePublishStreamAudio(bool mute, int channel);
  external muteMicrophone(bool mute);
  external isMicrophoneMuted();
  external enableAEC(bool enable);
  external enableAGC(bool enable);
  external enableANS(bool enable);
  external enableCamera(bool enable, int channel);
  external useFrontCamera(bool enable, int channel);
  external renewToken(String roomID, String token);
  external mutePlayStreamAudio(String streamID, bool mute);
  external mutePlayStreamVideo(String streamID, bool mute);
  external getAudioDeviceList(int type, dynamic success, dynamic fail);
  external getVideoDeviceList(dynamic success, dynamic fail);
  external useVideoDevice(String deviceID, int channel);
  external useAudioDevice(int type, String deviceID);
  external useAudioOutputDevice(dynamic media, String deviceID);
  external static setEngineConfig(dynamic config);
  external setStreamExtraInfo(String extraInfo, int channel);
  external sendSEI(dynamic data, int dataLength, int channel);
  external setRoomExtraInfo(String roomID, String key, String value);
  external createMediaPlayer();
  external setStyleByCanvas(dynamic canvas);
  external mediaPlayerSetPlayerCanvas(
      dynamic viewElem,
      Map<dynamic, dynamic> canvas,
      dynamic mediaPlayer,
      dynamic success,
      dynamic fail);
  external mediaPlayerEnableAux(bool enable, dynamic mediaPlayer);
  external mediaPlayerSetVolume(int volume, dynamic mediaPlayer);
  external setRoomScenario(int scenario);
  external static setRoomMode(int mode);
  external enableEffectsBeauty(bool enable);
  external stopEffectsEnv();
  external setEffectsBeautyParam(dynamic param);
  external createScreenCaptureSource(int? sourceId);
  external startCaptureScreen(int index, dynamic config);
  external setVideoSource(int source, int? instanceID, int? channel);
  external setAudioSource(int source, int? channel);
  external stopCaptureScreen(int index);
  external startMixerTask(String config, dynamic success, dynamic fail);
  external stopMixerTask(String taskId, dynamic success, dynamic fail);
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
  external factory Profile({int appID, String server, int scenario});

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
      {int encodeWidth, int encodeHeight, int fps, int bitrate, int codecID});

  external int get encodeWidth;
  external int get encodeHeight;
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
  external loadResourceFromMediaData(
      dynamic mediaData, int startPosition, dynamic success, dynamic fail);
  external start();
  external pause();
  external stop();
  external resume();
  external setPlaySpeed(double speed);
  external muteLocal(bool enable);
  external getTotalDuration();
  external destroy();
}

@JS("ScreenCaptureSource")
@anonymous
class ScreenCaptureSource {
  external ScreenCaptureSource instance;
  external dynamic engine;
  external int sourceId;
  external dynamic stream;
  external String sourceType;
  external dynamic config;
  external List channels;
  external stopCapture();
  external startCapture(dynamic config);
}

@JS("PublishConfig")
@anonymous
class PublishConfig {
  external factory PublishConfig({String roomID});
  external String get roomID;
}

@JS("EffectsBeautyParam")
@anonymous
class EffectsBeautyParam {
  external factory EffectsBeautyParam(
      {int? whitenIntensity,
      int? osyIntensity,
      int? smoothIntensity,
      int? sharpenIntensity});
  external int whitenIntensity;
  external int osyIntensity;
  external int smoothIntensity;
  external int sharpenIntensity;
}
