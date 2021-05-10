
#ifndef __ZEGO_EXPRESS_PUBLISHER_H__
#define __ZEGO_EXPRESS_PUBLISHER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Starts publishing a stream (for the specified channel). You can call this function to publish a second stream.
 *
 * This function allows users to publish their local audio and video streams to the ZEGO RTC server. Other users in the same room can use the streamID to play the audio and video streams for intercommunication.
 * Before you start to publish the stream, you need to join the room first by calling [loginRoom]. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
 * In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
 * After the first publish stream failure due to network reasons or the publish stream is interrupted, the default time for SDK reconnection is 20min.
 *
 * @param stream_id Stream ID, a string of up to 256 characters, needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_publishing_stream(const char * stream_id, enum zego_publish_channel channel);


/**
 * Stops publishing a stream (for the specified channel).
 *
 * This function allows the user to stop sending local audio and video streams and end the call.
 * If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
 * After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
 * Use this function to stop publishing stream of aux channel.
 *
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_publishing_stream(enum zego_publish_channel channel);


/**
 * Sets the extra information of the stream being published (for the specified channel).
 *
 * Use this function to set the extra info of the stream, the result will be notified via the [ZegoPublisherSetStreamExtraInfoCallback].
 * The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
 * Developers can synchronize variable content related to stream IDs based on stream additional information.
 *
 * @param extra_info Stream extra information, a string of up to 1024 characters.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_set_stream_extra_info(const char * extra_info, enum zego_publish_channel channel);


/**
 * Starts/Updates the local video preview (for the specified channel).
 *
 * The user can see his own local image by calling this function. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default.
 * Local view and preview modes can be updated by calling this function again.
 * You can set the mirror mode of the preview by calling the [setVideoMirrorMode] function. The default preview setting is image mirrored.
 * When this function is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video. In addition to calling this function normally to preview the local screen, developers can also pass [NULL] to the canvas parameter, in conjunction with ZegoExpressEngine's sound wave function, in order to achieve the purpose of detecting whether the audio equipment is working properly before logging in to the room.
 *
 * @param canvas The view used to display the preview image. If the view is set to NULL, no preview will be made.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_preview(struct zego_canvas* canvas, enum zego_publish_channel channel);


/**
 * Stops the local video preview (for the specified channel).
 *
 * This function can be called to stop previewing when there is no need to see the preview locally.
 *
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_preview(enum zego_publish_channel channel);


/**
 * Sets up the video configurations (for the specified channel).
 *
 * This function can be used to set the video frame rate, bit rate, video capture resolution, and video encoding output resolution. If you do not call this function, the default resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
 * It is necessary to set the relevant video configuration before publishing the stream, and only support the modification of the encoding resolution and the bit rate after publishing the stream.
 * Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
 *
 * @param config Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_video_config(struct zego_video_config config, enum zego_publish_channel channel);


/**
 * Gets the current video configurations (for the specified channel).
 *
 * This function can be used to get the specified publish channel's current video frame rate, bit rate, video capture resolution, and video encoding output resolution.
 *
 * @param channel Publish stream channel
 * @return Video configuration object
 */
ZEGOEXP_API struct zego_video_config EXP_CALL zego_express_get_video_config(enum zego_publish_channel channel);


/**
 * Sets the video mirroring mode (for the specified channel).
 *
 * This function can be called to set whether the local preview video and the published video have mirror mode enabled.
 *
 * @param mirror_mode Mirror mode for previewing or publishing the stream
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_video_mirror_mode(enum zego_video_mirror_mode mirror_mode, enum zego_publish_channel channel);


/**
 * Sets the video orientation (for the specified channel).
 *
 * This function sets the orientation of the video. The captured image is rotated 90, 180 or 270 degrees counterclockwise compared to the forward direction of the phone. After rotation, it is automatically adjusted to match the encoded image resolution.
 *
 * @param orientation Video orientation
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_app_orientation(enum zego_orientation orientation, enum zego_publish_channel channel);


/**
 * Sets the video orientation (for the specified channel).
 *
 * @param orientation Video orientation
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_ios_app_orientation(enum zego_ios_orientation orientation, enum zego_publish_channel channel);


/**
 * Sets up the audio configurations.
 *
 * You can set the combined value of the audio codec, bit rate, and audio channel through this function. If this function is not called, the default is standard quality mode. Should be used before publishing.
 * If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
 *
 * @param config Audio config
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_config(struct zego_audio_config config);


/**
 * Sets up the audio configurations.
 *
 * You can set the combined value of the audio codec, bit rate, and audio channel through this function. If this function is not called, the default is standard quality mode. Should be used before publishing.
 * If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
 *
 * @param config Audio config
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_config_by_channel(struct zego_audio_config config, enum zego_publish_channel channel);


/**
 * Gets the current audio configurations.
 *
 * You can get the current audio codec, bit rate, and audio channel through this function.
 *
 * @return Audio config
 */
