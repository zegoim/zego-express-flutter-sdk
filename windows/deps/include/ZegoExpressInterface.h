
#ifndef __ZEGOEXPRESSINTERFACE_H__
#define __ZEGOEXPRESSINTERFACE_H__

#include "ZegoExpressDefines.h"
#include "ZegoExpressErrorCode.h"
#include "ZegoExpressEventHandler.h"

namespace ZEGO {
    namespace EXPRESS {

        class IZegoMediaPlayer;
        class IZegoAudioEffectPlayer;


        class IZegoExpressEngine
        {
        protected:

            virtual ~IZegoExpressEngine() {}

        public:

            /**
             * Sets up the event notification callbacks that need to be handled. If the eventHandler is set to [nullptr], all the callbacks set previously will be cleared.
             *
             * Invoke this function will OVERWRITE the handler set in [createEngine] or the handler set by the last call to this method.
             *
             * @param eventHandler Event notification callback. Developers should override callback related methods to focus on specific notifications based on their own business scenarios. The main callback methods of SDK are in [IZegoEventHandler].
             */
            virtual void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) = 0;

            /**
             * Uploads logs to the ZEGO server.
             *
             * By default, SDK creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
             * Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
             * The function is valid for the entire life cycle of the SDK.
             */
            virtual void uploadLog() = 0;

            /**
             * Uploads logs to the ZEGO server.
             *
             * By default, SDK creates and prints log files in the app's default directory. Each log file defaults to a maximum of 5MB. Three log files are written over and over in a circular fashion. When calling this function, SDK will auto package and upload the log files to the ZEGO server.
             * Developers can provide a business “feedback” channel in the app. When users feedback problems, they can call this function to upload the local log information of SDK to help locate user problems.
             * The function is valid for the entire life cycle of the SDK.
             *
             * @param callback Log upload result callback
             */
            virtual void uploadLog(ZegoUploadLogResultCallback callback) = 0;

            /**
             * Turns on/off verbose debugging and sets up the log language.
             *
             * The debug switch is set to on and the language is English by default.
             *
             * @deprecated This method has been deprecated after version 2.3.0, please use the [setEngineConfig] function to set the advanced configuration property advancedConfig to achieve the original function.
             * @param enable Detailed debugging information switch
             * @param language Debugging information language
             */
            ZEGO_DEPRECATED
            virtual void setDebugVerbose(bool enable, ZegoLanguage language) = 0;

            /**
             * Call the RTC experimental API
             *
             * ZEGO provides some technical previews or special customization functions in RTC business through this API. If you need to get the use of the function or the details, please consult ZEGO technical support
             *
             * @param params You need to pass in a parameter in the form of a JSON string
             * @return Returns an argument in the format of a JSON string
             */
            virtual std::string callExperimentalAPI(const std::string& params) = 0;

            /**
             * Logs in to a room. You must log in to a room before publishing or playing streams.
             *
             * Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
             * Different users who log in to the same room can get room related notifications in the same room (eg [onRoomUserUpdate], [onRoomStreamUpdate], etc.), and users in one room cannot receive room signaling notifications in another room.
             * Messages sent in one room (e.g. [setStreamExtraInfo], [sendBroadcastMessage], [sendBarrageMessage], [sendCustomCommand], etc.) cannot be received callback ((eg [onRoomStreamExtraInfoUpdate], [onIMRecvBroadcastMessage], [onIMRecvBarrageMessage], [onIMRecvCustomCommand], etc) in other rooms. Currently, SDK does not provide the ability to send messages across rooms. Developers can integrate the SDK of third-party IM to achieve.
             * SDK supports startPlayingStream audio and video streams from different rooms under the same appID, that is, startPlayingStream audio and video streams across rooms. Since ZegoExpressEngine's room related callback notifications are based on the same room, when developers want to startPlayingStream streams across rooms, developers need to maintain related messages and signaling notifications by themselves.
             * If the network is temporarily interrupted due to network quality reasons, the SDK will automatically reconnect internally. You can get the current connection status of the local room by listening to the [onRoomStateUpdate] callback method, and other users in the same room will receive [onRoomUserUpdate] callback notification.
             * It is strongly recommended that userID corresponds to the user ID of the business APP, that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID. Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
             * After the first login failure due to network reasons or the room is disconnected, the default time of SDK reconnection is 20min.
             * For restrictions on the use of this function, please refer to https://doc-en.zego.im/article/7611  or contact ZEGO technical support.
             *
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
             */
            virtual void loginRoom(const std::string& roomID, ZegoUser user) = 0;

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
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param user User object instance, configure userID, userName. Note that the userID needs to be globally unique with the same appID, otherwise the user who logs in later will kick out the user who logged in first.
             * @param config Advanced room configuration
             */
            virtual void loginRoom(const std::string& roomID, ZegoUser user, ZegoRoomConfig config) = 0;

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
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param config Advanced room configuration
             */
            virtual void loginMultiRoom(const std::string& roomID, ZegoRoomConfig* config = nullptr) = 0;

            /**
             * Logs out of a room.
             *
             * Exiting the room will stop all publishing and playing streams for user, and inner audio and video engine will stop, and then SDK will auto stop local preview UI. If you want to keep the preview ability when switching rooms, please use the [switchRoom] method.
             * After calling this function, you will receive [onRoomStateUpdate] callback notification successfully exits the room, while other users in the same room will receive the [onRoomUserUpdate] callback notification(On the premise of enabling isUserStatusNotify configuration).'
             *
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             */
            virtual void logoutRoom(const std::string& roomID) = 0;

            /**
             * Switch the room with advanced room configurations.
             *
             * After successfully login room, if you need to quickly switch to the next room, you can call this function.
             * Calling this function is faster and easier to use than calling [logoutRoom] and then [loginRoom].
             * When this function is called, all streams currently publishing or playing will stop (but the local preview will not stop).
             * To prevent the app from being impersonated by a malicious user, you can add authentication before logging in to the room, that is, the [token] parameter in the ZegoRoomConfig object passed in by the [config] parameter. This parameter configuration affects the room to be switched over.
             *
             * @param fromRoomID Current roomID
             * @param toRoomID The next roomID
             * @param config Advanced room configuration
             */
            virtual void switchRoom(const std::string& fromRoomID, const std::string& toRoomID, ZegoRoomConfig* config = nullptr) = 0;

            /**
             * Set room extra information.
             *
             * After the user in the room calls this function to set the extra info of the room, other users in the same room will be notified through the [onRoomExtraInfoUpdate] callback function.
             * For restrictions on the use of this function, please refer to https://doc-en.zego.im/article/7611.html or contact ZEGO technical support.
             *
             * @param roomID Room ID.
             * @param key key of the extra info.
             * @param value value if the extra info.
             * @param callback Callback for setting room extra information
             */
            virtual void setRoomExtraInfo(const std::string& roomID, const std::string& key, const std::string& value, ZegoRoomSetRoomExtraInfoCallback callback) = 0;

