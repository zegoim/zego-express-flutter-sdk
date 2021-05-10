
#ifndef __ZEGO_EXPRESS_PLAYER_H__
#define __ZEGO_EXPRESS_PLAYER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Starts playing a stream from ZEGO RTC server.
 *
 * This function allows users to play audio and video streams from the ZEGO RTC server.
 * Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
 * In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
 * Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
 * The developer can update the player canvas by calling this function again (the streamID must be the same).
 * After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min.
 *
 * @param stream_id Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
 * @param canvas The view used to display the play audio and video stream's image. When the view is set to [NULL], no video is displayed, only audio is played.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_playing_stream(const char * stream_id, struct zego_canvas* canvas);


/**
 * Starts playing a stream from ZEGO RTC server or from third-party CDN.
 *
 * This function allows users to play audio and video streams both from the ZEGO RTC server or from third-party cdn.
 * Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
 * In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
 * Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
 * The developer can update the player canvas by calling this function again (the streamID must be the same).
 * After the first play stream failure due to network reasons or the play stream is interrupted, the default time for SDK reconnection is 20min.
 *
 * @param stream_id Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
 * @param canvas The view used to display the play audio and video stream's image. When the view is set to [NULL], no video is displayed, only audio is played.
 * @param config Advanced player configuration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_playing_stream_with_config(const char * stream_id, struct zego_canvas* canvas, struct zego_player_config config);


/**
 * Stops playing a stream.
 *
 * This function allows the user to stop playing the stream. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
 *
 * @param stream_id Stream ID
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_playing_stream(const char * stream_id);


/**
 * Set decryption key for the playing stream.
 *
 * Called before and after [startPlayingStream] can both take effect.
 * Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
 * Support calling this function to update the decryption key while playing stream. Note that developers need to update the player's decryption key before updating the publisher's encryption key.
 * This function is only valid when playing stream from Zego RTC or L3 server.
 *
 * @param stream_id Stream ID
 * @param key The decryption key, note that the key length only supports 16/24/32 bytes.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_stream_decryption_key(const char * stream_id, const char * key);


/**
 * Take a snapshot of the playing stream.
 *
 * Please call this function after calling [startPlayingStream]
 *
 * @param stream_id Stream ID to be snapshot
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_take_play_stream_snapshot(const char * stream_id);


/**
 * Sets the stream playback volume.
 *
 * This function is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
 * You need to reset after [stopPlayingStream] and [startPlayingStream].
 *
 * @param stream_id Stream ID. Set volume for all streams playing by set streamID as NULL or empty.
 * @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_volume(const char * stream_id, int volume);


/**
 * Sets the all stream playback volume.
 *
 * This function is used to set the sound size of all streaming streams, and the local user can control the playback volume of all audio streams.
 *
 * @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_all_play_stream_volume(int volume);


/**
 * Set the selected video layer of playing stream.
 *
 * When the publisher has set the codecID to SVC through [setVideoConfig], the player can dynamically set whether to use the standard layer or the base layer (the resolution of the base layer is one-half of the standard layer)
 * Under normal circumstances, when the network is weak or the rendered UI form is small, you can choose to use the video that plays the base layer to save bandwidth.
 * It can be set before and after playing stream.
 *
 * @deprecated This function has been deprecated since version 2.3.0. Please use [setPlayStreamVideoType] instead.
 * @param stream_id Stream ID.
 * @param video_layer Video layer of playing stream. AUTO by default.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_stream_video_layer(const char * stream_id, enum zego_player_video_layer video_layer);


/**
 * Set play video stream type
 *
 * When the publish stream sets the codecID to SVC through [setVideoConfig], the puller can dynamically set and select different stream types (small resolution is one-half of the standard layer).
 * In general, when the network is weak or the rendered UI window is small, you can choose to pull videos with small resolutions to save bandwidth.
 * It can be set before and after pulling the stream.
 *
 * @param stream_id Stream ID.
 * @param stream_type Video stream type
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_stream_video_type(const char * stream_id, enum zego_video_stream_type stream_type);


/**
 * Set the adaptive adjustment interval range of the buffer for playing stream.
 *
 * When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms.
 * When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
 * It can be set before and after playing stream.
 *
 * @param stream_id Stream ID.
 * @param min_buffer_interval The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms
 * @param max_buffer_interval The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_stream_buffer_interval_range(const char * stream_id, unsigned int min_buffer_interval, unsigned int max_buffer_interval);


/**
 * Set the weight of the pull stream priority.
 *
 * The stream that is set to focus will give priority to ensuring its quality. By default, all streams have the same weight.
 * When the local network is not good, while ensuring the focus flow, it may cause more jams.
 *
 * @param stream_id Stream ID.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_play_stream_focus_on(const char * stream_id);


/**
 * Stops or resumes playing the audio part of a stream.
 *
 * This function can be used to stop playing/retrieving the audio data of the stream. It can be called before and after playing the stream.
 *
 * @param stream_id Stream ID
 * @param mute Mute flag, true: mute play stream audio, false: resume play stream audio
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_play_stream_audio(const char * stream_id, bool mute);


/**
 * Stops or resumes playing the video part of a stream.
 *
 * This function can be used to stop playing/retrieving the video data of the stream. It can be called before and after playing the stream.
 *
 * @param stream_id Stream ID
 * @param mute mute flag, true: mute play stream video, false: resume play stream video
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_play_stream_video(const char * stream_id, bool mute);


/**
 * Stop or resume all pulled audio streams.
 *
 * This function can be called when the stream is pulled, so that the audio data of all remote users is not pulled, and it can be called before and after the stream is pulled.
 * This function does not affect the life cycle of the `mutePlayStreamAudio` interface. This means that neither this function nor the `mutePlayStreamAudio` function must prohibit audio data before audio data can be received.
 *
 * @param mute Mute flag, true: mute play stream audio, false: resume play stream audio
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_all_play_stream_audio(bool mute);


/**
 * Stop or resume pulling all video streams.
 *
 * This function can be called when the stream is pulled, so that the video data of all remote users is not pulled, and it can be called before and after the stream is pulled.
 * This function does not affect the life cycle of the `mutePlayStreamVideo` interface. This means that neither this function nor the `mutePlayStreamVideo` function prohibits video data before receiving video data.
 *
 * @param mute mute flag, true: mute play stream video, false: resume play stream video
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_all_play_stream_video(bool mute);


/**
 * Enables or disables hardware decoding.
 *
 * Turn on hardware decoding and use hardware to improve decoding efficiency. Need to be called before calling startPlayingStream.
 * Because hard-decoded support is not particularly good for a few models, SDK uses software decoding by default. If the developer finds that the device is hot when playing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard decoding.
 *
 * @param enable Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding. The default is false
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_hardware_decoder(bool enable);


/**
 * Enables or disables frame order detection.
 *
 * @param enable Whether to turn on frame order detection, true: enable check poc,not support B frames, false: disable check poc, support B frames but the screen may temporary splash. The default is true
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_check_poc(bool enable);


/**
 * The callback triggered when the state of stream playing changes.
 *
 * After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
 * You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
 *
 * @param stream_id stream ID
 * @param state State of playing stream
 * @param error_code The error code corresponding to the status change of the playing stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param extended_data Extended Information with state updates. As the standby, only an empty json table is currently returned
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_state_update)(const char * stream_id, enum zego_player_state state, zego_error error_code, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_state_update_callback(zego_on_player_state_update callback_func, void * user_context);


/**
 * Callback for current stream playing quality.
 *
 * After calling the [startPlayingStream] successfully, this callback will be triggered every 3 seconds. The collection frame rate, bit rate, RTT, packet loss rate and other quality data can be obtained, such the health of the publish stream can be monitored in real time.
 * You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback function, in order to show the downlink network status on the device UI in real time.
 * If you does not know how to use the various parameters of the callback function, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
 *
 * @param stream_id Stream ID
 * @param quality Playing stream quality, including audio and video framerate, bitrate, RTT, etc.
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_quality_update)(const char * stream_id, struct zego_play_stream_quality quality, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_quality_update_callback(zego_on_player_quality_update callback_func, void * user_context);


/**
 * The callback triggered when a media event occurs during streaming playing.
 *
 * This callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
 * You can use this callback to make statistics on stutters or to make friendly displays in the UI of the app.
 *
 * @param stream_id Stream ID
 * @param event Specific events received when playing the stream.
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_media_event)(const char * stream_id, enum zego_player_media_event event, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_media_event_callback(zego_on_player_media_event callback_func, void * user_context);


/**
 * The callback triggered when the first audio frame is received.
 *
 * After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
 *
 * @param stream_id Stream ID
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_recv_audio_first_frame)(const char * stream_id, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_recv_audio_first_frame_callback(zego_on_player_recv_audio_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the first video frame is received.
 *
 * After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of video data.
 *
 * @param stream_id Stream ID
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_recv_video_first_frame)(const char * stream_id, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_recv_video_first_frame_callback(zego_on_player_recv_video_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the first video frame is rendered.
 *
 * After the [startPlayingStream] function is called successfully, this callback will be called when SDK rendered the first frame of video data.
 * Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
 *
 * @param stream_id Stream ID
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_render_video_first_frame)(const char * stream_id, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_render_video_first_frame_callback(zego_on_player_render_video_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the stream playback resolution changes.
 *
 * If there is a change in the video resolution of the playing stream, the callback will be triggered, and the user can adjust the display for that stream dynamically.
 * If the publishing stream end triggers the internal stream flow control of SDK due to a network problem, the encoding resolution of the streaming end may be dynamically reduced, and this callback will also be received at this time.
 * If the stream is only audio data, the callback will not be received.
 * This callback will be triggered when the played audio and video stream is actually rendered to the set UI play canvas. You can use this callback notification to update or switch UI components that actually play the stream.
 *
 * @param stream_id Stream ID
 * @param width Video decoding resolution width
 * @param height Video decoding resolution height
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_video_size_changed)(const char * stream_id, int width, int height, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_video_size_changed_callback(zego_on_player_video_size_changed callback_func, void * user_context);


/**
 * The callback triggered when Supplemental Enhancement Information is received.
 *
 * After the remote stream is successfully played, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
 * Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
 *
 * @param stream_id Stream ID
 * @param data SEI content
 * @param data_length SEI content length.
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_recv_sei)(const char * stream_id, const unsigned char* data, unsigned int data_length, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_recv_sei_callback(zego_on_player_recv_sei callback_func, void * user_context);


/**
 * Results of take play stream snapshot.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param stream_id Stream ID
 * @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
 * @param user_context Context of user.
 */
typedef void(*zego_on_player_take_snapshot_result)(zego_error error_code, const char * stream_id, void * image, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_player_take_snapshot_result_callback(zego_on_player_take_snapshot_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
