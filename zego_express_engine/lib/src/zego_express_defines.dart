import 'package:flutter/material.dart';

class ZegoUser {
  String userID;
  String userName;

  ZegoUser(this.userID, this.userName);

  ZegoUser.id(String userID) {
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

  ZegoRoomConfig(this.maxMemberCount, this.isUserStatusNotify);

  ZegoRoomConfig.defaultConfig() {
    // 默认为0，表示房间人数无限制
    this.maxMemberCount = 0;
    this.isUserStatusNotify = false;
  }

}

class ZegoResolution {
  static const RESOLUTION_180x320 = 0;
  static const RESOLUTION_270x480 = 1;
  static const RESOLUTION_360x640 = 2;
  static const RESOLUTION_540x960 = 3;
  static const RESOLUTION_720x1280 = 4;
  static const RESOLUTION_1080x1920 = 5;
}

class ZegoVideoConfig {
  int videoCaptureResolutionWidth;
  int videoCaptureResolutionHeight;
  int videoEncodeResolutionWidth;
  int videoEncodeResolutionHeight;
  int bitrate;
  int fps;

  ZegoVideoConfig(this.videoCaptureResolutionWidth, this.videoCaptureResolutionHeight, this.videoEncodeResolutionWidth, this.videoEncodeResolutionHeight, this.bitrate, this.fps);

  ZegoVideoConfig.defaultConfig() {
    this.videoCaptureResolutionWidth = 360;
    this.videoEncodeResolutionHeight = 640;
    this.videoEncodeResolutionWidth = 360;
    this.videoEncodeResolutionHeight = 640;
    this.bitrate = 600 * 1000;
    this.fps = 15;
  }

  /// 预配置分辨率，参考 [ZegoResolution]
  ZegoVideoConfig.preset(int resolutionPresetType) {
    switch(resolutionPresetType) {
      case ZegoResolution.RESOLUTION_180x320:
        this.videoCaptureResolutionWidth = 180;
        this.videoCaptureResolutionHeight = 320;
        this.videoEncodeResolutionWidth = 180;
        this.videoEncodeResolutionHeight = 320;
        this.bitrate = 300 * 1000;
        this.fps = 15;
        break;
      case ZegoResolution.RESOLUTION_270x480:
        this.videoCaptureResolutionWidth = 270;
        this.videoCaptureResolutionHeight = 480;
        this.videoEncodeResolutionWidth = 270;
        this.videoEncodeResolutionHeight = 480;
        this.bitrate = 400 * 1000;
        this.fps = 15;
        break;
      case ZegoResolution.RESOLUTION_540x960:
        this.videoCaptureResolutionWidth = 540;
        this.videoCaptureResolutionHeight = 960;
        this.videoEncodeResolutionWidth = 540;
        this.videoEncodeResolutionHeight = 960;
        this.bitrate = 1200 * 1000;
        this.fps = 15;
        break;
      case ZegoResolution.RESOLUTION_720x1280:
        this.videoCaptureResolutionWidth = 720;
        this.videoCaptureResolutionHeight = 1280;
        this.videoEncodeResolutionWidth = 720;
        this.videoEncodeResolutionHeight = 1280;
        this.bitrate = 1500 * 1000;
        this.fps = 15;
        break;
      case ZegoResolution.RESOLUTION_1080x1920:
        this.videoCaptureResolutionWidth = 1080;
        this.videoCaptureResolutionHeight = 1920;
        this.videoEncodeResolutionWidth = 1080;
        this.videoEncodeResolutionHeight = 1920;
        this.bitrate = 3000 * 1000;
        this.fps = 15;
        break;
      case ZegoResolution.RESOLUTION_360x640:
      default:
        this.videoCaptureResolutionWidth = 360;
        this.videoCaptureResolutionHeight = 640;
        this.videoEncodeResolutionWidth = 360;
        this.videoEncodeResolutionHeight = 640;
        this.bitrate = 600 * 1000;
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

class ZegoVideoMirrorMode {
  static const int ONLY_PREVIEW_MIRROR = 0;
  static const int BOTH_MIRROR = 1;
  static const int NO_MIRROR = 2;
  static const int ONLY_PUBLISH_MIRROR = 3;
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

  ZegoWatermark(this.imageURL, this.layout);

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

class ZegoCapturePipelineScaleMode {
  static const int PRE_SCALE = 0;
  static const int POST_SCALE = 1;
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

  ZegoBeautifyOption(this.polishStep, this.whitenFactor, this.sharpenFactor);

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
