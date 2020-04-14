
import 'dart:typed_data';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

class ZegoExpressEngine {

    /// Private constructor
    ZegoExpressEngine._internal();

    /// Engine singleton instance
    static final ZegoExpressEngine instance = ZegoExpressEngine._internal();

    /// Create engine singleton instance
    ///
    /// The engine needs to be created and initialized before calling other APIs. The SDK only supports the creation of one instance of ZegoExpressEngine. Multiple calls to this interface return the same object.
    ///
    /// - [appID] Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console-express.zego.im/. The value ranges from 0 to 4294967295.
    /// - [appSign] Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'.
    /// - [isTestEnv] Choose to use a test environment or a formal commercial environment, the formal environment needs to submit work order configuration in the ZEGO management console. The test environment is for test development, with a limit of 30 rooms and 230 users. Official environment App is officially launched. ZEGO will provide corresponding server resources according to the configuration records submitted by the developer in the management console. The test environment and the official environment are two sets of environments and cannot be interconnected.
    /// - [scenario] The application scenario. Developers can choose one of ZegoScenario based on the scenario of the app they are developing, and the engine will preset a more general setting for specific scenarios based on the set scenario. After setting specific scenarios, developers can still call specific api to set specific parameters if they have customized parameter settings.
    /// - [enablePlatformView] Set whether to use Platform View for rendering, true: rendering using Platform View, false: rendering using Texture, default is false
    static Future<void> createEngine(int appID, String appSign, bool isTestEnv, ZegoScenario scenario, {bool enablePlatformView}) async {
        return await ZegoExpressImpl.createEngine(appID, appSign, isTestEnv, scenario, enablePlatformView: enablePlatformView);
    }

    /// Destroy engine singleton object asynchronously
    ///
    /// Used to release resources used by ZegoExpressEngine.
    static Future<void> destroyEngine() async {
        return await ZegoExpressImpl.destroyEngine();
    }

    /// Get SDK version number
    ///
    /// When the SDK is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
    /// Developers can also collect this information as the version information of the engine used by the app, so that the SDK corresponding to each version of the app on the line.
    ///
    /// - Returns SDK version
    static Future<String> getVersion() async {
        return await ZegoExpressImpl.getVersion();
    }

    /// Upload logs to ZEGO server
    ///
    /// By default, SDK creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this interface, SDK will auto package and upload the log files to the ZEGO server.
    /// Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this interface to upload the local log information of SDK to help locate user problems.
    Future<void> uploadLog() async {
        return await ZegoExpressImpl.instance.uploadLog();
    }

    /// Set debug details switch and language
    ///
    /// The debug switch is set to on and the language is English by default.
    ///
    /// - [enable] Detailed debugging information switch
    /// - [language] Debugging information language
    Future<void> setDebugVerbose(bool enable, ZegoLanguage language) async {
        return await ZegoExpressImpl.instance.setDebugVerbose(enable, language);
    }

    /// Debug error message callback
    ///
    /// When the APIs are not used correctly, the callback prompts for detailed error information, which is controlled by the [setDebugVerbose] interface
    ///
    /// - [errorCode] Error code, please refer to the common error code document [https://doc-zh.zego.im/zh/308.html] for details
    /// - [funcName] Function name
    /// - [info] Detailed error information
    static void Function(int errorCode, String funcName, String info) onDebugError;

    /// Room status change callback
    ///
    /// This callback is triggered when the connection status of the room changes, and the reason for the change is notified. Developers can use this callback to determine the status of the current user in the room. If the connection is being requested for a long time, the general probability is that the user's network is unstable.
    ///
    /// - [roomID] Room ID, a string of up to 128 bytes in length.
    /// - [state] Changed room state
    /// - [errorCode] Error code, please refer to the [common error code document](https://doc-en.zego.im/en/308.html) for details
    /// - [extendedData] Extended Information
    static void Function(String roomID, ZegoRoomState state, int errorCode, Map<String, dynamic> extendedData) onRoomStateUpdate;

