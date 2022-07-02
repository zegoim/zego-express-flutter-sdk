import 'zego_express_defines.dart';

extension ZegoAudioSampleRate_ on  ZegoAudioSampleRate {
  int get value {
    switch(this) {
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
    switch(value) {
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

extension ZegoStreamEvent_ on ZegoStreamEvent {
  int get value {
    switch(this) {
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
    switch(value) {
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