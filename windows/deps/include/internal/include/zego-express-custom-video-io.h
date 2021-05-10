
#ifndef __ZEGO_EXPRESS_CUSTOM_VIDEO_IO_H__
#define __ZEGO_EXPRESS_CUSTOM_VIDEO_IO_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Enables or disables custom video rendering.
 *
 * It must be set before the engine starts, that is, before calling [startPreview], [startPublishingStream], [startPlayingStream]; and the configuration can be modified after the engine is stopped， that is after calling [logoutRoom].
 * When the developer starts custom rendering, by calling [setCustomVideoRenderHandler], you can set to receive local and remote video frame data for custom rendering
 *
 * @param enable enable or disable
 * @param config custom video render config
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_custom_video_render(bool enable, struct zego_custom_video_render_config* config);


/**
 * Enables or disables custom video capture (for the specified channel).
 *
 * It must be set before the engine starts, that is, before calling [startPreview], [startPublishingStream]; and the configuration can be modified after the engine is stopped, that is, after calling [logoutRoom].
 * When the developer starts the custom capture, it can be set to receive notification of the start and stop of the custom capture by calling [setCustomVideoCaptureHandler].
 *
 * @param enable enable or disable
 * @param config custom video capture config
 * @param channel publish channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_custom_video_capture(bool enable, struct zego_custom_video_capture_config* config, enum zego_publish_channel channel);


/**
 * Sends the video frames (Raw Data) produced by custom video capture to the SDK (for the specified channel).
 *
 * This function need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
 *
 * @param data video frame data
 * @param data_length video frame data length
 * @param params video frame param
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_capture_raw_data(const unsigned char* data, unsigned int data_length, const struct zego_video_frame_param params, unsigned long long reference_time_millisecond, unsigned int reference_time_scale, enum zego_publish_channel channel);


/**
 * Sends the video frames (Texture Data) produced by custom video capture to the SDK (for the specified channel).
 *
 * This function need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
 *
 * @param texture_id texture ID
 * @param width Video frame width
 * @param height Video frame height
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_capture_texture_data(int texture_id, int width, int height, double reference_time_millisecond, enum zego_publish_channel channel);


/**
 * Gets the SurfaceTexture instance (for the specified channel).
 *
 * @param channel Publishing stream channel
 * @return SurfaceTexture instance
 */
ZEGOEXP_API void* EXP_CALL zego_express_get_custom_video_capture_surface_texture(enum zego_publish_channel channel);


