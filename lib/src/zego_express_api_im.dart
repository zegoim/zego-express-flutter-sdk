
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineIM on ZegoExpressEngine {

  /// Sends a Broadcast Message.
  ///
  /// The sending frequency of broadcast messages in the same room cannot be higher than 10 messages/s.
  /// A certain number of users in the same room who entered the room earlier can receive this callback. The message is reliable. It is generally used when the number of people in the live room is less than a certain number. The specific number is determined by the configuration of the ZEGO server.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 1024 bytes
  /// - Returns Send broadcast message result callback
  Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBroadcastMessage(roomID, message);
  }

  /// Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
  ///
  /// The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s.
  /// The message is unreliable. When the frequency of sending barrage messages in the entire room is greater than 20 messages/s, the recipient may not receive the message. It is generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live broadcast barrage.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [message] Message content, no longer than 1024 bytes
  /// - Returns Send barrage message result callback
  Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBarrageMessage(roomID, message);
  }

  /// Sends a Custom Command to the specified users in the same room.
  ///
  /// The frequency of custom messages sent to a single user in the same room cannot be higher than 200 messages/s, and the frequency of custom messages sent to multiple users cannot be higher than 10 messages/s.
  /// The point-to-point signaling type in the same room is generally used for remote control signaling or for sending messages between users. The messages are reliable.
  ///
  /// - [roomID] Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
  /// - [command] Custom command content, no longer than 1024 bytes
  /// - [toUserList] The users who will receive the command
  /// - Returns Send command result callback
  Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {
    return await ZegoExpressImpl.instance.sendCustomCommand(roomID, command, toUserList);
  }

}
