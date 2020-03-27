
import 'dart:typed_data';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

class ZegoExpressEngine {

    static ZegoExpressEngine _instance;

    ZegoExpressEngine._internal();

    /// Returns engine singleton instance
    ///
    /// If the engine has not been created or has been destroyed, an unusable engine object will be returned.
    static ZegoExpressEngine get instance {
        return _instance ?? new ZegoExpressEngine._internal();
    }

    /// Create engine singleton instance
    ///
    /// The engine needs to be created and initialized before calling other APIs. The SDK only supports the creation of one instance of ZegoExpressEngine. Multiple calls to this interface return the same object.
    /// [appID] Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console-express.zego.im/. The value ranges from 0 to 4294967295.
    /// [appSign] Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'.
    /// [isTestEnv] Choose to use a test environment or a formal commercial environment, the formal environment needs to submit work order configuration in the ZEGO management console. The test environment is for test development, with a limit of 30 rooms and 230 users. Official environment App is officially launched. ZEGO will provide corresponding server resources according to the configuration records submitted by the developer in the management console. The test environment and the official environment are two sets of environments and cannot be interconnected.
    /// [scenario] The application scenario. Developers can choose one of ZegoScenario based on the scenario of the app they are developing, and the engine will preset a more general setting for specific scenarios based on the set scenario. After setting specific scenarios, developers can still call specific api to set specific parameters if they have customized parameter settings.
    /// [enablePlatformView] Set whether to use Platform View for rendering, true: rendering using Platform View, false: rendering using Texture, default is false
    static Future<void> createEngine(int appID, String appSign, bool isTestEnv, ZegoScenario scenario, {bool enablePlatformView}) async {
        return await ZegoExpressImpl.createEngine(appID, appSign, isTestEnv, scenario, enablePlatformView: enablePlatformView);
    }

    /// Destroy engine singleton object asynchronously
    ///
    /// Used to release resources used by ZegoExpressEngine.
    static Future<void> destroyEngine() async {
        return await ZegoExpressImpl.destroyEngine();
    }

    /// Get ZegoExpressEngine version number
    ///
    /// When the ZegoExpressEngine is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
    /// Developers can also collect this information as the version information of the engine used by the app, so that the ZegoExpressEngine SDK corresponding to each version of the app on the line.
    /// Returns ZegoExpressEngine version
    static Future<String> getVersion() async {
        return await ZegoExpressImpl.getVersion();
    }

    /// Upload logs to ZEGO server
    ///
    /// By default, ZegoExpressEngine creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this interface, ZegoExpressEngine will auto package and upload the log files to the ZEGO server.
    /// Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this interface to upload the local log information of ZegoExpressEngine to help locate user problems.
    Future<void> uploadLog() async {
        return await ZegoExpressImpl.instance.uploadLog();
    }

    /// Set debug details switch and language
    ///
    /// The debug switch is set to on and the language is English by default.
    /// [enable] Detailed debugging information switch
    /// [language] Debugging information language
    Future<void> setDebugVerbose(bool enable, ZegoLanguage language) async {
        return await ZegoExpressImpl.instance.setDebugVerbose(enable, language);
    }

    /// Debug error message callback
    ///
    /// When the APIs are not used correctly, the callback prompts for detailed error information, which is controlled by the [setDebugVerbose] interface
    /// [errorCode] Error code, please refer to the common error code document [https://doc-zh.zego.im/zh/308.html] for details
    /// [funcName] Function name
    /// [info] Detailed error information
    static void Function(int errorCode, String funcName, String info) onDebugError;

