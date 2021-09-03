
class ZegoErrorCode {

  /// Execution successful.
  static const int CommonSuccess                                                          = 0;

  /// Description: The engine is not initialized and cannot call non-static functions. <br>Cause: Engine not created.<br>Solutions: Please call the [createEngine] function to create the engine first, and then call the current function.
  static const int CommonEngineNotCreate                                                  = 1000001;

  /// Description: Not logged in to the room, unable to support function implementation. <br>Cause: Not logged in to the room.<br>Solutions: Please call [loginRoom] to log in to the room first, and use related functions during the online period after entering the room.
  static const int CommonNotLoginRoom                                                     = 1000002;

  /// Description: The audio and video module of the engine is not started and cannot support function realization. <br>Cause: Audio and video modules with no engine started.<br>Solutions: Please call [startPreviewView] [startPublishingStream] [startPlayingStream] to start the audio and video module first.
  static const int CommonEngineNotStarted                                                 = 1000003;

  /// Description: Call functions that are not supported on the current system/platform. <br>Cause: For example, calling the function of setting the Android context environment on a non-Android system.<br>Solutions: Check if the system environment matches.
  static const int CommonUnsupportedPlatform                                              = 1000006;

  /// Description: Invalid Android context. <br>Cause: Not set or set the wrong Android context environment.<br>Solutions: Set the correct Android context.
  static const int CommonInvalidAndroidEnvironment                                        = 1000007;

  /// Description: `setEventHandler` has been called to set the handler, please do not set it repeatedly. <br>Cause: Call `setEventHandler` repeatedly to set the handler.<br>Solutions: If you need to repeat the settings, please call `setEventHandler` first to empty the previous handler.
  static const int CommonEventHandlerExists                                               = 1000008;

  /// Description: The current SDK does not support this feature. <br>Cause: The SDK version used does not include this feature.<br>Solutions: Please contact ZEGO technical support.
  static const int CommonSdkNoModule                                                      = 1000010;

  /// Description: The input streamID is too long. <br>Cause: The length of the streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] exceeds the limit. <br>Solutions: The streamID should be less than 256 bytes.
  static const int CommonStreamIdTooLong                                                  = 1000014;

  /// Description: The input StreamID is null. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] is null or empty string. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal.
  static const int CommonStreamIdNull                                                     = 1000015;

  /// Description: The input streamID contains invalid characters. <br>Cause: The streamID parameter passed in when calling [startPublishingStream] or [startPlayingStream] contains invalid characters. <br>Solutions: Check whether the streamID parameter passed in when calling the function is normal, only support numbers, english characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  static const int CommonStreamIdInvalidCharacter                                         = 1000016;

  /// Description: This AppID has been removed from production. <br>Solutions: Please check the status of the AppID on the ZEGO official website console or contact ZEGO technical support.
  static const int CommonAppOfflineError                                                  = 1000037;

  /// Description: Incorrect server configuration. <br>Solutions: Please contact ZEGO technical support.
  static const int CommonAppFlexiableConfigError                                          = 1000038;

  /// Description: Incorrect CDN address. <br>Cause: The set CDN URL is not a standard RTMP or FLV protocol. <br>Solutions: Please check the supported protocol and format.
  static const int CommonCdnUrlInvalid                                                    = 1000055;

  /// DNS resolution failed. Please check network configurations. This error code is deprecated.
  static const int CommonDnsResolveError                                                  = 1000060;

  /// Server dispatching exception. Please contact ZEGO technical support to solve the problem. This error code is deprecated.
  static const int CommonDispatchError                                                    = 1000065;

  /// Description: SDK internal null pointer error. <br>Cause: The Android JVM environment is abnormal. <br>Please check whether the Android JVM environment is normal or contact ZEGO technical support.
  static const int CommonInnerNullptr                                                     = 1000090;

  /// AppID cannot be 0. Please check if the AppID is correct.
  static const int EngineAppidZero                                                        = 1001000;

  /// The length of the input AppSign must be 64 bytes.
  static const int EngineAppsignInvalidLength                                             = 1001001;

  /// The input AppSign contains invalid characters. Only '0'-'9', 'a'-'f', 'A'-'F' are valid.
  static const int EngineAppsignInvalidCharacter                                          = 1001002;

  /// The input AppSign is empty.
  static const int EngineAppsignNull                                                      = 1001003;

  /// Description: Authentication failed. <br>Cause: Incorrect AppID; using AppID in wrong environment. <br>Solutions: Please check AppID is correct or not on ZEGO manage console; check the production environment was selected for SDK initialization without prior go-live process.
  static const int EngineAppidIncorrectOrNotOnline                                        = 1001004;

  /// Description: Authentication failed. <br>Cause: Incorrect AppSign. <br>Solutions: Please check AppSign is correct or not on ZEGO manage console.
  static const int EngineAppsignIncorrect                                                 = 1001005;

  /// Description: No write permission to the log file. <br>Cause: App has no write permission to log file folder. <br>Solutions: Please check has grant write permission to App or not; check log folder is protected or not.
  static const int EngineLogNoWritePermission                                             = 1001014;