ZEGOEXP_API struct zego_audio_config EXP_CALL zego_express_get_audio_config();


/**
 * Gets the current audio configurations.
 *
 * You can get the current audio codec, bit rate, and audio channel through this function.
 *
 * @param channel Publish stream channel
 * @return Audio config
 */
ZEGOEXP_API struct zego_audio_config EXP_CALL zego_express_get_audio_config_by_channel(enum zego_publish_channel channel);


/**
 * Set encryption key for the publishing stream.
 *
 * Called before and after [startPublishingStream] can both take effect.
 * Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
 * Support calling this function to update the encryption key while publishing stream. Note that developers need to update the player's decryption key before updating the publisher's encryption key.
 * This function is only valid when publishing stream to the Zego RTC server.
 *
 * @param key The encryption key, note that the key length only supports 16/24/32 bytes.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_publish_stream_encryption_key(const char * key, enum zego_publish_channel channel);


/**
 * Take a snapshot of the publishing stream (for the specified channel).
 *
 * Please call this function after calling [startPublishingStream] or [startPreview]
 * The resolution of the snapshot is the encoding resolution set in [setVideoConfig]. If you need to change it to capture resolution, please call [setCapturePipelineScaleMode] to change the capture pipeline scale mode to [Post]
 *
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_take_publish_stream_snapshot(enum zego_publish_channel channel);


/**
 * Stops or resumes sending the audio part of a stream (for the specified channel).
 *
 * This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but does not send the audio data to the network. It can be set before and after publishing.
 * If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks,
 *
 * @param mute Whether to stop sending audio streams, true means not to send audio stream, and false means sending audio stream. The default is false.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_publish_stream_audio(bool mute, enum zego_publish_channel channel);


/**
 * Stops or resumes sending the video part of a stream (for the specified channel).
 *
 * This function can be called when publishing the stream to realize not publishing the video stream. The local camera can still work normally, can capture, preview and process video images normally, but does not send the video data to the network. It can be set before and after publishing.
 * If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks,
 *
 * @param mute Whether to stop sending video streams, true means not to send video stream, and false means sending video stream. The default is false.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_publish_stream_video(bool mute, enum zego_publish_channel channel);


/**
 * Enables or disables traffic control.
 *
 * Traffic control enables SDK to dynamically adjust the bitrate of audio and video streaming according to its own and peer current network environment status.
 * Automatically adapt to the current network environment and fluctuations, so as to ensure the smooth publishing of stream.
 *
 * @param enable Whether to enable traffic control. The default is ture.
 * @param property Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_traffic_control(bool enable, int property);


/**
 * Enables or disables traffic control.
 *
 * Traffic control enables SDK to dynamically adjust the bitrate of audio and video streaming according to its own and peer current network environment status.
 * Automatically adapt to the current network environment and fluctuations, so as to ensure the smooth publishing of stream.
 *
 * @param enable Whether to enable traffic control. The default is ture.
 * @param property Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_traffic_control_by_channel(bool enable, int property, enum zego_publish_channel channel);


/**
 * Sets the minimum video bitrate for traffic control.
 *
 * Set how should SDK send video data when the network conditions are poor and the minimum video bitrate cannot be met.
 * When this function is not called, the SDK will automatically adjust the sent video data frames according to the current network uplink conditions by default.
 *
 * @param bitrate Minimum video bitrate (kbps)
 * @param mode Video sending mode below the minimum bitrate.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_min_video_bitrate_for_traffic_control(int bitrate, enum zego_traffic_control_min_video_bitrate_mode mode);


/**
 * Sets the minimum video bitrate for traffic control.
 *
 * Set how should SDK send video data when the network conditions are poor and the minimum video bitrate cannot be met.
 * When this function is not called, the SDK will automatically adjust the sent video data frames according to the current network uplink conditions by default.
 *
 * @param bitrate Minimum video bitrate (kbps)
 * @param mode Video sending mode below the minimum bitrate.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_min_video_bitrate_for_traffic_control_by_channel(int bitrate, enum zego_traffic_control_min_video_bitrate_mode mode, enum zego_publish_channel channel);


/**
 * Set the factors of concern that trigger traffic control
 *
 * When the traffic control of the specified push channel is enabled through the enableTrafficControl interface, the interface can be used to control whether the traffic control is started due to poor remote network conditions
 * The function is valid before the stream is pushed. If you do not reset the settings before each push, you will continue to use the last configuration.
 *
 * @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_traffic_control_focus_on(enum zego_traffic_control_focus_on_mode mode);


/**
 * Set the factors of concern that trigger traffic control
 *
 * When the traffic control of the specified push channel is enabled through the enableTrafficControl interface, the interface can be used to control whether the traffic control is started due to poor remote network conditions
 * The function is valid before the stream is pushed. If you do not reset the settings before each push, you will continue to use the last configuration.
 *
 * @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_traffic_control_focus_on_by_channel(enum zego_traffic_control_focus_on_mode mode, enum zego_publish_channel channel);


/**
 * Sets the audio recording volume for stream publishing.
 *
 * This function is used to set the audio collection volume. The local user can control the volume of the audio stream sent to the far end. It can be set before publishing.
 *
 * @param volume Volume percentage. The range is 0 to 200. Default value is 100.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_capture_volume(int volume);


/**
 * Set audio capture stereo mode.
 *
 * This function is used to set the audio stereo capture mode. The default is mono, that is, dual channel collection is not enabled.
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param mode Audio stereo capture mode
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_capture_stereo_mode(enum zego_audio_capture_stereo_mode mode);


/**
 * Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
 *
 * Developers can call this function to publish the audio and video streams that have been published to the ZEGO RTC server to a custom CDN content distribution network that has high latency but supports high concurrent playing stream.
 * Because this called function is essentially a dynamic relay of the audio and video streams published to the ZEGO RTC server to different CDNs, this function needs to be called after the audio and video stream is published to ZEGO RTC server successfully.
 * Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves.
 * You can use ZEGO's CDN audio and video streaming content distribution service at the same time by calling this function and then use the developer who owns the CDN content distribution service.
 * This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
 * When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
 *
 * @param stream_id Stream ID
 * @param target_url CDN relay address, supported address format is rtmp.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_add_publish_cdn_url(const char * stream_id, const char * target_url);


/**
 * Deletes the specified CDN URL, which is used for relaying streams from ZEGO RTC server to CDN.
 *
 * This function is called when a CDN relayed address has been added and needs to stop propagating the stream to the CDN.
 * This function does not stop publishing audio and video stream to the ZEGO ZEGO RTC server.
 *
 * @param stream_id Stream ID
 * @param target_url CDN relay address, supported address format rtmp.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_remove_publish_cdn_url(const char * stream_id, const char * target_url);


/**
 * Whether to publish streams directly from the client to CDN without passing through Zego RTC server (for the specified channel).
 *
 * This function needs to be set before [startPublishingStream].
 * After calling this function to publish the audio and video stream directly to the CDN, calling [addPublishCdnUrl] and [removePublishCdnUrl] to dynamically relay to the CDN no longer takes effect,
 * because these two functions are to relay or stop relaying the audio and video stream from ZEGO RTC server to CDN,
 * if you enable the direct publish of audio and video streams to CDN, you will not be able to dynamically relay the audio and video streams to the CDN through the ZEGO RTC server.
 *
 * @param enable Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN
 * @param config CDN configuration, if NULL, use Zego's background default configuration
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_publish_direct_to_cdn(bool enable, struct zego_cdn_config* config, enum zego_publish_channel channel);


/**
 * Sets up the stream watermark before stream publishing (for the specified channel).
 *
 * The layout of the watermark cannot exceed the video encoding resolution of the stream. It can be set at any time before or during the publishing stream.
 *
 * @param is_preview_visible the watermark is visible on local preview
 * @param watermark The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is NULL, the watermark is cancelled.
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_publish_watermark(bool is_preview_visible, struct zego_watermark* watermark, enum zego_publish_channel channel);


/**
 * Set the Supplemental Enhancement Information type
 *
 * It must be set before [startPublishingStream].
 *
 * @param config SEI configuration. The SEI defined by ZEGO is used by default.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_sei_config(struct zego_sei_config config);


/**
 * Sends Supplemental Enhancement Information.
 *
 * This function can synchronize some other additional information while the developer publishes streaming audio and video streaming data while sending streaming media enhancement supplementary information.
 * Generally, for scenarios such as synchronizing music lyrics or precise layout of video canvas, you can choose to use this function.
 * After the anchor sends the SEI, the audience can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
 * Since SEI information follows video frames, and because of network problems, frames may be dropped, so SEI information may also be dropped. To solve this situation, it should be sent several times within the limited frequency.
 * After calling [startPublishingStream] to publish the stream successfully, you can call this function.
 * Limit frequency: Do not exceed 30 times per second.
 * The SEI data length is limited to 4096 bytes.
 *
 * @param data SEI data
 * @param data_length SEI data length
 * @param channel Publish stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_sei(const unsigned char* data, unsigned int data_length, enum zego_publish_channel channel);


/**
 * Enables or disables hardware encoding.
 *
 * Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage. The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
 * Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard coding.
 *
 * @param enable Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_hardware_encoder(bool enable);


/**
 * Sets the timing of video scaling in the video capture workflow. You can choose to do video scaling right after video capture (the default value) or before encoding.
 *
 * This function needs to be set before previewing or streaming.
 * The main effect is whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
 *
 * @param mode The capture scale timing mode
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_capture_pipeline_scale_mode(enum zego_capture_pipeline_scale_mode mode);


/**
 * The callback triggered when the state of stream publishing changes.
 *
 * After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
 * You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
 * The parameter [extendedData] is extended information with state updates. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are [flv_url_list], [rtmp_url_list], [hls_url_list]. These correspond to the publishing stream URLs of the flv, rtmp, and hls protocols.
 *
 * @param stream_id Stream ID
 * @param state State of publishing stream
 * @param error_code The error code corresponding to the status change of the publish stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param extended_data Extended information with state updates.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_state_update)(const char * stream_id, enum zego_publisher_state state, zego_error error_code, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_state_update_callback(zego_on_publisher_state_update callback_func, void * user_context);


/**
 * Callback for current stream publishing quality.
 *
 * After calling the [startPublishingStream] successfully, the callback will be received every 3 seconds. Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.
 * You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback function, in order to show the uplink network status in real time on the device UI.
 * If you does not know how to use the parameters of this callback function, you can only pay attention to the [level] field of the [quality] parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
 *
 * @param stream_id Stream ID
 * @param quality Publishing stream quality, including audio and video framerate, bitrate, RTT, etc.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_quality_update)(const char * stream_id, struct zego_publish_stream_quality quality, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_quality_update_callback(zego_on_publisher_quality_update callback_func, void * user_context);


/**
 * The callback triggered when the first audio frame is captured.
 *
 * After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
 * In the case of no startPublishingStream audio and video stream or preview, the first startPublishingStream audio and video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect audio data of the local device and receive this callback.
 * Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
 *
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_captured_audio_first_frame)(void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_captured_audio_first_frame_callback(zego_on_publisher_captured_audio_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the first video frame is captured.
 *
 * After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of video data.
 * In the case of no startPublishingStream video stream or preview, the first startPublishingStream video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect video data of the local device and receive this callback.
 * Developers can use this callback to determine whether SDK has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
 *
 * @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_captured_video_first_frame)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_captured_video_first_frame_callback(zego_on_publisher_captured_video_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the first video frame is rendered.
 *
 * this callback will be called after SDK rendered the first frame of video data captured.
 *
 * @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_render_video_first_frame)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_render_video_first_frame_callback(zego_on_publisher_render_video_first_frame callback_func, void * user_context);


/**
 * The callback triggered when the video capture resolution changes.
 *
 * After the successful publish, the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
 * When the audio and video stream is not published or previewed for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the SDK is started, the video data of the local device will be collected, and the collection resolution will change at this time.
 * You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
 *
 * @param width Video capture resolution width
 * @param height Video capture resolution height
 * @param channel Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_video_size_changed)(int width, int height, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_video_size_changed_callback(zego_on_publisher_video_size_changed callback_func, void * user_context);


/**
 * The callback triggered when the state of relayed streaming to CDN changes.
 *
 * After the ZEGO RTC server relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
 * Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
 * If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
 *
 * @param stream_id Stream ID
 * @param info_list List of information that the current CDN is relaying
 * @param info_count List of information count that the current CDN is relaying
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_relay_cdn_state_update)(const char * stream_id, const struct zego_stream_relay_cdn_info* info_list, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_relay_cdn_state_update_callback(zego_on_publisher_relay_cdn_state_update callback_func, void * user_context);


/**
 * Callback for setting stream extra information.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Message sequence.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_update_stream_extra_info_result)(zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_update_stream_extra_info_result_callback(zego_on_publisher_update_stream_extra_info_result callback_func, void * user_context);


/**
 * Callback for add/remove CDN URL.
 *
 * @param stream_id Stream ID.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Message sequence.
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_update_cdn_url_result)(const char * stream_id, zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_update_cdn_url_result_callback(zego_on_publisher_update_cdn_url_result callback_func, void * user_context);


/**
 * Results of take publish stream snapshot.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param channel Publish stream channel
 * @param image Snapshot image (Windows: HBITMAP; macOS/iOS: CGImageRef; Linux: QImage; Android: Bitmap)
 * @param user_context Context of user.
 */
typedef void(*zego_on_publisher_take_snapshot_result)(zego_error error_code, enum zego_publish_channel channel, void * image, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_publisher_take_snapshot_result_callback(zego_on_publisher_take_snapshot_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
