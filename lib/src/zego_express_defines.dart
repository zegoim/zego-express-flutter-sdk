import 'package:flutter/material.dart';
import 'dart:async';

import 'package:zego_express_engine/src/zego_express_api.dart';

enum ZegoScenario {
  GENERAL,
  COMMUNICATION,
  LIVE
}

enum ZegoLanguage {
  ENGLISH,
  CHINESE
}

class ZegoUser {
  String userID;
  String userName;

  ZegoUser(this.userID, this.userName)
      : assert(userID != null),
        assert(userName != null);

  ZegoUser.id(String userID)
      : assert(userID != null) {
    this.userID = userID;
    this.userName = this.userID;
  }

  Map<String, dynamic> toMap() {
    return {
      'userID': userID,
      'userName': userName
    };
  }
}

class ZegoStream {
  String streamID;
  String extraInfo;
  ZegoUser user;

  ZegoStream(this.streamID, this.extraInfo, this.user);

  Map<String, dynamic> toMap() {
    return {
      'streamID': this.streamID,
      'extraInfo': this.extraInfo,
      'user': this.user.toMap()
    };
  }
}

class ZegoRoomConfig {
  int maxMemberCount;
  bool isUserStatusNotify;

  ZegoRoomConfig(this.maxMemberCount, this.isUserStatusNotify)
      : assert(maxMemberCount != null),
        assert(isUserStatusNotify != null);

  ZegoRoomConfig.defaultConfig() {
    // 默认为0，表示房间人数无限制
    this.maxMemberCount = 0;
    this.isUserStatusNotify = false;
  }

  Map<String, dynamic> toMap() {
    return {
      'maxMemberCount': this.maxMemberCount,
      'isUserStatusNotify': this.isUserStatusNotify
    };
  }

}

enum ZegoRoomState {
  DISCONNECT,
  CONNECTING,
  CONNECTED
}

enum ZegoUpdateType {
  ADD,
  DELETE
}

enum ZegoVideoConfigPreset {
  PRESET_180P,
  PRESET_270P,
  PRESET_360P,
  PRESET_540P,
  PRESET_720P,
  PRESET_1080P
}

enum ZegoVideoCodecID {
  DEFAULT,
  MULTI_LAYER,
  VP8
}

class ZegoVideoConfig {
  int videoCaptureResolutionWidth;
  int videoCaptureResolutionHeight;
  int videoEncodeResolutionWidth;
  int videoEncodeResolutionHeight;
  int bitrate;
  int fps;
  ZegoVideoCodecID videoCodecID;

  ZegoVideoConfig(this.videoCaptureResolutionWidth, this.videoCaptureResolutionHeight, this.videoEncodeResolutionWidth, this.videoEncodeResolutionHeight, this.bitrate, this.fps)
      : assert(videoCaptureResolutionWidth != null),
        assert(videoCaptureResolutionHeight != null),
        assert(videoEncodeResolutionWidth != null),
        assert(videoEncodeResolutionHeight != null),
        assert(bitrate != null),
        assert(fps != null);

  ZegoVideoConfig.defaultConfig() {
    this.videoCaptureResolutionWidth = 360;
    this.videoEncodeResolutionHeight = 640;
    this.videoEncodeResolutionWidth = 360;
    this.videoEncodeResolutionHeight = 640;
    this.bitrate = 600;
    this.fps = 15;
  }

  /// 预配置分辨率，参考 [ZegoResolution]
  ZegoVideoConfig.preset(ZegoVideoConfigPreset preset)
      : assert(preset != null) {
    switch(preset) {
      case ZegoVideoConfigPreset.PRESET_180P:
        this.videoCaptureResolutionWidth = 180;
        this.videoCaptureResolutionHeight = 320;
        this.videoEncodeResolutionWidth = 180;
        this.videoEncodeResolutionHeight = 320;
        this.bitrate = 300;
        this.fps = 15;
        break;
      case ZegoVideoConfigPreset.PRESET_270P:
        this.videoCaptureResolutionWidth = 270;
        this.videoCaptureResolutionHeight = 480;
        this.videoEncodeResolutionWidth = 270;
        this.videoEncodeResolutionHeight = 480;
        this.bitrate = 400;
        this.fps = 15;
        break;
      case ZegoVideoConfigPreset.PRESET_540P:
        this.videoCaptureResolutionWidth = 540;
        this.videoCaptureResolutionHeight = 960;
        this.videoEncodeResolutionWidth = 540;
        this.videoEncodeResolutionHeight = 960;
        this.bitrate = 1200;
        this.fps = 15;
        break;
      case ZegoVideoConfigPreset.PRESET_720P:
        this.videoCaptureResolutionWidth = 720;
        this.videoCaptureResolutionHeight = 1280;
        this.videoEncodeResolutionWidth = 720;
        this.videoEncodeResolutionHeight = 1280;
        this.bitrate = 1500;
        this.fps = 15;
        break;
      case ZegoVideoConfigPreset.PRESET_1080P:
        this.videoCaptureResolutionWidth = 1080;
        this.videoCaptureResolutionHeight = 1920;
        this.videoEncodeResolutionWidth = 1080;
        this.videoEncodeResolutionHeight = 1920;
        this.bitrate = 3000;
        this.fps = 15;
        break;
      case ZegoVideoConfigPreset.PRESET_360P:
      default:
        this.videoCaptureResolutionWidth = 360;
        this.videoCaptureResolutionHeight = 640;
        this.videoEncodeResolutionWidth = 360;
        this.videoEncodeResolutionHeight = 640;
        this.bitrate = 600;
        this.fps = 15;
        break;
    }
  }

