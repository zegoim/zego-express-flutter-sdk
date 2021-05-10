
#ifndef __ZEGO_EXPRESS_IM_H__
#define __ZEGO_EXPRESS_IM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Sends a Broadcast Message.
 *
 * The sending frequency of broadcast messages in the same room cannot be higher than 10 messages/s.
 * A certain number of users in the same room who entered the room earlier can receive this callback. The message is reliable. It is generally used when the number of people in the live room is less than a certain number. The specific number is determined by the configuration of the ZEGO server.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 * @param message Message content, no longer than 1024 bytes
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_send_broadcast_message(const char * room_id, const char * message);


/**
 * Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
 *
 * The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s.
 * The message is unreliable. When the frequency of sending barrage messages in the entire room is greater than 20 messages/s, the recipient may not receive the message. It is generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live broadcast barrage.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 * @param message Message content, no longer than 1024 bytes
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_send_barrage_message(const char * room_id, const char * message);


/**
 * Sends a Custom Command to the specified users in the same room.
 *
 * Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
 * The frequency of custom messages sent to a single user in the same room cannot be higher than 200 messages/s, and the frequency of custom messages sent to multiple users cannot be higher than 10 messages/s.
 * The point-to-point signaling type in the same room is generally used for remote control signaling or for sending messages between users. The messages are reliable.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 * @param command Custom command content, no longer than 1024 bytes
 * @param to_user_list The users who will receive the command
 * @return Sequence of message.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_send_custom_command(const char * room_id, const char * command, struct zego_user* to_user_list, unsigned int to_user_count);


/**
 * The callback triggered when Broadcast Messages are received.
 *
 * This callback is used to receive broadcast messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
 *
 * @param room_id Room ID
 * @param message_list list of received messages.
 * @param message_count List count of received messages.
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_recv_broadcast_message)(const char * room_id, const struct zego_broadcast_message_info* message_list, unsigned int message_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_recv_broadcast_message_callback(zego_on_im_recv_broadcast_message callback_func, void * user_context);


/**
 * The callback triggered when Barrage Messages are received.
 *
 * This callback is used to receive barrage messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
 *
 * @param room_id Room ID
 * @param message_list list of received messages.
 * @param message_count List count of received messages.
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_recv_barrage_message)(const char * room_id, const struct zego_barrage_message_info* message_list, unsigned int message_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_recv_barrage_message_callback(zego_on_im_recv_barrage_message callback_func, void * user_context);


/**
 * The callback triggered when a Custom Command is received.
 *
 * This callback is used to receive custom signaling sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
 *
 * @param room_id Room ID
 * @param from_user Sender of the command
 * @param command Command content received
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_recv_custom_command)(const char * room_id, struct zego_user from_user, const char * command, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_recv_custom_command_callback(zego_on_im_recv_custom_command callback_func, void * user_context);


/**
 * Callback for sending broadcast messages.
 *
 * @param room_id Room ID.
 * @param message_id ID of this message
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence of sending broadcast message.
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_send_broadcast_message_result)(const char * room_id, unsigned long long message_id, zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_send_broadcast_message_result_callback(zego_on_im_send_broadcast_message_result callback_func, void * user_context);


/**
 * Callback for sending barrage message.
 *
 * @param room_id Room ID.
 * @param message_id ID of this message
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence of sending broadcast message.
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_send_barrage_message_result)(const char * room_id, const char * message_id, zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_send_barrage_message_result_callback(zego_on_im_send_barrage_message_result callback_func, void * user_context);


/**
 * Callback for sending custom command.
 *
 * @param room_id Room ID.
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param seq Sequence of sending broadcast message.
 * @param user_context Context of user.
 */
typedef void(*zego_on_im_send_custom_command_result)(const char * room_id, zego_error error_code, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_im_send_custom_command_result_callback(zego_on_im_send_custom_command_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