  /// Description: The log file path is too long. <br>Cause: The length of log file path exceeds limit. <br>Solutions: Please check the length of log file path.
  static const int EngineLogPathTooLong                                                   = 1001015;

  /// Description: Set room mode failed. <br>Cause: Set room mode before initialize the SDK. <br>Solutions: Please set room mode after initialize the SDK.
  static const int EngineSetRoomModeErrorTime                                             = 1001020;

  /// Description: The experimental API json parameter parsing failed. <br>Cause: Invalid json format; wrong function name or parameter. <br>Solutions: Please check json format is valid or not; check function name or parameter is correct or not, contact ZEGO technical support for specific function name and parameters.
  static const int EngineExperimentalJsonStrInvalid                                       = 1001091;

  /// Description: The number of rooms the user attempted to log into simultaneously exceeds the maximum number allowed. Currently, a user can only be logged in to one main room.<br>Cause: Login multiple rooms simultaneously under single room mode. <br>Solutions: Please check is login multiple rooms simultaneously or not under single room mode.
  static const int RoomCountExceed                                                        = 1002001;

  /// Description: Haven't login with the input room ID.<br>Cause: Haven't login with the input room ID before call [logoutRoom] or [switchRoom] or [renewToken] or [setRoomExtraInfo]. <br>Solutions: Please check has login with the room ID or not.
  static const int RoomRoomidIncorrect                                                    = 1002002;

  /// Description: The input user ID is empty.<br>Cause: The input user ID is empty. <br>Solutions: Please check the input user ID is empty or not.
  static const int RoomUserIdNull                                                         = 1002005;

  /// Description: The input user ID contains invalid characters.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  static const int RoomUserIdInvalidCharacter                                             = 1002006;

  /// The input user ID is too long. <br>The length of the user ID input by the [loginRoom] function is greater than or equal to 64 bytes. <br>Please check the user ID entered when calling the [loginRoom] function to ensure that its length is less than 64 bytes.
  static const int RoomUserIdTooLong                                                      = 1002007;

  /// The input user name is empty. <br>The user name entered by the [loginRoom] function is empty. <br>Please check the user name entered when calling the [loginRoom] function to make sure it is not empty.
  static const int RoomUserNameNull                                                       = 1002008;

  /// The input user name contains invalid characters. <br>The user name entered by the [loginRoom] function contains illegal characters.<br>Please check the user name entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  static const int RoomUserNameInvalidCharacter                                           = 1002009;

  /// The input user name is too long. <br>The length of the user name input by the [loginRoom] function is greater than or equal to 256 bytes. <br>Please check the user name entered when calling the [loginRoom] function to ensure that its length is less than 256 bytes.
  static const int RoomUserNameTooLong                                                    = 1002010;

  /// The input room ID is empty. <br>The room ID entered by the [loginRoom] function is empty. <br>Please check the room ID entered when calling the [loginRoom] function to make sure it is not empty.
  static const int RoomRoomidNull                                                         = 1002011;

  /// The input room ID contains invalid characters. <br>The room ID entered by the [loginRoom] function contains illegal characters.<br>Please check the room ID entered when calling the [loginRoom] function to ensure that it is only contain numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  static const int RoomRoomidInvalidCharacter                                             = 1002012;

  /// The input room ID is too long. <br>The length of the room ID input by the [loginRoom] function is greater than or equal to 128 bytes. <br>Please check the room ID entered when calling the [loginRoom] function to ensure that its length is less than 128 bytes.
  static const int RoomRoomidTooLong                                                      = 1002013;

  /// The key for room extra info is empty. <br>The key for room extra info entered by the [setRoomExtraInfo] function is empty. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to make sure it is not empty.
  static const int RoomRoomExtraInfoKeyEmpty                                              = 1002014;

  /// The key for room extra info is too long. <br>The length of the key for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 128 bytes. <br>Please check the key for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 128 bytes.
  static const int RoomRoomExtraInfoKeyTooLong                                            = 1002015;

  /// The value for room extra info is too long. <br>The length of the value for room extra info input by the [setRoomExtraInfo] function is greater than or equal to 4096 bytes. <br>Please check the value for room extra info entered when calling the [setRoomExtraInfo] function to ensure that its length is less than 4096 bytes.
  static const int RoomRoomExtraInfoValueTooLong                                          = 1002016;

  /// Description: The number of keys set in the room additional message exceeds the maximum number of supported limits. <br>Cause: called setRoomExtraInfo Different keys have been passed in multiple times. <br> Solutions: please contact ZEGO technical support.
  static const int RoomRoomExtraInfoExceedKeys                                            = 1002017;

  /// Description: set multi room mode, userID or user name is different. <br>Cause: set multi room mode, login multi room use different user id or user name. <br> Solutions: Currently supports at most one key, if you need to support multiple, contact ZEGO technical support.
  static const int RoomMultiRoomLoginUserNotSame                                          = 1002018;

