#pragma once

#include "../ZegoExpressDefines.h"
#include "../ZegoExpressErrorCode.h"
#include "../ZegoExpressEventHandler.h"
#include "ZegoInternalBridge.h"

namespace ZEGO
{
    namespace EXPRESS
    {

#define declearSingleRawMember(MemberType)                                \
    MemberType m##MemberType = nullptr;                                   \
    std::recursive_mutex m##MemberType##Mutex;                            \
                                                                          \
    void set##MemberType(MemberType member)                               \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        m##MemberType = member;                                           \
    }                                                                     \
                                                                          \
    MemberType get##MemberType()                                          \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        return m##MemberType;                                             \
    }

#define declearSingleShareMember(MemberType)                              \
    std::shared_ptr<MemberType> m##MemberType;                            \
    std::recursive_mutex m##MemberType##Mutex;                            \
                                                                          \
    void set##MemberType(std::shared_ptr<MemberType> member)              \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        m##MemberType = member;                                           \
    }                                                                     \
                                                                          \
    std::shared_ptr<MemberType> get##MemberType()                         \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        return m##MemberType;                                             \
    }

#define declearMultiRawMember(KeyType, MemberType)                        \
    std::unordered_map<KeyType, MemberType> m##MemberType;                \
    std::recursive_mutex m##MemberType##Mutex;                            \
                                                                          \
    void insert##MemberType(KeyType seq, MemberType member)               \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        m##MemberType.insert({seq, member});                              \
    }                                                                     \
                                                                          \
    MemberType get##MemberType(KeyType seq)                               \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        MemberType member = nullptr;                                      \
        if (m##MemberType.count(seq) > 0)                                 \
        {                                                                 \
            member = m##MemberType[seq];                                  \
        }                                                                 \
        return member;                                                    \
    }                                                                     \
                                                                          \
    MemberType erase##MemberType(KeyType seq)                             \
    {                                                                     \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex); \
        MemberType member = get##MemberType(seq);                         \
        m##MemberType.erase(seq);                                         \
        return member;                                                    \
    }

