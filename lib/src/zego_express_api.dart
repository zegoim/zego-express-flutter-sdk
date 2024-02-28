import 'dart:typed_data';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';
import 'package:flutter/material.dart';

// ignore_for_file: deprecated_member_use_from_same_package

class ZegoExpressEngine {
  /// Private constructor
  ZegoExpressEngine._internal();

  /// Engine singleton instance
  static final ZegoExpressEngine instance = ZegoExpressEngine._internal();

  /// Create ZegoExpressEngine singleton object and initialize SDK.
  ///
  /// Available since: 2.14.0
  /// Description: Create ZegoExpressEngine singleton object and initialize SDK.
  /// When to call: The engine needs to be created before calling other functions.
  /// Restrictions: None.
  /// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
  ///
  /// - [profile] The basic configuration information is used to create the engine.
  static Future<void> createEngineWithProfile(ZegoEngineProfile profile) async {
    return await ZegoExpressImpl.createEngineWithProfile(profile);
  }

  /// Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
  ///
  /// Available since: 1.1.0
  /// Description: Destroy the ZegoExpressEngine singleton object and deinitialize the SDK.
  /// When to call: When the SDK is no longer used, the resources used by the SDK can be released through this interface
  /// Restrictions: None.
  /// Caution: After using [createEngine] to create a singleton, if the singleton object has not been created or has been destroyed, you will not receive related callbacks when calling this function.
  static Future<void> destroyEngine() async {
    return await ZegoExpressImpl.destroyEngine();
  }

  /// Set advanced engine configuration.
  ///
  /// Available since: 1.1.0
  /// Description: Used to enable advanced functions.
  /// When to call: Different configurations have different call timing requirements. For details, please consult ZEGO technical support.
  /// Restrictions: None.
  ///
  /// - [config] Advanced engine configuration
  static Future<void> setEngineConfig(ZegoEngineConfig config) async {
    return await ZegoExpressImpl.setEngineConfig(config);
  }

  /// Set log configuration.
  ///
  /// Available since: 2.3.0
  /// Description: If you need to customize the log file size and path, please call this function to complete the configuration.
  /// When to call: It must be set before calling [createEngine] to take effect. If it is set after [createEngine], it will take effect at the next [createEngine] after [destroyEngine].
  /// Restrictions: None.
  /// Caution: Once this interface is called, the method of setting log size and path via [setEngineConfig] will be invalid.Therefore, it is not recommended to use [setEngineConfig] to set the log size and path.
  ///
  /// - [config] log configuration.
  static Future<void> setLogConfig(ZegoLogConfig config) async {
    return await ZegoExpressImpl.setLogConfig(config);
  }

  /// Set local proxy config.
  ///
  /// Available since: 3.1.0
  /// Description: If you need to use the local proxy feature, please call this function to complete the configuration.
  /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
  /// Restrictions: After [createEngine], can not change the proxy.
  /// Caution: None.
  ///
  /// - [proxyList] proxy info.
  /// - [enable] enable proxy or not.
  static Future<void> setLocalProxyConfig(
      List<ZegoProxyInfo> proxyList, bool enable) async {
    return await ZegoExpressImpl.setLocalProxyConfig(proxyList, enable);
  }

  /// Set cloud proxy config.
  ///
  /// Available since: 3.1.0
  /// Description: If you need to use the cloud proxy feature, please call this function to complete the configuration.
  /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
  /// Restrictions: After [createEngine], can not change the proxy.
  /// Caution: None.
  ///
  /// - [proxyList] proxy info.
  /// - [token] token. if use appsign auth, ignore.
  /// - [enable] enable proxy or not.
  static Future<void> setCloudProxyConfig(
      List<ZegoProxyInfo> proxyList, String token, bool enable) async {
    return await ZegoExpressImpl.setCloudProxyConfig(proxyList, token, enable);
  }

  /// Set license auth.
  ///
  /// Available since: 3.5.0
  /// Description: This function needs to be called to complete the configuration when using License authentication charges.
  /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
  /// Restrictions: Does not support midway changes.
  /// Caution: None.
  ///
  /// - [license] license.
  static Future<void> setLicense(String license) async {
    return await ZegoExpressImpl.setLicense(license);
  }

  /// Set room mode.
  ///
  /// Available since: 2.9.0
  /// Description: If you need to use the multi-room feature, please call this function to complete the configuration.
  /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
  /// Restrictions: If you need to use the multi-room feature, please contact the instant technical support to configure the server support.
  /// Caution: None.
  ///
  /// - [mode] Room mode. Description: Used to set the room mode. Use cases: If you need to enter multiple rooms at the same time for publish-play stream, please turn on the multi-room mode through this interface. Required: True. Default value: ZEGO_ROOM_MODE_SINGLE_ROOM.
  static Future<void> setRoomMode(ZegoRoomMode mode) async {
    return await ZegoExpressImpl.setRoomMode(mode);
  }

  /// Set Geo Fence.
  ///
  /// Available since: 3.4.0
  /// Description: If you need to use the geo fence feature, please call this function to complete the configuration.
  /// When to call: Must be set before calling [createEngine] to take effect, otherwise it will fail.
  /// Restrictions: If you need to use the geo fence feature, please contact ZEGO Technical Support.
  /// Caution: None.
  ///
  /// - [type] Geo fence type. Description: Used to set the geo fence type.
  /// - [areaList] Geo fence area. Description: Used to describe the range of geo fence.
  static Future<void> setGeoFence(
      ZegoGeoFenceType type, List<int> areaList) async {
    return await ZegoExpressImpl.setGeoFence(type, areaList);
  }

  /// Gets the SDK's version number.
  ///
  /// Available since: 1.1.0
  /// Description: If you encounter an abnormality during the running of the SDK, you can submit the problem, log and other information to the ZEGO technical staff to locate and troubleshoot. Developers can also collect current SDK version information through this API, which is convenient for App operation statistics and related issues.
  /// When to call: Any time.
  /// Restrictions: None.
  /// Caution: None.
  ///
  /// - Returns SDK version.
  static Future<String> getVersion() async {
    return await ZegoExpressImpl.getVersion();
  }

  /// Query whether the current SDK supports the specified feature.
  ///
  /// Available since: 2.22.0
  /// Description:
  ///   Since the SDK supports feature trimming, some features may be trimmed;
  ///   you can use this function to quickly determine whether the current SDK supports the specified features,
  ///   such as querying whether the media player feature is supported.
  /// When to call: Any time.
  ///
  /// - [featureType] Type of feature to query.
  /// - Returns Whether the specified feature is supported. true: supported; false: not supported.
  static Future<bool> isFeatureSupported(ZegoFeatureType featureType) async {
    return await ZegoExpressImpl.isFeatureSupported(featureType);
  }

  /// Set room scenario.
  ///
  /// Available since: 3.0.0
  /// Description: You can set the scenario of the room, and the SDK will adopt different optimization strategies for different scenarios in order to obtain better effects; this function does exactly the same thing as the [scenario] parameter in the [profile] configuration of [createEngine].
  /// Use cases: This function is suitable for apps in various audio and video business scenarios, such as 1v1 video call (or voice call) scenario and live show scenario; this function can be used to switch scenarios without destroying the engine through [destroyEngine].
  /// When to call: Must be set before calling [loginRoom] AND after calling [createEngine].
  /// Restrictions: Once you log in to the room, you are no longer allowed to modify the room scenario. If you need to modify the scenario, you need to log out of the room first. If you log in to multiple rooms, you need to log out of all rooms before you can modify it.
  /// Caution:
  ///   1. Users in the same room are recommended to use the same room scenario for best results.
  ///   2. Setting the scenario will affect the audio and video bit rate, frame rate, resolution, codec id, audio device mode, audio route type, traffic control, 3A, ear return and other audio and video configurations. If you have special needs, you can call various other APIs to set the above configuration after calling this API.
  ///   3. Calling this function will override the scenario specified on [createEngine] or the scenario set by the last call to this function.
  ///   4. Calling this function will overwrite the audio and video related configuration you set through APIs such as [setVideoConfig], [setAudioConfig], so it is recommended to set the scenario first and then adjust the audio and video configuration through other APIs.
  ///
  /// - [scenario] Room scenario.
  Future<void> setRoomScenario(ZegoScenario scenario) async {
    return await ZegoExpressImpl.instance.setRoomScenario(scenario);
  }

  /// Uploads logs to the ZEGO server.
  ///
  /// Available since: 1.1.0
  /// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
  /// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
  /// When to call: After [createEngine].
  /// Restrictions:  The frequency limit is once per minute.
  /// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.If you want to call before [createEngine], use the [submitLog] interface.
  Future<void> uploadLog() async {
    return await ZegoExpressImpl.instance.uploadLog();
  }

  /// Uploads logs to the ZEGO server.
  ///
  /// Available since: 3.7.0
  /// Description: By default, SDK creates and prints log files in the App's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
  /// Use cases: Developers can provide a business “feedback” channel in the App. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
  /// When to call: None.
  /// Restrictions: The frequency limit is once per minute.
  /// Caution: 1.After calling this interface to upload logs, if you call [destroyEngine] or exit the App too quickly, there may be a failure.It is recommended to wait a few seconds, and then call [destroyEngine] or exit the App after receiving the upload success callback. 2.It is supported to call before [createEngine]. If it had called [createEngine] before, the last appid will be used to upload the log, otherwise the log will not be uploaded until the next [createEngine].
  static Future<void> submitLog() async {
    return await ZegoExpressImpl.submitLog();
  }

  /// Enable the debug assistant. Note, do not enable this feature in the online version! Use only during development phase!
  ///
  /// Available since: 2.17.0
  /// Description: After enabled, the SDK will print logs to the console, and will pop-up an alert (toast) UI message when there is a problem with calling other SDK functions.
  /// Default value: This function is disabled by default.
  /// When to call: This function can be called right after [createEngine].
  /// Platform differences: The pop-up alert function only supports Android / iOS / macOS / Windows, and the console log function supports all platforms.
  /// Caution: Be sure to confirm that this feature is turned off before the app is released to avoid pop-up UI alert when an error occurs in your release version's app. It is recommended to associate the [enable] parameter of this function with the DEBUG variable of the app, that is, only enable the debug assistant in the DEBUG environment.
  /// Restrictions: None.
  ///
  /// - [enable] Whether to enable the debug assistant.
  Future<void> enableDebugAssistant(bool enable) async {
    return await ZegoExpressImpl.instance.enableDebugAssistant(enable);
  }

  /// Call the experimental API.
  ///
  /// Available since: 2.7.0
  /// Description: ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support.
  /// When to call: After [createEngine].
  ///
  /// - [params] Parameters in the format of a JSON string, please consult ZEGO technical support for details.
  /// - Returns Returns an argument in the format of a JSON string, please consult ZEGO technical support for details.
  Future<String> callExperimentalAPI(String params) async {
    return await ZegoExpressImpl.instance.callExperimentalAPI(params);
  }

  /// The callback for obtaining debugging error information.
  ///
  /// Available since: 1.1.0
  /// Description: When the SDK functions are not used correctly, the callback prompts for detailed error information.
  /// Trigger: Notify the developer when an exception occurs in the SDK.
  /// Restrictions: None.
  /// Caution: None.
  ///
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [funcName] Function name.
  /// - [info] Detailed error information.
  static void Function(int errorCode, String funcName, String info)?
      onDebugError;