  /// Description: The [switchRoom] function cannot be used in multi-room mode. <br>Cause: multi room mode SDK not support. <br> Solutions: first call [logoutRoom] then call [loginRoom].
  static const int RoomMultiRoomSwtichRoomInvalid                                         = 1002019;

  /// Description: Login failed, possibly due to network problems. <br>Cause: The current network is abnormal. <br> Solutions: It is recommended to switch the network to try.
  static const int RoomErrorConnectFailed                                                 = 1002030;

  /// Description: Login timed out, possibly due to network problems. <br>Cause: The current network delay is large. <br> Solutions: It is recommended to switch the network to try.
  static const int RoomErrorLoginTimeout                                                  = 1002031;

  /// Description: Room login authentication failed. <br>Cause: login set token error or token expired. <br> Solutions: set new token.
  static const int RoomErrorAuthenticationFailed                                          = 1002033;

  /// Description: The number of users logging into the room exceeds the maximum number of concurrent users configured for the room. (In the test environment, the default maximum number of users in the room is 50). <br>Cause: too much user in room. <br> Solutions: contact ZEGO technical support.
  static const int RoomErrorExceedMaximumMember                                           = 1002034;

  /// Description: in test environment The total number of rooms logged in at the same time exceeds the limit. (In the test environment, the maximum number of concurrent rooms is 10). <br>Cause: Too many rooms logged in at the same time. <br> Solutions: logout some room, login room.
  static const int RoomErrorExceedMaximumRoomCount                                        = 1002035;

  /// Description: login failed, multi-room mode is not activate. <br>Cause: multi-room mode is not activate. <br> Solutions: please contact ZEGO technical support.
  static const int RoomErrorLoginMultiRoomNotOpen                                         = 1002036;

  /// The total number of rooms logged in at the same time exceeds the limit, Please contact ZEGO technical support.
  static const int RoomErrorMultiRoomExceedMaximumRoomCount                               = 1002037;

  /// Description: The user was kicked out of the room. <br>Cause: Possibly because the same user ID is logged in on another device. <br>Solutions: Use a unique user ID.
  static const int RoomKickedOut                                                          = 1002050;

  /// Description: Room connection is temporarily interrupted and is retrying. <br>Cause: Possibly due to network problems. <br>Solutions: Please wait or check whether the network is normal.
  static const int RoomConnectTemporaryBroken                                             = 1002051;

  /// Description: Room connection disconnected. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
  static const int RoomDisconnect                                                         = 1002052;

  /// Description: Room login retry has exceeded the maximum retry time. <br>Cause: Possibly due to network problems. <br>Solutions: Please check whether the network is working or switch the network environment.
  static const int RoomRetryTimeout                                                       = 1002053;

  /// Description: The business server has sent a signal to kick the user out of the room. <br>Cause: The business server has sent a signal to kick the user out of the room. <br>Solutions: Contact the business server for processing.
  static const int RoomManualKickedOut                                                    = 1002055;

  /// Description: Wrong order of login rooms. <br>Cause: Log in multi room without log in the main room. <br>Solutions: Please log in to the main room with `loginRoom` before logging in to multi room.
  static const int RoomWrongLoginSequence                                                 = 1002061;

  /// Description: Wrong order of logout rooms. <br>Cause: Log out the main room without log out multi room. <br>Solutions: Please log out of the multi room before logging out of the main room.
  static const int RoomWrongLogoutSequence                                                = 1002062;

  /// Description: No multi-room permission. <br>Cause: No multi-room permission. <br>Solutions: Please contact ZEGO technical support to enable it.
  static const int RoomNoMultiRoomPermission                                              = 1002063;

  /// Description: The room ID has been used by other login room interface. Current user can not login room with the room ID before release the room ID. <br>Cause: The room ID has been used by other login room interface. <br>Solutions: Logout the room with the same room ID first.
  static const int RoomRoomIdHasBeenUsed                                                  = 1002064;

  /// Description: This method has been deprecated after version 2.9.0. <br>Cause: This method has been deprecated after version 2.9.0. <br>Solutions: Please set [setRoomMode] to select multi-room mode before the engine started, and then call [loginRoom] to use multi-room.
  static const int RoomMultiRoomDeprecated                                                = 1002065;

  /// Description: Room login failed due to internal system exceptions.<br>Cause: Unknown internal error.<br>Solutions: Contact ZEGO technical support to deal with it.
  static const int RoomInnerError                                                         = 1002099;

  /// Description: Publishing failed due to no data in the stream.<br>Cause: No data in the stream.<br>Solutions: Check whether the video, audio capture module is working properly.
  static const int PublisherPublishStreamFailed                                           = 1003001;

  /// Description: Publishing failed due to wrong bitrate setting.<br>Cause: The set video bitrate, audio bitrate, or minimum video bitrate threshold for traffic control exceeds the upper limit.<br>Solutions: Please check if the bitrate value is in the correct unit (kbps).Adjust the bitrate setting.
  static const int PublisherBitrateInvalid                                                = 1003002;

  /// Description: The property param of the traffic control is set incorrectly.<br>Cause: The property param of the traffic control is less than 0 or exceeds all combinations.<br>Solutions: Check the settings of the property param of the traffic control.
  static const int PublisherTrafficModeInvalid                                            = 1003005;

