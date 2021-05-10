

#ifndef __ZEGOEXPRESSERRORCODE_H__
#define __ZEGOEXPRESSERRORCODE_H__

namespace ZEGO {
    namespace EXPRESS {

        enum ZegoErrorCode
        {
            /** Execution successful. */
            ZEGO_ERROR_CODE_COMMON_SUCCESS                                         = 0,

            /** Engine not yet created. Please create the engine first before calling non-static functions. */
            ZEGO_ERROR_CODE_COMMON_ENGINE_NOT_CREATE                               = 1000001,

            /** Not yet logged in to any room. Please log in to a room before publishing or playing streams. */
            ZEGO_ERROR_CODE_COMMON_NOT_LOGIN_ROOM                                  = 1000002,

            /** Engine not yet started. Please call `startPreviewView`, `startPublishing`, or `startPlayingStream` to get the engine started first. */
            ZEGO_ERROR_CODE_COMMON_ENGINE_NOT_STARTED                              = 1000003,

            /** An API not supported by the current platform is called; for example, an API that sets the Android context is called on a non-Android platform. */
            ZEGO_ERROR_CODE_COMMON_UNSUPPORTED_PLATFORM                            = 1000006,

            /** Invalid Android context. */
            ZEGO_ERROR_CODE_COMMON_INVALID_ANDROID_ENVIRONMENT                     = 1000007,

            /** The event handler has already been set by calling `setEventHandler`. Please do not repeat the setting. If you do need to set up the event handler again, please call `setEventHandler` to set it to null first before applying the new setting. */
            ZEGO_ERROR_CODE_COMMON_EVENT_HANDLER_EXISTS                            = 1000008,

            /** This feature is not included in the SDK. Please contact ZEGO technical support. */
            ZEGO_ERROR_CODE_COMMON_SDK_NO_MODULE                                   = 1000010,

            /** The input stream ID is too long. The maximum length should be less than 256 bytes. */
            ZEGO_ERROR_CODE_COMMON_STREAM_ID_TOO_LONG                              = 1000014,

            /** The input stream ID is empty. */
            ZEGO_ERROR_CODE_COMMON_STREAM_ID_NULL                                  = 1000015,

            /** The input stream ID contains invalid characters. */
            ZEGO_ERROR_CODE_COMMON_STREAM_ID_INVALID_CHARACTER                     = 1000016,

            /** This AppID has been removed from production. */
            ZEGO_ERROR_CODE_COMMON_APP_OFFLINE_ERROR                               = 1000037,

            /** There is an error in the backend configurations. Please check the configuration items of the app. */
            ZEGO_ERROR_CODE_COMMON_APP_FLEXIABLE_CONFIG_ERROR                      = 1000038,

            /** Incorrect CDN address. Please check the supported protocol and format. */
            ZEGO_ERROR_CODE_COMMON_CDN_URL_INVALID                                 = 1000055,

            /** DNS resolution failed. Please check network configurations. */
            ZEGO_ERROR_CODE_COMMON_DNS_RESOLVE_ERROR                               = 1000060,

            /** Server dispatching exception. Please contact ZEGO technical support to solve the problem. */
            ZEGO_ERROR_CODE_COMMON_DISPATCH_ERROR                                  = 1000065,

            /** Internal null pointer error. Please contact ZEGO technical support to solve the problem. */
            ZEGO_ERROR_CODE_COMMON_INNER_NULLPTR                                   = 1000090,

            /** AppID cannot be 0. Please check if the AppID is correct. */
            ZEGO_ERROR_CODE_ENGINE_APPID_ZERO                                      = 1001000,

            /** The length of the input AppSign must be 64 bytes. */
            ZEGO_ERROR_CODE_ENGINE_APPSIGN_INVALID_LENGTH                          = 1001001,

            /** The input AppSign contains invalid characters. Only '0'-'9', 'a'-'f', 'A'-'F' are valid. */
            ZEGO_ERROR_CODE_ENGINE_APPSIGN_INVALID_CHARACTER                       = 1001002,

            /** The input AppSign is empty. */
            ZEGO_ERROR_CODE_ENGINE_APPSIGN_NULL                                    = 1001003,

