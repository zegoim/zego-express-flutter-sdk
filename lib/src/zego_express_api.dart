
import 'dart:typed_data';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

class ZegoExpressEngine {

  /// Private constructor
  ZegoExpressEngine._internal();

  /// Engine singleton instance
  static final ZegoExpressEngine instance = ZegoExpressEngine._internal();

  /// Creates a singleton instance of ZegoExpressEngine.
  ///
  /// The engine needs to be created and initialized before calling other functions. The SDK only supports the creation of one instance of ZegoExpressEngine. Multiple calls to this function return the same object.
  ///
  /// - [appID] Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console-express.zego.im The value ranges from 0 to 4294967295.
  /// - [appSign] Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'.
  /// - [isTestEnv] Choose to use a test environment or a formal commercial environment, the formal environment needs to submit work order configuration in the ZEGO management console. The test environment is for test development, with a limit of 10 rooms and 50 users. Official environment App is officially launched. ZEGO will provide corresponding server resources according to the configuration records submitted by the developer in the management console. The test environment and the official environment are two sets of environments and cannot be interconnected.
  /// - [scenario] The application scenario. Developers can choose one of ZegoScenario based on the scenario of the app they are developing, and the engine will preset a more general setting for specific scenarios based on the set scenario. After setting specific scenarios, developers can still call specific functions to set specific parameters if they have customized parameter settings.
  /// - [enablePlatformView] Set whether to use Platform View for rendering, true: rendering using Platform View, false: rendering using Texture, default is false
  static Future<void> createEngine(int appID, String appSign, bool isTestEnv, ZegoScenario scenario, {bool enablePlatformView}) async {
    return await ZegoExpressImpl.createEngine(appID, appSign, isTestEnv, scenario, enablePlatformView: enablePlatformView);
  }

  /// Destroys the singleton instance of ZegoExpressEngine asynchronously.
  ///
  /// Used to release resources used by ZegoExpressEngine.
  static Future<void> destroyEngine() async {
    return await ZegoExpressImpl.destroyEngine();
  }

  /// Set advanced engine configuration.
  ///
  /// Developers need to call this function to set advanced function configuration when they need advanced functions of the engine.
  ///
  /// - [config] Advanced engine configuration
  static Future<void> setEngineConfig(ZegoEngineConfig config) async {
    return await ZegoExpressImpl.setEngineConfig(config);
  }

  /// Gets the SDK's version number.
  ///
  /// When the SDK is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
  /// Developers can also collect this information as the version information of the engine used by the app, so that the SDK corresponding to each version of the app on the line.
  ///
  /// - Returns SDK version
  static Future<String> getVersion() async {
    return await ZegoExpressImpl.getVersion();
  }

  /// Uploads logs to the ZEGO server.
  ///
  /// By default, SDK creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
  /// Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
  /// The function is valid for the entire life cycle of the SDK.
  Future<void> uploadLog() async {
    return await ZegoExpressImpl.instance.uploadLog();
  }

  /// Turns on/off verbose debugging and sets up the log language.
  ///
  /// The debug switch is set to on and the language is English by default.
  ///
  /// - [enable] Detailed debugging information switch
  /// - [language] Debugging information language
  Future<void> setDebugVerbose(bool enable, ZegoLanguage language) async {
    return await ZegoExpressImpl.instance.setDebugVerbose(enable, language);
  }

  /// The callback for obtaining debugging error information.
  ///
  /// When the SDK functions are not used correctly, the callback prompts for detailed error information, which is controlled by the [setDebugVerbose] function
  ///
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [funcName] Function name
  /// - [info] Detailed error information
  static void Function(int errorCode, String funcName, String info) onDebugError;

  /// The callback triggered when the audio/video engine state changes.
  ///
  /// When the developer calls the function that enables audio and video related functions, such as calling [startPreview], [startPublishingStream], [startPlayingStream] and MediaPlayer related function, the audio/video engine will start; when all audio and video functions are stopped, the engine state will become stopped.
  /// When the developer has been [loginRoom], once [logoutRoom] is called, the audio/video engine will stop (preview, publishing/playing stream, MediaPlayer and other audio and video related functions will also stop).
  ///
  /// - [state] The audio/video engine state
  static void Function(ZegoEngineState state) onEngineStateUpdate;