  /// Failed to publish the stream. Not support H.265 encoder.
  static const int PublisherErrorH265EncoderNotSupported                                  = 1003010;

  /// Description:Stream publishing is temporarily interrupted and is retrying. <br>Cause: The network fluctuates or the network signal is bad.<br>Solutions: Please wait or check whether the network is normal.
  static const int PublisherErrorNetworkInterrupt                                         = 1003020;

  /// Description: Stream publish retry has exceeds the maximum retry time.<br>Cause: The the network signal is bad, and the maximum retry time is exceeded.<br>Solutions: Check the network status or switch to another network.
  static const int PublisherErrorRetryTimeout                                             = 1003021;

  /// Description: Failed to publish the stream. The publish channel is already publishing streams.<br>Cause:  The publish channel is already publishing streams.<br>Solutions: Please check the business logic to avoid repeating the publish for publish channel which is publishing.
  static const int PublisherErrorAlreadyDoPublish                                         = 1003023;

  /// Description: Failed to publish the stream. Publishing of this stream is prohibited by backend configuration.<br>Cause: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Contact ZEGO technical support to deal with it.
  static const int PublisherErrorServerForbid                                             = 1003025;

  /// Description: Failed to publish the stream. The same stream already exists in the room.<br>Cause: The same stream already exists in the room.<br>Solutions: Replace with a new stream ID. Adjust the stream ID generation strategy to ensure uniqueness.
  static const int PublisherErrorRepetitivePublishStream                                  = 1003028;

  /// Description: Failed to publish the stream. The connection to the RTMP server is interrupted.<br>Cause: The publish address is wrong, or the network signal is bad.<br>Solutions: Please check whether there is any problem with the network connection or the stream publishing URL.
  static const int PublisherRtmpServerDisconnect                                          = 1003029;

  /// Description: Failed to take a screenshot of the publis stream screen. <br>Cause: The preview is stopped and the push is abnormal. <br>Solutions: Turn on preview or re-publish.
  static const int PublisherTakePublishStreamSnapshotFailed                               = 1003030;

  /// Description: Failed to update the relay CDN status. <br>Cause: The URL of the relay address is incorrect. <br>Solutions: Check whether the input URL is valid.
  static const int PublisherUpdateCdnTargetError                                          = 1003040;

  /// Description: Failed to send SEI. <br>Cause: data is empty. <br>Solutions: Incoming non-empty data.
  static const int PublisherSeiDataNull                                                   = 1003043;

  /// Description: Failed to send SEI. <br>Cause: The input data exceeds the length limit. <br>Solutions: The length of the sent SEI data should be less than 4096 bytes.
  static const int PublisherSeiDataTooLong                                                = 1003044;

  /// The extra info of the stream is null.
  static const int PublisherExtraInfoNull                                                 = 1003050;

  /// The extra info of the stream is too long. The maximum length should be less than 1024 bytes.
  static const int PublisherExtraInfoTooLong                                              = 1003051;

  /// Failed to update the extra info of the stream. Please check the network connection.
  static const int PublisherUpdateExtraInfoFailed                                         = 1003053;

  /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path is empty. <br>Solutions: Incoming non-empty path.
  static const int PublisherWatermarkUrlNull                                              = 1003055;

  /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path exceeds the byte size limit. <br>Solutions: The incoming watermark path should be less than 1024 bytes.
  static const int PublisherWatermarkUrlTooLong                                           = 1003056;

  /// Description: Failed to set publish watermark. <br>Cause: The incoming watermark path was entered incorrectly or the image format is not supported. <br>Solutions: Incoming the correct watermark path, only `jpg` and `png` image formats are supported.
  static const int PublisherWatermarkUrlInvalid                                           = 1003057;

  /// Description: Incorrect watermark layout.<br>Caution: The layout area exceed the encoding resolution.<br>Solutions: Make sure the layout area cannot exceed the encoding resolution and call current interface.
  static const int PublisherWatermarkLayoutInvalid                                        = 1003058;

  /// Description: The publish stream encryption key is invalid.<br>Caution: The set encryption key length is not supported.<br>Solutions: The Publish-stream encryption key length to be 16/24/32 bytes.
  static const int PublisherEncryptionKeyInvalid                                          = 1003060;

  /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function with the parameter 'ZegoPublisherConfig' needs to be called.
  static const int PublisherErrorPublishWhenUsingMultiRoom                                = 1003070;

  /// Description: StartPlayingStream failed.<br>Caution: In multi-room mode, the publish-stream function is called incorrectly.<br>Solutions: In multi-room mode, A publish-stream function parameter 'roomID' cannot be empty.
  static const int PublisherErrorPublishWithRoomIdIsNullWhenUsingMultiRoom                = 1003071;

  /// Description: Stream publishing failed due to system internal exceptions.<br>Caution: Stream publishing failed due to system internal exceptions.<br>Solutions: Please contact ZEGO technical support to solve the problem.
  static const int PublisherInnerError                                                    = 1003099;

