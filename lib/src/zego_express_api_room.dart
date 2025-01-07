import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineRoom on ZegoExpressEngine {
  /// Log in to the room by configuring advanced properties, and return the login result through the callback parameter. You must log in to the room before pushing or pulling the stream.
  ///
  /// Available since: 2.18.0
  /// Description: If the room does not exist, [loginRoom] creates and logs in the room. SDK uses the 'room' to organize users. After users log in to a room, they can use interface such as push stream [startPublishingStream], pull stream [startPlayingStream], send and receive broadcast messages [sendBroadcastMessage], etc. To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
  /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
  /// When to call /Trigger: This interface is called after [createEngine] initializes the SDK.
  /// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
  /// Caution:
  ///   1. Apps that use different appIDs cannot intercommunication with each other.
  ///   2. SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
  ///   3. It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
  ///   4. After the first login failure due to network reasons or the room is disconnected, the default time of SDK reconnection is 20min.
  /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  /// Related callbacks:
  ///   1. When the user starts to log in to the room, the room is successfully logged in, or the room fails to log in, the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback will be triggered to notify the developer of the status of the current user connected to the room.
  ///   2. Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
  ///   3. If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
  ///   4. Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
  /// Related APIs:
  ///   1. Users can call [logoutRoom] to log out. In the case that a user has successfully logged in and has not logged out, if the login interface is called again, the console will report an error and print the error code 1002001.
  ///   2. SDK supports multi-room login, please call [setRoomMode] function to select multi-room mode before engine initialization, and then call [loginRoom] to log in to multi-room.
  ///   3. Calling [destroyEngine] will also automatically log out.
  ///
  /// - [roomID] Room ID, a string of less 128 bytes in length.
  ///   Caution:
  ///   1. room ID is defined by yourself.
  ///   2. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
  ///   3. If you need to communicate with the Web SDK, please do not use '%'.
  /// - [user] User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
  /// - [config] Advanced room configuration.
  /// - Returns The result of this login room
  Future<ZegoRoomLoginResult> loginRoom(String roomID, ZegoUser user,
      {ZegoRoomConfig? config}) async {
    return await ZegoExpressImpl.instance
        .loginRoom(roomID, user, config: config);
  }

  /// Exit the room of the specified room ID with callback.
  ///
  /// Available since: 1.1.0
  /// Description: This API will log out the room named roomID.
  /// Use cases: In the same room, users can conduct live broadcast, audio and video calls, etc.
  /// When to call /Trigger: After successfully logging in to the room, if the room is no longer used, the user can call the function [logoutRoom].
  /// Restrictions: None.
  /// Caution: 1. Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method. 2. If the user logs out to the room, but the incoming 'roomID' is different from the logged-in room name, SDK will return failure.
  /// Related callbacks: After calling this function, you will receive [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).
  /// Related APIs: Users can use [loginRoom], [switchRoom] functions to log in or switch rooms.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length.
  ///   Caution:
  ///   1. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '\'.
  ///   2. If you need to communicate with the Web SDK, please do not use '%'.
  /// - Returns The result of this logout room
  Future<ZegoRoomLogoutResult> logoutRoom([String? roomID]) async {
    return await ZegoExpressImpl.instance.logoutRoom(roomID);
  }

  /// Switch the room with advanced room configurations.
  ///
  /// Available since: 1.15.0
  /// Description: Using this interface allows users to quickly switch from one room to another room.
  /// Use cases: if you need to quickly switch to the next room, you can call this function.
  /// When to call /Trigger: After successfully login room.
  /// Restrictions: None.
  /// Caution:
  ///   1. When this function is called, all streams currently publishing or playing will stop (but the local preview will not stop).
  ///   2. To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter. This parameter configuration affects the room to be switched over.
  ///   3. Multi-room mode is supported in version 3.5.0 (use the function [setRoomMode] to set ZegoRoomMode to ZEGO_ROOM_MODE_MULTI_ROOM).
  ///   4. If a Token is passed in for login when logging into the room [loginRoom], the [switchroom] interface must be used with the config parameter and the corresponding Token value passed in when switching rooms.
  /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  /// Related callbacks: When the user call the [switchRoom] function, the [onRoomStateChanged] (Not supported before 2.18.0, please use [onRoomStateUpdate]) callback will be triggered to notify the developer of the status of the current user connected to the room.
  /// Related APIs: Users can use the [logoutRoom] function to log out of the room.
  ///
  /// - [fromRoomID] Current roomID.
  /// - [toRoomID] The next roomID.
  /// - [config] Advanced room configuration.
  Future<void> switchRoom(String fromRoomID, String toRoomID,
      {ZegoRoomConfig? config}) async {
    return await ZegoExpressImpl.instance
        .switchRoom(fromRoomID, toRoomID, config: config);
  }

  /// Renew token.
  ///
  /// Available since: 2.8.0
  /// Description: After the developer receives [onRoomTokenWillExpire], they can use this API to update the token to ensure that the subsequent RTC functions are normal.
  /// Use cases: Used when the token is about to expire.
  /// When to call /Trigger: After the developer receives [onRoomTokenWillExpire].
  /// Restrictions: None.
  /// Caution: The token contains important information such as the user's room permissions, publish stream permissions, and effective time, please refer to https://docs.zegocloud.com/article/11649.
  /// Related callbacks: None.
  /// Related APIs: None.
  ///
  /// - [roomID] Room ID.
  /// - [token] The token that needs to be renew.
  Future<void> renewToken(String roomID, String token) async {
    return await ZegoExpressImpl.instance.renewToken(roomID, token);
  }

  /// Set room extra information.
  ///
  /// Available since: 1.13.0
  /// Description: The user can call this function to set the extra info of the room.
  /// Use cases: You can set some room-related business attributes, such as whether someone is Co-hosting.
  /// When to call /Trigger: After logging in the room successful.
  /// Restrictions: For restrictions on the use of this function, please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
  /// Caution: 'key' is non null. The length of key and value is limited, please refer to Restrictions. The newly set key and value will overwrite the old setting.
  /// Related callbacks: Other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback function.
  /// Related APIs: None.
  ///
  /// - [roomID] Room ID.
  /// - [key] key of the extra info.
  /// - [value] value if the extra info.
  /// - Returns Set room extra info execution result notification
  Future<ZegoRoomSetRoomExtraInfoResult> setRoomExtraInfo(
      String roomID, String key, String value) async {
    return await ZegoExpressImpl.instance.setRoomExtraInfo(roomID, key, value);
  }

  /// Get room stream list.
  ///
  /// Available since: 3.12.0
  /// Description: Get room stream list.
  /// Use cases: Get room stream list.
  /// When to call /Trigger: After logging in the room successful.
  /// Caution: This interface is to get a real-time internal stream list, which may be inaccurate when the room is disconnected from the service. Do not call this interface with high frequency.
  /// Related APIs: None.
  ///
  /// - [roomID] Room ID.
  /// - [streamListType] Get type
  /// - Returns return stream list
  Future<ZegoRoomStreamList> getRoomStreamList(
      String roomID, ZegoRoomStreamListType streamListType) async {
    return await ZegoExpressImpl.instance
        .getRoomStreamList(roomID, streamListType);
  }
}