    /// Login room. You must log in to the room before startPublishingStream and startPlayingStream the stream.
    ///
    /// To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
    /// Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
    /// Messages sent in one room (eg apis [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, ZegoExpressEngine does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
    /// ZegoExpressEngine supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
    /// If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
    /// [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
    /// [user] User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
    /// [config] Advanced room configuration
    Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config}) async {
        return await ZegoExpressImpl.instance.loginRoom(roomID, user, config: config);
    }

    /// Logout room
    ///
    /// Exiting the room will stop all publishing and playing streams for user, and then ZegoExpressEngine will auto stop local preview UI. After calling this interface, you will receive [onRoomStateUpdate] callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification.
    /// [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
    Future<void> logoutRoom(String roomID) async {
        return await ZegoExpressImpl.instance.logoutRoom(roomID);
    }

    /// Room status change callback
    ///
    /// This callback is triggered when the connection status of the room changes, and the reason for the change is notified. Developers can use this callback to determine the status of the current user in the room. If the connection is being requested for a long time, the general probability is that the user's network is unstable.
    /// [roomID] Room ID, a string of up to 128 bytes in length.
    /// [state] Changed room state
    /// [errorCode] Error code, please refer to the [common error code document](https://doc-en.zego.im/en/308.html) for details
    /// [extendedData] Extended Information
    static void Function(String roomID, ZegoRoomState state, int errorCode, Map<String, dynamic> extendedData) onRoomStateUpdate;

    /// Notification callback for other users in the room to increase or decrease
    ///
    /// Note that the callback is only triggered when the isUserStatusNotify parameter in the ZegoRoomConfig passed loginRoom function is true. Developers can use this callback to determine the situation of users in the room.
    /// When a user logs in to a room for the first time, other users already exist in this room, and a user list of the type of addition is received.
    /// When the user is already in the room, other users in this room will trigger this callback to notify the changed users when they enter or exit the room.
    /// [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// [updateType] Update type (add/delete)
    /// [userList] List of users changed in the current room
    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoUser> userList) onRoomUserUpdate;

    /// Notification of increase or decrease of streams published by other users in the same room
    ///
    /// When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
    /// When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
    /// Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
    /// [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// [updateType] Update type (add/delete)
    /// [streamList] Updated stream list
    static void Function(String roomID, ZegoUpdateType updateType, List<ZegoStream> streamList) onRoomStreamUpdate;

    /// Stream extra information update notification
    ///
    /// When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
    /// The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
    /// Developers can synchronize variable content related to stream IDs based on stream additional information.
    /// [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
    /// [streamList] List of streams that the extra info was updated.
    static void Function(String roomID, List<ZegoStream> streamList) onRoomStreamExtraInfoUpdate;

    /// Start publishing stream, you can call this api to publish the another stream.
    ///
    /// This interface allows users to publish their local audio and video streams to the ZEGO real-time audio and video cloud. Other users in the same room can use the streamID to play the audio and video streams for intercommunication.
    /// Before you start to publish the stream, you need to join the room first by calling [loginRoom]. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
    /// In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
    /// [streamID] Stream ID, a string of up to 256 characters, needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    /// [channel] Publish stream channel
    Future<void> startPublishingStream(String streamID, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.startPublishingStream(streamID, channel: channel);
    }

    /// Stop publishing stream of the specified channel
    ///
    /// This interface allows the user to stop sending local audio and video streams and end the call.
    /// If the user has initiated publish flow, this interface must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
    /// After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
    /// Use this API to stop publishing stream of aux channel.
    /// [channel] Publish stream channel
    Future<void> stopPublishingStream({ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.stopPublishingStream(channel: channel);
    }

    /// Set stream additional information. You can call this api to set params when publishing another streams
    ///
    /// User this interface to set the extra info of the stream, the result will be notified via the [ZegoPublisherSetStreamExtraInfoResult].
    /// The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
    /// Developers can synchronize variable content related to stream IDs based on stream additional information.
    /// [extraInfo] Stream extra information, a string of up to 1024 characters.
    /// [channel] Publish stream channel
    /// Returns Set stream extra information execution result notification
    Future<ZegoPublisherSetStreamExtraInfoResult> setStreamExtraInfo(String extraInfo, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setStreamExtraInfo(extraInfo, channel: channel);
    }

    /// Start/Update local preview. You can call this api to set params when publishing another streams
    ///
    /// The user can see his own local image by calling this interface. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, ZegoExpressEngine internally actively stops previewing by default.
    /// Local view and preview modes can be updated by calling this interface again.
    /// You can set the mirror mode of the preview by calling the [setVideoMirrorMode] interface. The default preview setting is image mirrored.
    /// When this api is called, the audio and video engine module inside ZegoExpressEngine will start really, and it will start to try to collect audio and video. In addition to calling this api normally to preview the local screen, developers can also pass [null] to the canvas parameter, in conjunction with ZegoExpressEngine's sound wave function, in order to achieve the purpose of detecting whether the audio equipment is working properly before logging in to the room.
    /// [canvas] The view used to display the preview image. If the view is set to null, no preview will be made.
    /// [channel] Publish stream channel
    Future<void> startPreview({ZegoCanvas canvas, ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.startPreview(canvas: canvas, channel: channel);
    }

    /// Stop local preview
    ///
    /// This api can be called to stop previewing when there is no need to see the preview locally.
    /// [channel] Publish stream channel
    Future<void> stopPreview({ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.stopPreview(channel: channel);
    }

    /// Set up video configuration. You can call this api to publish another streams
    ///
    /// This api can be used to set the video frame rate, bit rate, video capture resolution, and video encoding output resolution. If you do not call this api, the default resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
    /// It is necessary to set the relevant video configuration before publishing the stream, and only support the modification of the encoding resolution and the bit rate after publishing the stream.
    /// Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
    /// [config] Video configuration, the ZegoExpressEngine provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
    /// [channel] Publish stream channel
    Future<void> setVideoConfig(ZegoVideoConfig config, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setVideoConfig(config, channel: channel);
    }

    /// Set mirror mode. You can call this api to set params when publishing another streams
    ///
    /// This interface can be called to set whether the local preview video and the published video have mirror mode enabled.
    /// [mirrorMode] Mirror mode for previewing or publishing the stream
    /// [channel] Publish stream channel
    Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setVideoMirrorMode(mirrorMode, channel: channel);
    }

    /// set audio config
    ///
    /// You can set the combined value of the audio codec, bit rate, and audio channel through this interface. If this interface is not called, the default is standard quality mode. Should be used before publishing.
    /// If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
    /// [config] Audio config
    Future<void> setAudioConfig(ZegoAudioConfig config) async {
        return await ZegoExpressImpl.instance.setAudioConfig(config);
    }

    /// Stop or resume sending a audio stream. You can call this api to set params when publishing another streams
    ///
    /// This interface can be called when publishing the stream to publish only the video stream without publishing the audio. The SDK still collects and processes the audio, but does not send the audio data to the network. It can be set before publishing.
    /// If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks,
    /// [mute] Whether to stop sending audio streams, true means that only the video stream is sent without sending the audio stream, and false means that the audio and video streams are sent simultaneously. The default is false.
    /// [channel] Publish stream channel
    Future<void> mutePublishStreamAudio(bool mute, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.mutePublishStreamAudio(mute, channel: channel);
    }

    /// Stop or resume sending a video stream. You can call this api to set params when publishing another streams
    ///
    /// When publishing the stream, this interface can be called to publish only the audio stream without publishing the video stream. The local camera can still work normally, and can normally capture, preview and process the video picture, but does not send the video data to the network. It can be set before publishing.
    /// If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks,
    /// [mute] Whether to stop sending video streams, true means that only the audio stream is sent without sending the video stream, and false means that the audio and video streams are sent at the same time. The default is false.
    /// [channel] Publish stream channel
    Future<void> mutePublishStreamVideo(bool mute, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.mutePublishStreamVideo(mute, channel: channel);
    }

    /// Enable or disable traffic control
    ///
    /// Traffic control enables ZegoExpressEngine SDK to dynamically adjust the bitrate of audio and video streaming according to its own and peer current network environment status.
    /// Automatically adapt to the current network environment and fluctuations, so as to ensure the smooth publishing of stream.
    /// [enable] Whether to enable traffic control. The default is ture.
    /// [property] Adjustable property of traffic control. Should be one or the combinations of ZegoTrafficControlProperty enumeration. AdaptiveFPS as default.
    Future<void> enableTrafficControl(bool enable, int property) async {
        return await ZegoExpressImpl.instance.enableTrafficControl(enable, property);
    }

    /// Set the minimum video birtate for traffic control
    ///
    /// Set how should SDK send video data when the network conditions are poor and the minimum video birrate cannot be met.
    /// When this api is not called, the ZegoExpressEngine SDK will automatically adjust the sent video data frames according to the current network uplink conditions by default.
    /// [bitrate] Minimum video bitrate (kbps)
    /// [mode] Video sending mode below the minimum bitrate.
    Future<void> setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode) async {
        return await ZegoExpressImpl.instance.setMinVideoBitrateForTrafficControl(bitrate, mode);
    }

    /// Set the captured volume for publishing stream
    ///
    /// This interface is used to set the audio collection volume. The local user can control the volume of the audio stream sent to the far end. It can be set before publishing.
    /// [volume] Volume percentage. The range is 0 to 100. Default value is 100.
    Future<void> setCaptureVolume(int volume) async {
        return await ZegoExpressImpl.instance.setCaptureVolume(volume);
    }

    /// Add URL to relay to CDN
    ///
    /// You can call this api to publish the audio and video streams that have been published to the ZEGO real-time audio and video cloud to a custom CDN content distribution network that has high latency but supports high concurrent playing stream.
    /// Because this called api is essentially a dynamic retweet of the audio and video streams published to the ZEGO audio and video cloud to different CDNs, this api needs to be called after the audio and video stream is published to ZEGO real-time cloud successfully.
    /// Since ZEGO's audio and video cloud service itself can be configured to support CDN(content distribution networks), this api is mainly used by developers who have CDN content distribution services themselves.
    /// You can use ZEGO's CDN audio and video streaming content distribution service at the same time by calling this interface and then use the developer who owns the CDN content distribution service.
    /// This interface supports dynamic retweets to the CDN content distribution network, so developers can use this api as a disaster recovery solution for CDN content distribution services.
    /// When the [enablePublishDirectToCDN] api is set to true to publish the stream straight to the CDN, then calling this interface will have no effect.
    /// [streamID] Stream ID
    /// [targetURL] CDN relay address, supported address format rtmp.
    /// Returns The execution result notification of the relay CDN operation, and proceed to the next step according to the execution result.
    Future<ZegoPublisherUpdateCdnUrlResult> addPublishCdnUrl(String streamID, String targetURL) async {
        return await ZegoExpressImpl.instance.addPublishCdnUrl(streamID, targetURL);
    }

    /// Delete the URL relayed to the CDN
    ///
    /// This api is called when a CDN relayed address has been added and needs to stop propagating the stream to the CDN.
    /// This api does not stop publishing audio and video stream to the ZEGO audio and video cloud.
    /// [streamID] Stream ID
    /// [targetURL] CDN relay address, supported address format rtmp, flv, hls
    /// Returns The execution result notification of the relay CDN operation, and proceed to the next step according to the execution result.
    Future<ZegoPublisherUpdateCdnUrlResult> removePublishCdnUrl(String streamID, String targetURL) async {
        return await ZegoExpressImpl.instance.removePublishCdnUrl(streamID, targetURL);
    }

    /// Whether to publish stream directly to CDN without passing through Zego real-time video cloud server. You can call this api to set params when publishing another streams
    ///
    /// This api needs to be set before start publishing stream.
    /// After calling this api to publish the audio and video stream directly to the CDN, calling [addPublishCdnUrl] and [removePublishCdnUrl] to dynamically repost to the CDN no longer takes effect, because these two apis retweet or stop the audio and video stream from the ZEGO real-time audio and video cloud If it is published to CDN, if the direct audio and video stream is directly published to the CDN, the audio and video stream cannot be dynamically relay to the CDN through the ZEGO real-time audio and video cloud.
    /// [enable] Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN
    /// [config] CDN configuration, if null, use Zego's background default configuration
    /// [channel] Publish stream channel
    Future<void> enablePublishDirectToCDN(bool enable, ZegoCDNConfig config, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.enablePublishDirectToCDN(enable, config, channel: channel);
    }

    /// set publish watermark. You can call this api to set params when publishing another streams
    ///
    /// Set before publishing. The layout of the watermark cannot exceed the video encoding resolution of stream.
    /// [watermark] The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is null, the watermark is cancelled.
    /// [isPreviewVisible] the watermark is visible on local preview
    /// [channel] Publish stream channel
    Future<void> setPublishWatermark(ZegoWatermark watermark, bool isPreviewVisible, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setPublishWatermark(watermark, isPreviewVisible, channel: channel);
    }

    /// Send SEI. You can call this api to set params when publishing another streams
    ///
    /// This interface can synchronize some other additional information while the developer publishes streaming audio and video streaming data while sending streaming media enhancement supplementary information.
    /// Generally, for scenarios such as synchronizing music lyrics or precise layout of video canvas, you can choose to use this api.
    /// After the anchor sends the SEI, the audience can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
    /// Since SEI information follows video frames or audio frames, and because of network problems, frames may be dropped, so SEI information may also be dropped. To solve this situation, it should be sent several times within the limited frequency.
    /// Limit frequency: Do not exceed 30 times per second.
    /// Note: This api is effective only when there is video data published. SEI information will not be sent without publishing video data.
    /// The SEI data length is limited to 4096 bytes.
    /// [data] SEI data
    /// [dataLength] SEI data length
    /// [channel] Publish stream channel
    Future<void> sendSEI(Uint8List data, int dataLength, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.sendSEI(data, dataLength, channel: channel);
    }

    /// On/off hardware encoding
    ///
    /// Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage. The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
    /// Because hard-coded support is not particularly good for a few models, ZegoExpressEngine uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this interface to enable hard coding.
    /// [enable] Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding
    Future<void> enableHardwareEncoder(bool enable) async {
        return await ZegoExpressImpl.instance.enableHardwareEncoder(enable);
    }

    /// set capture pipline scale mode. Whether the video data is scaled immediately when it is acquired or scaled when it is encoded.
    ///
    /// This interface needs to be set before previewing or streaming.
    /// The main effect is whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
    /// [mode] capture scale mode
    Future<void> setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) async {
        return await ZegoExpressImpl.instance.setCapturePipelineScaleMode(mode);
    }

    /// Publish stream state callback
    ///
    /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback interface.
    /// You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
    /// [streamID] Stream ID
    /// [state] Status of publishing stream
    /// [errorCode] The error code corresponding to the status change of the publish stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details.
    /// [extendedData] Extended Information. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are flv_url_list, rtmp_url_list, hls_url_list. These correspond to the publishing stream URLs of the flv, rtmp, and hls protocols. For CDNs that do not use ZEGO, you do not need to pay attention to this parameter.
    static void Function(String streamID, ZegoPublisherState state, int errorCode, Map<String, dynamic> extendedData) onPublisherStateUpdate;

    /// Publish stream quality callback
    ///
    /// After the successful publish, the callback will be received every 3 seconds. Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.
    /// You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callbacl api, in order to show the uplink network status in real time on the device UI interface.
    /// If you does not know how to use the parameters of this callback api, you can only pay attention to the level field of the quality parameter, which is a comprehensive value describing the uplink network calculated by ZegoExpressEngine based on the quality parameters.
    /// [streamID] Stream ID
    /// [quality] Published stream quality, including audio and video frame rate, bit rate, resolution, RTT, etc.
    static void Function(String streamID, ZegoPublishStreamQuality quality) onPublisherQualityUpdate;

    /// First frame callback notification for local audio captured
    ///
    /// After the startPublishingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
    /// In the case of no startPublishingStream audio and video stream or preview, the first startPublishingStream audio and video stream or first preview, that is, when the engine of the audio and video module inside ZegoExpressEngine starts, it will collect audio data of the local device and receive this callback.
    /// Developers can use this callback to determine whether ZegoExpressEngine has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
    static void Function() onPublisherCapturedAudioFirstFrame;

    /// First frame callback notification for local video captured.
    ///
    /// After the startPublishingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of video data.
    /// In the case of no startPublishingStream video stream or preview, the first startPublishingStream video stream or first preview, that is, when the engine of the audio and video module inside ZegoExpressEngine starts, it will collect video data of the local device and receive this callback.
    /// Developers can use this callback to determine whether ZegoExpressEngine has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
    /// [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    static void Function(ZegoPublishChannel channel) onPublisherCapturedVideoFirstFrame;

    /// Video captured size change callback notification
    ///
    /// After the successful publish, the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
    /// When the audio and video stream is not published or previewed for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the ZegoExpressEngine is started, the video data of the local device will be collected, and the collection resolution will change at this time.
    /// You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
    /// [width] Video capture resolution width
    /// [height] Video capture resolution width
    /// [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
    static void Function(int width, int height, ZegoPublishChannel channel) onPublisherVideoSizeChanged;

    /// Add/Remove CDN address status callback
    ///
    /// After the ZEGO real-time audio and video cloud relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
    /// Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
    /// If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
    /// [streamID] Stream ID
    /// [infoList] List of information that the current CDN is relaying
    static void Function(String streamID, List<ZegoStreamRelayCDNInfo> infoList) onPublisherRelayCDNStateUpdate;

    /// Start playing stream
    ///
    /// This interface allows users to play audio and video streams both from the ZEGO real-time audio and video cloud and from third-party cdn.
    /// Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
    /// In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
    /// Playing the stream ID that does not exist, the SDK continues to try to play after executing this interface. After the stream ID is successfully published, the audio and video stream can be actually played.
    /// The developer can update the player canvas by calling this interface again (the streamID must be the same).
    /// [streamID] Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    /// [canvas] The view used to display the play audio and video stream's image. If the view is set to [null], it will not be displayed.
    /// [config] Advanced player configuration
    Future<void> startPlayingStream(String streamID, {ZegoCanvas canvas, ZegoPlayerConfig config}) async {
        return await ZegoExpressImpl.instance.startPlayingStream(streamID, canvas: canvas, config: config);
    }

    /// Stop playing stream
    ///
    /// This interface allows the user to stop playing the stream. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
    /// [streamID] Stream ID
    Future<void> stopPlayingStream(String streamID) async {
        return await ZegoExpressImpl.instance.stopPlayingStream(streamID);
    }

    /// Set the playback volume of the stream
    ///
    /// This interface is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
    /// You need to reset after [stopPlayingStream] and [startPlayingStream].
    /// [streamID] Stream ID
    /// [volume] Volume percentage. The value ranges from 0 to 100, and the default value is 100.
    Future<void> setPlayVolume(String streamID, int volume) async {
        return await ZegoExpressImpl.instance.setPlayVolume(streamID, volume);
    }

    /// Stop/resume playing the audio data of the stream
    ///
    /// This api can be used to stop playing/retrieving the audio data of the stream. Need to be called after calling startPlayingStream.
    /// This api is only effective for playing stream from ZEGO real-time audio and video cloud (not ZEGO CDN or third-party CDN).
    /// [streamID] Stream ID
    /// [mute] mute flag, true: mute play stream video, false: resume play stream video
    Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
        return await ZegoExpressImpl.instance.mutePlayStreamAudio(streamID, mute);
    }

    /// Stop/resume playing the video data of the stream
    ///
    /// This interface can be used to stop playing/retrieving the video data of the stream. Need to be called after calling startPlayingStream.
    /// This api is only effective for playing stream from ZEGO real-time audio and video cloud (not ZEGO CDN or third-party CDN).
    /// [streamID] Stream ID
    /// [mute] mute flag, true: mute play stream video, false: resume play stream video
    Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
        return await ZegoExpressImpl.instance.mutePlayStreamVideo(streamID, mute);
    }

    /// On/off hardware decoding
    ///
    /// Turn on hardware decoding and use hardware to improve decoding efficiency. Need to be called before calling startPlayingStream.
    /// Because hard-decoded support is not particularly good for a few models, ZegoExpressEngine uses software decoding by default. If the developer finds that the device is hot when playing a high-resolution audio and video stream during testing of some models, you can consider calling this interface to enable hard decoding.
    /// [enable] Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding. The default is false
    Future<void> enableHardwareDecoder(bool enable) async {
        return await ZegoExpressImpl.instance.enableHardwareDecoder(enable);
    }

    /// On/off frame order detection
    ///
    /// [enable] Whether to turn on frame order detection, true: enable check poc,not support B frames, false: disable check poc, support B frames but the screen may temporary splash. The default is true
    Future<void> enableCheckPoc(bool enable) async {
        return await ZegoExpressImpl.instance.enableCheckPoc(enable);
    }

    /// Play stream state callback
    ///
    /// After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback interface.
    /// You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
    /// [streamID] stream ID
    /// [state] Current play state
    /// [errorCode] The error code corresponding to the status change of the playing stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details.
    /// [extendedData] Extended Information
    static void Function(String streamID, ZegoPlayerState state, int errorCode, Map<String, dynamic> extendedData) onPlayerStateUpdate;

    /// Play stream quality callback
    ///
    /// After calling the startPlayingStream successfully, this callback will be triggered every 3 seconds. The collection frame rate, bit rate, RTT, packet loss rate and other quality data  can be obtained, such the health of the publish stream can be monitored in real time.
    /// You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback api, in order to show the downlink network status on the device UI interface in real time.
    /// If you does not know how to use the various parameters of the callback api, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by ZegoExpressEngine based on the quality parameters.
    /// [streamID] Stream ID
    /// [quality] Playing stream quality, including audio and video frame rate, bit rate, resolution, RTT, etc.
    static void Function(String streamID, ZegoPlayStreamQuality quality) onPlayerQualityUpdate;

    /// Play media event callback
    ///
    /// This callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
    /// You can use this callback to make statistics on stutters or to make friendly displays in the UI interface of the app.
    /// [streamID] Stream ID
    /// [event] Play media event callback
    static void Function(String streamID, ZegoPlayerMediaEvent event) onPlayerMediaEvent;

    /// First frame callback notification for remote audio received
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of audio data.
    /// [streamID] Stream ID
    static void Function(String streamID) onPlayerRecvAudioFirstFrame;

    /// First frame callback notification for remote video  received
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it collects the first frame of video  data.
    /// [streamID] Stream ID
    static void Function(String streamID) onPlayerRecvVideoFirstFrame;

    /// First video frame is rendered
    ///
    /// After the startPlayingStream interface is called successfully, the SDK will receive this callback notification when it rendered the first frame of video  data.
    /// You can use this callback to count time consuming that take the first frame time or update the playback stream.
    /// [streamID] Stream ID
    static void Function(String streamID) onPlayerRenderVideoFirstFrame;

    /// playing stream resolution change callback
    ///
    /// If there is a change in the video resolution of the playing stream, the callback will be triggered, and the user can adjust the display for that stream dynamically.
    /// If the publishing stream end triggers the internal stream flow control of ZegoExpressEngine due to a network problem, the encoding resolution of the streaming end may be dynamically reduced, and this callback will also be received at this time.
    /// If the stream is only audio data, the callback will not be received.
    /// This callback will be triggered when the played audio and video stream is actually rendered to the set UI play canvas. You can use this callback notification to update or switch UI components that actually play the stream.
    /// [streamID] Stream ID
    /// [width] The width of the video
    /// [height] The height of the video
    static void Function(String streamID, int width, int height) onPlayerVideoSizeChanged;

    /// Receive SEI
    ///
    /// If sendSEI was called on remote, this callback will be triggered.
    /// If only the pure audio stream is played, the SEI information sent by the streaming end will not be received.
    /// [streamID] Stream ID
    /// [data] SEI content
    static void Function(String streamID, Uint8List data) onPlayerRecvSEI;

    /// Start mix stream task
    ///
    /// Due to the performance considerations of the client device, ZegoExpressEngine's mix stream is to start the mixing stream task on the server side of the ZEGO audio and video cloud for mixing stream.
    /// After calling this api, ZegoExpressEngine initiates a mixing stream request to the ZEGO audio and video cloud. The ZEGO audio and video cloud will find the current publishing stream and perform video layer blending according to the parameters of the mixing stream task requested by ZegoExpressEngine.
    /// If an exception occurs when requesting to start the mixing stream task, for example, the most common mix input stream does not exist, it will be given from the callback error code. For specific error codes, please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html].
    /// If an input stream does not exist in the middle, the mixing stream task will automatically retry playing the input stream for 90 seconds, and will not retry after 90 seconds.
    /// [task] Mix stream task object
    /// Returns Start mix stream task result callback notification
    Future<ZegoMixerStartResult> startMixerTask(ZegoMixerTask task) async {
        return await ZegoExpressImpl.instance.startMixerTask(task);
    }

    /// Stop mix stream task
    ///
    /// Similar to [startMixerTask], after calling this api, ZegoExpressEngine initiates a request to end the mixing stream task to the ZEGO audio and video cloud server.
    /// If you starts the next mixing stream task without stopping the previous mixing stream task, the previous mixing stream task will not stop automatically. The previous mixing stream task will not be stopped automatically until 90 seconds after the input stream of the previous mixing stream task does not exist.
    /// When using the mixing stream function of the ZEGO audio and video cloud service, you should pay attention to the start of the next mixing stream task, and should stop the previous mixing stream task, so as not to cause the anchor has started the next streaming task and mixing with other anchors, and the audience is still playing the output stream of the previous mixing stream task.
    /// [task] Mix stream task object
    /// Returns Stop mix stream task result callback notification
    Future<ZegoMixerStopResult> stopMixerTask(ZegoMixerTask task) async {
        return await ZegoExpressImpl.instance.stopMixerTask(task);
    }

    /// Mixed stream relay CDN status update notification
    ///
    /// In the general case of the ZEGO audio and video cloud mixing stream task, the output stream is published to the CDN using the rtmp protocol, and changes in the state during the publish will be notified from this callback api.
    /// [infoList] List of information that the current CDN is being mixed
    /// [taskID] Mix stream task ID
    static void Function(List<ZegoStreamRelayCDNInfo> infoList, String taskID) onMixerRelayCDNStateUpdate;

    /// Callback when the soundLevel of every stream in the mix stream updated
    ///
    /// You can use this callback to show the effect of the anchors sound level when the audience plays the mixed stream. So audience can notice which anchor is speaking.
    /// [soundLevels] Sound level hashmap, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
    static void Function(Map<int, double> soundLevels) onMixerSoundLevelUpdate;

    /// On/off microphone
    ///
    /// This api is used to control whether the collected audio data is used. When the microphone is turned off, the data is collected and discarded, and the microphone is still occupied.
    /// The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this api simply discards the collected data.
    /// If you really want ZegoExpressEngine to give up occupy the microphone, you can call the [enableAudioCaptureDevice] interface.
    /// [mute] Whether to turn off the microphone, true: turn off microphone, false: turn on microphone. The default is true.
    Future<void> muteMicrophone(bool mute) async {
        return await ZegoExpressImpl.instance.muteMicrophone(mute);
    }

    /// Turn on/off audio output to the device
    ///
    /// This interface is used to control whether the ZegoExpressEngine needs to throw audio data to the device.
    /// [mute] Whether to disable audio output to the device, true: disable audio output, false: enable audio output
    Future<void> muteAudioOutput(bool mute) async {
        return await ZegoExpressImpl.instance.muteAudioOutput(mute);
    }

    /// Choose to use an audio device
    ///
    /// [deviceID] ID of a device obtained by getAudioDeviceList
    /// [deviceType] Audio device type
    Future<void> useAudioDevice(String deviceID, ZegoAudioDeviceType deviceType) async {
        return await ZegoExpressImpl.instance.useAudioDevice(deviceID, deviceType);
    }

    /// On/off audio capture device
    ///
    /// This api is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
    /// Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] interface is generally recommended.
    /// [enable] Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
    Future<void> enableAudioCaptureDevice(bool enable) async {
        return await ZegoExpressImpl.instance.enableAudioCaptureDevice(enable);
    }

    /// On/off camera. You can call this api to set params when publishing another streams
    ///
    /// This interface is used to control whether to start the camera acquisition. After the camera is turned off, video capture will not be performed. At this time, the publish stream will also have no video data.
    /// [enable] Whether to turn on the camera, true: turn on camera, false: turn off camera
    /// [channel] Publishing stream channel.
    Future<void> enableCamera(bool enable, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.enableCamera(enable, channel: channel);
    }

    /// Switch front and rear camera.You can call this api to set params when publishing another streams
    ///
    /// This interface is used to control the front or rear camera
    /// [enable] Whether to use the front camera, true: use the front camera, false: use the the rear camera
    /// [channel] Publishing stream channel.
    Future<void> useFrontCamera(bool enable, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.useFrontCamera(enable, channel: channel);
    }

    /// Start the sound level monitor
    ///
    /// After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
    /// Before entering the room, you can call [startPreview] with this api and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
    /// [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is 100 ms.
    Future<void> startSoundLevelMonitor() async {
        return await ZegoExpressImpl.instance.startSoundLevelMonitor();
    }

    /// Stop the sound level monitor
    ///
    /// After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
    Future<void> stopSoundLevelMonitor() async {
        return await ZegoExpressImpl.instance.stopSoundLevelMonitor();
    }

    /// Start the audio spectrum monitor
    ///
    /// After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
    /// [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is 100 ms.
    Future<void> startAudioSpectrumMonitor() async {
        return await ZegoExpressImpl.instance.startAudioSpectrumMonitor();
    }

    /// Captured sound level update callback
    ///
    /// Callback notification period is 100 ms'
    /// [soundLevel] Locally captured sound level value, ranging from 0.0 to 100.0
    static void Function(double soundLevel) onCapturedSoundLevelUpdate;

    /// Remote sound level update callback
    ///
    /// Callback notification period is 100 ms'
    /// [soundLevels] Remote sound level hashmap, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0
    static void Function(Map<int, double> soundLevels) onRemoteSoundLevelUpdate;

    /// Captured audio spectrum update callback
    ///
    /// Callback notification period is 100 ms'
    /// [audioSpectrum] Locally captured audio spectrum value list. Spectrum value range is [0-2^30]
    static void Function(List<double> audioSpectrum) onCapturedAudioSpectrumUpdate;

    /// Remote audio spectrum update callback
    ///
    /// Callback notification period is 100 ms'
    /// [audioSpectrums] Remote audio spectrum hashmap, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
    static void Function(Map<String, List<double>> audioSpectrums) onRemoteAudioSpectrumUpdate;

    /// Device exception notification
    ///
    /// This callback is triggered when an exception occurs when reading or writing the audio and video device.
    /// [errorCode] The error code corresponding to the status change of the playing stream. Please refer to the common error code documentation [https://doc-zh.zego.im/zh/308.html] for details
    /// [deviceName] device name
    static void Function(int errorCode, String deviceName) onDeviceError;

    /// Remote camera device status notification
    ///
    /// When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
    /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    /// [streamID] Stream ID
    /// [state] Remote camera status
    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteCameraStateUpdate;

    /// Remote microphone device status notification
    ///
    /// When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
    /// Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
    /// [streamID] Stream ID
    /// [state] Remote microphone status
    static void Function(String streamID, ZegoRemoteDeviceState state) onRemoteMicStateUpdate;

    /// On/off echo cancellation
    ///
    /// Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio. It needs to be set before starting the publish, and the setting is invalid after the start of the publish.
    /// [enable] Whether to enable echo cancellation, true: enable echo cancellation, false: disable echo cancellation
    Future<void> enableAEC(bool enable) async {
        return await ZegoExpressImpl.instance.enableAEC(enable);
    }

    /// Set echo cancellation mode
    ///
    /// Switch different echo cancellation modes to control the extent to which echo data is eliminated. Need to be set before starting the publish.
    /// [mode] Echo cancellation mode
    Future<void> setAECMode(ZegoAECMode mode) async {
        return await ZegoExpressImpl.instance.setAECMode(mode);
    }

    /// On/off automatic gain control
    ///
    /// When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent. Need to be set before starting the publish.
    /// [enable] Whether to enable automatic gain control, true: enable AGC, false: disable AGC
    Future<void> enableAGC(bool enable) async {
        return await ZegoExpressImpl.instance.enableAGC(enable);
    }

    /// On/off noise suppression
    ///
    /// Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer. Need to be set before starting the publish.
    /// [enable] Whether to enable noise suppression, true: enable AGC, false: disable AGC
    Future<void> enableANS(bool enable) async {
        return await ZegoExpressImpl.instance.enableANS(enable);
    }

    /// On/off beauty
    ///
    /// Identify the portraits in the video for beauty. It can be set before and after the start of the publish.
    /// [featureBitmask] Bit mask format, you can choose to enable several features in ZegoBeautifyFeature at the same time
    /// [channel] Publishing stream channel.
    Future<void> enableBeautify(ZegoBeautifyFeature featureBitmask, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.enableBeautify(featureBitmask, channel: channel);
    }

    /// Set beauty parameters
    ///
    /// [option] Beauty configuration options
    /// [channel] Publishing stream channel.
    Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
    }

    /// Send room broadcast message
    ///
    /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
    /// Users of up to the first 500 advanced rooms in the same room can receive it, which is generally used when the number of live broadcast rooms is less than 500.
    /// [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
    /// [message] Message content, no longer than 256 bytes
    /// Returns Send broadcast message result callback
    Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
        return await ZegoExpressImpl.instance.sendBroadcastMessage(roomID, message);
    }

    /// Send room barrage message
    ///
    /// There is no limit on the number of transmissions, but the server will actively drop messages if it is sent too frequently.
    /// It can be received by users with more than 500 people in the same room, but it is not reliable, that is, when there are many users in the room or messages are sent frequently between users, the users who receive the messages may not be able to receive them. Generally used for sending live barrage.
    /// [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
    /// [message] Message content, no longer than 256 bytes
    /// Returns Send barrage message result callback
    Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
        return await ZegoExpressImpl.instance.sendBarrageMessage(roomID, message);
    }

    /// Send custom command
    ///
    /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
    /// The type of point-to-point signaling in the same room is generally used for remote control signaling or message sending between users.
    /// [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
    /// [command] Custom command content, no longer than 256 bytes
    /// [toUserList] The users who will receive the command
    /// Returns Send commad result callback
    Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {
        return await ZegoExpressImpl.instance.sendCustomCommand(roomID, command, toUserList);
    }

    /// Receive room broadcast message notification
    ///
    /// [roomID] Room ID
    /// [messageList] list of received messages.
    static void Function(String roomID, List<ZegoBroadcastMessageInfo> messageList) onIMRecvBroadcastMessage;

    /// Receive room barrage message notification
    ///
    /// [roomID] Room ID
    /// [messageList] list of received messages.
    static void Function(String roomID, List<ZegoBarrageMessageInfo> messageList) onIMRecvBarrageMessage;

    /// Receive room custom command notification
    ///
    /// [roomID] Room ID
    /// [fromUser] Sender of the command
    /// [command] Command content received
    static void Function(String roomID, ZegoUser fromUser, String command) onIMRecvCustomCommand;


}
