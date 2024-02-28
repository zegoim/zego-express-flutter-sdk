import 'package:flutter/material.dart';
import 'dart:typed_data';

// ignore_for_file: unnecessary_this, non_constant_identifier_names

/// Room scenario.
enum ZegoScenario {
  /// [Deprecated] Legacy general scenario, this scenario has been deprecated since version 3.0.0, and it is not recommended to use, please migrate to other new scenario as soon as possible.
  @Deprecated('Legacy general scenario')
  General,

  /// [Deprecated] Legacy communication scenario, this scenario has been deprecated since version 3.0.0, and it is not recommended to use, please migrate to other new scenario as soon as possible.
  @Deprecated('Legacy communication scenario')
  Communication,

  /// [Deprecated] Legacy live broadcast scenario, this scenario has been deprecated since version 3.0.0, and it is not recommended to use, please migrate to other new scenario as soon as possible.
  @Deprecated('Legacy live broadcast scenario')
  Live,

  /// Available since: 3.0.0. Description: The default (generic) scenario. If none of the following scenarios conform to your actual application scenario, this default scenario can be used.
  Default,

  /// Available since: 3.0.0. Description: Standard video call scenario, it is suitable for one-to-one video call scenarios.
  StandardVideoCall,

  /// Available since: 3.0.0. Description: High quality video call scenario, it is similar to the standard video call scenario, but this scenario uses a higher video frame rate, bit rate, and resolution (540p) by default, which is suitable for video call scenario with high image quality requirements.
  HighQualityVideoCall,

  /// Available since: 3.0.0. Description: Standard chatroom scenario, suitable for multi-person pure voice calls (low data usage). Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario.
  StandardChatroom,

  /// Available since: 3.0.0. Description: High quality chatroom scenario, it is similar to the standard chatroom scenario, but this scenario uses a higher audio bit rate than the standard chatroom scenario by default. It is suitable for multi-person pure voice call scenarios with high requirements on sound quality. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario. The web platform does not currently support this scenario.
  HighQualityChatroom,

  /// Available since: 3.0.0. Description: Live broadcast scenario, it is suitable for one-to-many live broadcast scenarios such as shows, games, e-commerce, and large educational classes. The audio and video quality, fluency, and compatibility have been optimized. Note: Even in live broadcast scenarios, the SDK has no business "roles" (such as anchors and viewers), and all users in the room can publish and play streams.
  Broadcast,

  /// Available since: 3.0.0. Description: Karaoke (KTV) scenario, it is suitable for real-time chorus and online karaoke scenarios, and has optimized delay, sound quality, ear return, echo cancellation, etc., and also ensures accurate alignment and ultra-low delay when multiple people chorus. The web platform does not currently support this scenario.
  Karaoke,

  /// Available since: 3.3.0. Description: Standard voice call scenario, it is suitable for one-to-one video or voice call scenarios. Note: On the ExpressVideo SDK, the camera is not enabled by default in this scenario. The web platform does not currently support this scenario.
  StandardVoiceCall
}

/// SDK feature type.
enum ZegoFeatureType {
  /// Basic audio feature.
  Audio,

  /// Basic video feature.
  Video,

  /// Media player feature.
  MediaPlayer,

  /// Local media data recorder feature.
  MediaDataRecorder,

  /// Media data publisher feature.
  MediaDataPublisher,

  /// Supplemental Enhancement Information (media side info) feature.
  SEI,

  /// SDK video capture feature.
  SdkVideoCapture,

  /// Custom video capture feature.
  CustomVideoCapture,

  /// SDK video rendering feature.
  SdkVideoRender,

  /// Custom video rendering feature.
  CustomVideoRender,

  /// SDK video processing feature (including low-light enhancement feature).
  SdkVideoProcessing,

  /// Custom video processing feature.
  CustomVideoProcessing,

  /// Streaming encryption / decryption feature.
  StreamEncryption,

  /// RTMP streaming feature.
  Rtmp,

  /// RTMPS streaming feature.
  Rtmps,

  /// RTMP over QUIC streaming feature.
  RtmpOverQuic,

  /// RTMP streaming feature.
  HttpFlv,

  /// HTTPS-FLV streaming feature.
  HttpsFlv,

  /// HTTP-FLV over QUIC streaming feature.
  HttpFlvOverQuic,

  /// Super resolution imaging feature.
  SuperResolution,

  /// Effects beauty feature.
  EffectsBeauty,

  /// Whiteboard beauty feature.
  Whiteboard,

  /// Range audio feature.
  RangeAudio,

  /// Copy righted music feature.
  CopyRightedMusic,

  /// Video object segmentation feature.
  VideoObjectSegmentation,

  /// Range scene feature. (3.0.0 and above support)
  RangeScene,

  /// Screen capture feature. (3.1.0 and above support)
  ScreenCapture,

  /// AI voice changer feature. (3.8.0 and above support)
  AIVoiceChanger
}

/// Language.
enum ZegoLanguage {
  /// English
  English,

  /// Chinese
  Chinese
}

/// Room mode.
enum ZegoRoomMode {
  /// Single room mode.
  SingleRoom,

  /// Multiple room mode.
  MultiRoom
}

/// Geo fence type.
enum ZegoGeoFenceType {
  /// Not use geo fence.
  None,

  /// Include the specified geo fence areas.
  Include,

  /// Exclude the specified geo fence areas.
  Exclude
}

/// Geo fence area code.
class ZegoGeoFenceAreaCode {
  /// Chinese mainland (excluding Hong Kong, Macao and Taiwan).
  static const int CN = 2;

  /// North America.
  static const int NA = 3;

  /// Europe, including the UK.
  static const int EU = 4;

  /// Asia, excluding Chinese mainland and India.
  static const int AS = 5;

  /// India.
  static const int IN = 6;
}

/// engine state.
enum ZegoEngineState {
  /// The engine has started
  Start,

  /// The engine has stoped
  Stop
}

/// Room state.
enum ZegoRoomState {
  /// Unconnected state, enter this state before logging in and after exiting the room. If there is a steady state abnormality in the process of logging in to the room, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is KickOut, it will enter this state.
  Disconnected,

  /// The state that the connection is being requested. It will enter this state after successful execution login room function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting connection status.
  Connecting,

  /// The status that is successfully connected. Entering this status indicates that the login to the room has been successful. The user can receive the callback notification of the user and the stream information in the room.
  Connected
}

/// Room state change reason.
enum ZegoRoomStateChangedReason {
  /// Logging in to the room. When calling [loginRoom] to log in to the room or [switchRoom] to switch to the target room, it will enter this state, indicating that it is requesting to connect to the server. The application interface is usually displayed through this state.
  Logining,

  /// Log in to the room successfully. When the room is successfully logged in or switched, it will enter this state, indicating that the login to the room has been successful, and users can normally receive callback notifications of other users in the room and all stream information additions and deletions.
  Logined,

  /// Failed to log in to the room. When the login or switch room fails, it will enter this state, indicating that the login or switch room has failed, for example, AppID or Token is incorrect, etc.
  LoginFailed,

  /// The room connection is temporarily interrupted. If the interruption occurs due to poor network quality, the SDK will retry internally.
  Reconnecting,

  /// The room is successfully reconnected. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after successful reconnection.
  Reconnected,

  /// The room fails to reconnect. If there is an interruption due to poor network quality, the SDK will retry internally, and enter this state after the reconnection fails.
  ReconnectFailed,

  /// Kicked out of the room by the server. For example, if you log in to the room with the same user name in other places, and the local end is kicked out of the room, it will enter this state.
  KickOut,

  /// Logout of the room is successful. It is in this state by default before logging into the room. When calling [logoutRoom] to log out of the room successfully or [switchRoom] to log out of the current room successfully, it will enter this state.
  Logout,

  /// Failed to log out of the room. Enter this state when calling [logoutRoom] fails to log out of the room or [switchRoom] fails to log out of the current room internally.
  LogoutFailed
}

/// Room mode.
enum ZegoRoomTransparentMessageMode {
  /// Single room mode.
  OnlyClient,

  /// Multiple room mode.
  OnlyServer,

  /// Multiple room mode.
  ClientAndServer
}

/// Room mode.
enum ZegoRoomTransparentMessageType {
  /// Single room mode.
  ZegoRoomTransparentMessageNormal,

  /// Multiple room mode.
  ZegoRoomTransparentMessageSequence
}

/// Publish channel.
enum ZegoPublishChannel {
  /// The main (default/first) publish channel.
  Main,

  /// The auxiliary (second) publish channel
  Aux,

  /// The third publish channel
  Third,

  /// The fourth publish channel
  Fourth
}

/// Publish CensorshipMode.
enum ZegoStreamCensorshipMode {
  /// no censorship.
  None,

  /// only censorship stream audio.
  Audio,

  /// only censorship stream video.
  Video,

  /// censorship stream audio and video.
  AudioAndVideo
}

/// Video rendering fill mode.
enum ZegoViewMode {
  /// The proportional scaling up, there may be black borders
  AspectFit,

  /// The proportional zoom fills the entire View and may be partially cut
  AspectFill,

  /// Fill the entire view, the image may be stretched
  ScaleToFill
}

/// Mirror mode for previewing or playing the of the stream.
enum ZegoVideoMirrorMode {
  /// The mirror image only for previewing locally. This mode is used by default. When the mobile terminal uses a rear camera, this mode is still used by default, but it does not work. Local preview does not set mirroring.
  OnlyPreviewMirror,

  /// Both the video previewed locally and the far end playing the stream will see mirror image.
  BothMirror,

  /// Both the video previewed locally and the far end playing the stream will not see mirror image.
  NoMirror,

  /// The mirror image only for far end playing the stream.
  OnlyPublishMirror
}

/// SEI type
enum ZegoSEIType {
  /// Using H.264 SEI (nalu type = 6, payload type = 243) type packaging, this type is not specified by the SEI standard, there is no conflict with the video encoder or the SEI in the video file, users do not need to follow the SEI content Do filtering, SDK uses this type by default.
  ZegoDefined,

  /// SEI (nalu type = 6, payload type = 5) of H.264 is used for packaging. The H.264 standard has a prescribed format for this type: startcode + nalu type (6) + payload type (5) + len + payload (uuid + content) + trailing bits. Because the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for streaming, such SEI may also exist in the video file, so when using this type, the user needs to use uuid + context as a buffer sending SEI. At this time, in order to distinguish the SEI generated by the video encoder itself, when the App sends this type of SEI, it can fill in the service-specific uuid (uuid length is 16 bytes). When the receiver uses the SDK to parse the SEI of the payload type 5, it will set filter string filters out the SEI matching the uuid and throws it to the business. If the filter string is not set, the SDK will throw all received SEI to the developer. uuid filter string setting function, [ZegoEngineConfig.advancedConfig("unregister_sei_filter","XXXXXX")], where unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
  UserUnregister
}

/// Publish stream status.
enum ZegoPublisherState {
  /// The state is not published, and it is in this state before publishing the stream. If a steady-state exception occurs in the publish process, such as AppID or Token are incorrect, or if other users are already publishing the stream, there will be a failure and enter this state.
  NoPublish,

  /// The state that it is requesting to publish the stream after the [startPublishingStream] function is successfully called. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
  PublishRequesting,

  /// The state that the stream is being published, entering the state indicates that the stream has been successfully published, and the user can communicate normally.
  Publishing
}

/// Voice changer preset value.
enum ZegoVoiceChangerPreset {
  /// No Voice changer
  None,

  /// Male to child voice (loli voice effect)
  MenToChild,

  /// Male to female voice (kindergarten voice effect)
  MenToWomen,

  /// Female to child voice
  WomenToChild,

  /// Female to male voice
  WomenToMen,

  /// Foreigner voice effect
  Foreigner,

  /// Autobot Optimus Prime voice effect
  OptimusPrime,

  /// Android robot voice effect
  Android,

  /// Ethereal voice effect
  Ethereal,

  /// Magnetic(Male) voice effect
  MaleMagnetic,

  /// Fresh(Female) voice effect
  FemaleFresh,

  /// Electronic effects in C major voice effect
  MajorC,

  /// Electronic effects in A minor voice effect
  MinorA,

  /// Electronic effects in harmonic minor voice effect
  HarmonicMinor,

  /// Female Vitality Sound effect
  FemaleEnergetic,

  /// Richness effect
  RichNess,

  /// Muffled effect
  Muffled,

  /// Roundness effect
  Roundness,

  /// Falsetto effect
  Falsetto,

  /// Fullness effect
  Fullness,

  /// Clear effect
  Clear,

  /// Hight effect
  HighlyResonant,

  /// Loud clear effect
  LoudClear,

  /// Minions effect
  Minions
}

/// Reverberation preset value.
enum ZegoReverbPreset {
  /// No Reverberation
  None,

  /// Soft room reverb effect
  SoftRoom,

  /// Large room reverb effect
  LargeRoom,

  /// Concert hall reverb effect
  ConcertHall,

  /// Valley reverb effect
  Valley,

  /// Recording studio reverb effect
  RecordingStudio,

  /// Basement reverb effect
  Basement,

  /// KTV reverb effect
  KTV,

  /// Popular reverb effect
  Popular,

  /// Rock reverb effect
  Rock,

  /// Vocal concert reverb effect
  VocalConcert,

  /// Gramophone reverb effect
  GramoPhone,

  /// Enhanced KTV reverb effect. Provide KTV effect with more concentrated voice and better brightness. Compared with the original KTV reverb effect, the reverberation time is shortened and the dry-wet ratio is increased.
  EnhancedKTV,

  /// Enhanced Rock reverb effect
  EnhancedRock,

  /// Enhanced misty reverb effect
  EnhancedMisty
}

/// Mode of Electronic Effects.
enum ZegoElectronicEffectsMode {
  /// Major
  Major,

  /// Minor
  Minor,

  /// Harmonic Minor
  HarmonicMinor
}

/// Video configuration resolution and bitrate preset enumeration. The preset resolutions are adapted for mobile and desktop. On mobile, height is longer than width, and desktop is the opposite. For example, 1080p is actually 1080(w) x 1920(h) on mobile and 1920(w) x 1080(h) on desktop.
enum ZegoVideoConfigPreset {
  /// Set the resolution to 320x180, the default is 15 fps, the code rate is 300 kbps
  Preset180P,

  /// Set the resolution to 480x270, the default is 15 fps, the code rate is 400 kbps
  Preset270P,

  /// Set the resolution to 640x360, the default is 15 fps, the code rate is 600 kbps
  Preset360P,

  /// Set the resolution to 960x540, the default is 15 fps, the code rate is 1200 kbps
  Preset540P,

  /// Set the resolution to 1280x720, the default is 15 fps, the code rate is 1500 kbps
  Preset720P,

  /// Set the resolution to 1920x1080, the default is 15 fps, the code rate is 3000 kbps
  Preset1080P
}

/// Stream quality level.
enum ZegoStreamQualityLevel {
  /// Excellent
  Excellent,

  /// Good
  Good,

  /// Normal
  Medium,

  /// Bad
  Bad,

  /// Failed
  Die,

  /// Unknown
  Unknown
}

/// Audio channel type.
enum ZegoAudioChannel {
  /// Unknown
  Unknown,

  /// Mono
  Mono,

  /// Stereo
  Stereo
}

/// Audio capture stereo mode.
enum ZegoAudioCaptureStereoMode {
  /// Disable stereo capture, that is, mono.
  None,

  /// Always enable stereo capture.
  Always,

  /// [Deprecated] Same as [Always], that is, always enable stereo capture, this mode has been deprecated since version 2.16.0.
  @Deprecated('Same as [Always], that is, always enable stereo capture')
  Adaptive
}

/// Audio mix mode.
enum ZegoAudioMixMode {
  /// Default mode, no special behavior
  Raw,

  /// Audio focus mode, which can highlight the sound of a certain stream in multiple audio streams
  Focused
}

/// Audio codec ID.
enum ZegoAudioCodecID {
  /// Default, determined by the [scenario] when calling [createEngine].
  Default,

  /// Can be used for RTC and CDN streaming; bitrate range from 10kbps to 128kbps; supports stereo; latency is around 500ms. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
  Normal,

  /// Can be used for RTC and CDN streaming; good compatibility; bitrate range from 16kbps to 192kbps; supports stereo; latency is around 350ms; the sound quality is worse than [Normal] in the same (low) bitrate. Server cloud transcoding is required when communicating with the Web SDK, and it is not required when relaying to CDN.
  Normal2,

  /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
  Normal3,

  /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming.
  Low,

  /// Not recommended; if you need to use it, please contact ZEGO technical support. Can only be used for RTC streaming; maximum bitrate is 16kbps.
  Low2,

  /// Can only be used for RTC streaming; bitrate range from 6kbps to 192kbps; supports stereo; latency is around 200ms; Under the same bitrate (low bitrate), the sound quality is significantly better than [Normal] and [Normal2]; low CPU overhead. Server cloud transcoding is not required when communicating with the Web SDK, and it is required when relaying to CDN.
  Low3
}

/// Video codec ID.
enum ZegoVideoCodecID {
  /// Default (H.264)
  Default,

  /// Scalable Video Coding (H.264 SVC)
  Svc,

  /// VP8
  Vp8,

  /// H.265
  H265,

  /// Dualstream Scalable Video Coding
  H264DualStream,

  /// Unknown Video Coding
  Unknown
}

/// Backend implementation of video codec.
enum ZegoVideoCodecBackend {
  /// Software or Hardware
  Any,

  /// Software
  Software,

  /// Hardware
  Hardware
}

/// Video stream type
enum ZegoVideoStreamType {
  /// The type to be played depends on the network status
  Default,

  /// small resolution type
  Small,

  /// big resolution type
  Big
}

/// Audio echo cancellation mode.
enum ZegoAECMode {
  /// Aggressive echo cancellation may affect the sound quality slightly, but the echo will be very clean.
  Aggressive,

  /// Moderate echo cancellation, which may slightly affect a little bit of sound, but the residual echo will be less.
  Medium,

  /// Comfortable echo cancellation, that is, echo cancellation does not affect the sound quality of the sound, and sometimes there may be a little echo, but it will not affect the normal listening.
  Soft
}

/// Active Noise Suppression mode.
enum ZegoANSMode {
  /// Soft ANS. In most instances, the sound quality will not be damaged, but some noise will remain.
  Soft,

  /// Medium ANS. It may damage some sound quality, but it has a good noise reduction effect.
  Medium,

  /// Aggressive ANS. It may significantly impair the sound quality, but it has a good noise reduction effect.
  Aggressive,

  /// AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
  AI,

  /// Balanced AI mode ANS. It will cause great damage to music, so it can not be used for noise suppression of sound sources that need to collect background sound. Please contact ZEGO technical support before use.
  AIBalanced
}

/// video encode profile.
enum ZegoEncodeProfile {
  /// The default video encode specifications, The default value is the video encoding specification at the Main level.
  Default,

  /// Baseline-level video encode specifications have low decoding consumption and poor picture effects. They are generally used for low-level applications or applications that require additional fault tolerance.
  Baseline,

  /// Main-level video encode specifications, decoding consumption is slightly higher than Baseline, the picture effect is also better, generally used in mainstream consumer electronic products.
  Main,

  /// High-level video encode specifications, decoding consumption is higher than Main, the picture effect is better, generally used for broadcasting and video disc storage, high-definition TV.
  High
}

/// Video rate control mode, the default mode is constant video rate.
enum ZegoVideoRateControlMode {
  /// Constant rate.
  ConstantRate,

  /// Constant quality, if this mode is used, the video rate fluctuates according to the network speed. For example, in the live broadcast of games, the constant quality mode will be used to improve the video quality in order to let the audience see smooth operation pictures.
  ConstantQuality
}

/// Stream alignment mode.
enum ZegoStreamAlignmentMode {
  /// Disable stream alignment.
  None,

  /// Streams should be aligned as much as possible, call the [setStreamAlignmentProperty] function to enable the stream alignment of the push stream network time alignment of the specified channel.
  Try
}

/// Traffic control property (bitmask enumeration).
class ZegoTrafficControlProperty {
  /// Basic (Adaptive (reduce) video bitrate)
  static const int Basic = 0;

  /// Adaptive (reduce) video FPS
  static const int AdaptiveFPS = 1;

  /// Adaptive (reduce) video resolution
  static const int AdaptiveResolution = 1 << 1;

  /// Adaptive (reduce) audio bitrate
  static const int AdaptiveAudioBitrate = 1 << 2;
}

/// Video transmission mode when current bitrate is lower than the set minimum bitrate.
enum ZegoTrafficControlMinVideoBitrateMode {
  /// Stop video transmission when current bitrate is lower than the set minimum bitrate
  NoVideo,

  /// Video is sent at a very low frequency (no more than 2fps) which is lower than the set minimum bitrate
  UltraLowFPS
}

/// Factors that trigger traffic control
enum ZegoTrafficControlFocusOnMode {
  /// Focus only on the local network
  ZegoTrafficControlFounsOnLocalOnly,

  /// Pay attention to the local network, but also take into account the remote network, currently only effective in the 1v1 scenario
  ZegoTrafficControlFounsOnRemote
}

/// Playing stream status.
enum ZegoPlayerState {
  /// The state of the flow is not played, and it is in this state before the stream is played. If the steady flow anomaly occurs during the playing process, such as AppID or Token are incorrect, it will enter this state.
  NoPlay,

  /// The state that the stream is being requested for playing. After the [startPlayingStream] function is successfully called, it will enter the state. The UI is usually displayed through this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to the requesting state.
  PlayRequesting,

  /// The state that the stream is being playing, entering the state indicates that the stream has been successfully played, and the user can communicate normally.
  Playing
}

/// Media event when playing.
enum ZegoPlayerMediaEvent {
  /// Audio stuck event when playing
  AudioBreakOccur,

  /// Audio stuck event recovery when playing
  AudioBreakResume,

  /// Video stuck event when playing
  VideoBreakOccur,

  /// Video stuck event recovery when playing
  VideoBreakResume
}

/// Resource Type.
enum ZegoResourceType {
  /// CDN
  CDN,

  /// RTC
  RTC,

  /// L3
  L3
}

/// Stream Resource Mode
enum ZegoStreamResourceMode {
  /// Default mode. The SDK will automatically select the streaming resource according to the cdnConfig parameters set by the player config and the ready-made background configuration.
  Default,

  /// Playing stream only from CDN.
  OnlyCDN,

  /// Playing stream only from L3.
  OnlyL3,

  /// Playing stream only from RTC.
  OnlyRTC,

  /// CDN Plus mode. The SDK will automatically select the streaming resource according to the network condition.
  CDNPlus
}

/// Stream Switch Resource Mode
enum ZegoStreamResourceSwitchMode {
  /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
  Default,

  /// Auto switch to RTC resource when publishing.
  SwitchToRTC,

  /// Keep using original resource when publishing, not switch to RTC resource.
  KeepOriginal
}

/// Stream Resource Type
enum ZegoStreamResourceType {
  /// Default mode. The SDK will automatically select the streaming resource according to the parameters set by the player config and the ready-made background configuration.
  Default,

  /// CDN resource.
  CDN,

  /// L3 resource.
  L3
}

/// Update type.
enum ZegoUpdateType {
  /// Add
  Add,

  /// Delete
  Delete
}

/// Get room stream list type.
enum ZegoRoomStreamListType {
  /// Play stream list
  Play,

  /// Room publish and play stream list
  All
}

/// State of CDN relay.
enum ZegoStreamRelayCDNState {
  /// The state indicates that there is no CDN relay
  NoRelay,

  /// The CDN relay is being requested
  RelayRequesting,

  /// Entering this status indicates that the CDN relay has been successful
  Relaying
}

/// Reason for state of CDN relay changed.
enum ZegoStreamRelayCDNUpdateReason {
  /// No error
  None,

  /// Server error
  ServerError,

  /// Handshake error
  HandshakeFailed,

  /// Access point error
  AccessPointError,

  /// Stream create failure
  CreateStreamFailed,

  /// Bad stream ID
  BadName,

  /// CDN server actively disconnected
  CDNServerDisconnected,

  /// Active disconnect
  Disconnected,

  /// All mixer input streams sessions closed
  MixStreamAllInputStreamClosed,

  /// All mixer input streams have no data
  MixStreamAllInputStreamNoData,

  /// Internal error of stream mixer server
  MixStreamServerInternalError
}

/// Beauty feature (bitmask enumeration).
class ZegoBeautifyFeature {
  /// No beautifying
  static const int None = 0;

  /// Polish
  static const int Polish = 1 << 0;

  /// Sharpen
  static const int Whiten = 1 << 1;

  /// Skin whiten
  static const int SkinWhiten = 1 << 2;

  /// Whiten
  static const int Sharpen = 1 << 3;
}

/// Device type.
enum ZegoDeviceType {
  /// Unknown device type.
  Unknown,

  /// Camera device.
  Camera,