  /// Description: Stream playing failed.<br>Caution: Possibly due to no data in the stream.<br>Solutions: Check to see if the publish-stream is working or try to play stream again, and if the problem is still not resolved, please contact ZEGO technical support to solve the problem.
  static const int PlayerPlayStreamFailed                                                 = 1004001;

  /// Description: Stream playing failed.<br>Caution: The stream does not exist.<br>Solutions: Please check whether the remote end publish is indeed successful, or whether the publish and play environment are inconsistent.
  static const int PlayerPlayStreamNotExist                                               = 1004002;

  /// Description: Stream playing error.<br>Caution: The number of streams the user attempted to play simultaneously exceeds the maximum number allowed.<br>Solutions: Currently, up to 12 steams can be played at the same time. Please contact ZEGO technical support to increase the capacity if necessary.
  static const int PlayerCountExceed                                                      = 1004010;

  /// Description: Stream playing is temporarily interrupted.<br>Caution: Network exception.<br>Solutions: Please wait or check whether the network is normal.
  static const int PlayerErrorNetworkInterrupt                                            = 1004020;

  /// Description: Failed to play the stream.<br>Caution: Publishing of this stream is prohibited by backend configuration.<br>Solutions: Please contact ZEGO technical support to solve the problem.
  static const int PlayerErrorServerForbid                                                = 1004025;

  /// Description: Failed to capture the screenshot of the streaming screen, please check whether the stream to be captured is normal. <br>Cause: The stream is not pulled. <br>Solutions: Check whether it starts to play the stream, and whether there is an abnormality in the process of playing the stream.
  static const int PlayerTakePlayStreamSnapshotFailed                                     = 1004030;

  /// Description: The play stream decryption key is invalid, the key length only supports 16/24/32 bytes. <br>Cause: The input key length is not 16/24/32 bytes. <br>Solutions: Adjust the input key length to 16/24/32 bytes.
  static const int PlayerDecryptionKeyInvalid                                             = 1004060;

  /// Description: Pull stream decryption failed, please check whether the decryption key is correct. <br>Cause: Incorrect decryption key entered. <br>Solutions: Enter the correct decryption key.
  static const int PlayerDecryptionFailed                                                 = 1004061;

  /// Description: Calling the wrong function after enabling the multi-room function causes playing stream fail. <br>Cause: Called the playing stream function that only works for joining a single room mode. <br>Solutions: Please use the function of the same name with ZegoPlayerConfig and specify the room ID to play the stream.
  static const int PlayerErrorPlayStreamWhenUsingMultiRoom                                = 1004070;

  /// Description: In the multi-room mode, the roomID parameter of the play stream cannot be empty. <br>Cause: The roomID parameter of the pull stream is empty. <br>Solutions: Please enter the correct roomID.
  static const int PlayerErrorPlayStreamWithRoomIdIsNullWhenUsingMultiRoom                = 1004071;

  /// Description: An internal system exception causes a failure to pull the stream. <br>Cause: SDK internal error. <br>Solutions: Please contact ZEGO technical support.
  static const int PlayerInnerError                                                       = 1004099;

  /// Description: Does not support the use of stream mixing service. <br>Cause: No stream mixing service configured. <br>Solutions: Please open the service on the console or contact ZEGO business staff.
  static const int MixerNoServices                                                        = 1005000;

  /// Description: The mixing task ID is null. <br>Cause: The mixing task ID input when starting mixing task is empty. <br>Solutions: Please enter the correct mixing task ID.
  static const int MixerTaskIdNull                                                        = 1005001;

  /// Description: The stream mixing task ID is too long. <br>Cause: The stream mixing task ID is greater than 256 bytes. <br>Solutions: Please enter a mixing task ID less than 256 bytes.
  static const int MixerTaskIdTooLong                                                     = 1005002;

  /// Description: Invalid mixed flow task ID. <br>Cause: Illegal characters in the stream mixing task ID. <br>Solutions: Stream mixing task ID only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')', '_','+','=','-','`',';',''',',','.','<','>','/','\', please enter the stream mixing task ID in the correct format.
  static const int MixerTaskIdInvalidCharacter                                            = 1005003;

  /// No output is specified in the configuration of the stream mixing task.
  static const int MixerNoOutputTarget                                                    = 1005005;

  /// Incorrect stream mixing output. Please check if the streamID of the output target contains invalid characters.
  static const int MixerOutputTargetInvalid                                               = 1005006;

  /// Failed to start the stream mixing task, possibly due to network problems.
  static const int MixerStartRequestError                                                 = 1005010;

  /// Failed to stop the stream mixing task, possibly due to network problems.
  static const int MixerStopRequestError                                                  = 1005011;

  /// The stream mixing task must be stopped by the user who started the task.
  static const int MixerNotOwnerStopMixer                                                 = 1005012;

  /// Starts stream mixing tasks too frequently.
  static const int MixerStartQpsOverload                                                  = 1005015;

  /// Stops stream mixing tasks too frequently.
  static const int MixerStopQpsOverload                                                   = 1005016;