    /// Notification callback for other users in the room to increase or decrease
    ///
    /// Note that the callback is only triggered when the isUserStatusNotify parameter in the ZegoRoomConfig passed loginRoom function is true. Developers can use this callback to determine the situation of users in the room.
    /// When a user logs in to a room for the first time, other users already exist in this room, and a user list of the type of addition is received.
    /// When the user is already in the room, other users in this room will trigger this callback to notify the changed users when they enter or exit the room.
    ///
    /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// - [updateType] Update type (add/delete)
    /// - [userList] List of users changed in the current room
    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoUser> userList) onRoomUserUpdate;

    /// Notification of increase or decrease of streams published by other users in the same room
    ///
    /// When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
    /// When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
    /// Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
    ///
    /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// - [updateType] Update type (add/delete)
    /// - [streamList] Updated stream list
    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoStream> streamList) onRoomStreamUpdate;

    /// Stream extra information update notification
    ///
    /// When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
    /// The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
    /// Developers can synchronize variable content related to stream IDs based on stream additional information.
    ///
    /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// - [streamList] List of streams that the extra info was updated.
    static void Function(String roomID, List<ZegoStream> streamList) onRoomStreamExtraInfoUpdate;

    /// Publish stream state callback
    ///
    /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback interface.
    /// You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
    ///
    /// - [streamID] Stream ID
    /// - [state] Status of publishing stream
    /// - [errorCode] The error code corresponding to the status change of the publish stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details.
    /// - [extendedData] Extended Information. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are flv_url_list, rtmp_url_list, hls_url_list. These correspond to the publishing stream URLs of the flv, rtmp, and hls protocols. For CDNs that do not use ZEGO, you do not need to pay attention to this parameter.
    static void Function(String streamID, ZegoPublisherState state, int errorCode, Map<String, dynamic> extendedData) onPublisherStateUpdate;

    /// Publish stream quality callback
    ///
    /// After the successful publish, the callback will be received every 3 seconds. Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.
    /// You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback api, in order to show the uplink network status in real time on the device UI interface.
    /// If you does not know how to use the parameters of this callback api, you can only pay attention to the level field of the quality parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
    ///
    /// - [streamID] Stream ID
    /// - [quality] Published stream quality, including audio and video frame rate, bit rate, resolution, RTT, etc.
    static void Function(String streamID, ZegoPublishStreamQuality quality) onPublisherQualityUpdate;

    /// First frame callback notification for local audio captured
    ///
    /// After the startPublishingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
    /// In the case of no startPublishingStream audio and video stream or preview, the first startPublishingStream audio and video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect audio data of the local device and receive this callback.
    /// Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
    static void Function() onPublisherCapturedAudioFirstFrame;

    /// First frame callback notification for local video captured.
    ///
    /// After the startPublishingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of video data.
    /// In the case of no startPublishingStream video stream or preview, the first startPublishingStream video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect video data of the local device and receive this callback.
    /// Developers can use this callback to determine whether SDK has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
    ///
    /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    static void Function(ZegoPublishChannel channel) onPublisherCapturedVideoFirstFrame;

    /// Video captured size change callback notification
    ///
    /// After the successful publish, the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
    /// When the audio and video stream is not published or previewed for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the SDK is started, the video data of the local device will be collected, and the collection resolution will change at this time.
    /// You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
    ///
    /// - [width] Video capture resolution width
    /// - [height] Video capture resolution width
    /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    static void Function(int width, int height, ZegoPublishChannel channel) onPublisherVideoSizeChanged;

    /// Add/Remove CDN address status callback
    ///
    /// After the ZEGO real-time audio and video cloud relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
    /// Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
    /// If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
    ///
    /// - [streamID] Stream ID
    /// - [infoList] List of information that the current CDN is relaying
    static void Function(String streamID, List<ZegoStreamRelayCDNInfo> infoList) onPublisherRelayCDNStateUpdate;

    /// Play stream state callback
    ///
    /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback interface.
    /// You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
    ///
    /// - [streamID] stream ID
    /// - [state] Current play state
    /// - [errorCode] The error code corresponding to the status change of the playing stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details.
    /// - [extendedData] Extended Information
    static void Function(String streamID, ZegoPlayerState state, int errorCode, Map<String, dynamic> extendedData) onPlayerStateUpdate;

    /// Play stream quality callback
    ///
    /// After calling the startPlayingStream successfully, this callback will be triggered every 3 seconds. The collection frame rate, bit rate, RTT, packet loss rate and other quality data  can be obtained, such the health of the publish stream can be monitored in real time.
    /// You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback api, in order to show the downlink network status on the device UI interface in real time.
    /// If you does not know how to use the various parameters of the callback api, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
    ///
    /// - [streamID] Stream ID
    /// - [quality] Playing stream quality, including audio and video frame rate, bit rate, resolution, RTT, etc.
    static void Function(String streamID, ZegoPlayStreamQuality quality) onPlayerQualityUpdate;

    /// Play media event callback
    ///
    /// This callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
    /// You can use this callback to make statistics on stutters or to make friendly displays in the UI interface of the app.
    ///
    /// - [streamID] Stream ID
    /// - [event] Play media event callback
    static void Function(String streamID, ZegoPlayerMediaEvent event) onPlayerMediaEvent;

    /// First frame callback notification for remote audio received
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
    ///
    /// - [streamID] Stream ID
    static void Function(String streamID) onPlayerRecvAudioFirstFrame;

    /// First frame callback notification for remote video  received
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of video  data.
    ///
    /// - [streamID] Stream ID
    static void Function(String streamID) onPlayerRecvVideoFirstFrame;

    /// First video frame is rendered
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it rendered the first frame of video  data.
    /// You can use this callback to count time consuming that take the first frame time or update the playback stream.
    ///
    /// - [streamID] Stream ID
    static void Function(String streamID) onPlayerRenderVideoFirstFrame;

    /// playing stream resolution change callback
    ///
    /// If there is a change in the video resolution of the playing stream, the callback will be triggered, and the user can adjust the display for that stream dynamically.
    /// If the publishing stream end triggers the internal stream flow control of SDK due to a network problem, the encoding resolution of the streaming end may be dynamically reduced, and this callback will also be received at this time.
    /// If the stream is only audio data, the callback will not be received.
    /// This callback will be triggered when the played audio and video stream is actually rendered to the set UI play canvas. You can use this callback notification to update or switch UI components that actually play the stream.
    ///
    /// - [streamID] Stream ID
    /// - [width] The width of the video
    /// - [height] The height of the video
    static void Function(String streamID, int width, int height) onPlayerVideoSizeChanged;

    /// Receive SEI
    ///
    /// If sendSEI was called on remote, this callback will be triggered.
    /// If only the pure audio stream is played, the SEI information sent by the streaming end will not be received.
    ///
    /// - [streamID] Stream ID
    /// - [data] SEI content
    static void Function(String streamID, Uint8List data) onPlayerRecvSEI;

    /// Mixed stream relay CDN status update notification
    ///
    /// In the general case of the ZEGO audio and video cloud mixing stream task, the output stream is published to the CDN using the rtmp protocol, and changes in the state during the publish will be notified from this callback api.
    ///
    /// - [taskID] Mix stream task ID
    /// - [infoList] List of information that the current CDN is being mixed
    static void Function(String taskID, List<ZegoStreamRelayCDNInfo> infoList) onMixerRelayCDNStateUpdate;

    /// Callback when the soundLevel of every stream in the mix stream updated
    ///
    /// You can use this callback to show the effect of the anchors sound level when the audience plays the mixed stream. So audience can notice which anchor is speaking.
    ///
    /// - [soundLevels] Sound level hash map, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
    static void Function(Map<int, double> soundLevels) onMixerSoundLevelUpdate;

    /// Audio device status change
    ///
    /// This callback is triggered when an audio device is added or removed from the system. By listening to this callback, users can update the sound collection or output using a specific device when necessary.
    ///
    /// - [updateType] Update type (add/delete)
    /// - [deviceType] Audio device type
    /// - [deviceInfo] Audio device information
    static void Function(ZegoUpdateType updateType, ZegoAudioDeviceType deviceType, List<ZegoDeviceInfo> deviceInfo) onAudioDeviceStateChanged;

    /// Audio device status change
    ///
    /// This callback is triggered when a video device is added or removed from the system. By listening to this callback, users can update the video capture using a specific device when necessary.
    ///
    /// - [updateType] Update type (add/delete)
    /// - [deviceInfo] Audio device information
    static void Function(ZegoUpdateType updateType, List<ZegoDeviceInfo> deviceInfo) onVideoDeviceStateChanged;

    /// Captured sound level update callback
    ///
    /// Callback notification period is 100 ms'
    ///
    /// - [soundLevel] Locally captured sound level value, ranging from 0.0 to 100.0
    static void Function(double soundLevel) onCapturedSoundLevelUpdate;

    /// Remote sound level update callback
    ///
    /// Callback notification period is 100 ms'
    ///
    /// - [soundLevels] Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0
    static void Function(Map<String, double> soundLevels) onRemoteSoundLevelUpdate;

    /// Captured audio spectrum update callback
    ///
    /// Callback notification period is 100 ms'
    ///
    /// - [audioSpectrum] Locally captured audio spectrum value list. Spectrum value range is [0-2^30]
    static void Function(List<double> audioSpectrum) onCapturedAudioSpectrumUpdate;

    /// Remote audio spectrum update callback
    ///
    /// Callback notification period is 100 ms'
    ///
    /// - [audioSpectrums] Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
    static void Function(Map<String, List<double>> audioSpectrums) onRemoteAudioSpectrumUpdate;

    /// Device exception notification
    ///
    /// This callback is triggered when an exception occurs when reading or writing the audio and video device.
    ///
    /// - [errorCode] The error code corresponding to the status change of the playing stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details
    /// - [deviceName] device name
    static void Function(int errorCode, String deviceName) onDeviceError;

    /// Remote camera device status notification
    ///
    /// When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
    /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    ///
    /// - [streamID] Stream ID
    /// - [state] Remote camera status
    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteCameraStateUpdate;

    /// Remote microphone device status notification
    ///
    /// When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
    /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    ///
    /// - [streamID] Stream ID
    /// - [state] Remote microphone status
    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteMicStateUpdate;

    /// Receive room broadcast message notification
    ///
    /// - [roomID] Room ID
    /// - [messageList] list of received messages.
    static void Function(String roomID, List<ZegoBroadcastMessageInfo> messageList) onIMRecvBroadcastMessage;

    /// Receive room barrage message notification
    ///
    /// - [roomID] Room ID
    /// - [messageList] list of received messages.
    static void Function(String roomID, List<ZegoBarrageMessageInfo> messageList) onIMRecvBarrageMessage;

    /// Receive room custom command notification
    ///
    /// - [roomID] Room ID
    /// - [fromUser] Sender of the command
    /// - [command] Command content received
    static void Function(String roomID, ZegoUser fromUser, String command) onIMRecvCustomCommand;


}
