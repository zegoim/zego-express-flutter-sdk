
#ifndef __ZEGO_EXPRESS_ROOM_H__
#define __ZEGO_EXPRESS_ROOM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/**
 * Logs in to a room with advanced room configurations. You must log in to a room before publishing or playing streams.
 *
 * Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
 * To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
 * Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
 * Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
 * SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
 * If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
 * It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
 * After the first login failure due to network reasons or the room is disconnected, the default time of SDK reconnection is 20min.
 * For restrictions on the use of this function, please refer to https://doc-en.zego.im/article/7611  or contact ZEGO technical support.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 * @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
 * @param config Advanced room configuration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_login_room(const char * room_id, struct zego_user user, struct zego_room_config * config);


/**
 * Logs in multi room.
 *
 * You must log in the main room with [loginRoom] before invoke this function to logging in to multi room.
 * Currently supports logging into 1 main room and 1 multi room at the same time.
 * When logging out, you must log out of the multi room before logging out of the main room.
 * User can only publish the stream in the main room, but can play the stream in the main room and multi room at the same time, and can receive the signaling and callback in each room.
 * The advantage of multi room is that you can login another room without leaving the current room, receive signaling and callback from another room, and play streams from another room.
 * To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter.
 * Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
 * Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
 * SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
 * If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
 * It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 * @param config Advanced room configuration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_login_multi_room(const char * room_id, struct zego_room_config * config);


/**
 * Logs out of a room.
 *
 * Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method.
 * After calling this function, you will receive [onRoomStateUpdate] callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).'
 *
 * @param room_id Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_logout_room(const char * room_id);


/**
 * Switch the room with advanced room configurations.
 *
 * After successfully login room, if you need to quickly switch to the next room, you can call this function.
 * Calling this function is faster and easier to use than calling [logoutRoom] and then [loginRoom].
 * When this function is called, all streams currently publishing or playing will stop (but the local preview will not stop).
 * To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter. This parameter configuration affects the room to be switched over.
 *
 * @param from_room_id Current roomID
 * @param to_room_id The next roomID
 * @param config Advanced room configuration
 */
ZEGOEXP_API zego_error EXP_CALL zego_express_switch_room(const char * from_room_id, const char * to_room_id, struct zego_room_config * config);


/**
 * Set room extra information.
 *
 * After the user in the room calls this function to set the extra info of the room, other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback function.
 * For restrictions on the use of this function, please refer to https://doc-en.zego.im/article/7611.html or contact ZEGO technical support.
 *
 * @param room_id Room ID.
 * @param key key of the extra info.
 * @param value value if the extra info.
 */
ZEGOEXP_API zego_seq EXP_CALL zego_express_set_room_extra_info(const char * room_id, const char * key, const char * value);


/**
 * The callback triggered when the room connection state changes.
 *
 * This callback is triggered when the connection status of the room changes, and the reason for the change is notified. Developers can use this callback to determine the status of the current user in the room. If the connection is being requested for a long time, the general probability is that the user's network is unstable.
 *
 * @param room_id Room ID, a string of up to 128 bytes in length.
 * @param state Changed room state
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param extended_data Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
 * @param user_context context of user
 */
typedef void(*zego_on_room_state_update)(const char * room_id, enum zego_room_state state, zego_error error_code, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_state_update_callback(zego_on_room_state_update callback_func, void * user_context);


/**
 * The callback triggered when the number of other users in the room increases or decreases.
 *
 * Note that the callback is only triggered when the isUserStatusNotify parameter in the ZegoRoomConfig passed loginRoom function is true. Developers can use this callback to determine the situation of users in the room.
 * If developers need to use ZEGO room users notifications, please make sure that each login user sets isUserStatusNotify to true
 * When a user logs in to a room for the first time, other users already exist in this room, and a user list of the type of addition is received.
 * When the user is already in the room, other users in this room will trigger this callback to notify the changed users when they enter or exit the room.
 *
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param update_type Update type (add/delete)
 * @param user_list List of users changed in the current room
 * @param user_count List count of users changed in the current room
 * @param user_context context of user
 */
typedef void(*zego_on_room_user_update)(const char * room_id, enum zego_update_type update_type, const struct zego_user* user_list, unsigned int user_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_user_update_callback(zego_on_room_user_update callback_func, void * user_context);


/**
 * The callback triggered every 30 seconds to report the current number of online users.
 *
 * This function is called back every 30 seconds.
 * Developers can use this callback to show the number of user online in the current room.
 *
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param count Count of online users
 * @param user_context context of user
 */
typedef void(*zego_on_room_online_user_count_update)(const char * room_id, int count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_online_user_count_update_callback(zego_on_room_online_user_count_update callback_func, void * user_context);


/**
 * The callback triggered when the number of streams published by the other users in the same room increases or decreases.
 *
 * When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
 * When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
 * Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
 *
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param update_type Update type (add/delete)
 * @param stream_list Updated stream list
 * @param stream_info_count List count of streams changed in the current room
 * @param extended_data Extended information with stream updates.
 * @param user_context context of user
 */
typedef void(*zego_on_room_stream_update)(const char * room_id, enum zego_update_type update_type, const struct zego_stream* stream_list, unsigned int stream_info_count, const char * extended_data, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_stream_update_callback(zego_on_room_stream_update callback_func, void * user_context);


/**
 * The callback triggered when there is an update on the extra information of the streams published by other users in the same room.
 *
 * When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
 * The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
 * Developers can synchronize variable content related to stream IDs based on stream additional information.
 *
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param stream_list List of streams that the extra info was updated.
 * @param stream_info_count List count of streams changed in the current room
 * @param user_context context of user
 */
typedef void(*zego_on_room_stream_extra_info_update)(const char * room_id, const struct zego_stream* stream_list, unsigned int stream_info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_stream_extra_info_update_callback(zego_on_room_stream_extra_info_update callback_func, void * user_context);


/**
 * The callback triggered when there is an update on the extra information of the room.
 *
 * When a user update the room extra information, other users in the same room will receive the callback.
 *
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param room_extra_info_list List of the extra info updated.
 * @param room_extra_info_count List count of extra info changed in the current room
 * @param user_context context of user
 */
typedef void(*zego_on_room_extra_info_update)(const char * room_id, const struct zego_room_extra_info* room_extra_info_list, unsigned int room_extra_info_count, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_extra_info_update_callback(zego_on_room_extra_info_update callback_func, void * user_context);


/**
 * Callback for setting room extra information.
 *
 * @param error_code Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
 * @param room_id Room ID where the user is logged in, a string of up to 128 bytes in length.
 * @param key the key of the extra info
 * @param seq Message sequence.
 * @param user_context Context of user.
 */
typedef void(*zego_on_room_set_room_extra_info_result)(zego_error error_code, const char * room_id, const char * key, zego_seq seq, void * user_context);

ZEGOEXP_API void EXP_CALL zego_register_room_set_room_extra_info_result_callback(zego_on_room_set_room_extra_info_result callback_func, void * user_context);



ZEGO_END_DECLS

#endif