  /// Microphone device.
  Microphone,

  /// Speaker device.
  Speaker,

  /// Audio device. (Other audio device that cannot be accurately classified into microphones or speakers.)
  AudioDevice,

  /// Audio Session.
  AudioSession
}

/// The exception type for the device.
enum ZegoDeviceExceptionType {
  /// Unknown device exception.
  Unknown,

  /// Generic device exception.
  Generic,

  /// Invalid device ID exception.
  InvalidId,

  /// Device permission is not granted.
  PermissionNotGranted,

  /// The capture frame rate of the device is 0.
  ZeroCaptureFps,

  /// The device is being occupied.
  DeviceOccupied,

  /// The device is unplugged (not plugged in).
  DeviceUnplugged,

  /// The device requires the system to restart before it can work (Windows platform only).
  RebootRequired,

  /// The system media service is unavailable, e.g. when the iOS system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services (Apple platform only).
  MediaServicesWereLost,

  /// The device is being occupied by Siri (Apple platform only).
  SiriIsRecording,

  /// The device captured sound level is too low (Windows platform only).
  SoundLevelTooLow,

  /// The device is being occupied, and maybe cause by iPad magnetic case (Apple platform only).
  MagneticCase,

  /// Audio session deactive (Apple platform only).
  AudioSessionDeactive,

  /// Audio session category change (Apple platform only).
  AudioSessionCategoryChange
}

/// Remote device status.
enum ZegoRemoteDeviceState {
  /// Device on
  Open,

  /// General device error
  GenericError,

  /// Invalid device ID
  InvalidID,

  /// No permission
  NoAuthorization,

  /// Captured frame rate is 0
  ZeroFPS,

  /// The device is occupied
  InUseByOther,

  /// The device is not plugged in or unplugged
  Unplugged,

  /// The system needs to be restarted
  RebootRequired,

  /// System media services stop, such as under the iOS platform, when the system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services.
  SystemMediaServicesLost,

  /// The remote user calls [enableCamera] or [enableAudioCaptureDevice] to disable the camera or microphone.
  Disable,

  /// The remote user actively calls [muteMicrophone] or [mutePublishStreamAudio] or [mutePublishStreamVideo] to stop publish the audio or video stream.
  Mute,

  /// The device is interrupted, such as a phone call interruption, etc.
  Interruption,

  /// There are multiple apps at the same time in the foreground, such as the iPad app split screen, the system will prohibit all apps from using the camera.
  InBackground,

  /// CDN server actively disconnected
  MultiForegroundApp,

  /// The system is under high load pressure and may cause abnormal equipment.
  BySystemPressure,

  /// The remote device is not supported to publish the device state.
  NotSupport
}

/// Audio device type.
enum ZegoAudioDeviceType {
  /// Audio input type
  Input,

  /// Audio output type
  Output
}

/// Audio device mode.
enum ZegoAudioDeviceMode {
  /// Enable system echo cancellation.
  Communication,

  /// The system echo cancellation function is disabled.
  General,

  /// Automatically select whether to enable system echo cancellation based on the scenario.
  Auto,

  /// Enable system echo cancellation. Compared with Communication, this mode always occupies the microphone device.
  Communication2,

  /// Enable system echo cancellation. Compared with Communication, in this mode, the microphone is released and the media volume is reduced.
  Communication3,

  /// Disable system echo cancellation. Compared with General, this mode is not released when a microphone device is used.
  General2,

  /// Disable system echo cancellation. Compared with General, this mode will always occupy the microphone device.
  General3,

  /// Enable system echo cancellation. Compared with Communication, this mode of wheat after releasing the microphone, maintain the call volume.
  Communication4
}

/// Audio route
enum ZegoAudioRoute {
  /// Speaker
  Speaker,

  /// Headphone
  Headphone,

  /// Bluetooth device
  Bluetooth,

  /// Receiver
  Receiver,

  /// External USB audio device
  ExternalUSB,

  /// Apple AirPlay
  AirPlay
}

/// Mix stream content type.
enum ZegoMixerInputContentType {
  /// Mix stream for audio only
  Audio,

  /// Mix stream for both audio and video
  Video,

  /// Mix stream for video only. On web platforms, this property does not take effect
  VideoOnly
}

/// Capture pipeline scale mode.
enum ZegoCapturePipelineScaleMode {
  /// Zoom immediately after acquisition, default
  Pre,

  /// Scaling while encoding
  Post
}

/// Video frame format.
enum ZegoVideoFrameFormat {
  /// Unknown format, will take platform default
  Unknown,

  /// I420 (YUV420Planar) format
  I420,

  /// NV12 (YUV420SemiPlanar) format
  NV12,

  /// NV21 (YUV420SemiPlanar) format
  NV21,

  /// BGRA32 format
  BGRA32,

  /// RGBA32 format
  RGBA32,

  /// ARGB32 format
  ARGB32,

  /// ABGR32 format
  ABGR32,

  /// I422 (YUV422Planar) format
  I422,

  /// BGR24 format
  BGR24,

  /// RGB24 format
  RGB24
}

/// Video frame buffer type.
enum ZegoVideoBufferType {
  /// Raw data type video frame
  Unknown,

  /// Raw data type video frame
  RawData,

  /// Encoded data type video frame
  EncodedData,

  /// Texture 2D type video frame
  GLTexture2D,

  /// CVPixelBuffer type video frame
  CVPixelBuffer,

  /// Surface Texture type video frame
  SurfaceTexture,

  /// GL_TEXTURE_EXTERNAL_OES type video frame
  GLTextureExternalOES,

  /// Texture 2D and raw data type video frame
  GLTexture2DAndRawData,

  /// D3D Texture2D type video frame
  D3DTexture2D,

  /// CVPixelBuffer type nv12 format video frame
  NV12CVPixelBuffer
}

/// Video frame format series.
enum ZegoVideoFrameFormatSeries {
  /// RGB series
  RGB,

  /// YUV series
  YUV
}

/// Video frame flip mode.
enum ZegoVideoFlipMode {
  /// No flip
  None,

  /// X-axis flip
  X,

  /// Y-axis flip
  Y,

  /// X-Y-axis flip
  XY
}

/// Audio Config Preset.
enum ZegoAudioConfigPreset {
  /// Basic sound quality (16 kbps, Mono, ZegoAudioCodecIDDefault)
  BasicQuality,

  /// Standard sound quality (48 kbps, Mono, ZegoAudioCodecIDDefault)
  StandardQuality,

  /// Standard sound quality (56 kbps, Stereo, ZegoAudioCodecIDDefault)
  StandardQualityStereo,

  /// High sound quality (128 kbps, Mono, ZegoAudioCodecIDDefault)
  HighQuality,

  /// High sound quality (192 kbps, Stereo, ZegoAudioCodecIDDefault)
  HighQualityStereo
}

/// Range audio mode
enum ZegoRangeAudioMode {
  /// World mode, you can communicate with everyone in the room.
  World,

  /// Team mode, only communicate with members of the team.
  Team,

  /// Secret team mode, communicate with members of the team (with team members), can hear the voices of members who within the audio receive range (except the team members).
  SecretTeam
}

/// Range audio speak mode
enum ZegoRangeAudioSpeakMode {
  /// All mode, his voice can be heard by members of the team and by those within range.
  All,

  /// Only world mode, only those within range can hear his voice.
  World,

  /// Only team mode, only members of the team can hear his voice (not restricted by range).
  Team
}

/// Range audio listening mode
enum ZegoRangeAudioListenMode {
  /// All mode, can hear everyone in the room.
  All,

  /// Only world mode, only listen to people within range.
  World,

  /// Only team mode, only listen to the voices of members of the team you belong to (not restricted by range).
  Team
}

/// Range audio microphone state.
enum ZegoRangeAudioMicrophoneState {
  /// The range audio microphone is off.
  Off,

  /// The range audio microphone is turning on.
  TurningOn,

  /// The range audio microphone is on.
  On
}

/// Player state.
enum ZegoMediaPlayerState {
  /// Not playing
  NoPlay,

  /// Playing
  Playing,

  /// Pausing
  Pausing,

  /// End of play
  PlayEnded
}

/// Player audio track mode.
enum ZegoMediaPlayerAudioTrackMode {
  /// Normal audio track mode
  Normal,

  /// Multiple audio track mode
  Multiple
}

/// Player network event.
enum ZegoMediaPlayerNetworkEvent {
  /// Network resources are not playing well, and start trying to cache data
  BufferBegin,

  /// Network resources can be played smoothly
  BufferEnded
}

/// Audio channel.
enum ZegoMediaPlayerAudioChannel {
  /// Audio channel left
  Left,

  /// Audio channel right
  Right,

  /// Audio channel all
  All
}

/// Media player first frame event type.
enum ZegoMediaPlayerFirstFrameEvent {
  /// The first video frame is rendered event.
  VideoRendered,

  /// The first audio frame is rendered event.
  AudioRendered
}

/// AudioEffectPlayer state.
enum ZegoAudioEffectPlayState {
  /// Not playing
  NoPlay,

  /// Playing
  Playing,

  /// Pausing
  Pausing,

  /// End of play
  PlayEnded
}

/// volume type.
enum ZegoVolumeType {
  /// volume local
  Local,

  /// volume remote
  Remote
}

/// audio sample rate.
enum ZegoAudioSampleRate {
  /// Unknown
  Unknown,

  /// 8K
  SampleRate8K,

  /// 16K
  SampleRate16K,

  /// 22.05K
  SampleRate22K,

  /// 24K
  SampleRate24K,

  /// 32K
  SampleRate32K,

  /// 44.1K
  SampleRate44K,

  /// 48K
  SampleRate48K
}

/// Audio capture source type.
enum ZegoAudioSourceType {
  /// Default audio capture source (the main channel uses custom audio capture by default; the aux channel uses the same sound as main channel by default).
  Default,

  /// Use custom audio capture, refer to [enableCustomAudioIO] or [setAudioSource]. The web platform does not currently support.
  Custom,

  /// Use media player as audio source, only support aux channel. The web platform does not currently support.
  MediaPlayer,

  /// No audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
  None,

  /// Using microphone as audio source. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
  Microphone,

  /// Using main channel as audio source. Ineffective when used in main channel. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface. The web platform does not currently support.
  MainPublishChannel,

  /// Using screen capture as audio source. Typically used in mobile screen sharing scenarios. This audio source type can only be used in [setAudioSource] interface, has no effect when used in [enableCustomAudioIO] interface.
  ScreenCapture
}

/// Record type.
enum ZegoDataRecordType {
  /// This field indicates that the Express-Audio SDK records audio by default, and the Express-Video SDK records audio and video by default. When recording files in .aac format, audio is also recorded by default.
  Default,

  /// only record audio
  OnlyAudio,

  /// only record video, Audio SDK and recording .aac format files are invalid.
  OnlyVideo,

  /// record audio and video. Express-Audio SDK and .aac format files are recorded only audio.
  AudioAndVideo
}

/// Record state.
enum ZegoDataRecordState {
  /// Unrecorded state, which is the state when a recording error occurs or before recording starts.
  NoRecord,

  /// Recording in progress, in this state after successfully call [startRecordingCapturedData] function
  Recording,

  /// Record successs
  Success
}

/// Audio data callback function enable bitmask enumeration.
class ZegoAudioDataCallbackBitMask {
  /// The mask bit of this field corresponds to the enable [onCapturedAudioData] callback function
  static const int Captured = 1 << 0;

  /// The mask bit of this field corresponds to the enable [onPlaybackAudioData] callback function
  static const int Playback = 1 << 1;

  /// The mask bit of this field corresponds to the enable [onMixedAudioData] callback function
  static const int Mixed = 1 << 2;

  /// The mask bit of this field corresponds to the enable [onPlayerAudioData] callback function
  static const int Player = 1 << 3;
}

/// Network mode
enum ZegoNetworkMode {
  /// Offline (No network)
  Offline,

  /// Unknown network mode
  Unknown,

  /// Wired Ethernet (LAN)
  Ethernet,

  /// Wi-Fi (WLAN)
  WiFi,

  /// 2G Network (GPRS/EDGE/CDMA1x/etc.)
  Mode2G,

  /// 3G Network (WCDMA/HSDPA/EVDO/etc.)
  Mode3G,

  /// 4G Network (LTE)
  Mode4G,

  /// 5G Network (NR (NSA/SA))
  Mode5G
}

/// network speed test type
enum ZegoNetworkSpeedTestType {
  /// uplink
  Uplink,

  /// downlink
  Downlink
}

/// VOD billing mode.
enum ZegoCopyrightedMusicBillingMode {
  /// Pay-per-use.Each time a user obtains a song resource, a charge is required, that is, the user will be charged for each time based on the actual call to obtain the song resource interface (such as [requestSong], [requestAccompaniment], etc.).
  Count,

  /// Monthly billing by user.Billing for a single user is based on the monthly dimension, that is, the statistics call to obtain song resources (such as [requestSong], [requestAccompaniment], etc.) and the parameters are the user ID of the monthly subscription, and the charging is based on the monthly dimension.
  User,

  /// Monthly billing by room.The room users are billed on a monthly basis, that is, statistical calls to obtain song resources (such as [requestSong], [requestAccompaniment], etc.) are passed as Roomid for a monthly subscription of the room, and fees are charged on a monthly basis.
  Room,

  /// Monthly billing by master. Every time a user obtains a resource, it is counted as the owner’s acquisition of resources, that is, according to the actual call to obtain the song resource interface (such as [requestSong], [requestAccompaniment], etc.), the parameters are passed as the Roomid of the room and the Masterid of the owner, and the fee is charged according to the owner.
  Master
}

/// The music resource type. For [querycache] interface.
enum ZegoCopyrightedMusicType {
  /// Song. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicSong,

  /// Song with high quality. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicSongHQ,

  /// Song with super quality. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicSongSQ,

  /// Song accompaniment. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicAccompaniment,

  /// Song accompaniment clip. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicAccompanimentClip,

  /// Song accompaniment segment. Deprecated since version 3.9.0.
  @Deprecated(
      'Please use the [queryCache] interface that takes a ZegoCopyrightedMusicQueryCacheConfig as a parameter instead.')
  ZegoCopyrightedMusicAccompanimentSegment
}

/// The music resource type. For [ZegoCopyrightedMusicRequestConfig], [ZegoCopyrightedMusicGetSharedConfig] and [ZegoCopyrightedMusicQueryCacheConfig].
enum ZegoCopyrightedMusicResourceType {
  /// Song.
  ZegoCopyrightedMusicResourceSong,

  /// Song accompaniment.
  ZegoCopyrightedMusicResourceAccompaniment,

  /// Song accompaniment clip.
  ZegoCopyrightedMusicResourceAccompanimentClip,

  /// Song accompaniment segment.
  ZegoCopyrightedMusicResourceAccompanimentSegment
}

/// Copyright music resource song copyright provider. For more information about the copyright owner, please contact ZEGO business personnel.
enum ZegoCopyrightedMusicVendorID {
  /// Default copyright provider.
  ZegoCopyrightedMusicVendorDefault,

  /// First copyright provider.
  ZegoCopyrightedMusicVendor1,

  /// Second copyright provider.
  ZegoCopyrightedMusicVendor2,

  /// Third copyright provider.
  ZegoCopyrightedMusicVendor3
}

/// The music resource quality type. For [ZegoCopyrightedMusicQueryCacheConfig].
enum ZegoCopyrightedMusicResourceQualityType {
  /// Standard Definition Audio.
  ZegoCopyrightedMusicResourceQualityNormal,

  /// High Definition Audio.
  ZegoCopyrightedMusicResourceQualityHQ,

  /// Lossless Audio Quality.
  ZegoCopyrightedMusicResourceQualitySQ
}

/// Font type.
enum ZegoFontType {
  /// Source han sans.
  SourceHanSans,

  /// Alibaba sans.
  AlibabaSans,

  /// Pang men zheng dao title.
  PangMenZhengDaoTitle,

  /// HappyZcool.
  HappyZcool
}

/// Mixing stream video view render mode.
enum ZegoMixRenderMode {
  /// The proportional zoom fills the entire area and may be partially cut.
  Fill,

  /// Scale the filled area proportionally. If the scale does not match the set size after scaling, the extra part will be displayed as transparent.
  Fit
}

/// Camera focus mode.
enum ZegoCameraFocusMode {
  /// Auto focus.
  AutoFocus,

  /// Continuous auto focus.
  ContinuousAutoFocus
}

/// Camera exposure mode.
enum ZegoCameraExposureMode {
  /// Auto exposure.
  AutoExposure,

  /// Continuous auto exposure.
  ContinuousAutoExposure
}

/// voice activity detection type
enum ZegoAudioVADType {
  /// noise
  Noise,

  /// speech
  Speech
}

/// stable voice activity detection type
enum ZegoAudioVADStableStateMonitorType {
  /// captured
  Captured,

  /// custom processed
  CustomProcessed
}

/// Orientation mode of the video.
enum ZegoOrientationMode {
  /// Custom mode.Description: The default is the custom mode. In this mode, the user needs to set the orientation through [SetAppOrientation], and set the video resolution through [SetVideoConfig] to control the video ratio. The SDK rotates the video at the stream publishing end.
  Custom,

  /// Player self adaption mode.Description: The video orientation of the stream playing end is automatically vertically upward, and the user of the stream publishing end no longer needs to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Both the stream publishing end and the stream playing end need to be set to [ZegoOrientationModeAdaption] mode. 2. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.  3. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual orientation of the device.
  Adaption,

  /// Player adapt to pulisher mode.Description: Taking the Status Bar as a reference, the video direction of the stream playing end is the same as the preview video direction of the stream publishing end. The SDK will use the Status Bar as a reference to rotate the image on the stream playing end, and the rotation angle is the same as the rotation angle of the preview on the stream publishing end. Stream publishing end users no longer need to set the orientation through [SetAppOrientation], and no longer need to set the video resolution to control the video ratio through [SetVideoConfig]. Caution: 1. Media players, cloud recording, local recording, and publish or play streaming scenarios via CDN are not supported.2. In this mode, the SDK will automatically swap the width and height of the encoding resolution according to the actual position of the Status Bar.
  Alignment,

  /// Fixed resolution ratio mode.Description: Taking the Status Bar as a reference, the video orientation of the stream playing end is the same as the previewed video direction of the stream publishing end, and the video resolution is the same as the encoding resolution. Users of the streaming end no longer need to set the orientation through [SetAppOrientation].
  FixedResolutionRatio
}

/// CDN network protocol types supported by ZEGO
enum ZegoCDNProtocol {
  /// TCP protocol
  TCP,

  /// QUIC protocol
  QUIC
}

/// Supported httpDNS service types.
enum ZegoHttpDNSType {
  /// None.
  None,

  /// wangsu httpdns.
  Wangsu,

  /// tencent httpdns.
  Tencent,

  /// aliyun httpdns.
  Aliyun
}

/// Publish or play stream event
enum ZegoStreamEvent {
  /// Start publishing stream
  PublishStart,

  /// The first publish stream was successful
  PublishSuccess,

  /// Failed to publish stream for the first time
  PublishFail,

  /// Start retrying publishing stream
  RetryPublishStart,

  /// Retry publishing stream successfully
  RetryPublishSuccess,

  /// Failed to retry publishing stream
  RetryPublishFail,

  /// End of publishing stream
  PublishEnd,

  /// Start playing stream
  PlayStart,

  /// The first play stream was successful
  PlaySuccess,

  /// Failed to play stream for the first time
  PlayFail,

  /// Start retrying playing stream
  RetryPlayStart,

  /// Retry playing stream successfully
  RetryPlaySuccess,

  /// Failed to retry playing stream
  RetryPlayFail,

  /// End of playing stream
  PlayEnd
}

/// Type of capture target.
enum ZegoScreenCaptureSourceType {
  /// Unknow.
  Unknow,

  /// The capture target is a certain window.
  Window,

  /// The capture target is the screen of a certain monitor.
  Screen
}

/// Low light enhanced mode.
enum ZegoLowlightEnhancementMode {
  /// Turn off the low-light enhancement, and the brightness of the captured image remains unchanged.
  Off,

  /// Turn on low-light enhancement to enhance the brightness of the captured image.
  On,

  /// Automatic low-light enhancement, the brightness enhancement value of the captured picture is dynamically adjusted with the ambient brightness.
  Auto
}

/// Super resolution mode.
enum ZegoSuperResolutionState {
  /// Super resolution turned off.
  Off,

  /// Super resolution turned on.
  On
}

/// video capture source.
enum ZegoVideoSourceType {
  /// No capture, i.e. no video data.
  None,

  /// Video source from camera.
  Camera,

  /// Video source from custom capture. The web platform does not currently support.
  Custom,

  /// Video source from the main publish channel. When publishing the main channel, this value cannot be set. The web platform does not currently support.
  MainPublishChannel,

  /// Video source from media player. The web platform does not currently support.
  Player,

  /// Video source from screen capture.
  ScreenCapture,

  /// [Deprecated] this video source type has been deprecated since version 3.2.0.
  @Deprecated('Which video source to be used will determined by SDK')
  ZegoVideoSourceDefault,

  /// [Deprecated] Same as [Camera], that is, no capture, this video source type has been deprecated since version 3.2.0.
  @Deprecated('Same as [None], that is, no capture')
  ZegoVideoSourceNone,

  /// [Deprecated] Same as [Camera], that is, video source from camera, this video source type has been deprecated since version 3.2.0.
  @Deprecated('Same as [Camera], that is, video source from camera')
  ZegoVideoSourceCamera,

  /// [Deprecated] Same as [Custom], that is, video source from custom capture, this video source type has been deprecated since version 3.2.0.
  @Deprecated('Same as [Custom], that is, video source from custom capture')
  ZegoVideoSourceExternalCapture,

  /// [Deprecated] Same as [MainPublishChannel], that is, video source from the main publish channel, this video source type has been deprecated since version 3.2.0.
  @Deprecated(
      'Same as [MainPublishChannel], that is, video source from the main publish channel')
  ZegoVideoSourceMainPublishChannel,

  /// [Deprecated] Same as [Player], that is, video source from media player, this video source type has been deprecated since version 3.2.0.
  @Deprecated('Same as [Player], that is, video source from media player')
  ZegoVideoSourcePlayer,

  /// [Deprecated] Same as [ScreenCapture], that is, video source from screen capture, this video source type has been deprecated since version 3.2.0.
  @Deprecated(
      'Same as [ScreenCapture], that is, video source from screen capture')
  ZegoVideoSourceScreenCapture
}

/// Screen capture source exception type.
enum ZegoScreenCaptureSourceExceptionType {
  /// Unknown exception type.
  Unknown,

  /// The capture target fails, such as the monitor is unplugged and the window is closed.
  Invalid,

  /// Failed to collect target, internal reasons of the system.
  Failed,

  /// The set publish region is invalid, and the publish region is not within the capture screen region.
  PublishRegionInvalid
}

/// Screen capture source exception type. (only for Android)
enum ZegoScreenCaptureExceptionType {
  /// Unknown exception type.
  Unknown,

  /// The video capture system version does not support it, and Android only supports 5.0 and above.
  VideoNotSupported,

  /// The capture target fails, such as the monitor is unplugged and the window is closed.
  AudioNotSupported,

  /// Audio recording object creation failed. Possible reasons: 1. The audio recording permission is not enabled; 2. The allocated memory for audio recording is insufficient; 3. The creation of AudioRecord fails.
  AudioCreateFailed,

  /// MediaProjection request for dynamic permissions was denied.
  MediaProjectionPermissionDenied,

  /// Capture is not started. Need to start capturing with [startScreenCapture] first.
  NotStartCapture,

  /// Screen capture has already started, repeated calls failed. You need to stop the capture with [stopScreenCapture] first.
  AlreadyStarted,

  /// Failed to start the foreground service.
  ForegroundServiceFailed,

  /// Before starting screen capture, you need to call [setVideoSource], [setAudioSource] to specify the video and audio source `ScreenCapture`.
  SourceNotSpecified,

  /// System error exception. For example, low memory, etc.
  SystemError
}

/// The state of the screen capture source window changes.
enum ZegoScreenCaptureWindowState {
  /// The window is on the current screen, and the coordinate area changes.
  OnScreen,

  /// The window leaves the current screen and pauses the capture.
  OffScreen,

  /// The window is destroy.
  Destroy
}

/// Multimedia load type.
enum ZegoMultimediaLoadType {
  /// Load by file path.
  FilePath,

  /// Load by memory.
  Memory,

  /// Load by copyrighted music resource ID.
  ResourceID
}

/// Alpha channel data layout.
enum ZegoAlphaLayoutType {
  /// There is no alpha data.
  None,

  /// Alpha channel data is to the left of RGB/YUV data.
  Left,

  /// Alpha channel data is to the right of RGB/YUV data.
  Right,

  /// Alpha channel data is to the bottom of RGB/YUV data.
  Bottom
}