  /// The input stream list of the stream mixing task is null.
  static const int MixerInputListInvalid                                                  = 1005020;

  /// The output stream list of the stream mixing task is null.
  static const int MixerOutputListInvalid                                                 = 1005021;

  /// The video configuration of the stream mixing task is invalid.
  static const int MixerVideoConfigInvalid                                                = 1005023;

  /// The audio configuration of the stream mixing task is invalid. Please check if an unsupported codec is used.
  static const int MixerAudioConfigInvalid                                                = 1005024;

  /// The number of input streams exceeds the maximum number allowed. Up to 9 input streams can be specified.
  static const int MixerExceedMaxInputCount                                               = 1005025;

  /// The input stream does not exist.
  static const int MixerInputStreamNotExists                                              = 1005026;

  /// Invalid stream mixing input parameters. It may be that the layout of the input streams exceeds the canvas.
  static const int MixerInputParametersError                                              = 1005027;

  /// The number of output streams exceeds the maximum number allowed. Up to 3 output streams can be specified.
  static const int MixerExceedMaxOutputCount                                              = 1005030;

  /// Exceed the maximum number of focus voice input streams, and support up to 4 input streams to set focus voice
  static const int MixerExceedMaxAudioFocusStreamCount                                    = 1005031;

  /// Stream mixing authentication failed.
  static const int MixerAuthenticationFailed                                              = 1005050;

  /// The input watermark is null.
  static const int MixerWatermarkNull                                                     = 1005061;

  /// Invalid watermark parameter. It may be that the layout of the watermark exceeds the canvas.
  static const int MixerWatermarkParametersError                                          = 1005062;

  /// Invalid watermark URL. The URL must start with `preset-id://`, and must end with `.jpg` or `.png`.
  static const int MixerWatermarkUrlInvalid                                               = 1005063;

  /// Invalid background image URL. The URL must start with `preset-id://`, and must end with `.jpg` or `.png`.
  static const int MixerBackgroundImageUrlInvalid                                         = 1005067;

  /// User data too long, the maximum length of user data should not be more than 1000 bytes.
  static const int MixerUserDataTooLong                                                   = 1005068;

  /// The server for auto stream mixing is not found. Please contact ZEGO technical support to enable it.
  static const int MixerAutoMixStreamServerNotFound                                       = 1005070;

  /// Stream mixing internal error.
  static const int MixerInnerError                                                        = 1005099;

  /// Generic device error.
  static const int DeviceErrorTypeGeneric                                                 = 1006001;

  /// The device ID does not exist.
  static const int DeviceErrorTypeInvalidId                                               = 1006002;

  /// No permission to access the device. Please check the permissions of the camera or microphone.
  static const int DeviceErrorTypeNoAuthorization                                         = 1006003;

  /// The frame rate of the capture device is 0.
  static const int DeviceErrorTypeZeroFps                                                 = 1006004;

  /// The device is occupied.
  static const int DeviceErrorTypeInUseByOther                                            = 1006005;

  /// The device is unplugged.
  static const int DeviceErrorTypeUnplugged                                               = 1006006;

  /// The device needs to be restarted.
  static const int DeviceErrorTypeRebootRequired                                          = 1006007;

  /// The device media is lost.
  static const int DeviceErrorMediaServicesLost                                           = 1006008;

  /// The device list cannot be empty when trying to release devices.
  static const int DeviceFreeDeviceListNull                                               = 1006020;

  /// The set sound level monitoring interval is out of range.
  static const int DeviceSouldLevelIntervalInvalid                                        = 1006031;

  /// The set audio spectrum monitoring interval is out of range.
  static const int DeviceAudioSpectrumIntervalInvalid                                     = 1006032;

  /// The set camera zoom factor is out of range.
  static const int DeviceZoomFactorInvalid                                                = 1006040;

  /// The set camera exposure compensation is out of range.
  static const int DeviceExposureCompensationValueInvalid                                 = 1006041;

  /// Device internal error.
  static const int DeviceInnerError                                                       = 1006099;

  /// Unknown error in the preprocessing module. Please contact ZEGO technical support.
  static const int PreprocessPreprocessUnknownError                                       = 1007001;

  /// Invalid beauty option. Please check the input parameters.
  static const int PreprocessBeautifyOptionInvalid                                        = 1007005;

  /// The reverberation parameter is null. Please check the input parameter.
  static const int PreprocessReverbParamNull                                              = 1007006;

  /// The voice changer parameter is null. Please check the input parameter.
  static const int PreprocessVoiceChangerParamNull                                        = 1007007;

  /// The room size value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 1.0.
  static const int PreprocessReverbParamRoomSizeInvalid                                   = 1007011;

  /// The reverberance value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 0.5.
  static const int PreprocessReverbParamReverberanceInvalid                               = 1007012;

  /// The damping value of the reverberation parameters is invalid. The value should be in the range of 0.0 ~ 2.0.
  static const int PreprocessReverbParamDampingInvalid                                    = 1007013;

