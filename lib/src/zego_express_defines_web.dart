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
  external static ZegoCopyrightedMusic? copyMusic;

  /////////////////////////////初始化相关的前置功能////////////////////////////////////////
  external static createEngineWithProfile(dynamic profile, String framework);
  external static destroyEngine();
  external static getVersion();
  external static presetLogConfig(dynamic config);
  external static setEventHandler(Function handler);
  external static use(dynamic module);
  external setRoomScenario(int scenario);
  external static setRoomMode(int mode);
  external static setEngineConfig(dynamic config);
  external static setGeoFence(int type, dynamic areaList);
  external static setLocalProxyConfig(
      ZegoLocalProxyConfigWeb proxyConfig, bool enable);
  external static setCloudProxyConfig(
      List<ZegoProxyInfoWeb> proxyList, String token, bool enable);

  /////////////////////////////房间////////////////////////////////////////
  external loginRoom(String roomID, dynamic user, dynamic config,
      void Function(int errorCode) success, void Function(int errorCode) fail);
  external logoutRoom(
      void Function(int errorCode, String msg) onresult, String? roomID);
  external switchRoom(String fromRoomID, String toRoomID, dynamic config);
  external renewToken(String roomID, String token);
  external setRoomExtraInfo(String roomID, String key, String value,
      void Function(int errorCode) success, void Function(int errorCode) error);
  // external getRoomStreamList(String roomID, int streamListType)

  /////////////////////////////推流////////////////////////////////////////
  external startPublishingStream(String streamID, int? channel, dynamic config);
  external stopPublishingStream(int channel);
  external setStreamExtraInfo(String extraInfo, int channel);
  external startPreview(dynamic canvas, int channel, dynamic canvasOptions,
      dynamic success, dynamic fail);
  external stopPreview(int channel);
  external setVideoConfig(dynamic config, int channel);
  external getVideoConfig(int channel);
  //https://doc-zh.zego.im/unique-api/express-video-sdk/zh/dart_flutter/zego_express_engine/ZegoExpressEnginePublisher/setPublishDualStreamConfig.html
  //setPublishDualStreamConfig(List<ZegoPublishDualStreamConfig> configList, int channel)

  //https://doc-zh.zego.im/unique-api/express-video-sdk/zh/dart_flutter/zego_express_engine/ZegoExpressEnginePublisher/setVideoMirrorMode.html
  external setVideoMirrorMode(int mirrorMode, int channel);

  //https://doc-zh.zego.im/unique-api/express-video-sdk/zh/dart_flutter/zego_express_engine/ZegoExpressEnginePublisher/setAppOrientation.html
  //setAppOrientation

  external setAudioConfig(dynamic config, int channel);
  external getAudioConfig(int channel);

  //https://doc-zh.zego.im/unique-api/express-video-sdk/zh/dart_flutter/zego_express_engine/ZegoExpressEnginePublisher/setPublishStreamEncryptionKey.html
  //setPublishStreamEncryptionKey

  //https://doc-zh.zego.im/unique-api/express-video-sdk/zh/dart_flutter/zego_express_engine/ZegoExpressEnginePublisher/takePublishStreamSnapshot.html
  //takePublishStreamSnapshot(int channel)
  external mutePublishStreamVideo(bool mute, int channel);
  external mutePublishStreamAudio(bool mute, int channel);
  external setStreamAlignmentProperty(int alignment, int channel);

  external enableTrafficControl(bool enable, int channel);
  external setMinVideoBitrateForTrafficControl(
      int bitrate, int mode, int channel);
  // setMinVideoFpsForTrafficControl
  // setMinVideoResolutionForTrafficControl
  external setTrafficControlFocusOn(int mode, int channel);
  external setCaptureVolume(int volume);
  // setAudioCaptureStereoMode
  external addPublishCdnUrl(
      String streamID, String targetURL, dynamic success, dynamic fail);
  external removePublishCdnUrl(
      String streamID, String targetURL, dynamic success, dynamic fail);
  // enablePublishDirectToCDN
  // external setPublishWatermark
  external setSEIConfig(int type);
  external sendSEI(dynamic data, int dataLength, int channel);
  // sendAudioSideInfo
  external enableHardwareEncoder(bool enable);
  // setCapturePipelineScaleMode
  // setDummyCaptureImagePath
  // enableH265EncodeFallback
  // isVideoEncoderSupported
  // setAppOrientationMode
  external setLowlightEnhancement(int mode, int channel);
  external setVideoSource(int source, int instanceID, int channel);
  external setAudioSource(int source, int channel);
  // enableVideoObjectSegmentation
  // enableAlphaChannelVideoEncoder
  // setCameraStabilizationMode

  /////////////////////////////拉流////////////////////////////////////////
  external startPlayingStream(String streamID, dynamic remoteVideo,
      String audioOutput, dynamic config, dynamic canvasOptions);
  //switchPlayingStream
  external stopPlayingStream(String streamID);
  // setPlayStreamDecryptionKey
  // setPlayStreamCrossAppInfo
  // takePlayStreamSnapshot
  external setPlayVolume(String streamID, int volume);
  external setAllPlayStreamVolume(int volume);
  // setPlayStreamVideoType
  // setPlayStreamBufferIntervalRange
  // setPlayStreamFocusOn
  external mutePlayStreamAudio(String streamID, bool mute);
  external mutePlayStreamVideo(String streamID, bool mute);
  // muteAllPlayStreamAudio
  external muteAllPlayAudioStreams(bool mute);
  // muteAllPlayStreamVideo
  external muteAllPlayVideoStreams(bool mute);

  // external enableHardwareDecoder(bool enable);
  // enableCheckPoc
  // isVideoDecoderSupported
  // setPlayStreamsAlignmentProperty
  // enableVideoSuperResolution
  // initVideoSuperResolution
  // uninitVideoSuperResolution
  external updatePlayingCanvas(
      String streamID, dynamic view, dynamic canvasOptions);