            /** Authentication failed. Please check if the AppID is correct, or whether the production environment was selected for SDK initialization without prior go-live process. */
            ZEGO_ERROR_CODE_ENGINE_APPID_INCORRECT_OR_NOT_ONLINE                   = 1001004,

            /** AppSign authentication failed. Please check if the AppSign is correct. */
            ZEGO_ERROR_CODE_ENGINE_APPSIGN_INCORRECT                               = 1001005,

            /** No write permission to the log file. */
            ZEGO_ERROR_CODE_ENGINE_LOG_NO_WRITE_PERMISSION                         = 1001014,

            /** The log file path is too long. */
            ZEGO_ERROR_CODE_ENGINE_LOG_PATH_TOO_LONG                               = 1001015,

            /** The experimental API json parameter parsing failed. Please check the correctness of the json content. Such as the correctness of the json format, whether the method name or parameters are passed correctly, etc. */
            ZEGO_ERROR_CODE_ENGINE_EXPERIMENTAL_JSON_STR_INVALID                   = 1001091,

            /** The number of rooms the user attempted to log into simultaneously exceeds the maximum number allowed. Currently, a user can only be logged in to one main room and one multi room at the same time. */
            ZEGO_ERROR_CODE_ROOM_COUNT_EXCEED                                      = 1002001,

            /** The input room ID is incorrect, please check if this room ID is currently logged in. */
            ZEGO_ERROR_CODE_ROOM_ROOMID_INCORRECT                                  = 1002002,

            /** The input user ID is empty. */
            ZEGO_ERROR_CODE_ROOM_USER_ID_NULL                                      = 1002005,

            /** The input user ID contains invalid characters. */
            ZEGO_ERROR_CODE_ROOM_USER_ID_INVALID_CHARACTER                         = 1002006,

            /** The input user ID is too long. The maximum length should be less than 64 bytes. */
            ZEGO_ERROR_CODE_ROOM_USER_ID_TOO_LONG                                  = 1002007,

            /** The input user name is empty. */
            ZEGO_ERROR_CODE_ROOM_USER_NAME_NULL                                    = 1002008,

            /** The input user name contains invalid characters. */
            ZEGO_ERROR_CODE_ROOM_USER_NAME_INVALID_CHARACTER                       = 1002009,

            /** The input user name is too long. The maximum length should be less than 256 bytes. */
            ZEGO_ERROR_CODE_ROOM_USER_NAME_TOO_LONG                                = 1002010,

            /** The input room ID is empty. */
            ZEGO_ERROR_CODE_ROOM_ROOMID_NULL                                       = 1002011,

            /** The input room ID contains invalid characters. */
            ZEGO_ERROR_CODE_ROOM_ROOMID_INVALID_CHARACTER                          = 1002012,

            /** The input room ID is too long. The maximum length should be less than 128 bytes. */
            ZEGO_ERROR_CODE_ROOM_ROOMID_TOO_LONG                                   = 1002013,

            /** The key for room extra info is empty. */
            ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_KEY_EMPTY                         = 1002014,

            /** The key for room extra info is too long. The maximum length should be less than 128 bytes. */
            ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_KEY_TOO_LONG                      = 1002015,

            /** The value for room extra info is too long. The maximum length should be less than 4096 bytes. */
            ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_VALUE_TOO_LONG                    = 1002016,

            /** The set key of the room extra info exceeds the maximum number supported. If you need to modify the number of setting keys, please contact ZEGO technical support. */
            ZEGO_ERROR_CODE_ROOM_ROOM_EXTRA_INFO_EXCEED_KEYS                       = 1002017,

            /** Login failed, possibly due to network problems. */
            ZEGO_ERROR_CODE_ROOM_ERROR_CONNECT_FAILED                              = 1002030,

            /** Login timed out, possibly due to network problems. */
            ZEGO_ERROR_CODE_ROOM_ERROR_LOGIN_TIMEOUT                               = 1002031,

            /** Room login authentication failed. */
            ZEGO_ERROR_CODE_ROOM_ERROR_AUTHENTICATION_FAILED                       = 1002033,

            /** The number of users logging into the room exceeds the maximum number of concurrent users configured for the room. (In the test environment, the default maximum number of users in the room is 50) */
            ZEGO_ERROR_CODE_ROOM_ERROR_EXCEED_MAXIMUM_MEMBER                       = 1002034,

