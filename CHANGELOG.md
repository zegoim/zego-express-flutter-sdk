# Change Log

## 3.4.2

1. Support for Geofencing Function.

    Note:

    - To use this feature, please contact ZEGO technical support.

    - Please configure geofencing information before creating the engine.

    Restrict access to audio, video, and signaling data to a specific area to meet regional data privacy and security regulations, which restrict users' access to audio and video services in a specific area.

    For related API, please refer to [setGeoFence](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setGeoFence.html)

2. The media player supports functions such as downloading, retrieving data decryption, and playing at the same time.

    For copyright music protection in online players, the media player supports downloading while recalling unencrypted binary data, which is decrypted by the developer and then returned to the media player for playback. During the process, no files or cache files are generated.

    For related API, please refer to [enableBlockData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/enableBlockData.html)

3. Support dynamic switching of flow control strategies

    Supports dynamic switch flow control function, as well as setting flow control attributes.

    For related API, please refer to [enableTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/enableTrafficControl.html), [setMinVideoBitrateForTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setMinVideoBitrateForTrafficControl.html), [setMinVideoFpsForTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setMinVideoFpsForTrafficControl.html), [setMinVideoResolutionForTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setMinVideoResolutionForTrafficControl.html)

4. Expand the acquired song resource information.

    Support acquiring the start and end time information of the "snatch singing segment" of the song corresponding to the songID through the requestResource interface. Other users can obtain the shared information through the getSharedResource interface.

    Note: This feature is only supported for use by some copyright holders. Please contact ZEGO technical support.

    For related API, please refer to [requestResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/requestResource.html), [getSharedResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/getSharedResource.html)

5. Copyrighted music adds a new billing model.

    A new billing model has been added, which charges monthly based on the host. Please contact ZEGO technical support.

6. The copyright content center has added song resources from music copyright holders.

    It is supported to pass different values of copyright holders in the parameter "vendorID" of the relevant interface to perform operations such as song request and obtaining lyrics.

    Note: Detailed information about the copyright holder. Please contact ZEGO technical support.

    For related API, please refer to [requestResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/requestResource.html), [getSharedResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/getSharedResource.html) 

7. Expand the range of supported song rating capabilities.

    Support the ability to rate and display pitch lines for multiple copyrighted songs, with detailed information.  Please contact ZEGO technical support.

8. Support the use of copyrighted music in multi-room mode.

    By adding the roomID parameter to the "request song" and "get shared resources" interfaces, the use of copyrighted music in multi-room mode is enabled.

    For related API, please refer to [requestResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/requestResource.html), [getSharedResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/getSharedResource.html) 

9. Copyrighted music support song aggregation search.

    Support searching and querying through multiple copyright music libraries by inputting keywords such as singer or song title. The search results can be filtered, sorted, and customized for display.

    For related API, please refer to [sendExtendedRequest](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/sendExtendedRequest.html)

## **Enhancements**

1. Optimize SDK memory usage

    Delete some unnecessary memory applications within the SDK and optimize the SDK's memory usage. Compared to the previous version, the memory usage has decreased by about 10%.

2. Optimize the rotation logic of iOS screen sharing to avoid crashes caused by memory usage exceeding the 50MB limit caused by rotation during the screen recording process.

3. Optimize error code prompts.

    Due to the expiration limit of song resources, the API for obtaining song resources has added a new error code 1008011, indicating that the song resource has expired.

    For related API, please refer to [loadCopyrightedMusicResourceWithPosition](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/loadCopyrightedMusicResourceWithPosition.html)

4. To add a new enumeration value for a music copyright holder.

    To add a new enumeration value for a music copyright holder. Please contact ZEGO technical support.

    For related API, please refer to [ZegoCopyrightedMusicVendorID](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusicVendorID.html)

## **Bug Fixes**

1. Fixed the issue of macOS platform screen sharing where the actual collection frame rate is lower than the set frame rate.

2. Fixed the problem of iOS blurred video picture caused by uncontrolled video hard coding rate when hard coding streaming is performed on iOS 16.4.1 devices.

3. Fixed the issue of abnormal listening of game voice in certain situations.

## **Deleted**