/////////////////////////////消息////////////////////////////////////////
  external createRealTimeSequentialDataManager(String roomID);
  external destroyRealTimeSequentialDataManager(int index);
  external startBroadcasting(int index, String streamID);
  external stopBroadcasting(int index, String streamID);
  external sendRealTimeSequentialData(int index, String streamID, dynamic data);
  external startSubscribing(int index, String streamID);
  external stopSubscribing(int index, String streamID);
  // getIndex
  external sendBroadcastMessage(
      String roomID,
      String message,
      void Function(int errorCode, int messageID) success,
      void Function(int errorCode) error);
  external sendBarrageMessage(
      String roomID,
      String message,
      void Function(int errorCode, String messageID) success,
      void Function(int errorCode) error);
  external sendCustomCommand(String roomID, String message, List toUserList,
      void Function(int errorCode) success, void Function(int errorCode) error);

  /////////////////////////////设备管理////////////////////////////////////////
  external muteMicrophone(bool mute);
  external isMicrophoneMuted();
  // muteSpeaker
  // isSpeakerMuted
  external getAudioDeviceList(int type, dynamic success, dynamic fail);
  // getDefaultAudioDeviceID
  external useAudioDevice(int type, String deviceID);
  // getAudioDeviceVolume
  // setAudioDeviceVolume
  // setSpeakerVolumeInAPP
  // getSpeakerVolumeInAPP
  // startAudioDeviceVolumeMonitor
  // stopAudioDeviceVolumeMonitor
  // muteAudioDevice
  // setAudioDeviceMode
  // isAudioDeviceMuted
  // enableAudioCaptureDevice
  // getAudioRouteType
  // setAudioRouteToSpeaker
  external enableCamera(bool enable, int channel);
  external useFrontCamera(bool enable, int channel);
  // isCameraFocusSupported
  // setCameraFocusMode
  // setCameraFocusPointInPreview
  // setCameraExposureMode
  // setCameraExposurePointInPreview
  // setCameraExposureCompensation
  // setCameraZoomFactor
  // getCameraMaxZoomFactor
  // enableCameraAdaptiveFPS
  external useVideoDevice(String deviceID, int channel);
  external getVideoDeviceList(dynamic success, dynamic fail);
  external useAudioOutputDevice(dynamic media, String deviceID);
  // getDefaultVideoDeviceID
  external startSoundLevelMonitor(dynamic config);
  external stopSoundLevelMonitor();
  // startAudioSpectrumMonitor
  // stopAudioSpectrumMonitor
  // enableHeadphoneMonitor
  // setHeadphoneMonitorVolume
  // enableMixSystemPlayout
  // setMixSystemPlayoutVolume
  // enableMixEnginePlayout
  // startAudioVADStableStateMonitor
  // stopAudioVADStableStateMonitor
  // getCurrentAudioDevice

  /////////////////////////////混流////////////////////////////////////////
  external startMixerTask(
      String config, void Function(ZegoResponseWeb res) onresult);
  external stopMixerTask(
      String taskId, void Function(ZegoResponseWeb res) onresult);
  external startAutoMixerTask(
      String task, void Function(ZegoResponseWeb res) onresult);
  external stopAutoMixerTask(
      String task, void Function(ZegoResponseWeb res) onresult);
  external takePlayStreamSnapshot(String streamID);
  /////////////////////////////前处理////////////////////////////////////////
  external enableAEC(bool enable);
  external enableAGC(bool enable);
  external enableANS(bool enable);
  external enableEffectsBeauty(bool enable);
  external startEffectsEnv();
  external stopEffectsEnv();
  external setEffectsBeautyParam(dynamic param);
  external setVoiceChangerPreset(int preset);
  external setVoiceChangerParam(int pitch);
  external setReverbPreset(int prese);
  external enableVirtualStereo(bool enable, int angle);

  /////////////////////////////媒体播放器////////////////////////////////////////
  external createMediaPlayer(int index);
  external setStyleByCanvas(dynamic canvas);
  external mediaPlayerEnableAux(bool enable, dynamic mediaPlayer);
  external mediaPlayerSetVolume(int volume, dynamic mediaPlayer);

  ////////////////////////////others////////////////////////////////////////
  external createScreenCaptureSource(int? sourceId);
  external startCaptureScreen(int index, dynamic config);
  external stopCaptureScreen(int index);

  /////////////////////////////版权音乐////////////////////////////////////////
  external createCopyrightedMusic();
  external destroyCopyrightedMusic();
  external copyrightedMusicStartScore(dynamic copMusic, String resourceID);
}