            /** The total number of rooms logged in at the same time exceeds the limit. (In the test environment, the maximum number of concurrent rooms is 10) */
            ZEGO_ERROR_CODE_ROOM_ERROR_EXCEED_MAXIMUM_ROOM_COUNT                   = 1002035,

            /** The user is kicked out of the room, possibly because the same user ID is logged in on another device. */
            ZEGO_ERROR_CODE_ROOM_KICKED_OUT                                        = 1002050,

            /** Room connection is temporarily interrupted, possibly due to network problems. Retrying... */
            ZEGO_ERROR_CODE_ROOM_CONNECT_TEMPORARY_BROKEN                          = 1002051,

            /** Room disconnected, possibly due to network problems. */
            ZEGO_ERROR_CODE_ROOM_DISCONNECT                                        = 1002052,

            /** Room login retry has exceeded the maximum retry time. */
            ZEGO_ERROR_CODE_ROOM_RETRY_TIMEOUT                                     = 1002053,

            /** The business server has sent a signal to kick the user out of the room. Please check the reason for the kick-out. */
            ZEGO_ERROR_CODE_ROOM_MANUAL_KICKED_OUT                                 = 1002055,

            /** You must log in to the main room with `loginRoom` before logging in to multi room */
            ZEGO_ERROR_CODE_ROOM_WRONG_LOGIN_SEQUENCE                              = 1002061,

            /** You must log out of the multi room before logging out of the main room */
            ZEGO_ERROR_CODE_ROOM_WRONG_LOGOUT_SEQUENCE                             = 1002062,

            /** No multi-room permission, please contact ZEGO technical support to enable it. */
            ZEGO_ERROR_CODE_ROOM_NO_MULTI_ROOM_PERMISSION                          = 1002063,

            /** Room ID has been used by other login room interface */
            ZEGO_ERROR_CODE_ROOM_ROOM_ID_HAS_BEEN_USED                             = 1002064,

            /** Room login failed due to internal system exceptions. */
            ZEGO_ERROR_CODE_ROOM_INNER_ERROR                                       = 1002099,

            /** Stream publishing failed, possibly due to no data in the stream. */
            ZEGO_ERROR_CODE_PUBLISHER_PUBLISH_STREAM_FAILED                        = 1003001,

            /** Incorrect bitrate setting. Please check if the bitrate value is in the correct unit (kbps). */
            ZEGO_ERROR_CODE_PUBLISHER_BITRATE_INVALID                              = 1003002,

            /** Incorrect setting of stream publishing traffic control parameters. */
            ZEGO_ERROR_CODE_PUBLISHER_TRAFFIC_MODE_INVALID                         = 1003005,

            /** Stream publishing is temporarily interrupted. Retrying... */
            ZEGO_ERROR_CODE_PUBLISHER_ERROR_NETWORK_INTERRUPT                      = 1003020,

            /** Stream publish retry has exceeds the maximum retry time. */
            ZEGO_ERROR_CODE_PUBLISHER_ERROR_RETRY_TIMEOUT                          = 1003021,

            /** Failed to publish the stream. The user is already publishing streams. */
            ZEGO_ERROR_CODE_PUBLISHER_ERROR_ALREADY_DO_PUBLISH                     = 1003023,

            /** Failed to publish the stream. Publishing of this stream is prohibited by backend configuration. */
            ZEGO_ERROR_CODE_PUBLISHER_ERROR_SERVER_FORBID                          = 1003025,

            /** Failed to publish the stream. The same stream already exists in the room. */
            ZEGO_ERROR_CODE_PUBLISHER_ERROR_REPETITIVE_PUBLISH_STREAM              = 1003028,

            /** The connection to the RTMP server is interrupted. Please check whether there is any problem with the network connection or the stream publishing URL. */
            ZEGO_ERROR_CODE_PUBLISHER_RTMP_SERVER_DISCONNECT                       = 1003029,

            /** Failed to take publish stream snapshot, please check whether the state of the publish channel to be snapshot is normal. */
            ZEGO_ERROR_CODE_PUBLISHER_TAKE_PUBLISH_STREAM_SNAPSHOT_FAILED          = 1003030,