/// Object segmentation type.
enum ZegoObjectSegmentationType {
  /// Any background object segmentation.
  AnyBackground,

  /// Green screen background object segmentation.
  GreenScreenBackground
}

/// Object segmentation state.
enum ZegoObjectSegmentationState {
  /// Object segmentation turned off.
  Off,

  /// Object segmentation turned on.
  On
}

/// Video background process type.
enum ZegoBackgroundProcessType {
  /// Background is transparent.
  Transparent,

  /// Fill the background with a solid color.
  Color,

  /// Blur background.
  Blur,

  /// The background is the specified image.
  Image,

  /// The background is the specified video.
  Video
}

/// Background blur level.
enum ZegoBackgroundBlurLevel {
  /// Background blur level low.
  Low,

  /// Background blur level medium.
  Medium,

  /// Background blur level high.
  High
}

/// The config of the media data publisher.
enum ZegoMediaDataPublisherMode {
  /// Both publish video and audio.
  BothVideoAndAudio,

  /// Only publish video.
  OnlyVideo
}

/// Live audio effect mode.
enum ZegoLiveAudioEffectMode {
  /// No audio effect.
  None,

  /// Only local play.
  Local,

  /// Only publish.
  Publish,

  /// Publish and local play.
  All
}

/// Media stream type.
enum ZegoMediaStreamType {
  /// Media audio stream.
  Audio,

  /// Media video stream.
  Video,

  /// Media audio and video stream.
  AV
}

/// Dump data type.
enum ZegoDumpDataType {
  /// Audio.
  Audio
}

/// Log config.
///
/// Description: This parameter is required when calling [setlogconfig] to customize log configuration.
/// Use cases: This configuration is required when you need to customize the log storage path or the maximum log file size.
/// Caution: None.
class ZegoLogConfig {
  /// The storage path of the log file. Description: Used to customize the storage path of the log file. Use cases: This configuration is required when you need to customize the log storage path. Required: False. Default value: The default path of each platform is different, please refer to the official website document https://docs.zegocloud.com/faq/express_sdkLog. Caution: Developers need to ensure read and write permissions for files under this path.
  String logPath;

  /// Maximum log file size(Bytes). Description: Used to customize the maximum log file size. Use cases: This configuration is required when you need to customize the upper limit of the log file size. Required: False. Default value: 5MB (5 * 1024 * 1024 Bytes). Value range: Minimum 1MB (1 * 1024 * 1024 Bytes), maximum 100M (100 * 1024 * 1024 Bytes), 0 means no need to write logs. Caution: The larger the upper limit of the log file size, the more log information it carries, but the log upload time will be longer.
  int logSize;

  /// Log files count. Default is 3. Value range is [3, 20].
  int? logCount;

  ZegoLogConfig(this.logPath, this.logSize, {this.logCount});
}

/// Custom video capture configuration.
///
/// Custom video capture, that is, the developer is responsible for collecting video data and sending the collected video data to SDK for video data encoding and publishing to the ZEGO RTC server. This feature is generally used by developers who use third-party beauty features or record game screen living.
/// When you need to use the custom video capture function, you need to set an instance of this class as a parameter to the [enableCustomVideoCapture] function.
/// Because when using custom video capture, SDK will no longer start the camera to capture video data. You need to collect video data from video sources by yourself.
class ZegoCustomVideoCaptureConfig {
  /// Custom video capture video frame data type
  ZegoVideoBufferType bufferType;

  ZegoCustomVideoCaptureConfig(this.bufferType);
}

/// Custom video process configuration.
class ZegoCustomVideoProcessConfig {
  /// Custom video process video frame data type
  ZegoVideoBufferType bufferType;

  ZegoCustomVideoProcessConfig(this.bufferType);
}

/// Custom video render configuration.
///
/// When you need to use the custom video render function, you need to set an instance of this class as a parameter to the [enableCustomVideoRender] function.
class ZegoCustomVideoRenderConfig {
  /// Custom video capture video frame data type
  ZegoVideoBufferType bufferType;

  /// Custom video rendering video frame data format。Useless when set bufferType as [EncodedData]
  ZegoVideoFrameFormatSeries frameFormatSeries;

  /// Whether the engine also renders while customizing video rendering. The default value is [false]. Useless when set bufferType as [EncodedData]
  bool enableEngineRender;

  ZegoCustomVideoRenderConfig(
      this.bufferType, this.frameFormatSeries, this.enableEngineRender);
}

/// Custom audio configuration.
class ZegoCustomAudioConfig {
  /// Audio capture source type
  ZegoAudioSourceType sourceType;

  ZegoCustomAudioConfig(this.sourceType);
}

/// Profile for create engine
///
/// Profile for create engine
class ZegoEngineProfile {
  /// Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
  int appID;

  /// Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows null or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
  String? appSign;

  /// The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
  ZegoScenario scenario;

  /// Set whether to use Platform View for rendering, true: rendering using Platform View, false: rendering using Texture, default is false. Currently the web platform only supports rendering with Platform View. When using the [createCanvasView] interface, If the preferred render mode is not supported, another render mode is automatically used.
  bool? enablePlatformView;

  ZegoEngineProfile(this.appID, this.scenario,
      {this.appSign, this.enablePlatformView});
}

/// Advanced engine configuration.
class ZegoEngineConfig {
  /// Log configuration, if not set, use the default configuration. It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].
  @Deprecated(
      'This property has been deprecated since version 2.3.0, please use the [setLogConfig] function instead.')
  ZegoLogConfig? logConfig;

  /// Other special function switches, if not set, no special function will be used by default. Please contact ZEGO technical support before use.
  Map<String, String>? advancedConfig;

  ZegoEngineConfig({this.logConfig, this.advancedConfig});
}

/// Proxy config.
///
/// Set proxy config.
class ZegoProxyInfo {
  /// ip. Under local proxy: local proxy ip configured by the developer. under cloud proxy: proxy ip provided by ZEGO (either  hostname or ip will do).
  String? ip;

  /// port. Under Local Proxy: Local proxy port configured by the developer. Under Cloud Proxy: Proxy port provided by ZEGO.
  int? port;

  /// hostname.Under Local Proxy: Local proxy hostname configured by the developer. Under cloud proxy: proxy hostname provided by ZEGO. proxy hostname (either hostname or ip will do).
  String? hostName;

  /// userName.Under Local Proxy: the authentication username of the local Proxy configured by the developer, can be ignored if there is none. Under cloud proxy: can be ignored.
  String? userName;

  /// password.Under local proxy: authentication password of local proxy configured by developer, can be ignored if there is none. Under cloud proxy: can be ignored.
  String? password;

  ZegoProxyInfo(
      {this.ip, this.port, this.hostName, this.userName, this.password});
}

/// Advanced room configuration.
///
/// Configure maximum number of users in the room and authentication token, etc.
class ZegoRoomConfig {
  /// The maximum number of users in the room, Passing 0 means unlimited, the default is unlimited.
  int maxMemberCount;

  /// Whether to enable the user in and out of the room callback notification [onRoomUserUpdate], the default is off. If developers need to use ZEGO Room user notifications, make sure that each user who login sets this flag to true
  bool isUserStatusNotify;

  /// The token issued by the developer's business server is used to ensure security. For the generation rules, please refer to [Using Token Authentication](https://doc-zh.zego.im/article/10360), the default is an empty string, that is, no authentication. In versions 2.17.0 and above, if appSign is not passed in when calling the [createEngine] API to create an engine, or if appSign is empty, this parameter must be set for authentication when logging in to a room.
  String token;

  ZegoRoomConfig(this.maxMemberCount, this.isUserStatusNotify, this.token);

  /// Create a default room configuration
  ZegoRoomConfig.defaultConfig()
      : maxMemberCount = 0,
        isUserStatusNotify = false,
        token = "";

  Map<String, dynamic> toMap() {
    return {
      'maxMemberCount': this.maxMemberCount,
      'isUserStatusNotify': this.isUserStatusNotify,
      'token': this.token
    };
  }
}

/// Video config.
///
/// Configure parameters used for publishing stream, such as bitrate, frame rate, and resolution.
/// Developers should note that the width and height resolution of the mobile and desktop are opposite. For example, 360p, the resolution of the mobile is 360x640, and the desktop is 640x360.
/// When using external capture, the capture and encoding resolution of RTC cannot be set to 0*0, otherwise, there will be no video data in the publishing stream in the entire engine life cycle.
class ZegoVideoConfig {
  /// Capture resolution width, control the width of camera image acquisition. SDK requires this member to be set to an even number. Only the camera is not started and the custom video capture is not used, the setting is effective. For performance reasons, the SDK scales the video frame to the encoding resolution after capturing from camera and before rendering to the preview view. Therefore, the resolution of the preview image is the encoding resolution. If you need the resolution of the preview image to be this value, Please call [setCapturePipelineScaleMode] first to change the capture pipeline scale mode to [Post]
  int captureWidth;

  /// Capture resolution height, control the height of camera image acquisition. SDK requires this member to be set to an even number. Only the camera is not started and the custom video capture is not used, the setting is effective. For performance reasons, the SDK scales the video frame to the encoding resolution after capturing from camera and before rendering to the preview view. Therefore, the resolution of the preview image is the encoding resolution. If you need the resolution of the preview image to be this value, Please call [setCapturePipelineScaleMode] first to change the capture pipeline scale mode to [Post]
  int captureHeight;

  /// Encode resolution width, control the image width of the encoder when publishing stream. SDK requires this member to be set to an even number. The settings before and after publishing stream can be effective
  int encodeWidth;

  /// Encode resolution height, control the image height of the encoder when publishing stream. SDK requires this member to be set to an even number. The settings before and after publishing stream can be effective
  int encodeHeight;

  /// Frame rate, control the frame rate of the camera and the frame rate of the encoder. Only the camera is not started, the setting is effective. Publishing stream set to 60 fps, playing stream to take effect need contact technical support
  int fps;

  /// Bit rate in kbps. The settings before and after publishing stream can be effective. The SDK will automatically set the bit rate suitable for the scenario selected by the developer. If the bit rate manually set by the developer exceeds the reasonable range, the SDK will automatically process the bit rate according to the reasonable range. If you need to configure a high bit rate due to business needs, please contact ZEGO Business.
  int bitrate;

  /// The codec id to be used, the default value is [default]. Settings only take effect before publishing stream
  ZegoVideoCodecID codecID;

  /// Video keyframe interval, in seconds. Required: No. Default value: 2 seconds. Value range: [2, 5]. Caution: The setting is only valid before pushing.
  int? keyFrameInterval;

  ZegoVideoConfig(this.captureWidth, this.captureHeight, this.encodeWidth,
      this.encodeHeight, this.fps, this.bitrate, this.codecID,
      {this.keyFrameInterval});

  /// Create video configuration with preset enumeration values
  ZegoVideoConfig.preset(ZegoVideoConfigPreset preset)
      : captureWidth = 720,
        captureHeight = 1280,
        encodeWidth = 720,
        encodeHeight = 1280,
        fps = 15,
        bitrate = 1200,
        codecID = ZegoVideoCodecID.Default,
        keyFrameInterval = 2 {
    codecID = ZegoVideoCodecID.Default;
    switch (preset) {
      case ZegoVideoConfigPreset.Preset180P:
        captureWidth = 180;
        captureHeight = 320;
        encodeWidth = 180;
        encodeHeight = 320;
        bitrate = 300;
        fps = 15;
        break;
      case ZegoVideoConfigPreset.Preset270P:
        captureWidth = 270;
        captureHeight = 480;
        encodeWidth = 270;
        encodeHeight = 480;
        bitrate = 400;
        fps = 15;
        break;
      case ZegoVideoConfigPreset.Preset360P:
        captureWidth = 360;
        captureHeight = 640;
        encodeWidth = 360;
        encodeHeight = 640;
        bitrate = 600;
        fps = 15;
        break;
      case ZegoVideoConfigPreset.Preset540P:
        captureWidth = 540;
        captureHeight = 960;
        encodeWidth = 540;
        encodeHeight = 960;
        bitrate = 1200;
        fps = 15;
        break;
      case ZegoVideoConfigPreset.Preset720P:
        captureWidth = 720;
        captureHeight = 1280;
        encodeWidth = 720;
        encodeHeight = 1280;
        bitrate = 1500;
        fps = 15;
        break;
      case ZegoVideoConfigPreset.Preset1080P:
        captureWidth = 1080;
        captureHeight = 1920;
        encodeWidth = 1080;
        encodeHeight = 1920;
        bitrate = 3000;
        fps = 15;
        break;
    }
  }
}

/// Dual stream info.
class ZegoPublishDualStreamConfig {
  /// streamType
  ZegoVideoStreamType streamType;

  /// Video resolution width to be adjusted
  int encodeWidth;

  /// Video resolution height to be adjusted
  int encodeHeight;

  /// Video FPS to be adjusted
  int fps;

  /// Video bitrate in kbps to be adjusted
  int bitrate;

  ZegoPublishDualStreamConfig(this.streamType, this.encodeWidth,
      this.encodeHeight, this.fps, this.bitrate);

  Map<String, dynamic> toMap() {
    return {
      'streamType': this.streamType.index,
      'encodeWidth': this.encodeWidth,
      'encodeHeight': this.encodeHeight,
      'fps': this.fps,
      'bitrate': this.bitrate
    };
  }
}

/// SEI configuration
///
/// Used to set the relevant configuration of the Supplemental Enhancement Information.
class ZegoSEIConfig {
  /// SEI type
  ZegoSEIType type;

  ZegoSEIConfig(this.type);
}

/// Voice changer parameter.
///
/// Developer can use the built-in presets of the SDK to change the parameters of the voice changer.
class ZegoVoiceChangerParam {
  /// Pitch parameter, value range [-12.0, 12.0], the larger the value, the sharper the sound, set it to 0.0 to turn off. Note the tone-shifting sound effect is only effective for the sound played by the media player, and does not change the tone collected by the microphone. Note that on v2.18.0 and older version, the value range is [-8.0, 8.0].
  double pitch;

  ZegoVoiceChangerParam(this.pitch);
}

/// Audio reverberation advanced parameters.
///
/// Developers can use the SDK's built-in presets to change the parameters of the reverb.
class ZegoReverbAdvancedParam {
  /// Room size(%), in the range [0.0, 1.0], to control the size of the "room" in which the reverb is generated, the larger the room, the stronger the reverb.
  double roomSize;

  /// Echo(%), in the range [0.0, 100.0], to control the trailing length of the reverb.
  double reverberance;

  /// Reverb Damping(%), range [0.0, 100.0], controls the attenuation of the reverb, the higher the damping, the higher the attenuation.
  double damping;

  /// only wet
  bool wetOnly;

  /// wet gain(dB), range [-20.0, 10.0]
  double wetGain;

  /// dry gain(dB), range [-20.0, 10.0]
  double dryGain;

  /// Tone Low. 100% by default
  double toneLow;

  /// Tone High. 100% by default
  double toneHigh;

  /// PreDelay(ms), range [0.0, 200.0]
  double preDelay;

  /// Stereo Width(%). 0% by default
  double stereoWidth;

  ZegoReverbAdvancedParam(
      this.roomSize,
      this.reverberance,
      this.damping,
      this.wetOnly,
      this.wetGain,
      this.dryGain,
      this.toneLow,
      this.toneHigh,
      this.preDelay,
      this.stereoWidth);
}

/// Audio reverberation echo parameters.
class ZegoReverbEchoParam {
  /// Gain of input audio signal, in the range [0.0, 1.0]
  double inGain;

  /// Gain of output audio signal, in the range [0.0, 1.0]
  double outGain;

  /// Number of echos, in the range [0, 7]
  int numDelays;

  /// Respective delay of echo signal, in milliseconds, in the range [0, 5000] ms
  List<int> delay;

  /// Respective decay coefficient of echo signal, in the range [0.0, 1.0]
  List<double> decay;

  ZegoReverbEchoParam(
      this.inGain, this.outGain, this.numDelays, this.delay, this.decay);
}

/// User object.
///
/// Configure user ID and username to identify users in the room.
/// Note that the userID must be unique under the same appID, otherwise, there will be mutual kicks when logging in to the room.
/// It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
class ZegoUser {
  /// User ID, a utf8 string with a maximum length of 64 bytes or less.Privacy reminder: Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'. Do not use '%' if you need to communicate with the Web SDK.
  String userID;

  /// User Name, a utf8 string with a maximum length of 256 bytes or less.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  String userName;

  ZegoUser(this.userID, this.userName);

  /// Create a ZegoUser object
  ///
  /// userName and userID are set to match
  ZegoUser.id(this.userID) : this.userName = userID;
}

/// Stream object.
///
/// Identify an stream object
class ZegoStream {
  /// User object instance.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  ZegoUser user;

  /// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
  String streamID;

  /// Stream extra info
  String extraInfo;

  ZegoStream(this.user, this.streamID, this.extraInfo);
}

/// Room stream list.
///
/// Room stream list.
class ZegoRoomStreamList {
  /// Publish stream list
  List<ZegoStream> publishStreamList;

  /// Play stream list
  List<ZegoStream> playStreamList;

  ZegoRoomStreamList(this.publishStreamList, this.playStreamList);
}

/// Room extra information.
class ZegoRoomExtraInfo {
  /// The key of the room extra information.
  String key;

  /// The value of the room extra information.
  String value;

  /// The user who update the room extra information.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  ZegoUser updateUser;

  /// Update time of the room extra information, UNIX timestamp, in milliseconds.
  int updateTime;

  ZegoRoomExtraInfo(this.key, this.value, this.updateUser, this.updateTime);
}

/// View object.
///
/// Configure view object, view Mode, background color
class ZegoCanvas {
  /// ViewID, this parameter is the ViewID returned by calling [createCanvasView] through the callback function
  int view;

  /// View mode, default is ZegoViewModeAspectFit
  ZegoViewMode? viewMode;

  /// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
  int? backgroundColor;

  /// If enable alpha blend render, default is false.
  bool? alphaBlend;

  ZegoCanvas(this.view, {this.viewMode, this.backgroundColor, this.alphaBlend});

  /// Create a ZegoCanvas, default viewMode is AspectFit, default background color is black
  ZegoCanvas.view(this.view)
      : this.viewMode = ZegoViewMode.AspectFit,
        this.backgroundColor = 0x000000,
        this.alphaBlend = false;
}

/// Advanced publisher configuration.
///
/// Configure room id
class ZegoPublisherConfig {
  /// The Room ID, It is not necessary to pass in single room mode, but the ID of the corresponding room must be passed in multi-room mode
  String? roomID;

  /// Whether to synchronize the network time when pushing streams. 1 is synchronized with 0 is not synchronized. And must be used with setStreamAlignmentProperty. It is used to align multiple streams at the mixed stream service or streaming end, such as the chorus scene of KTV.
  int? forceSynchronousNetworkTime;

  /// When pushing a flow, review the pattern of the flow. By default, no audit is performed. If you want to use this function, contact ZEGO technical support.
  ZegoStreamCensorshipMode? streamCensorshipMode;

  ZegoPublisherConfig(
      {this.roomID,
      this.forceSynchronousNetworkTime,
      this.streamCensorshipMode});
}

/// Published stream quality information.
///
/// Audio and video parameters and network quality, etc.
class ZegoPublishStreamQuality {
  /// Video capture frame rate. The unit of frame rate is f/s
  double videoCaptureFPS;

  /// Video encoding frame rate. The unit of frame rate is f/s
  double videoEncodeFPS;

  /// Video transmission frame rate. The unit of frame rate is f/s
  double videoSendFPS;

  /// Video bit rate in kbps
  double videoKBPS;

  /// Audio capture frame rate. The unit of frame rate is f/s
  double audioCaptureFPS;

  /// Audio transmission frame rate. The unit of frame rate is f/s
  double audioSendFPS;

  /// Audio bit rate in kbps
  double audioKBPS;

  /// Local to server delay, in milliseconds
  int rtt;

  /// Packet loss rate, in percentage, 0.0 ~ 1.0
  double packetLostRate;

  /// Published stream quality level
  ZegoStreamQualityLevel level;

  /// Whether to enable hardware encoding
  bool isHardwareEncode;

  /// Video codec ID (Available since 1.17.0)
  ZegoVideoCodecID videoCodecID;

  /// Total number of bytes sent, including audio, video, SEI
  double totalSendBytes;

  /// Number of audio bytes sent
  double audioSendBytes;

  /// Number of video bytes sent
  double videoSendBytes;

  ZegoPublishStreamQuality(
      this.videoCaptureFPS,
      this.videoEncodeFPS,
      this.videoSendFPS,
      this.videoKBPS,
      this.audioCaptureFPS,
      this.audioSendFPS,
      this.audioKBPS,
      this.rtt,
      this.packetLostRate,
      this.level,
      this.isHardwareEncode,
      this.videoCodecID,
      this.totalSendBytes,
      this.audioSendBytes,
      this.videoSendBytes);
}

/// CDN config object.
///
/// Includes CDN URL and authentication parameter string
class ZegoCDNConfig {
  /// CDN URL
  String url;

  /// Auth param of URL. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
  String? authParam;

  /// URL supported protocols, candidate values are "tcp" and "quic". If there are more than one, separate them with English commas and try them in order. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
  String? protocol;

  /// QUIC version。 If [protocol] has the QUIC protocol, this information needs to be filled in. If there are multiple version numbers, separate them with commas. Please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored (set to null or empty string).
  String? quicVersion;

  /// customized httpdns service. This feature is only supported for playing stream currently.
  ZegoHttpDNSType? httpdns;

  /// QUIC establishes link mode. If the value is 1, quic 0 rtt is used preferentially to establish link. Otherwise, the link is established normally. If [protocol] has the QUIC protocol, this value takes effect.
  int? quicConnectMode;

  ZegoCDNConfig(this.url,
      {this.authParam,
      this.protocol,
      this.quicVersion,
      this.httpdns,
      this.quicConnectMode});
}

/// Relay to CDN info.
///
/// Including the URL of the relaying CDN, relaying state, etc.
class ZegoStreamRelayCDNInfo {
  /// URL of publishing stream to CDN
  String url;

  /// State of relaying to CDN
  ZegoStreamRelayCDNState state;

  /// Reason for relay state changed
  ZegoStreamRelayCDNUpdateReason updateReason;

  /// The timestamp when the state changed, UNIX timestamp, in milliseconds.
  int stateTime;

  ZegoStreamRelayCDNInfo(
      this.url, this.state, this.updateReason, this.stateTime);
}

/// Advanced player configuration.
///
/// Configure stream resource mode, CDN configuration and other advanced configurations.
class ZegoPlayerConfig {
  /// Stream resource mode.
  ZegoStreamResourceMode resourceMode;

  /// The CDN configuration for playing stream. If set, the stream is play according to the URL instead of the streamID. After that, the streamID is only used as the ID of SDK internal callback.
  ZegoCDNConfig? cdnConfig;

  /// The Room ID. It only needs to be filled in the multi-room mode, which indicates which room this stream needs to be bound to. This parameter is ignored in single room mode.
  String? roomID;

  /// The video encoding type of the stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
  ZegoVideoCodecID? videoCodecID;

  /// The resource type of the source stream, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
  ZegoResourceType? sourceResourceType;

  /// Preconfigured codec template ID, please contact ZEGO technical support if you need to use it, otherwise this parameter can be ignored.
  int? codecTemplateID;

  /// Play resource switching strategy mode, the default is ZegoStreamResourceSwitchModeDefault
  ZegoStreamResourceSwitchMode? resourceSwitchMode;

  /// Play resource type when stop publish, the default is ZegoStreamResourceTypeDefault. This setting takes effect when the user sets [resourceSwitchMode] to ZegoStreamResourceSwitchModeDefault or ZegoStreamResourceSwitchModeSwitchToRTC.
  ZegoStreamResourceType? resourceWhenStopPublish;

  ZegoPlayerConfig(this.resourceMode,
      {this.cdnConfig,
      this.roomID,
      this.videoCodecID,
      this.sourceResourceType,
      this.codecTemplateID,
      this.resourceSwitchMode,
      this.resourceWhenStopPublish});

  /// Create a default advanced player config object
  ZegoPlayerConfig.defaultConfig()
      : resourceMode = ZegoStreamResourceMode.Default,
        videoCodecID = ZegoVideoCodecID.Unknown,
        sourceResourceType = ZegoResourceType.RTC,
        codecTemplateID = 0,
        resourceSwitchMode = ZegoStreamResourceSwitchMode.Default,
        resourceWhenStopPublish = ZegoStreamResourceType.Default;
}

/// Played stream quality information.
///
/// Audio and video parameters and network quality, etc.
class ZegoPlayStreamQuality {
  /// Video receiving frame rate. The unit of frame rate is f/s
  double videoRecvFPS;

  /// Video dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
  double videoDejitterFPS;

  /// Video decoding frame rate. The unit of frame rate is f/s
  double videoDecodeFPS;