@JS()
class ZegoCopyrightedMusic {
  external initCopyrightedMusic(dynamic config);
  external sendExtendedRequest(String command, dynamic params);
  external requestResource(dynamic config, int type);
  external getSharedResource(dynamic config, int type);
  external getLrcLyric(String songID, int vendorID);
  external getKrcLyricByToken(String krcToken);
  external download(String resourceID);
  external clearCache();
  external getStandardPitch(String resourceID);
  external getCurrentPitch(String resourceID);
  external stopScore(String resourceID);
  external pauseScore(String resourceID);
  external resumeScore(String resourceID);
  external getPreviousScore(String resourceID);
  external getAverageScore(String resourceID);
  external getTotalScore(String resourceID);
  external getFullScore(String resourceID);
  external setScoringLevel(int level);
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
class LogConfig {
  external factory LogConfig({String logLevel});

  external String get logLevel;
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
class ZegoCopMusicInitConfigWeb {
  external factory ZegoCopMusicInitConfigWeb({ZegoUserWeb user});
  external ZegoUserWeb get user;
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

@JS()
@anonymous
class ZegoWebAudioConfig {
  external factory ZegoWebAudioConfig({int bitrate, int channel});
  external int get bitrate;
  external int get channel;
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
      int? rosyIntensity,
      int? smoothIntensity,
      int? sharpenIntensity});
  external int whitenIntensity;
  external int rosyIntensity;
  external int smoothIntensity;
  external int sharpenIntensity;
}

@JS("ZegoProxyInfoWeb")
@anonymous
class ZegoProxyInfoWeb {
  external factory ZegoProxyInfoWeb({String hostName, int? port});
  external String hostName;
  external int port;
}

@JS()
@anonymous
class ZegoLocalProxyConfigWeb {
  external factory ZegoLocalProxyConfigWeb(
      {String accesshubProxy, String? loggerProxy, String? detaillogProxy});
  external String accesshubProxy;
  external String loggerProxy;
  external String detaillogProxy;
}

@JS()
@anonymous
class ZegoResponseWeb {
  external factory ZegoResponseWeb({int errorCode, String? extendedData});
  external int errorCode;
  external String? extendedData;
}
