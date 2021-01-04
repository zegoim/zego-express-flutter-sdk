# Change Log

## 2.0.0

### **New Features**

1. Added low latency live broadcast feature.

    Low latency live broadcast focuses on providing stable and reliable live broadcast services. Compared with standard live video products, audio and video delays are lower, synchronization is stronger, and weak network resistance is better. It can provide users with a millisecond-level live broadcast experience; usage scenarios as education class, live show broadcast, e-commerce live broadcast, watch together, online auction. For more details, please refer to [Low Latency Live](https://doc-en.zego.im/en/6823.html).

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

2. Added support for H.265 (HEVC) encoding.

    Added support for H.265 encoding, which can reduce the bit rate at the same resolution and frame rate.

    For related API, please refer to [setVideoConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setVideoConfig.html)

### **Bug Fixes**

1. Fixed a parameter type convert issue for [getVideoConfig] on iOS.

### **Deleted**

1. Deleted [muteAudioOutput], please use [muteSpeaker] instead.

    The naming style and semantics of the legacy function are not clear. [muteSpeaker] uses the definition of Speaker to correspond to the Microphone.

    For related API, please refer to [muteSpeaker](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/muteSpeaker.html)

2. Deleted [ZegoMediaPlayer.getVolume], please use [getPublishVolume], [getPlayVolume] instead.

    The engine provides the function of separately acquiring the player's publish volume and local playback volume, so the original unified acquisition interface is deprecated.

    For related APIs, please refer to [ZegoMediaPlayer.getPublishVolume](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/getPublishVolume.html), [ZegoMediaPlayer.getPlayVolume](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/getPlayVolume.html)

## 1.20.0

### **New Features**

1. Added the ability to take snapshots of the publishing stream or the playing stream.

    It supports taking snapshots of the screen during publishing or playing stream, which can be used for scenes such as pornographic identification.

    For related API, please refer to [takePublishStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/takePublishStreamSnapshot.html), [takePlayStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/takePlayStreamSnapshot.html)

2. Added network mode changed notification callback.

    This callback will be called when the device's network mode changes, such as switching from WiFi to 5G, or when the network is disconnected.

    For related API, please refer to [onNetworkModeChanged](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onNetworkModeChanged.html)

3. Added camera zoom function.

    Set the zoom factor of the camera through the SDK to achieve the effect of zooming in on distant objects during shooting. For detailed function implementation, please refer to [Advanced Features - Zoom].

    For related API, please refer to [getCameraMaxZoomFactor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/getCameraMaxZoomFactor.html), [setCameraZoomFactor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setCameraZoomFactor.html)

4. Added audio route changed notification callback.

    This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.

    For related API, please refer to [onAudioRouteChange](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onAudioRouteChange.html)

### **Bug Fixes**

1. Fixed the occasional crash of the local recording function under abnormal conditions.

    For related API, please refer to [startRecordingCapturedData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/muteMicrophone.html), [stopRecordingCapturedData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRecord/stopRecordingCapturedData.html)

## 1.19.1

### **Enhancements**

1. Added error code 1002002, which means the RoomID is wrong, please check whether you are currently logged in to this room.

    After logging in to the room, if you log out or switch rooms, and the incoming RoomID is empty or the RoomID does not exist, the 1002002 error code will be thrown.

    For related API, please refer to [loginRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/loginRoom.html), [logoutRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/logoutRoom.html), [switchRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/switchRoom.html)

### **Bug Fixes**

1. Fixed the occasional popping sound when the sound effect player set the specified playback progress.

    For related API, please refer to [ZegoAudioEffectPlayer.seekTo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoAudioEffectPlayer/seekTo.html)

2. Fixed a crash when setting the media enhancement supplementary information type on Android.

    For related API, please refer to [setSEIConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setSEIConfig.html)

3. Fixed the issue that switching rendering view does not take effect when playing the stream on Android.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

4. Fixed the crash of FEC module under weak network conditions.

5. Fixed the occasional issue of abnormality when switching from media volume to call volume on iOS.

## 1.19.0

### **New Features**

1. Added system performance monitoring function, supports monitoring of CPU and memory usage.

    Developers can start monitoring after [createEngine], and support setting the monitoring callback interval (the default is 2s), which can generally be used to compare the memory growth before and after publish/play stream.

    For related API, please refer to [startPerformanceMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineUtilities/startPerformanceMonitor.html), [stopPerformanceMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineUtilities/stopPerformanceMonitor.html)

2. Added streaming media encryption function.

    Support the use of AES-128/192/256 to encrypt streaming media data.

    For related API, please refer to [setPublishStreamEncryptionKey](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setPublishStreamEncryptionKey.html), [setPlayStreamDecryptionKey](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/setPlayStreamDecryptionKey.html)

3. Added the difference between the video timestamp and the audio timestamp, which is located in [ZegoPlayStreamQuality](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoPlayStreamQuality-class.html) to reflect the synchronization of audio and video in the playing stream.

    This value is less than 0 means the number of milliseconds that the video leads the audio, greater than 0 means the number of milliseconds that the video lags the audio, and 0 means no difference. When the absolute value is less than 200, it can basically be regarded as synchronized audio and video, when the absolute value is greater than 200 for 10 consecutive seconds, it can be regarded as abnormal.

    For related API, please refer to [onPlayerQualityUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerQualityUpdate.html)

4. Added setting to select the video layer function for streaming.

    When the publisher has set the codecID of [setVideoConfig] to SVC, the player can call [setPlayStreamVideoLayer] API to select the standard layer or the base layer (the resolution of the base layer is one-half of the standard layer) to saving bandwidth.

    For related API, please refer to [setPlayStreamVideoLayer](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/setPlayStreamVideoLayer.html)

### **Enhancements**

1. Added error code 1002035 for room login failure, that is the maximum number of concurrent rooms in the test environment exceeds the upper limit.

    For related API, please refer to [loginRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/loginRoom.html)

2. Added error code 1009015 for room broadcast message sending failure, that is broadcast message QPS exceeds the limit, the maximum QPS is 2.

    For related API, please refer to [sendBroadcastMessage](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineIM/sendBroadcastMessage.html)

3. Added support for AAC file format for local media recording.

    For related API, please refer to [startRecordingCapturedData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRecord/startRecordingCapturedData.html)

4. The media player newly supports m3u8 format, if you need to use it, please contact ZEGO technical support.

    For related APIs, please refer to [ZegoMediaPlayer.loadResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/loadResource.html)

### **Deleted**

1. Deprecated the `videoLayer` property in [ZegoPlayerConfig], please use [setPlayStreamVideoLayer] instead.

    For related API, please refer to [setPlayStreamVideoLayer](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/setPlayStreamVideoLayer.html)

## 1.18.0

### **New Features**

1. Added advanced reverb parameters and preset values for reverb/voice change.

    Advanced reverberation parameters can be used to adjust finer reverberation effects as needed. In the original preset reverberation, effects such as studio, KTV, rock and concert have been added, and magnetic male and female voices have been added to the preset voice change. Fresh female voice effect, increase the interest of real-time voice, can adapt to more scenes.

    For related API, please refer to [setReverbAdvancedParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setReverbAdvancedParam.html), [setReverbPreset](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setReverbPreset.html), [setVoiceChangerPreset](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setVoiceChangerPreset.html)

2. Added SEI setting type function.

    By setting the SEI type, the developer can correctly parse the SEI when decoding with other decoders.

    For related API, please refer to [setSEIConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setSEIConfig.html)

3. Added a callback with additional information when the stream is updated, such as the reason information of the deleted stream.

    For related API, please refer to [onRoomStreamUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRoomStreamUpdate.html)

### **Bug Fixes**

1. Fix the problem that when calling the [startPlayingStream] interface to set the video layer, calling the [mutePlayStreamVideo] interface again will cause the wrong layer selection.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

2. Optimize [mutePlayStreamAudio] and [mutePlayStreamVideo] call timing, the settings can take effect before and after streaming.

    For related API, please refer to [mutePlayStreamAudio](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/mutePlayStreamAudio.html), [mutePlayStreamVideo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/mutePlayStreamVideo.html)

3. Optimize the SEI sending logic to support normal sending of SEI information when the microphone is turned on and no audio data is sent.

    For related API, please refer to [sendSEI](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/sendSEI.html)

4. Fix the problem that the media player's playback volume or publish volume obtained for the first time is 0 on iOS.

    For related API, please refer to [ZegoMediaPlayer.getPlayVolume](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/getPlayVolume.html), [ZegoMediaPlayer.getPublishVolume](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/getPublishVolume.html)

5. Fix the problem that setting `viewMode` does not work when using Texture to render on iOS.

    For related API, please refer to [ZegoCanvas.viewMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCanvas/viewMode.html)

### **Deleted**

1. Deprecate the original [setReverbParam] interface, and use [setReverbAdvancedParam] instead if necessary.

    For related API, please refer to [setReverbAdvancedParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setReverbAdvancedParam.html)

2. Deprecate the original [onRoomStreamUpdate] callback, please use the callback of the same name with additional [extendedData] parameter instead.

    For related API, please refer to [onRoomStreamUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRoomStreamUpdate.html)

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
