
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineIM on ZegoExpressEngine {

  /// Send room broadcast message
  ///
  /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
  /// Users of up to the first 500 advanced rooms in the same room can receive it, which is generally used when the number of live broadcast rooms is less than 500.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 256 bytes
  /// - Returns Send broadcast message result callback
  Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBroadcastMessage(roomID, message);
  }

  /// Send room barrage message
  ///
  /// There is no limit on the number of transmissions, but the server will actively drop messages if it is sent too frequently.
  /// It can be received by users with more than 500 people in the same room, but it is not reliable, that is, when there are many users in the room or messages are sent frequently between users, the users who receive the messages may not be able to receive them. Generally used for sending live barrage.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 256 bytes
  /// - Returns Send barrage message result callback
  Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBarrageMessage(roomID, message);
  }

  /// Send custom command
  ///
  /// The total sending frequency limit of [sendBroadcastMessage] and [sendCustomCommand] is 600 times per minute by default.
  /// The type of point-to-point signaling in the same room is generally used for remote control signaling or message sending between users.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [command] Custom command content, no longer than 256 bytes
  /// - [toUserList] The users who will receive the command
  /// - Returns Send command result callback
  Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {
    return await ZegoExpressImpl.instance.sendCustomCommand(roomID, command, toUserList);
  }

}
