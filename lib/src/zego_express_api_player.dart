
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePlayer on ZegoExpressEngine {

  /// Starts playing a stream from ZEGO RTC server or from third-party CDN. Support multi-room mode.
  ///
  /// This function allows users to play audio and video streams both from the ZEGO RTC server or from third-party cdn.
  /// Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
  /// In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
  /// Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
  /// The developer can update the player canvas by calling this function again (the streamID must be the same).
  /// After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min.
  /// To call [SetRoomMode] function to select multiple rooms, the room ID must be specified explicitly.
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  /// - [canvas] The view used to display the play audio and video stream's image. When the view is set to [null], no video is displayed, only audio is played.
  /// - [config] Advanced player configuration
  Future<void> startPlayingStream(String streamID, {ZegoCanvas? canvas, ZegoPlayerConfig? config}) async {
    return await ZegoExpressImpl.instance.startPlayingStream(streamID, canvas: canvas, config: config);
  }

  /// Stops playing a stream.
  ///
  /// This function allows the user to stop playing the stream. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
  ///
  /// - [streamID] Stream ID
  Future<void> stopPlayingStream(String streamID) async {
    return await ZegoExpressImpl.instance.stopPlayingStream(streamID);
  }

  /// Set decryption key for the playing stream.
  ///
  /// Available since: 1.19.0
  /// Description: When streaming, the audio and video data will be decrypted according to the set key.
  /// Use cases: Usually used in scenarios that require high security for audio and video calls.
  /// When to call: after [createEngine], before called [startPlayingStream] or after called [startPlayingStream].
  /// Restrictions: This function is only valid when calling from Zego RTC or L3 server.
  /// Related APIs: [setPublishStreamEncryptionKey]Set the publish stream encryption key.
  /// Caution: This interface can only be called if encryption is set on the publish. Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
  ///
  /// - [streamID] Stream ID.
  /// - [key] The decryption key, note that the key length only supports 16/24/32 bytes.
  Future<void> setPlayStreamDecryptionKey(String streamID, String key) async {
    return await ZegoExpressImpl.instance.setPlayStreamDecryptionKey(streamID, key);
  }

  /// Take a snapshot of the playing stream.
  ///
  /// Available since: 1.17.0
  /// Description: Take a screenshot of the specified stream ID.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Related callbacks: [onPlayerTakeSnapshotResult] Screenshot data callback.
  ///
  /// - [streamID] Stream ID to be snapshot.
  /// - Returns Results of take play stream snapshot.
  Future<ZegoPlayerTakeSnapshotResult> takePlayStreamSnapshot(String streamID) async {
    return await ZegoExpressImpl.instance.takePlayStreamSnapshot(streamID);
  }

  /// Sets the stream playback volume.
  ///
  /// Available since: 1.16.0
  /// Description: Set the sound size of the stream, the local user can control the playback volume of the audio stream.
  /// When to call: after called [startPlayingStream].
  /// Restrictions: None.
  /// Related APIs: [setAllPlayStreamVolume]Set all stream volume.
  /// Caution: You need to reset after [stopPlayingStream] and [startPlayingStream].
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
  /// Caution: You need to reset after [stopPlayingStream] and [startPlayingStream]. Set the specified streaming volume and [setAllPlayStreamVolume] interface to override each other, and the last call takes effect.
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
  ///
  /// - [streamID] Stream ID.
  /// - [streamType] Video stream type.
  Future<void> setPlayStreamVideoType(String streamID, ZegoVideoStreamType streamType) async {
    return await ZegoExpressImpl.instance.setPlayStreamVideoType(streamID, streamType);
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
  Future<void> setPlayStreamBufferIntervalRange(String streamID, int minBufferInterval, int maxBufferInterval) async {
    return await ZegoExpressImpl.instance.setPlayStreamBufferIntervalRange(streamID, minBufferInterval, maxBufferInterval);
  }

  /// Set the weight of the pull stream priority.
  ///
  /// Available since: 1.1.0
  /// Description: Set the weight of the streaming priority.
  /// Use cases: This interface can be used when developers need to prioritize the quality of a stream in business. For example: in class scene, if students pull multiple streams, you can set high priority for teacher stream.
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
  /// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `false`.
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
  /// Description: In the process of real-time video and video interaction, local users can use this function to control whether to receive video data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
  /// Use cases: This function can be called when developers need to quickly close and resume watching remote video. Compared to re-flow, it can greatly reduce the time and improve the interactive experience.
  /// When to call: This function can be called after calling [createEngine].
  /// Caution: This function is valid only when the [muteAllPlayStreamVideo] function is set to `false`.
  /// Related APIs: You can call the [muteAllPlayStreamVideo] function to control whether to receive all video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, local users will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
  ///
  /// - [streamID] Stream ID.
  /// - [mute] Whether it is possible to receive the video data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false".
  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamVideo(streamID, mute);
  }

  /// Enables or disables hardware decoding.
  ///
  /// Available since: 1.1.0
  /// Description: Control whether hardware decoding is used when playing streams, with hardware decoding enabled the SDK will use the GPU for decoding, reducing CPU usage.
  /// Use cases: If developers find that the device heats up badly when playing large resolution audio and video streams during testing on some models, consider calling this function to enable hardware decoding.
  /// Default value: Hardware decoding is disabled by default when this interface is not called.
  /// When to call: This function needs to be called after [createEngine] creates an instance.
  /// Restrictions: None.
  /// Caution: Need to be called before calling [startPlayingStream], if called after playing the stream, it will only take effect after stopping the stream and re-playing it.
  ///
  /// - [enable] Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding.
  Future<void> enableHardwareDecoder(bool enable) async {
    return await ZegoExpressImpl.instance.enableHardwareDecoder(enable);
  }

  /// Enables or disables frame order detection.
  ///
  /// Available since: 1.1.0
  /// Description: Control whether to turn on frame order detection, on to not support B frames, off to support B frames.
  /// Use cases: Prevent splash screen when playing cdn's stream.
  /// Default value: Turn on frame order detection by default when this interface is not called.
  /// When to call: This function needs to be called after [createEngine] creates an instance.
  /// Restrictions: None.
  /// Caution: Turn on frame order detection may result in a brief splash screen.
  ///
  /// - [enable] Whether to turn on frame order detection, true: enable check poc,not support B frames, false: disable check poc, support B frames.
  Future<void> enableCheckPoc(bool enable) async {
    return await ZegoExpressImpl.instance.enableCheckPoc(enable);
  }

}

extension ZegoExpressEngineDeprecatedApi on ZegoExpressEngine {

  /// [Deprecated] Set the selected video layer of playing stream.
  ///
  /// Available: 1.19.0 to 2.3.0, deprecated.
  /// This function has been deprecated since version 2.3.0, Please use [setPlayStreamVideoType] instead.
  /// When the publisher has set the codecID to SVC through [setVideoConfig], the player can dynamically set whether to use the standard layer or the base layer (the resolution of the base layer is one-half of the standard layer)
  /// Under normal circumstances, when the network is weak or the rendered UI form is small, you can choose to use the video that plays the base layer to save bandwidth.
  /// It can be set before and after playing stream.
  ///
  /// @deprecated This function has been deprecated since version 2.3.0, Please use [setPlayStreamVideoType] instead.
  /// - [streamID] Stream ID.
  /// - [videoLayer] Video layer of playing stream. AUTO by default.
  @Deprecated('This function has been deprecated since version 2.3.0, Please use [setPlayStreamVideoType] instead.')
  Future<void> setPlayStreamVideoLayer(String streamID, ZegoPlayerVideoLayer videoLayer) async {
    return await ZegoExpressImpl.instance.setPlayStreamVideoLayer(streamID, videoLayer);
  }

}
