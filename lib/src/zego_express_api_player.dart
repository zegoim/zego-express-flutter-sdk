
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePlayer on ZegoExpressEngine {

  /// Starts playing a stream from ZEGO's streaming cloud or from third-party CDN.
  ///
  /// This function allows users to play audio and video streams both from the ZEGO real-time audio and video cloud and from third-party cdn.
  /// Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
  /// In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
  /// Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
  /// The developer can update the player canvas by calling this function again (the streamID must be the same).
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  /// - [canvas] The view used to display the play audio and video stream's image. When the view is set to [null], no video is displayed, only audio is played.
  /// - [config] Advanced player configuration
  Future<void> startPlayingStream(String streamID, {ZegoCanvas canvas, ZegoPlayerConfig config}) async {
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

  /// Sets the stream playback volume.
  ///
  /// This function is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
  /// You need to reset after [stopPlayingStream] and [startPlayingStream].
  ///
  /// - [streamID] Stream ID
  /// - [volume] Volume percentage. The value ranges from 0 to 200, and the default value is 100.
  Future<void> setPlayVolume(String streamID, int volume) async {
    return await ZegoExpressImpl.instance.setPlayVolume(streamID, volume);
  }

  /// Stops or resumes playing the audio part of a stream.
  ///
  /// This api can be used to stop playing/retrieving the audio data of the stream. Need to be called after calling startPlayingStream.
  ///
  /// - [streamID] Stream ID
  /// - [mute] mute flag, true: mute play stream video, false: resume play stream video
  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamAudio(streamID, mute);
  }

  /// Stops or resumes playing the video part of a stream.
  ///
  /// This function can be used to stop playing/retrieving the video data of the stream. Need to be called after calling startPlayingStream.
  /// This api is only effective for playing stream from ZEGO real-time audio and video cloud (not ZEGO CDN or third-party CDN).
  ///
  /// - [streamID] Stream ID
  /// - [mute] mute flag, true: mute play stream video, false: resume play stream video
  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    return await ZegoExpressImpl.instance.mutePlayStreamVideo(streamID, mute);
  }

  /// Enables or disables hardware decoding.
  ///
  /// Turn on hardware decoding and use hardware to improve decoding efficiency. Need to be called before calling startPlayingStream.
  /// Because hard-decoded support is not particularly good for a few models, SDK uses software decoding by default. If the developer finds that the device is hot when playing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard decoding.
  ///
  /// - [enable] Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding. The default is false
  Future<void> enableHardwareDecoder(bool enable) async {
    return await ZegoExpressImpl.instance.enableHardwareDecoder(enable);
  }

  /// Enables or disables frame order detection.
  ///
  /// - [enable] Whether to turn on frame order detection, true: enable check poc,not support B frames, false: disable check poc, support B frames but the screen may temporary splash. The default is true
  Future<void> enableCheckPoc(bool enable) async {
    return await ZegoExpressImpl.instance.enableCheckPoc(enable);
  }

}
