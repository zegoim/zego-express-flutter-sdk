
class ZegoErrorCode {

  /// Execution successful.
  static const int CommonSuccess                                                          = 0;

  /// Engine not yet created. Please create the engine first before calling non-static functions.
  static const int CommonEngineNotCreate                                                  = 1000001;

  /// Not yet logged in to any room. Please log in to a room before publishing or playing streams.
  static const int CommonNotLoginRoom                                                     = 1000002;

  /// Engine not yet started. Please call `startPreviewView`, `startPublishing`, or `startPlayingStream` to get the engine started first.
  static const int CommonEngineNotStarted                                                 = 1000003;

  /// An API not supported by the current platform is called; for example, an API that sets the Android context is called on a non-Android platform.
  static const int CommonUnsupportedPlatform                                              = 1000006;

  /// Invalid Android context.
  static const int CommonInvalidAndroidEnvironment                                        = 1000007;

  /// The event handler has already been set by calling `setEventHandler`. Please do not repeat the setting. If you do need to set up the event handler again, please call `setEventHandler` to set it to null first before applying the new setting.
  static const int CommonEventHandlerExists                                               = 1000008;

  /// This feature is not included in the SDK. Please contact ZEGO technical support.
  static const int CommonSdkNoModule                                                      = 1000010;

  /// The input stream ID is too long. The maximum length should be less than 256 bytes.
  static const int CommonStreamIdTooLong                                                  = 1000014;

  /// The input stream ID is empty.
  static const int CommonStreamIdNull                                                     = 1000015;

  /// The input stream ID contains invalid characters.
  static const int CommonStreamIdInvalidCharacter                                         = 1000016;

  /// This AppID has been removed from production.
  static const int CommonAppOfflineError                                                  = 1000037;

  /// There is an error in the backend configurations. Please check the configuration items of the app.
  static const int CommonAppFlexiableConfigError                                          = 1000038;

  /// Incorrect CDN address. Please check the supported protocol and format.
  static const int CommonCdnUrlInvalid                                                    = 1000055;

  /// DNS resolution failed. Please check network configurations.
  static const int CommonDnsResolveError                                                  = 1000060;

  /// Server dispatching exception. Please contact ZEGO technical support to solve the problem.
  static const int CommonDispatchError                                                    = 1000065;

  /// Internal null pointer error. Please contact ZEGO technical support to solve the problem.
  static const int CommonInnerNullptr                                                     = 1000090;

  /// AppID cannot be 0. Please check if the AppID is correct.
  static const int EngineAppidZero                                                        = 1001000;

  /// The length of the input AppSign must be 64 bytes.
  static const int EngineAppsignInvalidLength                                             = 1001001;

  /// The input AppSign contains invalid characters. Only '0'-'9', 'a'-'f', 'A'-'F' are valid.
  static const int EngineAppsignInvalidCharacter                                          = 1001002;

  /// The input AppSign is empty.
  static const int EngineAppsignNull                                                      = 1001003;

  /// Authentication failed. Please check if the AppID is correct, or whether the production environment was selected for SDK initialization without prior go-live process.
  static const int EngineAppidIncorrectOrNotOnline                                        = 1001004;

  /// AppSign authentication failed. Please check if the AppSign is correct.
  static const int EngineAppsignIncorrect                                                 = 1001005;

  /// No write permission to the log file.
  static const int EngineLogNoWritePermission                                             = 1001014;

  /// The log file path is too long.
  static const int EngineLogPathTooLong                                                   = 1001015;

  /// The number of rooms the user attempted to log into simultaneously exceeds the maximum number allowed. Currently, a user can only be logged in to one main room and one multi room at the same time.
  static const int RoomCountExceed                                                        = 1002001;

  /// The input user ID is empty.
  static const int RoomUserIdNull                                                         = 1002005;

  /// The input user ID contains invalid characters.
  static const int RoomUserIdInvalidCharacter                                             = 1002006;

  /// The input user ID is too long. The maximum length should be less than 64 bytes.
  static const int RoomUserIdTooLong                                                      = 1002007;