  /// Method execution result callback
  ///
  /// Available since: 2.3.0
  /// Description: When the monitoring is turned on through [setApiCalledCallback], the results of the execution of all methods will be called back through this callback.
  /// Trigger: When the developer calls the SDK method, the execution result of the method is called back.
  /// Restrictions: None.
  /// Caution: It is recommended to monitor and process this callback in the development and testing phases, and turn off the monitoring of this callback after going online.
  ///
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [funcName] Function name.
  /// - [info] Detailed error information.
  static void Function(int errorCode, String funcName, String info)?
      onApiCalledResult;

  /// The callback triggered when the audio/video engine state changes.
  ///
  /// Available since: 1.1.0
  /// Description: Callback notification of audio/video engine status update. When audio/video functions are enabled, such as preview, push streaming, local media player, audio data observering, etc., the audio/video engine will enter the start state. When you exit the room or disable all audio/video functions , The audio/video engine will enter the stop state.
  /// Trigger: The developer called the relevant function to change the state of the audio and video engine. For example: 1. Called ZegoExpressEngine's [startPreview], [stopPreview], [startPublishingStream], [stopPublishingStream], [startPlayingStream], [stopPlayingStream], [startAudioDataObserver], [stopAudioDataObserver] and other functions. 2. The related functions of MediaPlayer are called. 3. The [LogoutRoom] function was called. 4. The related functions of RealTimeSequentialDataManager are called.
  /// Restrictions: None.
  /// Caution:
  ///   1. When the developer calls [destroyEngine], this notification will not be triggered because the resources of the SDK are completely released.
  ///   2. If there is no special need, the developer does not need to pay attention to this callback.
  ///
  /// - [state] The audio/video engine state.
  static void Function(ZegoEngineState state)? onEngineStateUpdate;

  /// Experimental API callback
  ///
  /// Available since: 2.7.0
  /// Description: Receive experimental API callbacks in JSON string format.
  /// Caution: Please use this feature with the help of ZEGO Technical Support.
  ///
  /// - [content] Callback content in JSON string format.
  static void Function(String content)? onRecvExperimentalAPI;

  /// The callback that triggered a fatal error, causing the SDK to malfunction and unable to function properly.
  ///
  /// Available since: 3.6.0
  /// Description: The callback that triggered a fatal error.
  /// Trigger: The APP has enabled the restriction of access from foreign IP addresses, and the current client is outside of the domain.
  /// Restrictions: None.
  /// Caution:
  ///   None.
  ///
  /// - [errorCode] Error code.
  static void Function(int errorCode)? onFatalError;

  /// Notification of the room connection state changes.
  ///
  /// Available since: 1.1.0
  /// Description: This callback is triggered when the connection status of the room changes, and the reason for the change is notified.For versions 2.18.0 and above, it is recommended to use the onRoomStateChanged callback instead of the onRoomStateUpdate callback to monitor room state changes.
  /// Use cases: Developers can use this callback to determine the status of the current user in the room.
  /// When to trigger:
  ///  1. The developer will receive this notification when calling the [loginRoom], [logoutRoom], [switchRoom] functions.
  ///  2. This notification may also be received when the network condition of the user's device changes (SDK will automatically log in to the room when disconnected, please refer to [Does ZEGO SDK support a fast reconnection for temporary disconnection] for details](https://docs.zegocloud.com/faq/reconnect?product=ExpressVideo&platform=all).
  /// Restrictions: None.
  /// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
  /// Related APIs: [loginRoom]、[logoutRoom]、[switchRoom]
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length.
  /// - [state] Changed room state.
  /// - [errorCode] Error code, For details, please refer to [Common Error Codes](https://docs.zegocloud.com/article/5548).
  /// - [extendedData] Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
  static void Function(String roomID, ZegoRoomState state, int errorCode,
      Map<String, dynamic> extendedData)? onRoomStateUpdate;

  /// Notification of the room connection state changes, including specific reasons for state change.
  ///
  /// Available since: 2.18.0
  /// Description: This callback is triggered when the connection status of the room changes, and the reason for the change is notified.For versions 2.18.0 and above, it is recommended to use the onRoomStateChanged callback instead of the onRoomStateUpdate callback to monitor room state changes.
  /// Use cases: Developers can use this callback to determine the status of the current user in the room.
  /// When to trigger: Users will receive this notification when they call room functions (refer to [Related APIs]). 2. This notification may also be received when the user device's network conditions change (SDK will automatically log in to the room again when the connection is disconnected, refer to https://doc-zh.zego.im/faq/reconnect ).
  /// Restrictions: None.
  /// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
  /// Related APIs: [loginRoom], [logoutRoom], [switchRoom]
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length.
  /// - [reason] Room state change reason.
  /// - [errorCode] Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
  /// - [extendedData] Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
  static void Function(String roomID, ZegoRoomStateChangedReason reason,
      int errorCode, Map<String, dynamic> extendedData)? onRoomStateChanged;

  /// The callback triggered when the number of other users in the room increases or decreases.
  ///
  /// Available since: 1.1.0
  /// Description: When other users in the room are online or offline, which causes the user list in the room to change, the developer will be notified through this callback.
  /// Use cases: Developers can use this callback to update the user list display in the room in real time.
  /// When to trigger:
  ///   1. When the user logs in to the room for the first time, if there are other users in the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `userList` is the other users in the room at this time.
  ///   2. The user is already in the room. If another user logs in to the room through the [loginRoom] or [switchRoom] functions, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
  ///   3. If other users log out of this room through the [logoutRoom] or [switchRoom] functions, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
  ///   4. The user is already in the room. If another user is kicked out of the room from the server, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
  /// Restrictions: If developers need to use ZEGO room users notifications, please ensure that the [ZegoRoomConfig] sent by each user when logging in to the room has the [isUserStatusNotify] property set to true, otherwise the callback notification will not be received.
  /// Related APIs: [loginRoom]、[logoutRoom]、[switchRoom]
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [updateType] Update type (add/delete).
  /// - [userList] List of users changed in the current room.
  static void Function(
          String roomID, ZegoUpdateType updateType, List<ZegoUser> userList)?
      onRoomUserUpdate;

  /// The callback triggered every 30 seconds to report the current number of online users.
  ///
  /// Available since: 1.7.0
  /// Description: This method will notify the user of the current number of online users in the room.
  /// Use cases: Developers can use this callback to show the number of user online in the current room.
  /// When to call /Trigger: After successfully logging in to the room.
  /// Restrictions: None.
  /// Caution: 1. This function is called back every 30 seconds. 2. Because of this design, when the number of users in the room exceeds 500, there will be some errors in the statistics of the number of online people in the room.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [count] Count of online users.
  static void Function(String roomID, int count)? onRoomOnlineUserCountUpdate;

  /// The callback triggered when the number of streams published by the other users in the same room increases or decreases.
  ///
  /// Available since: 1.1.0
  /// Description: When other users in the room start publishing stream or stop publishing stream, the streaming list in the room changes, and the developer will be notified through this callback.
  /// Use cases: This callback is used to monitor stream addition or stream deletion notifications of other users in the room. Developers can use this callback to determine whether other users in the same room start or stop publishing stream, so as to achieve active playing stream [startPlayingStream] or take the initiative to stop the playing stream [stopPlayingStream], and use it to change the UI controls at the same time.
  /// When to trigger:
  ///   1. When the user logs in to the room for the first time, if there are other users publishing streams in the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `streamList` is an existing stream list.
  ///   2. The user is already in the room. if another user adds a new push, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
  ///   3. The user is already in the room. If other users stop streaming, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
  ///   4. The user is already in the room. If other users leave the room, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
  /// Restrictions: None.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [updateType] Update type (add/delete).
  /// - [streamList] Updated stream list.
  /// - [extendedData] Extended information with stream updates.When receiving a stream deletion notification, the developer can convert the string into a json object to get the stream_delete_reason field, which is an array of stream deletion reasons, and the stream_delete_reason[].code field may have the following values: 1 (the user actively stops publishing stream) ; 2 (user heartbeat timeout); 3 (user repeated login); 4 (user kicked out); 5 (user disconnected); 6 (removed by the server).
  static void Function(
      String roomID,
      ZegoUpdateType updateType,
      List<ZegoStream> streamList,
      Map<String, dynamic> extendedData)? onRoomStreamUpdate;

  /// The callback triggered when there is an update on the extra information of the streams published by other users in the same room.
  ///
  /// Available since: 1.1.0
  /// Description: All users in the room will be notified by this callback when the extra information of the stream in the room is updated.
  /// Use cases: Users can realize some business functions through the characteristics of stream extra information consistent with stream life cycle.
  /// When to call /Trigger: When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
  /// Restrictions: None.
  /// Caution: Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be updated during the life cycle of the corresponding stream ID.
  /// Related APIs: Users who publish stream can set extra stream information through [setStreamExtraInfo].
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [streamList] List of streams that the extra info was updated.
  static void Function(String roomID, List<ZegoStream> streamList)?
      onRoomStreamExtraInfoUpdate;

  /// The callback triggered when there is an update on the extra information of the room.
  ///
  /// Available since: 1.1.0
  /// Description: After the room extra information is updated, all users in the room will be notified except update the room extra information user.
  /// Use cases: Extra information for the room.
  /// When to call /Trigger: When a user update the room extra information, other users in the same room will receive the callback.
  /// Restrictions: None.
  /// Related APIs: Users can update room extra information through [setRoomExtraInfo] function.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [roomExtraInfoList] List of the extra info updated.
  static void Function(
          String roomID, List<ZegoRoomExtraInfo> roomExtraInfoList)?
      onRoomExtraInfoUpdate;

  /// Callback notification that room Token authentication is about to expire.
  ///
  /// Available since: 2.8.0
  /// Description: The callback notification that the room Token authentication is about to expire, please use [renewToken] to update the room Token authentication.
  /// Use cases: In order to prevent illegal entry into the room, it is necessary to perform authentication control on login room, push streaming, etc., to improve security.
  /// When to call /Trigger: 30 seconds before the Token expires, the SDK will call [onRoomTokenWillExpire] to notify developer.
  /// Restrictions: None.
  /// Caution: The token contains important information such as the user's room permissions, publish stream permissions, and effective time, please refer to https://docs.zegocloud.com/article/11649.
  /// Related APIs: When the developer receives this callback, he can use [renewToken] to update the token authentication information.
  ///
  /// - [roomID] Room ID where the user is logged in, a string of up to 128 bytes in length.
  /// - [remainTimeInSecond] The remaining time before the token expires.
  static void Function(String roomID, int remainTimeInSecond)?
      onRoomTokenWillExpire;

