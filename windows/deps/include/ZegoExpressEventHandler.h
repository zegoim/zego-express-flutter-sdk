
#ifndef __ZEGOEXPRESSHANDLERS_H__
#define __ZEGOEXPRESSHANDLERS_H__

#include "ZegoExpressDefines.h"

namespace ZEGO {
    namespace EXPRESS {

        class IZegoMediaPlayer;
        class IZegoAudioEffectPlayer;


        class IZegoEventHandler
        {
        protected:

            virtual ~IZegoEventHandler() {}

        public:

            /**
             * The callback for obtaining debugging error information.
             *
             * When the SDK functions are not used correctly, the callback prompts for detailed error information, which is controlled by the [setDebugVerbose] function
             *
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param funcName Function name
             * @param info Detailed error information
             */
            virtual void onDebugError(int /*errorCode*/, const std::string& /*funcName*/, const std::string& /*info*/) {

            }

            /**
             * The callback triggered when the audio/video engine state changes.
             *
             * When the developer calls the function that enables audio and video related functions, such as calling [startPreview], [startPublishingStream], [startPlayingStream] and MediaPlayer related function, the audio/video engine will start; when all audio and video functions are stopped, the engine state will become stopped.
             * When the developer has been [loginRoom], once [logoutRoom] is called, the audio/video engine will stop (preview, publishing/playing stream, MediaPlayer and other audio and video related functions will also stop).
             *
             * @param state The audio/video engine state
             */
            virtual void onEngineStateUpdate(ZegoEngineState /*state*/) {

            }

            /**
             * The callback triggered when the room connection state changes.
             *
             * This callback is triggered when the connection status of the room changes, and the reason for the change is notified. Developers can use this callback to determine the status of the current user in the room. If the connection is being requested for a long time, the general probability is that the user's network is unstable.
             *
             * @param roomID Room ID, a string of up to 128 bytes in length.
             * @param state Changed room state
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param extendedData Extended Information with state updates. When the room login is successful, the key "room_session_id" can be used to obtain the unique RoomSessionID of each audio and video communication, which identifies the continuous communication from the first user in the room to the end of the audio and video communication. It can be used in scenarios such as call quality scoring and call problem diagnosis.
             */
            virtual void onRoomStateUpdate(const std::string& /*roomID*/, ZegoRoomState /*state*/, int /*errorCode*/, const std::string& /*extendedData*/) {

            }

            /**
             * The callback triggered when the number of other users in the room increases or decreases.
             *
             * Note that the callback is only triggered when the isUserStatusNotify parameter in the ZegoRoomConfig passed loginRoom function is true. Developers can use this callback to determine the situation of users in the room.
             * If developers need to use ZEGO room users notifications, please make sure that each login user sets isUserStatusNotify to true
             * When a user logs in to a room for the first time, other users already exist in this room, and a user list of the type of addition is received.
             * When the user is already in the room, other users in this room will trigger this callback to notify the changed users when they enter or exit the room.
             *
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param updateType Update type (add/delete)
             * @param userList List of users changed in the current room
             */
            virtual void onRoomUserUpdate(const std::string& /*roomID*/, ZegoUpdateType /*updateType*/, const std::vector<ZegoUser>& /*userList*/) {

            }

            /**
             * The callback triggered every 30 seconds to report the current number of online users.
             *
             * This function is called back every 30 seconds.
             * Developers can use this callback to show the number of user online in the current room.
             *
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param count Count of online users
             */
            virtual void onRoomOnlineUserCountUpdate(const std::string& /*roomID*/, int /*count*/) {

            }

            /**
             * (Deprecated, please use the callback with the same name with the [extendedData] parameter instead) The callback triggered when the number of streams published by the other users in the same room increases or decreases.
             *
             * When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
             * When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
             * Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
             *
             * @deprecated This callback function has been deprecated, please use the callback function with the same name with the [extendedData] parameter.
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param updateType Update type (add/delete)
             * @param streamList Updated stream list
             */
            ZEGO_DEPRECATED
            virtual void onRoomStreamUpdate(const std::string& /*roomID*/, ZegoUpdateType /*updateType*/, const std::vector<ZegoStream>& /*streamList*/) {

            }