            /** Failed to get status updates of relayed streaming to CDN. Please check whether the URL is valid. */
            ZEGO_ERROR_CODE_PUBLISHER_UPDATE_CDN_TARGET_ERROR                      = 1003040,

            /** Failed to send SEI. The SEI data is null. */
            ZEGO_ERROR_CODE_PUBLISHER_SEI_DATA_NULL                                = 1003043,

            /** Failed to send SEI because the SEI data is too long. The maximum length should be less than 4096 bytes. */
            ZEGO_ERROR_CODE_PUBLISHER_SEI_DATA_TOO_LONG                            = 1003044,

            /** The extra info of the stream is null. */
            ZEGO_ERROR_CODE_PUBLISHER_EXTRA_INFO_NULL                              = 1003050,

            /** The extra info of the stream is too long. The maximum length should be less than 1024 bytes. */
            ZEGO_ERROR_CODE_PUBLISHER_EXTRA_INFO_TOO_LONG                          = 1003051,

            /** Failed to update the extra info of the stream. Please check the network connection. */
            ZEGO_ERROR_CODE_PUBLISHER_UPDATE_EXTRA_INFO_FAILED                     = 1003053,

            /** The watermark URL is null. */
            ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_NULL                           = 1003055,

            /** The watermark URL is too long. The maximum length should be less than 1024 bytes. */
            ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_TOO_LONG                       = 1003056,

            /** Invalid watermark format. The supported formats are `jpg` and `png`. */
            ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_URL_INVALID                        = 1003057,

            /** Incorrect watermark layout. The layout area cannot exceed the encoding resolution. */
            ZEGO_ERROR_CODE_PUBLISHER_WATERMARK_LAYOUT_INVALID                     = 1003058,

            /** The publish stream encryption key is invalid, the key length only supports 16/24/32 bytes. */
            ZEGO_ERROR_CODE_PUBLISHER_ENCRYPTION_KEY_INVALID                       = 1003060,

            /** Stream publishing failed due to system internal exceptions. */
            ZEGO_ERROR_CODE_PUBLISHER_INNER_ERROR                                  = 1003099,

            /** Stream playing failed, possibly due to no data in the stream. */
            ZEGO_ERROR_CODE_PLAYER_PLAY_STREAM_FAILED                              = 1004001,

            /** Stream playing failed because the stream does not exist. Please check whether the remote end publish is indeed successful, or whether the publish and play environment are inconsistent */
            ZEGO_ERROR_CODE_PLAYER_PLAY_STREAM_NOT_EXIST                           = 1004002,

            /** The number of streams the user attempted to play simultaneously exceeds the maximum number allowed. Currently, up to 12 steams can be played at the same time. Please contact ZEGO technical support to increase the capacity if necessary. */
            ZEGO_ERROR_CODE_PLAYER_COUNT_EXCEED                                    = 1004010,

            /** Stream playing is temporarily interrupted. Retrying... */
            ZEGO_ERROR_CODE_PLAYER_ERROR_NETWORK_INTERRUPT                         = 1004020,

            /** Failed to take play stream snapshot, please check whether the state of the stream to be snapshot is normal. */
            ZEGO_ERROR_CODE_PLAYER_TAKE_PLAY_STREAM_SNAPSHOT_FAILED                = 1004030,

            /** The play stream decryption key is invalid, the key length only supports 16/24/32 bytes. */
            ZEGO_ERROR_CODE_PLAYER_DECRYPTION_KEY_INVALID                          = 1004060,

            /** Decrypt the play stream failed, please check whether the decryption key is correct */
            ZEGO_ERROR_CODE_PLAYER_DECRYPTION_FAILED                               = 1004061,

            /** Stream playing failed due to system internal exceptions. */
            ZEGO_ERROR_CODE_PLAYER_INNER_ERROR                                     = 1004099,

            /** Stream mixing service not yet enabled. Please contact ZEGO technical support to enable the service. */
            ZEGO_ERROR_CODE_MIXER_NO_SERVICES                                      = 1005000,

            /** The stream mixing task ID is null. */
            ZEGO_ERROR_CODE_MIXER_TASK_ID_NULL                                     = 1005001,

            /** The stream mixing task ID is too long. The maximum length should be less than 256 bytes. */
            ZEGO_ERROR_CODE_MIXER_TASK_ID_TOO_LONG                                 = 1005002,