  /// The callback triggered when the state of stream publishing changes.
  ///
  /// Available since: 1.1.0
  /// Description: After calling the [startPublishingStream] successfully, the notification of the publish stream state change can be obtained through the callback function. You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
  /// Caution: The parameter [extendedData] is extended information with state updates. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are [flv_url_list], [rtmp_url_list], [hls_url_list], these correspond to the publishing stream URLs of the flv, rtmp, and hls protocols.
  /// Related callbacks: After calling the [startPlayingStream] successfully, the notification of the play stream state change can be obtained through the callback function [onPlayerStateUpdate]. You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
  ///
  /// - [streamID] Stream ID.
  /// - [state] State of publishing stream.
  /// - [errorCode] The error code corresponding to the status change of the publish stream, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [extendedData] Extended information with state updates, include playing stream CDN address.
  static void Function(String streamID, ZegoPublisherState state, int errorCode,
      Map<String, dynamic> extendedData)? onPublisherStateUpdate;

  /// Callback for current stream publishing quality.
  ///
  /// Available since: 1.1.0
  /// Description: After calling the [startPublishingStream] successfully, the callback will be received every 3 seconds default(If you need to change the time, please contact the instant technical support to configure). Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback function, in order to show the uplink network status in real time on the device UI.
  /// Caution: If you does not know how to use the parameters of this callback function, you can only pay attention to the [level] field of the [quality] parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
  /// Related callbacks: After calling the [startPlayingStream] successfully, the callback [onPlayerQualityUpdate] will be received every 3 seconds. You can monitor the health of play streams in real time based on quality data such as frame rate, code rate, RTT, packet loss rate, etc.
  ///
  /// - [streamID] Stream ID.
  /// - [quality] Publishing stream quality, including audio and video framerate, bitrate, RTT, etc.
  static void Function(String streamID, ZegoPublishStreamQuality quality)?
      onPublisherQualityUpdate;

  /// The callback triggered when the first audio frame is captured.
  ///
  /// Available since: 1.1.0
  /// Description: This callback will be received when the SDK starts the microphone to capture the first frame of audio data. If this callback is not received, the audio capture device is occupied or abnormal.
  /// Trigger: When the engine of the audio/video module inside the SDK starts, the SDK will go and collect the audio data from the local device and will receive the callback at that time.
  /// Related callbacks: Determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
  static void Function()? onPublisherCapturedAudioFirstFrame;

  /// The callback triggered when the first video frame is captured.
  ///
  /// Available since: 1.1.0
  /// Description: The SDK will receive this callback when the first frame of video data is captured. If this callback is not received, the video capture device is occupied or abnormal.
  /// Trigger: When the SDK's internal audio/video module's engine starts, the SDK will collect video data from the local device and will receive this callback.
  /// Related callbacks: Determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(ZegoPublishChannel channel)?
      onPublisherCapturedVideoFirstFrame;

  /// The callback triggered when the first audio frame is sent.
  ///
  /// Available since: 3.5.0
  /// Description: After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data. Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
  /// Trigger: In the case of no startPublishingStream audio stream, the first startPublishingStream audio stream, it will receive this callback.
  /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
  ///
  /// - [channel] Publishing stream channel.If you only publish one audio stream, you can ignore this parameter.
  static void Function(ZegoPublishChannel channel)?
      onPublisherSendAudioFirstFrame;

  /// The callback triggered when the first video frame is sent.
  ///
  /// Available since: 3.5.0
  /// Description: After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of video data. Developers can use this callback to determine whether SDK has actually collected video data. If the callback is not received, the video capture device is occupied or abnormal.
  /// Trigger: In the case of no startPublishingStream video stream, the first startPublishingStream video stream, it will receive this callback.
  /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK has rendered the first frame of video data collected by calling back [onPublisherRenderVideoFirstFrame].
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [channel] Publishing stream channel.If you only publish one video stream, you can ignore this parameter.
  static void Function(ZegoPublishChannel channel)?
      onPublisherSendVideoFirstFrame;

  /// The callback triggered when the first video frame is rendered.
  ///
  /// Available since: 2.4.0
  /// Description: this callback will be called after SDK rendered the first frame of video data captured. This interface is for preview rendering. The first frame callback is only available for external collection and internal preview. If it is not for SDK rendering, there is no such callback.
  /// Related callbacks: After the [startPublishingStream] function is called successfully, determine if the SDK actually collected audio data by the callback function [onPublisherCapturedAudioFirstFrame], determine if the SDK actually collected video data by the callback function [onPublisherCapturedVideoFirstFrame].
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(ZegoPublishChannel channel)?
      onPublisherRenderVideoFirstFrame;

  /// The callback triggered when the video capture resolution changes.
  ///
  /// Available since: 1.1.0
  /// Description: When the audio and video stream is not published [startPublishingStream] or previewed [startPreview] for the first time, the publishing stream or preview first time, that is, the engine of the audio and video module inside the SDK is started, the video data of the local device will be collected, and the collection resolution will change at this time.
  /// Trigger: After the successful publish [startPublishingStream], the callback will be received if there is a change in the video capture resolution in the process of publishing the stream.
  /// Use cases: You can use this callback to remove the cover of the local preview UI and similar operations.You can also dynamically adjust the scale of the preview view based on the resolution of the callback.
  /// Caution: What is notified during external collection is the change in encoding resolution, which will be affected by flow control.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [width] Video capture resolution width.
  /// - [height] Video capture resolution height.
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(int width, int height, ZegoPublishChannel channel)?
      onPublisherVideoSizeChanged;

  /// The callback triggered when the state of relayed streaming to CDN changes.
  ///
  /// Available since: 1.1.0
  /// Description: Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
  /// Trigger: After the ZEGO RTC server relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
  /// Caution: If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
  ///
  /// - [streamID] Stream ID.
  /// - [infoList] List of information that the current CDN is relaying.
  static void Function(String streamID, List<ZegoStreamRelayCDNInfo> infoList)?
      onPublisherRelayCDNStateUpdate;

  /// The callback triggered when the video encoder changes in publishing stream.
  ///
  /// Available since: 2.12.0
  /// Description: After the H.265 automatic downgrade policy is enabled, if H.265 encoding is not supported or the encoding fails during the streaming process with H.265 encoding, the SDK will actively downgrade to the specified encoding (H.264), and this callback will be triggered at this time.
  /// When to trigger: In the process of streaming with H.265 encoding, if H.265 encoding is not supported or encoding fails, the SDK will actively downgrade to the specified encoding (H.264), and this callback will be triggered at this time.
  /// Caution: When this callback is triggered, if local video recording or cloud recording is in progress, multiple recording files will be generated. Developers need to collect all the recording files for processing after the recording ends. When this callback is triggered, because the streaming code has changed, the developer can evaluate whether to notify the streaming end, so that the streaming end can deal with it accordingly.
  ///
  /// - [fromCodecID] Video codec ID before the change.
  /// - [toCodecID] Video codec ID after the change.
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(ZegoVideoCodecID fromCodecID, ZegoVideoCodecID toCodecID,
      ZegoPublishChannel channel)? onPublisherVideoEncoderChanged;

  /// The callback triggered when publishing stream.
  ///
  /// Available since: 2.18.0
  /// Description: After start publishing stream, this callback will return the current stream address, resource type and protocol-related information.
  /// When to trigger: Publish and retry publish events.
  /// Caution: None.
  ///
  /// - [eventID] Publish stream event ID
  /// - [streamID] Stream ID.
  /// - [extraInfo] extra info. it is in JSON format. Included information includes "url" for address, "streamProtocol" for stream protocol, including rtmp, flv, avertp, hls, webrtc, etc. "netProtocol" for network protocol, including tcp, udp, quic, "resourceType" for resource type , including cdn, rtc, l3.
  static void Function(
          ZegoStreamEvent eventID, String streamID, String extraInfo)?
      onPublisherStreamEvent;