            /**
             * The callback triggered when the number of streams published by the other users in the same room increases or decreases.
             *
             * When a user logs in to a room for the first time, there are other users in the room who are publishing streams, and will receive a stream list of the added type.
             * When the user is already in the room, other users in this room will trigger this callback to notify the changed stream list when adding or deleting streams.
             * Developers can use this callback to determine if there are other users in the same room who have added or stopped streaming, in order to implement active play stream [startPlayingStream] or active stop playing stream [stopPlayingStream], and use simultaneous Changes to Streaming render UI widget;
             *
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param updateType Update type (add/delete)
             * @param streamList Updated stream list
             * @param extendedData Extended information with stream updates.
             */
            virtual void onRoomStreamUpdate(const std::string& /*roomID*/, ZegoUpdateType /*updateType*/, const std::vector<ZegoStream>& /*streamList*/, const std::string& /*extendedData*/) {

            }

            /**
             * The callback triggered when there is an update on the extra information of the streams published by other users in the same room.
             *
             * When a user publishing the stream update the extra information of the stream in the same room, other users in the same room will receive the callback.
             * The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
             * Developers can synchronize variable content related to stream IDs based on stream additional information.
             *
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param streamList List of streams that the extra info was updated.
             */
            virtual void onRoomStreamExtraInfoUpdate(const std::string& /*roomID*/, const std::vector<ZegoStream>& /*streamList*/) {

            }

            /**
             * The callback triggered when there is an update on the extra information of the room.
             *
             * When a user update the room extra information, other users in the same room will receive the callback.
             *
             * @param roomID Room ID where the user is logged in, a string of up to 128 bytes in length.
             * @param roomExtraInfoList List of the extra info updated.
             */
            virtual void onRoomExtraInfoUpdate(const std::string& /*roomID*/, const std::vector<ZegoRoomExtraInfo>& /*roomExtraInfoList*/) {

            }

            /**
             * The callback triggered when the state of stream publishing changes.
             *
             * After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
             * You can roughly judge the user's uplink network status based on whether the state parameter is in [PUBLISH_REQUESTING].
             * The parameter [extendedData] is extended information with state updates. If you use ZEGO's CDN content distribution network, after the stream is successfully published, the keys of the content of this parameter are [flv_url_list], [rtmp_url_list], [hls_url_list]. These correspond to the publishing stream URLs of the flv, rtmp, and hls protocols.
             *
             * @param streamID Stream ID
             * @param state State of publishing stream
             * @param errorCode The error code corresponding to the status change of the publish stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param extendedData Extended information with state updates.
             */
            virtual void onPublisherStateUpdate(const std::string& /*streamID*/, ZegoPublisherState /*state*/, int /*errorCode*/, const std::string& /*extendedData*/) {

            }

            /**
             * Callback for current stream publishing quality.
             *
             * After calling the [startPublishingStream] successfully, the callback will be received every 3 seconds. Through the callback, the collection frame rate, bit rate, RTT, packet loss rate and other quality data of the published audio and video stream can be obtained, and the health of the publish stream can be monitored in real time.
             * You can monitor the health of the published audio and video streams in real time according to the quality parameters of the callback function, in order to show the uplink network status in real time on the device UI.
             * If you does not know how to use the parameters of this callback function, you can only pay attention to the [level] field of the [quality] parameter, which is a comprehensive value describing the uplink network calculated by SDK based on the quality parameters.
             *
             * @param streamID Stream ID
             * @param quality Publishing stream quality, including audio and video framerate, bitrate, RTT, etc.
             */
            virtual void onPublisherQualityUpdate(const std::string& /*streamID*/, const ZegoPublishStreamQuality& /*quality*/) {

            }

            /**
             * The callback triggered when the first audio frame is captured.
             *
             * After the [startPublishingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
             * In the case of no startPublishingStream audio and video stream or preview, the first startPublishingStream audio and video stream or first preview, that is, when the engine of the audio and video module inside SDK starts, it will collect audio data of the local device and receive this callback.
             * Developers can use this callback to determine whether SDK has actually collected audio data. If the callback is not received, the audio capture device is occupied or abnormal.
             */
            virtual void onPublisherCapturedAudioFirstFrame() {

            }

