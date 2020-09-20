
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineIM on ZegoExpressEngine {

  /// Sends a Broadcast Message
  ///
  /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
  /// Users of a certain number of advanced rooms in the same room can receive this callback. It is generally used when the number of people in the live room is less than a certain number. The specific number is determined by the configuration of the ZEGO server.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 1024 bytes
  /// - Returns Send broadcast message result callback
  Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBroadcastMessage(roomID, message);
  }

  /// Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
  ///
  /// There is no limit on the number of transmissions, but the server will actively drop messages if it is sent too frequently.
  /// The api [sendBroadcastMessage] only supports that a certain number of users who entered the room can receive the sent message，but [sendBarrageMessage] can be received by users with more than the number of people in the same room, but it is not reliable, that is, when there are many users in the room or messages are sent frequently between users, the users who receive the messages may not be able to receive them. Generally used for sending live barrage.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 1024 bytes
  /// - Returns Send barrage message result callback
  Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBarrageMessage(roomID, message);
  }

  /// Sends a Custom Command to the specified users in the same room.
  ///
  /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
  /// The type of point-to-point signaling in the same room is generally used for remote control signaling or message sending between users.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [command] Custom command content, no longer than 1024 bytes
  /// - [toUserList] The users who will receive the command
  /// - Returns Send command result callback
  Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {
    return await ZegoExpressImpl.instance.sendCustomCommand(roomID, command, toUserList);
  }

}
