
#ifndef __ZEGO_EXPRESS_RECORD_H__
#define __ZEGO_EXPRESS_RECORD_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Starts to record locally captured audio or video and directly save the data to a file.
 *
 * Currently only one task can be recorded simultaneously.
 * This function needs to be called after the success of [startPreview] or [startPublishingStream] to be effective.
 * Developers should not [stopPreview] or [stopPublishingStream] during recording, otherwise the SDK will end the current recording task.
 * Developers will receive the [onCapturedDataRecordStateUpdate] and the [onCapturedDataRecordProgressUpdate] callback after start recording.
 *
 * @param config Record config
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_recording_captured_data(struct zego_data_record_config config, enum zego_publish_channel channel);


/**
 * Stops recording locally captured audio or video.
 *
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_recording_captured_data(enum zego_publish_channel channel);


/**
 * The callback triggered when the state of data recording (to a file) changes.
 *
 * @param state File recording status, according to which you should determine the state of the file recording or the prompt of the UI.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param config Record config
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_captured_data_record_state_update)(enum zego_data_record_state state, zego_error error_code, struct zego_data_record_config config, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_captured_data_record_state_update_callback(zego_on_captured_data_record_state_update callback_func, void * user_context);


/**
 * The callback to report the current recording progress.
 *
 * @param progress File recording progress, which allows developers to hint at the UI, etc.
 * @param config Record config
 * @param channel Publishing stream channel
 * @param user_context Context of user.
 */
typedef void(*zego_on_captured_data_record_progress_update)(struct zego_data_record_progress progress, struct zego_data_record_config config, enum zego_publish_channel channel, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_captured_data_record_progress_update_callback(zego_on_captured_data_record_progress_update callback_func, void * user_context);



ZEGO_END_DECLS

#endif
