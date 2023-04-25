// ignore_for_file: deprecated_member_use_from_same_package

import 'zego_express_defines.dart';

extension ZegoAudioSampleRateExtension on ZegoAudioSampleRate {
  int get value {
    switch (this) {
      case ZegoAudioSampleRate.Unknown:
        return 0;
      case ZegoAudioSampleRate.SampleRate8K:
        return 8000;
      case ZegoAudioSampleRate.SampleRate16K:
        return 16000;
      case ZegoAudioSampleRate.SampleRate22K:
        return 22050;
      case ZegoAudioSampleRate.SampleRate24K:
        return 24000;
      case ZegoAudioSampleRate.SampleRate32K:
        return 32000;
      case ZegoAudioSampleRate.SampleRate44K:
        return 44100;
      case ZegoAudioSampleRate.SampleRate48K:
        return 48000;
    }
  }

  static ZegoAudioSampleRate fromValue(int value) {
    switch (value) {
      case 0:
        return ZegoAudioSampleRate.Unknown;
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
}

extension ZegoStreamEventExtension on ZegoStreamEvent {
  int get value {
    switch (this) {
      case ZegoStreamEvent.PublishStart:
        return 100;
      case ZegoStreamEvent.PublishSuccess:
        return 101;
      case ZegoStreamEvent.PublishFail:
        return 102;
      case ZegoStreamEvent.RetryPublishStart:
        return 103;
      case ZegoStreamEvent.RetryPublishSuccess:
        return 104;
      case ZegoStreamEvent.RetryPublishFail:
        return 105;
      case ZegoStreamEvent.PublishEnd:
        return 106;
      case ZegoStreamEvent.PlayStart:
        return 200;
      case ZegoStreamEvent.PlaySuccess:
        return 201;
      case ZegoStreamEvent.PlayFail:
        return 202;
      case ZegoStreamEvent.RetryPlayStart:
        return 203;
      case ZegoStreamEvent.RetryPlaySuccess:
        return 204;
      case ZegoStreamEvent.RetryPlayFail:
        return 205;
      case ZegoStreamEvent.PlayEnd:
        return 206;
    }
  }

  static ZegoStreamEvent fromValue(int value) {
    switch (value) {
      case 100:
        return ZegoStreamEvent.PublishStart;
      case 101:
        return ZegoStreamEvent.PublishSuccess;
      case 102:
        return ZegoStreamEvent.PublishFail;
      case 103:
        return ZegoStreamEvent.RetryPublishStart;
      case 104:
        return ZegoStreamEvent.RetryPublishSuccess;
      case 105:
        return ZegoStreamEvent.RetryPublishFail;
      case 106:
        return ZegoStreamEvent.PublishEnd;
      case 200:
        return ZegoStreamEvent.PlayStart;
      case 201:
        return ZegoStreamEvent.PlaySuccess;
      case 202:
        return ZegoStreamEvent.PlayFail;
      case 203:
        return ZegoStreamEvent.RetryPlayStart;
      case 204:
        return ZegoStreamEvent.RetryPlaySuccess;
      case 205:
        return ZegoStreamEvent.RetryPlayFail;
      case 206:
        return ZegoStreamEvent.PlayEnd;
      default:
        if (value < 200) {
          return ZegoStreamEvent.PublishFail;
        } else {
          return ZegoStreamEvent.PlayFail;
        }
    }
  }
}

extension ZegoAudioDeviceModeExtension on ZegoAudioDeviceMode {
  int get value {
    switch (this) {
      case ZegoAudioDeviceMode.Communication:
        return 1;
      case ZegoAudioDeviceMode.General:
        return 2;
      case ZegoAudioDeviceMode.Auto:
        return 3;
      case ZegoAudioDeviceMode.Communication2:
        return 4;
      case ZegoAudioDeviceMode.Communication3:
        return 5;
      case ZegoAudioDeviceMode.General2:
        return 6;
      case ZegoAudioDeviceMode.General3:
        return 7;
      case ZegoAudioDeviceMode.Communication4:
        return 8;
    }
  }

  static ZegoAudioDeviceMode fromValue(int value) {
    switch (value) {
      case 1:
        return ZegoAudioDeviceMode.Communication;
      case 2:
        return ZegoAudioDeviceMode.General;
      case 3:
        return ZegoAudioDeviceMode.Auto;
      case 4:
        return ZegoAudioDeviceMode.Communication2;
      case 5:
        return ZegoAudioDeviceMode.Communication3;
      case 6:
        return ZegoAudioDeviceMode.General2;
      case 7:
        return ZegoAudioDeviceMode.General3;
      case 8:
        return ZegoAudioDeviceMode.Communication4;
      default:
        return ZegoAudioDeviceMode.Auto;
    }
  }
}

extension ZegoVideoSourceTypeExtension on ZegoVideoSourceType {
  int get value {
    switch (this) {
      case ZegoVideoSourceType.None:
        return 1;
      case ZegoVideoSourceType.Camera:
        return 2;
      case ZegoVideoSourceType.Custom:
        return 3;
      case ZegoVideoSourceType.MainPublishChannel:
        return 4;
      case ZegoVideoSourceType.Player:
        return 5;
      case ZegoVideoSourceType.ScreenCapture:
        return 6;
      case ZegoVideoSourceType.ZegoVideoSourceDefault:
        return 7;
      case ZegoVideoSourceType.ZegoVideoSourceNone:
        return 8;
      case ZegoVideoSourceType.ZegoVideoSourceCamera:
        return 9;
      case ZegoVideoSourceType.ZegoVideoSourceExternalCapture:
        return 10;
      case ZegoVideoSourceType.ZegoVideoSourceMainPublishChannel:
        return 11;
      case ZegoVideoSourceType.ZegoVideoSourcePlayer:
        return 12;
      case ZegoVideoSourceType.ZegoVideoSourceScreenCapture:
        return 13;
    }
  }

  static ZegoVideoSourceType fromValue(int value) {
    switch (value) {
      case 1:
        return ZegoVideoSourceType.None;
      case 2:
        return ZegoVideoSourceType.Camera;
      case 3:
        return ZegoVideoSourceType.Custom;
      case 4:
        return ZegoVideoSourceType.MainPublishChannel;
      case 5:
        return ZegoVideoSourceType.Player;
      case 6:
        return ZegoVideoSourceType.ScreenCapture;
      case 7:
        return ZegoVideoSourceType.ZegoVideoSourceDefault;
      case 8:
        return ZegoVideoSourceType.ZegoVideoSourceNone;
      case 9:
        return ZegoVideoSourceType.ZegoVideoSourceCamera;
      case 10:
        return ZegoVideoSourceType.ZegoVideoSourceExternalCapture;
      case 11:
        return ZegoVideoSourceType.ZegoVideoSourceMainPublishChannel;
      case 12:
        return ZegoVideoSourceType.ZegoVideoSourcePlayer;
      case 13:
        return ZegoVideoSourceType.ZegoVideoSourceScreenCapture;
      default:
        return ZegoVideoSourceType.ZegoVideoSourceDefault;
    }
  }
}

extension ZegoCopyrightedMusicVendorIDExtension
    on ZegoCopyrightedMusicVendorID {
  int get value {
    switch (this) {
      case ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendorDefault:
        return 0;
      case ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor1:
        return 1;
      case ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor2:
        return 2;
      case ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor3:
        return 4;
    }
  }

  static ZegoCopyrightedMusicVendorID fromValue(int value) {
    switch (value) {
      case 0:
        return ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendorDefault;
      case 1:
        return ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor1;
      case 2:
        return ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor2;
      case 4:
        return ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor3;
      default:
        return ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendorDefault;
    }
  }
}
