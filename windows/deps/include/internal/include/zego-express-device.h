
#ifndef __ZEGO_EXPRESS_DEVICE_H__
#define __ZEGO_EXPRESS_DEVICE_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Mutes or unmutes the microphone.
 *
 * This function is used to control whether the collected audio data is used. When the microphone is muted (disabled), the data is collected and discarded, and the microphone is still occupied.
 * The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this function simply discards the collected data.
 * If you really want SDK to give up occupy the microphone, you can call the [enableAudioCaptureDevice] function.
 * Developers who want to control whether to use microphone on the UI should use this function to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice].
 *
 * @param mute Whether to mute (disable) the microphone, true: mute (disable) microphone, false: enable microphone. The default is false.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_microphone(bool mute);


/**
 * Checks whether the microphone is muted.
 *
 * Can be used with [muteMicrophone], determine whether the microphone is muted.
 *
 * @return Whether the microphone is muted; true: the microphone is muted; false: the microphone is enable (not muted)
 */
ZEGOEXP_API bool EXP_CALL zego_express_is_microphone_muted();


/**
 * Mutes or unmutes the audio output speaker.
 *
 * After mute speaker, all the SDK sounds will not play, including playing stream, mediaplayer, etc. But the SDK will still occupy the output device.
 *
 * @param mute Whether to mute (disable) speaker audio output, true: mute (disable) speaker audio output, false: enable speaker audio output. The default value is false
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_speaker(bool mute);


/**
 * Checks whether the audio output speaker is muted.
 *
 * Can be used with [muteSpeaker], determine whether the speaker audio output is muted.
 *
 * @return Whether the speaker is muted; true: the speaker is muted; false: the speaker is enable (not muted)
 */
ZEGOEXP_API bool EXP_CALL zego_express_is_speaker_muted();


/**
 * Gets a list of audio devices.
 *
 * @param device_type Audio device type
 * @param device_count Count of device.
 * @return Audo device List
 */
ZEGOEXP_API struct zego_device_info *EXP_CALL zego_express_get_audio_device_list(enum zego_audio_device_type device_type, int * device_count);


/**
 * Release a list of audio devices, only supported in Windows/macOS.
 *
 * @param device_list Audio device type
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_free_audio_device_list(struct zego_device_info* device_list);


/**
 * Get the device ID of the default audio device.
 *
 * @param device_type Audio device type
 * @return Default Audio device ID
 */
ZEGOEXP_API const char* EXP_CALL zego_express_get_default_audio_device_id(enum zego_audio_device_type device_type);


/**
 * Chooses to use the specified audio device.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_use_audio_device(enum zego_audio_device_type device_type, const char * device_id);


/**
 * Get volume for the specified audio device.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 * @return Device volume
 */
ZEGOEXP_API int EXP_CALL zego_express_get_audio_device_volume(enum zego_audio_device_type device_type, const char * device_id);


/**
 * Set volume for the specified audio device.
 *
 * The direct operating system device may fail due to system restrictions. Please use [setCaptureVolume] and [setPlayVolume] first to adjust the volume of publish and play streams.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 * @param volume Device volume
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_device_volume(enum zego_audio_device_type device_type, const char * device_id, int volume);


/**
 * Turn on audio device volume monitoring.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_audio_device_volume_monitor(enum zego_audio_device_type device_type, const char * device_id);


/**
 * Turn on audio device volume monitoring.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_audio_device_volume_monitor(enum zego_audio_device_type device_type, const char * device_id);


/**
 * Mutes or unmutes the audio device.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 * @param mute Whether to mute the audio device; true means to mute the audio device; false means to unmute the audio device.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_audio_device(enum zego_audio_device_type device_type, const char * device_id, bool mute);


/**
 * Check if the audio device is muted.
 *
 * @param device_type Audio device type
 * @param device_id ID of a device obtained by [getAudioDeviceList]
 * @return Whether the audio device is muted; true means the audio device is muted; false means the audio device is not muted.
 */
ZEGOEXP_API bool EXP_CALL zego_express_is_audio_device_muted(enum zego_audio_device_type device_type, const char * device_id);


/**
 * Enables or disables the audio capture device.
 *
 * This function is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
 * Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] function is generally recommended.
 *
 * @param enable Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_audio_capture_device(bool enable);


/**
 * get current audio route type
 */
ZEGOEXP_API enum zego_audio_route EXP_CALL zego_express_get_audio_route_type();


