
class ZegoErrorCode {

  /// execute success
  static const int CommonSuccess                                                          = 0;

  /// Engine not create, engine need to be created before calling non-static function
  static const int CommonEngineNotCreate                                                  = 1000001;

  /// Room not logged in, please log in to the room before publishing or playing the stream
  static const int CommonNotLoginRoom                                                     = 1000002;

  /// Engine not started, please call startPreviewView/startPublishing/startPlayingStream first
  static const int CommonEngineNotStarted                                                 = 1000003;

  /// Called an interface that is not supported by the current platform, for example, an interface that sets the Android context environment is called on a non-Android platform
  static const int CommonUnsupportedPlatform                                              = 1000006;

  /// Invalid android environment context
  static const int CommonInvalidAndroidEnvironment                                        = 1000007;

  /// Calling setEventHandler API error. Please set the handler to null before resetting it
  static const int CommonEventHandlerExists                                               = 1000008;

  /// The SDK does not include this module function, please contact technical support to provide
  static const int CommonSdkNoModule                                                      = 1000010;

  /// The input stream ID is too long The maximum length is 256 characters
  static const int CommonStreamIdTooLong                                                  = 1000014;

  /// Input stream ID is empty
  static const int CommonStreamIdNull                                                     = 1000015;

  /// Input stream ID contains illegal characters
  static const int CommonStreamIdInvalidCharacter                                         = 1000016;

  /// The app ID has been offline
  static const int CommonAppOfflineError                                                  = 1000037;

  /// There is an error in the background configuration, please check the APP configuration item
  static const int CommonAppFlexiableConfigError                                          = 1000038;

  /// The CDN address is incorrect. Please check the supported protocol and format
  static const int CommonCdnUrlInvalid                                                    = 1000055;

  /// DNS resolution failed Please check network configuration
  static const int CommonDnsResolveError                                                  = 1000060;

  /// Server dispatch exception, please contact technical support to solve
  static const int CommonDispatchError                                                    = 1000065;

  /// inner null pointer error, please contact technical support to solve
  static const int CommonInnerNullptr                                                     = 1000090;

  /// App ID cannot be 0, please check if app ID is correct
  static const int EngineAppidZero                                                        = 1001000;

  /// The length of input appsign must be 64 bytes
  static const int EngineAppsignInvalidLength                                             = 1001001;

  /// The input appsign contains illegal characters, Only '0'-'9','a'-'f','A'-'F' are valid
  static const int EngineAppsignInvalidCharacter                                          = 1001002;

  /// Input appsign is empty
  static const int EngineAppsignNull                                                      = 1001003;

  /// Authentication failed, please check if AppID is correct, or whether online environment was selected without applying to ZEGO for online
  static const int EngineAppidIncorrectOrNotOnline                                        = 1001004;

  /// AppSign authentication failed, please check if AppID is correct
  static const int EngineAppsignIncorrect                                                 = 1001005;

  /// Log has no write permission
  static const int EngineLogNoWritePermission                                             = 1001014;

  /// The path of the log is too long
  static const int EngineLogPathTooLong                                                   = 1001015;

  /// The number of login rooms exceeds the upper limit. Currently, only one room can be logged in at the same time
  static const int RoomCountExceed                                                        = 1002001;

  /// Input user ID is empty
  static const int RoomUserIdNull                                                         = 1002005;

  /// The input user ID contains illegal characters
  static const int RoomUserIdInvalidCharacter                                             = 1002006;

  /// The input user ID is too long. The maximum length is 64 characters
  static const int RoomUserIdTooLong                                                      = 1002007;

  /// Input username is empty
  static const int RoomUserNameNull                                                       = 1002008;

  /// The input username contains illegal characters
  static const int RoomUserNameInvalidCharacter                                           = 1002009;

  /// The input username is too long. The maximum length is 256 characters
  static const int RoomUserNameTooLong                                                    = 1002010;

  /// Input room ID is empty
  static const int RoomRoomidNull                                                         = 1002011;

  /// The entered room ID contains illegal characters
  static const int RoomRoomidInvalidCharacter                                             = 1002012;

  /// The input room ID is too long. The maximum length is 128 characters
  static const int RoomRoomidTooLong                                                      = 1002013;

  /// Login failed, possibly due to network reasons
  static const int RoomErrorConnectFailed                                                 = 1002030;

  /// Login timeout, possibly due to network reasons
  static const int RoomErrorLoginTimeout                                                  = 1002031;

  /// Login room authentication failed
  static const int RoomErrorAuthenticationFailed                                          = 1002033;

  /// The number of users logging into the room exceeds the maximum number of users configured for the room
  static const int RoomErrorExceedMaximumMember                                           = 1002034;

  /// The user is kicked out of the room and may log in to other devices with the same user ID
  static const int RoomKickedOut                                                          = 1002050;

  /// Room disconnected, possibly due to network
  static const int RoomConnectTemporaryBroken                                             = 1002051;

  /// Room disconnected, possibly due to network
  static const int RoomDisconnect                                                         = 1002052;