1. Starting from version 3.4.1, support for iOS 11.0 and below has been discontinued, and the iOS Deployment Target (minimum supported version) has been upgraded to iOS 11.0.
For specific instructions, Please refer to [App Store submission requirement starts April 25](https://developer.apple.com/news/?id=jd9wcyov) and [Xcode 14 Release Notes](https://developer.apple.com/documentation/xcode-release-notes/xcode-14-release-notes#Build-System).

2. Starting from version 3.4.1, the iOS SDK no longer supports the 32-bit armv7 architecture.
For specific instructions, Please refer to [Xcode 14 Release Notes](https://developer.apple.com/documentation/xcode-release-notes/xcode-14-release-notes#Build-System).

## 3.3.1

### **Bug Fixes**

1. Fixed the failure of Android hardware ear feedback in some scenarios.

2. Fixed H265 decoding failure in Android  some scenarios.

## 3.3.0

### **New Features**

1. Support for voice enhancement effect in external scenes.

    In the external scene, the microphone of the device is too close to the speaker, which may easily lead to blurred or dull voice. In this scenario, voice enhancement can effectively improve the clarity of voice and improve the sense of boredom. Therefore, it is recommended to enable this function in an external scenario.

    In order to achieve the voice enhancement effect in the external broadcast scene, the voice enhancement effect can be turned on and the enhancement level can be set. It can be used in the KTV external broadcast scene to fine control the voice effect. The recommended enhancement level is 4.

    For related API, please refer to [enableSpeechEnhance](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableSpeechEnhance.html)

2. Game voice supports customized voice mode and listening mode.

    The game voice supports the customized setting of voice mode and listening mode, which can be used to shield the scene of the same team of players outside the range after joining the team.

    For related API, please refer to [setRangeAudioCustomMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoRangeAudio/setRangeAudioCustomMode.html)

3. Express SDK iOS streaming function supports picture-in-picture capability.

    Note: To use this function, please contact ZEGO technical support.

4. The single-stream transcoding function supports L3 or CDN streaming.

    Note: When pulling the transcoding stream through CDN, you must use the push CDN. To use this function, please contact ZEGO technical support.

    Single-stream transcoding refers to converting each original stream into transcoding streams with different encoding formats and different resolutions in the cloud. The transcoding template ID needs to be passed in to pull the transcoding stream. In live broadcast and other scenes, viewers can choose streams of different resolutions to watch based on the quality of the access network, terminal equipment, etc., to ensure the smoothness of playback.

5. The same mixed-stream task supports the output of multiple resolution video streams.

    Note:

    - At present, one mixing task can output up to four video streams with different resolutions, and only server mixing is supported.

    - To use this function, please contact ZEGO technical support.

    The same mixing task supports the output of multiple resolution video streams, which can be used to meet the transcoding requirements in the mixing scenario.

6. Mixed flow task supports input of super whiteboard information.

    In the mixed-stream function, the operation content in the whiteboard can be converted into real-time video, and the whiteboard configuration information can be set, for example, the whiteboard ID, the whiteboard aspect ratio, and whether dynamic PPT loading is supported.

    Note: Currently the super whiteboard flutter SDK is under development.

    For related API, please refer to [ZegoMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMixerTask-class.html), [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

7. Scenario-based audio and video configuration adds [ZegoScenario.StandardVoiceCall] standard voice call scenario.

    StandardVoiceCall standard voice call scenario is added for scenario-based audio and video configuration, which is applicable to 1v1 pure voice call scenario.

    For related API, please refer to [setRoomScenario](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setRoomScenario.html)

8. Android support adaptive device acquisition rendering delay and echo cancellation (AEC) in customized audio acquisition and SDK internal rendering mode.

    Note: To use this function, please contact ZEGO technical support.

    In the customized audio acquisition and SDK internal rendering mode, the AUX adaptive alignment and AEC functions are supported, which can achieve better results when performing chorus in KTV scenes.

9. Added a new copyright content center, and some interfaces support the query of distinguishing copyright parties.

    According to the copyright owner, it supports ordering songs, obtaining line-by-line analysis of lyrics, and querying whether resources are cached.

    Note: For detailed information about the copyright owner, please contact ZEGO technical support for processing.

    For related API, please refer to [requestResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/requestResource.html), [getLrcLyric](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/getLrcLyric.html), [queryCache](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/queryCache.html), [getSharedResource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/getSharedResource.html)

10. When sending an extended request, it is supported to obtain a list of tags, get songs based on tags, query song information in batches, and search for songs according to the copyright owner.

    For related API, please refer to [sendExtendedRequest](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/sendExtendedRequest.html)

### **Enhancements**

1. Optimize the supersection algorithm to greatly improve the coverage of aircraft models.

    Note:
    - [enableVideoSuperResolution] modifies the call time, and can only be called after [initVideoSuperResolution].
    - Video super resolution currently only supports Android and iOS

    For related API, please refer to [enableVideoSuperResolution](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/enableVideoSuperResolution.html), [initVideoSuperResolution](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/initVideoSuperResolution.html)

2. Optimize the over-division logic and add initialization and de-initialization interfaces.

    Note:
    - [enableVideoSuperResolution] modifies the call time, and can only be called after [initVideoSuperResolution].
    - Video super resolution currently only supports Android and iOS

    For related API, please refer to [initVideoSuperResolution](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/initVideoSuperResolution.html), [uninitVideoSuperResolution](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/uninitVideoSuperResolution.html)

3. Optimize the deep echo cancellation (AEC) effect of KTV scenes.

    The deep AEC optimization for KTV scenes has achieved:

    - The sound quality of the human voice in the external scene is greatly improved to make the human voice more fidelity.

    - While eliminating the echo, effectively avoid the occasional swallowing of words or the fluctuation of voice.

4. Optimize the inter-process communication performance of screen sharing on the iOS side of the Express SDK.

    In the application project, developers can start AppGroup configuration through the new [ZegoExpressEngine>setAppGroupID] and [ZegoReplayKitExt>setupWithDelegate:appGroup] interfaces to obtain better performance and stability.

    For related API, please refer to [setAppGroupID](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoScreenCaptureSource/setAppGroupID.html)

### **Bug Fixes**

1. Fixed the problem that the Android player has no sound after plugging the headset.

2. Fixed the problem of Android screen collection failure when the main process does not start the screen sharing service.

3. Fixed the problem that immediately getting the pitch line would fail when getting song accompaniment resources or climax clip resources

## 3.2.1

### **New Features**

1. Add screen capture stream function of web platform;

    Note: Firstly, getting screen capture instance by calling the [createScreenCaptureSource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineScreenCapture/createScreenCaptureSource.html).Sencondly, setting the video soure are "screen" by calling [setVideoSource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setVideoSource.html) function.Now, you can call [startCapture](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoScreenCaptureSource/startCapture.html) for the screen capture instance.

2. Add login multi-room function of web platform;

    Note: On the web platform, [setRoomMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setRoomMode.html) now allows users to log into multiple rooms if they want.

3. Add basic beauty function of web platform;

    Note: On the web platform, support users to use the beauty function by calling [enableEffectsBeauty](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableEffectsBeauty.html) and [setEffectsBeautyParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setEffectsBeautyParam.html) support .

### **Bug Fixes**

1. Fixed the problem that the customized audio acquisition and audio acquisition source need to be reset to take effect after logging in to the room again in some scenarios.

2. Fixed an exception caused by the [onRemoteSoundLevelUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRemoteSoundLevelUpdate.html) callback.

3. Fixed an issue with setting SEIType for the [setSEIConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setSEIConfig.html) interface on iOS that did not work.

## 3.3.0-prerelease.1

### **New Features**

1. Add screen capture stream function of web platform;

    Note: Firstly, getting screen capture instance by calling the [createScreenCaptureSource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineScreenCapture/createScreenCaptureSource.html).Sencondly, setting the video soure are "screen" by calling [setVideoSource](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setVideoSource.html) function.Now, you can call [startCapture](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoScreenCaptureSource/startCapture.html) for the screen capture instance.

2. Add login multi-room function of web platform;

    Note: On the web platform, [setRoomMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setRoomMode.html) now allows users to log into multiple rooms if they want.

3. Add basic beauty function of web platform;

    Note: On the web platform, support users to use the beauty function by calling [enableEffectsBeauty](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableEffectsBeauty.html) and [setEffectsBeautyParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setEffectsBeautyParam.html) support .

### **Bug Fixes**

1. Fixed an exception caused by the [onRemoteSoundLevelUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRemoteSoundLevelUpdate.html) callback.

2. Fixed an issue with setting SEIType for the [setSEIConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setSEIConfig.html) interface on iOS that did not work.

## 3.2.0+1

### **Bug Fixes**

1. Fix the problem that the ZegoVideoSourceType value does not match the actual effect.

## 3.2.0

### **New Features**

1. Support video DualStream.

    When video streams are layered through video DualStream coding (H.264 DualStream), Compared to Layered Video Coding (H.264 SVC), Video DualStream encoding (H.264 DualStream) supports hardware encoding, that is, [ZegoVideoCodecID] adds [H264DualStream] field.

    For related API, please refer to [ZegoVideoCodecID.H264DualStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoVideoCodecID.html).

### **Enhancements**

1. CustomCommand configuration support extended to 4KB.

    Note: The default size of the CustomCommand configuration is 1KB, if it needs to be extended to 4KB, please contact ZEGO technical support for processing.

### **Bug Fixes**

1. Fix hardware coded access to null pointer crash.

2. Fix the problem that the network time module fails to retry.

## 3.1.0

### **New Features**

1. Support multi-source acquisition capability.

    For a variety of interactive scenarios of audio and video sources such as online KTV, watching movies together, watching competitions, video conferences, and online education, multi-source acquisition provides flexible and easy-to-use audio and video acquisition sources and channel management capabilities, greatly reducing developers' development and maintenance costs.

    Multi source acquisition capability shortens, optimizes and normalizes the implementation path of common capabilities such as screen sharing and mixing. After version 3.1.0, you can no longer implement the above complex capabilities through custom acquisition.

    The main capabilities and characteristics are as follows.

    1. Streaming channel supports setting or switching multiple audio and video sources.

    2. Common capabilities such as screen sharing and mixing are supported.

2. SDK supports setting cloud proxy.

    Note: If you need to use this function, please contact ZEGO technical support.

    By setting the cloud proxy interface of the SDK, all the traffic corresponding to the SDK is transferred through the cloud proxy server to achieve communication with the RTC.

    For related API, please refer to [setCloudProxyConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setCloudProxyConfig.html).

### **Enhancements**

1. Greatly improve the audio and video connectivity in extremely weak network areas, and reduce the connection time.

    The ZEGO self-developed dispatching system has been deeply optimized for areas with poor network quality.

### **Bug Fixes**

1. Fixed the problem that sending room [Logout] signaling may fail.

2. Fixed Android the occasional black screen problem caused by the camera authority occupation.

3. Fixed occasional crash of Android audio player.

4. Fixed iOS the occasional crash of null pointer in restart access after the video hardware failure.

5. Fixed Android the problem of device restart in low latency mode.

## 3.0.3

### **💥 Breaking changes**

This version contains breaking changes, please refer to [v3.0.3 Upgrade Guide](./doc/v3.0.0+_upgrade_guide.md) for details.

### **New Features**

1. New video super-resolution capability

    Note: If you need to use this function, please contact ZEGO technical support. This feature currently only supports IOS and Android platforms.

    The new [enableVideoSuperResolution] interface supports super-resolution processing of a video stream to achieve better image quality. Super resolution, referred to as super resolution, is a technology that the client multiplies the width and height of the pulled video stream in real time. For example, from 640x360 to 1280x720.

    For related API, please refer to [enableVideoSuperResolution](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/enableVideoSuperResolution.html), [onPlayerVideoSuperResolutionUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerVideoSuperResolutionUpdate.html)

2. Scene AI noise reduction adds the ability to reduce noise in music scenes

    Note: If you need to use this function, please contact ZEGO technical support. This feature currently only supports IOS，Android，mac and window platforms.

    The scene AI noise reduction function, based on the previous noise reduction for all non human voices, supports the noise reduction capability in the music scene, and restores the music sound quality by identifying music and intelligently adjusting the noise reduction effect. The SDK will perform music detection on the microphone input in real time, and automatically adjust the noise reduction level in sound card, playing and singing or near field music scenes to ensure the high fidelity sound quality of music.

3. Scenario of adding room dimensions

    In order to facilitate the rapid access of developers and reduce the access threshold for developers, the SDK provides a variety of preset scenarios. Developers can select the corresponding room mode [ZegoScenario](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoScenario.html) according to the desired scenario, and the SDK will automatically apply audio and video codecs, audio and video parameters, flow control strategies and other configurations suitable for the scenario, so as to quickly achieve the best effect in the scenario.

    Currently supported scenarios include live show, KTV, standard 1v1 audio and video calls, high quality 1v1 audio and video calls, standard chat rooms, and high quality chat rooms.

    For related API, please refer to [setRoomScenario](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setRoomScenario.html)

4. Support for obtaining the codec capability of the specified video codec of the current device

    Note: The interface adds an optional parameter codecBackend. The return value is changed to an int type; 0 means not supported, and this encoding format cannot be used for streaming; 1 means supported, and this encoding format can be used for streaming; 2 means not confirmed, and it is recommended to call this interface later.This feature currently only supports IOS，Android，mac and window platforms.

    The SDK supports obtaining the support of the codec mode of the video codec specified by the current device, so as to better help developers choose the encoder and codec mode to use and obtain better results.

    The hardware or software encoding support of the current encoder can be obtained through the [isVideoEncoderSupported] interface.

    Through the [isVideoDecoderSupported] interface. The hardware or software decoding support of the current decoder can be obtained. The above two interfaces contain three enumerated values:support hardware or software, support hardware, and support software.

    For example, ZegoExpressEngine.instance.isVideoEncoderSupported(ZegoVideoCodecID.H265, codecBackend: ZegoVideoCodecBackend.Hardware)means to check whether the current device supports H265 hardcoding. If yes, return 1.

    For related API, please refer to [isVideoEncoderSupported](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/isVideoEncoderSupported.html), [isVideoDecoderSupported](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/isVideoDecoderSupported.html)

5. Added switch interface for acquiring GPS information

    Note: This function is enabled by default. If you need to disable this function, please contact ZEGO technical support.  n If the app has access to the geographical location, the developer can choose whether to allow the ZEGO SDK to obtain the GPS information cached by the system, which is obtained by default. When developers want to disable this function, they need to contact ZEGO technical support to set it.This feature currently only supports IOS ， Android，mac and window platforms.

6. New video first frame callback based on camera opening

    Support callback after SDK pulls the stream and renders the first frame of remote camera video data each time the remote camera is turned on. Developers can use this callback to calculate the first frame time consumption, or update the UI components of the playback stream.This feature currently only supports IOS，Android，mac and window platforms.

    For related API, please refer to [onPlayerRenderCameraVideoFirstFrame](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerRenderCameraVideoFirstFrame.html)

7. Add the CDN Plus playing configuration to the startPlayingStream interface on the web platform.

    Note: If you want to control the stream-playing mode from the cloud by more criteria such as region and user, please contact ZEGO technical support for related configuration.

    In the past, the web did not support playing CDN plus.Now, we can play CDN plus on the IOS，Android，mac, window and web platforms.The [startPlayingStream] interface adds CDN_PLUS as a new ZegoResourceType on the web platform. interface. You can enable CDN_PLUS to play stream by yourself based on to the stream critirion. The CDN Plus stream-playing is a cost-effective method, because its quality is higher than CDN stream-playing with similar price.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

### **Enhancements**

1. Optimized for 1v1 pure RTC call scenarios

    Note: If you need to use this function, please contact ZEGO technical support.

    It is optimized for 1v1 call scenarios and is applicable to pure RTC scenarios.

2. Optimize space audio function

    The space audio capability is optimized, so that users can distinguish the front and rear audio sources, so as to achieve a better sense of immersion.

3. Optimize the situation of broken sound caused by excessive collection volume

    The AGC automatic gain control algorithm is optimized. When the acquisition volume is too large, it will not cause broken sound.

4. Optimize the audio and video experience in extremely weak networks

    The SDK optimizes the internal strategy. In the audio and video scenarios, it supports a minimum downlink of 50 kbps without congestion, ensuring a better experience in extremely weak networks.

### **Bug Fixes**

1. Fixed the problem that under some system versions of Mac M1 chip computers, the Web side enabled hardware encoding streaming, and used multiple SPS (Sequence Parameter Set) and PPS (Picture Parameter Set) output forms.

2. Fixed the problem that the logs currently being uploaded may crash when the network status is switched from having a network to having no network.

3. Fixed GetCallbackController non thread safety issue.

4. Fixed the problem that the SDK did not trigger [onPlayerRenderVideoFirstFrame] without a View.

5. Fixed the SDK crash caused by repeatedly starting and stopping the camera under Android system.

6. Fixed the problem of abnormal screen scaling when iOS, macOS, and windows use texture mode to render the screen.

7. Fixed the problem of iOS hardware decoding failure.

    For related API, please refer to [enableHardwareDecoder](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/enableHardwareDecoder.html)

8. Fixed the problem that when streaming is stopped in the multi room mode, other people in the room cannot receive stream deletion notifications.

9. Fixed the possible crash of hardware decoding on iOS, macOS and Windows platforms.

10. Fixed the Android issue that [onNetworkQuality] not calling back

### **Deleted**

1. Three old versions of [ZegoScenario] scenarios have been abandoned
Discard [General], [Communication] and [Live] three scenarios in the [ZegoScenario] scenario enumeration.

2. [setDebugVerbose],[setPlayStreamVideoLayer],[setBuiltInSpeakerOn],[onDeviceError],[loginMultiRoom]  are deleted.

## 2.23.0

### **💥 Breaking changes**

This version contains breaking changes, please refer to [v2.23.0 Upgrade Guide](./doc/v2.23.0_upgrade_guide.md) for details.

### **New Features**

1. It supports macOS now.

    Now it supports macOS, and the interface is basically aligned with the mobile phone, except for a few interfaces that only support specific platforms. macOS now only supports TextureRenderer for rendering.

2. Windows support [setPlayerCanvas] [takePublishStreamSnapshot] [takePlayStreamSnapshot] [takeSnapshot].

    For related API, please refer to [setPlayerCanvas](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/setPlayerCanvas.html), [takePublishStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/takePublishStreamSnapshot.html), [takePlayStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/takePlayStreamSnapshot.html), [takeSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/takeSnapshot.html)

3. Added 3 new modes for video capture and rotation function on mobile.

    Due to the angle, resolution, rotation and other characteristics of mobile cameras, developers need to do many complex adaptations.

    The current SDK encapsulates various configurations and provides simple mode selection. On the basis of the original custom mode, a new fixed scale mode, adaptive mode and alignment mode can be added, which can effectively reduce the access cost of developers.

    Not supported in TextureRenderer render mode on iOS now.

    For related API, please refer to [setAppOrientationMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setAppOrientationMode.html)

4. Added support for reverb effects to the system ear feedback of Huawei mobile phones.

    Note: To use this feature, please contact ZEGO Technical Support.

    When use the system ear return of Huawei mobile phones, add the reverberation effect support. Due to the limitation of the system, only the following three reverb effects are supported: KTV, Theater, Concert, and the default is KTV. If you choose a reverb effect other than those three effects, it still defaults to the KTV effect.

5. Added web support for some interfaces.
    Some interface functions are not supported by the web. At present, the web supports more basic functions.we supports to setting video config, sending message, mixing audio, etc;

### **Deleted**

1. Legacy video rendering APIs have been removed.

    In order to facilitate developers to implement the video rendering feature, the [createTextureRenderer] [destroyTextureRenderer] [updateTextureRendererSize] and [createPlatformView] [destroyPlatformView] API modules have been removed since v2.23.0.

    Therefore, when upgrading from an old version to v2.23.0, developers need to migrate to the new [createCanvasView] and [destroyCanvasView] API to implement the video rendering feature.

    The new ZegoExpressCanvasViewUtils API module can support both External Texture and PlatformView rendering methods.

    For the migration documentation, see [v2.23.0 Upgrade Guide](https://docs.zegocloud.com/article/14906)

    For related API, please refer to [createCanvasView](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressCanvasViewUtils/createCanvasView.html), [destroyCanvasView](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressCanvasViewUtils/destroyCanvasView.html)

### **Enhancements**

1. Optimized the performance of the super-resolution feature for Android machines using Qualcomm chips.

    Note: To use this feature, please contact ZEGO Technical Support.

    Optimize the performance of the super-resolution function, increase the maximum supported resolution, improve the model coverage, optimize resource usage and inference speed.

### **Bug Fixes**

1. Fixed an issue where calling [loginRoom] during network switching (Wi-Fi or cellular) may not have a callback in multi-room mode.

2. Fixed the issue that the voice of people in the original squad could be heard outside the range of the range voice function after exiting the squad.

3. Fixed a possible crash on Android 5.1 and below.

4. Fixed some bugs found.

## 2.22.0

### **New Features**

1. Add support for SOCKS5 client proxy

    In an intranet or firewall scenario, you can interact with the public network through a proxy server, and set the proxy server address through [setEngineConfig] to ensure that the ZEGO's cloud-based RTC service is normal. Currently only supports SOCKS5 protocol.

    For related API, please refer to [setEngineConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setEngineConfig.html)

2. Low-light enhancement supports using Metal

    Note: Low-light enhancement uses OpenGL by default, if you need to specify Metal, please contact ZEGO technical support.

3. The custom video capture function supports H.265 encoded stream.

    Note: It is recommended to set a GOP every 2s, and each I frame must carry SPS and PPS and put them at the first. When calling [enableCustomVideoCapture], the type must be set to [ZegoVideoBufferType.EncodedData]. It does not supports B frame.

    For related API, please refer to [enableCustomVideoCapture](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineCustomVideoIO/enableCustomVideoCapture.html)

4. Support dynamically modify AudioDeviceMode [iOS/Android]

    Add [setAudioDeviceMode] to dynamically modify the audio mode of the device. This configuration determines the volume mode, preprocessing mode and Mic occupation logic of the device. You can choose according to specific scenarios.

    For related API, please refer to [setAudioDeviceMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setAudioDeviceMode.html)

5. Support for calling back vocal-aligned PCM data from media player. [iOS/Android]

    Note: 1. When using the media player to play the accompaniment, you need to use the [enableAux] interface at the same time. 2. After enabling the [enableAlignedAudioAuxData] interface, the data of the media player will not be pushed out.

    If you need to tune the accompaniment and align the vocals in the recording and singing scene, you can first mix the accompaniment into the main channel through the [enableAux] interface, then turn on the switch through the [enableAlignedAudioAuxData] interface, and finally through the [onAlignedAudioAuxData] interface. ] The callback obtains the PCM data of the media player. At this time, the data collected by the media player and the Mic are aligned, and the data frames correspond one-to-one.

    For related API, please refer to [enableAlignedAudioAuxData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineCustomAudioIO/enableAlignedAudioAuxData.html), [onAlignedAudioAuxData](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onAlignedAudioAuxData.html), [enableAux](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/enableAux.html)

6. The H.264 and H.265 hardware codec function supports using AMD/NVIDIA/Intel graphics cards. [Windows]

    Note: To enable this feature, you need to contact ZEGO technical support.

    It supports AMD/NVIDIA discrete graphics cards and Intel / AMD integrated graphics in Windows. You can modify the default graphics card priority through [setEngineConfig].

    For related API, please refer to [setEngineConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setEngineConfig.html), [enableHardwareEncoder](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/enableHardwareEncoder.html), [enableHardwareDecoder](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/enableHardwareDecoder.html)

7. It supports windows now.

    Now it supports windows, and the interface is basically aligned with the mobile phone, except for a few interfaces that only support specific platforms. Windows currently does not support [setPlayerCanvas] [takePublishStreamSnapshot] [takePlayStreamSnapshot] [takeSnapshot].

    For related API, please refer to [setPlayerCanvas](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/setPlayerCanvas.html), [takePublishStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/takePublishStreamSnapshot.html), [takePlayStreamSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/takePlayStreamSnapshot.html), [takeSnapshot](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/takeSnapshot.html)

8. Add [createCanvasView], Used to create canvas views.

    It integrates the [createTextureRenderer] and [createPlatformView] interfaces to simplify the creation of the rendering window. When it is a TextureRenderer, it is no longer necessary to actively call [updateTextureRendererSize] to update the window size. The corresponding destruction needs to use [destroyCanvasView].

    For related API, please refer to [createCanvasView](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressCanvasViewImpl/createCanvasView.html), [destroyCanvasView](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressCanvasViewImpl/destroyCanvasView.html)

### **Enhancements**

1. Optimize network quality callbacks to sense abnormal status of remote users.

    When the remote user is abnormal, [onNetworkQuality] will call back the quality unknown state (ZegoStreamQualityLevel.Unknown state) every 2s. When the user remains in this state for 8s, the remote user is considered to be abnormally disconnected, and the quality abnormal state (ZegoStreamQualityLevel.Die state) will be called back.

    For related API, please refer to [onNetworkQuality](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onNetworkQuality.html)

2. Optimize the network quality callback, the network quality feedback is more sensitive.

    The push-pull stream quality callback will call back the result with the worst quality every 3s. When serious jitter or packet loss occurs during the period, the poor stream quality can be immediately reported.

    For related API, please refer to [onPlayerQualityUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerQualityUpdate.html), [onPublisherQualityUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPublisherQualityUpdate.html), [onNetworkQuality](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onNetworkQuality.html)

3. Optimize log reporting strategy.

    Optimize the log reporting strategy, improve log upload efficiency.

4. Optimized harmonic detection algorithm for AGC.

    AGC's newly improved harmonic detection algorithm has a crash problem, and is now back to the old version of the harmonic detection algorithm.

### **Bug Fixes**

1. Fixed a very low probability of crash in the network module.

## 2.21.2

### **Bug Fixes**

1. Android build fail.

## 2.21.1

### **Bug Fixes**

1. Fixed the problem of sending SEI failed in pure audio scene.

## 2.21.0

### **New Features**

1. It supports web now.

    Now it supports the web, which will be different from other platforms in use. Some interface functions are not supported by the web. At present, the web supports basic functions

2. Range voice supports custom distance update frequency.

    The default distance update frequency of the SDK is changed from 1s to 100ms, which can basically meet the smooth attenuation effect for most developers when using range voice, optimize the experience of sound attenuation when using range voice, and achieve a smoother and more natural attenuation effect.

    If you want to better match the actual business demand, you can call the [setPositionUpdateFrequency] interface to modify the frequency by yourself.

    For related API, please refer to [setPositionUpdateFrequency](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoRangeAudio/setPositionUpdateFrequency.html)

3. Support setting the low-light enhancement.

    Note: The [setLowlightEnhancement] interface should be called after calling the [createEngine] interface to create an engine.

    When the surrounding environment of the stream-publishing user is dark, or the frame rate set by the camera is high, resulting in a dark live broadcast screen, and the subject cannot be displayed or recognized normally, you can call the [setLowlightEnhancement] interface to set the low-light enhancement to increase the brightness of the video screen. The low-light enhancement function includes three modes: 1: Disable the low-light enhancement (default), 2: Enable the low-light enhancement, 3: Automatically switch on/off the low-light enhancement.

    You can choose different low-light enhancement modes according to business scenarios: when you want to judge whether the low-light enhancement is needed, you can switch between modes 1 and 2; when you want the SDK to automatically enhance the brightness, you can enable the mode 3, and the SDK will automatically determine the lighting environment where the user is in, and turn on or off the low-light enhancement.

    For related API, please refer to [setLowlightEnhancement](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setLowlightEnhancement.html)

4. Support setting video borders to rounded corners when mixing streams.

    When calling the [startMixerTask] interface to mix streams, you can set the "cornerRadius" through the "ZegoMixerInput" class to turn the video border to rounded corners. The unit of "cornerRadius" is px, and the value cannot exceed the half of the width or the height of video screen, which is shorter.

    For related API, please refer to [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

5. Add the CDN Plus playing configuration to the startPlayingStream interface.

    Note: If you want to control the stream-playing mode from the cloud by more criteria such as region and user, please contact ZEGO technical support for related configuration.

    The [startPlayingStream] interface adds CDN_PLUS as a new ZegoResourceType. interface. You can enable CDN_PLUS to play stream by yourself based on to the stream critirion. The CDN Plus stream-playing is a cost-effective method, because its quality is higher than CDN stream-playing with similar price.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

### **Enhancements**

1. Optimize the related error codes of Token exceptions when mandatory login authentication is enabled.

    Added 1002074, 1002075, 1002076, 1002077, 1002078, 1002079, 1002080 and other error codes. After enabling mandatory login authentication, if the Token is incorrect, these error codes will be returned. For details, please refer to Error codes.

2. Optimized the alignment between vocal and accompaniment for real-time chorus scenario.

    The alignment between vocal and accompaniment for real-time chorus scenario is optimized. Therefore，you can avoid the unaligned streams of vocal and accompaniment caused by the delay of device playback when the client publishes the two streams at the same time.

### **Bug Fixes**

1. Fixed an issue where [onStop] callbacks would be triggered repeatedly when external filters are turned off.

2. Fixed an issue where play stream with L3 failed.

    Fixed the issue that when the 2.20.0 ~ 2.20.2 SDK uses L3 to play streams, if the played-stream is the stream which is published by the SDK of 2.15.0 and earlier versions, it may fail.

3. Fixed some bug about Custom Audio IO

## 2.21.0-prerelease.1

### **New Features**

1. It supports web now.
    Now it supports the web, which will be different from other platforms in use. Some interface functions are not supported by the web. At present, the web supports basic functions.

## 2.20.2

### **Bug Fixes**

1. Fixed a problem of probabilistic stream-playing failure.

2. Optimized the problem that the audio device mode set before the SDK initialization does not take effect.

## 2.20.0

### **New Features**

1. Media player supports setting the channel.

    After calling the [createEngine] interface to initialize the engine and the [createMediaPlayer] interface to create a media player, you can call the [setActiveAudioChannel] interface to set the left channel, right channel or all channels. When initialized, the media player defaults to all channels.

    For related API, please refer to [setActiveAudioChannel](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/setActiveAudioChannel.html)

2. Media player supports clearing the last frame when the playback ends.

    Note: You must wait for the media player to finish playing before the API call takes effect.

    Call the [createEngine] interface to initialize the engine, call the [createMediaPlayer] interface to create a media player, and call [clearView] to clear the last remaining frame.

    For related API, please refer to [clearView](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/clearView.html)

3. Support the camera to automatically change the frame rate.

    Note: When the frame rate set by [setVideoConfig] is less than the minimum expected frame rate of [enableCameraAdaptiveFPS], the frame rate value set by [setVideoConfig] will be used. Due to the different hardware and algorithm strategies of different mobile phone manufacturers, the effect of this interface is different on different models or on the front and back cameras of the same model.

    When the frame rate set by the user on the streaming end is high, and the ambient light is low and the subject cannot be displayed or recognized normally, you can call the [enableCameraAdaptiveFPS] interface to automatically reduce the frame rate within a certain range to increase exposure time, so as to improve the brightness of the video picture. This function is often used in live broadcast scenes with high exposure requirements. The [enableCameraAdaptiveFPS] interface needs to be called after calling the [createEngine] interface to initialize the engine and before starting the camera.

    For related API, please refer to [enableCameraAdaptiveFPS](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/enableCameraAdaptiveFPS.html)

4. Support setting the picture information of a single input stream in the mixed stream.

    Note: The length of the image address must not exceed 1024 bytes, otherwise the error code 1005034 will appear; the image format should be JPG and PNG format, otherwise the error code 1005035 will appear; the image must not exceed 1M, otherwise the error code 1005036 will appear.

    You can set the image address through the "ZegoMixerImageInfo" type parameter of the [startMixerTask] interface to set the content of a single input stream as an image, which is used to replace the video, that is, when the image is used, the video is not displayed. This function is mainly used in a video call when a video caller may need to temporarily turn off the camera to display the image, or in a call between a video caller and a voice caller when the image of the voice caller may need to be displayed.

    For related API, please refer to [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

5. Support setting whether to clear the last frame after calling the mutePlayStreamVideo interface.

    NOTE: To use this feature, please contact ZEGO Technical Support.

    When you finds that the stream publisher violates the regulations, you can call the [mutePlayStreamVideo] interface to discontinue the stream puller from pulling the video stream of the violating user, and request the violating user to make corrections. Using this function at the same time can avoid the risk of violation caused by the video interface of the stream puller still retaining the last frame.

6. Supports linearly increasing volume gain.

    NOTE: To use this feature, please contact ZEGO Technical Support.

    A new volume gain method is provided, and you can choose an appropriate volume gain method according to actual needs.

7. Supports automatic censorship of audio and video at the stream level.

    Note: To use this function, please contact ZEGO technical support to activate the background service.

    When calling the [startPublishingStream] API to start streaming, you can set the "ZegoStreamCensorshipMode" parameter to conduct automatic audio and video censorship at the stream level, and automatically identify sensitive content, thus reducing the integration difficulty and business maintenance costs.

    For related API, please refer to [startPublishingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/startPublishingStream.html)

### **Enhancements**

1. The error code when the developer passes in a non-existing AppID has been optimized from 1002099 to 1001004.

2. The echo cancellation has been optimized to solve the swallowed sound phenomenon in KTV and other scenarios.

3. Added 1009013 error code.

    It indicates that the message input length exceeds the limit. When this error code appears, please check the input content length or contact ZEGO technical support to extend the message content length.

4. Added 1017009 error code.

    When the copyrighted music is initialized, the authentication fails because the AppSign or Token is not set, and this error code will appear. At this time, if you use AppSign for authentication, please fill in AppSign when initializing the SDK; if you use Token authentication, before calling the [initCopyrightedMusic] interface, please call the [loginRoom] interface and pass in Token for authentication.

    For related API, please refer to [initCopyrightedMusic](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCopyrightedMusic/initCopyrightedMusic.html), [loginRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/loginRoom.html)

### **Bug Fixes**

1. Fixed an error reporting the custom capture AAC audio format.

## 2.19.0

### **New Features**

1. Direct push CDN streams support pulling streams through L3.

    When pushing CDN directly, without changing the push mode, the SDK pulls the stream from the customer's CDN source site, distributes the audio and video content to the audience through L3, and controls the source site resources through [ZegoResourceType]. This function is often used in live broadcast scenarios.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

2. Support SEI data synchronization with audio frame in audio and video scenarios.

    Note: Currently, only RTC scenarios are supported, and it is invalid in direct CDN and retweet CDN scenarios.

    Starting from version 2.19.0, SEI (Media Supplemental Enhancement Information) can be sent synchronously with audio frames in audio and video scenarios. This function is often used in video scenarios where SEI is strongly related to audio, such as real-time KTV.

    In versions before 2.19.0, the SEI data was sent along with the video frame data. Generally, the video frame rate is much lower than the audio frame rate, resulting in insufficient SEI accuracy/frequency in mixed stream alignment and accompaniment alignment scenarios.

    For related API, please refer to [onPlayerRecvAudioSideInfo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerRecvAudioSideInfo.html), [sendAudioSideInfo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/sendAudioSideInfo.html)

### **Enhancements**

1. For improved security, this release upgrades curl to 7.82.0.

### **Bug Fixes**

1. Fixed the problem that the network speed measurement callback could not be received when the start streaming interface was called immediately after the network speed measurement interface was called.

2. Fixed the issue that H.265 did not take effect when only recording local media (not streaming).

3. Fixed the issue that when using certain Android models and certain Bluetooth headsets together, the call volume was changed from Bluetooth to loudspeaker when the acquisition was enabled.

4. Fixed other known issues.

## 2.18.1

### **New Features**

1. Added AI noise reduction function.

    Note: AI noise reduction will currently cause great damage to the music collected by the microphone, including the sound of people singing through the microphone. To use this feature, please contact ZEGO Technical Support.

    AI noise reduction means that the SDK will perform noise reduction processing on the sound collected by the microphone. In the case of normal processing of the original steady-state noise, it will also deal with non-steady-state noise, mainly including mouse, keyboard sound, tapping , air conditioners, kitchen dishes, noisy restaurants, ambient wind, coughing, blowing and other non-human noises. The AI noise reduction mode is set through the ZegoANSMode parameter in the [setANSMode] interface, and the noise reduction mode can be adjusted in real time.

    This function is often used in calls, conferences and other scenarios without background music, such as normal voice chat rooms, voice conferences, voice blackouts, and one-to-one video calls.

    For related API, please refer to [setANSMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setANSMode.html)

2. Support setting the playback speed of sound effects.

    After playing the sound effect, you can call the [SetPlaySpeed] interface to set four playback speeds for the sound effect (the local playback speed and the streaming speed will be set at the same time), which are 0.5 times, original speed, 1.5 times and 2 times respectively, and the default is the original speed.

3. Supports push-pull streaming using QUIC protocol during CDN live streaming.

    The QUIC protocol push-pull streaming is mainly used to improve the unstable quality of CDN live streaming in a weak network environment, but the improvement is limited. It is recommended to use low-latency live streaming to enjoy high-quality and low-latency live streaming services. Currently, QUIC protocol push and pull streaming using Tencent and Wangsu's two CDN live streaming products are supported.

    Configure the push protocol and QUIC version through the "ZegoCDNConfig" parameter in the [enablePublishDirectToCDN] interface. If you want to perform custom CDN streaming of the QUIC protocol, you need to configure the pull protocol through the "ZegoPlayerConfig" parameter in [startPlayingStream] and the QUIC version.

    For related API, please refer to [enablePublishDirectToCDN](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/enablePublishDirectToCDN.html)

4. Address and protocol-related information that supports monitoring of push-pull streams.

    After the push stream is initiated, you can monitor the push stream status in real time through the [onPublisherStreamEvent] callback, which will return the current push stream address, resource type, and protocol-related information.

    After initiating the streaming, you can monitor the streaming status in real time through the [onPlayerStreamEvent] callback, which will return the current streaming address, resource type, and protocol-related information.

    For related API, please refer to [onPublisherStreamEvent](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPublisherStreamEvent.html), [onPlayerStreamEvent](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerStreamEvent.html)

5. Support setting muxed watermark and muxed input volume through URL.

    Call startMixerTask to start or update the muxing task. It supports setting the muxing watermark and muxing input volume through "backgroundUrl" and "inputVolume" respectively.

    For related API, please refer to [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

6. Added room state change notification [onRoomStateChanged].

    When the connection state of the room changes, the [onRoomStateChanged] callback will be triggered, and the "ZegoRoomStateChangedReason" parameter will provide more detailed connection state and the reason for the state change.

    For related API, please refer to [onRoomStateChanged](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onRoomStateChanged.html)

### **Enhancements**

1. Mixed text watermark supports setting font border and color.

    Call the startMixerTask interface, use the "border" property in "ZegoFontStyle" to set whether the font has a border, and use the "borderColor" property to set the font border color.

    For related API, please refer to [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

2. Improve the error code returned by the start mixing interface [startMixerTask].

    Added an error code of 1005000, indicating that the mixed streaming service has not been activated. When this error code occurs, please activate the mixed streaming service by contact ZEGOCLOUD technical support.

    For related API, please refer to [startMixerTask](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineMixer/startMixerTask.html)

### **Bug Fixes**

1. Fixed an issue that could cause a crash when the parameter name length of the string type is too long.

2. Fixed the issue that when the push-end App returned to the front-end from the background, the pull-end could not receive the BreakResume event.

3. Fix a memory leak that the android plugin binding is not released when using v2 embedded.

4. Fix a memory leak that the temp pixel buffer of the iOS texture renderer could not be released.

5. Fixed other known issues.

## 2.17.2

### **Bug Fixes**

1. Fixed the problem of log file collection during log reporting.

2. Fixed echo cancellation issues on some phones.

## 2.17.1

### **New Features**

1. Supports setting the minimum video frame rate and video resolution.

    Added the [setMinVideoFpsForTrafficControl] and [setMinVideoResolutionForTrafficControl] interfaces, which can be used to set the minimum video frame rate and resolution by calling the interface when the user's network is poor and the flow control is turned on, helping the user to comprehensively control the video display effect.

    For related API, please refer to [setMinVideoFpsForTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setMinVideoFpsForTrafficControl.html), [setMinVideoResolutionForTrafficControl](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setMinVideoResolutionForTrafficControl.html)

2. Support steady-state voice setting detection period parameter.

    The default detection period for steady-state voice is 3 seconds. If users need to modify the default detection period, they can customize the detection period parameters through the [startAudioVADStableStateMonitor] interface.

    For related API, please refer to [startAudioVADStableStateMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/startAudioVADStableStateMonitor.html)

3. A stealth squad mode has been added to the area voice.

    Added enum ZegoRangeAudioModeSecretTeam secret team mode. In this mode, users and listeners in the same room can not only communicate with people in the same team, but also hear the voices of all voices within the audio reception range that are voices in the world mode, such as the space werewolf killing game scene.

    For related API, please refer to [setRangeAudioMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoRangeAudio/setRangeAudioMode.html)

4. Added debugging assistant function.

    Note: This function is only used in the development stage, please do not enable this function in the online version.

    Added the [enableDebugAssistant] interface. The developer calls this interface to enable the debugging assistant function. The SDK will print the log to the console, and the UI will pop up an error message when other functions of the SDK are called abnormally.

    For related API, please refer to [enableDebugAssistant](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/setRangeAudioMode.html)

### **Enhancements**

1. The authentication method is optimized.

    For versions 2.17.0 and above, pass the AppSign blank or not when creating the engine, and you must pass in the Token when logging in to the room. After the authentication is passed, you can use the real-time audio and video function.

    Versions below 2.17.0, pass in AppSign when creating the engine, and use the real-time audio and video function after the authentication is passed.

    For related API, please refer to [createEngineWithProfile](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/createEngineWithProfile.html), [ZegoEngineProfile](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoEngineProfile-class.html),  [loginRoom](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineRoom/loginRoom.html), [ZegoRoomConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoRoomConfig-class.html)

### **Bug Fixes**

1. Fixed an issue that could cause a crash when the parameter name length of the string type is too long.

2. Fixed the issue that when the push-end App returned to the front-end from the background, the pull-end could not receive the BreakResume event.

3. Fixed other known issues.

4. Fixed an issue where pushing down real-time audio and video streams failed on 32-bit machines.

## 2.16.3

### **New Features**

1. Game voice support setting whether to receive audio data from the specified user.

    The [muteUser] interface has been added to the game voice module. Local users can set whether to receive audio data from the specified remote user through the [MuteUser] interface after initializing the game voice [CreateRangeAudio] according to their needs.

    This function is often used in game scenarios, such as the speaker is blocked by a wall, the listener does not need to receive the sound.

    For related API, please refer to [muteUser](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoRangeAudio/muteUser.html)

2. Pull stream quality callback [onPlayerQualityUpdate] added MOS sound quality score.

    The [onPlayerQualityUpdate] callback adds a "mos" parameter, which indicates the rating of the streaming quality. When developers are more concerned about audio quality, they can use this parameter to know the current audio quality.

    For related API, please refer to [ZegoPlayStreamQuality](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoPlayStreamQuality-class.html)

3. Supports streaming based on rtmp over quic protocol.

    NOTE: To use this feature, please contact ZEGO Technical Support.

    In order to allow the streaming end to push higher-quality video streams in a weak network environment, the SDK supports streaming based on the rtmp over quic protocol.

    This function is often used in single-host live CDN and live PK scenarios.

4. H.265 pull stream supports automatic downgrade.

    NOTE: To use this feature, please contact ZEGO Technical Support.

    Version 2.15.0 and earlier: When the SDK uses [startPlayingStream] to pull the H.265 encoded stream, if the decoding frame rate is insufficient due to poor hardware performance on the streaming end, the SDK cannot actively downgrade, and the user needs to first Stop pulling the H.265 encoded stream and re-pull the H.264 encoded stream.

    Version 2.16.0 and above: Added the H.265 streaming automatic downgrade policy. When using [startPlayingStream] to pull H.265 encoded streams, the SDK can compare the hardware performance of the streaming end according to the streaming quality. If the decoding frame rate is insufficient due to the difference, the H.264 encoded stream will be automatically downgraded.

    For related API, please refer to [startPlayingStream](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/startPlayingStream.html)

### **Enhancements**

1. Android SDK minimum supported OS version changed from Android 4.1 to Android 4.4.

    Starting from this version, the API level required by the Android SDK is not lower than 19, and the minimum supported OS version has changed from Android 4.1 to Android 4.4. If you need to support Android 4.1, please contact ZEGO technical support.

2. Optimized the basic beauty function.

    ZEGO provides a new basic beauty function, showing users a good skin condition and creating a natural beauty effect. Developers need to call the [startEffectsEnv] interface to initialize the beauty environment before pushing the stream, and then call the [enableEffectsBeauty] interface to enable the beauty function. Through the [setEffectsBeautyParam] interface, you can adjust the degree of whitening, smoothing, sharpening, and ruddy as needed to achieve basic beauty capabilities.

    This function is often used in video calls, live broadcasts and other scenarios.

    For related API, please refer to [startEffectsEnv](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/startEffectsEnv.html), [stopEffectsEnv](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/stopEffectsEnv.html), [enableEffectsBeauty](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableEffectsBeauty.html), [setEffectsBeautyParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setEffectsBeautyParam.html)

3. Android system of version 8.0 and above supports getting CPU usage.

4. Optimize NTP time error.

    When calling the [getNetworkTimeInfo] interface to obtain synchronized network time information, the SDK will regularly update the NTP time to reduce the error of the obtained NTP time.

    For related API, please refer to [getNetworkTimeInfo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineUtilities/getNetworkTimeInfo.html)

### **Deleted**

1. Abandoned the old version of the basic beauty related interface.

    The old beauty function is relatively simple and does not meet the developer's expectations. Therefore, the [enableBeautify] interface is deprecated in version 2.16.0 and above, please use the [enableEffectsBeauty] interface instead; the [setBeautifyOption] interface is deprecated, please use [setEffectsBeautyParam] ] replace.

    For related API, please refer to [enableBeautify](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDeprecatedApi/enableBeautify.html), [enableEffectsBeauty](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableEffectsBeauty.html), [setBeautifyOption](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDeprecatedApi/setBeautifyOption.html), [setEffectsBeautyParam](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/setEffectsBeautyParam.html)

### **Bug Fixes**

1. Fixed the issue that when the [loginRoom] and [startPublishingStream] interfaces are called at the same time, the peer end may not receive a new stream notification.

2. Fixed an issue where users could not receive room additional messages sent by other users in the room before logging into the room.

3. Fix known issues.

## 2.15.0

### **New Features**

1. The media player supports obtaining sound waves and spectrum when playing music

    The media player has added a new sound wave spectrum callback and switch interface, which can control whether to turn on the callback and the frequency of the callback, so as to obtain the current sound wave and spectrum of the media player. When playing resources through the media player, such as watching a movie together or chatting in a room with a game, this function can be used to perform the function of spectrum animation to increase the interest.

    After creating the media player, call the [enableSoundLevelMonitor] interface to enable sound monitoring. After enabling it, you can use the [onMediaPlayerSoundLevelUpdate] callback to monitor the sound changes.

    After creating the media player, call the [enableFrequencySpectrumMonitor] interface to enable spectrum monitoring. After enabling it, you can use the [onMediaPlayerSoundLevelUpdate] callback to monitor the spectrum changes.

    For related API, please refer to [enableSoundLevelMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/enableSoundLevelMonitor.html), [onMediaPlayerSoundLevelUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onMediaPlayerSoundLevelUpdate.html), [enableFrequencySpectrumMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/enableFrequencySpectrumMonitor.html), [onMediaPlayerFrequencySpectrumUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onMediaPlayerFrequencySpectrumUpdate.html)

2. Support all-round virtual stereo.

    Added support for omni-directional virtual stereo sound. The monophonic sound is processed by algorithms to simulate a somatosensory sound. This function is often used in KTV scenes to make the singing sound more three-dimensional.

    When the [enableVirtualStereo] interface is called and the angle parameter is set to -1, it means that the stereo effect is omnidirectional stereo.

    For related API, please refer to [enableVirtualStereo](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePreprocess/enableVirtualStereo.html)

3. Added local device exception callback [onLocalDeviceExceptionOccurred].

    Through the [onLocalDeviceExceptionOccurred] callback, you can set the device type to be detected, such as camera, speaker, microphone, etc. Developers can handle the error callbacks of different device types accordingly.

    For related API, please refer to [onLocalDeviceExceptionOccurred](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onLocalDeviceExceptionOccurred.html)

### **Enhancements**

1. Mixed stream output supports advanced encoding parameter configuration.

    Mixed-stream output video configuration [ZegoMixerOutputVideoConfig] Added encodeProfile and encodeLatency parameters, which are used to set the mixed-stream output video encoding specifications and the mixed-stream output video encoding delay respectively.

    For related API, please refer to [ZegoMixerOutputVideoConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMixerOutputVideoConfig-class.html)

2. Added 1015032 error code.

    Logging in to the room causes the network test to stop. As the network test consumes bandwidth, please do it before logging in to the room.

3. Added 1002066 error code.

    If the user is in the server blacklist when logging in to the room, this error code will be returned, indicating that the room is forbidden to log in.

4. Added 1004072 error code.

    When using the SDK to lower the latency of live streaming, this error code will be returned if you have not subscribed to the low latency live streaming service.

### **Bug Fixes**

1. Fixed the problem that the call to [startMixerTask] may crash

### **Deleted**

1. Deprecated [onDeviceError] callback.

    In order to allow developers to intuitively understand the type of abnormal device and the specific abnormal situation, the [onDeviceError] callback is abolished in 2.15.0 and above. Please use the [onLocalDeviceExceptionOccurred] callback instead.

    For related API, please refer to [onDeviceError](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onDeviceError.html), [onLocalDeviceExceptionOccurred](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onLocalDeviceExceptionOccurred.html)

## 2.14.0

### **New Features**

1. Added real time sequential data function.

    When developers need to distribute instructions such as remote control, cloud games, etc., through real-time signaling, they can obtain news from the publisher with low latency.

    For related API, please refer to [createRealTimeSequentialDataManager](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineIM/createRealTimeSequentialDataManager.html)

2. Added H.265 codec exception notification.

    Added a new alarm callback for insufficient H.265 decoding performance, which is used to prompt the user whether to perform downgrade processing in a scenario where the stream is pulled through the CDN. If the developer receives a low frame rate callback [onPlayerLowFpsWarning] during the process of pulling the H.265 stream, it is recommended that the developer stop pulling the H.265 stream and switch to the H.264 stream.

    H.265 codec error notifications have been added to the push stream state callback [onPublisherStateUpdate] and the pull stream state callback [onPlayerStateUpdate].

    For related API, please refer to [onPlayerLowFpsWarning](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerLowFpsWarning.html), [onPublisherStateUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPublisherStateUpdate.html), [onPlayerStateUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onPlayerStateUpdate.html)

3. The media player supports specifying the start playback progress when loading media resources.

    The media player has a new [loadResourceWithPosition] interface, which supports specifying the start playback progress when loading media resources, in milliseconds.

    For related APIs, please refer to [loadResourceWithPosition](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMediaPlayer/loadResourceWithPosition.html)

4. Support setting camera focus and exposure mode.

    The SDK supports setting the camera focus and exposure mode, which is often used in Jianbao live broadcast scenes to zoom in and focus on the details of some objects.

    After starting the local preview, you can call the [isCameraFocusSupported] interface to turn on the camera focus function. Through the [setCameraFocusPointInPreview] and [setCameraExposurePointInPreview] interfaces, you can set the focus and exposure point in the preview view (every time the camera restarts the capture, Both settings will be invalid and need to be reset). Call the [setCameraFocusMode] and [setCameraExposureMode] interfaces to set the camera focus mode and exposure mode respectively.

    For related API, please refer to [isCameraFocusSupported](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/isCameraFocusSupported.html), [setCameraFocusPointInPreview](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setCameraFocusPointInPreview.html), [setCameraFocusMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setCameraFocusMode.html), [setCameraExposureMode](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setCameraExposureMode.html), [setCameraExposurePointInPreview](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/setCameraExposurePointInPreview.html)

5. Added enable or disable pull flow alignment function.

    This function is often used in scenes that require mixed stream alignment, such as KTV. When playing at the streaming end, use the [setPlayStreamsAlignmentProperty] interface to control whether the real-time audio and video streams need to be accurately aligned. If necessary, all the streams that contain precise alignment parameters will be aligned; if not, all streams are not aligned.

    For related API, please refer to [setPlayStreamsAlignmentProperty](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/setPlayStreamsAlignmentProperty.html)

6. Added steady-state voice detection function.

    This function can be used to determine whether someone is speaking into the microphone within a certain period of time, and is used to detect whether the audio data after collection or audio pre-processing is human voice or noise.

    For related API, please refer to [startAudioVADStableStateMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/startAudioVADStableStateMonitor.html), [stopAudioVADStableStateMonitor](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngineDevice/stopAudioVADStableStateMonitor.html), [onAudioVADStateUpdate](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onAudioVADStateUpdate.html)

### **Improved optimization**

1. Nullable parameters are changed to optional parameters

    The nullable parameters of the interface before 2.14.0 need to be passed in null even when used for air conditioning.

    After 2.14.0, the nullable parameters of the interface are changed to optional parameters. When the value is empty, it can not be passed in. When the value is not empty, the parameter name must be brought into the parameter.

    For related API, please refer to [ZegoEngineConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoEngineConfig/ZegoEngineConfig.html)、[ZegoEngineProfile](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoEngineProfile/ZegoEngineProfile.html)、[ZegoPublisherConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoPublisherConfig/ZegoPublisherConfig.html)、[ZegoCanvas](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoCanvas/ZegoCanvas.html)、[ZegoVideoConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoVideoConfig/ZegoVideoConfig.html)、[ZegoPlayerConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoPlayerConfig/ZegoPlayerConfig.html)、[ZegoMixerInput](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMixerInput/ZegoMixerInput.html)

2. Optimize the structure of [ZegoMixerOutput]

    [ZegoMixerOutput]’s videoConfig is changed to an optional parameter, and the structure can not be transmitted when the videoConfig does not need to be configured separately.

    For related API, please refer to [ZegoMixerOutput](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMixerOutput/ZegoMixerOutput.html)

## 2.13.0

### **New Features**

1. Features are aligned with Native SDK 2.13.0 version.

    Support basic push and pull streaming, log in to multiple rooms, room real-time messaging, audio and video live broadcast, 3A processing, CDN direct push retweet, mixed streaming, media player, audio player, game voice, flow control, user control authority and other functions.

    It does not currently support defined video capture, custom video rendering, custom video pre-processing, and audio mixing functions.

### **Deleted**

1. Deprecated the [createEngine] function, please use [createEngineWithProfile] instead.

    In order to reduce the developer's understanding of the environment, the test environment was abandoned and the environment was used uniformly. The original [createEngine] interface has been abandoned in 2.13.0 and above. Please use the same interface without the `isTestEnv` parameter instead.

## 2.8.3

1. Add audio config parameter adaptation window.
2. Upgrade native sdk 2.10.1

## 2.8.2

1. Fix bugs on the window platform.

## 2.8.1

1. Add the parameter channel in audio config interface.

## 2.8.0

1. Add custom audio pre-processing related function.
2. Fix the problem that onCapturedAudioSpectrumUpdate has no callback.

## 2.7.1

1. Fix the bug for iOS that "startPublishingStream" not work after room mode is set to "MultiRoom"

## 2.7.0

1. Add experimental api.
2. Update deprecated api.
3. Upgrade native sdk 2.10.0

## 2.6.0

1. Add support for Windows platform. (Only support audio temporarily)
2. Add support for new multi-room feature.
3. Fix some bugs of mismatched parameters.

## 2.5.3

1. Fix some bugs of mismatched parameters
2. Update Native SDK to 2.9.3

## 2.5.2

1. Fix the bug that the dart impl file gets zego-user error

## 2.5.1

1. Fix a bug that List not convert to Iterable

## 2.5.0

1. BREAKING CHANGES: Add null-safety feature. This requires developers to upgrade Flutter to 2.x.
2. Update Native SDK to 2.8.0

## 2.2.5

1. Fix a crash that may occur when multiple accesses to copyPixelBuffer at the same time

## 2.2.4

1. Fix The adjust texture render size need to rebuild pixel buffer

## 2.2.3

1. Fix android native sdk source

## 2.2.2

1. Fix memory leak in iOS texture rendering
2. Update native sdk to 2.7.0

## 2.2.1

### **Bug Fixes**

1. Fixed the problem that the publish watermark may not be deleted.

    For related API, please refer to [setPublishWatermark](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setPublishWatermark.html)

## 2.2.0

### **New Features**

1. Add RoomSessionID information for audio and video calls.

    Multiple users conduct audio and video communication in the room, and each audio and video communication will have a unique RoomSessionID, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.

2. Added SEI callback for media player.

    Use a media player to play a media file. When the media player parses that the media file contains SEI, it will trigger the [onMediaPlayerRecvSEI] callback.

    For related API, please refer to [onMediaPlayerRecvSEI](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine/onMediaPlayerRecvSEI.html)

3. Support setting advanced configuration when start mixer task.

    add [advancedConfig] into [ZegoMixerTask] to support advance config for mixer task. if you need to use it, please contact ZEGO technical support.

    For related API, please refer to [ZegoMixerTask.advancedConfig](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoMixerTask/advancedConfig.html)

4. Added prism related quality report.

    Added Prism related quality reports to facilitate developers to discover, locate, and solve problems in a timely manner, so as to better and more comprehensively improve user experience.

### **Enhancements**

1. When the unknown error code 100XX99 appears when logging in to the room, publishing stream, or playing stream, the original server code will be thrown in the [extendedData] field, which is convenient for developers to feedback detailed problems.

## 2.1.1

### **Bug Fixes**

1. Fix the problem that the AudioEffectPlayer loads resources and the timing of callback loading results is inaccurate.

2. Fix a crash that may occur when [startPlayingStream], [stopPlayingStream] are invoked repeatedly at very high frequency.

## 2.1.0

### **New Features**

1. Added the function of setting the buffer interval value.

    This function is used to specify the range of adaptive adjustment of the playback buffer, and the developer can set it according to the scene.

    For related API, please refer to [setPlayStreamBufferIntervalRange](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePlayer/setPlayStreamBufferIntervalRange.html)

2. Enhance the function of setting publish watermark, add new adaptation to Flutter assets resources.

    Set the 'imageURL' parameter of the watermark object to the prefix of 'flutter-asset://' and append the relative path of the resource file declared in 'pubspec.yaml'.

    For related API, please refer to [setPublishWatermark](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEnginePublisher/setPublishWatermark.html)

### **Enhancements**

1. Optimization and refactoring of the JNI layer.

### **Bug Fixes**

1. Fix known problems

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