/**
 * Sends the video frames (CVPixelBuffer) produced by custom video capture to the SDK (for the specified channel).
 *
 * This function need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
 *
 * @param buffer Video frame data to send to the SDK
 * @param timestamp Timestamp of this video frame
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_capture_pixel_buffer(void* buffer, double timestamp, enum zego_publish_channel channel);


/**
 * Sends the video frames (Encoded Data) produced by custom video capture to the SDK (for the specified channel).
 *
 * This function need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
 *
 * @param data video frame encoded data
 * @param data_length video frame data length
 * @param params video frame param
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_capture_encoded_data(const unsigned char* data, unsigned int data_length, const struct zego_video_encoded_frame_param params, double reference_time_millisecond, enum zego_publish_channel channel);


/**
 * Sets the video fill mode of custom video capture (for the specified channel).
 *
 * @param mode View mode
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_custom_video_capture_fill_mode(enum zego_view_mode mode, enum zego_publish_channel channel);


/**
 * Sets the video flip mode of custom video capture (for the specified channel). This function takes effect only if the custom video buffer type is Texture2D.
 *
 * @param mode Flip mode
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_custom_video_capture_flip_mode(enum zego_video_flip_mode mode, enum zego_publish_channel channel);


/**
 * The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
 *
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_capture_start)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_capture_start_callback(zego_on_custom_video_capture_start callback_func, void * user_context);


/**
 * The callback triggered when SDK stops receiving captured video data.
 *
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_capture_stop)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_capture_stop_callback(zego_on_custom_video_capture_stop callback_func, void * user_context);


/**
 * SDK detects network changes and informs developers that it needs to do traffic control
 * In the case of custom video capture by sending encoded data, the SDK cannot know the external encoding configuration, so the traffic control operation needs to be completed by the developer.
 * The SDK will notify the developer of the recommended value of the video configuration according to the current network situation, and the developer needs to modify the encoder configuration by himself to ensure the smoothness of video transmission
 * Please do not perform time-consuming operations in this callback. If you need to perform time-consuming operations, please switch threads
 *
 * @param traffic_control_info traffic control info
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_capture_encoded_data_traffic_control)(struct zego_traffic_control_info traffic_control_info, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_capture_encoded_data_traffic_control_callback(zego_on_custom_video_capture_encoded_data_traffic_control callback_func, void * user_context);


/**
 * The callback for obtaining the locally captured video frames (Raw Data).
 *
 * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2])
 * @param data_length Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2])
 * @param param Video frame parameters
 * @param flip_mode video flip mode
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_render_captured_frame_data)(unsigned char** data, unsigned int* data_length, const struct zego_video_frame_param param, enum zego_video_flip_mode flip_mode, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_render_captured_frame_data_callback(zego_on_custom_video_render_captured_frame_data callback_func, void * user_context);


/**
 * The callback for obtaining the video frames (Raw Data) of the remote stream. Different streams can be identified by streamID.
 *
 * @param stream_id Stream ID
 * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2])
 * @param data_length Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2])
 * @param param Video frame parameters
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_render_remote_frame_data)(const char * stream_id, unsigned char** data, unsigned int* data_length, const struct zego_video_frame_param param, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_render_remote_frame_data_callback(zego_on_custom_video_render_remote_frame_data callback_func, void * user_context);


/**
 * The callback for obtaining the locally captured video frames (Encoded Data).
 *
 * @param data Encoded data of video frames
 * @param data_length Data length
 * @param param Video frame parameters
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_render_captured_frame_encoded_data)(const unsigned char* data, unsigned int data_length, const struct zego_video_encoded_frame_param param, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_render_captured_frame_encoded_data_callback(zego_on_custom_video_render_captured_frame_encoded_data callback_func, void * user_context);


/**
 * The callback for obtianing the video frames (Encoded Data) of the remote stream. Different streams can be identified by streamID.
 *
 * @param data Encoded data of video frames
 * @param data_length Data length
 * @param param Video frame parameters
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param stream_id Stream ID
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_render_remote_frame_encoded_data)(const unsigned char* data, unsigned int data_length, const struct zego_video_encoded_frame_param param, unsigned long long reference_time_millisecond, const char * stream_id, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_render_remote_frame_encoded_data_callback(zego_on_custom_video_render_remote_frame_encoded_data callback_func, void * user_context);


/**
 * Enables or disables custom video processing.
 *
 * When developers to open before the custom processing, by calling [setCustomVideoCaptureHandler] can be set up to receive a custom video processing of the original video data
 * Precondition： Call [CreateEngine] to initialize the Zego SDK
 * Call timing： must be set before calling [startPreview], [startPublishingStream]; The configuration cannot be changed again until the [logoutRoom] is called, otherwise the call will not take effect
 * Supported version： 2.2.0
 *
 * @param enable enable or disable. disable by default
 * @param config custom video processing configuration. If NULL is passed, the platform default value is used.
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_custom_video_processing(bool enable, struct zego_custom_video_process_config* config, enum zego_publish_channel channel);


/**
 * Send the custom video processed [RawData] type video data to the SDK (for the specified channel).
 *
 * This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA]
 * Precondition：Call [createEngine] to initialize Zego SDK
 * Call timing：It must be called in the [onCapturedUnprocessedRawData] callback
 * Supported version：2.2.0
 *
 * @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2]
 * @param data_length Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2]
 * @param params video frame param
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_processed_raw_data(const unsigned char** data, unsigned int* data_length, struct zego_video_frame_param params, unsigned long long reference_time_millisecond, enum zego_publish_channel channel);


/**
 * Send the [CVPixelBuffer] type video data after the custom video processing to the SDK (for the specified channel).
 *
 * This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_CVPIXELBUFFER]
 * Precondition：Call [createEngine] to initialize Zego SDK
 * Call timing：It must be called in the [onCapturedUnprocessedCVPixelBuffer] callback
 * Supported version：2.2.0
 *
 * @param buffer CVPixelBuffer type video frame data to be sent to the SDK
 * @param timestamp Timestamp of this video frame
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_processed_cv_pixel_buffer(void* buffer, unsigned long long timestamp, enum zego_publish_channel channel);


/**
 * Send the [Texture] type video data after the pre-processing of the custom video to the SDK (for the specified channel).
 *
 * This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_GL_TEXTURE_2D]
 * Developers are requested to ensure that they have sufficient openGL programming experience before using Texture type video processing, otherwise it is easy to cause unforeseen problems
 * Precondition：Call [createEngine] to initialize Zego SDK
 * Call timing：It must be called in the [onCapturedUnprocessedTextureData] callback
 * Supported version：2.2.0
 *
 * @param texture_id texture ID
 * @param width Texture width
 * @param height Texture height
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_send_custom_video_processed_texture_data(int texture_id, int width, int height, unsigned long long reference_time_millisecond, enum zego_publish_channel channel);


/**
 * The SDK informs the developer that it is about to start custom video processing
 *
 * It is recommended to initialize other resources in this callback
 *
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_process_start)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_start_callback(zego_on_custom_video_process_start callback_func, void * user_context);


/**
 * The SDK informs the developer to stop custom video processing
 *
 * It is recommended to destroy other resources in this callback
 *
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_process_stop)(enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_stop_callback(zego_on_custom_video_process_stop callback_func, void * user_context);


/**
 * Get SurfaceTexture for output data (for the specified channel).
 *
 * This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_SURFACETEXTURE]
 * After the developer receives the original video data, they can call this interface to get the SurfaceTexture used to output the video data
 * After the developer obtains the Output SurfaceTexture, it needs to be converted to the Surface type as the producer of the output data. After the developer completes the video processing, send the processed video data to the SDK through this Surface
 * Precondition：Call [createEngine] to initialize Zego SDK
 * Call timing：It is recommended to call in the callback [onGetSurfaceTexture]
 * Supported version：2.2.0
 *
 * @param width Specify the width of the output video data
 * @param height Specify the height of the output video data
 * @param channel Publishing stream channel
 * @return SurfaceTexture instance
 */