  /// The dry_wet_ratio value of the reverberation parameters is invalid. The value should be greater than 0.0.
  static const int PreprocessReverbParamDryWetRatioInvalid                                = 1007014;

  /// The angle value of the virtual stereo parameters is invalid. The value should be in the range of 0 ~ 180.
  static const int PreprocessVirtualStereoAngleInvalid                                    = 1007015;

  /// The voice changer param is invalid. The value should be in the range of -8.0 ~ 8.0.
  static const int PreprocessVoiceChangerParamInvalid                                     = 1007016;

  /// The reverberation echo parameters is null. Please check the input parameter.
  static const int PreprocessReverbEchoParamNull                                          = 1007017;

  /// The reverberation echo parameters is invalid.
  static const int PreprocessReverbEchoParamInvalid                                       = 1007018;

  /// The MediaPlayer instance is not created.
  static const int MediaPlayerNoInstance                                                  = 1008001;

  /// The MediaPlayer failed to play the media. The resource file is not loaded.
  static const int MediaPlayerNoFilePath                                                  = 1008003;

  /// The MediaPlayer failed to load the file. The file format is not supported.
  static const int MediaPlayerFileFormatError                                             = 1008005;

  /// The MediaPlayer failed to load the file. The file path does not exist.
  static const int MediaPlayerFilePathNotExists                                           = 1008006;

  /// The MediaPlayer failed to load the file due to decoding errors.
  static const int MediaPlayerFileDecodeError                                             = 1008007;

  /// The MediaPlayer failed to load files. No supported audio/video stream exists.
  static const int MediaPlayerFileNoSupportedStream                                       = 1008008;

  /// The MediaPlayer failed to play the file due to demuxing errors.
  static const int MediaPlayerDemuxError                                                  = 1008010;

  /// The MediaPlayer failed to seek, possibly because the file hasn't been loaded yet.
  static const int MediaPlayerSeekError                                                   = 1008016;

  /// The MediaPlayer is configured with a video data format not supported by the platform (e.g., CVPixelBuffer on iOS does not support NV21).
  static const int MediaPlayerPlatformFormatNotSupported                                  = 1008020;

  /// The number of MediaPlayer instances exceeds the maximum number allowed. Up to 4 instances can be created.
  static const int MediaPlayerExceedMaxCount                                              = 1008030;

  /// The media player failed to specify the audio track index
  static const int MediaPlayerSetAudioTrackIndexError                                     = 1008040;

  /// Invalid voice changing parameters set by media player
  static const int MediaPlayerSetVoiceChangerParamInvalid                                 = 1008041;

  /// To make `takeSnapshot` effective, you need to ensure that the video is playing and `setPlayerCanvas` has been called to display the video on the control
  static const int MediaPlayerTakeSnapshotTimingError                                     = 1008042;

  /// The input parameter is not within the legal value range. Please check the interface notes and input a value within the legal value range.
  static const int MediaPlayerParamValueRangeIllegal                                      = 1008043;

  /// MediaPlayer internal error.
  static const int MediaPlayerInnerError                                                  = 1008099;

  /// The input message content is empty.
  static const int IMContentNull                                                          = 1009001;

  /// The input message content is too long. The maximum length should be less than 1024 bytes.
  static const int IMContentTooLong                                                       = 1009002;

  /// The room where the message is sent is different from the room currently logged in
  static const int IMInconsistentRoomId                                                   = 1009005;

  /// Failed to send the message, possibly due to network problems.
  static const int IMSendFailed                                                           = 1009010;

  /// Failed to send broadcast message, QPS exceeds the limit, the maximum QPS is 2
  static const int IMBroadcastMessageQpsOverload                                          = 1009015;

  /// The file name suffix is not supported. Only .mp4/.aac/.flv are supported currently. Depending on file name suffix, SDK sets the specified recording format accordingly.
  static const int RecorderFileSuffixNameFormatNotSupport                                 = 1010002;

  /// Generic error of recording API, generally due to invalid input parameters.
  static const int RecorderCommonLiveroomApiError                                         = 1010003;

  /// The specified recorded file path is too long. The maximum length should be less than 1024 bytes.
  static const int RecorderFilePathTooLong                                                = 1010011;

  /// SDK internal VE error. Please contact ZEGO technical support to solve the problem.
  static const int RecorderInnerVeError                                                   = 1010012;

  /// Failed to open the file. Invalid file path or no permissions to the file.
  static const int RecorderOpenFileFailed                                                 = 1010013;

  /// Failed to write to the file, possibly due to no write permission to the file.
  static const int RecorderWriteFileError                                                 = 1010014;

  /// Not enough spare capacity.
  static const int RecorderNoEnoughSpareCapacity                                          = 1010017;

  /// File handle exceptions.
  static const int RecorderFileHandleExceptions                                           = 1010018;

  /// I/O exceptions.
  static const int RecorderIoExceptions                                                   = 1010019;

  /// The custom video capturer is not created. Please make sure to use it only after the `onStart` callback is received.
  static const int CustomVideoIOCapturerNotCreated                                        = 1011001;