  /// The callback triggered when the room connection state changes.
  ///
  /// This callback is triggered when the connection status of the room changes, and the reason for the change is notified. Developers can use this callback to determine the status of the current user in the room. If the connection is being requested for a long time, the general probability is that the user's network is unstable.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length.
  /// - [state] Changed room state
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [extendedData] Extended Information with state updates. As the standby, only an empty json table is currently returned
  static void Function(String roomID, ZegoRoomState state, int errorCode, Map<String, dynamic> extendedData) onRoomStateUpdate;

  /// The callback triggered when the number of other users in the room increases or decreases.
  ///
  /// Note that the callback is only triggered when the isUserStatusNotify parameter in the ZegoRoomConfig passed loginRoom function is true. Developers can use this callback to determine the situation of users in the room.
  /// If developers need to use ZEGO room users notifications, please make sure that each login user sets isUserStatusNotify to true
  /// When a user logs in to a room for the first time, other users already exist in this room, and a user list of the type of addition is received.
  /// When the user is already in the room, other users in this room will trigger this callback to notify the changed users when they enter or exit the room.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [updateType] Update type (add/delete)
  /// - [userList] List of users changed in the current room
  static void Function(String roomID, ZegoUpdateType updateType, List<ZegoUser> userList) onRoomUserUpdate;

  /// The callback triggered every 30 seconds to report the current number of online users.
  ///
  /// This function is called back every 30 seconds.
  /// Developers can use this callback to show the number of user online in the current room.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [count] Count of online users
  static void Function(String roomID, int count) onRoomOnlineUserCountUpdate;

  /// The callback triggered when the number of streams published by the other users in the same room increases or decreases.
  ///
  /// When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
  /// When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
  /// Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [updateType] Update type (add/delete)
  /// - [streamList] Updated stream list
  /// - [extendedData] Extended information with stream updates.
  static void Function(String roomID, ZegoUpdateType updateType, List<ZegoStream> streamList, Map<String, dynamic> extendedData) onRoomStreamUpdate;

  /// The callback triggered when there is an update on the extra information of the streams published by other users in the same room.
  ///
  /// When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
  /// The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
  /// Developers can synchronize variable content related to stream IDs based on stream additional information.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [streamList] List of streams that the extra info was updated.
  static void Function(String roomID, List<ZegoStream> streamList) onRoomStreamExtraInfoUpdate;

  /// The callback triggered when there is an update on the extra information of the room.
  ///
  /// When a user update the room extra information, other users in the same room will receive the callback.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [roomExtraInfoList] List of the extra info updated.
  static void Function(String roomID, List<ZegoRoomExtraInfo> roomExtraInfoList) onRoomExtraInfoUpdate;

  /// The callback triggered when the state of stream publishing changes.
  ///
  /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
  /// You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
  /// ExtendedData is extended information with state updates. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are flv_url_list, rtmp_url_list, hls_url_list. These correspond to the publishing stream URLs of the flv, rtmp, and hls protocols.
  ///
  /// - [streamID] Stream ID
  /// - [state] Status of publishing stream
  /// - [errorCode] The error code corresponding to the status change of the publish stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [extendedData] Extended information with state updates.
  static void Function(String streamID, ZegoPublisherState state, int errorCode, Map<String, dynamic> extendedData) onPublisherStateUpdate;

  /// The callback triggered every 3 seconds to report the current stream publishing quality.
  ///
  /// After the successful publish, the callback will be received every 3 seconds. Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.
  /// You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback function, in order to show the uplink network status in real time on the device UI.
  /// If you does not know how to use the parameters of this callback function, you can only pay attention to the level field of the quality parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
  ///
  /// - [streamID] Stream ID
  /// - [quality] Publishing stream quality, including audio and video framerate, bitrate, RTT, etc.
  static void Function(String streamID, ZegoPublishStreamQuality quality) onPublisherQualityUpdate;

  /// The callback triggered when the first audio frame is captured.
  ///
  /// After the [startPublishingStream] function is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
  /// In the case of no startPublishingStream audio and video stream or preview, the first startPublishingStream audio and video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect audio data of the local device and receive this callback.
  /// Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
  static void Function() onPublisherCapturedAudioFirstFrame;

