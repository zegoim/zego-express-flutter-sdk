
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
  /// Called before and after [startPlayingStream] can both take effect.
  /// Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
  /// Support calling this function to update the decryption key while playing stream. Note that developers need to update the player's decryption key before updating the publisher's encryption key.
  /// This function is only valid when playing stream from Zego RTC or L3 server.
  ///
  /// - [streamID] Stream ID
  /// - [key] The decryption key, note that the key length only supports 16/24/32 bytes.
  Future<void> setPlayStreamDecryptionKey(String streamID, String key) async {
    return await ZegoExpressImpl.instance.setPlayStreamDecryptionKey(streamID, key);
  }

  /// Take a snapshot of the playing stream.
  ///
  /// Please call this function after calling [startPlayingStream]
  ///
  /// - [streamID] Stream ID to be snapshot
  /// - Returns Results of take play stream snapshot
  Future<ZegoPlayerTakeSnapshotResult> takePlayStreamSnapshot(String streamID) async {
    return await ZegoExpressImpl.instance.takePlayStreamSnapshot(streamID);
  }

  /// Sets the stream playback volume.
  ///
  /// This function is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
  /// You need to reset after [stopPlayingStream] and [startPlayingStream].
  ///
  /// - [streamID] Stream ID.
  /// - [volume] Volume percentage. The value ranges from 0 to 200, and the default value is 100.
  Future<void> setPlayVolume(String streamID, int volume) async {
    return await ZegoExpressImpl.instance.setPlayVolume(streamID, volume);
  }

  /// Set play video stream type
  ///
  /// When the publish stream sets the codecID to SVC through [setVideoConfig], the puller can dynamically set and select different stream types (small resolution is one-half of the standard layer).
  /// In general, when the network is weak or the rendered UI window is small, you can choose to pull videos with small resolutions to save bandwidth.
  /// It can be set before and after pulling the stream.
  ///
  /// - [streamID] Stream ID.
  /// - [streamType] Video stream type
  Future<void> setPlayStreamVideoType(String streamID, ZegoVideoStreamType streamType) async {
    return await ZegoExpressImpl.instance.setPlayStreamVideoType(streamID, streamType);
  }

  /// Set the adaptive adjustment interval range of the buffer for playing stream.
  ///
  /// When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms.
  /// When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
  /// It can be set before and after playing stream.
  ///
  /// - [streamID] Stream ID.
  /// - [minBufferInterval] The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms
  /// - [maxBufferInterval] The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms
  Future<void> setPlayStreamBufferIntervalRange(String streamID, int minBufferInterval, int maxBufferInterval) async {
    return await ZegoExpressImpl.instance.setPlayStreamBufferIntervalRange(streamID, minBufferInterval, maxBufferInterval);
  }

  /// Whether the pull stream can receive the specified audio data.
  ///
  /// Available since: 1.0.0 and above.
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed.
  /// When to call: This function can be called before and after calling [startPlayingStream] to pull the stream.
  /// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `false`.
  /// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. When the two functions [muteAllPlayStreamAudio] and [mutePlayStreamAudio] are set to `false` at the same time, the local user can receive the audio data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamAudio(true)] function is called, it is globally effective, that is, local users will be prohibited from receiving all remote users' audio data. At this time, the [mutePlayStreamAudio] function will not take effect whether it is called before or after [muteAllPlayStreamAudio].2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [mutePlayStreamAudio] function can be used to control whether to receive a single audio data. Calling the [mutePlayStreamAudio(true, streamID)] function allows the local user to receive audio data other than the `streamID`; calling the [mutePlayStreamAudio(false, streamID)] function allows the local user to receive all audio data.
  ///
  /// - [streamID] Stream ID
  /// - [mute] Whether it can receive the audio data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false".
  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamAudio(streamID, mute);
  }

  /// Whether the pull stream can receive the specified video data.
  ///
  /// Available since: 1.0.0 and above.
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive video data from designated remote users when pulling streams as needed.
  /// When to call: This function can be called before and after calling [startPlayingStream] to pull the stream.
  /// Caution: This function is valid only when the [muteAllPlayStreamVideo] function is set to `false`.
  /// Related APIs: You can call the [muteAllPlayStreamVideo] function to control whether to receive all video data. When the two functions [muteAllPlayStreamVideo] and [mutePlayStreamVideo] are set to `false` at the same time, the local user can receive the video data of the remote user when the stream is pulled: 1. When the [muteAllPlayStreamVideo(true)] function is called, it will take effect globally, that is, local users will be prohibited from receiving all remote users' video data. At this time, the [mutePlayStreamVideo] function will not take effect whether it is called before or after [muteAllPlayStreamVideo]. 2. When the [muteAllPlayStreamVideo(false)] function is called, the local user can receive the video data of all remote users. At this time, the [mutePlayStreamVideo] function can be used to control whether to receive a single video data. Call the [mutePlayStreamVideo(true, streamID)] function, the local user can receive other video data other than the `streamID`; call the [mutePlayStreamVideo(false, streamID)] function, the local user can receive all the video data.
  ///
  /// - [streamID] Stream ID
  /// - [mute] Whether it is possible to receive the video data of the specified remote user when streaming, "true" means prohibition, "false" means receiving, the default value is "false".
  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamVideo(streamID, mute);
  }

  /// Enables or disables hardware decoding.
  ///
  /// Turn on hardware decoding and use hardware to improve decoding efficiency. Need to be called before calling startPlayingStream.
  /// Because hard-decoded support is not particularly good for a few models, SDK uses software decoding by default. If the developer finds that the device is hot when playing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard decoding.
  /// This function needs to be called after [createEngine] creates an instance.
  ///
  /// - [enable] Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding. The default is false
  Future<void> enableHardwareDecoder(bool enable) async {
    return await ZegoExpressImpl.instance.enableHardwareDecoder(enable);
  }

  /// Enables or disables frame order detection.
  ///
  /// This function needs to be called after [createEngine] creates an instance.
  ///
  /// - [enable] Whether to turn on frame order detection, true: enable check poc,not support B frames, false: disable check poc, support B frames but the screen may temporary splash. The default is true
  Future<void> enableCheckPoc(bool enable) async {
    return await ZegoExpressImpl.instance.enableCheckPoc(enable);
  }

}