  /// Video rendering frame rate. The unit of frame rate is f/s
  double videoRenderFPS;

  /// Video bit rate in kbps
  double videoKBPS;

  /// Video break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
  double videoBreakRate;

  /// Audio receiving frame rate. The unit of frame rate is f/s
  double audioRecvFPS;

  /// Audio dejitter frame rate. The unit of frame rate is f/s (Available since 1.17.0)
  double audioDejitterFPS;

  /// Audio decoding frame rate. The unit of frame rate is f/s
  double audioDecodeFPS;

  /// Audio rendering frame rate. The unit of frame rate is f/s
  double audioRenderFPS;

  /// Audio bit rate in kbps
  double audioKBPS;

  /// Audio break rate, the unit is (number of breaks / every 10 seconds) (Available since 1.17.0)
  double audioBreakRate;

  /// The audio quality of the playing stream determined by the audio MOS (Mean Opinion Score) measurement method, value range [-1, 5], where -1 means unknown, [0, 5] means valid score, the higher the score, the better the audio quality. For the subjective perception corresponding to the MOS value, please refer to https://docs.zegocloud.com/article/3720#4_4 (Available since 2.16.0)
  double mos;

  /// Server to local delay, in milliseconds
  int rtt;

  /// Packet loss rate, in percentage, 0.0 ~ 1.0
  double packetLostRate;

  /// Delay from peer to peer, in milliseconds
  int peerToPeerDelay;

  /// Packet loss rate from peer to peer, in percentage, 0.0 ~ 1.0
  double peerToPeerPacketLostRate;

  /// Published stream quality level
  ZegoStreamQualityLevel level;

  /// Delay after the data is received by the local end, in milliseconds
  int delay;

  /// The difference between the video timestamp and the audio timestamp, used to reflect the synchronization of audio and video, in milliseconds. This value is less than 0 means the number of milliseconds that the video leads the audio, greater than 0 means the number of milliseconds that the video lags the audio, and 0 means no difference. When the absolute value is less than 200, it can basically be regarded as synchronized audio and video, when the absolute value is greater than 200 for 10 consecutive seconds, it can be regarded as abnormal (Available since 1.19.0)
  int avTimestampDiff;

  /// Whether to enable hardware decoding
  bool isHardwareDecode;

  /// Video codec ID (Available since 1.17.0)
  ZegoVideoCodecID videoCodecID;

  /// Total number of bytes received, including audio, video, SEI
  double totalRecvBytes;

  /// Number of audio bytes received
  double audioRecvBytes;

  /// Number of video bytes received
  double videoRecvBytes;

  /// Accumulated audio break count (Available since 2.9.0)
  int audioCumulativeBreakCount;

  /// Accumulated audio break time, in milliseconds (Available since 2.9.0)
  int audioCumulativeBreakTime;

  /// Accumulated audio break rate, in percentage, 0.0 ~ 100.0 (Available since 2.9.0)
  double audioCumulativeBreakRate;

  /// Accumulated audio decode time, in milliseconds (Available since 2.9.0)
  int audioCumulativeDecodeTime;

  /// Accumulated video break count (Available since 2.9.0)
  int videoCumulativeBreakCount;

  /// Accumulated video break time, in milliseconds (Available since 2.9.0)
  int videoCumulativeBreakTime;

  /// Accumulated video break rate, in percentage, 0.0 ~ 1.0 (Available since 2.9.0)
  double videoCumulativeBreakRate;

  /// Accumulated video decode time, in milliseconds (Available since 2.9.0)
  int videoCumulativeDecodeTime;

  /// Mute video (Available since 3.13.0)
  int muteVideo;

  /// Mute audio (Available since 3.13.0)
  int muteAudio;

  ZegoPlayStreamQuality(
      this.videoRecvFPS,
      this.videoDejitterFPS,
      this.videoDecodeFPS,
      this.videoRenderFPS,
      this.videoKBPS,
      this.videoBreakRate,
      this.audioRecvFPS,
      this.audioDejitterFPS,
      this.audioDecodeFPS,
      this.audioRenderFPS,
      this.audioKBPS,
      this.audioBreakRate,
      this.mos,
      this.rtt,
      this.packetLostRate,
      this.peerToPeerDelay,
      this.peerToPeerPacketLostRate,
      this.level,
      this.delay,
      this.avTimestampDiff,
      this.isHardwareDecode,
      this.videoCodecID,
      this.totalRecvBytes,
      this.audioRecvBytes,
      this.videoRecvBytes,
      this.audioCumulativeBreakCount,
      this.audioCumulativeBreakTime,
      this.audioCumulativeBreakRate,
      this.audioCumulativeDecodeTime,
      this.videoCumulativeBreakCount,
      this.videoCumulativeBreakTime,
      this.videoCumulativeBreakRate,
      this.videoCumulativeDecodeTime,
      this.muteVideo,
      this.muteAudio);
}

/// Cross APP playing stream configuration.
class ZegoCrossAppInfo {
  /// AppID for playing streams across apps.
  int appID;

  /// The token that needs to be set.
  String token;

  ZegoCrossAppInfo(this.appID, this.token);
}

/// SEI Callback info.
class ZegoMediaSideInfo {
  /// Stream ID.
  String streamID;

  /// SEI data
  Uint8List SEIData;

  /// timestamp
  int timestampNs;

  ZegoMediaSideInfo(this.streamID, this.SEIData, this.timestampNs);
}

/// Device Info.
///
/// Including device ID and name
class ZegoDeviceInfo {
  /// Device ID
  String deviceID;

  /// Device name
  String deviceName;

  ZegoDeviceInfo(this.deviceID, this.deviceName);
}

/// System performance monitoring status
class ZegoPerformanceStatus {
  /// Current CPU usage of the app, value range [0, 1]
  double cpuUsageApp;

  /// Current CPU usage of the system, value range [0, 1]
  double cpuUsageSystem;

  /// Current memory usage of the app, value range [0, 1]
  double memoryUsageApp;

  /// Current memory usage of the system, value range [0, 1]
  double memoryUsageSystem;

  /// Current memory used of the app, in MB
  double memoryUsedApp;

  ZegoPerformanceStatus(this.cpuUsageApp, this.cpuUsageSystem,
      this.memoryUsageApp, this.memoryUsageSystem, this.memoryUsedApp);
}

/// Beauty configuration options.
///
/// Configure the parameters of skin peeling, whitening and sharpening
class ZegoBeautifyOption {
  /// The sample step size of beauty peeling, the value range is [0,1], default 0.2
  double polishStep;

  /// Brightness parameter for beauty and whitening, the larger the value, the brighter the brightness, ranging from [0,1], default 0.5
  double whitenFactor;

  /// Beauty sharpening parameter, the larger the value, the stronger the sharpening, value range [0,1], default 0.1
  double sharpenFactor;

  ZegoBeautifyOption(this.polishStep, this.whitenFactor, this.sharpenFactor);

  /// Create a default beauty parameter object
  ZegoBeautifyOption.defaultConfig()
      : polishStep = 0.2,
        whitenFactor = 0.5,
        sharpenFactor = 0.1;
}

/// Beauty configuration param.
///
/// Configure the whiten, rosy, smooth, and sharpen parameters for beauty.
class ZegoEffectsBeautyParam {
  /// The whiten intensity parameter, the value range is [0,100], and the default is 50.
  int whitenIntensity;

  /// the rosy intensity parameter, value range [0,100], and the default is 50.
  int rosyIntensity;

  /// the smooth intensity parameter, value range [0,100], and the default is 50.
  int smoothIntensity;

  /// the sharpen intensity parameter, value range [0,100], and the default is 50.
  int sharpenIntensity;

  ZegoEffectsBeautyParam(this.whitenIntensity, this.rosyIntensity,
      this.smoothIntensity, this.sharpenIntensity);

  /// Create a default beauty parameter object
  ZegoEffectsBeautyParam.defaultParam()
      : whitenIntensity = 50,
        rosyIntensity = 50,
        smoothIntensity = 50,
        sharpenIntensity = 50;
}

/// Mix stream audio configuration.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
class ZegoMixerAudioConfig {
  /// Audio bitrate in kbps, default is 48 kbps, cannot be modified after starting a mixer task
  int bitrate;

  /// Audio channel, default is Mono
  ZegoAudioChannel channel;

  /// codec ID, default is ZegoAudioCodecIDDefault
  ZegoAudioCodecID codecID;

  /// Multi-channel audio stream mixing mode. If [ZegoAudioMixMode] is selected as [Focused], the SDK will select 4 input streams with [isAudioFocus] set as the focus voice highlight. If it is not selected or less than 4 channels are selected, it will automatically fill in 4 channels. On web platforms, this property does not take effect.
  ZegoAudioMixMode mixMode;

  ZegoMixerAudioConfig(this.bitrate, this.channel, this.codecID, this.mixMode);

  /// Create a default mix stream audio configuration
  ZegoMixerAudioConfig.defaultConfig()
      : bitrate = 48,
        channel = ZegoAudioChannel.Mono,
        codecID = ZegoAudioCodecID.Normal,
        mixMode = ZegoAudioMixMode.Raw;

  Map<String, dynamic> toMap() {
    return {
      'bitrate': this.bitrate,
      'channel': this.channel.index,
      'codecID': this.codecID.index,
      'mixMode': this.mixMode.index
    };
  }
}

/// Mix stream video config object.
///
/// Configure video frame rate, bitrate, and resolution for mixer task
class ZegoMixerVideoConfig {
  /// Video resolution width
  int width;

  /// Video resolution height
  int height;

  /// Video FPS, cannot be modified after starting a mixer task
  int fps;

  /// Video bitrate in kbps
  int bitrate;

  /// Video quality, this value is valid when the video rate control mode parameter is set to constant quality. The valid value ranges from 0 to 51. The default value is 23. If you want better video quality, lower the quality value based on 23 to test the adjustment. If you want a smaller file size, test the adjustment by increasing the high quality value at the base of 23. Take the file size under the value x as an example. The file size under the value x + 6 is half the size of the file size under the value x, and the file size under the value x-6 is twice the size of the file size under the value x. On web platforms, this property does not take effect.
  int quality;

  /// Video bitrate control mode.  On web platforms, this property does not take effect.
  ZegoVideoRateControlMode rateControlMode;

  ZegoMixerVideoConfig(this.width, this.height, this.fps, this.bitrate,
      this.quality, this.rateControlMode);

  /// Create a default mixer video configuration
  ZegoMixerVideoConfig.defaultConfig()
      : width = 360,
        height = 640,
        fps = 15,
        bitrate = 600,
        quality = 32,
        rateControlMode = ZegoVideoRateControlMode.ConstantRate;

  Map<String, dynamic> toMap() {
    return {
      'width': this.width,
      'height': this.height,
      'fps': this.fps,
      'bitrate': this.bitrate,
      'quality': this.quality,
      'rateControlMode': this.rateControlMode.index
    };
  }
}

/// Mix stream output video config object.
///
/// Description: Configure the video parameters, coding format and bitrate of mix stream output.
/// Use cases: Manual mixed stream scenario, such as Co-hosting.
class ZegoMixerOutputVideoConfig {
  /// Mix stream output video coding format, supporting H.264 and h.265 coding.
  ZegoVideoCodecID videoCodecID;

  /// Mix stream output video bitrate in kbps. The default value is the bitrate configured in [ZegoMixerVideoConfig].
  int bitrate;

  /// Mix stream video encode profile. Default value is [ZegoEncodeProfileDefault].
  ZegoEncodeProfile encodeProfile;

  /// The video encoding delay of mixed stream output, Valid value range [0, 2000], in milliseconds. The default value is 0.
  int encodeLatency;

  /// Enable high definition low bitrate. Default is false.
  bool enableLowBitrateHD;

  ZegoMixerOutputVideoConfig(this.videoCodecID, this.bitrate,
      {this.encodeProfile = ZegoEncodeProfile.Default,
      this.encodeLatency = 0,
      this.enableLowBitrateHD = false});
}

/// Font style.
///
/// Description: Font style configuration, can be used to configure font type, font size, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
class ZegoFontStyle {
  /// Font type. Required: False. Default value: Source han sans [ZegoFontTypeSourceHanSans]
  ZegoFontType type;

  /// Font size in px. Required: False. Default value: 24. Value range: [12,100].
  int size;

  /// Font color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 16777215(white). Value range: [0,16777215].
  int color;

  /// Font transparency. Required: False. Default value: 0. Value range: [0,100], 100 is completely opaque, 0 is completely transparent.
  int transparency;

  /// Whether the font has a border. Required: False. Default value: False. Value range: True/False.
  bool border;

  /// Font border color, the calculation formula is: R + G x 256 + B x 65536, the value range of R (red), G (green), and B (blue) [0,255]. Required: False. Default value: 0. Value range: [0,16777215].
  int borderColor;

  ZegoFontStyle(this.type, this.size, this.color, this.transparency,
      this.border, this.borderColor);

  Map<String, dynamic> toMap() {
    return {
      'type': this.type.index,
      'size': this.size,
      'color': this.color,
      'transparency': this.transparency,
      'border': this.border,
      'borderColor': this.borderColor
    };
  }

  /// Create a default font style object.
  ZegoFontStyle.defaultStyle()
      : type = ZegoFontType.SourceHanSans,
        size = 24,
        color = 16777215,
        transparency = 0,
        border = false,
        borderColor = 0;
}

/// Label info.
///
/// Description: Font style configuration, can be used to configure font type, font si-e, font color, font transparency.
/// Use cases: Set text watermark in manual stream mixing scene, such as Co-hosting.
class ZegoLabelInfo {
  /// Text content, support for setting simplified Chinese, English, half-width, not full-width. Required: True.Value range: Maximum support for displaying 100 Chinese characters and 300 English characters.
  String text;

  /// The distance between the font and the left border of the output canvas, in px. Required: False. Default value: 0.
  int left;

  /// The distance between the font and the top border of the output canvas, in px. Required: False. Default value: 0.
  int top;

  /// Font style. Required: False.
  ZegoFontStyle font;

  ZegoLabelInfo(this.text, this.left, this.top, this.font);

  Map<String, dynamic> toMap() {
    return {
      'text': this.text,
      'left': this.left,
      'top': this.top,
      'font': this.font.toMap()
    };
  }

  /// Build a label info object with text.
  ZegoLabelInfo.text(this.text)
      : left = 0,
        top = 0,
        font = ZegoFontStyle.defaultStyle();
}

/// Set the image information of a single input stream in the mux.
///
/// Available since: 2.19.0
/// Description: Sets a picture for the content of a single input stream, which is used in place of the video, i.e. the video is not displayed when the picture is used. The `layout` layout in [ZegoMixerInput] for image multiplexing.
/// Use case: The developer needs to temporarily turn off the camera to display the image during the video connection to the microphone, or display the picture when the audio is connected to the microphone.
/// Restrictions: Image size is limited to 1M.
class ZegoMixerImageInfo {
  /// The image path, if not empty, the image will be displayed, otherwise, the video will be displayed. JPG and PNG formats are supported. There are 2 ways to use it: 1. URI: Provide the picture to ZEGO technical support for configuration. After the configuration is complete, the picture URI will be provided, for example: preset-id://xxx.jpg. 2. URL: Only HTTP protocol is supported.
  String url;

  /// Image display mode. 0: Default. Use image to replace video content when url is not null. 1: Display image based on camera status. Display image when camera is turned off. Display video content when camera is turned on (no need to clear the url parameter). 2: Display image based on the input stream is empty or not. Display image when the input stream is empty for 3 consecutive seconds. The default duration for determine a input stream is empty or not is 3 seconds. If you need change this setting, please contact ZEGO technical support. Display video content when the input stream has video data.On web platforms, this property does not take effect.
  int? displayMode;

  ZegoMixerImageInfo(this.url, {this.displayMode});

  Map<String, dynamic> toMap() {
    return {'url': this.url, 'displayMode': this.displayMode};
  }
}

/// Mixer input.
///
/// Configure the mix stream input stream ID, type, and the layout
class ZegoMixerInput {
  /// Stream ID, a string of up to 256 characters. Caution: You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '-', '_'.
  String streamID;

  /// Mix stream content type
  ZegoMixerInputContentType contentType;

  /// Stream layout. When the mixed stream is an audio stream (that is, the ContentType parameter is set to the audio mixed stream type). Developers do not need to assign a value to this field, just use the SDK default.
  Rect layout;

  /// If enable soundLevel in mix stream task, an unique soundLevelID is need for every stream
  int soundLevelID;

  /// Input stream volume, valid range [0, 200], default is 100. On web platforms, this property does not take effect.
  int volume;

  /// Whether the focus voice is enabled in the current input stream, the sound of this stream will be highlighted if enabled. On web platforms, this property does not take effect.
  bool isAudioFocus;

  /// The direction of the audio. Valid direction is between 0 to 360. Set -1 means disable. Default value is -1. On web platforms, this property does not take effect.
  int audioDirection;

  /// Text watermark.
  ZegoLabelInfo? label;

  /// Video view render mode.
  ZegoMixRenderMode? renderMode;

  /// User image information.
  ZegoMixerImageInfo? imageInfo;

  /// Description: Video frame corner radius, in px. Required: False. Value range: Does not exceed the width and height of the video screen set by the [layout] parameter. Default value: 0.
  int? cornerRadius;

  /// Set advanced configuration. Please contact ZEGO technical support. On web platforms, this property does not take effect.
  Map<String, String>? advancedConfig;

  ZegoMixerInput(this.streamID, this.contentType, this.layout,
      this.soundLevelID, this.volume, this.isAudioFocus, this.audioDirection,
      {this.label,
      this.renderMode,
      this.imageInfo,
      this.cornerRadius,
      this.advancedConfig});

  Map<String, dynamic> toMap() {
    return {
      'streamID': this.streamID,
      'contentType': this.contentType.index,
      'left': this.layout.left.toInt(),
      'top': this.layout.top.toInt(),
      'right': this.layout.right.toInt(),
      'bottom': this.layout.bottom.toInt(),
      'soundLevelID': this.soundLevelID,
      'volume': this.volume,
      'isAudioFocus': this.isAudioFocus,
      'audioDirection': this.audioDirection,
      'label': this.label?.toMap(),
      'renderMode': this.renderMode?.index,
      'imageInfo': this.imageInfo?.toMap(),
      'cornerRadius': this.cornerRadius,
      'advancedConfig': this.advancedConfig
    };
  }

  ZegoMixerInput.defaultConfig()
      : this.streamID = "",
        this.contentType = ZegoMixerInputContentType.Video,
        this.layout = const Rect.fromLTRB(0, 0, 0, 0),
        this.soundLevelID = 0,
        this.volume = 100,
        this.isAudioFocus = false,
        this.audioDirection = -1,
        this.label = ZegoLabelInfo.text(""),
        this.renderMode = ZegoMixRenderMode.Fill,
        this.imageInfo = ZegoMixerImageInfo(""),
        this.cornerRadius = 0;
}

/// Mixer output object, currently, a mixed-stream task only supports a maximum of four video streams with different resolutions.
///
/// Configure mix stream output target URL or stream ID
class ZegoMixerOutput {
  /// Mix stream output target, URL or stream ID, if set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx, addresses with two identical mixed-stream outputs cannot be passed in.
  String target;

  /// Mix stream output video config. On web platforms, this property does not take effect.
  ZegoMixerOutputVideoConfig? videoConfig;

  ZegoMixerOutput(this.target, {this.videoConfig});
}

/// Watermark object.
///
/// Configure a watermark image URL and the layout of the watermark in the screen.
class ZegoWatermark {
  /// The path of the watermark image. Support local file absolute path (file://xxx). The format supports png, jpg. The maximum length is less than 512 bytes.
  String imageURL;

  /// Watermark image layout
  Rect layout;

  ZegoWatermark(this.imageURL, this.layout);

  Map<String, dynamic> toMap() {
    return {
      'imageURL': this.imageURL,
      'left': this.layout.left.toInt(),
      'top': this.layout.top.toInt(),
      'right': this.layout.right.toInt(),
      'bottom': this.layout.bottom.toInt(),
    };
  }
}

/// Mixer whiteboard object.
///
/// Configure the mix whiteboard ID, aspect ratio and the layout.
class ZegoMixerWhiteboard {
  /// Whiteboard ID.
  int whiteboardID;

  /// Whiteboard aspect ratio(width), the default aspect ratio is 16:9.
  int horizontalRatio;

  /// Whiteboard aspect ratio(height), the default aspect ratio is 16:9.
  int verticalRatio;

  /// Whether the whiteboard will load dynamic PPT files or not, default value is false.
  bool isPPTAnimation;

  /// Whiteboard layout.
  Rect layout;

  /// Whiteboard z-order.
  int zOrder;

  /// Whiteboard background color. Defaule is 0xF1F3F400 (gray). The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.F
  int? backgroundColor;

  ZegoMixerWhiteboard(this.whiteboardID, this.horizontalRatio,
      this.verticalRatio, this.isPPTAnimation, this.layout, this.zOrder,
      {this.backgroundColor});

  /// Create a mixer whiteboard object
  ZegoMixerWhiteboard.defaultConfig()
      : whiteboardID = 0,
        horizontalRatio = 16,
        verticalRatio = 9,
        isPPTAnimation = false,
        layout = Rect.zero,
        zOrder = 0,
        backgroundColor = 0xF1F3F400;

  Map<String, dynamic> toMap() {
    return {
      'whiteboardID': this.whiteboardID,
      'horizontalRatio': this.horizontalRatio,
      'verticalRatio': this.verticalRatio,
      'isPPTAnimation': this.isPPTAnimation,
      'layout': {
        'top': this.layout.top.toInt(),
        'left': this.layout.left.toInt(),
        'right': this.layout.right.toInt(),
        'bottom': this.layout.bottom.toInt()
      },
      'zOrder': this.zOrder,
      'backgroundColor': this.backgroundColor
    };
  }
}

/// Mix stream task object.
///
/// This class is the configuration class of the stream mixing task. When a stream mixing task is requested to the ZEGO RTC server, the configuration of the stream mixing task is required.
/// This class describes the detailed configuration information of this stream mixing task.
class ZegoMixerTask {
  /// The task ID of the task
  String taskID;

  /// The audio config of the task
  ZegoMixerAudioConfig audioConfig;

  /// The audio config of the task
  ZegoMixerVideoConfig videoConfig;

  /// The input list of the task
  List<ZegoMixerInput> inputList;

  /// The output list of the task
  List<ZegoMixerOutput> outputList;

  /// The watermark of the task
  ZegoWatermark watermark;

  /// Mix stream whiteboard. On web platforms, this property does not take effect.
  ZegoMixerWhiteboard whiteboard;

  /// Mix stream background color, The color value corresponding to RGBA is 0xRRGGBBAA, and setting the transparency of the background color is currently not supported. The AA in 0xRRGGBBAA is 00. For example, select RGB as \#87CEFA as the background color, this parameter passes 0x87CEFA00.
  int backgroundColor;

  /// The background image URL of the task
  String backgroundImageURL;

  /// Enable or disable sound level callback for the task. If enabled, then the remote player can get the soundLevel of every stream in the inputlist by [onMixerSoundLevelUpdate] callback. On web platforms, this property does not take effect.
  bool enableSoundLevel;

  /// The stream mixing alignment mode. On web platforms, this property does not take effect.
  ZegoStreamAlignmentMode streamAlignmentMode;

  /// User data, the length of user data should not be more than 1000 bytes,After setting, the streaming party can obtain the SEI content by listening to the callback of [onPlayerRecvSEI].
  Uint8List userData;

  /// Set advanced configuration, such as specifying video encoding and others. If you need to use it, contact ZEGO technical support. On web platforms, this property does not take effect.
  Map<String, String> advancedConfig;

  /// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
  int minPlayStreamBufferLength;

  /// Create a mix stream task object with TaskID
  ZegoMixerTask(this.taskID)
      : inputList = [],
        outputList = [],
        audioConfig = ZegoMixerAudioConfig.defaultConfig(),
        videoConfig = ZegoMixerVideoConfig.defaultConfig(),
        watermark = ZegoWatermark('', const Rect.fromLTRB(0, 0, 0, 0)),
        whiteboard = ZegoMixerWhiteboard.defaultConfig(),
        backgroundColor = 0,
        backgroundImageURL = "",
        enableSoundLevel = false,
        streamAlignmentMode = ZegoStreamAlignmentMode.None,
        userData = Uint8List.fromList([]),
        advancedConfig = {},
        minPlayStreamBufferLength = -1;