  /// The video object segmentation state changed.
  ///
  /// Available since: 3.4.0
  /// Description: The object segmentation state of the stream publishing end changes.
  /// When to trigger: When [enableVideoObjectSegmentation] enables or disables object segmentation, notify the developer whether to enable object segmentation according to the actual state.
  /// Caution: This callback depends on enabling preview or stream publishing.
  ///
  /// - [state] Object segmentation state.
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  /// - [errorCode] The error code corresponding to the status change of the object segmentation, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(
      ZegoObjectSegmentationState state,
      ZegoPublishChannel channel,
      int errorCode)? onVideoObjectSegmentationStateChanged;

  /// Video encoding low frame rate warning.
  ///
  /// Available since: 3.8.0
  /// Description: Video encoding low frame rate warning.
  /// When to trigger: This callback triggered by low frame rate in video encoding.
  /// Caution: This callback is disabled by default, if necessary, please contact ZEGO technical support.
  ///
  /// - [codecID] Video codec ID.
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(ZegoVideoCodecID codecID, ZegoPublishChannel channel)?
      onPublisherLowFpsWarning;

  /// The notification for setting the path of the static image displayed when the camera is turned off is incorrect.
  ///
  /// Available since: 3.9.0
  /// Description: The notification for setting the path of the static image displayed when the camera is turned off is incorrect.
  /// When to trigger: If the path for the image is set using [setDummyCaptureImagePath], but the image cannot be obtained during streaming, this callback will be triggered.
  /// Caution: Please make sure that the image path is correct and has read permission before setting it.
  ///
  /// - [errorCode] error code.
  /// - [path] Image path.
  /// - [channel] Publishing stream channel.If you only publish one audio and video stream, you can ignore this parameter.
  static void Function(int errorCode, String path, ZegoPublishChannel channel)?
      onPublisherDummyCaptureImagePathError;

  /// The callback triggered when the state of stream playing changes.
  ///
  /// Available since: 1.1.0
  /// Description: After calling the [startPlayingStream] successfully, the notification of the playing stream state change can be obtained through the callback function. You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
  /// When to trigger:  After calling the [startPublishingStream], this callback is triggered when a playing stream's state changed.
  /// Related callbacks: After calling the [startPublishingStream] successfully, the notification of the publish stream state change can be obtained through the callback function [onPublisherStateUpdate]. You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
  ///
  /// - [streamID] stream ID.
  /// - [state] State of playing stream.
  /// - [errorCode] The error code corresponding to the status change of the playing stream, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [extendedData] Extended Information with state updates. As the standby, only an empty json table is currently returned.
  static void Function(String streamID, ZegoPlayerState state, int errorCode,
      Map<String, dynamic> extendedData)? onPlayerStateUpdate;

  /// Callback for current stream playing quality.
  ///
  /// Available since: 1.1.0
  /// Description: After calling the [startPlayingStream] successfully, the callback will be received every 3 seconds default(If you need to change the time, please contact the instant technical support to configure). Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data can be obtained, and the health of the played audio and video streams can be monitored in real time.
  /// Use cases: You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback function, in order to show the downlink network status on the device UI in real time.
  /// Caution: If you does not know how to use the various parameters of the callback function, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
  /// Related callbacks: After calling the [startPublishingStream] successfully, a callback [onPublisherQualityUpdate] will be received every 3 seconds. You can monitor the health of publish streams in real time based on quality data such as frame rate, code rate, RTT, packet loss rate, etc.
  ///
  /// - [streamID] Stream ID.
  /// - [quality] Playing stream quality, including audio and video framerate, bitrate, RTT, etc.
  static void Function(String streamID, ZegoPlayStreamQuality quality)?
      onPlayerQualityUpdate;

  /// The callback triggered when a media event occurs during streaming playing.
  ///
  /// Available since: 1.1.0
  /// Description: This callback is used to receive pull streaming events.
  /// Use cases: You can use this callback to make statistics on stutters or to make friendly displays in the UI of the app.
  /// When to trigger:  After calling the [startPublishingStream], this callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
  ///
  /// - [streamID] Stream ID.
  /// - [event] Specific events received when playing the stream.
  static void Function(String streamID, ZegoPlayerMediaEvent event)?
      onPlayerMediaEvent;

  /// The callback triggered when the first audio frame is received.
  ///
  /// Available since: 1.1.0
  /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
  /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
  /// Trigger: This callback is triggered when SDK receives the first frame of audio data from the network.
  /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data, and the callback [onPlayerRenderVideoFirstFrame] determines whether the SDK has rendered the first frame of the received video data.
  ///
  /// - [streamID] Stream ID.
  static void Function(String streamID)? onPlayerRecvAudioFirstFrame;

  /// The callback triggered when the first video frame is received. Except for Linux systems, this callback is thrown from the ui thread by default.
  ///
  /// Available since: 1.1.0
  /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of video data.
  /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
  /// Trigger: This callback is triggered when SDK receives the first frame of video data from the network.
  /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvAudioFirstFrame] determines whether the SDK has received the audio data, and the callback [onPlayerRenderVideoFirstFrame] determines whether the SDK has rendered the first frame of the received video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  static void Function(String streamID)? onPlayerRecvVideoFirstFrame;

  /// The callback triggered when the first video frame is rendered.
  ///
  /// Available since: 1.1.0
  /// Description: After the [startPlayingStream] function is called successfully, this callback will be called when SDK rendered the first frame of video data.
  /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
  /// Trigger: This callback is triggered when SDK rendered the first frame of video data from the network.
  /// Related callbacks: After a successful call to [startPlayingStream], the callback function [onPlayerRecvAudioFirstFrame] determines whether the SDK has received the audio data, and the callback [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  static void Function(String streamID)? onPlayerRenderVideoFirstFrame;

  /// Calls back when the stream playing end renders the first frame of the video from the remote camera.
  ///
  /// Available since: 3.0.0
  /// Description: After calling the [startPlayingStream] function to pull the stream successfully, the SDK will receive this callback after pulling the stream and rendering the first frame of remote camera video data.
  /// Use cases: Developer can use this callback to count time consuming that take the first frame time or update the UI for playing stream.
  /// Trigger: After the remote [enableCamera] enables the camera, or after [mutePublishStreamVideo] is true and starts to send video data, the SDK will receive this callback after playing the stream and rendering the first frame of the remote camera video data.
  /// Caution: It is only applicable when the remote end uses the camera to push the stream. Only applicable to RTC publishing and playing streaming scenarios.
  /// Related callbacks: After a successful call to [startPlayingStream], the callback [onPlayerRecvVideoFirstFrame] determines whether the SDK has received the video data.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  static void Function(String streamID)? onPlayerRenderCameraVideoFirstFrame;

  /// The callback triggered when the stream playback resolution changes.
  ///
  /// Available since: 1.1.0
  /// Description: After the [startPlayingStream] function is called successfully, the play resolution will change when the first frame of video data is received, or when the publisher changes the encoding resolution by calling [setVideoConfig], or when the network traffic control strategies work.
  /// Use cases: Developers can update or switch the UI components that actually play the stream based on the final resolution of the stream.
  /// Trigger: After the [startPlayingStream] function is called successfully, this callback is triggered when the video resolution changes while playing the stream.
  /// Caution: If the stream is only audio data, the callback will not be triggered.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  /// - [width] Video decoding resolution width.
  /// - [height] Video decoding resolution height.
  static void Function(String streamID, int width, int height)?
      onPlayerVideoSizeChanged;

  /// The callback triggered when Supplemental Enhancement Information is received.
  ///
  /// Available since: 1.1.0
  /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
  /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI, the local end will receive this callback.
  /// Caution:
  ///  1. This function will switch the UI thread callback data, and the customer can directly operate the UI control in this callback function.
  ///  2. Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
  ///  3. When [mutePlayStreamVideo] or [muteAllPlayStreamVideo] is called to set only the audio stream to be pulled, the SEI will not be received.
  ///
  /// - [streamID] Stream ID.
  /// - [data] SEI content.
  static void Function(String streamID, Uint8List data)? onPlayerRecvSEI;

  /// The callback triggered when Supplemental Enhancement Information is received synchronously.
  ///
  /// Available since: 3.9.0
  /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
  /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends SEI, the local end will receive this callback.
  /// Caution: 1. Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set. 2. When [mutePlayStreamVideo] or [muteAllPlayStreamVideo] is called to set only the audio stream to be pulled, the SEI will not be received.
  ///
  /// - [info] SEI Callback info.
  static void Function(ZegoMediaSideInfo info)? onPlayerRecvMediaSideInfo;

  /// Receive the audio side information content of the remote stream.
  ///
  /// Available since: 2.19.0
  /// Description: After the [startPlayingStream] function is called successfully, when the remote stream sends audio side information, the local end will receive this callback.
  /// Trigger: After the [startPlayingStream] function is called successfully, when the remote stream sends audio side information, the local end will receive this callback.
  /// Caution: 1. When [mutePlayStreamAudio] or [muteAllPlayStreamAudio] is called to set only the video stream to be pulled, the audio side information not be received. 2. Due to factors such as the network, the received data may be missing, but the order is guaranteed.
  /// Related APIs: Send audio side information by the [sendAudioSideInfo] function.
  ///
  /// - [streamID] Stream ID.
  /// - [data] Audio side information content.
  static void Function(String streamID, Uint8List data)?
      onPlayerRecvAudioSideInfo;

  /// Playing stream low frame rate warning.
  ///
  /// Available since: 2.14.0
  /// Description: This callback triggered by low frame rate when playing stream.
  /// When to trigger: This callback triggered by low frame rate when playing stream.
  /// Caution: If the callback is triggered when the user playing the h.265 stream, you can stop playing the h.265 stream and switch to play the H.264 stream.
  ///
  /// - [codecID] Video codec ID.
  /// - [streamID] Stream ID.
  static void Function(ZegoVideoCodecID codecID, String streamID)?
      onPlayerLowFpsWarning;

  /// The callback triggered when playing stream.
  ///
  /// Available since: 2.18.0
  /// Description: After start playing stream, this callback will return the current stream address, resource type and protocol-related information.
  /// When to trigger: Play and retry play events.
  /// Caution: None.
  ///
  /// - [eventID] Play stream event ID
  /// - [streamID] Stream ID.
  /// - [extraInfo] extra info. it is in JSON format. Included information includes "url" for address, "streamProtocol" for stream protocol, including rtmp, flv, avertp, hls, webrtc, etc. "netProtocol" for network protocol, including tcp, udp, quic, "resourceType" for resource type , including cdn, rtc, l3.
  static void Function(
          ZegoStreamEvent eventID, String streamID, String extraInfo)?
      onPlayerStreamEvent;

  /// Playing stream video super resolution enabled state changes.
  ///
  /// Available since: 3.0.0
  /// Description: Playing stream video super resolution enabled state changes.
  /// When to trigger: When [enableVideoSuperResolution] enables or disables video super resolution, the developer will be notified whether to enable video super resolution according to the actual situation when playing stream video rendering.
  /// Caution: None.
  ///
  /// - [streamID] Stream ID.
  /// - [state] Video super resolution state.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(
          String streamID, ZegoSuperResolutionState state, int errorCode)?
      onPlayerVideoSuperResolutionUpdate;

  /// The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
  ///
  /// Available since: 1.2.1
  /// Description: The general situation of the mixing task on the ZEGO RTC server will push the output stream to the CDN using the RTMP protocol, and the state change during the push process will be notified from the callback function.
  /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
  /// When to trigger: After the developer calls the [startMixerTask] function to start mixing, when the ZEGO RTC server pushes the output stream to the CDN, there is a state change.
  /// Restrictions: None.
  /// Related callbacks: Develop can get the sound update notification of each single stream in the mixed stream through [OnMixerSoundLevelUpdate].
  /// Related APIs: Develop can start a mixed flow task through [startMixerTask].
  ///
  /// - [taskID] The mixing task ID. Value range: the length does not exceed 256. Caution: This parameter is in string format and cannot contain URL keywords, such as 'http' and '?' etc., otherwise the push and pull flow will fail. Only supports numbers, English characters and'~','!','@','$','%','^','&','*','(',')','_' ,'+','=','-','`',';',''',',','.','<','>','/','\'.
  /// - [infoList] List of information that the current CDN is being mixed.
  static void Function(String taskID, List<ZegoStreamRelayCDNInfo> infoList)?
      onMixerRelayCDNStateUpdate;

  /// The callback triggered when the sound level of any input stream changes in the stream mixing process.
  ///
  /// Available since: 1.2.1
  /// Description: Developers can use this callback to display the effect of which stream’s anchor is talking on the UI interface of the mixed stream of the audience.
  /// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live teacher and student images.
  /// When to trigger: After the developer calls the [startPlayingStream] function to start playing the mixed stream. Callback notification period is 100 ms.
  /// Restrictions: The callback is triggered every 100 ms, and the trigger frequency cannot be set.Due to the high frequency of this callback, please do not perform time-consuming tasks or UI operations in this callback to avoid stalling.
  /// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to get update notification of mixing stream repost CDN status.
  /// Related APIs: Develop can start a mixed flow task through [startMixerTask].
  ///
  /// - [soundLevels] The sound key-value pair of each single stream in the mixed stream, the key is the soundLevelID of each single stream, and the value is the sound value of the corresponding single stream. Value range: The value range of value is 0.0 ~ 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(Map<int, double> soundLevels)? onMixerSoundLevelUpdate;

  /// The callback triggered when the sound level of any input stream changes in the auto stream mixing process.
  ///
  /// Available since: 2.10.0
  /// Description: According to this callback, user can obtain the sound level information of each stream pulled during auto stream mixing.
  /// Use cases: Often used in voice chat room scenarios.Users can use this callback to show which streamer is speaking when an audience pulls a mixed stream.
  /// Trigger: Call [startPlayingStream] function to pull the stream.
  /// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
  ///
  /// - [soundLevels] Sound level hash map, key is the streamID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(Map<String, double> soundLevels)?
      onAutoMixerSoundLevelUpdate;

  /// The callback triggered when there is a change to audio devices (i.e. new device added or existing device deleted).
  ///
  /// Only supports desktop.
  /// This callback is triggered when an audio device is added or removed from the system. By listening to this callback, users can update the sound collection or output using a specific device when necessary.
  ///
  /// - [updateType] Update type (add/delete)
  /// - [deviceType] Audio device type
  /// - [deviceInfo] Audio device information
  static void Function(
      ZegoUpdateType updateType,
      ZegoAudioDeviceType deviceType,
      ZegoDeviceInfo deviceInfo)? onAudioDeviceStateChanged;

  /// The callback triggered when there is a change of the volume for the audio devices.
  ///
  /// Available since: 1.1.0
  /// Description: Audio device volume change event callback.
  /// When to trigger: After calling the [startAudioDeviceVolumeMonitor] function to start the device volume monitor, and the volume of the monitored audio device changes.
  /// Platform differences: Only supports Windows and macOS.
  ///
  /// - [deviceType] Audio device type
  /// - [deviceID] Audio device ID
  /// - [volume] audio device volume
  static void Function(
          ZegoAudioDeviceType deviceType, String deviceID, int volume)?
      onAudioDeviceVolumeChanged;

  /// The callback triggered when there is a change to video devices (i.e. new device added or existing device deleted).
  ///
  /// Available since: 1.1.0
  /// Description: By listening to this callback, users can update the video capture using a specific device when necessary.
  /// When to trigger: This callback is triggered when a video device is added or removed from the system.
  /// Restrictions: None
  /// Platform differences: Only supports Windows and macOS.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [updateType] Update type (add/delete)
  /// - [deviceInfo] Audio device information
  static void Function(ZegoUpdateType updateType, ZegoDeviceInfo deviceInfo)?
      onVideoDeviceStateChanged;

  /// The local captured audio sound level callback.
  ///
  /// Available since: 1.1.0
  /// Description: The local captured audio sound level callback.
  /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor].
  /// Caution:
  ///   1. The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called. The callback value is the default value of 0 When you have not called the interface [startPublishingStream] and [startPreview].
  ///   2. This callback is a high-frequency callback, and it is recommended not to do complex logic processing inside the callback.
  /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring remote played audio sound level by callback [onRemoteSoundLevelUpdate]
  ///
  /// - [soundLevel] Locally captured sound level value, ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.) .
  static void Function(double soundLevel)? onCapturedSoundLevelUpdate;

  /// The local captured audio sound level callback, supported vad.
  ///
  /// Available since: 2.10.0
  /// Description: The local captured audio sound level callback.
  /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor].
  /// Caution:
  ///   1. The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
  ///   2. This callback is a high-frequency callback, and it is recommended not to do complex logic processing inside the callback.
  /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring remote played audio sound level by callback [onRemoteSoundLevelUpdate] or [onRemoteSoundLevelInfoUpdate].
  ///
  /// - [soundLevelInfo] Locally captured sound level value, ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(ZegoSoundLevelInfo soundLevelInfo)?
      onCapturedSoundLevelInfoUpdate;

  /// The remote playing streams audio sound level callback.
  ///
  /// Available since: 1.1.0
  /// Description: The remote playing streams audio sound level callback.
  /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor], you are in the state of playing the stream [startPlayingStream].
  /// Caution: The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
  /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring local captured audio sound by callback [onCapturedSoundLevelUpdate] or [onCapturedSoundLevelInfoUpdate].
  ///
  /// - [soundLevels] Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(Map<String, double> soundLevels)?
      onRemoteSoundLevelUpdate;

  /// The remote playing streams audio sound level callback, supported vad.
  ///
  /// Available since: 2.10.0
  /// Description: The remote playing streams audio sound level callback.
  /// Trigger: After you start the sound level monitor by calling [startSoundLevelMonitor], you are in the state of playing the stream [startPlayingStream].
  /// Caution: The callback notification period is the parameter value set when the [startSoundLevelMonitor] is called.
  /// Related APIs: Start sound level monitoring via [startSoundLevelMonitor]. Monitoring local captured audio sound by callback [onCapturedSoundLevelUpdate] or [onCapturedSoundLevelInfoUpdate].
  ///
  /// - [soundLevelInfos] Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0 (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(Map<String, ZegoSoundLevelInfo> soundLevelInfos)?
      onRemoteSoundLevelInfoUpdate;

  /// The local captured audio spectrum callback.
  ///
  /// Available since: 1.1.0
  /// Description: The local captured audio spectrum callback.
  /// Trigger: After you start the audio spectrum monitor by calling [startAudioSpectrumMonitor].
  /// Caution: The callback notification period is the parameter value set when the [startAudioSpectrumMonitor] is called. The callback value is the default value of 0 When you have not called the interface [startPublishingStream] and [startPreview].
  /// Related APIs: Start audio spectrum monitoring via [startAudioSpectrumMonitor]. Monitoring remote played audio spectrum by callback [onRemoteAudioSpectrumUpdate]
  ///
  /// - [audioSpectrum] Locally captured audio spectrum value list. Spectrum value range is [0-2^30].
  static void Function(List<double> audioSpectrum)?
      onCapturedAudioSpectrumUpdate;

  /// The remote playing streams audio spectrum callback.
  ///
  /// Available since: 1.1.0
  /// Description: The remote playing streams audio spectrum callback.
  /// Trigger: After you start the audio spectrum monitor by calling [startAudioSpectrumMonitor], you are in the state of playing the stream [startPlayingStream].
  /// Caution: The callback notification period is the parameter value set when the [startAudioSpectrumMonitor] is called.
  /// Related APIs: Start audio spectrum monitoring via [startAudioSpectrumMonitor]. Monitoring local played audio spectrum by callback [onCapturedAudioSpectrumUpdate].
  ///
  /// - [audioSpectrums] Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
  static void Function(Map<String, List<double>> audioSpectrums)?
      onRemoteAudioSpectrumUpdate;

  /// The callback triggered when a local device exception occurred.
  ///
  /// Available since: 2.15.0
  /// Description: The callback triggered when a local device exception occurs.
  /// Trigger: This callback is triggered when the function of the local audio or video device is abnormal.
  ///
  /// - [exceptionType] The type of the device exception.
  /// - [deviceType] The type of device where the exception occurred.
  /// - [deviceID] Device ID. Currently, only desktop devices are supported to distinguish different devices; for mobile devices, this parameter will return an empty string.
  static void Function(
      ZegoDeviceExceptionType exceptionType,
      ZegoDeviceType deviceType,
      String deviceID)? onLocalDeviceExceptionOccurred;

  /// The callback triggered when the state of the remote camera changes.
  ///
  /// Available since: 1.1.0
  /// Description: The callback triggered when the state of the remote camera changes.
  /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the camera device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
  /// Trigger: When the state of the remote camera device changes, such as switching the camera, by monitoring this callback, it is possible to obtain an event related to the far-end camera, which can be used to prompt the user that the video may be abnormal.
  /// Caution: This callback will not be called back when the remote stream is play from the CDN, or when custom video acquisition is used at the peer.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [streamID] Stream ID.
  /// - [state] Remote camera status.
  static void Function(String streamID, ZegoRemoteDeviceState state)?
      onRemoteCameraStateUpdate;

  /// The callback triggered when the state of the remote microphone changes.
  ///
  /// Available since: 1.1.0
  /// Description: The callback triggered when the state of the remote microphone changes.
  /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
  /// Trigger: When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
  /// Caution: This callback will not be called back when the remote stream is play from the CDN, or when custom audio acquisition is used at the peer (But the stream is not published to the ZEGO RTC server.).
  ///
  /// - [streamID] Stream ID.
  /// - [state] Remote microphone status.
  static void Function(String streamID, ZegoRemoteDeviceState state)?
      onRemoteMicStateUpdate;

  /// The callback triggered when the state of the remote speaker changes.
  ///
  /// Available since: 1.1.0
  /// Description: The callback triggered when the state of the remote microphone changes.
  /// Use cases: Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the speaker device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
  /// Trigger: When the state of the remote speaker device changes, such as switching the speaker, by monitoring this callback, you can get events related to the remote speaker.
  /// Caution: This callback will not be called back when the remote stream is play from the CDN.
  ///
  /// - [streamID] Stream ID.
  /// - [state] Remote speaker status.
  static void Function(String streamID, ZegoRemoteDeviceState state)?
      onRemoteSpeakerStateUpdate;

  /// Callback for device's audio route changed.
  ///
  /// Available since: 1.20.0
  /// Description: Callback for device's audio route changed.
  /// Trigger: This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.
  /// Platform differences: Only supports iOS and Android.
  ///
  /// - [audioRoute] Current audio route.
  static void Function(ZegoAudioRoute audioRoute)? onAudioRouteChange;

  /// Callback for audio VAD  stable state update.
  ///
  /// Available since: 2.14.0
  /// Description: Callback for audio VAD  stable state update.
  /// When to trigger: the [startAudioVADStableStateMonitor] function must be called to start the audio VAD monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
  /// Restrictions: The callback notification period is 3 seconds.
  /// Related APIs: [startAudioVADStableStateMonitor], [stopAudioVADStableStateMonitor].
  ///
  /// - [type] audio VAD monitor type
  /// - [state] VAD result
  static void Function(
          ZegoAudioVADStableStateMonitorType type, ZegoAudioVADType state)?
      onAudioVADStateUpdate;

  /// Callback for receiving real-time sequential data.
  ///
  /// Available since: 2.14.0
  /// Description: Through this callback, you can receive real-time sequential data from the current subscribing stream.
  /// Use cases: You need to listen to this callback when you need to receive real-time sequential data.
  /// When to trigger: After calling [startSubscribing] to successfully start the subscription, and when data is sent on the stream, this callback will be triggered.
  /// Restrictions: None.
  /// Caution: None.
  ///
  /// - [manager] The real-time sequential data manager instance that triggers this callback.
  /// - [data] The received real-time sequential data.
  /// - [streamID] Subscribed stream ID
  static void Function(ZegoRealTimeSequentialDataManager manager,
      Uint8List data, String streamID)? onReceiveRealTimeSequentialData;

  /// The callback triggered when Barrage Messages are received.
  ///
  /// Available since: 1.5.0
  /// Description: This callback is used to receive room passthrough messages sent by other users in the same room.
  /// When to trigger: After calling [loginRoom] to login to the room, this callback is triggered if there is a user in the room who sends a message received by the specified client through the [sendTransparentMessage] function.
  /// Restrictions: None
  /// Caution: Barrage messages sent by users themselves will not be notified through this callback. When there are a large number of barrage messages in the room, the notification may be delayed, and some barrage messages may be lost.
  /// Related callbacks: A bullet-screen message sent by the user himself is not notified by this callback. [sendTransparentMessage] specifies that only a server callback is used. This callback is not triggered.
  ///
  /// - [roomID] Room ID. Value range: The maximum length is 128 bytes.
  /// - [message] recv message.
  static void Function(String roomID, ZegoRoomRecvTransparentMessage message)?
      onRecvRoomTransparentMessage;

  /// The callback triggered when Broadcast Messages are received.
  ///
  /// Available since: 1.2.1
  /// Description: This callback is used to receive broadcast messages sent by other users in the same room.
  /// Use cases: Generally used when the number of people in the live room does not exceed 500
  /// When to trigger: After calling [loginRoom] to log in to the room, if a user in the room sends a broadcast message via [sendBroadcastMessage] function, this callback will be triggered.
  /// Restrictions: None
  /// Caution: The broadcast message sent by the user will not be notified through this callback.
  /// Related callbacks: You can receive room barrage messages through [onIMRecvBarrageMessage], and you can receive room custom signaling through [onIMRecvCustomCommand].
  ///
  /// - [roomID] Room ID. Value range: The maximum length is 128 bytes.
  /// - [messageList] List of received messages. Value range: Up to 50 messages can be received each time.
  static void Function(
          String roomID, List<ZegoBroadcastMessageInfo> messageList)?
      onIMRecvBroadcastMessage;

  /// The callback triggered when Barrage Messages are received.
  ///
  /// Available since: 1.5.0
  /// Description: This callback is used to receive barrage messages sent by other users in the same room.
  /// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
  /// When to trigger: After calling [loginRoom] to log in to the room, if a user in the room sends a barrage message through the [sendBarrageMessage] function, this callback will be triggered.
  /// Restrictions: None
  /// Caution: Barrage messages sent by users themselves will not be notified through this callback. When there are a large number of barrage messages in the room, the notification may be delayed, and some barrage messages may be lost.
  /// Related callbacks: Develop can receive room broadcast messages through [onIMRecvBroadcastMessage], and can receive room custom signaling through [onIMRecvCustomCommand].
  ///
  /// - [roomID] Room ID. Value range: The maximum length is 128 bytes.
  /// - [messageList] List of received messages. Value range: Up to 50 messages can be received each time.
  static void Function(String roomID, List<ZegoBarrageMessageInfo> messageList)?
      onIMRecvBarrageMessage;

  /// The callback triggered when a Custom Command is received.
  ///
  /// Available since: 1.2.1
  /// Description: This callback is used to receive custom command sent by other users in the same room.
  /// Use cases: Generally used when the number of people in the live room does not exceed 500
  /// When to trigger: After calling [loginRoom] to log in to the room, if other users in the room send custom signaling to the developer through the [sendCustomCommand] function, this callback will be triggered.
  /// Restrictions: None
  /// Caution: The custom command sent by the user himself will not be notified through this callback.
  /// Related callbacks: You can receive room broadcast messages through [onIMRecvBroadcastMessage], and you can receive room barrage message through [onIMRecvBarrageMessage].
  ///
  /// - [roomID] Room ID. Value range: The maximum length is 128 bytes.
  /// - [fromUser] Sender of the command.
  /// - [command] Command content received.Value range: The maximum length is 1024 bytes.
  static void Function(String roomID, ZegoUser fromUser, String command)?
      onIMRecvCustomCommand;

  /// MediaPlayer playback status callback.
  ///
  /// Available since: 1.3.4
  /// Description: MediaPlayer playback status callback.
  /// Trigger: The callback triggered when the state of the media player changes.
  /// Restrictions: None.
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [state] Media player status.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerState state,
      int errorCode)? onMediaPlayerStateUpdate;

  /// The callback triggered when the network status of the media player changes.
  ///
  /// Available since: 1.3.4
  /// Description: The callback triggered when the network status of the media player changes.
  /// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
  /// Restrictions: The callback will only be triggered when the network resource is played.
  /// Related APIs: [setNetWorkBufferThreshold].
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [networkEvent] Network status event.
  static void Function(ZegoMediaPlayer mediaPlayer,
      ZegoMediaPlayerNetworkEvent networkEvent)? onMediaPlayerNetworkEvent;

  /// The callback to report the current playback progress of the media player.
  ///
  /// Available since: 1.3.4
  /// Description: The callback triggered when the network status of the media player changes. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
  /// Trigger: When the media player is playing network resources, this callback will be triggered when the status change of the cached data.
  /// Restrictions: None.
  /// Related APIs: [setProgressInterval].
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [millisecond] Progress in milliseconds.
  static void Function(ZegoMediaPlayer mediaPlayer, int millisecond)?
      onMediaPlayerPlayingProgress;

  /// The callback to report the current rendering progress of the media player.
  ///
  /// Available since: 3.8.0
  /// Description: The callback to report the current rendering progress of the media player. Set the callback interval by calling [setProgressInterval]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1 second.
  /// Trigger: This callback will be triggered when the media player starts playing resources.
  /// Restrictions: None.
  /// Related APIs: [setProgressInterval].
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [millisecond] Progress in milliseconds.
  static void Function(ZegoMediaPlayer mediaPlayer, int millisecond)?
      onMediaPlayerRenderingProgress;

  /// The callback triggered when when the resolution of the playback video changes.
  ///
  /// Available since: 3.11.0
  /// Description: The callback triggered when when the resolution of the playback video changes.
  /// Trigger: When the media player is playing a video resource, This callback will be triggered when playback starts and the resolution of the video changes.
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [width] width.
  /// - [height] height.
  static void Function(ZegoMediaPlayer mediaPlayer, int width, int height)?
      onMediaPlayerVideoSizeChanged;

  /// The callback triggered when the media player got media side info.
  ///
  /// Available since: 2.2.0
  /// Description: The callback triggered when the media player got media side info.
  /// Trigger: When the media player starts playing media files, the callback is triggered if the SEI is resolved to the media file.
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [data] SEI content.
  static void Function(ZegoMediaPlayer mediaPlayer, Uint8List data)?
      onMediaPlayerRecvSEI;

  /// The callback of sound level update.
  ///
  /// Available since: 2.15.0
  /// Description: The callback of sound level update.
  /// Trigger: The callback frequency is specified by [EnableSoundLevelMonitor].
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Related APIs: To monitor this callback, you need to enable it through [EnableSoundLevelMonitor].
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [soundLevel] Sound level value, value range: [0.0, 100.0] (This value only represents the range of the sound level value returned by the callback, not the precision.).
  static void Function(ZegoMediaPlayer mediaPlayer, double soundLevel)?
      onMediaPlayerSoundLevelUpdate;

  /// The callback of frequency spectrum update.
  ///
  /// Available since: 2.15.0
  /// Description: The callback of frequency spectrum update.
  /// Trigger: The callback frequency is specified by [EnableFrequencySpectrumMonitor].
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Related APIs: To monitor this callback, you need to enable it through [EnableFrequencySpectrumMonitor].
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [spectrumList] Locally captured frequency spectrum value list. Spectrum value range is [0-2^30].
  static void Function(ZegoMediaPlayer mediaPlayer, List<double> spectrumList)?
      onMediaPlayerFrequencySpectrumUpdate;

  /// The callback triggered when the media player plays the first frame.
  ///
  /// Available since: 3.5.0
  /// Description: The callback triggered when the media player plays the first frame.
  /// Trigger: This callback is generated when the media player starts playing.
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Related APIs: You need to call the [setPlayerCanvas] interface to set the view for the media player in order to receive the video first frame event callback.
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [event] The first frame callback event type.
  static void Function(
          ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerFirstFrameEvent event)?
      onMediaPlayerFirstFrameEvent;

  /// The callback triggered when the media player caches http/https network resource locally.
  ///
  /// Available since: 3.12.0
  /// Description: The callback triggered when the media player caches http/https network resource locally.
  /// Trigger: This callback occurs after the media player caches http/https network resources.
  /// Caution: This callback will not take effect until the [enableLocalCache] API is called and the http/https network resources are played.
  /// Related APIs: Need to call the [enableLocalCache] interface.
  ///
  /// - [mediaPlayer] Callback player object.
  /// - [errorCode] Error code.
  /// - [resource] Played resource
  /// - [cachedFile] Cached file
  static void Function(ZegoMediaPlayer mediaPlayer, int errorCode,
      String resource, String cachedFile)? onMediaPlayerLocalCache;

  /// Audio effect playback state callback.
  ///
  /// Available since: 1.16.0
  /// Description: This callback is triggered when the playback state of a audio effect of the audio effect player changes.
  /// Trigger: This callback is triggered when the playback status of the audio effect changes.
  /// Restrictions: None.
  ///
  /// - [audioEffectPlayer] Audio effect player instance that triggers this callback.
  /// - [audioEffectID] The ID of the audio effect resource that triggered this callback.
  /// - [state] The playback state of the audio effect.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(
      ZegoAudioEffectPlayer audioEffectPlayer,
      int audioEffectID,
      ZegoAudioEffectPlayState state,
      int errorCode)? onAudioEffectPlayStateUpdate;

  /// The event callback of the media data publisher opening a media file.
  ///
  /// Available since: 2.17.0
  /// Description: The event callback of the media data publisher opening a media file.
  /// Trigger: The callback triggered when the media data publisher start loading a media file.
  /// Restrictions: None.
  ///
  /// - [mediaDataPublisher] Callback publisher object
  /// - [path] Path of currently open file
  static void Function(ZegoMediaDataPublisher mediaDataPublisher, String path)?
      onMediaDataPublisherFileOpen;

  /// The event callback of the media data publisher closing a media file.
  ///
  /// Available since: 2.17.0
  /// Description: The event callback of the media data publisher closing a media file.
  /// Trigger: The callback triggered when the media data publisher start unloading a media file.
  /// Restrictions: None.
  ///
  /// - [mediaDataPublisher] Callback publisher object
  /// - [errorCode] error code. 0 means closing the file normally. -1 is a file error. -2 is a path exception. -3 is a decoding exception. -4 is an incorrect timestamp. -5 Unable to fix audio/video synchronization. -6 Unsupported audio sample rate. Contact technical support for details.
  /// - [path] Path of currently open file
  static void Function(ZegoMediaDataPublisher mediaDataPublisher, int errorCode,
      String path)? onMediaDataPublisherFileClose;

  /// The event callback that the media data publisher has read data from the media file.
  ///
  /// Available since: 2.17.0
  /// Description: The event callback that the media data publisher has read data from the media file.
  /// Trigger: The callback triggered when the media data publisher begin to read media data from a media file.
  /// Restrictions: None.
  ///
  /// - [mediaDataPublisher] Callback publisher object
  /// - [path] Path of currently open file
  static void Function(ZegoMediaDataPublisher mediaDataPublisher, String path)?
      onMediaDataPublisherFileDataBegin;

  /// The callback triggered when the state of data recording (to a file) changes.
  ///
  /// Available since: 1.10.0
  /// Description: The callback triggered when the state of data recording (to a file) changes.
  /// Use cases: The developer should use this callback to determine the status of the file recording or for UI prompting.
  /// When to trigger: After [startRecordingCapturedData] is called, if the state of the recording process changes, this callback will be triggered.
  /// Restrictions: None.
  ///
  /// - [state] File recording status.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [config] Record config.
  /// - [channel] Publishing stream channel.
  static void Function(
      ZegoDataRecordState state,
      int errorCode,
      ZegoDataRecordConfig config,
      ZegoPublishChannel channel)? onCapturedDataRecordStateUpdate;

  /// The callback to report the current recording progress.
  ///
  /// Available since: 1.10.0
  /// Description: Recording progress update callback, triggered at regular intervals during recording.
  /// Use cases: Developers can do UI hints for the user interface.
  /// When to trigger: After [startRecordingCapturedData] is called, If configured to require a callback, timed trigger during recording.
  /// Restrictions: None.
  ///
  /// - [progress] File recording progress, which allows developers to hint at the UI, etc.
  /// - [config] Record config.
  /// - [channel] Publishing stream channel.
  static void Function(
      ZegoDataRecordProgress progress,
      ZegoDataRecordConfig config,
      ZegoPublishChannel channel)? onCapturedDataRecordProgressUpdate;

  /// System performance monitoring callback.
  ///
  /// Available since: 1.19.0
  /// Description: System performance monitoring callback. The callback notification period is the value of millisecond parameter set by call [startPerformanceMonitor].
  /// Use cases: Monitor system performance can help user quickly locate and solve performance problems and improve user experience.
  /// When to trigger: It will triggered after [createEngine], and call [startPerformanceMonitor] to start system performance monitoring.
  /// Restrictions: None.
  ///
  /// - [status] System performance monitoring status.
  static void Function(ZegoPerformanceStatus status)? onPerformanceStatusUpdate;

  /// Network mode changed callback.
  ///
  /// Available since: 1.20.0
  /// Description: Network mode changed callback.
  /// When to trigger: This callback will be triggered when the device's network mode changed, such as switched from WiFi to 5G, or when network is disconnected.
  /// Restrictions: None.
  ///
  /// - [mode] Current network mode.
  static void Function(ZegoNetworkMode mode)? onNetworkModeChanged;

  /// Network speed test error callback.
  ///
  /// Available since: 1.20.0
  /// Description: Network speed test error callback.
  /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
  /// When to Trigger: If an error occurs during the speed test, such as: can not connect to speed test server, this callback will be triggered.
  /// Restrictions: None.
  ///
  /// - [errorCode] Network speed test error code. Please refer to error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [type] Uplink or downlink.
  static void Function(int errorCode, ZegoNetworkSpeedTestType type)?
      onNetworkSpeedTestError;

  /// Network speed test quality callback.
  ///
  /// Available since: 1.20.0
  /// Description: Network speed test quality callback when the network can be connected.
  /// Use cases: This function can be used to detect whether the network environment is suitable for pushing/pulling streams with specified bitrates.
  /// When to Trigger: After call [startNetworkSpeedTest] start network speed test, this callback will be triggered. The trigger period is determined by the parameter value specified by call [startNetworkSpeedTest], default value is 3 seconds
  /// Restrictions: None.
  /// Caution: When error occurred during network speed test or [stopNetworkSpeedTest] called, this callback will not be triggered.
  ///
  /// - [quality] Network speed test quality.
  /// - [type] Uplink or downlink.
  static void Function(
          ZegoNetworkSpeedTestQuality quality, ZegoNetworkSpeedTestType type)?
      onNetworkSpeedTestQualityUpdate;

  /// The network quality callback of users who are publishing in the room.
  ///
  /// Available since: 2.10.0
  /// Description: The uplink and downlink network callbacks of the local and remote users, that would be called by default every two seconds for the local and each playing remote user's network quality.
  ///   Versions 2.10.0 to 2.13.1:
  ///   1. Developer must both publish and play streams before you receive your own network quality callback.
  ///   2. When playing a stream, the publish end has a play stream and the publish end is in the room where it is located, then the user's network quality will be received.
  ///   Versions 2.14.0 to 2.21.1:
  ///   1. As long as you publish or play a stream, you will receive your own network quality callback.
  ///   2. When you play a stream, the publish end is in the room where you are, and you will receive the user's network quality.
  ///   Version 2.22.0 and above:
  ///   1. Estimate the network conditions of the remote stream publishing user. If the remote stream publishing user loses one heartbeat, the network quality will be called back as unknown; if the remote stream publishing user's heartbeat loss reaches 3 Second, call back its network quality to die.
  /// Use case: When the developer wants to analyze the network condition on the link, or wants to know the network condition of local and remote users.
  /// When to Trigger: After publishing a stream by called [startPublishingStream] or playing a stream by called [startPlayingStream].
  ///
  /// - [userID] User ID, empty means local user
  /// - [upstreamQuality] Upstream network quality
  /// - [downstreamQuality] Downstream network quality
  static void Function(String userID, ZegoStreamQualityLevel upstreamQuality,
      ZegoStreamQualityLevel downstreamQuality)? onNetworkQuality;

  /// Successful callback of network time synchronization.
  ///
  /// Available since: 2.12.0
  /// This callback is triggered when internal network time synchronization completes after a developer calls [createEngine].
  static void Function()? onNetworkTimeSynchronized;

  /// Request to dump data.
  ///
  /// Available since: 3.10.0
  /// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
  static void Function()? onRequestDumpData;

  /// Request to dump data.
  ///
  /// Available since: 3.11.0
  /// When to Trigger: When the customer reports back the problem, ZEGO expects the user to dump the data to analyze the audio / video processing problem, which will trigger this callback.
  ///
  /// - [dumpDir] Dump data dir.
  /// - [takePhoto] Need to take photo when uploading dump data
  static void Function(String dumpDir, bool takePhoto)? onRequestUploadDumpData;

  /// Callback when starting to dump data.
  ///
  /// Available since: 3.10.0
  /// When to Trigger: This callback is triggered when [startDumpData] is called.
  ///
  /// - [errorCode] Error code.
  static void Function(int errorCode)? onStartDumpData;

  /// Callback when stopping to dump data.
  ///
  /// Available since: 3.10.0
  /// When to Trigger: This callback is triggered when [stopDumpData] is called.
  ///
  /// - [errorCode] Error code.
  /// - [dumpDir] Dump data dir.
  static void Function(int errorCode, String dumpDir)? onStopDumpData;

  /// Callback after uploading the dump data.
  ///
  /// Available since: 3.10.0
  /// When to Trigger: When [uploadDumpData] is called, this callback will be triggered after SDK executes the upload task.
  ///
  /// - [errorCode] Error code.
  static void Function(int errorCode)? onUploadDumpData;

  /// Custom audio processing local captured PCM audio frame callback.
  ///
  /// Available: Since 2.13.0
  /// Description: In this callback, you can receive PCM audio frames collected locally by custom audio processing. The returned timestamp can be used for data synchronization, such as lyrics, etc. The returned data cannot be modified. If you need the data after used headphone monitor, please use the [onProcessCapturedAudioDataAfterUsedHeadphoneMonitor] callback.
  /// When to trigger: You need to call [enableCustomAudioCaptureProcessing] to enable the function first, and call [startPreivew] or [startPublishingStream] to trigger this callback function.
  /// Restrictions: None.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  /// - [timestamp] The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
  static void Function(Uint8List data, int dataLength,
      ZegoAudioFrameParam param, double timestamp)? onProcessCapturedAudioData;

  /// Custom audio processing local captured PCM audio frame callback after used headphone monitor.
  ///
  /// Available: Since 2.13.0
  /// Description: In this callback, you can receive the PCM audio frame after the custom audio processing is locally collected and returned to the ear. The returned timestamp can be used for data synchronization, such as lyrics, etc. The returned data cannot be changed.
  /// When to trigger: You need to call [enableCustomAudioCaptureProcessingAfterHeadphoneMonitor] to enable the function first, and call [startPreivew] or [startPublishingStream] to trigger this callback function.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format
  /// - [dataLength] Length of the data
  /// - [param] Parameters of the audio frame
  /// - [timestamp] The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
  static void Function(
      Uint8List data,
      int dataLength,
      ZegoAudioFrameParam param,
      double timestamp)? onProcessCapturedAudioDataAfterUsedHeadphoneMonitor;

  /// Aligned audio aux frames callback.
  ///
  /// Available: Since 2.22.0
  /// Description: In this callback, you can receive the audio aux frames which aligned with accompany. Developers can record locally.
  /// When to trigger: This callback function will not be triggered until [enableAlignedAudioAuxData] is called to turn on the function and [startpublishingstream] or [startrecordingcaptureddata] is called.
  /// Restrictions: To obtain audio aux data of the media player from this callback, developers need to call [enableAux] and [start] of MediaPlayer.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback, and the data in this callback cannot be modified.
  ///
  /// - [data] Audio data in PCM format.
  /// - [param] Parameters of the audio frame.
  static void Function(Uint8List data, ZegoAudioFrameParam param)?
      onAlignedAudioAuxData;

  /// Custom audio processing remote playing stream PCM audio frame callback.
  ///
  /// Available: Since 2.13.0
  /// Description: In this callback, you can receive custom audio processing remote streaming PCM audio frames, and the returned timestamp can be used for data synchronization, such as lyrics, etc. The returned data cannot be changed.
  /// When to trigger: You need to call [enableCustomAudioRemoteProcessing] to enable the function first, and call [startPlayingStream] to trigger this callback function.
  /// Restrictions: None.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  /// - [streamID] Corresponding stream ID.
  /// - [timestamp] The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds.
  static void Function(
      Uint8List data,
      int dataLength,
      ZegoAudioFrameParam param,
      String streamID,
      double timestamp)? onProcessRemoteAudioData;

  /// Custom audio processing SDK playback PCM audio frame callback.
  ///
  /// Available: Since 2.13.0
  /// Description: In this callback, you can receive the PCM audio frame of the audio played by the custom audio processing SDK, and the returned timestamp can be used for data synchronization, such as lyrics, etc. The returned data cannot be changed.
  /// When to trigger: You need to call [enableCustomAudioPlaybackProcessing] to enable the function first, and call [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] or [createAudioEffectPlayer] to trigger this callback function.
  /// Restrictions: None.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  /// - [timestamp] The audio frame timestamp, starting from 0 when capture is started, the unit is milliseconds (It is effective when there is one and only one stream).
  static void Function(Uint8List data, int dataLength,
      ZegoAudioFrameParam param, double timestamp)? onProcessPlaybackAudioData;

  /// The callback for obtaining the audio data captured by the local microphone.
  ///
  /// Available: Since 1.1.0
  /// Description: In non-custom audio capture mode, the SDK capture the microphone's sound, but the developer may also need to get a copy of the audio data captured by the SDK is available through this callback.
  /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0b01 that means 1 << 0, this callback will be triggered only when it is in the publishing stream state.
  /// Restrictions: None.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  static void Function(
          Uint8List data, int dataLength, ZegoAudioFrameParam param)?
      onCapturedAudioData;

  /// The callback for obtaining the audio data of all the streams playback by SDK.
  ///
  /// Available: Since 1.1.0
  /// Description: This function will callback all the mixed audio data to be playback. This callback can be used for that you needs to fetch all the mixed audio data to be playback to proccess.
  /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0b10 that means 1 << 1, this callback will be triggered only when it is in the SDK inner audio and video engine started(called the [startPreivew] or [startPlayingStream] or [startPublishingStream]).
  /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
  /// Caution: This callback is a high-frequency callback. Please do not perform time-consuming operations in this callback. When the engine is not in the stream publishing state and the media player is not used to play media files, the audio data in the callback is muted audio data.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  static void Function(
          Uint8List data, int dataLength, ZegoAudioFrameParam param)?
      onPlaybackAudioData;

  /// Callback to get the audio data played by the SDK and the audio data captured by the local microphone. The audio data is the data mixed by the SDK.
  ///
  /// Available: Since 1.1.0
  /// Description: The audio data played by the SDK is mixed with the data captured by the local microphone before being sent to the speaker, and is called back through this function.
  /// When to trigger: On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [startAudioDataObserver] to set the mask 0x04, this callback will be triggered only when it is in the publishing stream state or playing stream state.
  /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  static void Function(
          Uint8List data, int dataLength, ZegoAudioFrameParam param)?
      onMixedAudioData;

  /// The callback for obtaining the audio data of each stream.
  ///
  /// Available: Since 1.1.0
  /// Description: This function will call back the data corresponding to each playing stream. Different from [onPlaybackAudioData], the latter is the mixed data of all playing streams. If developers need to process a piece of data separately, they can use this callback.
  /// When to trigger: On the premise of calling [setAudioDataHandler] to set up listening for this callback, calling [startAudioDataObserver] to set the mask 0x08 that is 1 << 3, and this callback will be triggered when the SDK audio and video engine starts to play the stream.
  /// Restrictions: None.
  /// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
  ///
  /// - [data] Audio data in PCM format.
  /// - [dataLength] Length of the data.
  /// - [param] Parameters of the audio frame.
  /// - [streamID] Corresponding stream ID.
  static void Function(Uint8List data, int dataLength,
      ZegoAudioFrameParam param, String streamID)? onPlayerAudioData;

  /// Range audio microphone state callback.
  ///
  /// Available since: 2.11.0
  /// Description: The status change notification of the microphone, starting to send audio is an asynchronous process, and the state switching in the middle is called back through this function.
  /// When to Trigger: After the [enableMicrophone] function.
  /// Caution: It must be monitored before the [enableMicrophone] function is called.
  ///
  /// - [rangeAudio] Range audio instance that triggers this callback.
  /// - [state] The use state of the range audio.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(
      ZegoRangeAudio rangeAudio,
      ZegoRangeAudioMicrophoneState state,
      int errorCode)? onRangeAudioMicrophoneStateUpdate;

  /// Callback for download song or accompaniment progress rate.
  ///
  /// - [copyrightedMusic] Copyrighted music instance that triggers this callback.
  /// - [resourceID] The resource ID of the song or accompaniment that triggered this callback.
  /// - [progressRate] download progress rate.
  static void Function(ZegoCopyrightedMusic copyrightedMusic, String resourceID,
      double progressRate)? onDownloadProgressUpdate;

  /// Real-time pitch line callback.
  ///
  /// - [copyrightedMusic] Copyrighted music instance that triggers this callback.
  /// - [resourceID] The resource ID of the song or accompaniment that triggered this callback.
  /// - [currentDuration] Current playback progress.
  /// - [pitchValue] Real-time pitch accuracy or value.
  static void Function(ZegoCopyrightedMusic copyrightedMusic, String resourceID,
      int currentDuration, int pitchValue)? onCurrentPitchValueUpdate;

  /// The callback triggered when a screen capture source exception occurred
  ///
  /// Available since: 3.1.0
  /// Description: The callback triggered when a screen capture source exception occurred.
  /// Trigger: This callback is triggered when an exception occurs after the screen start capture.
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [source] Callback screen capture source object.
  /// - [exceptionType] Capture source exception type.
  static void Function(ZegoScreenCaptureSource source,
      ZegoScreenCaptureSourceExceptionType exceptionType)? onExceptionOccurred;

  /// The callback will be triggered when the state of the capture target window change.
  ///
  /// Available since: 3.4.0
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [source] Callback screen capture source object.
  /// - [windowState] Capture window state.
  /// - [windowRect] Capture window rect.
  static void Function(
      ZegoScreenCaptureSource source,
      ZegoScreenCaptureWindowState windowState,
      Rect windowRect)? onWindowStateChanged;

  /// The callback will be triggered when the state of the capture target window change.
  ///
  /// Available since: 3.7.0
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Restrictions: Only available on Windows/macOS.
  ///
  /// - [source] Callback screen capture source object.
  /// - [captureRect] Capture source rect.
  static void Function(ZegoScreenCaptureSource source, Rect captureRect)?
      onRectChanged;

  /// The callback triggered when a screen capture source exception occurred
  ///
  /// Available since: 3.6.0
  /// Description: The callback triggered when the mobile screen capture source exception occurred.
  /// Trigger: This callback is triggered when an exception occurs after the mobile screen capture started.
  /// Caution: The callback does not actually take effect until call [setEventHandler] to set.
  /// Restrictions: Only available on Android.
  ///
  /// - [exceptionType] Screen capture exception type.
  static void Function(ZegoScreenCaptureExceptionType exceptionType)?
      onMobileScreenCaptureExceptionOccurred;

  /// Initialize AI voice changer engine status callback.
  ///
  /// Available since: 3.10.0.
  /// Description: Initialize AI voice changer engine status callback.
  /// Trigger: The callback triggered when call [init] function.
  /// Restrictions: None.
  ///
  /// - [aiVoiceChanger] Callback AI voice changer instance.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(ZegoAIVoiceChanger aiVoiceChanger, int errorCode)?
      onAIVoiceChangerInit;

  /// Update AI voice changer engine models progress callback.
  ///
  /// Available since: 3.12.0.
  /// Description: Update AI voice changer engine models progress callback.
  /// Trigger: The callback triggered when call [update] function.
  /// Restrictions: None.
  ///
  /// - [aiVoiceChanger] Callback AI voice changer instance.
  /// - [percent] Current file update progress.
  /// - [fileIndex] Current update file index.
  /// - [fileCount] Total update file count.
  static void Function(ZegoAIVoiceChanger aiVoiceChanger, double percent,
      int fileIndex, int fileCount)? onAIVoiceChangerUpdateProgress;

  /// Update AI voice changer engine models status callback.
  ///
  /// Available since: 3.10.0.
  /// Description: Update AI voice changer engine models status callback.
  /// Trigger: The callback triggered when call [update] function.
  /// Restrictions: None.
  ///
  /// - [aiVoiceChanger] Callback AI voice changer instance.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  static void Function(ZegoAIVoiceChanger aiVoiceChanger, int errorCode)?
      onAIVoiceChangerUpdate;

  /// Get AI voice changer engine available speaker list callback.
  ///
  /// Available since: 3.10.0.
  /// Description: Get AI voice changer engine available speaker list callback.
  /// Trigger: The callback triggered when call [getSpeakerList] function.
  /// Restrictions: None.
  ///
  /// - [aiVoiceChanger] Callback AI voice changer instance.
  /// - [errorCode] Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
  /// - [speakerList] Available speaker list.
  static void Function(ZegoAIVoiceChanger aiVoiceChanger, int errorCode,
          List<ZegoAIVoiceChangerSpeakerInfo> speakerList)?
      onAIVoiceChangerGetSpeakerList;

  /// [Deprecated] Create ZegoExpressEngine singleton object and initialize SDK. Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead. Please refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) for more details.
  ///
  /// Available: 1.1.0 ~ 2.13.1, deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead
  /// Description: Create ZegoExpressEngine singleton object and initialize SDK.
  /// When to call: The engine needs to be created before calling other functions.
  /// Restrictions: None.
  /// Caution: The SDK only supports the creation of one instance of ZegoExpressEngine. If you need call [createEngine] multiple times, you need call [destroyEngine] before you call the next [createEngine]. Otherwise it will return the instance which created by [createEngine] you called last time.
  ///
  /// @deprecated Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead.
  /// - [appID] Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zegocloud.com The value ranges from 0 to 4294967295.
  /// - [appSign] Application signature for each AppID, please apply from the ZEGO Admin Console. Application signature is a 64 character string. Each character has a range of '0' ~ '9', 'a' ~ 'z'. AppSign 2.17.0 and later allows null or no transmission. If the token is passed empty or not passed, the token must be entered in the [ZegoRoomConfig] parameter for authentication when the [loginRoom] interface is called to login to the room.
  /// - [isTestEnv] [Deprecated] For providing better and more standardized services, starting from 2021-11-16, ZEGO no longer classifies environments into production environments and testing environments. f you create your project in ZEGO Admin Console on/before 2021-11-16, refer to [Testing environment deprecation](https://docs.zegocloud.com/article/13315) to upgrade the SDK and adjust related codes.
  /// - [scenario] The room scenario. the SDK will optimize the audio and video configuration for the specified scenario to achieve the best effect in this scenario. After specifying the scenario, you can call other APIs to adjusting the audio and video configuration. Differences between scenarios and how to choose a suitable scenario, please refer to https://docs.zegocloud.com/article/14940
  /// - [enablePlatformView] Set whether to use Platform View for rendering, true: rendering using Platform View, false: rendering using Texture, default is false. Currently the web platform only supports rendering with Platform View. When using the [createCanvasView] interface, If the preferred render mode is not supported, another render mode is automatically used.
  @Deprecated(
      'Deprecated since 2.14.0, please use the method with the same name without [isTestEnv] parameter instead.')
  static Future<void> createEngine(
      int appID, String appSign, bool isTestEnv, ZegoScenario scenario,
      {bool? enablePlatformView}) async {
    return await ZegoExpressImpl.createEngine(
        appID, appSign, isTestEnv, scenario,
        enablePlatformView: enablePlatformView);
  }
}