  /// The input user name is empty.
  static const int RoomUserNameNull                                                       = 1002008;

  /// The input user name contains invalid characters.
  static const int RoomUserNameInvalidCharacter                                           = 1002009;

  /// The input user name is too long. The maximum length should be less than 256 bytes.
  static const int RoomUserNameTooLong                                                    = 1002010;

  /// The input room ID is empty.
  static const int RoomRoomidNull                                                         = 1002011;

  /// The input room ID contains invalid characters.
  static const int RoomRoomidInvalidCharacter                                             = 1002012;

  /// The input room ID is too long. The maximum length should be less than 128 bytes.
  static const int RoomRoomidTooLong                                                      = 1002013;

  /// The key for room extra info is empty.
  static const int RoomRoomExtraInfoKeyEmpty                                              = 1002014;

  /// The key for room extra info is too long. The maximum length should be less than 128 bytes.
  static const int RoomRoomExtraInfoKeyTooLong                                            = 1002015;

  /// The value for room extra info is too long. The maximum length should be less than 4096 bytes.
  static const int RoomRoomExtraInfoValueTooLong                                          = 1002016;

  /// The set key of the room extra info exceeds the maximum number supported. If you need to modify the number of setting keys, please contact ZEGO technical support.
  static const int RoomRoomExtraInfoExceedKeys                                            = 1002017;

  /// Login failed, possibly due to network problems.
  static const int RoomErrorConnectFailed                                                 = 1002030;

  /// Login timed out, possibly due to network problems.
  static const int RoomErrorLoginTimeout                                                  = 1002031;

  /// Room login authentication failed.
  static const int RoomErrorAuthenticationFailed                                          = 1002033;

  /// The number of users logging into the room exceeds the maximum number of concurrent users configured for the room.
  static const int RoomErrorExceedMaximumMember                                           = 1002034;

  /// The user is kicked out of the room, possibly because the same user ID is logged in on another device.
  static const int RoomKickedOut                                                          = 1002050;

  /// Room connection is temporarily interrupted, possibly due to network problems. Retrying...
  static const int RoomConnectTemporaryBroken                                             = 1002051;

  /// Room disconnected, possibly due to network problems.
  static const int RoomDisconnect                                                         = 1002052;

  /// Room login retry has exceeded the maximum retry time.
  static const int RoomRetryTimeout                                                       = 1002053;

  /// The business server has sent a signal to kick the user out of the room. Please check the reason for the kick-out.
  static const int RoomManualKickedOut                                                    = 1002055;

  /// You must log in to the main room with `loginRoom` before logging in to multi room
  static const int RoomWrongLoginSequence                                                 = 1002061;

  /// You must log out of the multi room before logging out of the main room
  static const int RoomWrongLogoutSequence                                                = 1002062;

  /// No multi-room permission, please contact ZEGO technical support to enable it.
  static const int RoomNoMultiRoomPermission                                              = 1002063;

  /// Room ID has been used by other login room interface
  static const int RoomRoomIdHasBeenUsed                                                  = 1002064;

  /// Room login failed due to internal system exceptions.
  static const int RoomInnerError                                                         = 1002099;

  /// Stream publishing failed, possibly due to no data in the stream.
  static const int PublisherPublishStreamFailed                                           = 1003001;

  /// Incorrect bitrate setting. Please check if the bitrate value is in the correct unit (kbps).
  static const int PublisherBitrateInvalid                                                = 1003002;

  /// Incorrect setting of stream publishing traffic control parameters.
  static const int PublisherTrafficModeInvalid                                            = 1003005;

  /// Stream publishing is temporarily interrupted. Retrying...
  static const int PublisherErrorNetworkInterrupt                                         = 1003020;

  /// Stream publish retry has exceeds the maximum retry time.
  static const int PublisherErrorRetryTimeout                                             = 1003021;

  /// Failed to publish the stream. The user is already publishing streams.
  static const int PublisherErrorAlreadyDoPublish                                         = 1003023;