  Map<String, dynamic> toMap() {
    return {
      'taskID': this.taskID,
      'audioConfig': this.audioConfig.toMap(),
      'videoConfig': this.videoConfig.toMap(),
      'inputList': this.inputList,
      'outputList': this.outputList,
      'watermark': this.watermark.toMap(),
      'whiteboard': this.whiteboard.toMap(),
      'backgroundColor': this.backgroundColor,
      'backgroundImageURL': this.backgroundImageURL,
      'enableSoundLevel': this.enableSoundLevel,
      'streamAlignmentMode': this.streamAlignmentMode.index,
      'userData': this.userData,
      'advancedConfig': this.advancedConfig,
      'minPlayStreamBufferLength': this.minPlayStreamBufferLength
    };
  }
}

/// Configuration for start sound level monitor.
class ZegoSoundLevelConfig {
  /// Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
  int millisecond;

  /// Set whether the sound level callback includes the VAD detection result.
  bool enableVAD;

  ZegoSoundLevelConfig(this.millisecond, this.enableVAD);
}

/// Sound level info object.
class ZegoSoundLevelInfo {
  /// Sound level value.
  double soundLevel;

  /// Whether the stream corresponding to StreamID contains voice, 0 means noise, 1 means normal voice. This value is valid only when the [enableVAD] parameter in the [ZegoSoundLevelConfig] configuration is set to true when calling [startSoundLevelMonitor].
  int vad;

  ZegoSoundLevelInfo(this.soundLevel, this.vad);
}

/// Auto mix stream task object.
///
/// Description: When using [StartAutoMixerTask] function to start an auto stream mixing task to the ZEGO RTC server, user need to set this parameter to configure the auto stream mixing task, including the task ID, room ID, audio configuration, output stream list, and whether to enable the sound level callback.
/// Use cases: This configuration is required when an auto stream mixing task is requested to the ZEGO RTC server.
/// Caution: As an argument passed when [StartAutoMixerTask] function is called.
class ZegoAutoMixerTask {
  /// The taskID of the auto mixer task.Description: Auto stream mixing task id, must be unique in a room.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 256 bytes.Caution: When starting a new auto stream mixing task, only one auto stream mixing task ID can exist in a room, that is, to ensure the uniqueness of task ID. You are advised to associate task ID with room ID. You can directly use the room ID as the task ID.Cannot include URL keywords, for example, 'http' and '?' etc, otherwise publishing stream and playing stream will fail. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  String taskID;

  /// The roomID of the auto mixer task.Description: Auto stream mixing task id.Use cases: User need to set this parameter when initiating an auto stream mixing task.Required: Yes.Recommended value: Set this parameter based on requirements.Value range: A string up to 128 bytes.Caution: Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.If you need to communicate with the Web SDK, please do not use '%'.
  String roomID;

  /// The audio config of the auto mixer task.Description: The audio config of the auto mixer task.Use cases: If user needs special requirements for the audio config of the auto stream mixing task, such as adjusting the audio bitrate, user can set this parameter as required. Otherwise, user do not need to set this parameter.Required: No.Default value: The default audio bitrate is `48 kbps`, the default audio channel is `ZEGO_AUDIO_CHANNEL_MONO`, the default encoding ID is `ZEGO_AUDIO_CODEC_ID_DEFAULT`, and the default multi-channel audio stream mixing mode is `ZEGO_AUDIO_MIX_MODE_RAW`.Recommended value: Set this parameter based on requirements.
  ZegoMixerAudioConfig audioConfig;

  /// The output list of the auto mixer task.Description: The output list of the auto stream mixing task, items in the list are URL or stream ID, if the item set to be URL format, only RTMP URL surpported, for example rtmp://xxxxxxxx.Use cases: User need to set this parameter to specify the mix stream output target when starting an auto stream mixing task.Required: Yes.
  List<ZegoMixerOutput> outputList;

  /// Enable or disable sound level callback for the task. If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Description: Enable or disable sound level callback for the task.If enabled, then the remote player can get the sound level of every stream in the inputlist by [onAutoMixerSoundLevelUpdate] callback.Use cases: This parameter needs to be configured if user need the sound level information of every stream when an auto stream mixing task started.Required: No.Default value: `false`.Recommended value: Set this parameter based on requirements.
  bool enableSoundLevel;

  /// Stream mixing alignment mode.
  ZegoStreamAlignmentMode streamAlignmentMode;

  /// Description: Sets the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server. In the real-time chorus KTV scenario, slight fluctuations in the network at the push end may cause the mixed stream to freeze. At this time, when the audience pulls the mixed stream, there is a high probability of the problem of freeze. By adjusting the lower limit of the interval range for the adaptive adjustment of the stream playing cache of the stream mixing server, it can optimize the freezing problem that occurs when playing mixing streams at the player end, but it will increase the delay. It is not set by default, that is, the server uses its own configuration values. It only takes effect for the new input stream setting, and does not take effect for the input stream that has already started mixing.Value Range: [0,10000], exceeding the maximum value will result in a failure of the stream mixing request. On web platforms, this property does not take effect.
  int minPlayStreamBufferLength;

  /// Create a auto mix stream task object
  ZegoAutoMixerTask()
      : taskID = "",
        roomID = "",
        outputList = [],
        audioConfig = ZegoMixerAudioConfig.defaultConfig(),
        enableSoundLevel = false,
        streamAlignmentMode = ZegoStreamAlignmentMode.None,
        minPlayStreamBufferLength = -1;

  Map<String, dynamic> toMap() {
    return {
      'taskID': this.taskID,
      'roomID': this.roomID,
      'audioConfig': this.audioConfig.toMap(),
      'outputList': this.outputList,
      'enableSoundLevel': this.enableSoundLevel,
      'streamAlignmentMode': this.streamAlignmentMode.index,
      'minPlayStreamBufferLength': this.minPlayStreamBufferLength
    };
  }
}

/// Broadcast message info.
///
/// The received object of the room broadcast message, including the message content, message ID, sender, sending time
class ZegoBroadcastMessageInfo {
  /// message content
  String message;

  /// message id
  int messageID;

  /// Message send time, UNIX timestamp, in milliseconds.
  int sendTime;

  /// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  ZegoUser fromUser;

  ZegoBroadcastMessageInfo(
      this.message, this.messageID, this.sendTime, this.fromUser);
}

/// Barrage message info.
///
/// The received object of the room barrage message, including the message content, message ID, sender, sending time
class ZegoBarrageMessageInfo {
  /// message content
  String message;

  /// message id
  String messageID;

  /// Message send time, UNIX timestamp, in milliseconds.
  int sendTime;

  /// Message sender.Please do not fill in sensitive user information in this field, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  ZegoUser fromUser;

  ZegoBarrageMessageInfo(
      this.message, this.messageID, this.sendTime, this.fromUser);
}

/// Transparent message info.
///
/// Room transparent message, including room id, message content, sending user, sending type, sending mode, timeout period
class ZegoRoomSendTransparentMessage {
  /// send mode
  ZegoRoomTransparentMessageMode sendMode;

  /// send type
  ZegoRoomTransparentMessageType sendType;

  /// Message send content.
  Uint8List content;

  /// Message receiver list, when sendType appointed ZegoRoomTransparentMessageModeOnlyServer don't fill in. When appointed ZegoRoomTransparentMessageModeClientAndServer or ZegoRoomTransparentMessageModeOnlyClient, empty room will be sent to all online users.
  List<ZegoUser> recvUserList;

  /// send message timeout, The default value is 10s.
  int timeOut;

  ZegoRoomSendTransparentMessage(this.sendMode, this.sendType, this.content,
      this.recvUserList, this.timeOut);
}

/// Received a transparent message from the room.
///
/// Room transparent message, including message content, sending user, sending type, sending mode
class ZegoRoomRecvTransparentMessage {
  /// send message user
  ZegoUser sendUser;

  /// Message send content.
  Uint8List content;

  ZegoRoomRecvTransparentMessage(this.sendUser, this.content);
}

/// Parameter object for audio frame.
///
/// Including the sampling rate and channel of the audio frame
class ZegoAudioFrameParam {
  /// Sampling Rate
  ZegoAudioSampleRate sampleRate;

  /// Audio channel, default is Mono
  ZegoAudioChannel channel;

  ZegoAudioFrameParam(this.sampleRate, this.channel);
}

/// Audio configuration.
///
/// Configure audio bitrate, audio channel, audio encoding for publishing stream
class ZegoAudioConfig {
  /// Audio bitrate in kbps, default is 48 kbps. The settings before and after publishing stream can be effective
  int bitrate;

  /// Audio channel, default is Mono. The setting only take effect before publishing stream
  ZegoAudioChannel channel;

  /// codec ID, default is ZegoAudioCodecIDDefault. The setting only take effect before publishing stream
  ZegoAudioCodecID codecID;

  ZegoAudioConfig(this.bitrate, this.channel, this.codecID);

  /// Create a audio configuration with preset enumeration values
  ZegoAudioConfig.preset(ZegoAudioConfigPreset preset)
      : bitrate = 48,
        channel = ZegoAudioChannel.Mono,
        codecID = ZegoAudioCodecID.Default {
    codecID = ZegoAudioCodecID.Default;
    switch (preset) {
      case ZegoAudioConfigPreset.BasicQuality:
        bitrate = 16;
        channel = ZegoAudioChannel.Mono;
        break;
      case ZegoAudioConfigPreset.StandardQuality:
        bitrate = 48;
        channel = ZegoAudioChannel.Mono;
        break;
      case ZegoAudioConfigPreset.StandardQualityStereo:
        bitrate = 56;
        channel = ZegoAudioChannel.Stereo;
        break;
      case ZegoAudioConfigPreset.HighQuality:
        bitrate = 128;
        channel = ZegoAudioChannel.Mono;
        break;
      case ZegoAudioConfigPreset.HighQualityStereo:
        bitrate = 192;
        channel = ZegoAudioChannel.Stereo;
        break;
    }
  }
}

/// audio mixing data.
class ZegoAudioMixingData {
  /// Audio PCM data that needs to be mixed into the stream
  Uint8List audioData;

  /// the length of the audio PCM data that needs to be mixed into the stream. If the data length is sufficient, it must be the same as expectedDataLength
  int audioDataLength;

  /// Audio data attributes, including sample rate and number of channels. Currently supports 16k, 32k, 44.1k, 48k sampling rate, mono or stereo, 16-bit deep PCM data. Developers need to explicitly specify audio data attributes, otherwise mixing will not take effect.
  ZegoAudioFrameParam param;

  /// SEI data, used to transfer custom data. When audioData is null, SEIData will not be sent
  Uint8List SEIData;

  /// SEI data length
  int SEIDataLength;

  ZegoAudioMixingData(this.audioData, this.audioDataLength, this.param,
      this.SEIData, this.SEIDataLength);
}

/// Customize the audio processing configuration object.
///
/// Including custom audio acquisition type, sampling rate, channel number, sampling number and other parameters
class ZegoCustomAudioProcessConfig {
  /// Sampling rate, the sampling rate of the input data expected by the audio pre-processing module in App. If 0, the default is the SDK internal sampling rate.
  ZegoAudioSampleRate sampleRate;

  /// Number of sound channels, the expected number of sound channels for input data of the audio pre-processing module in App. If 0, the default is the number of internal channels in the SDK
  ZegoAudioChannel channel;

  /// The number of samples required to encode a frame; if samples = 0, the SDK will use the internal sample number, and the SDK will pass the audio data to the external pre-processing module. If the samples! = 0 (the effective value of samples is between [160, 2048]), and the SDK will send audio data to the external preprocessing module that sets the length of sample number.
  int samples;

  ZegoCustomAudioProcessConfig(this.sampleRate, this.channel, this.samples);

  ZegoCustomAudioProcessConfig.defaultConfig()
      : this.sampleRate = ZegoAudioSampleRate.Unknown,
        this.channel = ZegoAudioChannel.Unknown,
        this.samples = 0;
}

/// Record config.
class ZegoDataRecordConfig {
  /// The path to save the recording file, absolute path, need to include the file name, the file name need to specify the suffix, currently supports .mp4/.flv/.aac format files, if multiple recording for the same path, will overwrite the file with the same name. The maximum length should be less than 1024 bytes.
  String filePath;

  /// Type of recording media
  ZegoDataRecordType recordType;

  ZegoDataRecordConfig(this.filePath, this.recordType);
}

/// File recording progress.
class ZegoDataRecordProgress {
  /// Current recording duration in milliseconds
  int duration;

  /// Current recording file size in byte
  int currentFileSize;

  /// The quality of current recording file
  ZegoPublishStreamQuality quality;

  ZegoDataRecordProgress(this.duration, this.currentFileSize, this.quality);
}

/// Network probe config
class ZegoNetworkProbeConfig {
  /// Whether do traceroute, enabling tranceRoute will significantly increase network detection time
  bool enableTraceroute;

  ZegoNetworkProbeConfig(this.enableTraceroute);
}

/// http probe result
class ZegoNetworkProbeHttpResult {
  /// http probe errorCode, 0 means the connection is normal
  int errorCode;

  /// http request cost time, the unit is millisecond
  int requestCostTime;

  ZegoNetworkProbeHttpResult(this.errorCode, this.requestCostTime);
}

/// tcp probe result
class ZegoNetworkProbeTcpResult {
  /// tcp probe errorCode, 0 means the connection is normal
  int errorCode;

  /// tcp rtt, the unit is millisecond
  int rtt;

  /// tcp connection cost time, the unit is millisecond
  int connectCostTime;

  ZegoNetworkProbeTcpResult(this.errorCode, this.rtt, this.connectCostTime);
}

/// udp probe result
class ZegoNetworkProbeUdpResult {
  /// udp probe errorCode, 0 means the connection is normal
  int errorCode;

  /// The total time that the SDK send udp data to server and receive a reply, the unit is millisecond
  int rtt;

  ZegoNetworkProbeUdpResult(this.errorCode, this.rtt);
}

/// traceroute result
///
/// Jump up to 30 times. The traceroute result is for reference and does not represent the final network connection result. The priority is http, tcp, udp probe result.
class ZegoNetworkProbeTracerouteResult {
  /// traceroute error code, 0 means normal
  int errorCode;

  /// Time consumed by trace route, the unit is millisecond
  int tracerouteCostTime;

  ZegoNetworkProbeTracerouteResult(this.errorCode, this.tracerouteCostTime);
}

/// Network probe result
class ZegoNetworkProbeResult {
  /// http probe result
  ZegoNetworkProbeHttpResult? httpProbeResult;

  /// tcp probe result
  ZegoNetworkProbeTcpResult? tcpProbeResult;

  /// udp probe result
  ZegoNetworkProbeUdpResult? udpProbeResult;

  /// traceroute result
  ZegoNetworkProbeTracerouteResult? tracerouteResult;

  ZegoNetworkProbeResult(
      {this.httpProbeResult,
      this.tcpProbeResult,
      this.udpProbeResult,
      this.tracerouteResult});
}

/// Network speed test config
class ZegoNetworkSpeedTestConfig {
  /// Test uplink or not
  bool testUplink;

  /// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network uplink environment is suitable.
  int expectedUplinkBitrate;

  /// Test downlink or not
  bool testDownlink;

  /// The unit is kbps. Recommended to use the bitrate in ZegoVideoConfig when call startPublishingStream to determine whether the network downlink environment is suitable.
  int expectedDownlinkBitrate;

  ZegoNetworkSpeedTestConfig(this.testUplink, this.expectedUplinkBitrate,
      this.testDownlink, this.expectedDownlinkBitrate);
}

/// network speed test quality
class ZegoNetworkSpeedTestQuality {
  /// Time to connect to the server, in milliseconds. During the speed test, if the network connection is disconnected, it will automatically initiate a reconnection, and this variable will be updated accordingly.
  int connectCost;

  /// rtt, in milliseconds
  int rtt;

  /// packet lost rate. in percentage, 0.0 ~ 1.0
  double packetLostRate;

  /// network quality. excellent, good, medium and poor
  ZegoStreamQualityLevel quality;

  ZegoNetworkSpeedTestQuality(
      this.connectCost, this.rtt, this.packetLostRate, this.quality);
}

/// The NTP info
class ZegoNetworkTimeInfo {
  /// Network timestamp after synchronization, 0 indicates not yet synchronized
  int timestamp;

  /// The max deviation
  int maxDeviation;

  ZegoNetworkTimeInfo(this.timestamp, this.maxDeviation);
}

/// AudioEffectPlayer play configuration.
class ZegoAudioEffectPlayConfig {
  /// The number of play counts. When set to 0, it will play in an infinite loop until the user invoke [stop]. The default is 1, which means it will play only once.
  int playCount;

  /// Whether to mix audio effects into the publishing stream, the default is false.
  bool isPublishOut;

  ZegoAudioEffectPlayConfig(this.playCount, this.isPublishOut);
}

/// Precise seek configuration
class ZegoAccurateSeekConfig {
  /// The timeout time for precise search; if not set, the SDK internal default is set to 5000 milliseconds, the effective value range is [2000, 10000], the unit is ms
  int timeout;

  ZegoAccurateSeekConfig(this.timeout);

  ZegoAccurateSeekConfig.defaultConfig() : timeout = 5000;
}

/// Media player network cache information
class ZegoNetWorkResourceCache {
  /// Cached duration, unit ms
  int time;

  /// Cached size, unit byte
  int size;

  ZegoNetWorkResourceCache(this.time, this.size);
}

/// CopyrightedMusic play configuration.
class ZegoCopyrightedMusicConfig {
  /// User object instance, configure userID, userName. Note that the user ID set here needs to be consistent with the user ID set when logging in to the room, otherwise the request for the copyright music background service will fail.
  ZegoUser user;

  ZegoCopyrightedMusicConfig(this.user);
}

/// The configuration of getting lyric.
class ZegoCopyrightedMusicGetLyricConfig {
  /// the ID of the song.
  String songID;

  /// Copyright music resource song copyright provider.
  int? vendorID;

  ZegoCopyrightedMusicGetLyricConfig(this.songID, {this.vendorID});
}

/// The configuration of requesting resource.
class ZegoCopyrightedMusicRequestConfig {
  /// the ID of the song.
  String songID;

  /// VOD billing mode.
  ZegoCopyrightedMusicBillingMode mode;

  /// Copyright music resource song copyright provider.
  ZegoCopyrightedMusicVendorID? vendorID;

  /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
  String? roomID;

  /// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
  String? masterID;

  /// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
  int? sceneID;

  ZegoCopyrightedMusicRequestConfig(this.songID, this.mode,
      {this.vendorID, this.roomID, this.masterID, this.sceneID});
}

/// The configuration of requesting resource.
class ZegoCopyrightedMusicRequestConfigV2 {
  /// the ID of the song.
  String songID;

  /// VOD billing mode.
  int mode;

  /// Copyright music resource song copyright provider.
  int? vendorID;

  /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicate in which room to order song/accompaniment/accompaniment clip/accompaniment segment.
  String? roomID;

  /// The master ID, which must be passed when the billing mode is billed by host. Indicate which homeowner to order song/accompaniment/accompaniment clip/accompaniment segment.
  String? masterID;

  /// The scene ID, indicate the actual business. For details, please consult ZEGO technical support.
  int? sceneID;

  /// The resource type of music.
  int? resourceType;

  ZegoCopyrightedMusicRequestConfigV2(this.songID, this.mode,
      {this.vendorID,
      this.roomID,
      this.masterID,
      this.sceneID,
      this.resourceType});
}

/// The configuration of getting shared resource.
class ZegoCopyrightedMusicGetSharedConfig {
  /// the ID of the song.
  String songID;

  /// Copyright music resource song copyright provider.
  ZegoCopyrightedMusicVendorID? vendorID;

  /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
  String? roomID;

  ZegoCopyrightedMusicGetSharedConfig(this.songID,
      {this.vendorID, this.roomID});
}

/// The configuration of getting shared resource.
class ZegoCopyrightedMusicGetSharedConfigV2 {
  /// the ID of the song.
  String songID;

  /// Copyright music resource song copyright provider.
  int? vendorID;

  /// The room ID, the single-room mode can not be passed, and the corresponding room ID must be passed in the multi-room mode. Indicates which room to get resources from.
  String? roomID;

  /// The resource type of music.
  int? resourceType;

  ZegoCopyrightedMusicGetSharedConfigV2(this.songID,
      {this.vendorID, this.roomID, this.resourceType});
}

/// The configuration of querying cache.
class ZegoCopyrightedMusicQueryCacheConfig {
  /// the ID of the song.
  String songID;

  /// The resource type of music.
  ZegoCopyrightedMusicResourceType resourceType;

  /// The resource quality type of music.
  ZegoCopyrightedMusicResourceQualityType resourceQualityType;

  /// Copyright music resource song copyright provider.
  ZegoCopyrightedMusicVendorID vendorID;

  ZegoCopyrightedMusicQueryCacheConfig(
      this.songID, this.resourceType, this.resourceQualityType, this.vendorID);
}

/// The configuration of querying cache.
class ZegoCopyrightedMusicQueryCacheConfigV2 {
  /// the ID of the song.
  String songID;

  /// The resource type of music.
  int? resourceType;

  /// The resource quality type of music.
  int? resourceQualityType;

  /// Copyright music resource song copyright provider.
  int? vendorID;

  ZegoCopyrightedMusicQueryCacheConfigV2(this.songID,
      {this.resourceType, this.resourceQualityType, this.vendorID});
}

/// Screen capture configuration parameters.
class ZegoScreenCaptureConfig {
  /// Whether to capture video when screen capture. The default is true.
  bool captureVideo;

  /// Whether to capture audio when screen capture. The default is true.
  bool captureAudio;

  /// Set Microphone audio volume for ReplayKit. The range is 0 ~ 200. The default is 100. (only for iOS)
  int? microphoneVolume;

  /// Set Application audio volume for ReplayKit. The range is 0 ~ 200. The default is 100. (only for iOS)
  int? applicationVolume;

  /// Set the audio capture parameters during screen capture. (only for Android)
  ZegoAudioFrameParam? audioParam;

  ZegoScreenCaptureConfig(this.captureVideo, this.captureAudio,
      {this.microphoneVolume, this.applicationVolume, this.audioParam});

  ZegoScreenCaptureConfig.defaultConfig()
      : captureVideo = true,
        captureAudio = true,
        microphoneVolume = 100,
        applicationVolume = 100,
        audioParam = ZegoAudioFrameParam(
            ZegoAudioSampleRate.SampleRate16K, ZegoAudioChannel.Stereo);
}

/// The screen captures source information.
class ZegoScreenCaptureSourceInfo {
  /// Target type for screen capture. (only for desktop)
  ZegoScreenCaptureSourceType sourceType;

  /// The ID of the capture source.
  int sourceID;

  /// Capture source name (in UTF8 encoding).
  String sourceName;

  /// Thumbnail of the capture window.
  MemoryImage? thumbnailImage;

  /// The image content of the icon.
  MemoryImage? iconImage;

  ZegoScreenCaptureSourceInfo(this.sourceType, this.sourceID, this.sourceName,
      this.thumbnailImage, this.iconImage);
}

/// Audio source mix config
///
/// Used to config whether mix media player, audio effect player and captured system audio into publish stream or not when set audio source.
class ZegoAudioSourceMixConfig {
  /// Media player instance index list.
  List<int>? mediaPlayerIndexList;

  /// Audio effect player instance index list.
  List<int>? audioEffectPlayerIndexList;

  /// Enable or disable mix captured system audio into publish stream.
  bool? enableMixSystemPlayout;

  /// Enable or disable mix SDK playout into publish stream.
  bool? enableMixEnginePlayout;

  ZegoAudioSourceMixConfig(
      {this.mediaPlayerIndexList,
      this.audioEffectPlayerIndexList,
      this.enableMixSystemPlayout,
      this.enableMixEnginePlayout});
}

/// Multimedia resource for ZEGO media player.
///
/// Used to configure loading parameters when loading multimedia resources.
class ZegoMediaPlayerResource {
  /// Used to specify the loading type of multimedia resources.
  ZegoMultimediaLoadType loadType;

  /// The progress at which the plaback started.
  int? startPosition;

  /// If the specified resource has a transparent effect, you need to specify the layout position of the alpha data.
  ZegoAlphaLayoutType? alphaLayout;

  /// Common resource path.The absolute resource path or the URL of the network resource and cannot be null or "". Android can set this path string with Uri.
  String? filePath;

  /// binary data memory.
  Uint8List? memory;

  /// The resource ID obtained from the copyrighted music module.
  String? resourceID;

  ZegoMediaPlayerResource(this.loadType,
      {this.startPosition,
      this.alphaLayout,
      this.filePath,
      this.memory,
      this.resourceID});

  /// Constructs a media player resource object by default.
  ZegoMediaPlayerResource.defaultConfig()
      : loadType = ZegoMultimediaLoadType.FilePath,
        startPosition = 0,
        alphaLayout = ZegoAlphaLayoutType.None,
        filePath = '',
        memory = Uint8List.fromList([]),
        resourceID = '';
}

/// Background config.
///
/// It is used to configure background when the object segmentation is turned on.
class ZegoBackgroundConfig {
  /// Background process type.
  ZegoBackgroundProcessType processType;