            /** The stream mixing task ID contains invalid characters. */
            ZEGO_ERROR_CODE_MIXER_TASK_ID_INVALID_CHARACTER                        = 1005003,

            /** No output is specified in the configuration of the stream mixing task. */
            ZEGO_ERROR_CODE_MIXER_NO_OUTPUT_TARGET                                 = 1005005,

            /** Incorrect stream mixing output. Please check if the streamID of the output target contains invalid characters. */
            ZEGO_ERROR_CODE_MIXER_OUTPUT_TARGET_INVALID                            = 1005006,

            /** Failed to start the stream mixing task, possibly due to network problems. */
            ZEGO_ERROR_CODE_MIXER_START_REQUEST_ERROR                              = 1005010,

            /** Failed to stop the stream mixing task, possibly due to network problems. */
            ZEGO_ERROR_CODE_MIXER_STOP_REQUEST_ERROR                               = 1005011,

            /** The stream mixing task must be stopped by the user who started the task. */
            ZEGO_ERROR_CODE_MIXER_NOT_OWNER_STOP_MIXER                             = 1005012,

            /** Starts stream mixing tasks too frequently. */
            ZEGO_ERROR_CODE_MIXER_START_QPS_OVERLOAD                               = 1005015,

            /** Stops stream mixing tasks too frequently. */
            ZEGO_ERROR_CODE_MIXER_STOP_QPS_OVERLOAD                                = 1005016,

            /** The input stream list of the stream mixing task is null. */
            ZEGO_ERROR_CODE_MIXER_INPUT_LIST_INVALID                               = 1005020,

            /** The output stream list of the stream mixing task is null. */
            ZEGO_ERROR_CODE_MIXER_OUTPUT_LIST_INVALID                              = 1005021,

            /** The video configuration of the stream mixing task is invalid. */
            ZEGO_ERROR_CODE_MIXER_VIDEO_CONFIG_INVALID                             = 1005023,

            /** The audio configuration of the stream mixing task is invalid. Please check if an unsupported codec is used. */
            ZEGO_ERROR_CODE_MIXER_AUDIO_CONFIG_INVALID                             = 1005024,

            /** The number of input streams exceeds the maximum number allowed. Up to 9 input streams can be specified. */
            ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_INPUT_COUNT                           = 1005025,

            /** The input stream does not exist. */
            ZEGO_ERROR_CODE_MIXER_INPUT_STREAM_NOT_EXISTS                          = 1005026,

            /** Invalid stream mixing input parameters. It may be that the layout of the input streams exceeds the canvas. */
            ZEGO_ERROR_CODE_MIXER_INPUT_PARAMETERS_ERROR                           = 1005027,

            /** The number of output streams exceeds the maximum number allowed. Up to 3 output streams can be specified. */
            ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_OUTPUT_COUNT                          = 1005030,

            /** Exceed the maximum number of focus voice input streams, and support up to 4 input streams to set focus voice */
            ZEGO_ERROR_CODE_MIXER_EXCEED_MAX_AUDIO_FOCUS_STREAM_COUNT              = 1005031,

            /** Stream mixing authentication failed. */
            ZEGO_ERROR_CODE_MIXER_AUTHENTICATION_FAILED                            = 1005050,

            /** The input watermark is null. */
            ZEGO_ERROR_CODE_MIXER_WATERMARK_NULL                                   = 1005061,

            /** Invalid watermark parameter. It may be that the layout of the watermark exceeds the canvas. */
            ZEGO_ERROR_CODE_MIXER_WATERMARK_PARAMETERS_ERROR                       = 1005062,

            /** Invalid watermark URL. The URL must start with `preset-id://`, and must end with `.jpg` or `.png`. */
            ZEGO_ERROR_CODE_MIXER_WATERMARK_URL_INVALID                            = 1005063,

            /** Invalid background image URL. The URL must start with `preset-id://`, and must end with `.jpg` or `.png`. */
            ZEGO_ERROR_CODE_MIXER_BACKGROUND_IMAGE_URL_INVALID                     = 1005067,

            /** The server for auto stream mixing is not found. Please contact ZEGO technical support to enable it. */
            ZEGO_ERROR_CODE_MIXER_AUTO_MIX_STREAM_SERVER_NOT_FOUND                 = 1005070,

