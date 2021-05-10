
#ifndef __ZEGO_EXPRESS_MIXER_H__
#define __ZEGO_EXPRESS_MIXER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Starts a stream mixing task.
 *
 * Due to the performance considerations of the client device, ZegoExpressEngine's mix stream is to start the mixing stream task on the server side of the ZEGO RTC server for mixing stream.
 * After calling this function, SDK initiates a mixing stream request to the ZEGO RTC server. The server will find the current publishing stream and perform video layer blending according to the parameters of the mixing stream task requested by SDK.
 * When you need to update the mixing stream task, that is, the input stream list needs to be updated when the input stream increases or decreases, you can update the field of the [ZegoMixerTask] object inputList and call this function again to pass the same [ZegoMixerTask] object to update the mixing stream task.
 * If an exception occurs when requesting to start the mixing stream task, for example, the most common mix input stream does not exist, it will be given from the callback error code. For specific error codes, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * If an input stream does not exist in the middle, the mixing stream task will automatically retry playing the input stream for 90 seconds, and will not retry after 90 seconds.
 *
 * @param task Stream mixing task object
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_start_mixer_task(struct zego_mixer_task task);


/**
 * Stops a stream mixing task.
 *
 * Similar to [startMixerTask], after calling this function, SDK initiates a request to end the mixing stream task to the ZEGO RTC server.
 * If you starts the next mixing stream task without stopping the previous mixing stream task, the previous mixing stream task will not stop automatically. The previous mixing stream task will not be stopped automatically until 90 seconds after the input stream of the previous mixing stream task does not exist.
 * Developers should pay attention when using the stream mixing function that, before starting the next mixer task, they should stop the previous mixer task, so as avoid that when an anchor has start the next mixer task to mix stream with other anchors, and the audience is still playing the previous mixer task's output stream.
 *
 * @param task Stream mixing task object
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_stop_mixer_task(struct zego_mixer_task task);


/**
 * Starts an auto stream mixing task.
 *
 * @param task Auto mix stream task object
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_start_auto_mixer_task(struct zego_auto_mixer_task task);


/**
 * Stops an auto stream mixing task.
 *
 * @param task Auto mix stream task object
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_stop_auto_mixer_task(struct zego_auto_mixer_task task);


/**
 * The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
 *
 * In the general case of the ZEGO RTC server's stream mixing task, the output stream is published to the CDN using the RTMP protocol, and changes in the state during the publish will be notified from this callback function.
 *
 * @param task_id Stream mixing task ID
 * @param info_list List of information that the current CDN is being mixed
 * @param info_count List count of information that the current CDN is being mixed
 * @param user_context Context of user.
 */
typedef void(*zego_on_mixer_relay_cdn_state_update)(const char * task_id, const struct zego_stream_relay_cdn_info* info_list, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_mixer_relay_cdn_state_update_callback(zego_on_mixer_relay_cdn_state_update callback_func, void * user_context);


/**
 * The callback triggered when the sound level of any input stream changes in the stream mixing process.
 *
 * You can use this callback to show the effect of the anchors sound level when the audience plays the mixed stream. So audience can notice which anchor is speaking.
 *
 * @param sound_levels Sound level hash map, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
 * @param info_count Count of mixing stream sound level
 * @param user_context Context of user.
 */
typedef void(*zego_on_mixer_sound_level_update)(const struct zego_mixer_sound_level_info* sound_levels, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_mixer_sound_level_update_callback(zego_on_mixer_sound_level_update callback_func, void * user_context);


/**
 * The callback triggered when the sound level of any input stream changes in the auto streaming mixing process.
 *
 * @param sound_levels Sound level hash map, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
 * @param info_count Count of mixing stream sound level
 * @param user_context Context of user.
 */
typedef void(*zego_on_auto_mixer_sound_level_update)(const struct zego_mixer_sound_level_info* sound_levels, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_sound_level_update_callback(zego_on_auto_mixer_sound_level_update callback_func, void * user_context);


/**
 * Results of starting a mixer task.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence.
 * @param extended_data Extended Information
 * @param user_context Context of user.
 */
typedef void(*zego_on_mixer_start_result)(zego_error error_code, zego_seq seq, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_mixer_start_result_callback(zego_on_mixer_start_result callback_func, void * user_context);


/**
 * Results of stoping a mixer task.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence.
 * @param user_context Context of user.
 */
typedef void(*zego_on_mixer_stop_result)(zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_mixer_stop_result_callback(zego_on_mixer_stop_result callback_func, void * user_context);


/**
 * Results of starting a auto mixer task.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence.
 * @param extended_data Extended Information
 * @param user_context Context of user.
 */
typedef void(*zego_on_auto_mixer_start_result)(zego_error error_code, zego_seq seq, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_start_result_callback(zego_on_auto_mixer_start_result callback_func, void * user_context);


/**
 * Results of stoping a auto mixer task.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence.
 * @param user_context Context of user.
 */
typedef void(*zego_on_auto_mixer_stop_result)(zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_auto_mixer_stop_result_callback(zego_on_auto_mixer_stop_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