  /// Background color, the format is 0xRRGGBB, default is black, which is 0x000000
  int color;

  /// Background image URL.
  String imageURL;

  /// Background video URL.
  String videoURL;

  /// Background blur level.
  ZegoBackgroundBlurLevel blurLevel;

  ZegoBackgroundConfig(this.processType, this.color, this.imageURL,
      this.videoURL, this.blurLevel);

  /// Constructs a background image configuration object by default.
  ZegoBackgroundConfig.defaultConfig()
      : processType = ZegoBackgroundProcessType.Transparent,
        color = 0,
        imageURL = '',
        videoURL = '',
        blurLevel = ZegoBackgroundBlurLevel.Medium;
}

/// Object segmentation config.
///
/// It is used to configure parameters when the object segmentation is turned on.
class ZegoObjectSegmentationConfig {
  /// The type of object segmentation.
  ZegoObjectSegmentationType objectSegmentationType;

  /// Background config.
  ZegoBackgroundConfig backgroundConfig;

  ZegoObjectSegmentationConfig(
      this.objectSegmentationType, this.backgroundConfig);

  /// Constructs a background image configuration object by default.
  ZegoObjectSegmentationConfig.defaultConfig()
      : objectSegmentationType = ZegoObjectSegmentationType.AnyBackground,
        backgroundConfig = ZegoBackgroundConfig.defaultConfig();
}

/// Media Infomration of media file.
///
/// Meida information such as video resolution of media file.
class ZegoMediaPlayerMediaInfo {
  /// Video resolution width.
  int width;

  /// Video resolution height.
  int height;

  /// Video frame rate.
  int frameRate;

  ZegoMediaPlayerMediaInfo(this.width, this.height, this.frameRate);

  /// Constructs a media player information object by default.
  ZegoMediaPlayerMediaInfo.defaultInfo()
      : width = 0,
        height = 0,
        frameRate = 0;
}

/// Used to config the media data publisher.
///
/// Used to config the media data publisher when creating it.
class ZegoMediaDataPublisherConfig {
  /// Used to specify the publish channel index of publisher.
  int channel;

  /// Used to specify the mode of publisher.
  ZegoMediaDataPublisherMode mode;

  ZegoMediaDataPublisherConfig(this.channel, this.mode);
}

/// Media player play statistics.
///
/// Media player play statistics.
class ZegoMediaPlayerStatisticsInfo {
  /// Video source fps.
  double videoSourceFps;

  /// Video decode fps.
  double videoDecodeFps;

  /// Video render fps.
  double videoRenderFps;

  /// Audio source fps.
  double audioSourceFps;

  /// Audio decode fps.
  double audioDecodeFps;

  /// Audio render fps.
  double audioRenderFps;

  ZegoMediaPlayerStatisticsInfo(
      this.videoSourceFps,
      this.videoDecodeFps,
      this.videoRenderFps,
      this.audioSourceFps,
      this.audioDecodeFps,
      this.audioRenderFps);

  /// Constructs a media player play statistics object by default.
  ZegoMediaPlayerStatisticsInfo.defaultParam()
      : videoSourceFps = 0,
        videoDecodeFps = 0,
        videoRenderFps = 0,
        audioSourceFps = 0,
        audioDecodeFps = 0,
        audioRenderFps = 0;
}

/// Receive range configuration.
class ZegoReceiveRangeParam {
  /// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
  double min;

  /// The maximum range received, the value needs to be >= min, the default value is 0.
  double max;

  ZegoReceiveRangeParam(this.min, this.max);

  /// Constructs a range audio receive range object by default.
  ZegoReceiveRangeParam.defaultParam()
      : min = 0,
        max = 0;
}

/// Vocal range configuration.
class ZegoVocalRangeParam {
  /// The minimum distance at which the 3D sound effect starts to have attenuation effect, the value needs to be >= 0 and <= max, the default value is 0.
  double min;

  /// The maximum range of the sound, the value needs to be >= min, the default value is 0.
  double max;

  ZegoVocalRangeParam(this.min, this.max);

  /// Constructs a range audio vocal range object by default.
  ZegoVocalRangeParam.defaultParam()
      : min = 0,
        max = 0;
}

/// Dump data config.
class ZegoDumpDataConfig {
  /// Data type.
  ZegoDumpDataType dataType;

  ZegoDumpDataConfig(this.dataType);
}

/// AI voice changer speaker detail.
class ZegoAIVoiceChangerSpeakerInfo {
  /// Speaker ID.
  int id;

  /// Speaker name.
  String name;

  ZegoAIVoiceChangerSpeakerInfo(this.id, this.name);
}

/// Color enhancement params.
class ZegoColorEnhancementParams {
  /// Description: color enhancement intensity. Value range: [0,1], the larger the value, the stronger the intensity of color enhancement. Default value: 0.
  double intensity;

  /// Description: Skin tone protection level. Value range: [0,1], the larger the value, the greater the level of skin protection. Default value: 1.
  double skinToneProtectionLevel;

  /// Description: Lip color protection level. Value range: [0,1], the larger the value, the greater the level of lip color protection. Default value: 0.
  double lipColorProtectionLevel;

  ZegoColorEnhancementParams(this.intensity, this.skinToneProtectionLevel,
      this.lipColorProtectionLevel);

  /// Constructs a color enhancement params object by default.
  ZegoColorEnhancementParams.defaultParam()
      : intensity = 0,
        skinToneProtectionLevel = 1,
        lipColorProtectionLevel = 0;
}

abstract class ZegoRealTimeSequentialDataManager {
  /// Start broadcasting real-time sequential data stream.
  ///
  /// Available since: 2.14.0
  /// Description: This function allows users to broadcast their local real-time sequential data stream to the ZEGO RTC server, and other users in the same room can subscribe to the real-time sequential data stream for intercommunication through "streamID".
  /// Use cases: Before sending real-time sequential data, you need to call this function to start broadcasting.
  /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
  /// Restrictions: None.
  /// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After the broadcast is successful, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been added to the room.
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters.
  ///   Caution:
  ///   1. Need to be globally unique within the entire AppID (Note that it cannot be the same as the stream ID passed in [startPublishingStream]). If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails.
  ///   2. Only support numbers, English characters and '-', '_'.
  Future<void> startBroadcasting(String streamID);

  /// Stop broadcasting real-time sequential data stream.
  ///
  /// Available since: 2.14.0
  /// Description: This function allows users to stop broadcasting their local real-time sequential data stream.
  /// Use cases: When you no longer need to send real-time sequential data, you need to call this function to stop broadcasting.
  /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
  /// Restrictions: None.
  /// Caution: After calling this function, you will receive the [onPublisherStateUpdate] callback to tell you the broadcast state (publish state) of this stream. After stopping the broadcast, other users in the same room will receive the [onRoomStreamUpdate] callback to tell them this stream has been deleted from the room.
  ///
  /// - [streamID] The ID of the stream that needs to stop broadcasting.
  Future<void> stopBroadcasting(String streamID);

  /// Send real-time sequential data to the broadcasting stream ID.
  ///
  /// Available since: 2.14.0
  /// Description: This function can be used to send real-time sequential data on the stream currently being broadcast.
  /// Use cases: You need to call this function when you need to send real-time sequential data.
  /// When to call: After calling [startBroadcasting].
  /// Restrictions: None.
  /// Caution: None.
  ///
  /// - [data] The real-time sequential data to be sent.
  /// - [streamID] The stream ID to which the real-time sequential data is sent.
  /// - Returns Send real-time sequential data result notification.
  Future<ZegoRealTimeSequentialDataSentResult> sendRealTimeSequentialData(
      Uint8List data, String streamID);

  /// Start subscribing real-time sequential data stream.
  ///
  /// Available since: 2.14.0
  /// Description: This function allows users to subscribe to the real-time sequential data stream of remote users from the ZEGO RTC server.
  /// Use cases: When you need to receive real-time sequential data sent from other remote users, you need to call this function to start subscribing to the stream broadcasted by other remote users.
  /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
  /// Restrictions: None.
  /// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters.
  ///   Caution:
  ///   Only support numbers, English characters and '-', '_'.
  Future<void> startSubscribing(String streamID);

  /// Stop subscribing real-time sequential data stream.
  ///
  /// Available since: 2.14.0
  /// Description: This function can be used to stop subscribing to the real-time sequential data stream.
  /// Use cases: When you no longer need to receive real-time sequential data sent by other users, you need to call this function to stop subscribing to the other user's stream.
  /// When to call: After creating the [ZegoRealTimeSequentialDataManager] instance.
  /// Restrictions: None.
  /// Caution: After calling this function, you will receive the [onPlayerStateUpdate] callback to tell you the subscribe state (play state) of this stream.
  ///
  /// - [streamID] The ID of the stream that needs to stop subscribing.
  Future<void> stopSubscribing(String streamID);

  /// Get real-time sequential data manager index.
  ///
  /// - Returns Index of the real-time sequential data manager.
  int getIndex();
}

abstract class ZegoMediaPlayer {
  /// Load local or network media resource.
  ///
  /// Available: since 1.3.4
  /// Description: Load media resources.
  /// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Related APIs: Resources can be loaded through the [loadResourceWithPosition] or [loadResourceFromMediaData] function.
  ///
  /// - [path] The absolute resource path or the URL of the network resource and cannot be null or "". Android can set this path string with Uri.
  /// - Returns Callback result of loading media resource.
  Future<ZegoMediaPlayerLoadResourceResult> loadResource(String path);

  /// Load local or network media resource and specify the start position.
  ///
  /// Available: since 2.14.0
  /// Description: Load media resources, and specify the progress, in milliseconds, at which playback begins.
  /// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Related APIs: Resources can be loaded through the [loadResource] or [loadResourceFromMediaData] function.
  /// Caution: When [startPosition] exceeds the total playing time, it will start playing from the beginning.
  ///
  /// - [path] The absolute resource path or the URL of the network resource and cannot be null or "". Android can set this path string with Uri.
  /// - [startPosition] The progress at which the playback started.
  /// - Returns Callback result of loading media resource.
  Future<ZegoMediaPlayerLoadResourceResult> loadResourceWithPosition(
      String path, int startPosition);

  /// Load binary audio resource.
  ///
  /// Available: since 2.10.0
  /// Description: Load binary audio data.
  /// Use case: Developers do not want to cache the audio data locally, and directly transfer the audio binary data to the media player, directly load and play the audio.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Related APIs: Resources can be loaded through the [loadResource] or [loadResourceWithPosition] function.
  /// Caution: When [startPosition] exceeds the total playing time, it will start playing from the beginning.
  ///
  /// - [mediaData] Binary audio data.
  /// - [startPosition] Position of starting playback, in milliseconds.
  /// - Returns Callback result of loading media resource.
  Future<ZegoMediaPlayerLoadResourceResult> loadResourceFromMediaData(
      Uint8List mediaData, int startPosition);

  /// Load copyrighted music resource.
  ///
  /// Available: since 2.14.0
  /// Description: Load media resources, and specify the progress, in milliseconds, at which playback begins.
  /// Use case: Developers can load the resource ID of copyrighted music.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Caution: When [startPosition] exceeds the total playing time, it will start playing from the beginning.
  ///
  /// - [resourceID] The resource ID obtained from the copyrighted music module.
  /// - [startPosition] The progress at which the playback started.
  /// - Returns Callback result of loading media resource.
  Future<ZegoMediaPlayerLoadResourceResult>
      loadCopyrightedMusicResourceWithPosition(
          String resourceID, int startPosition);

  /// Load local or network media resource with config.
  ///
  /// Available: since 3.3.0
  /// Description: Load media resources.
  /// Use case: Developers can load the absolute path to the local resource or the URL of the network resource incoming.
  /// When to call: Called after the engine [createEngine] has been initialized and the media player [createMediaPlayer] has been created.
  /// Related APIs: Support for loading resources through the [loadResourceWithPosition] or [loadResourceFromMediaData] interface.
  ///
  /// - [resource] Multimedia resources that need to be loaded.
  /// - Returns Callback result of loading media resource.
  Future<ZegoMediaPlayerLoadResourceResult> loadResourceWithConfig(
      ZegoMediaPlayerResource resource);

  /// Start playing.
  ///
  /// You need to load resources before playing
  Future<void> start();

  /// Stop playing.
  Future<void> stop();

  /// Pause playing.
  Future<void> pause();

  /// Resume playing.
  Future<void> resume();

  /// Set the specified playback progress.
  ///
  /// Unit is millisecond
  ///
  /// - [millisecond] Point in time of specified playback progress
  /// - Returns The result notification of set the specified playback progress
  Future<ZegoMediaPlayerSeekToResult> seekTo(int millisecond);

  /// Whether to repeat playback.
  ///
  /// - [enable] repeat playback flag. The default is false.
  Future<void> enableRepeat(bool enable);

  /// Set the speed of play.
  ///
  /// Available since: 2.12.0
  /// Description: Set the playback speed of the player.
  /// When to call: You should load resource before invoking this function.
  /// Restrictions: None.
  /// Related APIs: Resources can be loaded through the [loadResource] function.
  ///
  /// - [speed] The speed of play. The range is 0.5 ~ 4.0. The default is 1.0.
  Future<void> setPlaySpeed(double speed);

  /// Whether to mix the player's sound into the stream being published.
  ///
  /// This interface will only mix the media player sound into the main channel
  ///
  /// - [enable] Aux audio flag. The default is false.
  Future<void> enableAux(bool enable);

  /// Whether to play locally silently.
  ///
  /// If [enableAux] switch is turned on, there is still sound in the publishing stream. The default is false.
  ///
  /// - [mute] Mute local audio flag, The default is false.
  Future<void> muteLocal(bool mute);

  /// Set the view of the player playing video.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [canvas] Video rendered canvas object
  Future<void> setPlayerCanvas(ZegoCanvas canvas);

  /// Set mediaplayer volume. Both the local play volume and the publish volume are set.
  ///
  /// - [volume] The range is 0 ~ 200. The default is 60.
  Future<void> setVolume(int volume);

  /// Set mediaplayer local playback volume.
  ///
  /// - [volume] The range is 0 ~ 200. The default is 60.
  Future<void> setPlayVolume(int volume);

  /// Set mediaplayer publish volume.
  ///
  /// - [volume] The range is 0 ~ 200. The default is 60.
  Future<void> setPublishVolume(int volume);

  /// Set playback progress callback interval.
  ///
  /// This function can control the callback frequency of [onMediaPlayerPlayingProgress]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1s
  /// This callback are not returned exactly at the set callback interval, but rather at the frequency at which the audio or video frames are processed to determine whether the callback is needed to call
  ///
  /// - [millisecond] Interval of playback progress callback in milliseconds
  Future<void> setProgressInterval(int millisecond);

  /// Gets the current local playback volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
  ///
  /// - Returns current volume
  Future<int> getPlayVolume();

  /// Gets the current publish volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
  ///
  /// - Returns current volume
  Future<int> getPublishVolume();

  /// Get the total progress of your media resources.
  ///
  /// You should load resource before invoking this function, otherwise the return value is 0
  ///
  /// - Returns Unit is millisecond
  Future<int> getTotalDuration();

  /// Get current playing progress.
  ///
  /// You should load resource before invoking this function, otherwise the return value is 0
  ///
  /// - Returns current progress
  Future<int> getCurrentProgress();

  /// Get current rendering progress.
  ///
  /// You should load resource before invoking this function, otherwise the return value is 0
  ///
  /// - Returns current rendering progress
  Future<int> getCurrentRenderingProgress();

  /// Get the number of audio tracks of the playback file.
  ///
  /// - Returns Number of audio tracks
  Future<int> getAudioTrackCount();

  /// Set the audio track of the playback file.
  ///
  /// - [index] Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
  Future<void> setAudioTrackIndex(int index);

  /// Set the audio track mode of the player.
  ///
  /// Available since: 3.1.0
  /// Description: Set the audio track mode of the player.
  /// Use case: Under the real-time chorus (KTV), call the interface enable multi-track mode, call the interface [setAudioTrackIndex] to specify the original track to play, call interface [setAudioTrackPublishIndex] specified need publish of accompaniment tracks.
  /// When to call: The call takes effect before [start] starts playing
  /// Related APIs: Call [setAudioTrackIndex] to specified the play track of media file and call [setAudioTrackPublishIndex] to specified the publish track of media file.
  /// Caution: When multi-track mode is enabled, the resource consumption of the hardware device is increased.
  ///
  /// - [mode] Audio track mode.
  Future<void> setAudioTrackMode(ZegoMediaPlayerAudioTrackMode mode);

  /// Set the audio track for the media file to be publish.
  ///
  /// Available since: 3.1.0
  /// Description: Set the audio track for the media file to be publish.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Related APIs: The number of audio tracks can be obtained through the [getAudioTrackCount] function.
  /// Caution: This call takes effect only after multitrack mode is enabled by calling the interface [setAudioTrackMode].
  ///
  /// - [index] Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
  Future<void> setAudioTrackPublishIndex(int index);

  /// Setting up the specific voice changer parameters.
  ///
  /// - [audioChannel] The audio channel to be voice changed
  /// - [param] Voice changer parameters
  Future<void> setVoiceChangerParam(
      ZegoMediaPlayerAudioChannel audioChannel, ZegoVoiceChangerParam param);

  /// Get the current playback status.
  ///
  /// - Returns current state
  Future<ZegoMediaPlayerState> getCurrentState();

  /// Get media player index.
  ///
  /// Description: Get media player index.
  /// When to call: It can be called after [createMediaPlayer].
  /// Restrictions: None.
  ///
  /// - Returns Media player index.
  int getIndex();

  /// Whether to audio data playback.
  ///
  /// - [enable] Audio data playback flag. The default is false.
  Future<void> enableAudioData(bool enable);

  /// Whether to video data playback.
  ///
  /// Available since: 2.1.0
  /// Description: Whether to throw out the video data of the media resource file played by the media player.
  /// When to call: After the [ZegoMediaPlayer] instance created.
  /// Restrictions: None.
  /// Caution: When you no longer need to get the video frame data, please call this function again to clear the handler to stop the video frame data callback.
  ///
  /// - [enable] Video data playback flag. The default is false.
  /// - [format] Video frame format for video data
  Future<void> enableVideoData(bool enable, ZegoVideoFrameFormat format);

  /// Whether to throw block data of the media resource.
  ///
  /// Available since: 3.4.0
  /// Description: Whether to throw block data of the media resource.
  /// When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
  /// Restrictions: None.
  /// Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
  ///
  /// - [enable] Throw out the media resource block data tag, which is false by default.
  /// - [blockSize] The size of the encrypted data block. The bufferSize in the OnBlockData callback is an integer multiple of blockSize.
  Future<void> enableBlockData(bool enable, int blockSize);

  /// Take a screenshot of the current playing screen of the media player.
  ///
  /// Only in the case of calling [setPlayerCanvas] to set the display controls and the playback state, can the screenshot be taken normally
  /// Note: This function is only available in ZegoExpressVideo SDK!
  Future<ZegoMediaPlayerTakeSnapshotResult> takeSnapshot();

  /// Enable accurate seek and set relevant attributes.
  ///
  /// Available since: 2.4.0
  /// Description: The timestamp specified by normal seek may not an I frame, and then returns the I frame near the specified timestamp, which is not so accurate. But the accurate seek, when the specified timestamp is not an I frame, it will use the I frame near the specified timestamp to decode the frame of the specified timestamp.
  /// Use cases: When user needs to seek to the specified timestamp accurately.
  /// When to call: The setting must be called before [loadResource], and it will take effect during the entire life cycle of the media player.
  ///
  /// - [enable] Whether to enable accurate seek
  /// - [config] The property setting of accurate seek, only valid when enable is true.
  Future<void> enableAccurateSeek(bool enable, ZegoAccurateSeekConfig config);

  /// Set the maximum cache duration and cache data size of web materials.
  ///
  /// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
  /// Time and size are not allowed to be 0 at the same time. The SDK internal default time is 5000, and the size is 15*1024*1024 byte.When one of time and size reaches the set value first, the cache will stop.
  ///
  /// - [time] The maximum length of the cache time, in ms, the SDK internal default is 5000; the effective value is greater than or equal to 2000; if you fill in 0, it means no limit.
  /// - [size] The maximum size of the cache, the unit is byte, the internal default size of the SDK is 15*1024*1024 byte; the effective value is greater than or equal to 5000000, if you fill in 0, it means no limit.
  Future<void> setNetWorkResourceMaxCache(int time, int size);

  /// Get the playable duration and size of the cached data of the current network material cache queue
  ///
  /// - Returns Returns the current cached information, including the length of time the data can be played and the size of the cached data.
  Future<ZegoNetWorkResourceCache> getNetWorkResourceCache();

  /// Use this interface to set the cache threshold that the media player needs to resume playback. The SDK default value is 5000ms，The valid value is greater than or equal to 1000ms
  ///
  /// The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
  /// When the network status is poor and the media player has finished playing the cached network resources, it will stop playing, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_BEGIN` state of the callback interface `onMediaPlayerNetworkEvent` that the network resources are being recached.
  /// Only when the cached network resources are greater than the set threshold, the media player will automatically resume playback at the original paused position, and notify the user through the `ZegoMediaPlayerNetworkEvent.BUFFER_ENDED` of the callback interface `onMediaPlayerNetworkEvent` that the user has cached the network resources The threshold was reached and playback resumed.
  ///
  /// - [threshold] Threshold that needs to be reached to resume playback, unit ms.
  Future<void> setNetWorkBufferThreshold(int threshold);

  /// Whether to enable sound level monitoring.
  ///
  /// Available since: 2.15.0
  /// Description: Whether to enable sound level monitoring.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: None.
  /// Related callbacks: After it is turned on, user can use the [onMediaPlayerSoundLevelUpdate] callback to monitor sound level updates.
  ///
  /// - [enable] Whether to enable monitoring, true is enabled, false is disabled.
  /// - [millisecond] Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000].
  Future<void> enableSoundLevelMonitor(bool enable, int millisecond);

  /// Whether to enable frequency spectrum monitoring.
  ///
  /// Available since: 2.15.0
  /// Description: Whether to enable frequency spectrum monitoring.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: None.
  /// Related APIs: After it is turned on, user can use the [onMediaPlayerFrequencySpectrumUpdate] callback to monitor frequency spectrum updates.
  ///
  /// - [enable] Whether to enable monitoring, true is enabled, false is disabled.
  /// - [millisecond] Monitoring time period of the frequency spectrum, in milliseconds, has a value range of [100, 3000].
  Future<void> enableFrequencySpectrumMonitor(bool enable, int millisecond);

  /// Set the playback channel.
  ///
  /// Available since: 2.20.0
  /// Description: Set the playback channel.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: None.
  ///
  /// - [audioChannel] Playback channel, the default is ZegoMediaPlayerAudioChannelAll.
  Future<void> setActiveAudioChannel(ZegoMediaPlayerAudioChannel audioChannel);

  /// Clears the last frame of the playback control that remains on the control after playback ends.
  ///
  /// Available since: 2.20.0
  /// Description: Clears the last frame of the playback control that remains on the control after playback ends.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: The interface call takes effect only when the media player ends playing.
  Future<void> clearView();

  /// Get meida information such as video resolution from media file.
  ///
  /// Available since: 3.6.0
  /// Description: Get meida information such as video resolution from media file.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: None.
  Future<ZegoMediaPlayerMediaInfo> getMediaInfo();

  /// Update the position of the media player (audio source).
  ///
  /// Available since: 3.6.0
  /// Description: Update the position of the media player (audio source).
  /// Use cases: The media player also needs to have 3D spatial sound.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
  ///
  /// - [position] The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
  Future<void> updatePosition(Float32List position);

  /// Set http headers.
  ///
  /// Available since: 3.8.0
  /// Description: Call this function to set the http headers of the http network resource.
  /// Use cases: When the network resource needs to set special header information.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Restrictions: Called before the corresponding network resource is loaded.
  ///
  /// - [headers] Headers info.
  Future<void> setHttpHeader(Map<String, String> headers);

  /// Set play media stream type.
  ///
  /// Available since: 3.10.0
  /// Description: Configure the media stream type to be played. You can only play video streams or audio streams. This will take effect during the life cycle of the media player.
  /// Use cases: When the network resource needs to set special header information.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Caution: Changing the media stream type during playing will take effect in the next playing.
  ///
  /// - [streamType] Stream type.
  Future<void> setPlayMediaStreamType(ZegoMediaStreamType streamType);