            /** Stream mixing internal error. */
            ZEGO_ERROR_CODE_MIXER_INNER_ERROR                                      = 1005099,

            /** Generic device error. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_GENERIC                              = 1006001,

            /** The device ID does not exist. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_INVALID_ID                           = 1006002,

            /** No permission to access the device. Please check the permissions of the camera or microphone. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_NO_AUTHORIZATION                     = 1006003,

            /** The frame rate of the capture device is 0. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_ZERO_FPS                             = 1006004,

            /** The device is occupied. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_IN_USE_BY_OTHER                      = 1006005,

            /** The device is unplugged. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_UNPLUGGED                            = 1006006,

            /** The device needs to be restarted. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_TYPE_REBOOT_REQUIRED                      = 1006007,

            /** The device media is lost. */
            ZEGO_ERROR_CODE_DEVICE_ERROR_MEDIA_SERVICES_LOST                       = 1006008,

            /** The device list cannot be empty when trying to release devices. */
            ZEGO_ERROR_CODE_DEVICE_FREE_DEVICE_LIST_NULL                           = 1006020,

            /** The set sound level monitoring interval is out of range. */
            ZEGO_ERROR_CODE_DEVICE_SOULD_LEVEL_INTERVAL_INVALID                    = 1006031,

            /** The set audio spectrum monitoring interval is out of range. */
            ZEGO_ERROR_CODE_DEVICE_AUDIO_SPECTRUM_INTERVAL_INVALID                 = 1006032,

            /** The set camera zoom factor is out of range. */
            ZEGO_ERROR_CODE_DEVICE_ZOOM_FACTOR_INVALID                             = 1006040,

            /** Device internal error. */
            ZEGO_ERROR_CODE_DEVICE_INNER_ERROR                                     = 1006099,

            /** Unknown error in the preprocessing module. Please contact ZEGO technical support. */
            ZEGO_ERROR_CODE_PREPROCESS_PREPROCESS_UNKNOWN_ERROR                    = 1007001,

            /** Invalid beauty option. Please check the input parameters. */
            ZEGO_ERROR_CODE_PREPROCESS_BEAUTIFY_OPTION_INVALID                     = 1007005,

            /** The reverberation parameter is null. Please check the input parameter. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_NULL                           = 1007006,

            /** The voice changer parameter is null. Please check the input parameter. */
            ZEGO_ERROR_CODE_PREPROCESS_VOICE_CHANGER_PARAM_NULL                    = 1007007,

            /** The room size value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 1.0. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_ROOM_SIZE_INVALID              = 1007011,

            /** The reverberance value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 0.5. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_REVERBERANCE_INVALID           = 1007012,

            /** The damping value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 2.0. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_DAMPING_INVALID                = 1007013,

            /** The dry_wet_ratio value of the reverberation parameters is invalid. The value should be greater than 0.0. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_PARAM_DRY_WET_RATIO_INVALID          = 1007014,

            /** The angle value of the virtual stereo parameters is invalid. The value should be in the range of 0 ~ 180. */
            ZEGO_ERROR_CODE_PREPROCESS_VIRTUAL_STEREO_ANGLE_INVALID                = 1007015,

            /** The voice changer param is invalid. The value should be in the range of -8.0 ~ 8.0. */
            ZEGO_ERROR_CODE_PREPROCESS_VOICE_CHANGER_PARAM_INVALID                 = 1007016,

            /** The reverberation echo parameters is null. Please check the input parameter. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_ECHO_PARAM_NULL                      = 1007017,

            /** The reverberation echo parameters is invalid. */
            ZEGO_ERROR_CODE_PREPROCESS_REVERB_ECHO_PARAM_INVALID                   = 1007018,