  /// Retry login room exceeded the maximum retry time
  static const int RoomRetryTimeout                                                       = 1002053;

  /// The server has sent a signal to kick out of the room. Please check the reason for the kick
  static const int RoomManualKickedOut                                                    = 1002055;

  /// Room login failure due to internal system exception
  static const int RoomInnerError                                                         = 1002099;

  /// Publish stream failed. It may be that there is no data in the publishing stream
  static const int PublisherPublishStreamFailed                                           = 1003001;

  /// The bitrate setting is incorrect. Please check the unit (kbps) for errors
  static const int PublisherBitrateInvalid                                                = 1003002;

  /// Wrong setting of publisher traffic control parameter
  static const int PublisherTrafficModeInvalid                                            = 1003005;

  /// Publish stream temporarily interrupted, retrying
  static const int PublisherErrorNetworkInterrupt                                         = 1003020;

  /// Publish stream failed, the user is already publishing
  static const int PublisherErrorAlreadyDoPublish                                         = 1003023;

  /// Publish stream failed, the stream is configured by the background system to prohibit push
  static const int PublisherErrorServerForbid                                             = 1003025;

  /// Publish stream failed. The same stream already exists in the room
  static const int PublisherErrorRepetitivePublishStream                                  = 1003028;

  /// The connection with the RTMP server was interrupted. Please check whether the network or publish url is abnormal
  static const int PublisherRtmpServerDisconnect                                          = 1003029;

  /// Update publish cdn url failed. Please check whether the URL is legal
  static const int PublisherUpdateCdnTargetError                                          = 1003040;

  /// Send SEI failed. data is null
  static const int PublisherSeiDataNull                                                   = 1003043;

  /// Send SEI failed, data is too long, maximum is 4096 characters
  static const int PublisherSeiDataTooLong                                                = 1003044;

  /// The extra info of publshing stream is null
  static const int PublisherExtraInfoNull                                                 = 1003050;

  /// Mixed stream extra info is too long, maximum is 1024 characters
  static const int PublisherExtraInfoTooLong                                              = 1003051;

  /// Update extra info failed, please check network connection
  static const int PublisherUpdateExtraInfoFailed                                         = 1003053;

  /// Watermark's URL is null
  static const int PublisherWatermarkUrlNull                                              = 1003055;

  /// Mixed watermark's URL is too long, maximum is 1024 characters
  static const int PublisherWatermarkUrlTooLong                                           = 1003056;

  /// Watermark format error, only supports jpg and png formats
  static const int PublisherWatermarkUrlInvalid                                           = 1003057;

  /// The layout of the watermark is incorrect. The layout area cannot exceed the encoding resolution
  static const int PublisherWatermarkLayoutInvalid                                        = 1003058;

  /// System internal exception causes streaming failure
  static const int PublisherInnerError                                                    = 1003099;

  /// Play stream failed. It may be that there is no data in the playing stream
  static const int PlayerPlayStreamFailed                                                 = 1004001;

  /// Playing stream exceeds the maximum number, currently supports up to 12 (please contact technical support for capacity expansion if necessary)
  static const int PlayerCountExceed                                                      = 1004010;

  /// Play stream temporarily interrupted, retrying
  static const int PlayerErrorNetworkInterrupt                                            = 1004020;

  /// Play stream failure caused by system internal exception
  static const int PlayerInnerError                                                       = 1004099;

  /// No mix stream service, please contact technical support to open
  static const int MixerNoServices                                                        = 1005000;

  /// The mixer task is null
  static const int MixerTaskIdNull                                                        = 1005001;

  /// Mixed stream task ID is too long, maximum is 256 characters
  static const int MixerTaskIdTooLong                                                     = 1005002;

  /// Illegal characters in mixed stream task ID
  static const int MixerTaskIdInvalidCharacter                                            = 1005003;

  /// Mixed stream task configuration does not specify output
  static const int MixerNoOutputTarget                                                    = 1005005;

  /// The mixed stream output target is incorrect. Check to see if an illegal character was passed in when the target was streamID
  static const int MixerOutputTargetInvalid                                               = 1005006;

  /// Failed to start the mixed stream task request, possibly due to network reasons
  static const int MixerStartRequestError                                                 = 1005010;

  /// Failed to stop the mixed stream task request, possibly due to network reasons
  static const int MixerStopRequestError                                                  = 1005011;

  /// The mixed stream task must be stopped by the start user of the task
  static const int MixerNotOwnerStopMixer                                                 = 1005012;

  /// Mixed stream task started too frequently
  static const int MixerStartQpsOverload                                                  = 1005015;

  /// Mixed stream task stopped too frequently
  static const int MixerStopQpsOverload                                                   = 1005016;

  /// Mixed stream task input list is null
  static const int MixerInputListInvalid                                                  = 1005020;

  /// Mixed stream task output list is null
  static const int MixerOutputListInvalid                                                 = 1005021;

  /// Invalid mixed stream task video configuration
  static const int MixerVideoConfigInvalid                                                = 1005023;