/**
 * Whether to use the built-in speaker to play audio.
 *
 * When you choose not to use the built-in speaker to play sound, that is, set to [false], the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
 *
 * @param default_to_speaker Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_route_to_speaker(bool default_to_speaker);


/**
 * Turns on/off the camera (for the specified channel).
 *
 * This function is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
 * In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
 *
 * @param enable Whether to turn on the camera, true: turn on camera, false: turn off camera
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_camera(bool enable, enum zego_publish_channel channel);


/**
 * Switches to the front or the rear camera (for the specified channel).
 *
 * This function is used to control the front or rear camera
 * In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
 *
 * @param enable Whether to use the front camera, true: use the front camera, false: use the the rear camera. The default value is true
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_use_front_camera(bool enable, enum zego_publish_channel channel);


/**
 * Set the camera zoom factor.
 *
 * Every time the camera is restarted, the camera zoom factor will be restored to its initial value.
 *
 * @param factor The zoom factor of the camera, the minimum value is 1.0, and the maximum value is the return value of [getCameraMaxZoomFactor].
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_camera_zoom_factor(float factor, enum zego_publish_channel channel);


/**
 * Get the maximum zoom factor of the camera.
 *
 * This is only available after the camera has been successfully started, and can generally be called when the captured first frame is received, aka [onPublisherCapturedVideoFirstFrame] callback.
 *
 * @param channel Publishing stream channel
 * @return The maximum zoom factor of the camera.
 */
ZEGOEXP_API float EXP_CALL zego_express_get_camera_max_zoom_factor(enum zego_publish_channel channel);


/**
 * Chooses to use the specified video device (for the specified channel).
 *
 * @param device_id ID of a device obtained by getVideoDeviceList
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_use_video_device(const char * device_id, enum zego_publish_channel channel);


/**
 * Gets a list of video devices.
 *
 * @return Video device List
 */
ZEGOEXP_API struct zego_device_info *EXP_CALL zego_express_get_video_device_list(int * device_count);


/**
 * Release a list of video devices, only supported in Windows/macOS.
 *
 * @param device_list Video device type
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_free_video_device_list(struct zego_device_info* device_list);


/**
 * Get the deviceID of the default video device.
 *
 * @return Default video device ID
 */
ZEGOEXP_API const char* EXP_CALL zego_express_get_default_video_device_id();


/**
 * Starts sound level monitoring. Support setting the listening interval.
 *
 * After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
 * Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
 * [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
 *
 * @param millisecond Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_sound_level_monitor(unsigned int millisecond);


/**
 * Stops sound level monitoring.
 *
 * After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_sound_level_monitor();


/**
 * Starts audio spectrum monitoring. Support setting the listening interval.
 *
 * After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
 * [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
 *
 * @param millisecond Monitoring time period of the audio spectrum, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_start_audio_spectrum_monitor(unsigned int millisecond);


/**
 * Stops audio spectrum monitoring.
 *
 * After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_stop_audio_spectrum_monitor();


/**
 * Enables or disables headphone monitoring.
 *
 * enable/disable headphone monitor, this setting takes effect when the headset is connected.
 *
 * @param enable Whether to use headphone monitor, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_headphone_monitor(bool enable);


/**
 * Sets the headphone monitor volume.
 *
 * set headphone monitor volume, this setting takes effect when the headset is connected.
 *
 * @param volume headphone monitor volume, range from 0 to 200, 100 as default
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_headphone_monitor_volume(int volume);


/**
 * Enables or disables system audio capture.
 *
 * Enable sound card capture to mix sounds played by the system into the publishing stream, such as sounds played by the browser, sounds played by other software, etc.
 *
 * @param enable Whether to mix system playout
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_mix_system_playout(bool enable);


/**
 * set mix system playout volume
 *
 * only windows/macOS supported
 *
 * @param volume the volume. Valid range [0, 200], default is 100
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_mix_system_playout_volume(int volume);


/**
 * Enables or disables mix engine playout.
 *
 * Enable engine playout to mix sounds played by engine into the stream publishing.
 *
 * @param enable Whether to mix engine playout
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_mix_engine_playout(bool enable);


/**
 * The callback triggered when there is a change to audio devices (i.e. new device added or existing device deleted).
 *
 * This callback is triggered when an audio device is added or removed from the system. By listening to this callback, users can update the sound collection or output using a specific device when necessary.
 *
 * @param update_type Update type (add/delete)
 * @param device_type Audio device type
 * @param device_info Audio device information
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_device_state_changed)(enum zego_update_type update_type, enum zego_audio_device_type device_type, struct zego_device_info device_info, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_device_state_changed_callback(zego_on_audio_device_state_changed callback_func, void * user_context);


/**
 * The callback triggered when there is a change of the volume fo the audio devices.
 *
 * @param device_type Audio device type
 * @param device_id Audio device ID
 * @param volume audio device volume
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_device_volume_changed)(enum zego_audio_device_type device_type, const char * device_id, int volume, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_device_volume_changed_callback(zego_on_audio_device_volume_changed callback_func, void * user_context);


/**
 * The callback triggered when there is a change to video devices (i.e. new device added or existing device deleted).
 *
 * This callback is triggered when a video device is added or removed from the system. By listening to this callback, users can update the video capture using a specific device when necessary.
 *
 * @param update_type Update type (add/delete)
 * @param device_info Audio device information
 * @param user_context Context of user.
 */