            /** The MediaPlayer instance is not created. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_NO_INSTANCE                               = 1008001,

            /** The MediaPlayer failed to play the media. The resource file is not loaded. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_NO_FILE_PATH                              = 1008003,

            /** The MediaPlayer failed to load the file. The file format is not supported. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_FORMAT_ERROR                         = 1008005,

            /** The MediaPlayer failed to load the file. The file path does not exist. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_PATH_NOT_EXISTS                      = 1008006,

            /** The MediaPlayer failed to load the file due to decoding errors. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_DECODE_ERROR                         = 1008007,

            /** The MediaPlayer failed to load files. No supported audio/video stream exists. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_FILE_NO_SUPPORTED_STREAM                  = 1008008,

            /** The MediaPlayer failed to play the file due to demuxing errors. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_DEMUX_ERROR                               = 1008010,

            /** The MediaPlayer failed to seek, possibly because the file hasn't been loaded yet. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_SEEK_ERROR                                = 1008016,

            /** The MediaPlayer is configured with a video data format not supported by the platform (e.g., CVPixelBuffer on iOS does not support NV21). */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_PLATFORM_FORMAT_NOT_SUPPORTED             = 1008020,

            /** The number of MediaPlayer instances exceeds the maximum number allowed. Up to 4 instances can be created. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_EXCEED_MAX_COUNT                          = 1008030,

            /** The media player failed to specify the audio track index */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_SET_AUDIO_TRACK_INDEX_ERROR               = 1008040,

            /** Invalid voice changing parameters set by media player */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_SET_VOICE_CHANGER_PARAM_INVALID           = 1008041,

            /** To make `takeSnapshot` effective, you need to ensure that the video is playing and `setPlayerCanvas` has been called to display the video on the control */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_TAKE_SNAPSHOT_TIMING_ERROR                = 1008042,

            /** The input parameter is not within the legal value range. Please check the interface notes and input a value within the legal value range. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_PARAM_VALUE_RANGE_ILLEGAL                 = 1008043,

            /** MediaPlayer internal error. */
            ZEGO_ERROR_CODE_MEDIA_PLAYER_INNER_ERROR                               = 1008099,

            /** The input message content is empty. */
            ZEGO_ERROR_CODE_IM_CONTENT_NULL                                        = 1009001,

            /** The input message content is too long. The maximum length should be less than 1024 bytes. */
            ZEGO_ERROR_CODE_IM_CONTENT_TOO_LONG                                    = 1009002,

            /** The room where the message is sent is different from the room currently logged in */
            ZEGO_ERROR_CODE_IM_INCONSISTENT_ROOM_ID                                = 1009005,

            /** Failed to send the message, possibly due to network problems. */
            ZEGO_ERROR_CODE_IM_SEND_FAILED                                         = 1009010,

            /** Failed to send broadcast message, QPS exceeds the limit, the maximum QPS is 2 */
            ZEGO_ERROR_CODE_IM_BROADCAST_MESSAGE_QPS_OVERLOAD                      = 1009015,

            /** The file name suffix is not supported. Only .mp4/.aac/.flv are supported currently. Depending on file name suffix, SDK sets the specified recording format accordingly. */
            ZEGO_ERROR_CODE_RECORDER_FILE_SUFFIX_NAME_FORMAT_NOT_SUPPORT           = 1010002,

            /** Generic error of recording API, generally due to invalid input parameters. */
            ZEGO_ERROR_CODE_RECORDER_COMMON_LIVEROOM_API_ERROR                     = 1010003,

            /** The specified recorded file path is too long. The maximum length should be less than 1024 bytes. */
            ZEGO_ERROR_CODE_RECORDER_FILE_PATH_TOO_LONG                            = 1010011,

            /** SDK internal VE error. Please contact ZEGO technical support to solve the problem. */
            ZEGO_ERROR_CODE_RECORDER_INNER_VE_ERROR                                = 1010012,

            /** Failed to open the file. Invalid file path or no permissions to the file. */
            ZEGO_ERROR_CODE_RECORDER_OPEN_FILE_FAILED                              = 1010013,

            /** Failed to write to the file, possibly due to no write permission to the file. */
            ZEGO_ERROR_CODE_RECORDER_WRITE_FILE_ERROR                              = 1010014,

            /** Not enough spare capacity. */
            ZEGO_ERROR_CODE_RECORDER_NO_ENOUGH_SPARE_CAPACITY                      = 1010017,

            /** File handle exceptions. */
            ZEGO_ERROR_CODE_RECORDER_FILE_HANDLE_EXCEPTIONS                        = 1010018,

            /** I/O exceptions. */
            ZEGO_ERROR_CODE_RECORDER_IO_EXCEPTIONS                                 = 1010019,