extension ZegoExpressEngineDeprecatedApi on ZegoExpressEngine {
  /// [Deprecated] Enables or disables the beauty features for the specified publish channel. Deprecated since 2.16.0, please use the [enableEffectsBeauty] function instead.
  ///
  /// Available since: 1.1.0
  /// Description: When developers do not have much need for beauty features, they can use this function to set some very simple beauty effects.
  /// When to call: It needs to be called after [createEngine].
  /// Default value: When this function is not called, the beauty feature is not enabled by default.
  /// Related APIs: After turning on the beauty features, you can call the [setBeautifyOption] function to adjust the beauty parameters.
  /// Caution: This beauty feature is very simple and may not meet the developer’s expectations. It is recommended to use the custom video processing function [enableCustomVideoProcessing] or the custom video capture function [enableCustomVideoCapture] to connect the AI Effects SDK [ZegoEffects] https://docs.zegocloud.com/article/9896 for best results.
  /// Restrictions: In the case of using the custom video capture function, since the developer has handle the video data capturing, the SDK is no longer responsible for the video data capturing, so this function is no longer valid. It is also invalid when using the custom video processing function.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// @deprecated Deprecated since 2.16.0, please use the [enableEffectsBeauty] function instead.
  /// - [featureBitmask] Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
  /// - [channel] Publishing stream channel
  @Deprecated(
      'Deprecated since 2.16.0, please use the [enableEffectsBeauty] function instead.')
  Future<void> enableBeautify(int featureBitmask,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .enableBeautify(featureBitmask, channel: channel);
  }

  /// [Deprecated] Set beautify option. Deprecated since 2.16.0, please use the [setEffectsBeautyParam] function instead.
  ///
  /// Available since: 1.1.0
  /// Description: set beautify option for main publish channel.
  /// Use cases: Often used in video call, live broadcasting.
  /// When to call: It needs to be called after [createEngine].
  /// Restrictions: None.
  /// Caution: In the case of using a custom video capture function, because the developer has taken over the video data capturing, the SDK is no longer responsible for the video data capturing, call this function will not take effect. When using custom video processing, the video data collected by the SDK will be handed over to the business for further processing, call this function will not take effect either.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// @deprecated Deprecated since 2.16.0, please use the [setEffectsBeautyParam] function instead.
  /// - [option] Beautify option.
  /// - [channel] stream publish channel.
  @Deprecated(
      'Deprecated since 2.16.0, please use the [setEffectsBeautyParam] function instead.')
  Future<void> setBeautifyOption(ZegoBeautifyOption option,
      {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance
        .setBeautifyOption(option, channel: channel);
  }
}