            /**
             * The callback triggered when the state of relayed streaming to CDN changes.
             *
             * After the ZEGO RTC server relays the audio and video streams to the CDN, this callback will be received if the CDN relay status changes, such as a stop or a retry.
             * Developers can use this callback to determine whether the audio and video streams of the relay CDN are normal. If they are abnormal, further locate the cause of the abnormal audio and video streams of the relay CDN and make corresponding disaster recovery strategies.
             * If you do not understand the cause of the abnormality, you can contact ZEGO technicians to analyze the specific cause of the abnormality.
             *
             * @param streamID Stream ID
             * @param infoList List of information that the current CDN is relaying
             */
            virtual void onPublisherRelayCDNStateUpdate(const std::string& /*streamID*/, const std::vector<ZegoStreamRelayCDNInfo>& /*infoList*/) {

            }

            /**
             * The callback triggered when the state of stream playing changes.
             *
             * After publishing the stream successfully, the notification of the publish stream state change can be obtained through the callback function.
             * You can roughly judge the user's downlink network status based on whether the state parameter is in [PLAY_REQUESTING].
             *
             * @param streamID stream ID
             * @param state State of playing stream
             * @param errorCode The error code corresponding to the status change of the playing stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param extendedData Extended Information with state updates. As the standby, only an empty json table is currently returned
             */
            virtual void onPlayerStateUpdate(const std::string& /*streamID*/, ZegoPlayerState /*state*/, int /*errorCode*/, const std::string& /*extendedData*/) {

            }

            /**
             * Callback for current stream playing quality.
             *
             * After calling the [startPlayingStream] successfully, this callback will be triggered every 3 seconds. The collection frame rate, bit rate, RTT, packet loss rate and other quality data can be obtained, such the health of the publish stream can be monitored in real time.
             * You can monitor the health of the played audio and video streams in real time according to the quality parameters of the callback function, in order to show the downlink network status on the device UI in real time.
             * If you does not know how to use the various parameters of the callback function, you can only focus on the level field of the quality parameter, which is a comprehensive value describing the downlink network calculated by SDK based on the quality parameters.
             *
             * @param streamID Stream ID
             * @param quality Playing stream quality, including audio and video framerate, bitrate, RTT, etc.
             */
            virtual void onPlayerQualityUpdate(const std::string& /*streamID*/, const ZegoPlayStreamQuality& /*quality*/) {

            }

            /**
             * The callback triggered when a media event occurs during streaming playing.
             *
             * This callback is triggered when an event such as audio and video jamming and recovery occurs in the playing stream.
             * You can use this callback to make statistics on stutters or to make friendly displays in the UI of the app.
             *
             * @param streamID Stream ID
             * @param event Specific events received when playing the stream.
             */
            virtual void onPlayerMediaEvent(const std::string& /*streamID*/, ZegoPlayerMediaEvent /*event*/) {

            }

            /**
             * The callback triggered when the first audio frame is received.
             *
             * After the [startPlayingStream] function is called successfully, this callback will be called when SDK received the first frame of audio data.
             *
             * @param streamID Stream ID
             */
            virtual void onPlayerRecvAudioFirstFrame(const std::string& /*streamID*/) {

            }

            /**
             * The callback triggered when Supplemental Enhancement Information is received.
             *
             * After the remote stream is successfully played, when the remote stream sends SEI (such as directly calling [sendSEI], audio mixing with SEI data, and sending custom video capture encoded data with SEI, etc.), the local end will receive this callback.
             * Since the video encoder itself generates an SEI with a payload type of 5, or when a video file is used for publishing, such SEI may also exist in the video file. Therefore, if the developer needs to filter out this type of SEI, it can be before [createEngine] Call [ZegoEngineConfig.advancedConfig("unregister_sei_filter", "XXXXX")]. Among them, unregister_sei_filter is the key, and XXXXX is the uuid filter string to be set.
             *
             * @param streamID Stream ID
             * @param data SEI content
             * @param dataLength SEI content length.
             */
            virtual void onPlayerRecvSEI(const std::string& /*streamID*/, const unsigned char* /*data*/, unsigned int /*dataLength*/) {

            }