  Map<String, dynamic> toMap() {
    return {
      'captureWidth': this.videoCaptureResolutionWidth,
      'captureHeight': this.videoCaptureResolutionHeight,
      'encodeWidth': this.videoEncodeResolutionWidth,
      'encodeHeight': this.videoEncodeResolutionHeight,
      'bitrate': this.bitrate,
      'fps': this.fps
    };
  }

}

enum ZegoVideoMirrorMode {
  ONLY_PREVIEW_MIRROR,
  BOTH_MIRROR,
  NO_MIRROR,
  ONLY_PUBLISH_MIRROR
}

class ZegoAudioConfigPreset {
  static const int LOW_LATENCY_BASIC_QUALITY = 0;
  static const int LOW_LATENCY_STANDARD_QUALITY = 1;
  static const int LOW_LATENCY_STANDARD_QUALITY_STEREO = 2;
  static const int LOW_LATENCY_HIGH_QUALITY = 3;
  static const int LOW_LATENCY_HIGH_QUALITY_STEREO = 4;
  static const int NORMAL_LATENCY_STANDARD_QUALITY = 5;
  static const int NORMAL_LATENCY_STANDARD_QUALITY_STEREO = 6;
  static const int NORMAL_LATENCY_HIGH_QUALITY = 7;
  static const int NORMAL_LATENCY_HIGH_QUALITY_STEREO = 8;
}

//enum ZegoAudioConfigPreset {
//
//}

class ZegoAudioConfig {
  int bitrate;
  int channels;
  int codecID;

  ZegoAudioConfig(this.bitrate, this.channels, this.codecID);

  ZegoAudioConfig.defaultConfig() {
    this.bitrate = 48 * 1000;
    this.channels = 1;
    this.codecID = 0;
  }

  ZegoAudioConfig.preset(int audioConfigPresetType) {
    switch(audioConfigPresetType) {
      case ZegoAudioConfigPreset.LOW_LATENCY_BASIC_QUALITY:
        this.bitrate = 16 * 1000;
        this.channels = 1;
        this.codecID = 5;
        break;
      case ZegoAudioConfigPreset.LOW_LATENCY_STANDARD_QUALITY:
        this.bitrate = 48 * 1000;
        this.channels = 1;
        this.codecID = 5;
        break;
      case ZegoAudioConfigPreset.LOW_LATENCY_STANDARD_QUALITY_STEREO:
        this.bitrate = 56 * 1000;
        this.channels = 2;
        this.codecID = 5;
        break;
      case ZegoAudioConfigPreset.LOW_LATENCY_HIGH_QUALITY:
        this.bitrate = 128 * 1000;
        this.channels = 1;
        this.codecID = 5;
        break;
      case ZegoAudioConfigPreset.LOW_LATENCY_HIGH_QUALITY_STEREO:
        this.bitrate = 192 * 1000;
        this.channels = 2;
        this.codecID = 5;
        break;
      case ZegoAudioConfigPreset.NORMAL_LATENCY_STANDARD_QUALITY_STEREO:
        this.bitrate = 56 * 1000;
        this.channels = 2;
        this.codecID = 0;
        break;
      case ZegoAudioConfigPreset.NORMAL_LATENCY_HIGH_QUALITY:
        this.bitrate = 128 * 1000;
        this.channels = 1;
        this.codecID = 1;
        break;
      case ZegoAudioConfigPreset.NORMAL_LATENCY_HIGH_QUALITY_STEREO:
        this.bitrate = 192 * 1000;
        this.channels = 2;
        this.codecID = 1;
        break;
      case ZegoAudioConfigPreset.NORMAL_LATENCY_STANDARD_QUALITY:
      default:
      this.bitrate = 48 * 1000;
      this.channels = 1;
      this.codecID = 0;
        break;
    }
  }

  Map<String, dynamic> toMap() {
    return {
      'bitrate': this.bitrate,
      'channels': this.channels,
      'codecID': this.codecID
    };
  }
}

class ZegoWatermark {
  String imageURL;
  Rect layout;

  ZegoWatermark(this.imageURL, this.layout)
      : assert (imageURL != null), assert(layout != null);

  Map<String, dynamic> toMap() {
    return {
      'imageURL': imageURL,
      'left': layout.left,
      'top': layout.top,
      'bottom': layout.bottom,
      'right': layout.right
    };
  }
}

enum ZegoCapturePipelineScaleMode {
  PRE_SCALE,
  POST_SCALE
}

