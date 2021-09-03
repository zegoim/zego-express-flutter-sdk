
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineIM on ZegoExpressEngine {

  /// Sends a Broadcast Message.
  ///
  /// Available since: 1.2.1
  /// Description: Send a broadcast message to the room, users who have entered the same room can receive the message, and the message is reliable.
  /// Use cases: Generally used when the number of people in the live room does not exceed 500.
  /// When to call: After calling [loginRoom] to log in to the room.
  /// Restrictions: It is not supported when the number of people online in the room exceeds 500. If you need to increase the limit, please contact ZEGO technical support to apply for evaluation. The frequency of sending broadcast messages in the same room cannot be higher than 10 messages/s. For restrictions on the use of this function, please refer to https://doc-zh.zego.im/article/7581 or contact ZEGO technical support.
  /// Related callbacks: The room broadcast message can be received through [onIMRecvBroadcastMessage].
  /// Related APIs: Barrage messages can be sent through the [sendBarrageMessage] function, and custom command can be sent through the [sendCustomCommand] function.
  ///
  /// - [roomID] Room ID. Required: Yes. Value range: The maximum length is 128 bytes. Caution: The room ID is in string format and only supports numbers, English characters and'~','!','@','#','$','%','^','&', ' *','(',')','_','+','=','-','`',';',''',',','.','<' ,'>','/','\'.
  /// - [message] The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
  /// - Returns Send broadcast message result callback
  Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBroadcastMessage(roomID, message);
  }

  /// Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
  ///
  /// Available since: 1.5.0
  /// Description: Send a barrage message to the room, users who have logged in to the same room can receive the message, the message is unreliable.
  /// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
  /// When to call: After calling [loginRoom] to log in to the room.
  /// Restrictions: The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s. For restrictions on the use of this function, please refer to https://doc-zh.zego.im/article/7581 or contact ZEGO technical support.
  /// Related callbacks: The room barrage message can be received through [onIMRecvBarrageMessage].
  /// Related APIs: Broadcast messages can be sent through the [sendBroadcastMessage] function, and custom command can be sent through the [sendCustomCommand] function.
  ///
  /// - [roomID] Room ID. Required: Yes. Value range: The maximum length is 128 bytes. Caution: The room ID is in string format and only supports numbers, English characters and'~','!','@','#','$','%','^','&', ' *','(',')','_','+','=','-','`',';',''',',','.','<' ,'>','/','\'.
  /// - [message] The content of the message. Required: Yes. Value range: The length does not exceed 1024 bytes.
  /// - Returns Send barrage message result callback.
  Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
    return await ZegoExpressImpl.instance.sendBarrageMessage(roomID, message);
  }

  /// Sends a Custom Command to the specified users in the same room.
  ///
  /// Available since: 1.2.1
  /// Description: After calling this function, users in the same room who have entered the room can receive the message, the message is unreliable.
  /// Use cases: Generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live barrage.
  /// When to call: After calling [loginRoom] to log in to the room.
  /// Restrictions: Generally used when the number of people in the live room does not exceed 500.The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s. For restrictions on the use of this function, please refer to https://doc-zh.zego.im/article/7581 or contact ZEGO technical support.
  /// Related callbacks: The room custom command can be received through [onIMRecvCustomCommand].
  /// Related APIs: Broadcast messages can be sent through the [sendBroadcastMessage] function, and barrage messages can be sent through the [sendBarrageMessage] function.
  /// Privacy reminder: Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  ///
  /// - [roomID] Room ID. Required: Yes. Value range: The maximum length is 128 bytes. Caution: The room ID is in string format and only supports numbers, English characters and'~','!','@','#','$','%','^','&', ' *','(',')','_','+','=','-','`',';',''',',','.','<' ,'>','/','\'.
  /// - [command] Custom command content. Required: Yes. Value range: The maximum length is 1024 bytes. Caution: To protect privacy, please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
  /// - [toUserList] List of recipients of signaling. Required: Yes. Value range: user list or [null]. Caution: When it is [null], the SDK will send custom signaling back to all users in the room
  /// - Returns Send command result callback.
  Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {
    return await ZegoExpressImpl.instance.sendCustomCommand(roomID, command, toUserList);
  }

}