  /// The callback triggered when the first video frame is captured.
  ///
  /// After the [startPublishingStream] function is called successfully, the SDK will receive this callback notification when it collects the first frame of video data.
  /// In the case of no startPublishingStream video stream or preview, the first startPublishingStream video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect video data of the local device and receive this callback.
  /// Developers can use this callback to determine whether SDK has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
  ///
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(ZegoPublishChannel channel) onPublisherCapturedVideoFirstFrame;

  /// The callback triggered when the video capture resolution changes.
  ///
  /// After the successful publish, the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
  /// When the audio and video stream is not published or previewed for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the SDK is started, the video data of the local device will be collected, and the collection resolution will change at this time.
  /// You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
  ///
  /// - [width] Video capture resolution width
  /// - [height] Video capture resolution height
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(int width, int height, ZegoPublishChannel channel) onPublisherVideoSizeChanged;

  /// The callback triggered when the state of relayed streaming to CDN changes.
  ///
  /// After the ZEGO real-time audio and video cloud relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
  /// Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
  /// If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
  ///
  /// - [streamID] Stream ID
  /// - [infoList] List of information that the current CDN is relaying
  static void Function(String streamID, List<ZegoStreamRelayCDNInfo> infoList) onPublisherRelayCDNStateUpdate;

  /// The callback triggered when the state of stream playing changes.
  ///
  /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
  /// You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
  ///
  /// - [streamID] stream ID
  /// - [state] Current play state
  /// - [errorCode] The error code corresponding to the status change of the publish stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [extendedData] Extended Information with state updates. As the standby, only an empty json table is currently returned
  static void Function(String streamID, ZegoPlayerState state, int errorCode, Map<String, dynamic> extendedData) onPlayerStateUpdate;

  /// The callback triggered every 3 seconds to report the current stream playing quality.
  ///
  /// After calling the startPlayingStream successfully, this callback will be triggered every 3 seconds. The collection frame rate, bit rate, RTT, packet loss rate and other quality data can be obtained, such the health of the publish stream can be monitored in real time.
  /// You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback function, in order to show the downlink network status on the device UI in real time.
  /// If you does not know how to use the various parameters of the callback function, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
  ///
  /// - [streamID] Stream ID
  /// - [quality] Playing stream quality, including audio and video framerate, bitrate, RTT, etc.
  static void Function(String streamID, ZegoPlayStreamQuality quality) onPlayerQualityUpdate;

  /// The callback triggered when a media event occurs during streaming playing.
  ///
  /// This callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
  /// You can use this callback to make statistics on stutters or to make friendly displays in the UI of the app.
  ///
  /// - [streamID] Stream ID
  /// - [event] Play media event callback
  static void Function(String streamID, ZegoPlayerMediaEvent event) onPlayerMediaEvent;

  /// The callback triggered when the first audio frame is received.
  ///
  /// After the [startPlayingStream] function is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
  ///
  /// - [streamID] Stream ID
  static void Function(String streamID) onPlayerRecvAudioFirstFrame;

  /// The callback triggered when the first video frame is received.
  ///
  /// After the [startPlayingStream] function is called successfully, the SDK will receive this callback notification when it collects the first frame of video data.
  ///
  /// - [streamID] Stream ID
  static void Function(String streamID) onPlayerRecvVideoFirstFrame;

  /// The callback triggered when the first video frame is rendered.
  ///
  /// After the [startPlayingStream] function is called successfully, the SDK will receive this callback notification when it rendered the first frame of video data.
  /// Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
  ///
  /// - [streamID] Stream ID
  static void Function(String streamID) onPlayerRenderVideoFirstFrame;

  /// The callback triggered when the stream playback resolution changes.
  ///
  /// If there is a change in the video resolution of the playing stream, the callback will be triggered, and the user can adjust the display for that stream dynamically.
  /// If the publishing stream end triggers the internal stream flow control of SDK due to a network problem, the encoding resolution of the streaming end may be dynamically reduced, and this callback will also be received at this time.
  /// If the stream is only audio data, the callback will not be received.
  /// This callback will be triggered when the played audio and video stream is actually rendered to the set UI play canvas. You can use this callback notification to update or switch UI components that actually play the stream.
  ///
  /// - [streamID] Stream ID
  /// - [width] Video decoding resolution width
  /// - [height] Video decoding resolution height
  static void Function(String streamID, int width, int height) onPlayerVideoSizeChanged;