  /// Enable live audio effect.
  ///
  /// Available since: 3.10.0
  /// Description: When the live audio effect is turned on, the spatial sense is enhanced and the instrument sounds become more prominent, without any increase in delay.
  /// Use cases: It is commonly used in voice chat rooms and karaoke scenarios to enhance the live audio effects of the accompaniment.
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer].
  /// Caution: To enhance the live audio effect experience, it is recommended to configure dual-channel stereo encoding. Developers can achieve this configuration by using the [setAudioCaptureStereoMode] method. Failure to configure dual-channel stereo encoding may significantly diminish the effects of certain songs, as the left and right channel effects may cancel each other out when synthesizing mono audio, resulting in less noticeable effects.
  ///
  /// - [enable] Whether to enable live audio effect.
  /// - [mode] Live audio effect mode.
  Future<void> enableLiveAudioEffect(bool enable, ZegoLiveAudioEffectMode mode);

  /// Enable local caching of http/https network resources.
  ///
  /// Available since: 3.12.0
  /// Description: When playing http/https network resources, when the local cache is enabled, the network resources will be saved locally and the cache information will be called back through [onMediaPlayerLocalCache].
  /// When to call: It can be called after the engine by [createEngine] has been initialized and the media player has been created by [createMediaPlayer]. Called after [loadResource] or during playback, it will take effect next playback.
  /// Caution: Only http/https single file type network resources are supported. If there is a [seek] operation during playback, the cache will fail.
  ///
  /// - [enable] Whether to enable local caching.
  /// - [cacheDir] Cache dir. If left blank, the directory specified internally by SDK will be used.
  Future<void> enableLocalCache(bool enable, String cacheDir);

  /// Get playback statistics.
  ///
  /// Available since: 3.12.0
  /// Description: Get current playback statistics to monitor whether decoding and rendering anomalies occur in the player.
  /// Use cases: Typically used in cloud-based media player scenarios.
  /// When to call: Invoke after the [loadResource] callback succeeds.
  Future<ZegoMediaPlayerStatisticsInfo> getPlaybackStatistics();
}

abstract class ZegoAudioEffectPlayer {
  /// Start playing audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Start playing audio effect. The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config] param.
  /// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use this interface to achieve, and further configure the number of plays through the [config] parameter, and mix the sound effects into the push stream.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] Description: ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique ID. The recommended methods are static self-incrementing ID or the hash of the incoming sound effect file path.
  /// - [path] The absolute path of the local resource or "assets://" . <br>Value range: "ipod-library://" and network url are not supported. Set path as null or "" if resource is loaded already using [loadResource].
  /// - [config] Audio effect playback configuration. <br>Default value: Set null will only be played once, and will not be mixed into the publishing stream.
  Future<void> start(int audioEffectID,
      {String? path, ZegoAudioEffectPlayConfig? config});

  /// Stop playing audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Stop playing the specified audio effect [audioEffectID].
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  Future<void> stop(int audioEffectID);

  /// Pause playing audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Pause playing the specified audio effect [audioEffectID].
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  Future<void> pause(int audioEffectID);

  /// Resume playing audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Resume playing the specified audio effect [audioEffectID].
  /// When to call: The specified [audioEffectID] is [pause].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  Future<void> resume(int audioEffectID);

  /// Stop playing all audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Stop playing all audio effect.
  /// When to call: Some audio effects are Playing.
  /// Restrictions: None.
  Future<void> stopAll();

  /// Pause playing all audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Pause playing all audio effect.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  Future<void> pauseAll();

  /// Resume playing all audio effect.
  ///
  /// Available since: 1.16.0
  /// Description: Resume playing all audio effect.
  /// When to call: It can be called after [pauseAll].
  /// Restrictions: None.
  Future<void> resumeAll();

  /// Set the specified playback progress.
  ///
  /// Available since: 1.16.0
  /// Description: Set the specified audio effect playback progress. Unit is millisecond.
  /// When to call: The specified [audioEffectID] is[start], and not finished.
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [millisecond] Point in time of specified playback progress.
  /// - Returns Result for audio effect player seek to playback progress
  Future<ZegoAudioEffectPlayerSeekToResult> seekTo(
      int audioEffectID, int millisecond);

  /// Set volume for a single audio effect. Both the local play volume and the publish volume are set.
  ///
  /// Available since: 1.16.0
  /// Description: Set volume for a single audio effect. Both the local play volume and the publish volume are set.
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setVolume(int audioEffectID, int volume);

  /// Set local play volume for a single audio effect.
  ///
  /// Available since: 3.11.0
  /// Description: Set local play volume for a single audio effect.
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setPlayVolume(int audioEffectID, int volume);

  /// Set publish volume for a single audio effect.
  ///
  /// Available since: 3.11.0
  /// Description: Set publish volume for a single audio effect.
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setPublishVolume(int audioEffectID, int volume);

  /// Set volume for all audio effect. Both the local play volume and the publish volume are set.
  ///
  /// Available since: 1.16.0
  /// Description: Set volume for all audio effect. Both the local play volume and the publish volume are set.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setVolumeAll(int volume);

  /// Set local play volume for all audio effect.
  ///
  /// Available since: 3.11.0
  /// Description: Set local play volume for all audio effect.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setPlayVolumeAll(int volume);

  /// Set publish volume for all audio effect.
  ///
  /// Available since: 3.11.0
  /// Description: Set publish volume for all audio effect.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - [volume] Volume. <br>Value range: The range is 0 ~ 200. <br>Default value: The default is 100.
  Future<void> setPublishVolumeAll(int volume);

  /// Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
  ///
  /// Available since: 2.18.0
  /// Description: Set the playback speed for a given audio effect. Both the local play speed and the publish speed are set. (separate settings are not supported).
  /// When to call: The specified [audioEffectID] is [start].
  /// Restrictions: None.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [speed] The speed of play. <br>Value range: The range is 0.5 ~ 2.0. <br>Default value: The default is 1.0.
  Future<void> setPlaySpeed(int audioEffectID, double speed);

  /// Get the total duration of the specified audio effect resource.
  ///
  /// Available since: 1.16.0
  /// Description: Get the total duration of the specified audio effect resource. Unit is millisecond.
  /// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
  /// Restrictions: It can be called after [createAudioEffectPlayer].
  /// Related APIs: [start], [loadResource].
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - Returns Unit is millisecond.
  Future<int> getTotalDuration(int audioEffectID);

  /// Get current playback progress.
  ///
  /// Available since: 1.16.0
  /// Description: Get current playback progress of the specified audio effect. Unit is millisecond.
  /// When to call: You should invoke this function after the audio effect resource already loaded, otherwise the return value is 0.
  /// Restrictions: None.
  /// Related APIs: [start], [loadResource].
  ///
  /// - [audioEffectID] ID for the audio effect.
  Future<int> getCurrentProgress(int audioEffectID);

  /// Load audio effect resource.
  ///
  /// Available since: 1.16.0
  /// Description: Load audio effect resource.
  /// Use cases: In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [path] the absolute path or "assets://" of the audio effect resource and cannot be null or "". <br>Value range: "ipod-library://" and network url are not supported.
  /// - Returns Result for audio effect player loads resources
  Future<ZegoAudioEffectPlayerLoadResourceResult> loadResource(
      int audioEffectID, String path);

  /// Unload audio effect resource.
  ///
  /// Available since: 1.16.0
  /// Description: Unload the specified audio effect resource.
  /// When to call: After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
  /// Restrictions: None.
  /// Related APIs: [loadResource].
  ///
  /// - [audioEffectID] ID for the audio effect loaded.
  Future<void> unloadResource(int audioEffectID);

  /// Update the position of the audio effect player (audio source).
  ///
  /// Available since: 3.6.0
  /// Description: Update the position of the audio effect player (audio source).
  /// Use cases: The audio effect player also needs to have 3D spatial sound.
  /// When to call: Listen to the [onAudioEffectPlayStateUpdate] callback, please call this interface after the player state is ZegoAudioEffectPlayState.Playing and before ZegoAudioEffectPlayState.NoPlay/PlayEnded.
  /// Restrictions: This interface needs to be used in conjunction with the RangeAudio/RangeScene module. This interface can only be called successfully after the RangeAudio/RangeScene module enables 3D sound effects.
  ///
  /// - [audioEffectID] ID for the audio effect.
  /// - [position] The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3.
  Future<void> updatePosition(int audioEffectID, Float32List position);

  /// Get audio effect player index.
  ///
  /// Available since: 1.16.0
  /// Description: Get audio effect player index.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  ///
  /// - Returns Audio effect player index.
  int getIndex();
}

abstract class ZegoMediaDataPublisher {
  /// Add media file to the publish queue. Currently, only mp4 / m4a / aac files are supported, and special conversion is required.
  ///
  /// Available since: 2.17.0
  /// Description: Add media file to the publish queue. Currently, only mp4 / m4a / aac file are supported, and special conversion is required.
  /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
  /// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
  /// Caution: The mp4 file format must meet the following points：The video must be encoded as H.264 and cannot contain B frames, only I and P frames. The I frame interval is 2s, that is, a single GOP value is 2s; The frame rate, bit rate, and resolution of the video are consistent with the frame rate, bit rate, and resolution set by [setVideoConfig] before publishing stream; Audio encoding must be MPEG-4 AAC.
  ///
  /// - [path] Local absolute path to the media file.
  /// - [isClear] Whether to clear the publish queue.
  Future<void> addMediaFilePath(String path, bool isClear);

  /// Clear all the status in this media data publisher, so that it can be restarted next time.
  ///
  /// Available since: 2.17.0
  /// Description: When you need to re-publish stream and do not need to continue publishing from the previous publish queue, you can call this function to reset this media data publisher's state.
  /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
  /// Caution: When the developer calls [logoutRoom], the state is automatically reset.
  Future<void> reset();

  /// Set the delay time of video playback.
  ///
  /// Available since: 2.17.0
  /// Description: When this value is set, when publishing video file stream, the SDK will permanently delay the video to the set time value before sending.
  /// Use cases: Mainly used to correct the fixed audio and picture asynchronous phenomenon that occurs during streaming.
  /// When to call: After calling the [createMediaDataPublisher] function to create a media data publisher.
  ///
  /// - [delayTime] Video playback time.Required: Yes.Value range: [0, 100] ms.
  Future<void> setVideoSendDelayTime(int delayTime);

  /// Specify the starting point of the current video file publishing.
  ///
  /// Available since: 2.17.0
  /// Description: Specify the starting point of the current video file publishing.
  /// When to call: Called at any point between [OnMediaDataPublisherFileOpen] and [OnMediaDataPublisherFileClose]. For example: this function can be called directly in the [OnMediaDataPublisherFileOpen] callback.
  ///
  /// - [millisecond] The timestamp of the start of streaming (relative to the timestamp of the file currently being published, the starting value is 0). the unit is milliseconds
  Future<void> seekTo(int millisecond);

  /// Get the total duration of the current file.
  ///
  /// Available since: 2.17.0
  /// Description: Get the total duration of the current file, in milliseconds.
  /// When to call: After [onMediaDataPublisherFileDataBegin] callback.
  ///
  /// - Returns The total duration of the current file.
  Future<int> getTotalDuration();

  /// Get the playing progress of the current file.
  ///
  /// Available since: 2.17.0
  /// Description: Get the playing progress of the current file, in milliseconds.
  /// When to call: After received the [onMediaDataPublisherFileDataBegin] callback.
  ///
  /// - Returns The playing progress of the current file.
  Future<int> getCurrentDuration();

  /// Get the channel index of the media data publisher.
  ///
  /// Available since: 3.4.0
  /// Description: Get the channel index of the media data publisher.
  int getIndex();
}

abstract class ZegoRangeAudio {
  /// Set the configuration of the audio receiving range.
  ///
  /// Available since: 3.7.0
  /// Description: Set the audio receiving range, the audio source sound beyond this range will not be received.
  /// Use case: Set the receiver's receiving range in the `World` mode.
  /// Default value: When this function is not called, only the voices of the members in the team can be received, and all voices outside the team cannot be received.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Restrictions: This range only takes effect for people outside the team.
  ///
  /// - [param] Configuration of audio receiving range.
  /// - Returns Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  Future<int> setAudioReceiveRange(ZegoReceiveRangeParam param);

  /// Set the frequency of real-time update locations within the SDK.
  ///
  /// Available since: 2.21.0
  /// Description: Set the frequency of real-time update locations within the SDK min 15 ms.
  /// Use case: After setting the update position, the sensitivity of audio gradient is very high.
  /// Default value: 100 ms.
  /// When to call: After initializing the range audio [createRangeAudio].
  ///
  /// - [frequency] the frequency, the value must be greater than 15 ms.
  Future<void> setPositionUpdateFrequency(int frequency);

  /// Set range voice volume.
  ///
  /// Available since: 2.23.0
  /// Description: Set range voice volume.
  /// Use case: This interface allows you to increase or decrease the volume of a range voice stream when the user calls [startPlayingStream] and pulls another stream.
  /// Default value: 100.
  /// When to call: After initializing the range audio [createRangeAudio].
  ///
  /// - [volume] volume, [0,200].
  Future<void> setRangeAudioVolume(int volume);

  /// Set the sound range for the stream.
  ///
  /// Available since: 3.7.0
  /// Description: Set range voice volume.
  /// Use case: When a user calls [startPlayingStream] and pulls another stream, the stream has a range speech effect by setting the range of sounds for that stream and calling [updateStreamPosition]. After the call will be the sound source of the sound range of the distance attenuation effect.
  /// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
  /// Caution:  When calling [enableMicrophone] to enable range speech, the resource of the stream will be switched to RTC, regardless of whether the resource specified when [startPlayingStream] was originally called to pull the stream is RTC. If you really need to specify the resource of the stream as CDN, please configure it to pull a custom CDN stream and specify the CDN address information.
  ///
  /// - [streamID] play stream id
  /// - [param] Flow sound range.
  /// - Returns Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  Future<int> setStreamVocalRange(String streamID, ZegoVocalRangeParam param);

  /// Update the location of the flow.
  ///
  /// Available since: 2.23.0
  /// Description: Set range voice volume.
  /// Use case: When the user calls [startPlayingStream] to pull another stream, call [setStreamVocalRange] to set the stream's voice position, then call this interface to set the stream's position, so that the stream also has the range voice effect.
  /// When to call: After initializing the range audio [createRangeAudio] and after [startPlayingStream].
  ///
  /// - [streamID] play stream id.
  /// - [position] The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  Future<void> updateStreamPosition(String streamID, Float32List position);

  /// Update self position and orentation.
  ///
  /// Available since: 2.11.0
  /// Description: Update the user's position and orientation so that the SDK can calculate the distance between the user and the audio source and the stereo effect of the left and right ears.
  /// Use case: When the role operated by the user in the game moves on the world map, the position information and head orientation of the role are updated.
  /// When to call: Called after logging in to the room [loginRoom].
  /// Caution: Before calling [enableSpeaker] to turn on the speaker, if you do not call this interface to set the location information, you will not be able to receive voices from other people except the team.
  ///
  /// - [position] The coordinates of the oneself in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  /// - [axisForward] The unit vector of the front axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  /// - [axisRight] The unit vector of the right axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  /// - [axisUp] The unit vector of the up axis of its own coordinate system. The parameter is a float array with a length of 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  Future<void> updateSelfPosition(Float32List position, Float32List axisForward,
      Float32List axisRight, Float32List axisUp);

  /// Add or update audio source position information.
  ///
  /// Available since: 2.11.0
  /// Description: Set the position of the audio source corresponding to the userID on the game map in the room, so that the SDK can calculate the distance and orientation of the listener to the audio source.
  /// Use case: Update the position of the voice user in the game map coordinates.
  /// When to call: Call [loginRoom] to call after logging in to the room, and the recorded audio source information will be cleared after logging out of the room.
  ///
  /// - [userID] The userID of the sound source.
  /// - [position] The coordinates of the speaker in the world coordinate system. The parameter is a float array of length 3. The three values ​​represent the front, right, and top coordinate values ​​in turn.
  Future<void> updateAudioSource(String userID, Float32List position);

  /// Turn the 3D spatial sound on or off.
  ///
  /// Available since: 2.11.0
  /// Description: After the 3D sound effect is turned on, the sound effect in the actual space will be simulated according to the position of the speaker equivalent to the listener. The intuitive feeling is that the sound size and the left and right sound difference will also change when the distance and orientation of the sound source change.
  /// Use case: It is a feature of audio recognition in FPS games or social scene games.
  /// Default value: When this function is not called, 3D sound effects are turned off by default.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Caution: The 3D audio effect will only take effect when [setRangeAudioMode] is called and set to `World` mode.
  /// Related APIs: After enabling the 3D sound effect, you can use [updateAudioSource] or [updateSelfPosition] to change the position and orientation to experience the 3D effect.
  ///
  /// - [enable] Whether to enable 3D sound effects.
  Future<void> enableSpatializer(bool enable);

  /// Turn the microphone on or off.
  ///
  /// Available since: 2.11.0
  /// Description: When enable is `true`, turn on the microphone and push audio stream; when it is `false`, turn off the microphone and stop pushing audio stream.
  /// Use case: The user turns on or off the microphone to communicate in the room.
  /// Default value: When this function is not called, the microphone is turned off by default.
  /// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
  /// Caution: Turning on the microphone will automatically use the main channel to push the audio stream.
  /// Related callbacks: Get the microphone switch state change through the callback [onRangeAudioMicrophoneStateUpdate].
  ///
  /// - [enable] Whether to turn on the microphone.
  Future<void> enableMicrophone(bool enable);

  /// Turn the speaker on or off.
  ///
  /// Available since: 2.11.0
  /// Description: When enable is `true`, turn on the speaker and play audio stream; when it is `false`, turn off the speaker and stop playing audio stream.
  /// Use case: The user turns on or off the speaker to communicate in the room.
  /// Default value: When this function is not called, the speaker is turned off by default.
  /// When to call: After initializing the range audio [createRangeAudio] and login room [loginRoom].
  /// Caution: Turning on the speaker will automatically pull the audio stream in the room.
  ///
  /// - [enable] Whether to turn on the speaker.
  Future<void> enableSpeaker(bool enable);

  /// Set range audio mode.
  ///
  /// Available since: 2.11.0
  /// Description: The audio mode can be set to `World` mode or `Team` mode.
  /// Use case: The user can choose to chat with everyone in the `World` mode (with distance limitation), or to communicate within the team in the `Team` mode (without distance limitation).
  /// Default value: If this function is not called, the `World` mode is used by default.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Related APIs: In the `World` mode, you can set the sound receiving range [setAudioReceiveRange], in the `Team` mode, you need to set [setTeamID] to join the corresponding team to hear the voice in the team.
  ///
  /// - [mode] The range audio mode.
  Future<void> setRangeAudioMode(ZegoRangeAudioMode mode);

  /// Set range audio custom mode.
  ///
  /// Available since: 3.3.0
  /// Description: Can set the speak mode and listening mode respectively to control the speak and listening behavior in the world and team.
  /// Use case: The user can decide who can listen to his voice by selecting the speak mode, and can also decide whose voice to listen to by selecting the listening mode.
  /// Default value: If this interface is not called, the ZegoRangeAudioSpeakModeAll mode and ZegoRangeAudioListenModeAll mode is used by default.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Related APIs: When you want to listen to sounds from the world, you need to set the sound reception range [setAudioReceiveRange]. When you want to sound and listen in the squad, you need to set [setTeamID] to join the corresponding squad.
  /// Restrictions: 1. Cannot be called with [setRangeAudioMode];
  ///  2. Not compatible with versions prior to 3.3.0.
  ///
  /// - [speakMode] The range audio speak mode.
  /// - [listenMode] The range audio listening mode.
  Future<void> setRangeAudioCustomMode(
      ZegoRangeAudioSpeakMode speakMode, ZegoRangeAudioListenMode listenMode);

  /// Set team ID.
  ///
  /// Available: since 2.11.0
  /// Description: After setting the team ID, you will be able to communicate with other users of the same team, and the sound will not change with the distance. It is also possible to exit the team by setting an empty string.
  /// Use case: Users join the team or exit the team.
  /// Default value: When this function is not called, no team will be added by default.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Caution: There will be no distance limit for the sounds in the team, and there will be no 3D sound effects.
  ///
  /// - [teamID] Team ID, empty to exit the team, a string of up to 64 bytes in length. Support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  Future<void> setTeamID(String teamID);

  /// Whether can receive the audio data of the specified user.
  ///
  /// Available since: 2.16.0
  /// Description: In the process of real-time audio and video interaction, local users can use this function to control whether to receive audio data from designated remote users when pulling streams as needed. When the developer does not receive the audio receipt, the hardware and network overhead can be reduced.
  /// Use cases: When developers need to quickly close and restore remote audio, they can call this function to enhance the interactive experience.
  /// Default value: The default is `false`, which means to receive audio data from all users.
  /// When to call: After initializing the range audio [createRangeAudio].
  /// Caution: This function is valid only when the [muteAllPlayStreamAudio] function is set to `false`.
  /// Related APIs: You can call the [muteAllPlayStreamAudio] function to control whether to receive all audio data. 1. When the [muteAllPlayStreamAudio(true)] function is called, it takes effect globally, that is, local users will be prohibited from receiving audio data from all remote users. At this time, the [muteUser] function will not take effect regardless of whether the [muteUser] function is called before or after [muteAllPlayStreamAudio]. 2. When the [muteAllPlayStreamAudio(false)] function is called, the local user can receive the audio data of all remote users. At this time, the [muteUser] function can be used to control whether to receive the audio data of the specified user. Calling the [muteUser(userID, true)] function allows the local user to receive audio data other than the `userID`; calling the [muteUser(userID, false)] function allows the local user to receive the audio data of the `userID`.
  ///
  /// - [userID] User ID.
  /// - [mute] Whether it can receive the audio data of the specified remote user, "true" means prohibition, "false" means receiving, the default value is "false".
  Future<void> muteUser(String userID, bool mute);
}

abstract class ZegoCopyrightedMusic {
  /// Initialize the copyrighted music module.
  ///
  /// Available since: 2.13.0
  /// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
  /// When to call: After call [createCopyrightedMusic] and [loginRoom].
  /// Caution: 1. The real user information must be passed in, otherwise the song resources cannot be obtained for playback. 2. The user ID set when initializing copyrighted music needs to be the same as the user ID set when logging in to the room.
  ///
  /// - [config] the copyrighted music configuration.
  Future<ZegoCopyrightedMusicInitResult> initCopyrightedMusic(
      ZegoCopyrightedMusicConfig config);

  /// Get cache size.
  ///
  /// Available since: 2.13.0
  /// Description: When using this module, some cache files may be generated, and the size of the cache file can be obtained through this interface.
  /// Use case: Used to display the cache size of the App.
  /// When to call: After initializing the copyrighted music [createCopyrightedMusic].
  ///
  /// - Returns cache file size, in byte.
  Future<int> getCacheSize();

  /// Clear cache.
  ///
  /// Available since: 2.13.0
  /// Description: When using this module, some cache files may be generated, which can be cleared through this interface.
  /// Use case: Used to clear the cache of the App.
  /// When to call: After initializing the copyrighted music [createCopyrightedMusic].
  Future<void> clearCache();

  /// Send extended feature request.
  ///
  /// Available since: 2.13.0
  /// Description: Initialize the copyrighted music so that you can use the function of the copyrighted music later.
  /// Use case: Used to get a list of songs.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [command] request command, details about the commands supported.
  /// - [params] request parameters, each request command has corresponding request parameters.
  Future<ZegoCopyrightedMusicSendExtendedRequestResult> sendExtendedRequest(
      String command, String params);

  /// Get lyrics in lrc format.
  ///
  /// Available since: 3.2.1
  /// Description: Get lyrics in lrc format, support parsing lyrics line by line.
  /// Use case: Used to display lyrics line by line.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [songID] the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
  /// - [vendorID] Copyright music resource song copyright provider.
  Future<ZegoCopyrightedMusicGetLrcLyricResult> getLrcLyric(String songID,
      {ZegoCopyrightedMusicVendorID? vendorID});

  /// Get lyrics in lrc format.
  ///
  /// Available since: 3.12.0
  /// Description: Get lyrics in lrc format, support parsing lyrics line by line.
  /// Use case: Used to display lyrics line by line.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [config] The configuration of get lyric.
  Future<ZegoCopyrightedMusicGetLrcLyricResult> getLrcLyricWithConfig(
      ZegoCopyrightedMusicGetLyricConfig config);

  /// Get lyrics in krc format.
  ///
  /// Available since: 2.13.0
  /// Description: Get verbatim lyrics, support parsing lyrics word by word.
  /// Use case: Used to display lyrics word by word.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [krcToken] The krcToken obtained by calling requestAccompaniment.
  Future<ZegoCopyrightedMusicGetKrcLyricByTokenResult> getKrcLyricByToken(
      String krcToken);

  /// Request music resource.
  ///
  /// Available since: 3.0.2
  /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
  /// Use case: Get copyrighted songs for local playback and sharing.
  /// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
  /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
  /// Note:
  ///   1. Each resource has a unique resource ID.
  ///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
  ///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
  ///
  /// - [config] The configuration of requesting music resource.
  /// - [type] The resource type of music.
  /// - Returns Result of requesting music resource.
  Future<ZegoCopyrightedMusicRequestResourceResult> requestResource(
      ZegoCopyrightedMusicRequestConfig config,
      ZegoCopyrightedMusicResourceType type);