            /**
             * The callback triggered when the state of relayed streaming of the mixed stream to CDN changes.
             *
             * In the general case of the ZEGO RTC server's stream mixing task, the output stream is published to the CDN using the RTMP protocol, and changes in the state during the publish will be notified from this callback function.
             *
             * @param taskID Stream mixing task ID
             * @param infoList List of information that the current CDN is being mixed
             */
            virtual void onMixerRelayCDNStateUpdate(const std::string& /*taskID*/, const std::vector<ZegoStreamRelayCDNInfo>& /*infoList*/) {

            }

            /**
             * The callback triggered when the sound level of any input stream changes in the stream mixing process.
             *
             * You can use this callback to show the effect of the anchors sound level when the audience plays the mixed stream. So audience can notice which anchor is speaking.
             *
             * @param soundLevels Sound level hash map, key is the soundLevelID of every single stream in this mixer stream, value is the sound level value of that single stream, value ranging from 0.0 to 100.0
             */
            virtual void onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& /*soundLevels*/) {

            }

            /**
             * The callback triggered when there is a change to audio devices (i.e. new device added or existing device deleted).
             *
             * This callback is triggered when an audio device is added or removed from the system. By listening to this callback, users can update the sound collection or output using a specific device when necessary.
             *
             * @param updateType Update type (add/delete)
             * @param deviceType Audio device type
             * @param deviceInfo Audio device information
             */
            virtual void onAudioDeviceStateChanged(ZegoUpdateType /*updateType*/, ZegoAudioDeviceType /*deviceType*/, const ZegoDeviceInfo& /*deviceInfo*/) {

            }

            /**
             * The callback triggered when there is a change of the volume fo the audio devices.
             *
             * @param deviceType Audio device type
             * @param deviceID Audio device ID
             * @param volume audio device volume
             */
            virtual void onAudioDeviceVolumeChanged(ZegoAudioDeviceType /*deviceType*/, const std::string& /*deviceID*/, int /*volume*/) {

            }

            /**
             * The local captured audio sound level callback.
             *
             * To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
             * The callback notification period is the setting parameter of [startSoundLevelMonitor].
             *
             * @param soundLevel Locally captured sound level value, ranging from 0.0 to 100.0
             */
            virtual void onCapturedSoundLevelUpdate(float /*soundLevel*/) {

            }

            /**
             * The remote playing streams audio sound level callback.
             *
             * To trigger this callback function, the [startSoundLevelMonitor] function must be called to start the sound level monitor and you must be in a state where it is playing the audio and video stream.
             * The callback notification period is the setting parameter of [startSoundLevelMonitor].
             *
             * @param soundLevels Remote sound level hash map, key is the streamID, value is the sound level value of the corresponding streamID, value ranging from 0.0 to 100.0
             */
            virtual void onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& /*soundLevels*/) {

            }

            /**
             * The local captured audio spectrum callback.
             *
             * To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is publishing the audio and video stream or be in [startPreview] state.
             * The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
             *
             * @param audioSpectrum Locally captured audio spectrum value list. Spectrum value range is [0-2^30]
             */
            virtual void onCapturedAudioSpectrumUpdate(const ZegoAudioSpectrum& /*audioSpectrum*/) {

            }