  /// The callback triggered when Supplemental Enhancement Information is received.
  ///
  /// After the remote stream is successfully played, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
  /// Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
  ///
  /// - [streamID] Stream ID
  /// - [data] SEI content
  static void Function(String streamID, Uint8List data) onPlayerRecvSEI;

  /// The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
  ///
  /// In the general case of the ZEGO audio and video cloud mixing stream task, the output stream is published to the CDN using the rtmp protocol, and changes in the state during the publish will be notified from this callback function.
  ///
  /// - [taskID] Mix stream task ID
  /// - [infoList] List of information that the current CDN is being mixed
  static void Function(String taskID, List<ZegoStreamRelayCDNInfo> infoList) onMixerRelayCDNStateUpdate;

  /// The callback triggered when the sound level of any input stream changes in the stream mixing process.
  ///
  /// You can use this callback to show the effect of the anchors sound level when the audience plays the mixed stream. So audience can notice which anchor is speaking.
  ///
  /// - [soundLevels] Sound level hash map, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
  static void Function(Map<int, double> soundLevels) onMixerSoundLevelUpdate;

  /// The callback triggered when there is a change to audio devices (i.e. new device added or existing device deleted).
  ///
  /// This callback is triggered when an audio device is added or removed from the system. By listening to this callback, users can update the sound collection or output using a specific device when necessary.
  ///
  /// - [updateType] Update type (add/delete)
  /// - [deviceType] Audio device type
  /// - [deviceInfo] Audio device information
  static void Function(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, List<ZegoDeviceInfo> deviceInfo) onAudioDeviceStateChanged;

  /// The callback triggered when there is a change to video devices (i.e. new device added or existing device deleted).
  ///
  /// This callback is triggered when a video device is added or removed from the system. By listening to this callback, users can update the video capture using a specific device when necessary.
  ///
  /// - [updateType] Update type (add/delete)
  /// - [deviceInfo] Audio device information
  static void Function(ZegoUpdateType updateType, List<ZegoDeviceInfo> deviceInfo) onVideoDeviceStateChanged;

  /// The local captured audio sound level callback.
  ///
  /// To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
  /// The callback notification period is the setting parameter of [startSoundLevelMonitor].
  ///
  /// - [soundLevel] Locally captured sound level value, ranging from 0.0 to 100.0
  static void Function(double soundLevel) onCapturedSoundLevelUpdate;

  /// The remote playing streams audio sound level callback.
  ///
  /// To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is playing the audio and video stream.
  /// The callback notification period is the setting parameter of [startSoundLevelMonitor].
  ///
  /// - [soundLevels] Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0
  static void Function(Map<String, double> soundLevels) onRemoteSoundLevelUpdate;

  /// The local captured audio spectrum callback.
  ///
  /// To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
  /// The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
  ///
  /// - [audioSpectrum] Locally captured audio spectrum value list. Spectrum value range is [0-2^30]
  static void Function(List<double> audioSpectrum) onCapturedAudioSpectrumUpdate;

  /// The remote playing streams audio spectrum callback.
  ///
  /// To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is playing the audio and video stream.
  /// The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
  ///
  /// - [audioSpectrums] Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
  static void Function(Map<String, List<double>> audioSpectrums) onRemoteAudioSpectrumUpdate;

  /// The callback triggered when a device exception occurs.
  ///
  /// This callback is triggered when an exception occurs when reading or writing the audio and video device.
  ///
  /// - [errorCode] The error code corresponding to the status change of the playing stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [deviceName] device name
  static void Function(int errorCode, String deviceName) onDeviceError;

  /// The callback triggered when the state of the remote camera changes.
  ///
  /// When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
  /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
  ///
  /// - [streamID] Stream ID
  /// - [state] Remote camera status
  static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteCameraStateUpdate;

  /// The callback triggered when the state of the remote microphone changes.
  ///
  /// When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
  /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
  ///
  /// - [streamID] Stream ID
  /// - [state] Remote microphone status
  static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteMicStateUpdate;

