
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineRoom on ZegoExpressEngine {

  /// Logs in to a room with advanced room configurations. You must log in to a room before publishing or playing streams.
  ///
  /// To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
  /// Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
  /// Messages sent in one room (eg apis [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
  /// SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
  /// If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
  /// It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [user] User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
  /// - [config] Advanced room configuration
  Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config}) async {
    return await ZegoExpressImpl.instance.loginRoom(roomID, user, config: config);
  }

  /// Logs in multi room
  ///
  /// You must log in the main room with [loginRoom] before invoke this function to logging in to multi room.
  /// Currently supports logging into 1 main room and 1 multi room at the same time.
  /// When logging out, you must log out of the multi room before logging out of the main room.
  /// User can only publish the stream in the main room, but can play the stream in the main room and multi room at the same time, and can receive the signaling and callback in each room.
  /// The advantage of multi room is that you can login another room without leaving the current room, receive signaling and callback from another room, and play streams from another room.
  /// To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
  /// Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
  /// Messages sent in one room (eg apis [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
  /// SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
  /// If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
  /// It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [config] Advanced room configuration
  Future<void> loginMultiRoom(String roomID, {ZegoRoomConfig config}) async {
    return await ZegoExpressImpl.instance.loginMultiRoom(roomID, config: config);
  }

  /// Logs out of a room.
  ///
  /// Exiting the room will stop all publishing and playing streams for user, and then SDK will auto stop local preview UI. After calling this function, you will receive [onRoomStateUpdate] callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  Future<void> logoutRoom(String roomID) async {
    return await ZegoExpressImpl.instance.logoutRoom(roomID);
  }

  /// Switch the room with advanced room configurations
  ///
  /// After successfully login room, if you need to quickly switch to the next room, you can call this function.
  /// Calling this function is faster and easier to use than calling [logoutRoom] and then [loginRoom].
  /// When this function is called, all streams currently publishing or playing will stop (but the local preview will not stop).
  /// To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter. This parameter configuration affects the room to be switched over.
  ///
  /// - [fromRoomID] Current roomID
  /// - [toRoomID] The next roomID
  /// - [config] Advanced room configuration
  Future<void> switchRoom(String fromRoomID, String toRoomID, {ZegoRoomConfig config}) async {
    return await ZegoExpressImpl.instance.switchRoom(fromRoomID, toRoomID, config: config);
  }

  /// Set room extra information
  ///
  /// After the user in the room calls this api to set the extra info of the room, other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback api
  ///
  /// - [roomID] Room ID.
  /// - [key] key of the extra info.
  /// - [value] value if the extra info.
  /// - Returns Set room extra info execution result notification
  Future<ZegoRoomSetRoomExtraInfoResult> setRoomExtraInfo(String roomID, String key, String value) async {
    return await ZegoExpressImpl.instance.setRoomExtraInfo(roomID, key, value);
  }

}