            /**
             * The remote playing streams audio spectrum callback.
             *
             * To trigger this callback function, the [startAudioSpectrumMonitor] function must be called to start the audio spectrum monitor and you must be in a state where it is playing the audio and video stream.
             * The callback notification period is the setting parameter of [startAudioSpectrumMonitor].
             *
             * @param audioSpectrums Remote audio spectrum hash map, key is the streamID, value is the audio spectrum list of the corresponding streamID. Spectrum value range is [0-2^30]
             */
            virtual void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, ZegoAudioSpectrum>& /*audioSpectrums*/) {

            }

            /**
             * The callback triggered when a device exception occurs.
             *
             * This callback is triggered when an exception occurs when reading or writing the audio and video device.
             *
             * @param errorCode The error code corresponding to the status change of the playing stream, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param deviceName device name
             */
            virtual void onDeviceError(int /*errorCode*/, const std::string& /*deviceName*/) {

            }

            /**
             * The callback triggered when the state of the remote microphone changes.
             *
             * When the state of the remote microphone device is changed, such as switching a microphone, etc., by listening to the callback, it is possible to obtain an event related to the remote microphone, which can be used to prompt the user that the audio may be abnormal.
             * Developers of 1v1 education scenarios or education small class scenarios and similar scenarios can use this callback notification to determine whether the microphone device of the remote publishing stream device is working normally, and preliminary understand the cause of the device problem according to the corresponding state.
             * This callback will not be called back when the remote stream is play from the CDN, and will not be called back if the remote stream end user has enabled custom audio capture function.
             *
             * @param streamID Stream ID
             * @param state Remote microphone status
             */
            virtual void onRemoteMicStateUpdate(const std::string& /*streamID*/, ZegoRemoteDeviceState /*state*/) {

            }

#if TARGET_OS_IPHONE || defined(ANDROID)
            /**
             * Callback for device's audio route changed.
             *
             * This callback will be called when there are changes in audio routing such as earphone plugging, speaker and receiver switching, etc.
             *
             * @param audioRoute Current audio route
             */
            virtual void onAudioRouteChange(ZegoAudioRoute /*audioRoute*/) {

            }
