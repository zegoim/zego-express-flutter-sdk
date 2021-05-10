
#ifndef __ZEGO_EXPRESS_AUDIO_EFFECT_PLAYER_H__
#define __ZEGO_EXPRESS_AUDIO_EFFECT_PLAYER_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Creates a audio effect player instance.
 *
 * Currently, a maximum of 1 instances can be created, after which it will return NULL.
 *
 * @return audio effect player instance, NULL will be returned when the maximum number is exceeded.
 */
ZEGOEXP_API enum zego_audio_effect_player_instance_index EXP_CALL zego_express_create_audio_effect_player();


/**
 * Destroys a audio effect player instance.
 *
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_destroy_audio_effect_player(enum zego_audio_effect_player_instance_index instance_index);


/**
 * Start playing audio effect.
 *
 * The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config].
 *
 * @param audio_effect_id ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique id. The recommended methods are static self-incrementing id or the hash of the incoming sound effect file path.
 * @param path The absolute path of the local resource. "assets://"、"ipod-library://" and network url are not supported. Set path as NULL or "" if resource is loaded already using [loadResource]
 * @param config Audio effect playback configuration. Set NULL will only be played once, and will not be mixed into the publishing stream.
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_start(unsigned int audio_effect_id, const char * path, struct zego_audio_effect_play_config* config, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Stop playing audio effect.
 *
 * @param audio_effect_id ID for the audio effect
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_stop(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Pause playing audio effect.
 *
 * @param audio_effect_id ID for the audio effect
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_pause(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Resume playing audio effect.
 *
 * @param audio_effect_id ID for the audio effect
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_resume(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Stop playing all audio effect.
 *
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_stop_all(enum zego_audio_effect_player_instance_index instance_index);


/**
 * Pause playing all audio effect.
 *
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_pause_all(enum zego_audio_effect_player_instance_index instance_index);


/**
 * Resume playing all audio effect.
 *
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_resume_all(enum zego_audio_effect_player_instance_index instance_index);


/**
 * Set the specified playback progress.
 *
 * Unit is millisecond
 *
 * @param audio_effect_id ID for the audio effect
 * @param millisecond Point in time of specified playback progress
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_audio_effect_player_seek_to(unsigned int audio_effect_id, unsigned long long millisecond, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Set volume for the audio effect. Both the local play volume and the publish volume are set.
 *
 * @param audio_effect_id ID for the audio effect
 * @param volume The range is 0 ~ 200. The default is 100.
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_volume(unsigned int audio_effect_id, int volume, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Set volume for all audio effect. Both the local play volume and the publish volume are set.
 *
 * @param volume The range is 0 ~ 200. The default is 100.
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_set_volume_all(int volume, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Get the total progress of your media resources.
 *
 * You should load resource before invoking this function, otherwise the return value is 0
 *
 * @param audio_effect_id ID for the audio effect
 * @param instance_index audio effect instance index
 * @return Unit is millisecond
 */
ZEGOEXP_API unsigned long long EXP_CALL zego_express_audio_effect_player_get_total_duration(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Get current playing progress.
 *
 * You should load resource before invoking this function, otherwise the return value is 0
 *
 * @param audio_effect_id ID for the audio effect
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API unsigned long long EXP_CALL zego_express_audio_effect_player_get_current_progress(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Load audio effect resource.
 *
 * In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file. Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading
 *
 * @param audio_effect_id ID for the audio effect
 * @param path the absolute path of the audio effect resource.
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_audio_effect_player_load_resource(unsigned int audio_effect_id, const char * path, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Unload audio effect resource.
 *
 * After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
 *
 * @param audio_effect_id ID for the audio effect loaded
 * @param instance_index audio effect instance index
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_audio_effect_player_unload_resource(unsigned int audio_effect_id, enum zego_audio_effect_player_instance_index instance_index);


/**
 * Audio effect playback state callback.
 *
 * This callback is triggered when the playback state of a audio effect of the audio effect player changes.
 *
 * @param audio_effect_id The ID of the audio effect resource that triggered this callback
 * @param state The playback state of the audio effect
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param instance_index audio effect instance index
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_effect_play_state_update)(unsigned int audio_effect_id, enum zego_audio_effect_play_state state, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_effect_play_state_update_callback(zego_on_audio_effect_play_state_update callback_func, void * user_context);


/**
 * Callback for audio effect player loads resources.
 *
 * @param seq Sequence.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param instance_index Audio effect player instance index
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_effect_player_load_resource)(zego_seq seq, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_effect_player_load_resource_callback(zego_on_audio_effect_player_load_resource callback_func, void * user_context);


/**
 * Callback for audio effect player seek to playback progress.
 *
 * @param seq Sequence.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param instance_index Audio effect player instance index
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_effect_player_seek_to)(zego_seq seq, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_effect_player_seek_to_callback(zego_on_audio_effect_player_seek_to callback_func, void * user_context);



ZEGO_END_DECLS

#endif