class ZegoAECMode {
  static const int AGGRESSIVE = 0;
  static const int MEDIUM = 1;
  static const int SOFT = 2;
}

class ZegoBeautifyFeature {
  static const int NONE = 0;
  static const int POLISH = 1;
  static const int SHARPEN = 1<<1;
  static const int SKIN_WHITEN = 1<<2;
  static const int WHITEN = 1<<3;
}

class ZegoBeautifyOption {
  double polishStep;
  double whitenFactor;
  double sharpenFactor;

  ZegoBeautifyOption(this.polishStep, this.whitenFactor, this.sharpenFactor)
      : assert(polishStep != null),
        assert(whitenFactor != null),
        assert(sharpenFactor != null);

  ZegoBeautifyOption.defaultOption() {
    this.polishStep = 0.2;
    this.whitenFactor = 0.5;
    this.sharpenFactor = 0.1;
  }

  Map<String, dynamic> toMap() {
    return {
      'polishStep': this.polishStep,
      'whitenFactor': this.whitenFactor,
      'sharpenFactor': this.sharpenFactor
    };
  }
}

class ZegoPublishStreamQuality {

  final int level;
  final double videoCaptureFPS;
  final double videoEncodeFps;
  final double videoSendFPS;
  final double videoKBPS;
  final double audioCaptureFPS;
  final double audioSendFPS;
  final double audioKBPS;
  final int rtt;
  final double packetLostRate;
  final bool isHardwareEncode;

  const ZegoPublishStreamQuality(this.level, this.videoCaptureFPS, this.videoEncodeFps, this.videoSendFPS, this.videoKBPS, this.audioCaptureFPS, this.audioSendFPS, this.audioKBPS, this.rtt, this.packetLostRate, this.isHardwareEncode);
}

class ZegoPublisherFirstFrameEvent {
  static const int AUDIO_FIRST_FRAME_CAPTURED = 0;
  static const int VIDEO_FIRST_FRAME_CAPTURED = 1;
}

class ZegoStreamRelayCDNInfo {
  final String URL;
  final int state;
  final int reason;
  final int stateTime;

  const ZegoStreamRelayCDNInfo(this.URL, this.state, this.reason, this.stateTime);
}

enum ZegoPublisherState {
  NO_PUBLISH,
  PUBLISH_REQUESTING,
  PUBLISHING
}

class ZegoPlayStreamQuality {
  final int level;
  final double videoRecvFPS;
  final double videoDecodeFPS;
  final double videoRenderFPS;
  final double videoKBPS;
  final double audioRecvFPS;
  final double audioDecodeFPS;
  final double audioRenderFPS;
  final double audioKBPS;
  final int rtt;
  final double packetLostRate;
  final int delay;
  final bool isHardwareDecode;

  const ZegoPlayStreamQuality(this.level, this.videoRecvFPS, this.videoDecodeFPS, this.videoRenderFPS, this.videoKBPS, this.audioRecvFPS, this.audioDecodeFPS, this.audioRenderFPS, this.audioKBPS, this.rtt, this.packetLostRate, this.delay, this.isHardwareDecode);
}

enum ZegoPlayerState {
  NO_PLAY,
  PLAY_REQUESTING,
  PLAYING
}

enum ZegoPlayerMediaEvent {
  AUDIO_BREAK_OCCUR,
  AUDIO_BREAK_RESUME,
  VIDEO_BREAK_OCCUR,
  VIDEO_BREAK_RESUME
}

enum ZegoRemoteDeviceState {
  OPEN,
  GENERIC_ERROR,
  INVALID_ID,
  NO_AUTHORIZATION,
  ZERO_FPS,
  IN_USE_BY_OTHER,
  UNPLUGGED,
  REBOOT_REQUIRED,
  SYSTEM_MEDIA_SERVICES_LOST,
  DISABLE,
  MUTE,
  INTERRUPTION,
  IN_BACKGROUND,
  MULTI_FOREGROUND_APP,
  SYSTEM_PRESSURE
}

class ZegoMessageInfo {
  final String message;
  final int sendTime;
  final ZegoUser fromUser;

  const ZegoMessageInfo(this.message, this.sendTime, this.fromUser);
}

class ZegoStreamExtraInfoResult {
  final int errorCode;
  const ZegoStreamExtraInfoResult(this.errorCode);
}

class ZegoUpdateCDNURLResult {
  final int errorCode;
  const ZegoUpdateCDNURLResult(this.errorCode);
}

class ZegoBroadcastMessageResult {
  final int errorCode;
  const ZegoBroadcastMessageResult(this.errorCode);
}

class ZegoCustomCommandResult {
  final int errorCode;
  const ZegoCustomCommandResult(this.errorCode);
}


typedef ZegoViewCreatedCallback = void Function(int viewID);

/*class ZegoRenderViewController {
  int _viewID;

  ZegoRenderViewController();

  int get viewID => _viewID;

  Future<void> initialize() async {

    _viewID =
  }

  Future<void> release() async {

  }
}*/