typedef void(*zego_on_video_device_state_changed)(enum zego_update_type update_type, struct zego_device_info device_info, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_video_device_state_changed_callback(zego_on_video_device_state_changed callback_func, void * user_context);


/**
 * The local captured audio sound level callback.
 *
 * To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
 * The callback notification period is the setting parameter of [startSoundLevelMonitor].
 *
 * @param sound_level Locally captured sound level value, ranging from 0.0 to 100.0
 * @param user_context Context of user.
 */
typedef void(*zego_on_captured_sound_level_update)(const struct zego_sound_level_info* sound_level, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_captured_sound_level_update_callback(zego_on_captured_sound_level_update callback_func, void * user_context);


/**
 * The remote playing streams audio sound level callback.
 *
 * To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is playing the audio and video stream.
 * The callback notification period is the setting parameter of [startSoundLevelMonitor].
 *
 * @param sound_levels Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0
 * @param info_count Count of sound level info.
 * @param user_context Context of user.
 */
typedef void(*zego_on_remote_sound_level_update)(const struct zego_sound_level_info* sound_levels, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_remote_sound_level_update_callback(zego_on_remote_sound_level_update callback_func, void * user_context);


/**
 * The local captured audio spectrum callback.
 *
 * To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
 * The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
 *
 * @param audio_spectrum Locally captured audio spectrum value list. Spectrum value range is [0-2^30]
 * @param user_context Context of user.
 */
typedef void(*zego_on_captured_audio_spectrum_update)(const struct zego_audio_spectrum_info* audio_spectrum, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_captured_audio_spectrum_update_callback(zego_on_captured_audio_spectrum_update callback_func, void * user_context);


/**
 * The remote playing streams audio spectrum callback.
 *
 * To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is playing the audio and video stream.
 * The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
 *
 * @param audio_spectrums Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
 * @param info_count Count of audio spectrum.
 * @param user_context Context of user.
 */
typedef void(*zego_on_remote_audio_spectrum_update)(const struct zego_audio_spectrum_info* audio_spectrums, unsigned int info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_remote_audio_spectrum_update_callback(zego_on_remote_audio_spectrum_update callback_func, void * user_context);


/**
 * The callback triggered when a device exception occurs.
 *
 * This callback is triggered when an exception occurs when reading or writing the audio and video device.
 *
 * @param error_code The error code corresponding to the status change of the playing stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param device_name device name
 * @param user_context Context of user.
 */
typedef void(*zego_on_device_error)(zego_error error_code, const char * device_name, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_device_error_callback(zego_on_device_error callback_func, void * user_context);


/**
 * The callback triggered when the state of the remote camera changes.
 *
 * When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
 * Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
 * This callback will not be called back when the remote stream is play from the CDN, and will not be called back if the remote stream end user has enabled custom video capture function.
 *
 * @param stream_id Stream ID
 * @param state Remote camera status
 * @param user_context Context of user.
 */
typedef void(*zego_on_remote_camera_state_update)(const char * stream_id, enum zego_remote_device_state state, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_remote_camera_state_update_callback(zego_on_remote_camera_state_update callback_func, void * user_context);


/**
 * The callback triggered when the state of the remote microphone changes.
 *
 * When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
 * Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
 * This callback will not be called back when the remote stream is play from the CDN, and will not be called back if the remote stream end user has enabled custom audio capture function.
 *
 * @param stream_id Stream ID
 * @param state Remote microphone status
 * @param user_context Context of user.
 */
typedef void(*zego_on_remote_mic_state_update)(const char * stream_id, enum zego_remote_device_state state, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_remote_mic_state_update_callback(zego_on_remote_mic_state_update callback_func, void * user_context);


/**
 * Callback for device's audio route changed.
 *
 * This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.
 *
 * @param audio_route Current audio route
 * @param user_context Context of user.
 */
typedef void(*zego_on_audio_route_change)(enum zego_audio_route audio_route, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_audio_route_change_callback(zego_on_audio_route_change callback_func, void * user_context);


/**
 * Whether to use the built-in speaker to play audio.This function has been deprecated since version 2.3.0. Please use [setAudioRouteToSpeaker] instead.
 *
 * When you choose not to use the built-in speaker to play sound, that is, set to false, the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
 *
 * @deprecated This function has been deprecated since version 2.3.0. Please use [setAudioRouteToSpeaker] instead.
 * @param enable Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_built_in_speaker_on(bool enable);



ZEGO_END_DECLS

#endif