  /// Callback for device's audio route changed.
  ///
  /// This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.
  ///
  /// - [audioRoute] Current audio route
  static void Function(ZegoAudioRoute audioRoute) onAudioRouteChange;

  /// The callback triggered when Broadcast Messages are received.
  ///
  /// This callback is used to receive broadcast messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
  ///
  /// - [roomID] Room ID
  /// - [messageList] list of received messages.
  static void Function(String roomID, List<ZegoBroadcastMessageInfo> messageList) onIMRecvBroadcastMessage;

  /// The callback triggered when Barrage Messages are received.
  ///
  /// This callback is used to receive barrage messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
  ///
  /// - [roomID] Room ID
  /// - [messageList] list of received messages.
  static void Function(String roomID, List<ZegoBarrageMessageInfo> messageList) onIMRecvBarrageMessage;

  /// The callback triggered when a Custom Command is received.
  ///
  /// This callback is used to receive custom signaling sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
  ///
  /// - [roomID] Room ID
  /// - [fromUser] Sender of the command
  /// - [command] Command content received
  static void Function(String roomID, ZegoUser fromUser, String command) onIMRecvCustomCommand;

  /// The callback triggered when the state of the media player changes.
  ///
  /// - [mediaPlayer] Callback player object
  /// - [state] Media player status
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  static void Function(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerState state, int errorCode) onMediaPlayerStateUpdate;

  /// The callback triggered when the network status of the media player changes.
  ///
  /// - [mediaPlayer] Callback player object
  /// - [networkEvent] Network status event
  static void Function(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerNetworkEvent networkEvent) onMediaPlayerNetworkEvent;

  /// The callback to report the current playback progress of the media player.
  ///
  /// - [mediaPlayer] Callback player object
  /// - [millisecond] Progress in milliseconds
  static void Function(ZegoMediaPlayer mediaPlayer, int millisecond) onMediaPlayerPlayingProgress;

  /// Audio effect playback state callback.
  ///
  /// This callback is triggered when the playback state of a audio effect of the audio effect player changes.
  ///
  /// - [audioEffectPlayer] Audio effect player instance that triggers this callback
  /// - [audioEffectID] The ID of the audio effect resource that triggered this callback
  /// - [state] The playback state of the audio effect
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  static void Function(ZegoAudioEffectPlayer audioEffectPlayer, int audioEffectID, ZegoAudioEffectPlayState state, int errorCode) onAudioEffectPlayStateUpdate;

  /// The callback triggered when the state of data recording (to a file) changes.
  ///
  /// - [state] File recording status, according to which you should determine the state of the file recording or the prompt of the UI.
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [config] Record config
  /// - [channel] Publishing stream channel
  static void Function(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel) onCapturedDataRecordStateUpdate;

  /// The callback to report the current recording progress.
  ///
  /// - [progress] File recording progress, which allows developers to hint at the UI, etc.
  /// - [config] Record config
  /// - [channel] Publishing stream channel
  static void Function(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel) onCapturedDataRecordProgressUpdate;

  /// The system performance status callback.
  ///
  /// To trigger this callback function, the [startPerformanceMonitor] function must be called to start the system performance monitor.
  /// The callback notification period is the setting parameter of [startPerformanceMonitor].
  ///
  /// - [status] The system performance status.
  static void Function(ZegoPerformanceStatus status) onPerformanceStatusUpdate;

  /// Callback for network mode changed.
  ///
  /// This callback will be called when the device's network mode changes, such as switching from WiFi to 5G, or when the network is disconnected.
  ///
  /// - [mode] Current network mode.
  static void Function(ZegoNetworkMode mode) onNetworkModeChanged;

  /// The callback triggered when error occurred when testing network speed.
  ///
  /// - [errorCode] The error code corresponding to the network speed test, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [type] Uplink or downlink
  static void Function(int errorCode, ZegoNetworkSpeedTestType type) onNetworkSpeedTestError;

  /// The callback triggered when quality updated when testing network speed.
  ///
  /// When error occurs or called stopNetworkSpeedTest, this callback will be stopped.
  ///
  /// - [quality] Network speed quality
  /// - [type] Uplink or downlink
  static void Function(ZegoNetworkSpeedTestQuality quality, ZegoNetworkSpeedTestType type) onNetworkSpeedTestQualityUpdate;

}
