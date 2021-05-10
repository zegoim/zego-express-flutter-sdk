
#ifndef __ZEGO_EXPRESS_ENGINE_H__
#define __ZEGO_EXPRESS_ENGINE_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Creates a singleton instance of ZegoExpressEngine.
 *
 * The engine needs to be created and initialized before calling other functions. The SDK only supports the creation of one instance of ZegoExpressEngine. Multiple calls to this function return the same object.
 *
 * @param app_id Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console-express.zego.im The value ranges from 0 to 4294967295.
 * @param app_sign Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'.
 * @param is_test_env Choose to use a test environment or a formal commercial environment, the formal environment needs to submit work order configuration in the ZEGO management console. The test environment is for test development, with a limit of 10 rooms and 50 users. Official environment App is officially launched. ZEGO will provide corresponding server resources according to the configuration records submitted by the developer in the management console. The test environment and the official environment are two sets of environments and cannot be interconnected.
 * @param scenario The application scenario. Developers can choose one of ZegoScenario based on the scenario of the app they are developing, and the engine will preset a more general setting for specific scenarios based on the set scenario. After setting specific scenarios, developers can still call specific functions to set specific parameters if they have customized parameter settings.
 * @return Zego error code
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_engine_init(unsigned int app_id, const char * app_sign, bool is_test_env, enum zego_scenario scenario);


/**
 * Destroys the singleton instance of ZegoExpressEngine asynchronously.
 *
 * Used to release resources used by ZegoExpressEngine.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_engine_uninit_async(void);


/**
 * Set advanced engine configuration.
 *
 * Developers need to call this function to set advanced function configuration when they need advanced functions of the engine.
 *
 * @param config Advanced engine configuration
 */
ZEGOEXP_API void EXP_CALL zego_express_set_engine_config(struct zego_engine_config config);


/**
 * Set log configuration.
 *
 * When developers need to customize the log file size and path, they need to call this function to complete the configuration.It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].Once this interface is called, the old way of setting log size and path through [setEngineConfig] will be invalid before destroyEngine, that is, during the entire life cycle of the engine.It is recommended that once you use this method, you always only use this method to complete the requirements for setting the log path and size.
 *
 * @param config log configuration
 */
ZEGOEXP_API void EXP_CALL zego_express_set_log_config(struct zego_log_config config);


/**
 * Gets the SDK's version number.
 *
 * When the SDK is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
 * Developers can also collect this information as the version information of the engine used by the app, so that the SDK corresponding to each version of the app on the line.
 *
 * @return SDK version
 */
ZEGOEXP_API const char * EXP_CALL zego_express_get_version();


/**
 * Sets the JVM and Context for the Android platform. It must be called before calling [createEngine] on the Android platform.
 *
 * @param jvm Java VM Object
 * @param context Android Context, it must be valid in the SDK lifecycle.
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_set_android_env(void* jvm, void* context);


/**
 * Get the previously set Android context.
 *
 * @return If [setAndroidEnv] has not been called before [createEngine], or if it is not called on the Android platform, it returns NULL.
 */
ZEGOEXP_API void * EXP_CALL zego_express_get_android_context();


/**
 * Uploads logs to the ZEGO server.
 *
 * By default, SDK creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
 * Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
 * The function is valid for the entire life cycle of the SDK.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_upload_log();


/**
 * Turns on/off verbose debugging and sets up the log language.
 *
 * The debug switch is set to on and the language is English by default.
 *
 * @deprecated This method has been deprecated after version 2.3.0, please use the [setEngineConfig] function to set the advanced configuration property advancedConfig to achieve the original function.
 * @param enable Detailed debugging information switch
 * @param language Debugging information language
 */
ZEGOEXP_API void EXP_CALL zego_express_set_debug_verbose(bool enable, enum zego_language language);


/**
 * Call the RTC experimental API
 *
 * ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support
 *
 * @param params You need to pass in a parameter in the form of a JSON string
 * @return Returns an argument in the format of a JSON string
 */
ZEGOEXP_API const char * EXP_CALL zego_express_call_experimental_api(const char * params);


/**
 * The callback for obtaining debugging error information.
 *
 * When the SDK functions are not used correctly, the callback prompts for detailed error information, which is controlled by the [setDebugVerbose] function
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param func_name Function name
 * @param info Detailed error information
 * @param user_context context of user
 */
typedef void(*zego_on_debug_error)(int error_code, const char * func_name, const char * info, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_debug_error_callback(zego_on_debug_error callback_func, void * user_context);


/**
 * Method execution result callback
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param func_name Function name
 * @param info Detailed error information
 * @param user_context context of user
 */
typedef void(*zego_on_api_called_result)(int error_code, const char * func_name, const char * info, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_api_called_result_callback(zego_on_api_called_result callback_func, void * user_context);


/**
 * The callback triggered when the audio/video engine state changes.
 *
 * When the developer calls the function that enables audio and video related functions, such as calling [startPreview], [startPublishingStream], [startPlayingStream] and MediaPlayer related function, the audio/video engine will start; when all audio and video functions are stopped, the engine state will become stopped.
 * When the developer has been [loginRoom], once [logoutRoom] is called, the audio/video engine will stop (preview, publishing/playing stream, MediaPlayer and other audio and video related functions will also stop).
 *
 * @param state The audio/video engine state
 * @param user_context context of user
 */
typedef void(*zego_on_engine_state_update)(enum zego_engine_state state, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_engine_state_update_callback(zego_on_engine_state_update callback_func, void * user_context);


/**
 * Audio and video engine destruction notification callback.
 *
 * When you use the asynchronous destruction engine function, you can obtain whether the SDK has completely released resources by listening to this callback
 *
 * @param user_context context of user
 */
typedef void(*zego_on_engine_uninit)(void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_engine_uninit_callback(zego_on_engine_uninit callback_func, void * user_context);


/**
 * Log upload result callback.
 *
 * @param seq Sequence.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param user_context Context of user.
 */
typedef void(*zego_on_upload_log_result)(zego_seq seq, zego_error error_code, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_upload_log_result_callback(zego_on_upload_log_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