            /** The custom video capturer is not created. Please make sure to use it only after the `onStart` callback is received. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_CAPTURER_NOT_CREATED                   = 1011001,

            /** The custom video capture module is not enabled. Please make sure it is enabled in the initialization configurations. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NO_CUSTOM_VIDEO_CAPTURE                = 1011002,

            /** Failed to enable/disable custom video capture/rendering. Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`). */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_ENABLE_CUSTOM_IO_FAILED                = 1011003,

            /** The custom video capturer is not created. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_PROCESS_MODULE_NOT_CREATED             = 1011004,

            /** The custom video process module is not enabled. Please make sure that is called [enableCustomVideoProcessing]. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NO_CUSTOM_VIDEO_PROCESSING             = 1011005,

            /** The currently configured custom video capture format does not support this API. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NOT_SUPPORTED_FORMAT                   = 1011010,

            /** Custom video rendering does not support the currently set video buffer type. */
            ZEGO_ERROR_CODE_CUSTOM_VIDEO_IO_NOT_SUPPORTED_BUFFER_TYPE              = 1011011,

            /** Unsupported custom audio source type. */
            ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_UNSUPPORTED_AUDIO_SOURCE_TYPE          = 1012001,

            /** The custom audio capture feature is not enabled. Please make sure that the custom audio IO module is enabled for the specified stream publishing channel. */
            ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_CAPTURER_NOT_CREATED                   = 1012002,

            /** The custom audio rendering feature is not enabled. Please make sure that the custom audio IO module is enabled. */
            ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_RENDERER_NOT_CREATED                   = 1012003,

            /** Failed to enable/disable custom audio IO. Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`). */
            ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_ENABLE_CUSTOM_AUDIO_IO_FAILED          = 1012004,

            /** The sample rate parameter is illegal, please confirm whether the sample rate parameter value allowed by the interface is legal */
            ZEGO_ERROR_CODE_CUSTOM_AUDIO_IO_AUDIO_DATA_CALLBACK_SAMPLE_RATE_NO_SUPPORT = 1012010,

            /** The MediaDataPublisher instance is not created. */
            ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_NO_INSTANCE                       = 1013000,

            /** File error, failed to open */
            ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_PARSE_ERROR                  = 1013001,

            /** File path error */
            ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_PATH_ERROR                   = 1013002,

            /** File decoding exception */
            ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_FILE_CODEC_ERROR                  = 1013003,

            /** Timestamp error (the later frame timestamp is smaller than the previous frame timestamp) */
            ZEGO_ERROR_CODE_MEDIA_DATA_PUBLISHER_TIMESTAMP_GO_BACK_ERROR           = 1013004,

            /** The AudioEffectPlayer instance is not created. */
            ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_NO_INSTANCE                        = 1014000,

            /** loadResource failed */
            ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_LOAD_FAILED                        = 1014001,

            /** play audio effect failed */
            ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_PLAY_FAILED                        = 1014002,

            /** seekTo failed */
            ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_SEEK_FAILED                        = 1014003,

            /** The number of AudioEffectPlayer instances exceeds the maximum number allowed. */
            ZEGO_ERROR_CODE_AUDIO_EFFECT_PLAYER_EXCEED_MAX_COUNT                   = 1014004,

            /** Network connectivity test failed. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_CONNECTIVITY_TEST_FAILED             = 1015001,

            /** Network speed test connection failure. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_CONNECT_SERVER_FAILED           = 1015002,

            /** RTP timeout, please check whether the network is normal. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_RTP_TIMEOUT_ERROR               = 1015003,

            /** engine denied to continue testing network. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_ENGINE_DENIED                   = 1015004,

            /** Actively stop network test when starting to publish the stream. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_STOPPED_BY_PUBLISHING_STREAM    = 1015005,

            /** Actively stop network test when starting to play the stream. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_STOPPED_BY_PLAYING_STREAM       = 1015006,

            /** Network test internal error. */
            ZEGO_ERROR_CODE_UTILITIES_NETWORK_TOOL_INNER_ERROR                     = 1015009,

            /** The set system performance monitoring interval is out of range. */
            ZEGO_ERROR_CODE_UTILITIES_PERFORMANCE_MONITOR_INTERVAL_INVALID         = 1015031,


        };

    } //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSERRORCODE_H__