#define declearMultiShareMember(MemberType)                                   \
    std::unordered_map<zego_seq, std::shared_ptr<MemberType>> m##MemberType;  \
    std::recursive_mutex m##MemberType##Mutex;                                \
                                                                              \
    void insert##MemberType(zego_seq seq, std::shared_ptr<MemberType> member) \
    {                                                                         \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);     \
        m##MemberType.insert({seq, member});                                  \
    }                                                                         \
                                                                              \
    std::shared_ptr<MemberType> get##MemberType(zego_seq seq)                 \
    {                                                                         \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);     \
        std::shared_ptr<MemberType> member = nullptr;                         \
        if (m##MemberType.count(seq) > 0)                                     \
        {                                                                     \
            member = m##MemberType[seq];                                      \
        }                                                                     \
        return member;                                                        \
    }                                                                         \
                                                                              \
    std::shared_ptr<MemberType> erase##MemberType(zego_seq seq)               \
    {                                                                         \
        std::lock_guard<std::recursive_mutex> lock(m##MemberType##Mutex);     \
        std::shared_ptr<MemberType> member = get##MemberType(seq);            \
        m##MemberType.erase(seq);                                             \
        return member;                                                        \
    }

        using ZegoVoidPtr = void *;
        using ZegoUInt = unsigned int;

        template <typename T>
        class ZegoSingleton
        {
        public:
            template <typename... Args>
            static T *CreateInstance(Args &&... args)
            {
                if (m_pInstance == nullptr)
                {
                    m_pInstance = new T(std::forward<Args>(args)...);
                }
                return m_pInstance;
            }

            static T *GetInstance()
            {
                return m_pInstance;
            }

            static void DestroyInstance()
            {
                if (m_pInstance)
                {
                    delete m_pInstance;
                    m_pInstance = nullptr;
                }
            }

        private:
            ZegoSingleton(void) = default;
            virtual ~ZegoSingleton(void) = default;

        private:
            static T *m_pInstance;
        };
        template <class T>
        T *ZegoSingleton<T>::m_pInstance = nullptr;

        class ZegoExpressConvert
        {
        public:
            static ZegoUser I2OUser(const zego_user &_user)
            {
                ZegoUser user;
                user.userID = _user.user_id;
                user.userName = _user.user_name;
                return user;
            }

            static zego_user O2IUser(const ZegoUser &user)
            {
                zego_user _user;
                memset(_user.user_id, 0, sizeof(_user.user_id));
                memset(_user.user_name, 0, sizeof(_user.user_name));
                ZegoStrncpy(_user.user_id, user.userID.c_str(), ZEGO_EXPRESS_MAX_USERID_LEN);
                ZegoStrncpy(_user.user_name, user.userName.c_str(), ZEGO_EXPRESS_MAX_USERNAME_LEN);
                return _user;
            }

            static zego_room_config O2IRoomConfig(const ZegoRoomConfig &config)
            {
                zego_room_config _config;
                _config.max_member_count = config.maxMemberCount;
                _config.is_user_status_notify = config.isUserStatusNotify;
                memset(_config.token, 0, sizeof(_config.token));
                ZegoStrncpy(_config.token, config.token.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
                return _config;
            }

            static ZegoStream I2OStream(const zego_stream &_stream)
            {
                ZegoStream stream;
                stream.user.userID = _stream.user.user_id;
                stream.user.userName = _stream.user.user_name;
                stream.streamID = _stream.stream_id;
                stream.extraInfo = _stream.extra_info;
                return stream;
            }

            static ZegoRoomExtraInfo I2ORoomExtraInfo(const zego_room_extra_info &_info)
            {
                ZegoRoomExtraInfo info;
                info.key = _info.key;
                info.value = _info.value;
                info.updateUser.userID = _info.update_user.user_id;
                info.updateUser.userName = _info.update_user.user_name;
                info.updateTime = _info.update_time;
                return info;
            }

            static zego_canvas O2ICanvas(const ZegoCanvas &canvas)
            {
                zego_canvas _canvas;
                _canvas.view = canvas.view;
                _canvas.view_mode = zego_view_mode(canvas.viewMode);
                _canvas.background_color = canvas.backgroundColor;
                return _canvas;
            }

            static ZegoPublishStreamQuality I2OPushlishQuality(const zego_publish_stream_quality &_quality)
            {
                ZegoPublishStreamQuality quality;

                quality.videoCaptureFPS = _quality.video_capture_fps;
                quality.videoEncodeFPS = _quality.video_encode_fps;
                quality.videoSendFPS = _quality.video_send_fps;
                quality.videoKBPS = _quality.video_kbps;

                quality.audioCaptureFPS = _quality.audio_capture_fps;
                quality.audioSendFPS = _quality.audio_send_fps;
                quality.audioKBPS = _quality.audio_kbps;

                quality.rtt = _quality.rtt;
                quality.packetLostRate = _quality.packet_lost_rate;
                quality.level = ZegoStreamQualityLevel(_quality.level);
                quality.isHardwareEncode = _quality.is_hardware_encode;
                quality.videoCodecID = ZegoVideoCodecID(_quality.video_codec_id);

                quality.totalSendBytes = _quality.total_send_bytes;
                quality.audioSendBytes = _quality.audio_send_bytes;
                quality.videoSendBytes = _quality.video_send_bytes;

                return quality;
            }

            static ZegoPlayStreamQuality I2OPlayQuality(const zego_play_stream_quality &_quality)
            {
                ZegoPlayStreamQuality quality;
                quality.videoRecvFPS = _quality.video_recv_fps;
                quality.videoDejitterFPS = _quality.video_dejitter_fps;
                quality.videoDecodeFPS = _quality.video_decode_fps;
                quality.videoRenderFPS = _quality.video_render_fps;
                quality.videoKBPS = _quality.video_kbps;
                quality.videoBreakRate = _quality.video_break_rate;

                quality.audioRecvFPS = _quality.audio_recv_fps;
                quality.audioDejitterFPS = _quality.audio_dejitter_fps;
                quality.audioDecodeFPS = _quality.audio_decode_fps;
                quality.audioRenderFPS = _quality.audio_render_fps;
                quality.audioKBPS = _quality.audio_kbps;
                quality.audioBreakRate = _quality.audio_break_rate;

                quality.rtt = _quality.rtt;
                quality.packetLostRate = _quality.packet_lost_rate;
                quality.peerToPeerDelay = _quality.peer_to_peer_delay;
                quality.peerToPeerPacketLostRate = _quality.peer_to_peer_packet_lost_rate;
                quality.level = ZegoStreamQualityLevel(_quality.level);
                quality.delay = _quality.delay;
                quality.avTimestampDiff = _quality.av_timestamp_diff;

                quality.isHardwareDecode = _quality.is_hardware_decode;
                quality.videoCodecID = ZegoVideoCodecID(_quality.video_codec_id);

                quality.totalRecvBytes = _quality.total_recv_bytes;
                quality.audioRecvBytes = _quality.audio_recv_bytes;
                quality.videoRecvBytes = _quality.video_recv_bytes;

                return quality;
            }

            static ZegoDeviceInfo I2ODeviceInfo(const zego_device_info &_deviceInfo)
            {
                ZegoDeviceInfo deviceInfo;
                deviceInfo.deviceID = _deviceInfo.device_id;
                deviceInfo.deviceName = _deviceInfo.device_name;
                return deviceInfo;
            }

            static ZegoPerformanceStatus I2OPerformanceStatus(const zego_performance_status &_status)
            {
                ZegoPerformanceStatus status;
                status.cpuUsageApp = _status.cpu_usage_app;
                status.cpuUsageSystem = _status.cpu_usage_system;
                status.memoryUsageApp = _status.memory_usage_app;
                status.memoryUsageSystem = _status.memory_usage_system;
                status.memoryUsedApp = _status.memory_used_app;
                return status;
            }

            static zego_audio_config O2IAudioConfig(const ZegoAudioConfig &config)
            {
                zego_audio_config _config;
                _config.bitrate = config.bitrate;
                _config.channel = zego_audio_channel(config.channel);
                _config.codec_id = zego_audio_codec_id(config.codecID);
                return _config;
            }

            static ZegoAudioConfig I2OAudioConfig(const zego_audio_config &_config)
            {
                ZegoAudioConfig config;
                config.bitrate = _config.bitrate;
                config.channel = ZegoAudioChannel(_config.channel);
                config.codecID = ZegoAudioCodecID(_config.codec_id);
                return config;
            }

            static zego_video_config O2IVideoConfig(const ZegoVideoConfig &config)
            {
                zego_video_config _config;
                _config.bitrate = config.bitrate;
                _config.fps = config.fps;
                _config.capture_height = config.captureHeight;
                _config.capture_width = config.captureWidth;
                _config.encode_height = config.encodeHeight;
                _config.encode_width = config.encodeWidth;
                _config.codec_id = zego_video_codec_id(config.codecID);
                return _config;
            }

            static ZegoVideoConfig I2OVideoConfig(const zego_video_config &_config)
            {
                ZegoVideoConfig config;
                config.bitrate = _config.bitrate;
                config.fps = _config.fps;
                config.captureHeight = _config.capture_height;
                config.captureWidth = _config.capture_width;
                config.encodeHeight = _config.encode_height;
                config.encodeWidth = _config.encode_width;
                config.codecID = ZegoVideoCodecID(_config.codec_id);
                return config;
            }

            static zego_video_frame_param O2IVideoFrameParam(const ZegoVideoFrameParam &param)
            {
                zego_video_frame_param _param;
                _param.format = zego_video_frame_format(param.format);
                memcpy(_param.strides, param.strides, sizeof(_param.strides));
                _param.height = param.height;
                _param.width = param.width;
                _param.rotation = param.rotation;
                return _param;
            }

            static ZegoVideoFrameParam I2OVideoFrameParam(const zego_video_frame_param &_param)
            {
                ZegoVideoFrameParam param;
                param.format = ZegoVideoFrameFormat(_param.format);
                memcpy(param.strides, _param.strides, sizeof(param.strides));
                param.height = _param.height;
                param.width = _param.width;
                param.rotation = _param.rotation;
                return param;
            }

            static zego_video_encoded_frame_param O2IVideoEncodedFrameParam(const ZegoVideoEncodedFrameParam &param)
            {
                zego_video_encoded_frame_param _param;
                _param.format = zego_video_encoded_frame_format(param.format);
                _param.is_key_frame = param.isKeyFrame;
                _param.height = param.height;
                _param.width = param.width;
                _param.rotation = param.rotation;
                _param.sei_data = param.SEIData;
                _param.sei_data_length = param.SEIDataLength;
                return _param;
            }

            static ZegoVideoEncodedFrameParam I2OVideoEncodedFrameParam(const zego_video_encoded_frame_param &_param)
            {
                ZegoVideoEncodedFrameParam param;
                param.format = ZegoVideoEncodedFrameFormat(_param.format);
                param.isKeyFrame = _param.is_key_frame;
                param.height = _param.height;
                param.width = _param.width;
                param.rotation = _param.rotation;
                param.SEIData = _param.sei_data;
                param.SEIDataLength = _param.sei_data_length;
                return param;
            }

			static ZegoAudioFrameParam I2OAudioFrameParam(const zego_audio_frame_param &_param)
			{
				ZegoAudioFrameParam param;
				param.channel = ZegoAudioChannel(_param.channel);
                                param.sampleRate = ZegoAudioSampleRate(_param.sample_rate);
				return param;
			}

			static zego_audio_frame_param O2IAudioFrameParam(const ZegoAudioFrameParam &param)
			{
				zego_audio_frame_param _param;
				_param.channel = zego_audio_channel(param.channel);
                                _param.sample_rate = zego_audio_sample_rate(param.sampleRate);
				return _param;
			}

			static ZegoDataRecordConfig I2ODataRecordConfig(const zego_data_record_config &_config)
			{
				ZegoDataRecordConfig config;
                                config.filePath = _config.file_path;
				config.recordType = ZegoDataRecordType(_config.record_type);
				return config;
			}

			static zego_data_record_config O2IDataRecordConfig(const ZegoDataRecordConfig &config)
			{
				zego_data_record_config _config;
                                memset(_config.file_path, 0, sizeof(_config.file_path));
                                ZegoStrncpy(_config.file_path, config.filePath.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
				_config.record_type = zego_data_record_type(config.recordType);
				return _config;
			}

            static zego_cdn_config O2ICDNConfig(const ZegoCDNConfig &config)
            {
                zego_cdn_config _config;
                memset(_config.url, 0, sizeof(_config.url));
                memset(_config.auth_param, 0, sizeof(_config.auth_param));
                ZegoStrncpy(_config.url, config.url.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
                ZegoStrncpy(_config.auth_param, config.authParam.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
                return _config;
            }

            static zego_mixer_input O2IMixerInput(const ZegoMixerInput &input)
            {
                zego_mixer_input _input;
                _input.content_type = zego_mixer_input_content_type(input.contentType);
                _input.layout.left = input.layout.x;
                _input.layout.right = input.layout.x + input.layout.width;
                _input.layout.top = input.layout.y;
                _input.layout.bottom = input.layout.y + input.layout.height;
                _input.sound_level_id = input.soundLevelID;
                _input.is_audio_focus = input.isAudioFocus;
                memset(_input.stream_id, 0, sizeof(_input.stream_id));
                ZegoStrncpy(_input.stream_id, input.streamID.c_str(), ZEGO_EXPRESS_MAX_STREAM_LEN);
                return _input;
            }

            static zego_mixer_output O2IMixerOutput(const ZegoMixerOutput &output)
            {
                zego_mixer_output _output;
                memset(_output.target, 0, sizeof(_output.target));
                ZegoStrncpy(_output.target, output.target.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);
                return _output;
            }

            static zego_mixer_video_config O2IMixerVideoConfig(const ZegoMixerVideoConfig &config)
            {
                zego_mixer_video_config _config;
                _config.bitrate = config.bitrate;
                _config.fps = config.fps;
                _config.width = config.width;
                _config.height = config.height;
                return _config;
            }

            static zego_mixer_audio_config O2IMixerAudioConfig(const ZegoMixerAudioConfig &config)
            {
                zego_mixer_audio_config _config;
                _config.bitrate = config.bitrate;
                _config.codec_id = zego_audio_codec_id(config.codecID);
                _config.channel = zego_audio_channel(config.channel);
                _config.mix_mode = zego_audio_mix_mode(config.mixMode);
                return _config;
            }

            static zego_watermark O2IWatermark(const ZegoWatermark &watermark)
            {
                zego_watermark _watermark;
                _watermark.layout.left = watermark.layout.x;
                _watermark.layout.right = watermark.layout.x + watermark.layout.width;
                _watermark.layout.top = watermark.layout.y;
                _watermark.layout.bottom = watermark.layout.y + watermark.layout.height;
                memset(_watermark.image_url, 0, sizeof(_watermark.image_url));
                ZegoStrncpy(_watermark.image_url, watermark.imageURL.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
                return _watermark;
            }

            static zego_custom_audio_process_config O2ICustomAudioProcessConfig(const ZegoCustomAudioProcessConfig& config)
            {
                zego_custom_audio_process_config _config;
                _config.channel = zego_audio_channel(config.channel);
                _config.sample_rate = zego_audio_sample_rate(config.sampleRate);
                _config.samples = config.samples;
                return _config;
            }
            
            static zego_sei_config O2SEIConfig(const ZegoSEIConfig &config)
            {
                zego_sei_config _config;
                _config.type = zego_sei_type(config.type);
                return _config;
            }
        };

    } // namespace EXPRESS
} // namespace ZEGO