  /// Failed to publish the stream. Publishing of this stream is prohibited by backend configuration.
  static const int PublisherErrorServerForbid                                             = 1003025;

  /// Failed to publish the stream. The same stream already exists in the room.
  static const int PublisherErrorRepetitivePublishStream                                  = 1003028;

  /// The connection to the RTMP server is interrupted. Please check whether there is any problem with the network connection or the stream publishing URL.
  static const int PublisherRtmpServerDisconnect                                          = 1003029;

  /// Failed to get status updates of relayed streaming to CDN. Please check whether the URL is valid.
  static const int PublisherUpdateCdnTargetError                                          = 1003040;

  /// Failed to send SEI. The SEI data is null.
  static const int PublisherSeiDataNull                                                   = 1003043;

  /// Failed to send SEI because the SEI data is too long. The maximum length should be less than 4096 bytes.
  static const int PublisherSeiDataTooLong                                                = 1003044;

  /// The extra info of the stream is null.
  static const int PublisherExtraInfoNull                                                 = 1003050;

  /// The extra info of the stream is too long. The maximum length should be less than 1024 bytes.
  static const int PublisherExtraInfoTooLong                                              = 1003051;

  /// Failed to update the extra info of the stream. Please check the network connection.
  static const int PublisherUpdateExtraInfoFailed                                         = 1003053;

  /// The watermark URL is null.
  static const int PublisherWatermarkUrlNull                                              = 1003055;

  /// The watermark URL is too long. The maximum length should be less than 1024 bytes.
  static const int PublisherWatermarkUrlTooLong                                           = 1003056;

  /// Invalid watermark format. The supported formats are `jpg` and `png`.
  static const int PublisherWatermarkUrlInvalid                                           = 1003057;

  /// Incorrect watermark layout. The layout area cannot exceed the encoding resolution.
  static const int PublisherWatermarkLayoutInvalid                                        = 1003058;

  /// Stream publishing failed due to system internal exceptions.
  static const int PublisherInnerError                                                    = 1003099;

  /// Stream playing failed, possibly due to no data in the stream.
  static const int PlayerPlayStreamFailed                                                 = 1004001;

  /// Stream playing failed because the stream does not exist. Please check whether the remote end publish is indeed successful, or whether the publish and play environment are inconsistent
  static const int PlayerPlayStreamNotExist                                               = 1004002;

  /// The number of streams the user attempted to play simultaneously exceeds the maximum number allowed. Currently, up to 12 steams can be played at the same time. Please contact ZEGO technical support to increase the capacity if necessary.
  static const int PlayerCountExceed                                                      = 1004010;

  /// Stream playing is temporarily interrupted. Retrying...
  static const int PlayerErrorNetworkInterrupt                                            = 1004020;

  /// Stream playing failed due to system internal exceptions.
  static const int PlayerInnerError                                                       = 1004099;

  /// Stream mixing service not yet enabled. Please contact ZEGO technical support to enable the service.
  static const int MixerNoServices                                                        = 1005000;

  /// The stream mixing task ID is null.
  static const int MixerTaskIdNull                                                        = 1005001;

  /// The stream mixing task ID is too long. The maximum length should be less than 256 bytes.
  static const int MixerTaskIdTooLong                                                     = 1005002;

  /// The stream mixing task ID contains invalid characters.
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

  /// The sampling frame rate of the video capture device is 0.
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

  /// MediaPlayer internal error.
  static const int MediaPlayerInnerError                                                  = 1008099;

  /// The input message content is empty.
  static const int IMContentNull                                                          = 1009001;

  /// The input message content is too long. The maximum length should be less than 1024 bytes.
  static const int IMContentTooLong                                                       = 1009002;

  /// Failed to send the message, possibly due to network problems.
  static const int IMSendFailed                                                           = 1009010;

  /// The file name suffix is not supported. Only .mp4 and .flv are supported currently. Depending on file name suffix, SDK sets the specified recording format accordingly.
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

}