  /// Invalid mixed stream task vaudio configuration, Please check if an unsupported codec is used
  static const int MixerAudioConfigInvalid                                                = 1005024;

  /// More than the maximum number of input streams, supporting a maximum of 9 input streams
  static const int MixerExceedMaxInputCount                                               = 1005025;

  /// Input stream does not exist
  static const int MixerInputStreamNotExists                                              = 1005026;

  /// Mixed stream input parameters are wrong. Maybe the layout of the input stream exceed the canvas range
  static const int MixerInputParametersError                                              = 1005027;

  /// More than the maximum number of output streams, supporting a maximum of 3 output streams
  static const int MixerExceedMaxOutputCount                                              = 1005030;

  /// Mixed stream authentication failed
  static const int MixerAuthenticationFailed                                              = 1005050;

  /// Input watermark is null
  static const int MixerWatermarkNull                                                     = 1005061;

  /// The input watermark parameter is wrong. Maybe the layout exceed the canvas range
  static const int MixerWatermarkParametersError                                          = 1005062;

  /// Illegal input watermark URL, must start with preset-id://, and must end with .jpg or .png
  static const int MixerWatermarkUrlInvalid                                               = 1005063;

  /// Illegal input background image URL, must start with preset-id://, and must end with .jpg or .png
  static const int MixerBackgroundImageUrlInvalid                                         = 1005067;

  /// The auto-mix stream server was not found. Please contact technical support to enable it
  static const int MixerAutoMixStreamServerNotFound                                       = 1005070;

  /// Mixed stream internal error
  static const int MixerInnerError                                                        = 1005099;

  /// Device general error
  static const int DeviceErrorTypeGeneric                                                 = 1006001;

  /// Device ID does not exist
  static const int DeviceErrorTypeInvalidId                                               = 1006002;

  /// No device permission, please check the permission of camera or microphone
  static const int DeviceErrorTypeNoAuthorization                                         = 1006003;

  /// Video device acquisition frame rate is 0
  static const int DeviceErrorTypeZeroFps                                                 = 1006004;

  /// Equipment occupied
  static const int DeviceErrorTypeInUseByOther                                            = 1006005;

  /// Device unplugged
  static const int DeviceErrorTypeUnplugged                                               = 1006006;

  /// Device needs to be restarted
  static const int DeviceErrorTypeRebootRequired                                          = 1006007;

  /// Device media lost
  static const int DeviceErrorMediaServicesLost                                           = 1006008;

  /// When releasing the release device list, the list is empty
  static const int DeviceFreeDeviceListNull                                               = 1006020;

  /// Device internal error
  static const int DeviceInnerError                                                       = 1006099;

  /// Beautify option is invalid, please check the input parameters
  static const int PreprocessBeautifyOptionInvalid                                        = 1007005;

  /// The mediaplayer instance is not created
  static const int MediaPlayerNoInstance                                                  = 1008001;

  /// The mediaplayer play failed. Resource file is not loaded
  static const int MediaPlayerNoFilePath                                                  = 1008003;

  /// The mediaplayer failed to load the file. The format file is not supported
  static const int MediaPlayerFileFormatError                                             = 1008005;

  /// The mediaplayer failed to load the file. The  file path does not exist
  static const int MediaPlayerFilePathNotExists                                           = 1008006;

  /// The mediaplayer failed to load the file.  Decoding error
  static const int MediaPlayerFileDecodeError                                             = 1008007;

  /// The mediaplayer failed to load the file.  No supported audio/video stream
  static const int MediaPlayerFileNoSupportedStream                                       = 1008008;

  /// The mediaplayer play failed.  Demux error
  static const int MediaPlayerDemuxError                                                  = 1008010;

  /// The mediaplayer seek failed. Maybe the file hasn't loaded yet
  static const int MediaPlayerSeekError                                                   = 1008016;

  /// The mediaplayer has set a video data format that is not supported by the platform (eg CVPixelBuffer on iOS platform does not support NV21)
  static const int MediaPlayerPlatformFormatNotSupported                                  = 1008020;

  /// The mediaplayer creation instance exceeds maximum limit, up to 4 instances can be created
  static const int MediaPlayerExceedMaxCount                                              = 1008030;

  /// Mediaplayer internal error
  static const int MediaPlayerInnerError                                                  = 1008099;

  /// Input message content is empty
  static const int IMContentNull                                                          = 1009001;

  /// The input message content is too long, with a maximum length of 1024 bytes
  static const int IMContentTooLong                                                       = 1009002;

  /// Failed to send message, possibly due to network reasons
  static const int IMSendFailed                                                           = 1009010;

  /// The custom video capturer is not created, be sure to use it after you receive 'onStart' callback
  static const int CustomVideoIOCapturerNotCreated                                        = 1011001;

  /// The custom video capture module is not open. Make sure that the custom video capture is enabled in the initialization configuration
  static const int CustomVideoIONoCustomVideoCapture                                      = 1011002;

  /// The currently custom video capture format does not support this API
  static const int CustomVideoIONotSupportedFormat                                        = 1011010;

}
