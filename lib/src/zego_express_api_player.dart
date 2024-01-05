import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEnginePlayer on ZegoExpressEngine {
  /// Starts playing a stream from ZEGO RTC server or from third-party CDN. Support multi-room mode.
  ///
  /// Available since: 1.1.0
  /// Description: Play audio and video streams from the ZEGO RTC server or CDN.
  /// Use cases: In real-time or live broadcast scenarios, developers can listen to the [onRoomStreamUpdate] event callback to obtain the new stream information in the room where they are located, and call this interface to pass in streamID for play streams.
  /// When to call: After [loginRoom].
  /// Restrictions: None.
  /// Caution: 1. The developer can update the player canvas by calling this function again (the streamID must be the same). 2. After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min. 3. In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event. please refer to https://docs.zegocloud.com/faq/reconnect. 4. Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters.
  ///   Caution:
  ///   Only support numbers, English characters and '-', '_'.
  /// - [canvas] The view used to display the play audio and video stream's image. When the view is set to [null], no video is displayed, only audio is played.
  /// - [config] Advanced player configuration Room [roomID] in [ZegoPlayerConfig] is the login roomID.
  Future<void> startPlayingStream(String streamID,
      {ZegoCanvas? canvas, ZegoPlayerConfig? config}) async {
    return await ZegoExpressImpl.instance
        .startPlayingStream(streamID, canvas: canvas, config: config);
  }

  /// Stops playing a stream.
  ///
  /// Available since: 1.1.0
  /// Description: Play audio and video streams from the ZEGO RTC server.
  /// Use cases: In the real-time scenario, developers can listen to the [onRoomStreamUpdate] event callback to obtain the delete stream information in the room where they are located, and call this interface to pass in streamID for stop play streams.
  /// When to call: After [loginRoom].
  /// Restrictions: None.
  /// Caution: When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
  ///
  /// - [streamID] Stream ID.
  Future<void> stopPlayingStream(String streamID) async {
    return await ZegoExpressImpl.instance.stopPlayingStream(streamID);
  }

  /// Set decryption key for the playing stream.
  ///
  /// Available since: 1.19.0
  /// Description: When streaming, the audio and video data will be decrypted according to the set key.
  /// Use cases: Usually used in scenarios that require high security for audio and video calls.
  /// When to call: after [createEngine], after the play stream can be changed at any time.
  /// Restrictions: This function is only valid when calling from Zego RTC or L3 server.
  /// Related APIs: [setPublishStreamEncryptionKey]Set the publish stream encryption key.
  /// Caution: This interface can only be called if encryption is set on the publish. Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
  ///
  /// - [streamID] Stream ID.
  /// - [key] The decryption key, note that the key length only supports 16/24/32 bytes.
  Future<void> setPlayStreamDecryptionKey(String streamID, String key) async {
    return await ZegoExpressImpl.instance
        .setPlayStreamDecryptionKey(streamID, key);
  }

  /// Set up cross App playing stream information.
  ///
  /// Available since: 2.19.0
  /// Description: This information is used for authentication before playing a stream or when retrying playing a stream.
  /// Use cases: Used in scenarios that playing streams across apps.
  /// When to call: after [createEngine], after the play stream can be changed at any time.
  /// Restrictions: This function is only valid when playing stream from Zego RTC server.
  /// Caution: Calling [stopPlayingStream] or [logoutRoom] will clear this information.
  ///
  /// - [streamID] Stream ID.
  /// - [info] Information for cross App playing stream.
  Future<void> setPlayStreamCrossAppInfo(
      String streamID, ZegoCrossAppInfo info) async {
    return await ZegoExpressImpl.instance
        .setPlayStreamCrossAppInfo(streamID, info);
  }

  /// Take a snapshot of the playing stream.
  ///
  /// Available since: 1.17.0
  /// Description: Take a screenshot of the specified stream ID.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Related callbacks: [onPlayerTakeSnapshotResult] Screenshot data callback.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID to be snapshot.
  /// - Returns Results of take play stream snapshot.
  Future<ZegoPlayerTakeSnapshotResult> takePlayStreamSnapshot(
      String streamID) async {
    return await ZegoExpressImpl.instance.takePlayStreamSnapshot(streamID);
  }

  /// Sets the stream playback volume.
  ///
  /// Available since: 1.16.0
  /// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Related APIs: [setAllPlayStreamVolume] Set all stream volume.
  /// Caution: You need to reset after [stopPlayingStream] and [startPlayingStream]. This function and the [setAllPlayStreamVolume] function overwrite each other, and the last call takes effect.
  ///
  /// - [streamID] Stream ID.
  /// - [volume] Volume percentage. The value ranges from 0 to 200, and the default value is 100.
  Future<void> setPlayVolume(String streamID, int volume) async {
    return await ZegoExpressImpl.instance.setPlayVolume(streamID, volume);
  }

  /// Sets the all stream playback volume.
  ///
  /// Available since: 2.3.0
  /// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Related APIs: [setPlayVolume] Set the specified streaming volume.
  /// Caution: Set the specified streaming volume and [setAllPlayStreamVolume] interface to override each other, and the last call takes effect.
  ///
  /// - [volume] Volume percentage. The value ranges from 0 to 200, and the default value is 100.
  Future<void> setAllPlayStreamVolume(int volume) async {
    return await ZegoExpressImpl.instance.setAllPlayStreamVolume(volume);
  }

  /// Set play video stream type.
  ///
  /// Available since: 2.3.0
  /// Description: When the publish stream sets the codecID to SVC through [setVideoConfig], the puller can dynamically set and select different stream types (small resolution is one-half of the standard layer).
  /// Use cases: In general, when the network is weak or the rendered UI window is small, you can choose to pull videos with small resolutions to save bandwidth.
  /// When to call: before or after called [startPlayingStream].
  /// Restrictions: None.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  /// - [streamType] Video stream type.
  Future<void> setPlayStreamVideoType(
      String streamID, ZegoVideoStreamType streamType) async {
    return await ZegoExpressImpl.instance
        .setPlayStreamVideoType(streamID, streamType);
  }

  /// Set the adaptive adjustment interval range of the buffer for playing stream.
  ///
  /// Available since: 2.1.0
  /// Description: Set the range of adaptive adjustment of the internal buffer of the sdk when streaming is 0-4000ms.
  /// Use cases: Generally, in the case of a poor network environment, adjusting and increasing the playback buffer of the pull stream will significantly reduce the audio and video freezes, but will increase the delay.
  /// When to call: after called [createEngine].
  /// Restrictions: None.
  /// Caution: When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms. When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
  ///
  /// - [streamID] Stream ID.
  /// - [minBufferInterval] The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms.
  /// - [maxBufferInterval] The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms.
  Future<void> setPlayStreamBufferIntervalRange(
      String streamID, int minBufferInterval, int maxBufferInterval) async {
    return await ZegoExpressImpl.instance.setPlayStreamBufferIntervalRange(
        streamID, minBufferInterval, maxBufferInterval);
  }

  /// Set the weight of the pull stream priority.
  ///
  /// Available since: 1.1.0
  /// Description: Set the weight of the streaming priority.
  /// Use cases: This interface can be used when developers need to prioritize the quality of a audio and video stream in business（do not use in pure audio case）. For example: in class scene, if students pull multiple streams, you can set high priority for teacher stream.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Caution: By default, all streams have the same weight. Only one stream can be set with high priority, whichever is set last. After the flow is stopped, the initial state is automatically restored, and all flows have the same weight.When the local network is not good, while ensuring the focus flow, other stalls may be caused more.
  ///
  /// - [streamID] Stream ID.
  Future<void> setPlayStreamFocusOn(String streamID) async {
    return await ZegoExpressImpl.instance.setPlayStreamFocusOn(streamID);
  }

  /// Whether the pull stream can receive the specified audio data.
  ///
  /// Available since: 1.1.0
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
  /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution:
  ///  1. When used together with [muteAllPlayAudioStreams], they can override each other's configurations.
  ///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamAudio] function is set to `false`.
  ///  3. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
  /// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it is globally effective, that is, local users will be prohibited from receiving all remote users' audio data. At this time, the [mutePlayStreamAudio] function will not take effect whether it is called before or after [muteAllPlayStreamAudio].2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
  ///
  /// - [streamID] Stream ID.
  /// - [mute] Whether it can receive the audio data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false".
  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamAudio(streamID, mute);
  }

  /// Whether the pull stream can receive the specified video data.
  ///
  /// Available since: 1.1.0
  /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive video data from designated remote users when pulling streams as needed. When the developer does not receive the video data, the hardware and network overhead can be reduced.
  /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution:
  ///  1. When used together with [muteAllPlayVideoStreams], they can override each other's configurations.
  ///  2. When used together with [muteAllPlayStreamAudio], this function only works when the [muteAllPlayStreamVideo] function is set to `false`.
  ///  3. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
  ///  4. After stopping streaming, any properties previously set for this stream such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo] and other streaming-related configurations will become invalid and need to be reset before the next stream is pulled.
  /// Related APIs: You can call the [muteAllPlayStreamVideo] function to control whether to receive all video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, local users will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  /// - [mute] Whether it is possible to receive the video data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false". The default value for automatically played streams within the SDK is false.
  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamVideo(streamID, mute);
  }

  /// Can the pull stream receive all audio data.
  ///
  /// Available since: 2.4.0
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
  /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution: This function cannot be used together with [muteAllPlayAudioStreams] throughout the SDK lifecycle.
  /// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
  ///
  /// - [mute] Whether it is possible to receive audio data from all remote users when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
  Future<void> muteAllPlayStreamAudio(bool mute) async {
    return await ZegoExpressImpl.instance.muteAllPlayStreamAudio(mute);
  }

  /// Can the pull stream receive all audio data.
  ///
  /// Available since: 3.10.0
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from all remote users when pulling streams (including the audio streams pushed by users who have newly joined the room after calling this function). By default, users can receive audio data pushed by all remote users after joining the room. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
  /// Use cases: Call this function when developers need to quickly close and restore remote audio. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution: This function cannot be used together with [muteAllPlayStreamAudio] throughout the SDK lifecycle.
  /// Related APIs: You can call the [mutePlayStreamAudio] function to control whether to receive a single piece of audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [mutePlayStreamAudio] function will not take effect no matter if the [mutePlayStreamAudio] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
  ///
  /// - [mute] Whether it is possible to receive audio data from all remote users when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
  Future<void> muteAllPlayAudioStreams(bool mute) async {
    return await ZegoExpressImpl.instance.muteAllPlayAudioStreams(mute);
  }

  /// Can the pull stream receive all video data.
  ///
  /// Available since: 2.4.0
  /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
  /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution:
  ///  1. This function cannot be used together with [muteAllPlayVideoStreams] throughout the SDK lifecycle.
  ///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
  /// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, the local user will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [mute] Whether it is possible to receive all remote users' video data when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
  Future<void> muteAllPlayStreamVideo(bool mute) async {
    return await ZegoExpressImpl.instance.muteAllPlayStreamVideo(mute);
  }

  /// Can the pull stream receive all video data.
  ///
  /// Available since: 3.10.0
  /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive all remote users' video data when pulling the stream (including the video stream pushed by the new user who joins the room after calling this function). By default, users can receive video data pushed by all remote users after joining the room. When the developer does not receive the video data, the hardware and network overhead can be reduced.
  /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution:
  ///  1. This function cannot be used together with [muteAllPlayStreamVideo] throughout the SDK lifecycle.
  ///  2. When you mute the video stream, the view remains at the last frame by default, if you need to clear the last frame, please contact ZEGO technical support.
  /// Related APIs: You can call the [mutePlayStreamVideo] function to control whether to receive a single piece of video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [mute] Whether it is possible to receive all remote users' video data when streaming, "true" means prohibition, "false" means receiving, and the default value is "false".
  Future<void> muteAllPlayVideoStreams(bool mute) async {
    return await ZegoExpressImpl.instance.muteAllPlayVideoStreams(mute);
  }

  /// Enables or disables hardware decoding.
  ///
  /// Available since: 1.1.0
  /// Description: Control whether hardware decoding is used when playing streams, with hardware decoding enabled the SDK will use the GPU for decoding, reducing CPU usage.
  /// Use cases: If developers find that the device heats up badly when playing large resolution audio and video streams during testing on some models, consider calling this function to enable hardware decoding.
  /// Default value: Hardware decoding is disabled by default when this interface is not called.
  /// When to call: This function needs to be called after [createEngine] creates an instance.
  /// Restrictions: None.
  /// Caution: Need to be called before calling [startPlayingStream], if called after playing the stream, it will only take effect after stopping the stream and re-playing it. Once this configuration has taken effect, it will remain in force until the next call takes effect.
  ///
  /// - [enable] Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding.
  Future<void> enableHardwareDecoder(bool enable) async {
    return await ZegoExpressImpl.instance.enableHardwareDecoder(enable);
  }

  /// Enables or disables frame order detection.
  ///
  /// Available since: 1.1.0
  /// Description: Control whether to turn on frame order detection.
  /// Use cases: Turning on frame order detection when pulling cdn's stream will prevent splash screens.
  /// Default value: Turn on frame order detection by default when this interface is not called.
  /// When to call: This function needs to be called after [createEngine] creates an instance.
  /// Restrictions: None.
  /// Caution: Turn off frame order detection during playing stream may result in a brief splash screen.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [enable] Whether to turn on frame order detection, true: enable check poc, false: disable check poc.
  Future<void> enableCheckPoc(bool enable) async {
    return await ZegoExpressImpl.instance.enableCheckPoc(enable);
  }

  /// Whether the specified video decoding format is supported.
  ///
  /// Available since: 3.0.0
  /// Description: Whether the specified video decoding is supported depends on the following aspects: whether the hardware model supports hard decoding, whether the performance of the hardware model supports soft decoding, and whether the SDK includes the decoding module.
  /// When to call: After creating the engine.
  /// Caution: It is recommended that users call this interface to obtain the H.265 decoding support capability before pulling the H.265 stream. If it is not supported, the user can pull the stream of other encoding formats, such as H.264.
  ///
  /// - [codecID] Video codec id. Required: Yes.
  /// - [codecBackend] Backend implementation of decoder. Required: Yes.
  /// - Returns Whether the specified video decoding format is supported; 0 means not supported, and the decoding format cannot be used for play stream; 1 means support, you can use this decoding format for playing stream; 2 means not confirmed, it is recommended to call this interface later.
  Future<int> isVideoDecoderSupported(ZegoVideoCodecID codecID,
      {ZegoVideoCodecBackend? codecBackend}) async {
    return await ZegoExpressImpl.instance
        .isVideoDecoderSupported(codecID, codecBackend: codecBackend);
  }

  /// Set the play stream alignment properties.
  ///
  /// Available since: 2.14.0
  /// Description: When playing at the streaming end, control whether the playing RTC stream needs to be accurately aligned. If necessary, all the streams that contain precise alignment parameters will be aligned; if not, all streams are not aligned.
  /// Use case: It is often used in scenes that require mixed stream alignment such as KTV to ensure that users can switch between singing anchors, ordinary Maishangyu chat anchors, and Maixia audiences at any time during use.
  /// Default value: If this interface is not called, the default is not aligned.
  /// When to call: Called after [createEngine]. Call the interface repeatedly, and the latest setting is valid.
  /// Related APIs: Set the precise alignment parameter of the stream channel [setStreamAlignmentProperty].
  ///
  /// - [mode] Setting the stream alignment mode.
  Future<void> setPlayStreamsAlignmentProperty(
      ZegoStreamAlignmentMode mode) async {
    return await ZegoExpressImpl.instance.setPlayStreamsAlignmentProperty(mode);
  }

  /// Enable video super resolution.
  ///
  /// Available since: 3.0.0
  /// Description: Whether to enable video super resolution when playing stream, the resolution of the played video can be doubled at the stream playing end through video super resolution. For example, the original resolution is 640x360, and the super-resolution is 1280x720.
  /// Use cases: Live streaming scenario.
  /// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [initVideoSuperResolution].
  /// Caution:
  ///  1. This function requires a special package, please contact ZEGO technical support;
  ///  2. This function will consume extra system resources. In order to ensure user experience, ZEGO can only enable video super resolution for one stream, and the original video resolution is not recommended to exceed 640 × 360.
  /// Related callbacks: Developer can use the [onPlayerVideoSuperResolutionUpdate] callback to monitor the video super resolution status change.
  ///
  /// - [streamID] The ID of the stream that currently needs to turn on or off overscore.
  /// - [enable] Whether to enable super resolution, it is not enabled by default.
  Future<void> enableVideoSuperResolution(String streamID, bool enable) async {
    return await ZegoExpressImpl.instance
        .enableVideoSuperResolution(streamID, enable);
  }

  /// Init video super resolution.
  ///
  /// Available since: 3.3.0
  /// Description: After the super resolution is initialized, the super resolution function can be used normally
  /// Use cases: Live streaming scenario.
  /// When to call: Video super resolution is only valid for playing stream video. Needs to be called after [createEngine].
  /// Caution:
  ///  1. Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended;
  ///  2. This function requires a special package, please contact ZEGO technical support.
  Future<void> initVideoSuperResolution() async {
    return await ZegoExpressImpl.instance.initVideoSuperResolution();
  }

  /// Uninit video super resolution.
  ///
  /// Available since: 3.3.0
  /// Description: After deinitializing the super resolution, the SDK will release the resources occupied by the super resolution and make the super resolution function unavailable.
  /// Use cases: Live streaming scenario.
  /// When to call: Needs to be called after [initVideoSuperResolution].
  /// Caution: Initializing the video screen is divided into time-consuming operations, and frequent initialization and de-initialization are not recommended.
  Future<void> uninitVideoSuperResolution() async {
    return await ZegoExpressImpl.instance.uninitVideoSuperResolution();
  }

  /// Update playing canvas.
  ///
  /// Available: since 3.4.0
  /// Description: This interface will update playing view.
  /// Use case: The user can call this function to update canvas display video.
  /// When to call: After receiving a successful playing stream from the [onPlayerStateUpdate] or [onUserStreamStateUpdate] callback.
  /// Restrictions: None.
  /// Caution: None.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters.
  ///   Caution:
  ///   Only support numbers, English characters and '-', '_'.
  /// - [canvas] The view used to display the play audio and video stream's image. When the view is set to [null], no video is displayed, only audio is played.
  /// - Returns Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  Future<int> updatePlayingCanvas(String streamID, ZegoCanvas canvas) async {
    return await ZegoExpressImpl.instance.updatePlayingCanvas(streamID, canvas);
  }
}
