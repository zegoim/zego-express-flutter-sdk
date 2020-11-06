# Change Log

## 1.17.5

### **Enhancements**

1. When the login room is inconsistent with the room ID sent by the real-time message, an accurate error code (1009005) is thrown.

    For related API, please refer to [loginRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/loginRoom.html), [sendBroadcastMessage](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineIM/sendBroadcastMessage.html), [sendBarrageMessage](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineIM/sendBarrageMessage.html), [sendCustomCommand](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineIM/sendCustomCommand.html)

### **Bug Fixes**

1. Fixed an issue where the play-stream end did not receive the SEI when the aux publisher sent it.

    For related API, please refer to [sendSEI](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/sendSEI.html)

2. Fix the issue that sometimes audio and video are out of sync in publish iOS screen capture stream.

## 1.17.0

### **New Features**

1. Added support for more voice changer effects.

    Added 4 voice changer effects - Foreigner, Optimus Prime, Robot, and Ethereal - to easily create unique sound effects and make users' voices more interesting. Create a quirky atmosphere between friends' voices in voice scenes to enhance entertainment.

    For related API, please refer to [setVoiceChangerPreset](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setVoiceChangerPreset.html)

2. Added support for setting reverb echo parameters

    Allows to set up to 7 echoes (delay), and supports to individually set the delay and decay of each echo, and overall input and output gain values.

    For related API, please refer to [setReverbEchoParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setReverbEchoParam.html)

3. Added support for transient noise suppression

    Can be used to suppress transient noises such as keyboard and desk knocks.

    For related API, please refer to [enableTransientANS](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableTransientANS.html)

4. Media Player added support for switching audio tracks.

5. Media Player added support for setting voice changer effects.

### **Enhancements**

1. Added parameters like [videoBreakRate], [audioBreakRate] to the play stream quality parameter to count the break rate.

    For related API, please refer to [onPlayerQualityUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerQualityUpdate.html)

### **Bug Fixes**

1. Fix the issue that calling media player's [enableRepeat] does not take effect before receives the [loadResource] callback.

## 1.16.2

### **Enhancements**

1. Optimize custom video capture API style. and add necessary notes.

## 1.16.1

### **Enhancements**

1. The fast seek configuration is enabled inside the media player to optimize the slow loading of MP3 files by the media player.

2. Upgrade the third-party library to ensure the security and quality level of the SDK, and FFmpeg is upgraded to version 4.2.2.

### **Bug Fixes**

1. Fixed a problem where Jni caused some machines to crash with low probability when ClassLoad was released.

2. Fix the problem of a small number of users experiencing a sudden drop in video encoding rate.

## 1.16.0

### **New Features**

1. Add audio effect player function

    Sound effects refer to short sound effects played to enhance the sense of reality or to enhance the atmosphere of the scene, such as: playing applause, gift sound effects, prompt sounds, etc. during the live broadcast; in the game, playing bullets, collision sounds.

    The sound effect player supports functions such as sound effect playback (multiple sound effects can be overlapped), playback control (such as pause playback, volume adjustment, set playback progress), pre-loaded sound effects and other functions.

    For related API, please refer to [createAudioEffectPlayer](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineAudioEffectPlayer/createAudioEffectPlayer.html), [destroyAudioEffectPlayer](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineAudioEffectPlayer/destroyAudioEffectPlayer.html)

### **Bug Fixes**

1. Fix the problem that [onRoomStreamExtraInfoUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRoomStreamExtraInfoUpdate.html) may call back with empty list unexpectedly

2. Fix the problem that an error occurs when calling SDK functions for the first time

3. Fix the problem of no callback after two consecutive calls to [createEngine](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/createEngine.html)

## 1.15.2

### **Bug Fixes**

1. Fixed issue where media player setting local playback volume or publishing stream volume might not work

2. Fixed a problem with the [onRoomUserUpdate] callback that had the probability of triggering a full member callback by mistake

## 1.15.0

### **New Features**

1. Add media player volume control function

    Allows to set and get the local playback volume and publish volume of the media player separately.

    For related API, please refer to `setPlayVolume`, `setPublishVolume`, `getPlayVolume`, `getPublishVolume` of the `ZegoMediaPlayer`.

2. Add the function of setting audio capture dual channel mode

3. The API for start sound level / audio spectrum monitoring supports setting the callback interval

4. Added the function to configure advanced properties when switching rooms

    Switch room allows developer to configure the properties of the next room, such as login authentication

5. Add `setEngineConfig` API to set log path and size, and advanced special configuration

### **Deleted**

1. Deprecated the `getVolume` interface in the mediaplayer, please use `getPlayVolume` and `getPublishVolume` instead

## 1.14.0

1. Update native SDK to version 1.14.0

2. Add `loginMultiRoom`, `switchRoom`, `setRoomExtraInfo` APIs and `onRoomExtraInfoUpdate` callback

3. Optimize the callback log on the flutter native side

## 1.1.0

1. Update native SDK to version 1.12.3

2. Add `setANSMode`, `setAudioEqualizerGain`, `setVoiceChangerParam`, `setReverbParam`, `enableVirtualStereo` APIs

3. Add `Record` module with `startRecordingCapturedData`, `stopRecordingCapturedData`, `onCapturedDataRecordStateUpdate`, `onCapturedDataRecordProgressUpdate`

4. Add `getVideoConfig` implementation for Android

5. Fix issue of `stopMixerTask` for Android

## 0.15.1

- Fix the problem that the `codecID` parameter of `setVideoConfig` API does not take effect

## 0.15.0

- Update native SDK to version 1.11.4

## 0.14.2

- Fix the conversion issue of generic Map for some callbacks

## 0.14.1

1. Fix `enablePublishDirectToCDN` API

2. Fix `updateTextureRendererSize` API on Android

## 0.14.0

1. Update native SDK to version 1.10.0

2. Fix `updateTextureRendererSize` API on Android

3. Fix `onRoomOnlineUserCountUpdate` callback

## 0.13.1

- Fix mixer module issue

## 0.13.0

- Update native SDK to version 1.9.2

## 0.12.2

- Update native SDK to version 1.8.2

## 0.12.1

- Fix `onIMRecvCustomCommand` callback on iOS

## 0.12.0

1. Update native iOS SDK to version 1.8.0

2. Update native Android SDK to version 1.8.1

3. Fix some bugs in the IM module

## 0.11.1

- Update native SDK to version 1.7.9

## 0.11.0

1. Update native SDK to version 1.7.8

2. Optimize native method handler and callback handler

## 0.10.0

1. Add media player (audio only) module

2. Update native SDK to version 1.7.0

3. Add error code definition class

4. Fix iOS callback bug

## 0.9.3

- Fix type conversion issue in dart callback

## 0.9.2

- Update native SDK version to 1.6.0

## 0.9.1

1. Implement Android mixer API

2. Fix iOS mixer bug

## 0.9.0

- Initial release, with native SDK dependency version 1.5.5
