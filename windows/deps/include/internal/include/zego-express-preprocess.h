
#ifndef __ZEGO_EXPRESS_PREPROCESS_H__
#define __ZEGO_EXPRESS_PREPROCESS_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Enables or disables acoustic echo cancellation (AEC).
 *
 * Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param enable Whether to enable echo cancellation, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_aec(bool enable);


/**
 * Whether to turn on acoustic echo cancellation (AEC) when using the headphone.
 *
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param enable Whether to enable, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_headphone_aec(bool enable);


/**
 * Sets the acoustic echo cancellation (AEC) mode.
 *
 * Switch different echo cancellation modes to control the extent to which echo data is eliminated.
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param mode Echo cancellation mode
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_aec_mode(enum zego_aec_mode mode);


/**
 * Enables or disables automatic gain control (AGC).
 *
 * When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent.
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param enable Whether to enable automatic gain control, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_agc(bool enable);


/**
 * Enables or disables active noise suppression (ANS, aka ANC).
 *
 * Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer.
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param enable Whether to enable noise suppression, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_ans(bool enable);


/**
 * Enables or disables transient noise suppression.
 *
 * Suppress transient noises such as keyboard and desk knocks
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param enable Whether to enable transient noise suppression, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_transient_ans(bool enable);


/**
 * Sets the automatic noise suppression (ANS) mode.
 *
 * Default is medium mode
 * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
 *
 * @param mode Audio Noise Suppression mode
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_ans_mode(enum zego_ans_mode mode);


/**
 * Enables or disables audio mixing.
 *
 * Enable audio mixing, work with setAudioMixingHandler providing the audio data for mixing
 *
 * @param enable Whether to enable audio mixting, true: enable, false: disable
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_audio_mixing(bool enable);


/**
 * Stops or resumes playing the mixed audio locally.
 *
 * When stop play audio mixing locally, the audio will not be heard on the mix side (publisher side) while still be heard on the remote side (player side).
 *
 * @param mute Whether to mute local audio mixting, true: mute, false: unmute
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_mute_local_audio_mixing(bool mute);


/**
 * Sets the audio mixing output volume for both local playback and the stream to be published.
 *
 * This function will modify the volume of the local playback and the mixed data that is mixed into the publishing stream at the same time.
 *
 * @param volume The audio mixing volume, range from 0 to 200, 100 as default
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_mixing_volume(int volume);


/**
 * Sets the audio mixing output volume for either local playback or the stream to published.
 *
 * This function can individually set the mixing volume of local playback or the mixing volume of the publishing stream.
 *
 * @param volume The audio mixing volume, range from 0 to 200, 100 as default
 * @param type volume local playback / volume in stream published
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_mixing_volume_with_type(int volume, enum zego_volume_type type);


/**
 * Enables or disables the beauty features (for the specified channel).
 *
 * The current beauty function is simple and may not meet the developer's expectations, it is recommended to use [enableCustomVideoCapture] function to connect to a third party professional beauty SDK to get the best results.
 * The [setBeautifyOption] function can be called to adjust the beauty parameters after the beauty function is enabled.
 * In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
 *
 * @param feature_bit_mask Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_beautify(int feature_bit_mask, enum zego_publish_channel channel);


/**
 * Sets up the beauty parameters (for the specified channel).
 *
 * Developer need to call [enableBeautify] function first to enable the beautify function before calling this function
 * In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, this function is no longer valid.
 *
 * @param option Beauty configuration options
 * @param channel Publishing stream channel
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_beautify_option(struct zego_beautify_option option, enum zego_publish_channel channel);


/**
 * Set the sound equalizer (EQ).
 *
 * @param band_index Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
 * @param band_gain Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_audio_equalizer_gain(int band_index, float band_gain);


/**
 * Setting up the voice changer via preset enumeration.
 *
 * Voice changer effect is only effective for the captured sound.
 * This function is an encapsulated version of [setVoiceChangerParam], which provides some preset values. If you need to configure the voice changer effects, please use [setVoiceChangerParam]
 * This function is mutually exclusive with [setReverbPreset]. If used at the same time, it will produce undefined effects.
 * Some enumerated preset will modify the parameters of reverberation or reverberation echo, so after calling this function, calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect.
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @param preset The voice changer preset enumeration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_voice_changer_preset(enum zego_voice_changer_preset preset);


/**
 * Setting up the specific voice changer parameters.
 *
 * Voice changer effect is only effective for the captured sound.
 * This function is an advanced version of [setVoiceChangerPreset], you can configure the voice changer effect by yourself.
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @param param Voice changer parameters
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_voice_changer_param(float param);


/**
 * Setting up the reverberation via preset enumeration.
 *
 * Support dynamic settings when publishing stream.
 * This function is a encapsulated version of [setReverbAdvancedParam], which provides some preset values. If you need to configure the reverb, please use [setReverbAdvancedParam]
 * This function is mutually exclusive with [setVoiceChangerPreset]. If used at the same time, it will produce undefined effects.
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @param preset The reverberation preset enumeration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_reverb_preset(enum zego_reverb_preset preset);


/**
 * Setting up the specific reverberation parameters.
 *
 * Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
 * This function is an advanced version of [setReverbPreset], you can configure the reverb effect by yourself.
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @param param Reverb advanced parameter
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_reverb_advanced_param(struct zego_reverb_advanced_param param);


/**
 * Setting up the specific reverberation echo parameters.
 *
 * This function can be used with voice changer and reverb to achieve a variety of custom sound effects
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @param param The reverberation echo parameter
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_reverb_echo_param(struct zego_reverb_echo_param param);


/**
 * Enables the virtual stereo feature.
 *
 * Note: You need to set up a dual channel setAudioConfig for the virtual stereo to take effect!
 *
 * @param enable true to turn on the virtual stereo, false to turn off the virtual stereo
 * @param angle angle of the sound source in the virtual stereo, ranging from 0 to 180, with 90 being the front, and 0 and 180 being respectively Corresponds to rightmost and leftmost, usually use 90.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_enable_virtual_stereo(bool enable, int angle);


/**
 * The callback for copying audio data to the SDK for audio mixing. This function should be used together with enableAudioMixing.
 *
 * Supports 16k 32k 44.1k 48k sample rate, mono or dual channel, 16-bit deep PCM audio data
 * This callback is a high frequency callback. To ensure the quality of the mixing data, please do not handle time-consuming operations in this callback
 *
 * @param data audio mixing data
 * @param user_context Context of user.
 */
typedef void(*zego_on_copy_audio_mixing_data)(struct zego_audio_mixing_data * data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_copy_audio_mixing_data_callback(zego_on_copy_audio_mixing_data callback_func, void * user_context);


/**
 * Setting up the specific reverberation parameters.
 *
 * Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
 * This function is an advanced version of [setReverbPreset], you can configure the reverb effect by yourself.
 * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
 *
 * @deprecated This function has been deprecated since version 1.18.0, please use [setReverbAdvancedParam] instead.
 * @param param Reverb parameter
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_reverb_param(struct zego_reverb_param param);



ZEGO_END_DECLS

#endif