            /**
             * Starts publishing a stream (for the specified channel). You can call this function to publish a second stream.
             *
             * This function allows users to publish their local audio and video streams to the ZEGO RTC server. Other users in the same room can use the streamID to play the audio and video streams for intercommunication.
             * Before you start to publish the stream, you need to join the room first by calling [loginRoom]. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
             * In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
             * After the first publish stream failure due to network reasons or the publish stream is interrupted, the default time for SDK reconnection is 20min.
             *
             * @param streamID Stream ID, a string of up to 256 characters, needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
             * @param channel Publish stream channel
             */
            virtual void startPublishingStream(const std::string& streamID, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Stops publishing a stream (for the specified channel).
             *
             * This function allows the user to stop sending local audio and video streams and end the call.
             * If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
             * After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
             * Use this function to stop publishing stream of aux channel.
             *
             * @param channel Publish stream channel
             */
            virtual void stopPublishingStream(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Sets the extra information of the stream being published (for the specified channel).
             *
             * Use this function to set the extra info of the stream, the result will be notified via the [ZegoPublisherSetStreamExtraInfoCallback].
             * The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
             * Developers can synchronize variable content related to stream IDs based on stream additional information.
             *
             * @param extraInfo Stream extra information, a string of up to 1024 characters.
             * @param callback Set stream extra information execution result notification
             * @param channel Publish stream channel
             */
            virtual void setStreamExtraInfo(const std::string& extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Starts the local audio preview. Call this function after creating the engine and before publishing/playing a stream, and then the engine will start to capture audio.
             *
             * Since this function can be called before loginRoom and startPublishingStream, developers can check whether the audio equipment works ok by calling this function and by the soundlevel function of Engine.
             * When this function is called, the audio and video engine module inside SDK will start, and it will start to try to collect audio. Developers can pass [nullptr] to the canvas parameter to cooperate with ZegoExpressEngine's sound wave function in order to achieve the purpose of detecting whether the audio equipment works normally before logging into the room.
             */
            virtual void startPreview() = 0;

            /**
             * Stops the local audio preview.
             *
             * This function can be called to stop previewing when there is no need to see the preview locally.
             */
            virtual void stopPreview() = 0;

            /**
             * Sets up the audio configurations.
             *
             * You can set the combined value of the audio codec, bit rate, and audio channel through this function. If this function is not called, the default is standard quality mode. Should be used before publishing.
             * If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
             *
             * @param config Audio config
             * @param channel Publish stream channel
             */
            virtual void setAudioConfig(ZegoAudioConfig config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Gets the current audio configurations.
             *
             * You can get the current audio codec, bit rate, and audio channel through this function.
             *
             * @param channel Publish stream channel
             * @return Audio config
             */
            virtual ZegoAudioConfig getAudioConfig(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Set encryption key for the publishing stream.
             *
             * Called before and after [startPublishingStream] can both take effect.
             * Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
             * Support calling this function to update the encryption key while publishing stream. Note that developers need to update the player's decryption key before updating the publisher's encryption key.
             * This function is only valid when publishing stream to the Zego RTC server.
             *
             * @param key The encryption key, note that the key length only supports 16/24/32 bytes.
             * @param channel Publish stream channel
             */
            virtual void setPublishStreamEncryptionKey(const std::string& key, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Stops or resumes sending the audio part of a stream (for the specified channel).
             *
             * This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but does not send the audio data to the network. It can be set before and after publishing.
             * If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks,
             *
             * @param mute Whether to stop sending audio streams, true means not to send audio stream, and false means sending audio stream. The default is false.
             * @param channel Publish stream channel
             */
            virtual void mutePublishStreamAudio(bool mute, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Enables or disables traffic control.
             *
             * Traffic control enables SDK to dynamically adjust the bitrate of audio and video streaming according to its own and peer current network environment status.
             * Automatically adapt to the current network environment and fluctuations, so as to ensure the smooth publishing of stream.
             *
             * @param enable Whether to enable traffic control. The default is ture.
             * @param property Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
             * @param channel Publish stream channel
             */
            virtual void enableTrafficControl(bool enable, int property, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Set the factors of concern that trigger traffic control
             *
             * When the traffic control of the specified push channel is enabled through the enableTrafficControl interface, the interface can be used to control whether the traffic control is started due to poor remote network conditions
             * The function is valid before the stream is pushed. If you do not reset the settings before each push, you will continue to use the last configuration.
             *
             * @param mode When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
             * @param channel Publish stream channel
             */
            virtual void setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Sets the audio recording volume for stream publishing.
             *
             * This function is used to set the audio collection volume. The local user can control the volume of the audio stream sent to the far end. It can be set before publishing.
             *
             * @param volume Volume percentage. The range is 0 to 200. Default value is 100.
             */
            virtual void setCaptureVolume(int volume) = 0;

            /**
             * Set audio capture stereo mode.
             *
             * This function is used to set the audio stereo capture mode. The default is mono, that is, dual channel collection is not enabled.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param mode Audio stereo capture mode
             */
            virtual void setAudioCaptureStereoMode(ZegoAudioCaptureStereoMode mode) = 0;

            /**
             * Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
             *
             * Developers can call this function to publish the audio and video streams that have been published to the ZEGO RTC server to a custom CDN content distribution network that has high latency but supports high concurrent playing stream.
             * Because this called function is essentially a dynamic relay of the audio and video streams published to the ZEGO RTC server to different CDNs, this function needs to be called after the audio and video stream is published to ZEGO RTC server successfully.
             * Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves.
             * You can use ZEGO's CDN audio and video streaming content distribution service at the same time by calling this function and then use the developer who owns the CDN content distribution service.
             * This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
             * When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
             *
             * @param streamID Stream ID
             * @param targetURL CDN relay address, supported address format is rtmp.
             * @param callback The execution result of update the relay CDN operation
             */
            virtual void addPublishCdnUrl(const std::string& streamID, const std::string& targetURL, ZegoPublisherUpdateCdnUrlCallback callback) = 0;

            /**
             * Deletes the specified CDN URL, which is used for relaying streams from ZEGO RTC server to CDN.
             *
             * This function is called when a CDN relayed address has been added and needs to stop propagating the stream to the CDN.
             * This function does not stop publishing audio and video stream to the ZEGO ZEGO RTC server.
             *
             * @param streamID Stream ID
             * @param targetURL CDN relay address, supported address format rtmp.
             * @param callback The execution result of update the relay CDN operation
             */
            virtual void removePublishCdnUrl(const std::string& streamID, const std::string& targetURL, ZegoPublisherUpdateCdnUrlCallback callback) = 0;

            /**
             * Whether to publish streams directly from the client to CDN without passing through Zego RTC server (for the specified channel).
             *
             * This function needs to be set before [startPublishingStream].
             * After calling this function to publish the audio and video stream directly to the CDN, calling [addPublishCdnUrl] and [removePublishCdnUrl] to dynamically relay to the CDN no longer takes effect,
             * because these two functions are to relay or stop relaying the audio and video stream from ZEGO RTC server to CDN,
             * if you enable the direct publish of audio and video streams to CDN, you will not be able to dynamically relay the audio and video streams to the CDN through the ZEGO RTC server.
             *
             * @param enable Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN
             * @param config CDN configuration, if nullptr, use Zego's background default configuration
             * @param channel Publish stream channel
             */
            virtual void enablePublishDirectToCDN(bool enable, ZegoCDNConfig* config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Set the Supplemental Enhancement Information type
             *
             * It must be set before [startPublishingStream].
             *
             * @param config SEI configuration. The SEI defined by ZEGO is used by default.
             */
            virtual void setSEIConfig(ZegoSEIConfig config) = 0;

            /**
             * Sends Supplemental Enhancement Information.
             *
             * This function can synchronize some other additional information while the developer publishes streaming audio and video streaming data while sending streaming media enhancement supplementary information.
             * Generally, for scenarios such as synchronizing music lyrics or precise layout of video canvas, you can choose to use this function.
             * After the anchor sends the SEI, the audience can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
             * Since SEI information follows video frames, and because of network problems, frames may be dropped, so SEI information may also be dropped. To solve this situation, it should be sent several times within the limited frequency.
             * After calling [startPublishingStream] to publish the stream successfully, you can call this function.
             * Limit frequency: Do not exceed 30 times per second.
             * The SEI data length is limited to 4096 bytes.
             *
             * @param data SEI data
             * @param dataLength SEI data length
             * @param channel Publish stream channel
             */
            virtual void sendSEI(const unsigned char* data, unsigned int dataLength, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Enables or disables hardware encoding.
             *
             * Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage. The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
             * Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard coding.
             *
             * @param enable Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding
             */
            virtual void enableHardwareEncoder(bool enable) = 0;

            /**
             * Starts playing a stream from ZEGO RTC server.
             *
             * This function allows users to play audio and video streams from the ZEGO RTC server.
             * Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
             * In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
             * Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
             * The developer can update the player canvas by calling this function again(the streamID must be the same).
             *
             * @param streamID Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
             */
            virtual void startPlayingStream(const std::string& streamID) = 0;

            /**
             * Starts playing a stream from ZEGO RTC server or from third-party CDN.
             *
             * This function allows users to play audio and video streams both from the ZEGO RTC server and from third-party cdn.
             * Before starting to play the stream, you need to join the room first, you can get the new streamID in the room by listening to the [onRoomStreamUpdate] event callback.
             * In the case of poor network quality, user play may be interrupted, the SDK will try to reconnect, and the current play status and error information can be obtained by listening to the [onPlayerStateUpdate] event.
             * Playing the stream ID that does not exist, the SDK continues to try to play after calling this function. After the stream ID is successfully published, the audio and video stream can be actually played.
             * The developer can update the player canvas by calling this function again(the streamID must be the same).
             *
             * @param streamID Stream ID, a string of up to 256 characters. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
             * @param config Advanced player configuration
             */
            virtual void startPlayingStream(const std::string& streamID, ZegoPlayerConfig config) = 0;

            /**
             * Stops playing a stream.
             *
             * This function allows the user to stop playing the stream. When stopped, the attributes set for this stream previously, such as [setPlayVolume], [mutePlayStreamAudio], [mutePlayStreamVideo], etc., will be invalid and need to be reset when playing the the stream next time.
             *
             * @param streamID Stream ID
             */
            virtual void stopPlayingStream(const std::string& streamID) = 0;

            /**
             * Set decryption key for the playing stream.
             *
             * Called before and after [startPlayingStream] can both take effect.
             * Calling [stopPlayingStream] or [logoutRoom] will clear the decryption key.
             * Support calling this function to update the decryption key while playing stream. Note that developers need to update the player's decryption key before updating the publisher's encryption key.
             * This function is only valid when playing stream from Zego RTC or L3 server.
             *
             * @param streamID Stream ID
             * @param key The decryption key, note that the key length only supports 16/24/32 bytes.
             */
            virtual void setPlayStreamDecryptionKey(const std::string& streamID, const std::string& key) = 0;

            /**
             * Sets the stream playback volume.
             *
             * This function is used to set the playback volume of the stream. Need to be called after calling startPlayingStream.
             * You need to reset after [stopPlayingStream] and [startPlayingStream].
             *
             * @param streamID Stream ID. Set volume for all streams playing by set streamID as nullptr or empty.
             * @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
             */
            virtual void setPlayVolume(const std::string& streamID, int volume) = 0;

            /**
             * Sets the all stream playback volume.
             *
             * This function is used to set the sound size of all streaming streams, and the local user can control the playback volume of all audio streams.
             *
             * @param volume Volume percentage. The value ranges from 0 to 200, and the default value is 100.
             */
            virtual void setAllPlayStreamVolume(int volume) = 0;

            /**
             * Set the adaptive adjustment interval range of the buffer for playing stream.
             *
             * When the upper limit of the cache interval set by the developer exceeds 4000ms, the value will be 4000ms.
             * When the upper limit of the cache interval set by the developer is less than the lower limit of the cache interval, the upper limit will be automatically set as the lower limit.
             * It can be set before and after playing stream.
             *
             * @param streamID Stream ID.
             * @param minBufferInterval The lower limit of the buffer adaptation interval, in milliseconds. The default value is 0ms
             * @param maxBufferInterval The upper limit of the buffer adaptation interval, in milliseconds. The default value is 4000ms
             */
            virtual void setPlayStreamBufferIntervalRange(const std::string& streamID, unsigned int minBufferInterval, unsigned int maxBufferInterval) = 0;

            /**
             * Set the weight of the pull stream priority.
             *
             * The stream that is set to focus will give priority to ensuring its quality. By default, all streams have the same weight.
             * When the local network is not good, while ensuring the focus flow, it may cause more jams.
             *
             * @param streamID Stream ID.
             */
            virtual void setPlayStreamFocusOn(const std::string& streamID) = 0;

            /**
             * Stops or resumes playing the audio part of a stream.
             *
             * This function can be used to stop playing/retrieving the audio data of the stream. It can be called before and after playing the stream.
             *
             * @param streamID Stream ID
             * @param mute Mute flag, true: mute play stream audio, false: resume play stream audio
             */
            virtual void mutePlayStreamAudio(const std::string& streamID, bool mute) = 0;

            /**
             * Stop or resume all pulled audio streams.
             *
             * This function can be called when the stream is pulled, so that the audio data of all remote users is not pulled, and it can be called before and after the stream is pulled.
             * This function does not affect the life cycle of the `mutePlayStreamAudio` interface. This means that neither this function nor the `mutePlayStreamAudio` function must prohibit audio data before audio data can be received.
             *
             * @param mute Mute flag, true: mute play stream audio, false: resume play stream audio
             */
            virtual void muteAllPlayStreamAudio(bool mute) = 0;

            /**
             * Enables or disables hardware decoding.
             *
             * Turn on hardware decoding and use hardware to improve decoding efficiency. Need to be called before calling startPlayingStream.
             * Because hard-decoded support is not particularly good for a few models, SDK uses software decoding by default. If the developer finds that the device is hot when playing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard decoding.
             *
             * @param enable Whether to turn on hardware decoding switch, true: enable hardware decoding, false: disable hardware decoding. The default is false
             */
            virtual void enableHardwareDecoder(bool enable) = 0;

            /**
             * Starts a stream mixing task.
             *
             * Due to the performance considerations of the client device, ZegoExpressEngine's mix stream is to start the mixing stream task on the server side of the ZEGO RTC server for mixing stream.
             * After calling this function, SDK initiates a mixing stream request to the ZEGO RTC server. The server will find the current publishing stream and perform video layer blending according to the parameters of the mixing stream task requested by SDK.
             * When you need to update the mixing stream task, that is, the input stream list needs to be updated when the input stream increases or decreases, you can update the field of the [ZegoMixerTask] object inputList and call this function again to pass the same [ZegoMixerTask] object to update the mixing stream task.
             * If an exception occurs when requesting to start the mixing stream task, for example, the most common mix input stream does not exist, it will be given from the callback error code. For specific error codes, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * If an input stream does not exist in the middle, the mixing stream task will automatically retry playing the input stream for 90 seconds, and will not retry after 90 seconds.
             *
             * @param task Stream mixing task object
             * @param callback Start stream mixing task result callback notification
             */
            virtual void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) = 0;

            /**
             * Stops a stream mixing task.
             *
             * Similar to [startMixerTask], after calling this function, SDK initiates a request to end the mixing stream task to the ZEGO RTC server.
             * If you starts the next mixing stream task without stopping the previous mixing stream task, the previous mixing stream task will not stop automatically. The previous mixing stream task will not be stopped automatically until 90 seconds after the input stream of the previous mixing stream task does not exist.
             * Developers should pay attention when using the stream mixing function that, before starting the next mixer task, they should stop the previous mixer task, so as avoid that when an anchor has start the next mixer task to mix stream with other anchors, and the audience is still playing the previous mixer task's output stream.
             *
             * @param task Stream mixing task object
             * @param callback Stop stream mixing task result callback notification
             */
            virtual void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) = 0;

            /**
             * Mutes or unmutes the microphone.
             *
             * This function is used to control whether the collected audio data is used. When the microphone is muted (disabled), the data is collected and discarded, and the microphone is still occupied.
             * The microphone is still occupied because closing or opening the microphone on the hardware is a relatively heavy operation, and real users may have frequent operations. For trade-off reasons, this function simply discards the collected data.
             * If you really want SDK to give up occupy the microphone, you can call the [enableAudioCaptureDevice] function.
             * Developers who want to control whether to use microphone on the UI should use this function to avoid unnecessary performance overhead by using the [enableAudioCaptureDevice].
             *
             * @param mute Whether to mute (disable) the microphone, true: mute (disable) microphone, false: enable microphone. The default is false.
             */
            virtual void muteMicrophone(bool mute) = 0;

            /**
             * Checks whether the microphone is muted.
             *
             * Can be used with [muteMicrophone], determine whether the microphone is muted.
             *
             * @return Whether the microphone is muted; true: the microphone is muted; false: the microphone is enable (not muted)
             */
            virtual bool isMicrophoneMuted() = 0;

            /**
             * Mutes or unmutes the audio output speaker.
             *
             * After mute speaker, all the SDK sounds will not play, including playing stream, mediaplayer, etc. But the SDK will still occupy the output device.
             *
             * @param mute Whether to mute (disable) speaker audio output, true: mute (disable) speaker audio output, false: enable speaker audio output. The default value is false
             */
            virtual void muteSpeaker(bool mute) = 0;

            /**
             * Checks whether the audio output speaker is muted.
             *
             * Can be used with [muteSpeaker], determine whether the speaker audio output is muted.
             *
             * @return Whether the speaker is muted; true: the speaker is muted; false: the speaker is enable (not muted)
             */
            virtual bool isSpeakerMuted() = 0;

            /**
             * Gets a list of audio devices.
             *
             * @param deviceType Audio device type
             * @return Audo device List
             */
            virtual std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) = 0;

            /**
             * Get the device ID of the default audio device.
             *
             * @param deviceType Audio device type
             * @return Default Audio device ID
             */
            virtual std::string getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) = 0;

            /**
             * Chooses to use the specified audio device.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             */
            virtual void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * Get volume for the specified audio device.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             * @return Device volume
             */
            virtual int getAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * Set volume for the specified audio device.
             *
             * The direct operating system device may fail due to system restrictions. Please use [setCaptureVolume] and [setPlayVolume] first to adjust the volume of publish and play streams.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             * @param volume Device volume
             */
            virtual void setAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume) = 0;

            /**
             * Turn on audio device volume monitoring.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             */
            virtual void startAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * Turn on audio device volume monitoring.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             */
            virtual void stopAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * Mutes or unmutes the audio device.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             * @param mute Whether to mute the audio device; true means to mute the audio device; false means to unmute the audio device.
             */
            virtual void muteAudioDevice(ZegoAudioDeviceType deviceType, const std::string& deviceID, bool mute) = 0;

            /**
             * Check if the audio device is muted.
             *
             * @param deviceType Audio device type
             * @param deviceID ID of a device obtained by [getAudioDeviceList]
             * @return Whether the audio device is muted; true means the audio device is muted; false means the audio device is not muted.
             */
            virtual bool isAudioDeviceMuted(ZegoAudioDeviceType deviceType, const std::string& deviceID) = 0;

            /**
             * Enables or disables the audio capture device.
             *
             * This function is used to control whether to release the audio collection device. When the audio collection device is turned off, the SDK will no longer occupy the audio device. Of course, if the stream is being published at this time, there is no audio data.
             * Occupying the audio capture device and giving up Occupying the audio device is a relatively heavy operation, and the [muteMicrophone] function is generally recommended.
             *
             * @param enable Whether to enable the audio capture device, true: disable audio capture device, false: enable audio capture device
             */
            virtual void enableAudioCaptureDevice(bool enable) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID)
            /**
             * get current audio route type
             */
            virtual ZegoAudioRoute getAudioRouteType() = 0;
#endif

#if TARGET_OS_IPHONE || defined(ANDROID)
            /**
             * Whether to use the built-in speaker to play audio.
             *
             * When you choose not to use the built-in speaker to play sound, that is, set to [false], the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
             *
             * @param defaultToSpeaker Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
             */
            virtual void setAudioRouteToSpeaker(bool defaultToSpeaker) = 0;
#endif

            /**
             * Starts sound level monitoring. Support setting the listening interval.
             *
             * After starting monitoring, you can receive local audio sound level via [onCapturedSoundLevelUpdate] callback, and receive remote audio sound level via [onRemoteSoundLevelUpdate] callback.
             * Before entering the room, you can call [startPreview] with this function and combine it with [onCapturedSoundLevelUpdate] callback to determine whether the audio device is working properly.
             * [onCapturedSoundLevelUpdate] and [onRemoteSoundLevelUpdate] callback notification period is the value set by the parameter.
             *
             * @param millisecond Monitoring time period of the sound level, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
             */
            virtual void startSoundLevelMonitor(unsigned int millisecond = 100) = 0;

            /**
             * Stops sound level monitoring.
             *
             * After the monitoring is stopped, the callback of the local/remote audio sound level will be stopped.
             */
            virtual void stopSoundLevelMonitor() = 0;

            /**
             * Starts audio spectrum monitoring. Support setting the listening interval.
             *
             * After starting monitoring, you can receive local audio spectrum via [onCapturedAudioSpectrumUpdate] callback, and receive remote audio spectrum via [onRemoteAudioSpectrumUpdate] callback.
             * [onCapturedAudioSpectrumUpdate] and [onRemoteAudioSpectrumUpdate] callback notification period is the value set by the parameter.
             *
             * @param millisecond Monitoring time period of the audio spectrum, in milliseconds, has a value range of [100, 3000]. Default is 100 ms.
             */
            virtual void startAudioSpectrumMonitor(unsigned int millisecond = 100) = 0;

            /**
             * Stops audio spectrum monitoring.
             *
             * After the monitoring is stopped, the callback of the local/remote audio spectrum will be stopped.
             */
            virtual void stopAudioSpectrumMonitor() = 0;

            /**
             * Enables or disables headphone monitoring.
             *
             * enable/disable headphone monitor, this setting takes effect when the headset is connected.
             *
             * @param enable Whether to use headphone monitor, true: enable, false: disable
             */
            virtual void enableHeadphoneMonitor(bool enable) = 0;

            /**
             * Sets the headphone monitor volume.
             *
             * set headphone monitor volume, this setting takes effect when the headset is connected.
             *
             * @param volume headphone monitor volume, range from 0 to 200, 100 as default
             */
            virtual void setHeadphoneMonitorVolume(int volume) = 0;

            /**
             * Enables or disables system audio capture.
             *
             * Enable sound card capture to mix sounds played by the system into the publishing stream, such as sounds played by the browser, sounds played by other software, etc.
             *
             * @param enable Whether to mix system playout
             */
            virtual void enableMixSystemPlayout(bool enable) = 0;

            /**
             * set mix system playout volume
             *
             * only windows/macOS supported
             *
             * @param volume the volume. Valid range [0, 200], default is 100
             */
            virtual void setMixSystemPlayoutVolume(int volume) = 0;

            /**
             * Enables or disables mix engine playout.
             *
             * Enable engine playout to mix sounds played by engine into the stream publishing.
             *
             * @param enable Whether to mix engine playout
             */
            virtual void enableMixEnginePlayout(bool enable) = 0;

            /**
             * Enables or disables acoustic echo cancellation (AEC).
             *
             * Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable echo cancellation, true: enable, false: disable
             */
            virtual void enableAEC(bool enable) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID)
            /**
             * Whether to turn on acoustic echo cancellation (AEC) when using the headphone.
             *
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable, true: enable, false: disable
             */
            virtual void enableHeadphoneAEC(bool enable) = 0;
#endif

            /**
             * Sets the acoustic echo cancellation (AEC) mode.
             *
             * Switch different echo cancellation modes to control the extent to which echo data is eliminated.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param mode Echo cancellation mode
             */
            virtual void setAECMode(ZegoAECMode mode) = 0;

            /**
             * Enables or disables automatic gain control (AGC).
             *
             * When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable automatic gain control, true: enable, false: disable
             */
            virtual void enableAGC(bool enable) = 0;

            /**
             * Enables or disables active noise suppression (ANS, aka ANC).
             *
             * Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable noise suppression, true: enable, false: disable
             */
            virtual void enableANS(bool enable) = 0;

            /**
             * Enables or disables transient noise suppression.
             *
             * Suppress transient noises such as keyboard and desk knocks
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable transient noise suppression, true: enable, false: disable
             */
            virtual void enableTransientANS(bool enable) = 0;

            /**
             * Sets the automatic noise suppression (ANS) mode.
             *
             * Default is medium mode
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param mode Audio Noise Suppression mode
             */
            virtual void setANSMode(ZegoANSMode mode) = 0;

            /**
             * Enables or disables audio mixing.
             *
             * Enable audio mixing, work with setAudioMixingHandler providing the audio data for mixing
             *
             * @param enable Whether to enable audio mixting, true: enable, false: disable
             */
            virtual void enableAudioMixing(bool enable) = 0;

            /**
             * Sets up the audio mixing event handler.
             *
             * @param handler Audio mixing callback handler
             */
            virtual void setAudioMixingHandler(std::shared_ptr<IZegoAudioMixingHandler> handler) = 0;

            /**
             * Stops or resumes playing the mixed audio locally.
             *
             * When stop play audio mixing locally, the audio will not be heard on the mix side (publisher side) while still be heard on the remote side (player side).
             *
             * @param mute Whether to mute local audio mixting, true: mute, false: unmute
             */
            virtual void muteLocalAudioMixing(bool mute) = 0;

            /**
             * Sets the audio mixing output volume for either local playback or the stream to published.
             *
             * This function can individually set the mixing volume of local playback or the mixing volume of the publishing stream.
             *
             * @param volume The audio mixing volume, range from 0 to 200, 100 as default
             * @param type volume local playback / volume in stream published
             */
            virtual void setAudioMixingVolume(int volume, ZegoVolumeType type) = 0;

            /**
             * Set the sound equalizer (EQ).
             *
             * @param bandIndex Band frequency index, the value range is [0, 9], corresponding to 10 frequency bands, and the center frequencies are [31, 62, 125, 250, 500, 1K, 2K, 4K, 8K, 16K] Hz.
             * @param bandGain Band gain for the index, the value range is [-15, 15]. Default value is 0, if all gain values in all frequency bands are 0, EQ function will be disabled.
             */
            virtual void setAudioEqualizerGain(int bandIndex, float bandGain) = 0;

            /**
             * Setting up the voice changer via preset enumeration.
             *
             * Voice changer effect is only effective for the captured sound.
             * This function is an encapsulated version of [setVoiceChangerParam], which provides some preset values. If you need to configure the voice changer effects, please use [setVoiceChangerParam]
             * This function is mutually exclusive with [setReverbPreset]. If used at the same time, it will produce undefined effects.
             * Some enumerated preset will modify the parameters of reverberation or reverberation echo, so after calling this function, calling [setVoiceChangerParam], [setReverbAdvancedParam], [setReverbEchoParam] may affect the voice changer effect.
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @param preset The voice changer preset enumeration
             */
            virtual void setVoiceChangerPreset(ZegoVoiceChangerPreset preset) = 0;

            /**
             * Setting up the specific voice changer parameters.
             *
             * Voice changer effect is only effective for the captured sound.
             * This function is an advanced version of [setVoiceChangerPreset], you can configure the voice changer effect by yourself.
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @param param Voice changer parameters
             */
            virtual void setVoiceChangerParam(ZegoVoiceChangerParam param) = 0;

            /**
             * Setting up the reverberation via preset enumeration.
             *
             * Support dynamic settings when publishing stream.
             * This function is a encapsulated version of [setReverbAdvancedParam], which provides some preset values. If you need to configure the reverb, please use [setReverbAdvancedParam]
             * This function is mutually exclusive with [setVoiceChangerPreset]. If used at the same time, it will produce undefined effects.
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @param preset The reverberation preset enumeration
             */
            virtual void setReverbPreset(ZegoReverbPreset preset) = 0;

            /**
             * Setting up the specific reverberation parameters.
             *
             * Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
             * This function is an advanced version of [setReverbPreset], you can configure the reverb effect by yourself.
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @param param Reverb advanced parameter
             */
            virtual void setReverbAdvancedParam(ZegoReverbAdvancedParam param) = 0;

            /**
             * Setting up the specific reverberation echo parameters.
             *
             * This function can be used with voice changer and reverb to achieve a variety of custom sound effects
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @param param The reverberation echo parameter
             */
            virtual void setReverbEchoParam(ZegoReverbEchoParam param) = 0;

            /**
             * Enables the virtual stereo feature.
             *
             * Note: You need to set up a dual channel setAudioConfig for the virtual stereo to take effect!
             *
             * @param enable true to turn on the virtual stereo, false to turn off the virtual stereo
             * @param angle angle of the sound source in the virtual stereo, ranging from 0 to 180, with 90 being the front, and 0 and 180 being respectively Corresponds to rightmost and leftmost, usually use 90.
             */
            virtual void enableVirtualStereo(bool enable, int angle) = 0;

            /**
             * Sends a Broadcast Message.
             *
             * The sending frequency of broadcast messages in the same room cannot be higher than 10 messages/s.
             * A certain number of users in the same room who entered the room earlier can receive this callback. The message is reliable. It is generally used when the number of people in the live room is less than a certain number. The specific number is determined by the configuration of the ZEGO server.
             *
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param message Message content, no longer than 1024 bytes
             * @param callback Send broadcast message result callback
             */
            virtual void sendBroadcastMessage(const std::string& roomID, const std::string& message, ZegoIMSendBroadcastMessageCallback callback) = 0;

            /**
             * Sends a Barrage Message (bullet screen) to all users in the same room, without guaranteeing the delivery.
             *
             * The frequency of sending barrage messages in the same room cannot be higher than 20 messages/s.
             * The message is unreliable. When the frequency of sending barrage messages in the entire room is greater than 20 messages/s, the recipient may not receive the message. It is generally used in scenarios where there is a large number of messages sent and received in the room and the reliability of the messages is not required, such as live broadcast barrage.
             *
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param message Message content, no longer than 1024 bytes
             * @param callback Send barrage message result callback
             */
            virtual void sendBarrageMessage(const std::string& roomID, const std::string& message, ZegoIMSendBarrageMessageCallback callback) = 0;

            /**
             * Sends a Custom Command to the specified users in the same room.
             *
             * Please do not fill in sensitive user information in this interface, including but not limited to mobile phone number, ID number, passport number, real name, etc.
             * The frequency of custom messages sent to a single user in the same room cannot be higher than 200 messages/s, and the frequency of custom messages sent to multiple users cannot be higher than 10 messages/s.
             * The point-to-point signaling type in the same room is generally used for remote control signaling or for sending messages between users. The messages are reliable.
             *
             * @param roomID Room ID, a string of up to 128 bytes in length. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'
             * @param command Custom command content, no longer than 1024 bytes
             * @param toUserList The users who will receive the command
             * @param callback Send command result callback
             */
            virtual void sendCustomCommand(const std::string& roomID, const std::string& command, std::vector<ZegoUser> toUserList, ZegoIMSendCustomCommandCallback callback) = 0;

            /**
             * Creates a media player instance.
             *
             * Currently, a maximum of 4 instances can be created, after which it will return nullptr. The more instances of a media player, the greater the performance overhead on the device.
             *
             * @return Media player instance, nullptr will be returned when the maximum number is exceeded.
             */
            virtual IZegoMediaPlayer* createMediaPlayer() = 0;

            /**
             * Destroys a media player instance.
             *
             * @param mediaPlayer The media player instance object to be destroyed
             */
            virtual void destroyMediaPlayer(IZegoMediaPlayer*& mediaPlayer) = 0;

            /**
             * Creates a audio effect player instance.
             *
             * Currently, a maximum of 1 instances can be created, after which it will return nullptr.
             *
             * @return audio effect player instance, nullptr will be returned when the maximum number is exceeded.
             */
            virtual IZegoAudioEffectPlayer* createAudioEffectPlayer() = 0;

            /**
             * Destroys a audio effect player instance.
             *
             * @param audioEffectPlayer The audio effect player instance object to be destroyed
             */
            virtual void destroyAudioEffectPlayer(IZegoAudioEffectPlayer*& audioEffectPlayer) = 0;

            /**
             * Starts to record locally captured audio or video and directly save the data to a file.
             *
             * Currently only one task can be recorded simultaneously.
             * This function needs to be called after the success of [startPreview] or [startPublishingStream] to be effective.
             * Developers should not [stopPreview] or [stopPublishingStream] during recording, otherwise the SDK will end the current recording task.
             * Developers will receive the [onCapturedDataRecordStateUpdate] and the [onCapturedDataRecordProgressUpdate] callback after start recording.
             *
             * @param config Record config
             * @param channel Publishing stream channel
             */
            virtual void startRecordingCapturedData(ZegoDataRecordConfig config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Stops recording locally captured audio or video.
             *
             * @param channel Publishing stream channel
             */
            virtual void stopRecordingCapturedData(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Sets up the event callback handler for data recording.
             *
             * Implemente the functions of ZegoDataRecordEventHandler to get notified when recode state and process changed
             *
             * @param eventHandler Data record event handler
             */
            virtual void setDataRecordEventHandler(std::shared_ptr<IZegoDataRecordEventHandler> eventHandler) = 0;

            /**
             * Starts system performance status monitoring. Support setting the listening interval.
             *
             * After starting monitoring, you can receive system performance status via [onPerformanceStatusUpdate] callback.
             * [onPerformanceStatusUpdate] callback notification period is the value set by the parameter.
             *
             * @param millisecond Monitoring time period of the audio spectrum, in milliseconds, has a value range of [1000, 10000]. Default is 2000 ms.
             */
            virtual void startPerformanceMonitor(unsigned int millisecond = 2000) = 0;

            /**
             * Stops system performance status monitoring.
             *
             * After the monitoring is stopped, the callback of the system performance status will be stopped.
             */
            virtual void stopPerformanceMonitor() = 0;

            /**
             * start network probe.
             *
             * Some local network problems may cause audio and video calls to fail. Using this function to probe the network protocols, assist in locating and solving related network problems.
             * The SDK internally detects http, tcp, and udp in sequence. If the probe fails in the middle, the subsequent detection will not continue. Therefore, when reading the values ​​in the probe result, please pay attention to check if the value is nullptr.
             * The SDK will not perform multiple network probe at the same time, that is, if the network probe is in progress, the SDK will not work if you call this method repeatedly.
             * Network probe may take a long time. Developers can call [stopNetworkProbe] to stop network probe as needed.
             *
             * @param config network probe config
             * @param callback Network probe result callback.
             */
            virtual void startNetworkProbe(ZegoNetworkProbeConfig config, ZegoNetworkProbeResultCallback callback) = 0;

            /**
             * stop network probe.
             */
            virtual void stopNetworkProbe() = 0;

            /**
             * Start network speed test.
             *
             * This function cannot be called together with [startPublishingStream], otherwise the network probe will automatically stop.
             * Developers can listen to the [onNetworkSpeedTestQualityUpdate] callback to get the speed test result, which will be called back every 3 seconds.
             * If an error occurs during the speed measurement process, [onNetworkSpeedTestError] callback will be triggered.
             * If this function is repeatedly called multiple times, the last invoke's configuration will be used.
             *
             * @param config Network speed test configuration.
             */
            virtual void startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config) = 0;

            /**
             * Stop network speed test.
             *
             * After stopping the speed test, [onNetworkSpeedTestQualityUpdate] will no longer call back.
             */
            virtual void stopNetworkSpeedTest() = 0;

            /**
             * Enable local capture custom audio processing.
             *
             * When enabled, developers can receive local captured audio frame through [onProcessCapturedAudioData], and can modify the audio data.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable local capture custom audio processing.
             * @param config Custom audio processing configuration.
             */
            virtual void enableCustomAudioCaptureProcessing(bool enable, ZegoCustomAudioProcessConfig* config) = 0;

            /**
             * Enable custom audio processing for remote playing stream.
             *
             * When enabled, developers can receive audio frame from remote playing stream through [onProcessRemoteAudioData], and can modify the audio data.
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable custom audio processing for remote playing stream.
             * @param config Custom audio processing configuration.
             */
            virtual void enableCustomAudioRemoteProcessing(bool enable, ZegoCustomAudioProcessConfig* config) = 0;

            /**
             * Set up callback handler for custom audio processing.
             *
             * Developers can modify the processing of audio frame data in the callback.
             *
             * @param handler Callback handler for custom audio processing.
             */
            virtual void setCustomAudioProcessHandler(std::shared_ptr<IZegoCustomAudioProcessHandler> handler) = 0;

            /**
             * Enables the callback for receiving audio data.
             *
             * The callback to the corresponding setting of [setAudioDataHandler] is triggered when this function is called and at publishing stream state or playing stream state. If you want to enable the [onPlayerAudioData] callback, the sample rate passed in by calling the [enableAudioDataCallback] function does not support 8000Hz, 22050Hz and 24000Hz.
             *
             * @param enable Whether to enable audio data callback
             * @param callbackBitMask The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
             * @param param param of audio frame
             */
            virtual void enableAudioDataCallback(bool enable, unsigned int callbackBitMask, ZegoAudioFrameParam param) = 0;

            /**
             * Enable audio data observering
             *
             * It will only be triggered after this function is called and the callback has been set by calling [setAudioDataHandler]. If you want to enable the [onPlayerAudioData] callback, you must also be playing stream, and the sampling rate passed in by calling the [startAudioDataObserver] function does not support 8000Hz, 22050Hz, and 24000Hz.
             *
             * @param observerBitMask The callback function bitmask marker for receive audio data, refer to enum [ZegoAudioDataCallbackBitMask], when this param converted to binary, 0b01 that means 1 << 0 for triggering [onCapturedAudioData], 0x10 that means 1 << 1 for triggering [onPlaybackAudioData], 0x100 that means 1 << 2 for triggering [onMixedAudioData], 0x1000 that means 1 << 3 for triggering [onPlayerAudioData]. The masks can be combined to allow different callbacks to be triggered simultaneously.
             * @param param param of audio frame
             */
            virtual void startAudioDataObserver(unsigned int observerBitMask, ZegoAudioFrameParam param) = 0;

            /**
             * Disable audio data observering
             */
            virtual void stopAudioDataObserver() = 0;

            /**
             * Sets up the event callback handler for receiving audio data.
             *
             * You can call this function to receive audio data from the SDK when you need to get the audio data of the remote user or get the data collected by the local microphone for other purposes (such as audio-only recording, audio-only third-party monitoring, and audio-only real-time analysis).
             * The set callback needs to be after the call to [enableAudioDataCallback] and in a publishing or playing stream state for the callback to work.
             * The format of the audio data callback by SDK is pcm. The SDK still controls the collection and playback of the sound device. This function is to copy a piece of data collected or played inside the SDK to the developer for use.
             *
             * @param handler Audio data handler for receive audio data
             */
            virtual void setAudioDataHandler(std::shared_ptr<IZegoAudioDataHandler> handler) = 0;

            /**
             * Enables the custom audio I/O function (for the specified channel).
             *
             * It needs to be invoked before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer] to take effect.
             *
             * @param enable Whether to enable custom audio IO, default is false
             * @param config Custom audio IO config
             * @param channel Specify the publish channel to enable custom audio IO
             */
            virtual void enableCustomAudioIO(bool enable, ZegoCustomAudioConfig* config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Sends AAC audio data produced by custom audio capture to the SDK (for the specified channel).
             *
             * @param data AAC buffer data
             * @param dataLength The total length of the buffer data
             * @param configLength The length of AAC specific config (Note: The AAC encoded data length is 'encodedLength = dataLength - configLength')
             * @param referenceTimeMillisecond The UNIX timestamp of this AAC audio frame in millisecond.
             * @param param The param of this AAC audio frame
             * @param channel Publish channel for capturing audio frames
             */
            virtual void sendCustomAudioCaptureAACData(unsigned char * data, unsigned int dataLength, unsigned int configLength, unsigned long long referenceTimeMillisecond, ZegoAudioFrameParam param, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Sends PCM audio data produced by custom audio capture to the SDK (for the specified channel).
             *
             * @param data PCM buffer data
             * @param dataLength The total length of the buffer data
             * @param param The param of this PCM audio frame
             * @param channel Publish channel for capturing audio frames
             */
            virtual void sendCustomAudioCapturePCMData(unsigned char * data, unsigned int dataLength, ZegoAudioFrameParam param, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) = 0;

            /**
             * Fetches PCM audio data of the remote stream for custom audio rendering.
             *
             * It is recommended to use the system framework to periodically invoke this function to drive audio data rendering
             *
             * @param data A block of memory for storing audio PCM data that requires user to manage the memory block's lifecycle, the SDK will copy the audio frame rendering data to this memory block
             * @param dataLength The length of the audio data to be fetch this time (dataLength = duration * sampleRate * channels * 2(16 bit depth i.e. 2 Btye))
             * @param param Specify the parameters of the fetched audio frame
             */
            virtual void fetchCustomAudioRenderPCMData(unsigned char * data, unsigned int dataLength, ZegoAudioFrameParam param) = 0;

            /**
             * Setting up the specific reverberation parameters.
             *
             * Different values dynamically set during publishing stream will take effect. When all parameters are set to 0, the reverberation is turned off.
             * This function is an advanced version of [setReverbPreset], you can configure the reverb effect by yourself.
             * If you need to configure the reverb/echo/voice changer effect, please use [setReverbAdvancedParam], [setReverbEchoParam], [setVoiceChangerParam] together.
             *
             * @deprecated This function has been deprecated since version 1.18.0, please use [setReverbAdvancedParam] instead.
             * @param param Reverb parameter
             */
            ZEGO_DEPRECATED
            virtual void setReverbParam(ZegoReverbParam param) = 0;

#if TARGET_OS_IPHONE || defined(ANDROID)
            /**
             * Whether to use the built-in speaker to play audio.This function has been deprecated since version 2.3.0. Please use [setAudioRouteToSpeaker] instead.
             *
             * When you choose not to use the built-in speaker to play sound, that is, set to false, the SDK will select the currently highest priority audio output device to play the sound according to the system schedule
             *
             * @deprecated This function has been deprecated since version 2.3.0. Please use [setAudioRouteToSpeaker] instead.
             * @param enable Whether to use the built-in speaker to play sound, true: use the built-in speaker to play sound, false: use the highest priority audio output device scheduled by the current system to play sound
             */
            ZEGO_DEPRECATED
            virtual void setBuiltInSpeakerOn(bool enable) = 0;
#endif

        };


        class IZegoAudioEffectPlayer
        {
        protected:

            virtual ~IZegoAudioEffectPlayer() {}

        public:

            /**
             * set audio effect player event handler.
             *
             * @param handler event handler for audio effect player
             */
            virtual void setEventHandler(std::shared_ptr<IZegoAudioEffectPlayerEventHandler> handler) = 0;

            /**
             * Start playing audio effect.
             *
             * The default is only played once and is not mixed into the publishing stream, if you want to change this please modify [config].
             *
             * @param audioEffectID ID for the audio effect. The SDK uses audioEffectID to control the playback of sound effects. The SDK does not force the user to pass in this parameter as a fixed value. It is best to ensure that each sound effect can have a unique id. The recommended methods are static self-incrementing id or the hash of the incoming sound effect file path.
             * @param path The absolute path of the local resource. "assets://"、"ipod-library://" and network url are not supported. Set path as nullptr or "" if resource is loaded already using [loadResource]
             * @param config Audio effect playback configuration. Set nullptr will only be played once, and will not be mixed into the publishing stream.
             */
            virtual void start(unsigned int audioEffectID, const std::string& path, ZegoAudioEffectPlayConfig* config = nullptr) = 0;

            /**
             * Stop playing audio effect.
             *
             * @param audioEffectID ID for the audio effect
             */
            virtual void stop(unsigned int audioEffectID) = 0;

            /**
             * Pause playing audio effect.
             *
             * @param audioEffectID ID for the audio effect
             */
            virtual void pause(unsigned int audioEffectID) = 0;

            /**
             * Resume playing audio effect.
             *
             * @param audioEffectID ID for the audio effect
             */
            virtual void resume(unsigned int audioEffectID) = 0;

            /**
             * Stop playing all audio effect.
             */
            virtual void stopAll() = 0;

            /**
             * Pause playing all audio effect.
             */
            virtual void pauseAll() = 0;

            /**
             * Resume playing all audio effect.
             */
            virtual void resumeAll() = 0;

            /**
             * Set the specified playback progress.
             *
             * Unit is millisecond
             *
             * @param audioEffectID ID for the audio effect
             * @param millisecond Point in time of specified playback progress
             * @param callback seek to result
             */
            virtual void seekTo(unsigned int audioEffectID, unsigned long long millisecond, ZegoAudioEffectPlayerSeekToCallback callback) = 0;

            /**
             * Set volume for the audio effect. Both the local play volume and the publish volume are set.
             *
             * @param audioEffectID ID for the audio effect
             * @param volume The range is 0 ~ 200. The default is 100.
             */
            virtual void setVolume(unsigned int audioEffectID, int volume) = 0;

            /**
             * Set volume for all audio effect. Both the local play volume and the publish volume are set.
             *
             * @param volume The range is 0 ~ 200. The default is 100.
             */
            virtual void setVolumeAll(int volume) = 0;

            /**
             * Get the total progress of your media resources.
             *
             * You should load resource before invoking this function, otherwise the return value is 0
             *
             * @param audioEffectID ID for the audio effect
             * @return Unit is millisecond
             */
            virtual unsigned long long getTotalDuration(unsigned int audioEffectID) = 0;

            /**
             * Get current playing progress.
             *
             * You should load resource before invoking this function, otherwise the return value is 0
             *
             * @param audioEffectID ID for the audio effect
             */
            virtual unsigned long long getCurrentProgress(unsigned int audioEffectID) = 0;

            /**
             * Load audio effect resource.
             *
             * In a scene where the same sound effect is played frequently, the SDK provides the function of preloading the sound effect file into the memory in order to optimize the performance of repeatedly reading and decoding the file. Preloading supports loading up to 15 sound effect files at the same time, and the duration of the sound effect files cannot exceed 30s, otherwise an error will be reported when loading
             *
             * @param audioEffectID ID for the audio effect
             * @param path the absolute path of the audio effect resource.
             * @param callback load audio effect resource result
             */
            virtual void loadResource(unsigned int audioEffectID, const std::string& path, ZegoAudioEffectPlayerLoadResourceCallback callback) = 0;

            /**
             * Unload audio effect resource.
             *
             * After the sound effects are used up, related resources can be released through this function; otherwise, the SDK will release the loaded resources when the AudioEffectPlayer instance is destroyed.
             *
             * @param audioEffectID ID for the audio effect loaded
             */
            virtual void unloadResource(unsigned int audioEffectID) = 0;

            /**
             * Get audio effect player index.
             *
             * @return Audio effect player index
             */
            virtual int getIndex() = 0;

        };
        /**
         * Zego MediaPlayer.
         *
         * Yon can use ZegoMediaPlayer to play media resource files on the local or remote server, and can mix the sound of the media resource files that are played into the publish stream to achieve the effect of background music.
         */
        class IZegoMediaPlayer
        {
        protected:

            virtual ~IZegoMediaPlayer() {}

        public:

            /**
             * Set event callback handler for media player.
             *
             * Developers can change the player UI widget according to the related event callback of the media player
             *
             * @param handler Media player event callback object
             */
            virtual void setEventHandler(std::shared_ptr<IZegoMediaPlayerEventHandler> handler) = 0;

            /**
             * Set audio callback handler.
             *
             * You can set this callback to throw the audio data of the media resource file played by the media player
             *
             * @param handler Audio event callback object for media player
             */
            virtual void setAudioHandler(std::shared_ptr<IZegoMediaPlayerAudioHandler> handler) = 0;

            /**
             * Load media resource.
             *
             * Yon can pass the absolute path of the local resource or the URL of the network resource
             *
             * @param path the absolute path of the local resource or the URL of the network resource
             * @param callback Notification of resource loading results
             */
            virtual void loadResource(const std::string& path, ZegoMediaPlayerLoadResourceCallback callback) = 0;

            /**
             * Start playing.
             *
             * You need to load resources before playing
             */
            virtual void start() = 0;

            /**
             * Stop playing.
             */
            virtual void stop() = 0;

            /**
             * Pause playing.
             */
            virtual void pause() = 0;

            /**
             * resume playing.
             */
            virtual void resume() = 0;

            /**
             * Set the specified playback progress.
             *
             * Unit is millisecond
             *
             * @param millisecond Point in time of specified playback progress
             * @param callback the result notification of set the specified playback progress
             */
            virtual void seekTo(unsigned long long millisecond, ZegoMediaPlayerSeekToCallback callback) = 0;

            /**
             * Whether to repeat playback.
             *
             * @param enable repeat playback flag. The default is false.
             */
            virtual void enableRepeat(bool enable) = 0;

            /**
             * Whether to mix the player's sound into the stream being published.
             *
             * @param enable Aux audio flag. The default is false.
             */
            virtual void enableAux(bool enable) = 0;

            /**
             * Whether to play locally silently.
             *
             * If [enableAux] switch is turned on, there is still sound in the publishing stream. The default is false.
             *
             * @param mute Mute local audio flag, The default is false.
             */
            virtual void muteLocal(bool mute) = 0;

            /**
             * Set mediaplayer volume. Both the local play volume and the publish volume are set.
             *
             * @param volume The range is 0 ~ 200. The default is 60.
             */
            virtual void setVolume(int volume) = 0;

            /**
             * Set mediaplayer local play volume.
             *
             * @param volume The range is 0 ~ 200. The default is 60.
             */
            virtual void setPlayVolume(int volume) = 0;

            /**
             * Set mediaplayer publish volume.
             *
             * @param volume The range is 0 ~ 200. The default is 60.
             */
            virtual void setPublishVolume(int volume) = 0;

            /**
             * Set playback progress callback interval.
             *
             * This function can control the callback frequency of [onMediaPlayerPlayingProgress]. When the callback interval is set to 0, the callback is stopped. The default callback interval is 1s
             * This callback are not returned exactly at the set callback interval, but rather at the frequency at which the audio or video frames are processed to determine whether the callback is needed to call
             *
             * @param millisecond Interval of playback progress callback in milliseconds
             */
            virtual void setProgressInterval(unsigned long long millisecond) = 0;

            /**
             * Gets the current local playback volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
             */
            virtual int getPlayVolume() = 0;

            /**
             * Gets the current publish volume of the mediaplayer, the range is 0 ~ 200, with the default value of 60.
             */
            virtual int getPublishVolume() = 0;

            /**
             * Get the total progress of your media resources.
             *
             * You should load resource before invoking this function, otherwise the return value is 0
             *
             * @return Unit is millisecond
             */
            virtual unsigned long long getTotalDuration() = 0;

            /**
             * Get current playing progress.
             *
             * You should load resource before invoking this function, otherwise the return value is 0
             */
            virtual unsigned long long getCurrentProgress() = 0;

            /**
             * Get the number of audio tracks of the playback file.
             */
            virtual unsigned int getAudioTrackCount() = 0;

            /**
             * Set the audio track of the playback file.
             *
             * @param index Audio track index, the number of audio tracks can be obtained through the [getAudioTrackCount] function.
             */
            virtual void setAudioTrackIndex(unsigned int index) = 0;

            /**
             * Setting up the specific voice changer parameters.
             *
             * @param audioChannel The audio channel to be voice changed
             * @param param Voice changer parameters
             */
            virtual void setVoiceChangerParam(ZegoMediaPlayerAudioChannel audioChannel, ZegoVoiceChangerParam param) = 0;

            /**
             * Get the current playback status.
             */
            virtual ZegoMediaPlayerState getCurrentState() = 0;

            /**
             * Get media player index.
             */
            virtual int getIndex() = 0;

            /**
             * Open precise seek and set relevant attributes.
             *
             * Call the setting before loading the resource. After setting, it will be valid throughout the life cycle of the media player. For multiple calls to ‘enableAccurateSeek’, the configuration is an overwrite relationship, and each call to ‘enableAccurateSeek’ only takes effect on the resources loaded later.
             *
             * @param enable Whether to enable accurate seek
             * @param config The property setting of precise seek is valid only when enable is true.
             */
            virtual void enableAccurateSeek(bool enable, ZegoAccurateSeekConfig* config) = 0;

            /**
             * Set the maximum cache duration and cache data size of web materials.
             *
             * The setting must be called before loading the resource, and it will take effect during the entire life cycle of the media player.
             * Time and size are not allowed to be 0 at the same time. The SDK internal default time is 5000, and the size is 15*1024*1024 byte.When one of time and size reaches the set value first, the cache will stop.
             *
             * @param time The maximum length of the cache time, in ms, the SDK internal default is 5000; the effective value is greater than or equal to 2000; if you fill in 0, it means no limit.
             * @param size The maximum size of the cache, the unit is byte, the internal default size of the SDK is 15*1024*1024 byte; the effective value is greater than or equal to 5000000, if you fill in 0, it means no limit.
             */
            virtual void setNetWorkResourceMaxCache(unsigned int time, unsigned int size) = 0;

            /**
             * Get the playable duration and size of the cached data of the current network material cache queue
             *
             * @return Returns the current cached information, including the length of time the data can be played and the size of the cached data.
             */
            virtual ZegoNetWorkResourceCache* getNetWorkResourceCache() = 0;

            /**
             * Use this interface to set the cache threshold that the media player needs to resume playback. The SDK default value is 5000ms，The valid value is greater than or equal to 1000ms
             *
             * @param threshold Threshold that needs to be reached to resume playback, unit ms.
             */
            virtual void setNetWorkBufferThreshold(unsigned int threshold) = 0;

        };


    }
}
#endif // __ZEGOEXPRESSINTERFACE_H__