ZEGOEXP_API void* EXP_CALL zego_express_get_custom_video_process_output_surface_texture(int width, int height, enum zego_publish_channel channel);


/**
 * Call back when the raw video data of type [RawData] is obtained.
 *
 * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA]
 * After the developer has processed the original image, he must call [sendCustomVideoProcessedRawData] to send the processed data back to the SDK, otherwise it will cause frame loss
 * Precondition：call [setCustomVideoProcessHandler] to set callback
 * Supported version：2.2.0
 *
 * @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2]
 * @param data_length Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2]
 * @param param Video frame parameters
 * @param reference_time_millisecond Video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_process_captured_unprocessed_raw_data)(const unsigned char** data, unsigned int* data_length, const struct zego_video_frame_param param, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_captured_unprocessed_raw_data_callback(zego_on_custom_video_process_captured_unprocessed_raw_data callback_func, void * user_context);


/**
 * Call back when the original video data of type [CVPixelBuffer] is obtained
 *
 * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_CVPIXELBUFFER]
 * After the developer has processed the original image, he must call [sendCustomVideoProcessedCVPixelbuffer] to send the processed data back to the SDK, otherwise it will cause frame loss
 * Precondition： call [setCustomVideoProcessHandler] to set callback
 * Supported version： 2.2.0
 *
 * @param buffer CVPixelBufferRef type data
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer)(void * buffer, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_captured_unprocessed_cvpixelbuffer_callback(zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer callback_func, void * user_context);


/**
 * Call back when the original video data of type [Texture] is obtained
 *
 * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_GL_TEXTURE_2D]
 * After the developer has processed the original image, he must call [sendCustomVideoProcessedTextureData] to send the processed data back to the SDK, otherwise it will cause frame loss
 * Precondition： call [setCustomVideoProcessHandler] to set callback
 * Supported version： 2.2.0
 *
 * @param texture_id Texture ID
 * @param width Texture width
 * @param height Texture height
 * @param reference_time_millisecond video frame reference time, UNIX timestamp, in milliseconds.
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_custom_video_process_captured_unprocessed_texture_data)(int texture_id, int width, int height, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_captured_unprocessed_texture_data_callback(zego_on_custom_video_process_captured_unprocessed_texture_data callback_func, void * user_context);


/**
 * When the developer chooses the SurfaceTexture type, the SDK needs to obtain the SurfaceTexture from the developer for inputting original video data
 *
 * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_SURFACETEXTURE]
 * Developers need to create a SurfaceTexture object and listen to its member method [setOnFrameAvailableListener], and then return the corresponding SurfaceTexture. When the SDK original video data input is complete, the developer will receive [onFrameAvailable]
 * After the developer passes the SurfaceTexture(input) object to the SDK, he can call [getCustomVideoProcessOutputSurfaceTexture] to get the SurfaceTexture used by the SDK to encode and publish the stream, and then the developer can pass the video processed data into this SurfaceTexture(output) object
 *
 * @param width The capture width of the original video
 * @param height The capture height of the original video
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 * @return SurfaceTexture instance
 */
typedef void*(*zego_on_custom_video_process_get_input_surface_texture)(int width, int height, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_custom_video_process_get_input_surface_texture_callback(zego_on_custom_video_process_get_input_surface_texture callback_func, void * user_context);



ZEGO_END_DECLS

#endif