  /// The custom video capture module is not enabled. Please make sure it is enabled in the initialization configurations.
  static const int CustomVideoIONoCustomVideoCapture                                      = 1011002;

  /// Failed to enable/disable custom video capture/rendering. Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`).
  static const int CustomVideoIOEnableCustomIoFailed                                      = 1011003;

  /// The custom video capturer is not created.
  static const int CustomVideoIOProcessModuleNotCreated                                   = 1011004;

  /// The custom video process module is not enabled. Please make sure that is called [enableCustomVideoProcessing].
  static const int CustomVideoIONoCustomVideoProcessing                                   = 1011005;

  /// The currently configured custom video capture format does not support this API.
  static const int CustomVideoIONotSupportedFormat                                        = 1011010;

  /// Custom video rendering does not support the currently set video buffer type.
  static const int CustomVideoIONotSupportedBufferType                                    = 1011011;

  /// Unsupported custom audio source type.
  static const int CustomAudioIOUnsupportedAudioSourceType                                = 1012001;

  /// The custom audio capture feature is not enabled. Please make sure that the custom audio IO module is enabled for the specified stream publishing channel.
  static const int CustomAudioIOCapturerNotCreated                                        = 1012002;

  /// The custom audio rendering feature is not enabled. Please make sure that the custom audio IO module is enabled.
  static const int CustomAudioIORendererNotCreated                                        = 1012003;

  /// Failed to enable/disable custom audio IO. Please make sure to enable/disable it before the engine is started (i.e., before calling `startPreview`, `startPublishingStream` or `startPlayingStream`).
  static const int CustomAudioIOEnableCustomAudioIoFailed                                 = 1012004;

  /// The sample rate parameter is illegal, please confirm whether the sample rate parameter value allowed by the interface is legal
  static const int CustomAudioIOAudioDataCallbackSampleRateNoSupport                      = 1012010;

  /// The MediaDataPublisher instance is not created.
  static const int MediaDataPublisherNoInstance                                           = 1013000;

  /// File error, failed to open
  static const int MediaDataPublisherFileParseError                                       = 1013001;

  /// File path error
  static const int MediaDataPublisherFilePathError                                        = 1013002;

  /// File decoding exception
  static const int MediaDataPublisherFileCodecError                                       = 1013003;

  /// Timestamp error (the later frame timestamp is smaller than the previous frame timestamp)
  static const int MediaDataPublisherTimestampGoBackError                                 = 1013004;

  /// The AudioEffectPlayer instance is not created.
  static const int AudioEffectPlayerNoInstance                                            = 1014000;

  /// loadResource failed
  static const int AudioEffectPlayerLoadFailed                                            = 1014001;

  /// play audio effect failed
  static const int AudioEffectPlayerPlayFailed                                            = 1014002;

  /// seekTo failed
  static const int AudioEffectPlayerSeekFailed                                            = 1014003;

  /// The number of AudioEffectPlayer instances exceeds the maximum number allowed.
  static const int AudioEffectPlayerExceedMaxCount                                        = 1014004;

  /// Network connectivity test failed.
  static const int UtilitiesNetworkConnectivityTestFailed                                 = 1015001;

  /// Network speed test connection failure.
  static const int UtilitiesNetworkToolConnectServerFailed                                = 1015002;

  /// RTP timeout, please check whether the network is normal.
  static const int UtilitiesNetworkToolRtpTimeoutError                                    = 1015003;

  /// engine denied to continue testing network.
  static const int UtilitiesNetworkToolEngineDenied                                       = 1015004;

  /// Actively stop network test when starting to publish the stream.
  static const int UtilitiesNetworkToolStoppedByPublishingStream                          = 1015005;

  /// Actively stop network test when starting to play the stream.
  static const int UtilitiesNetworkToolStoppedByPlayingStream                             = 1015006;

  /// Network test internal error.
  static const int UtilitiesNetworkToolInnerError                                         = 1015009;

  /// The set system performance monitoring interval is out of range.
  static const int UtilitiesPerformanceMonitorIntervalInvalid                             = 1015031;

  /// Description: The function call failed. <br>Cause: No range auido instance has been created. <br>Solutions: Create a range audio instance.
  static const int RangeAudioNoInstance                                                   = 1016000;

  /// Description: Failed to create range audio. <br>Cause: The instance exceeds the maximum limit. <br>Solutions: Use the used range audio example.
  static const int RangeAudioExceedMaxCount                                               = 1016001;

  /// Description: Failed to create range voice. <br>Cause: Range voice cannot be used in multi-room mode. <br>Solutions: Set the single-party mode.
  static const int RangeAudioNotSupportMultiRoom                                          = 1016002;

  /// Description: Failed to set the team ID. <br>Cause: The input team ID exceeds the maximum limit. <br>Solutions: The input string is less than 64 bytes.
  static const int RangeAudioTeamIdTooLong                                                = 1016003;

  /// Description: Failed to set the team ID.<br>Cause: The input user ID contains invalid characters. <br>Solutions: User ID can only contains numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  static const int RangeAudioTeamIdInvalidCharacter                                       = 1016004;

}