#endif

            /**
             * The callback triggered when Broadcast Messages are received.
             *
             * This callback is used to receive broadcast messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
             *
             * @param roomID Room ID
             * @param messageList list of received messages.
             */
            virtual void onIMRecvBroadcastMessage(const std::string& /*roomID*/, std::vector<ZegoBroadcastMessageInfo> /*messageList*/) {

            }

            /**
             * The callback triggered when Barrage Messages are received.
             *
             * This callback is used to receive barrage messages sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
             *
             * @param roomID Room ID
             * @param messageList list of received messages.
             */
            virtual void onIMRecvBarrageMessage(const std::string& /*roomID*/, std::vector<ZegoBarrageMessageInfo> /*messageList*/) {

            }

            /**
             * The callback triggered when a Custom Command is received.
             *
             * This callback is used to receive custom signaling sent by other users, and barrage messages sent by users themselves will not be notified through this callback.
             *
             * @param roomID Room ID
             * @param fromUser Sender of the command
             * @param command Command content received
             */
            virtual void onIMRecvCustomCommand(const std::string& /*roomID*/, ZegoUser /*fromUser*/, const std::string& /*command*/) {

            }

            /**
             * The system performance status callback.
             *
             * To trigger this callback function, the [startPerformanceMonitor] function must be called to start the system performance monitor.
             * The callback notification period is the setting parameter of [startPerformanceMonitor].
             *
             * @param status The system performance status.
             */
            virtual void onPerformanceStatusUpdate(const ZegoPerformanceStatus& /*status*/) {

            }

            /**
             * Callback for network mode changed.
             *
             * This callback will be called when the device's network mode changes, such as switching from WiFi to 5G, or when the network is disconnected.
             *
             * @param mode Current network mode.
             */
            virtual void onNetworkModeChanged(ZegoNetworkMode /*mode*/) {

            }

            /**
             * The callback triggered when error occurred when testing network speed.
             *
             * @param errorCode The error code corresponding to the network speed test, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param type Uplink or downlink
             */
            virtual void onNetworkSpeedTestError(int /*errorCode*/, ZegoNetworkSpeedTestType /*type*/) {

            }

            /**
             * The callback triggered when quality updated when testing network speed.
             *
             * When error occurs or called stopNetworkSpeedTest, this callback will be stopped.
             *
             * @param quality Network speed quality
             * @param type Uplink or downlink
             */
            virtual void onNetworkSpeedTestQualityUpdate(const ZegoNetworkSpeedTestQuality& /*quality*/, ZegoNetworkSpeedTestType /*type*/) {

            }

        };


        class IZegoApiCalledEventHandler
        {
        protected:

            virtual ~IZegoApiCalledEventHandler() {}

        public:

            /**
             * Method execution result callback
             *
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param funcName Function name
             * @param info Detailed error information
             */
            virtual void onApiCalledResult(int /*errorCode*/, const std::string& /*funcName*/, const std::string& /*info*/) {

            }

        };


        class IZegoAudioMixingHandler
        {
        protected:

            virtual ~IZegoAudioMixingHandler() {}

        public:

            /**
             * The callback for copying audio data to the SDK for audio mixing. This function should be used together with enableAudioMixing.
             *
             * Supports 16k 32k 44.1k 48k sample rate, mono or dual channel, 16-bit deep PCM audio data
             * This callback is a high frequency callback. To ensure the quality of the mixing data, please do not handle time-consuming operations in this callback
             *
             * @param data audio mixing data
             */
            virtual void onAudioMixingCopyData(ZegoAudioMixingData* /*data*/) {

            }

        };


        class IZegoMediaPlayerEventHandler
        {
        protected:

            virtual ~IZegoMediaPlayerEventHandler() {}

        public:

            /**
             * The callback triggered when the state of the media player changes.
             *
             * @param mediaPlayer Callback player object
             * @param state Media player status
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             */
            virtual void onMediaPlayerStateUpdate(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerState /*state*/, int /*errorCode*/) {

            }

            /**
             * The callback triggered when the network status of the media player changes.
             *
             * @param mediaPlayer Callback player object
             * @param networkEvent Network status event
             */
            virtual void onMediaPlayerNetworkEvent(IZegoMediaPlayer* /*mediaPlayer*/, ZegoMediaPlayerNetworkEvent /*networkEvent*/) {

            }

            /**
             * The callback to report the current playback progress of the media player.
             *
             * @param mediaPlayer Callback player object
             * @param millisecond Progress in milliseconds
             */
            virtual void onMediaPlayerPlayingProgress(IZegoMediaPlayer* /*mediaPlayer*/, unsigned long long /*millisecond*/) {

            }

            /**
             * The callback triggered when the media player got media side info.
             *
             * @param mediaPlayer Callback player object
             * @param data SEI content
             * @param dataLength SEI content length.
             */
            virtual void onMediaPlayerRecvSEI(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char* /*data*/, unsigned int /*dataLength*/) {

            }

        };


        class IZegoMediaPlayerVideoHandler
        {
        protected:

            virtual ~IZegoMediaPlayerVideoHandler() {}

        public:

            /**
             * The callback triggered when the media player throws out video frame data.
             *
             * @param mediaPlayer Callback player object
             * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2])
             * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2])
             * @param param video data frame param
             */
            virtual void onVideoFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/) {

            }

        };


        class IZegoMediaPlayerAudioHandler
        {
        protected:

            virtual ~IZegoMediaPlayerAudioHandler() {}

        public:

            /**
             * The callback triggered when the media player throws out audio frame data.
             *
             * @param mediaPlayer Callback player object
             * @param data Raw data of audio frames
             * @param dataLength Data length
             * @param param audio frame flip mode
             */
            virtual void onAudioFrame(IZegoMediaPlayer* /*mediaPlayer*/, const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {

            }

        };


        class IZegoAudioEffectPlayerEventHandler
        {
        protected:

            virtual ~IZegoAudioEffectPlayerEventHandler() {}

        public:

            /**
             * Audio effect playback state callback.
             *
             * This callback is triggered when the playback state of a audio effect of the audio effect player changes.
             *
             * @param audioEffectPlayer Audio effect player instance that triggers this callback
             * @param audioEffectID The ID of the audio effect resource that triggered this callback
             * @param state The playback state of the audio effect
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             */
            virtual void onAudioEffectPlayStateUpdate(IZegoAudioEffectPlayer* /*audioEffectPlayer*/, unsigned int /*audioEffectID*/, ZegoAudioEffectPlayState /*state*/, int /*errorCode*/) {

            }

        };


        class IZegoDataRecordEventHandler
        {
        protected:

            virtual ~IZegoDataRecordEventHandler() {}

        public:

            /**
             * The callback triggered when the state of data recording (to a file) changes.
             *
             * @param state File recording status, according to which you should determine the state of the file recording or the prompt of the UI.
             * @param errorCode Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
             * @param config Record config
             * @param channel Publishing stream channel
             */
            virtual void onCapturedDataRecordStateUpdate(ZegoDataRecordState /*state*/, int /*errorCode*/, ZegoDataRecordConfig /*config*/, ZegoPublishChannel /*channel*/) {

            }

            /**
             * The callback to report the current recording progress.
             *
             * @param progress File recording progress, which allows developers to hint at the UI, etc.
             * @param config Record config
             * @param channel Publishing stream channel
             */
            virtual void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress /*progress*/, ZegoDataRecordConfig /*config*/, ZegoPublishChannel /*channel*/) {

            }

        };


        class IZegoCustomVideoCaptureHandler
        {
        protected:

            virtual ~IZegoCustomVideoCaptureHandler() {}

        public:

            /**
             * The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
             *
             * @param channel Publishing stream channel
             */
            virtual void onStart(ZegoPublishChannel channel) = 0;

            /**
             * The callback triggered when SDK stops receiving captured video data.
             *
             * @param channel Publishing stream channel
             */
            virtual void onStop(ZegoPublishChannel channel) = 0;

            /**
             * SDK detects network changes and informs developers that it needs to do traffic control
             * In the case of custom video capture by sending encoded data, the SDK cannot know the external encoding configuration, so the traffic control operation needs to be completed by the developer.
             * The SDK will notify the developer of the recommended value of the video configuration according to the current network situation, and the developer needs to modify the encoder configuration by himself to ensure the smoothness of video transmission
             * Please do not perform time-consuming operations in this callback. If you need to perform time-consuming operations, please switch threads
             *
             * @param trafficControlInfo traffic control info
             * @param channel Publishing stream channel
             */
            virtual void onEncodedDataTrafficControl(ZegoTrafficControlInfo /*trafficControlInfo*/, ZegoPublishChannel /*channel*/) {

            }

        };


        class IZegoCustomVideoRenderHandler
        {
        protected:

            virtual ~IZegoCustomVideoRenderHandler() {}

        public:

            /**
             * The callback for obtaining the locally captured video frames (Raw Data).
             *
             * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2])
             * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2])
             * @param param Video frame parameters
             * @param flipMode video flip mode
             * @param channel Publishing stream channel
             */
            virtual void onCapturedVideoFrameRawData(unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/, ZegoVideoFlipMode /*flipMode*/, ZegoPublishChannel /*channel*/) {

            }

            /**
             * The callback for obtaining the video frames (Raw Data) of the remote stream. Different streams can be identified by streamID.
             *
             * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2])
             * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2])
             * @param param Video frame parameters
             * @param streamID Stream ID
             */
            virtual void onRemoteVideoFrameRawData(unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/, const std::string& /*streamID*/) {

            }

            /**
             * The callback for obtianing the video frames (Encoded Data) of the remote stream. Different streams can be identified by streamID.
             *
             * @param data Encoded data of video frames
             * @param dataLength Data length
             * @param param Video frame parameters
             * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
             * @param streamID Stream ID
             */
            virtual void onRemoteVideoFrameEncodedData(const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoVideoEncodedFrameParam /*param*/, unsigned long long /*referenceTimeMillisecond*/, const std::string& /*streamID*/) {

            }

        };


        class IZegoCustomVideoProcessHandler
        {
        protected:

            virtual ~IZegoCustomVideoProcessHandler() {}

        public:

            /**
             * Call back when the raw video data of type [RawData] is obtained.
             *
             * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA]
             * After the developer has processed the original image, he must call [sendCustomVideoProcessedRawData] to send the processed data back to the SDK, otherwise it will cause frame loss
             * Precondition：call [setCustomVideoProcessHandler] to set callback
             * Supported version：2.2.0
             *
             * @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2]
             * @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2]
             * @param param Video frame parameters
             * @param referenceTimeMillisecond Video frame reference time, UNIX timestamp, in milliseconds.
             * @param channel Publishing stream channel
             */
            virtual void onCapturedUnprocessedRawData(const unsigned char** /*data*/, unsigned int* /*dataLength*/, ZegoVideoFrameParam /*param*/, unsigned long long /*referenceTimeMillisecond*/, ZegoPublishChannel /*channel*/) {

            }

            /**
             * Call back when the original video data of type [CVPixelBuffer] is obtained
             *
             * This callback takes effect when [enableCustomVideoProcessing] is called to enable custom video processing and the bufferType of config is passed in [ZEGO_VIDEO_BUFFER_TYPE_CVPIXELBUFFER]
             * After the developer has processed the original image, he must call [sendCustomVideoProcessedCVPixelbuffer] to send the processed data back to the SDK, otherwise it will cause frame loss
             * Precondition： call [setCustomVideoProcessHandler] to set callback
             * Supported version： 2.2.0
             *
             * @param buffer CVPixelBufferRef type data
             * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
             * @param channel Publishing stream channel
             */
            virtual void onCapturedUnprocessedCVPixelBuffer(void * /*buffer*/, unsigned long long /*referenceTimeMillisecond*/, ZegoPublishChannel /*channel*/) {

            }

        };


        class IZegoCustomAudioProcessHandler
        {
        protected:

            virtual ~IZegoCustomAudioProcessHandler() {}

        public:

            /**
             * Custom audio processing local captured PCM audio frame callback.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             */
            virtual void onProcessCapturedAudioData(unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam* /*param*/) {

            }

            /**
             * Custom audio processing remote playing stream PCM audio frame callback.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             * @param streamID Corresponding stream ID
             */
            virtual void onProcessRemoteAudioData(unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam* /*param*/, const std::string& /*streamID*/) {

            }

        };


        class IZegoAudioDataHandler
        {
        protected:

            virtual ~IZegoAudioDataHandler() {}

        public:

            /**
             * The callback for obtaining the audio data captured by the local microphone.
             *
             * In non-custom audio capture mode, the SDK capture the microphone's sound, but the developer may also need to get a copy of the audio data captured by the SDK is available through this callback.
             * On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [enableAudioDataCallback] to set the mask 0b01 that means 1 << 0, this callback will be triggered only when it is in the publishing stream state.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             */
            virtual void onCapturedAudioData(const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {

            }

            /**
             * The callback for obtaining the audio data of all the streams playback by SDK.
             *
             * This function will callback all the mixed audio data to be playback. This callback can be used for that you needs to fetch all the mixed audio data to be playback to proccess.
             * On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [enableAudioDataCallback] to set the mask 0b100 that means 1 << 2, this callback will be triggered only when it is in the SDK inner audio and video engine started(called the [startPreivew] or [startPlayingStream] or [startPublishingStream]).
             * When the engine is started in the non-playing stream state or the media player is not used to play the media file, the audio data to be called back is muted audio data.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             */
            virtual void onPlaybackAudioData(const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {

            }

            /**
             * The callback for obtaining the mixed audio data. Such mixed auido data are generated by the SDK by mixing the audio data of all the remote playing streams and the auido data captured locally.
             *
             * The audio data of all playing data is mixed with the data captured by the local microphone before it is sent to the loudspeaker, and calleback out in this way.
             * On the premise of calling [setAudioDataHandler] to set the listener callback, after calling [enableAudioDataCallback] to set the mask 0x04, this callback will be triggered only when it is in the publishing stream state or playing stream state.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             */
            virtual void onMixedAudioData(const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/) {

            }

            /**
             * The callback for obtaining the audio data of each stream.
             *
             * This function will call back the data corresponding to each playing stream. Different from [onPlaybackAudioData], the latter is the mixed data of all playing streams. If developers need to process a piece of data separately, they can use this callback.
             * On the premise of calling [setAudioDataHandler] to set up listening for this callback, calling [enableAudioDataCallback] to set the mask 0x08 that is 1 << 3, and this callback will be triggered when the SDK audio and video engine starts to play the stream.
             *
             * @param data Audio data in PCM format
             * @param dataLength Length of the data
             * @param param Parameters of the audio frame
             * @param streamID Corresponding stream ID
             */
            virtual void onPlayerAudioData(const unsigned char* /*data*/, unsigned int /*dataLength*/, ZegoAudioFrameParam /*param*/, const std::string& /*streamID*/) {

            }

        };

    } //namespace EXPRESS
} //namespace ZEGO

#endif // __ZEGOEXPRESSHANDLERS_H__