  /// Request music resource.
  ///
  /// Available since: 3.12.0
  /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
  /// Use case: Get copyrighted songs for local playback and sharing.
  /// Related APIs: After a user in the room successfully calls this interface to request a music resource, other users in the room can call the [getsharedresource] interface to get the music resource for free once.
  /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
  /// Note:
  ///   1. Each resource has a unique resource ID.
  ///   2. Every time this API is called, it will be billed once, please consult ZEGO business personnel for details.
  ///   3. Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
  ///
  /// - [config] The configuration of requesting music resource.
  /// - Returns Result of requesting music resource.
  Future<ZegoCopyrightedMusicRequestResourceResult> requestResourceV2(
      ZegoCopyrightedMusicRequestConfigV2 config);

  /// Get shared music resource.
  ///
  /// Available since: 3.0.2
  /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
  /// Use case: Get copyrighted songs for local playback.
  /// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
  /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
  /// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
  ///
  /// - [config] The configuration of getting shared music resource.
  /// - [type] The resource type of music.
  /// - Returns Result of getting shared music resource.
  Future<ZegoCopyrightedMusicGetSharedResourceResult> getSharedResource(
      ZegoCopyrightedMusicGetSharedConfig config,
      ZegoCopyrightedMusicResourceType type);

  /// Get shared music resource.
  ///
  /// Available since: 3.12.0
  /// Description: In addition to obtaining the basic information of the song (duration, song name, singer, etc.), and the most important resource id that can be used for local playback, there are also some related authentications information.
  /// Use case: Get copyrighted songs for local playback.
  /// Related APIs: After a user in the room calls the [requestresource] interface to request a music resource successfully, other users in the room can call this interface to get the music resource for free once.
  /// When to call: After initializing the copyrighted music [initCopyrightedMusic].
  /// Note: Each resource has a unique resource ID. The resources obtained by calling this API are time-sensitive, the valid duration is the minimum value between the SDK life cycle and 24-hour.
  ///
  /// - [config] The configuration of getting shared music resource.
  /// - Returns Result of getting shared music resource.
  Future<ZegoCopyrightedMusicGetSharedResourceResult> getSharedResourceV2(
      ZegoCopyrightedMusicGetSharedConfigV2 config);

  /// Download music resource.
  ///
  /// Available since: 2.13.0
  /// Description: Download music resource. It can only be played after downloading successfully.
  /// Use case: After obtaining the music resource authorization, use this interface to download the corresponding resources.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  /// Caution: Downloading copyrighted music resources is affected by the Internet.
  ///
  /// - [resourceID] the resource ID corresponding to the song or accompaniment.
  Future<ZegoCopyrightedMusicDownloadResult> download(String resourceID);

  /// Cancel download of music resource.
  ///
  /// Available since: 3.11.0
  /// Description: Cancel download of music resource.
  /// Use case: After starting to download music resources, use this interface to cancel the corresponding download task.
  /// When to call: After starting the download [download].
  /// Caution: When a valid resourceID is passed in, only unfinished download tasks will be cancelled. When empty is passed in, all unfinished download tasks will be canceled.
  ///
  /// - [resourceID] the resource ID corresponding to the song or accompaniment.
  Future<void> cancelDownload(String resourceID);

  /// Query the resource's cache is existed or not.
  ///
  /// Available since: 3.9.0
  /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
  /// Use case: Can be used to check the resource's cache is existed or not
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [config] The configuration of querying cache.
  Future<bool> queryCacheWithConfig(
      ZegoCopyrightedMusicQueryCacheConfig config);

  /// Query the resource's cache is existed or not.
  ///
  /// Available since: 3.12.0
  /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
  /// Use case: Can be used to check the resource's cache is existed or not
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [config] The configuration of querying cache.
  Future<bool> queryCacheWithConfigV2(
      ZegoCopyrightedMusicQueryCacheConfigV2 config);

  /// Get the playing time of a song or accompaniment file.
  ///
  /// Available since: 2.13.0
  /// Description: Get the playing time of a song or accompaniment file.
  /// Use case: Can be used to display the playing time information of the song or accompaniment on the view.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// - [resourceID] the resource ID corresponding to the song or accompaniment.
  Future<int> getDuration(String resourceID);

  /// Set the difficulty level of scoring.
  ///
  /// Available since: 2.22.0
  /// Description: Users can set the scoring difficulty level through this function.
  /// When to call: After calling [initCopyrightedMusic] to initialize copyrighted music successfully, call [startScore] to start scoring.
  /// Default value: When this function is not called, the difficulty level of scoring is 4.
  /// Restrictions: This function does not support dynamic settings. After calling this function successfully, the next call to [startScore] will take effect.
  ///
  /// - [level] The difficulty level of scoring. The level ranges from 0 to 4. The scoring difficulty decreases from 0 to 4.
  Future<void> setScoringLevel(int level);

  /// Start scoring.
  ///
  /// Available since: 2.15.0
  /// Description: Start the scoring function.After starting scoring, the scoring result OnCurrentPitchValueUpdate callback will be received according to the set callback time interval.
  /// Use case: Can be used to display the singing score on the view.
  /// When to call: After obtaining verbatim lyrics and playing the accompaniment resources of copyrighted music.
  /// Restrictions: Only support use this api after [startPublishingStream].
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  /// - [pitchValueInterval] the time interval of real-time pitch line callback, in milliseconds, the default is 50 milliseconds.
  Future<int> startScore(String resourceID, int pitchValueInterval);

  /// Pause scoring.
  ///
  /// Available since: 2.15.0
  /// Description: Pause ongoing scoring,will stop the [OnCurrentPitchValueUpdate] callback.
  /// Use case: You can call this interface to pause the scoring function while scoring.
  /// When to call: It can be called while grading.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> pauseScore(String resourceID);

  /// Resume scoring.
  ///
  /// Available since: 2.15.0
  /// Description: Resume currently paused scoring.
  /// Use case: When there is currently paused scoring, this interface can be called to resume the scoring function.
  /// When to call: It can be called when there is currently a paused scoring.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> resumeScore(String resourceID);

  /// Stop scoring.
  ///
  /// Available since: 2.15.0
  /// Description: End the current rating.The [OnCurrentPitchValueUpdate] callback will be stopped, but the average or total score can still be obtained normally.
  /// Use case: You can call this interface to end the scoring while scoring.
  /// When to call: It can be called while grading.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> stopScore(String resourceID);

  /// Reset scoring.
  ///
  /// Available since: 2.15.0
  /// Description: Reset the scores that have already been performed,The [OnCurrentPitchValueUpdate] callback will be stopped and the average or total score will be 0.
  /// Use case: Often used in scenes where the same song is re-sung.
  /// When to call: It can be called after scoring has been performed.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> resetScore(String resourceID);

  /// Get the score of the previous sentence.
  ///
  /// Available since: 2.15.0
  /// Description: Get the score of the previous sentence.
  /// Use case: Can be used to display the score of each sentence on the view.
  /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> getPreviousScore(String resourceID);

  /// Get average score.
  ///
  /// Available since: 2.15.0
  /// Description: Get the average score.
  /// Use case: Can be used to display the average score on the view.
  /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> getAverageScore(String resourceID);

  /// Get total score .
  ///
  /// Available since: 2.15.0
  /// Description: Get the total score.
  /// Use case: Can be used to display the total score on the view.
  /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> getTotalScore(String resourceID);

  /// Get full score .
  ///
  /// Available since: 3.0.2
  /// Description: Get the full score.
  /// Use case: Can be used to display the full score on the view.
  /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<int> getFullScore(String resourceID);

  /// Get standard pitch data.
  ///
  /// Available since: 2.15.0
  /// Description: Get standard pitch data.
  /// Use case: Can be used to display standard pitch lines on the view.
  /// Cation: Only accompaniment or climactic clip assets have pitch lines.
  ///
  /// - [resourceID] the resource ID corresponding to the accompaniment or accompaniment clip.
  Future<ZegoCopyrightedMusicGetStandardPitchResult> getStandardPitch(
      String resourceID);

  /// Get real-time pitch data.
  ///
  /// Available since: 2.15.0
  /// Description: Get real-time pitch data.
  /// Use case: Can be used to display real-time pitch lines on the view.
  /// When to call: It can be called after playing the copyright accompaniment or accompaniment clip and starting to score.
  ///
  /// - [resourceID] the resource ID corresponding to the song or accompaniment.
  Future<int> getCurrentPitch(String resourceID);

  /// [Deprecated] Request a song. Deprecated since 3.0.2, please use the [requestResource] function instead.
  ///
  /// Available since: 2.13.0
  /// Description: By requesting a song, you can not only obtain basic information about a song (such as duration, song name, and artist), but also obtain the resource ID for local playback, share_token for sharing with others, and related authentication information. Support by the time, by the user monthly, by the room monthly subscription three ways.
  /// Use case: Get copyrighted songs for local playback and sharing.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  /// Caution: This interface will trigger billing. A song may have three sound qualities: normal, high-definition, and lossless. Each sound quality has a different resource file, and each resource file has a unique resource ID.
  ///
  /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
  /// - [config] request configuration.
  @Deprecated(
      'Deprecated since 3.0.2, please use the [requestResource] function instead.')
  Future<ZegoCopyrightedMusicRequestSongResult> requestSong(
      ZegoCopyrightedMusicRequestConfig config);

  /// [Deprecated] Request accompaniment. Deprecated since 3.0.2, please use the [requestResource] function instead.
  ///
  /// Available since: 2.13.0
  /// Description: You can get the accompaniment resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports click-by-point accompaniment.
  /// Use case: Get copyrighted accompaniment for local playback and sharing.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  /// Caution: This interface will trigger billing.
  ///
  /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
  /// - [config] request configuration.
  @Deprecated(
      'Deprecated since 3.0.2, please use the [requestResource] function instead.')
  Future<ZegoCopyrightedMusicRequestAccompanimentResult> requestAccompaniment(
      ZegoCopyrightedMusicRequestConfig config);

  /// [Deprecated] Request accompaniment clip. Deprecated since 3.0.2, please use the [requestResource] function instead.
  ///
  /// Available since: 2.13.0
  /// Description: You can get the accompaniment clip resources of the song corresponding to the songID, including resource_id, krc_token, share_token, etc. Supports accompaniment clips by pay-per-use.
  /// Use case: Get copyrighted accompaniment clip for local playback and sharing.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  /// Caution: This interface will trigger billing.
  ///
  /// @deprecated Deprecated since 3.0.2, please use the [requestResource] function instead.
  /// - [config] request configuration.
  @Deprecated(
      'Deprecated since 3.0.2, please use the [requestResource] function instead.')
  Future<ZegoCopyrightedMusicRequestAccompanimentClipResult>
      requestAccompanimentClip(ZegoCopyrightedMusicRequestConfig config);

  /// [Deprecated] Get a song or accompaniment. Deprecated since 3.0.2, please use the [getSharedResource] function instead.
  ///
  /// Available since: 2.13.0
  /// Description: After the user successfully obtains the song/accompaniment/accompaniment clip resource, he can get the corresponding shareToken, share the shareToken with other users, and other users call this interface to obtain the shared music resources.
  /// Use case: In the online KTV scene, after receiving the song or accompaniment token shared by the lead singer, the chorus obtains the corresponding song or accompaniment through this interface, and then plays it on the local end.
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// @deprecated Deprecated since 3.0.2, please use the [getSharedResource] function instead.
  /// - [shareToken] access the corresponding authorization token for a song or accompaniment.
  @Deprecated(
      'Deprecated since 3.0.2, please use the [getSharedResource] function instead.')
  Future<ZegoCopyrightedMusicGetMusicByTokenResult> getMusicByToken(
      String shareToken);

  /// [Deprecated] Query the resource's cache is existed or not. Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
  ///
  /// Available since: 3.2.1
  /// Description: Query the resource is existed or not, query the Yinsuda resource cache by default
  /// Use case: Can be used to check the resource's cache is existed or not
  /// When to call: After initializing the copyrighted music success [initCopyrightedMusic].
  ///
  /// @deprecated Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.
  /// - [songID] the ID of the song or accompaniment, the song and accompaniment of a song share the same ID.
  /// - [type] the song resource type.
  /// - [vendorID] Copyright music resource song copyright provider.
  @Deprecated(
      'Deprecated since 3.9.0, please use the method with the same name with [config] parameter instead.')
  Future<bool> queryCache(String songID, ZegoCopyrightedMusicType type,
      {ZegoCopyrightedMusicVendorID? vendorID});
}

abstract class ZegoScreenCaptureSource {
  /// Create the screen capture source
  ///
  /// Available: since 3.1.0
  /// Description: Update a screen capture source object based on the provided source ID and source type.
  /// Use cases: It is used when you need to record and share the screen or window.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [sourceId] The specified screen ID or window ID.
  /// - [sourceType] The specified screen source type.
  Future<void> updateCaptureSource(
      int sourceId, ZegoScreenCaptureSourceType sourceType);

  /// Start screen capture.
  ///
  /// Available since: 3.1.0
  /// Description: Start screen capture.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  ///
  /// - [config] Screen capture parameter configuration, only available on the iOS and Android platform.
  /// - [inApp] in-app capture only, only available on the iOS platform.
  Future<void> startCapture({ZegoScreenCaptureConfig? config, bool? inApp});

  /// Stop screen capture.
  ///
  /// Available since: 3.1.0
  /// Description: Stop screen capture.
  Future<void> stopCapture();

  /// Gets the rectangle of the screen capture source.
  ///
  /// Available since: 3.6.0
  /// Description: Gets the rectangle of the screen capture source.
  /// Restrictions: Called after starting [startScreenCapture] acquisition, only support in Windows/macOS.
  ///
  /// - Returns Rect information about the capture resource.
  Future<Rect> getCaptureSourceRect();

  /// Update the area captured by the screen.
  ///
  /// Available since: 3.1.0
  /// Description: Update the area captured by the screen.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  /// Restrictions: Only support in Windows/macOS.
  ///
  /// - [rect] The position of the area to be captured relative to the entire screen or window.
  Future<void> updateCaptureRegion(Rect rect);

  /// Update the streaming area of ​​the screen capture.
  ///
  /// Available since: 3.8.0
  /// Description: Update the streaming area of ​​the screen capture.
  /// When to call: After updating the screen capture source [updateScreenCaptureSource].
  /// Restrictions: Every time the screen capture source [updateScreenCaptureSource] is updated, the area will be cleared and needs to be reset, only support in Windows/macOS.
  /// Caution: The set area cannot exceed the size returned by [onAvailableFrame], otherwise the setting will fail and the original frame will be pushed. The failure of the streaming area will be notified through [onExceptionOccurred].
  ///
  /// - [rect] The position of the area to be pushed relative to the upper left corner of the capture screen, the actual capture screen size can be obtained through [onAvailableFrame].
  Future<void> updatePublishRegion(Rect rect);

  /// Sets the filtered list of windows.
  ///
  /// Available since: 3.1.0
  /// Description: Specify a list of windows, and filter these windows when capturing the screen, and not display them on the screen.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [list] List of IDs to filter windows.
  Future<void> setExcludeWindowList(List<int> list);

  /// Whether to activate the promotion of the window to the foreground.
  ///
  /// Available since: 3.1.0
  /// Description: When the capture target is a window, set whether to activate the window to be displayed in the foreground during the first capture.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [active] Whether to activate the window. true to activate the window, false to not activate the window, the default is true.
  Future<void> enableWindowActivate(bool active);

  /// Set whether to show the cursor
  ///
  /// Available since: 3.1.0
  /// Description: Set whether to show the cursor.
  /// When to call: It can be called after the engine by [createScreenCaptureSource] has been initialized.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [visible] Whether to show the cursor. true to show the cursor, false to not show the cursor, the default is false.
  Future<void> enableCursorVisible(bool visible);

  /// Whether to collect the sound of the window process during window collection
  ///
  /// Available since: 3.13.0
  /// Description: Whether to collect the sound of the window process during window collection.
  /// When to call: Before starting the collection [startScreencapture].
  /// Restrictions: Only applicable to Windows 10 and above versions.
  ///
  /// - [enable] Whether to collect sound. true for collection, false for no collection, default false.
  /// - [audioParam] Audio collection parameters.
  Future<void> enableAudioCapture(bool enable, ZegoAudioFrameParam audioParam);

  /// Get screen capture source index.
  ///
  /// - Returns Index of the screen capture source.
  int getIndex();

  /// Set the App Group configuration item.
  ///
  /// Available since: 3.3.0
  /// Use cases: You need to use the iOS cross-process screen sharing function, and you need to start the App Group, which can provide better performance and stability. Must be used with [setupWithAppGroupID:] in the `ZegoReplayKit` extension class.
  /// When to call: Called after [createEngine], before calling [startScreenCapture].
  /// Restrictions: Only available on iOS platform.
  ///
  /// - [groupID] The host app and the extension app should belong to the same App Group, and the AppGroupID needs to be passed in here.
  Future<void> setAppGroupID(String groupID);

  /// Update screen capture parameter configuration.
  ///
  /// Available since: 3.1.0
  /// Description: Update screen capture parameter configuration.
  /// When to call: After calling [startScreenCapture] to start capturing.
  /// Restrictions: Only valid for iOS system. Only available on iOS 12.0 or newer
  ///
  /// - [config] Screen capture parameter configuration.
  Future<void> updateScreenCaptureConfig(ZegoScreenCaptureConfig config);
}

abstract class ZegoAIVoiceChanger {
  /// Get AI voice changer instance index.
  ///
  /// - Returns AI voice changer instance index.
  int getIndex();

  /// Initialize AI voice changer engine.
  Future<void> initEngine();

  /// Update AI voice changer engine models.
  Future<void> update();

  /// Get AI voice changer speaker list.
  Future<void> getSpeakerList();

  /// Set AI voice changer speaker.
  ///
  /// - [speakerID] Speaker ID.
  Future<void> setSpeaker(int speakerID);
}

/// Callback for setting room extra information.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoRoomSetRoomExtraInfoResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoRoomSetRoomExtraInfoResult(this.errorCode);
}

/// Login room result callback.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [extendedData] Extended Information
class ZegoRoomLoginResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Extended Information
  Map<dynamic, dynamic> extendedData;

  ZegoRoomLoginResult(this.errorCode, this.extendedData);
}

/// Logout room result callback.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [extendedData] Extended Information
class ZegoRoomLogoutResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Extended Information
  Map<dynamic, dynamic> extendedData;

  ZegoRoomLogoutResult(this.errorCode, this.extendedData);
}

/// Callback for setting stream extra information.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoPublisherSetStreamExtraInfoResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoPublisherSetStreamExtraInfoResult(this.errorCode);
}

/// Callback for add/remove CDN URL.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoPublisherUpdateCdnUrlResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoPublisherUpdateCdnUrlResult(this.errorCode);
}

/// Results of take publish stream snapshot.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [image] Snapshot image
class ZegoPublisherTakeSnapshotResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Snapshot image
  MemoryImage? image;

  ZegoPublisherTakeSnapshotResult(this.errorCode, {this.image});
}

/// Results of take play stream snapshot.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [image] Snapshot image
class ZegoPlayerTakeSnapshotResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Snapshot image
  MemoryImage? image;

  ZegoPlayerTakeSnapshotResult(this.errorCode, {this.image});
}

/// Results of starting a mixer task.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [extendedData] Extended Information
class ZegoMixerStartResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Extended Information
  Map<dynamic, dynamic> extendedData;

  ZegoMixerStartResult(this.errorCode, this.extendedData);
}

/// Results of stoping a mixer task.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoMixerStopResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoMixerStopResult(this.errorCode);
}

/// Callback for sending real-time sequential data.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoRealTimeSequentialDataSentResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoRealTimeSequentialDataSentResult(this.errorCode);
}

/// Callback for sending broadcast messages.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [messageID] ID of this message
class ZegoIMSendBroadcastMessageResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// ID of this message
  int messageID;

  ZegoIMSendBroadcastMessageResult(this.errorCode, this.messageID);
}

/// Callback for sending barrage message.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [messageID] ID of this message
class ZegoIMSendBarrageMessageResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// ID of this message
  String messageID;

  ZegoIMSendBarrageMessageResult(this.errorCode, this.messageID);
}

/// Callback for sending custom command.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoIMSendCustomCommandResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoIMSendCustomCommandResult(this.errorCode);
}

/// Callback for sending custom command.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoRoomSendTransparentMessageResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoRoomSendTransparentMessageResult(this.errorCode);
}

/// Callback for media player loads resources.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoMediaPlayerLoadResourceResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoMediaPlayerLoadResourceResult(this.errorCode);
}

/// Callback for media player seek to playback progress.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoMediaPlayerSeekToResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoMediaPlayerSeekToResult(this.errorCode);
}

/// The callback of the screenshot of the media player playing screen
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [image] Snapshot image
class ZegoMediaPlayerTakeSnapshotResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Snapshot image
  MemoryImage? image;

  ZegoMediaPlayerTakeSnapshotResult(this.errorCode, {this.image});
}

/// Callback for audio effect player loads resources.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoAudioEffectPlayerLoadResourceResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoAudioEffectPlayerLoadResourceResult(this.errorCode);
}

/// Callback for audio effect player seek to playback progress.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoAudioEffectPlayerSeekToResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoAudioEffectPlayerSeekToResult(this.errorCode);
}

/// Callback for copyrighted music init.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoCopyrightedMusicInitResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoCopyrightedMusicInitResult(this.errorCode);
}

/// Callback of sending extended feature request.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [command] request command.
/// - [result] request result, each request command has corresponding request result.
class ZegoCopyrightedMusicSendExtendedRequestResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// request command.
  String command;

  /// request result, each request command has corresponding request result.
  String result;

  ZegoCopyrightedMusicSendExtendedRequestResult(
      this.errorCode, this.command, this.result);
}

/// Get lrc format lyrics complete callback.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [lyrics] lrc format lyrics.
class ZegoCopyrightedMusicGetLrcLyricResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// lrc format lyrics.
  String lyrics;

  ZegoCopyrightedMusicGetLrcLyricResult(this.errorCode, this.lyrics);
}

/// Get verbatim lyrics complete callback.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [lyrics] verbatim lyrics.
class ZegoCopyrightedMusicGetKrcLyricByTokenResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// verbatim lyrics.
  String lyrics;

  ZegoCopyrightedMusicGetKrcLyricByTokenResult(this.errorCode, this.lyrics);
}

/// Callback for request song.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] The JSON string returned by the song ordering service, including song resource information.
class ZegoCopyrightedMusicRequestSongResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// The JSON string returned by the song ordering service, including song resource information.
  String resource;

  ZegoCopyrightedMusicRequestSongResult(this.errorCode, this.resource);
}

/// Callback for request accompaniment.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] accompany resource information.
class ZegoCopyrightedMusicRequestAccompanimentResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// accompany resource information.
  String resource;

  ZegoCopyrightedMusicRequestAccompanimentResult(this.errorCode, this.resource);
}

/// Callback for request accompaniment clip.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] accompany clip resource information.
class ZegoCopyrightedMusicRequestAccompanimentClipResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// accompany clip resource information.
  String resource;

  ZegoCopyrightedMusicRequestAccompanimentClipResult(
      this.errorCode, this.resource);
}

/// Callback for acquire songs or accompaniment through authorization token.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] song or accompany resource information.
class ZegoCopyrightedMusicGetMusicByTokenResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// song or accompany resource information.
  String resource;

  ZegoCopyrightedMusicGetMusicByTokenResult(this.errorCode, this.resource);
}

/// Callback of requesting music resource.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] The JSON string returned by the song ordering service, including music resource information.
class ZegoCopyrightedMusicRequestResourceResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// The JSON string returned by the song ordering service, including music resource information.
  String resource;

  ZegoCopyrightedMusicRequestResourceResult(this.errorCode, this.resource);
}

/// Callback of getting shared music resource.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [resource] The JSON string returned by the song ordering service, including music resource information.
class ZegoCopyrightedMusicGetSharedResourceResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// The JSON string returned by the song ordering service, including music resource information.
  String resource;

  ZegoCopyrightedMusicGetSharedResourceResult(this.errorCode, this.resource);
}

/// Callback for download song or accompaniment.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
class ZegoCopyrightedMusicDownloadResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  ZegoCopyrightedMusicDownloadResult(this.errorCode);
}

/// Get standard pitch data complete callback.
///
/// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// - [pitch] Standard pitch data.
class ZegoCopyrightedMusicGetStandardPitchResult {
  /// Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  int errorCode;

  /// Standard pitch data.
  String pitch;

  ZegoCopyrightedMusicGetStandardPitchResult(this.errorCode, this.pitch);
}
