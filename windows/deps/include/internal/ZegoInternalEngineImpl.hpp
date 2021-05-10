#pragma once

#include "../ZegoExpressInterface.h"
#include "ZegoInternalBridge.h"
#include "ZegoInternalBase.h"
#include "ZegoInternalCallbackImpl.hpp"
#include "ZegoInternalMediaPlayer.hpp"
#include "ZegoInternalAudioEffectPlayer.hpp"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO
{
    namespace EXPRESS
    {
        class ZegoExpressEngineImp : public IZegoExpressEngine
        {
        public:
            using ZegoExpressEngineImpPtr = ZegoExpressEngineImp *;
            void setEventHandler(std::shared_ptr<IZegoEventHandler> eventHandler) override
            {
                if (oInternalCallbackCenter->getIZegoEventHandler() && eventHandler)
                {
                    oInternalOriginBridge->printDebugInfoAndPopupWhenError(1, "setEventHandler", ZEGO_ERROR_CODE_COMMON_EVENT_HANDLER_EXISTS);
                }
                else
                {
                    oInternalCallbackCenter->setIZegoEventHandler(eventHandler);
                }
            }

            void uploadLog() override
            {
                oInternalOriginBridge->uploadLog();
            }

            void uploadLog(ZegoUploadLogResultCallback callback) override
            {
                int seq = oInternalOriginBridge->uploadLog();
                if(callback != nullptr){
                    oInternalCallbackCenter->insertZegoUploadLogResultCallback(seq, callback);
                }
            }

            void setDebugVerbose(bool enable, ZegoLanguage language) override
            {
                oInternalOriginBridge->setDebugVerbose(enable, zego_language(language));
            }
            
            std::string callExperimentalAPI(const std::string& params) override
            {
                return oInternalOriginBridge->callExperimentalAPI(params);
            }

            ///===================================================================================================
            void loginRoom(const std::string &roomID, ZegoUser user) override
            {
                const char *_roomId = roomID.c_str();
                zego_user _user = ZegoExpressConvert::O2IUser(user);
                oInternalOriginBridge->loginRoom(_roomId, _user, nullptr);
            }

            void loginRoom(const std::string &roomID, ZegoUser user, ZegoRoomConfig config) override
            {
                const char *_roomId = roomID.c_str();
                zego_user _user = ZegoExpressConvert::O2IUser(user);
                zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(config);
                oInternalOriginBridge->loginRoom(_roomId, _user, &_config);
            }

            void loginMultiRoom(const std::string &roomID, ZegoRoomConfig *config) override
            {
                if (config == nullptr)
                {
                    oInternalOriginBridge->loginMultiRoom(roomID.c_str(), nullptr);
                }
                else
                {
                    zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(*config);
                    oInternalOriginBridge->loginMultiRoom(roomID.c_str(), &_config);
                }
            }

            void logoutRoom(const std::string &roomID) override
            {
                const char *_roomId = roomID.c_str();
                oInternalOriginBridge->logoutRoom(_roomId);
            }

            void switchRoom(const std::string &fromRoomID, const std::string &toRoomID, ZegoRoomConfig *config) override
            {
                const char *from_room_id = fromRoomID.c_str();
                const char *to_room_id = toRoomID.c_str();
                if (config == nullptr)
                {
                    oInternalOriginBridge->switchRoom(from_room_id, to_room_id, nullptr);
                }
                else
                {
                    zego_room_config _config = ZegoExpressConvert::O2IRoomConfig(*config);
                    oInternalOriginBridge->switchRoom(from_room_id, to_room_id, &_config);
                }
            }

            void setRoomExtraInfo(const std::string &roomID, const std::string &key, const std::string &value, ZegoRoomSetRoomExtraInfoCallback callback) override
            {
                int seq = oInternalOriginBridge->setRoomExtraInfo(roomID.c_str(), key.c_str(), value.c_str());
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoRoomSetRoomExtraInfoCallback(seq, callback);
                }
            }

            ///===================================================================================================
            void startPublishingStream(const std::string &streamID, ZegoPublishChannel channel) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->startPublishingStream(stream_id, zego_publish_channel(channel));
            }

            void stopPublishingStream(ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->stopPublishingStream(zego_publish_channel(channel));
            }

            void setStreamExtraInfo(const std::string &extraInfo, ZegoPublisherSetStreamExtraInfoCallback callback, ZegoPublishChannel channel) override
            {
                const char *_extraInfo = extraInfo.c_str();
                int seq = oInternalOriginBridge->setStreamExtraInfo(_extraInfo, zego_publish_channel(channel));
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoPublisherSetStreamExtraInfoCallback(seq, callback);
                }
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void startPreview(ZegoCanvas *canvas, ZegoPublishChannel channel) override
            {
                if (canvas == nullptr)
                {
                    oInternalOriginBridge->startPreview(nullptr, zego_publish_channel(channel));
                }
                else
                {
                    zego_canvas _canvas;
                    _canvas.view = canvas->view;
                    _canvas.view_mode = zego_view_mode(canvas->viewMode);
                    _canvas.background_color = canvas->backgroundColor;
                    oInternalOriginBridge->startPreview(&_canvas, zego_publish_channel(channel));
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void stopPreview(ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->stopPreview(zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_AUDIO_SDK
            void startPreview() override
            {
                oInternalOriginBridge->startPreview(nullptr, zego_publish_channel_main);
            }
#endif

#if ZEGO_EXPRESS_AUDIO_SDK
            void stopPreview() override
            {
                oInternalOriginBridge->stopPreview(zego_publish_channel_main);
            }
#endif

            void setReverbPreset(ZegoReverbPreset preset) override
            {
                oInternalOriginBridge->setReverbPreset(zego_reverb_preset(preset));
            }

            void setReverbParam(ZegoReverbParam param) override
            {
                zego_reverb_param _param;
                _param.damping = param.damping;
                _param.dry_wet_ratio = param.dryWetRatio;
                _param.reverberance = param.reverberance;
                _param.room_size = param.roomSize;

                oInternalOriginBridge->setReverbParam(_param);
            }

            void setReverbAdvancedParam(ZegoReverbAdvancedParam param) override
            {
                zego_reverb_advanced_param _param;

                _param.damping = param.damping;
                _param.reverberance = param.reverberance;
                _param.room_size = param.roomSize;
                _param.pre_delay = param.preDelay;
                _param.tone_low = param.toneLow;
                _param.tone_high = param.toneHigh;
                _param.wet_gain = param.wetGain;
                _param.dry_gain = param.dryGain;
                _param.stereo_width = param.stereoWidth;
                _param.wet_only = param.wetOnly;

                oInternalOriginBridge->setReverbParam(_param);
            }

            void setReverbEchoParam(ZegoReverbEchoParam param) override
            {
                zego_reverb_echo_param _param;
                _param.in_gain = param.inGain;
                _param.out_gain = param.outGain;
                _param.num_delays = param.numDelays;
                for (int i = 0; i < 7; i++)
                {
                    _param.delay[i] = param.delay[i];
                    _param.decay[i] = param.decay[i];
                }
                oInternalOriginBridge->setReverbEchoParam(_param);
            }

            void enableVirtualStereo(bool enable, int angle) override
            {
                oInternalOriginBridge->enableVirtualStereo(enable, angle);
            }

            void setVoiceChangerPreset(ZegoVoiceChangerPreset preset) override
            {
                oInternalOriginBridge->setVoiceChangerPreset(zego_voice_changer_preset(preset));
            }

            void setVoiceChangerParam(ZegoVoiceChangerParam param) override
            {
                oInternalOriginBridge->setVoiceChangerParam(param.pitch);
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void setVideoConfig(ZegoVideoConfig videoConfig, ZegoPublishChannel channel) override
            {
                zego_video_config _config = ZegoExpressConvert::O2IVideoConfig(videoConfig);
                oInternalOriginBridge->setVideoConfig(_config, zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            ZegoVideoConfig getVideoConfig(ZegoPublishChannel channel) override
            {
                zego_video_config video_config = oInternalOriginBridge->getVideoConfig(zego_publish_channel(channel));
                auto videoConfig = ZegoExpressConvert::I2OVideoConfig(video_config);
                return videoConfig;
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->setVideoMirrorMode(zego_video_mirror_mode(mirrorMode), zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
#if (TARGET_OS_IPHONE || defined(ANDROID))
            void setAppOrientation(ZegoOrientation orientation, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->setAppOrientation((zego_orientation)orientation, zego_publish_channel(channel));
            }
#endif
#endif

            void setAudioConfig(ZegoAudioConfig audioConfig, ZegoPublishChannel channel) override
            {
                zego_audio_config config = ZegoExpressConvert::O2IAudioConfig(audioConfig);
                oInternalOriginBridge->setAudioConfig(config, zego_publish_channel(channel));
            }

            ZegoAudioConfig getAudioConfig(ZegoPublishChannel channel) override
            {
                zego_audio_config _config = oInternalOriginBridge->getAudioConfig(zego_publish_channel(channel));
                ZegoAudioConfig config = ZegoExpressConvert::I2OAudioConfig(_config);
                return config;
            }

            void setPublishStreamEncryptionKey(const std::string &key, ZegoPublishChannel channel) override
            {
                const char *encryption_key = key.c_str();
                oInternalOriginBridge->setPublishStreamEncryptionKey(encryption_key, zego_publish_channel(channel));
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void takePublishStreamSnapshot(ZegoPublisherTakeSnapshotCallback callback, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                oInternalOriginBridge->takePublishStreamSnapshot(zego_publish_channel(channel));
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoPublisherTakeSnapshotCallback((int)channel, callback);
                }
            }
#endif

            void mutePublishStreamAudio(bool mute, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->mutePublishStreamAudio(mute, zego_publish_channel(channel));
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void mutePublishStreamVideo(bool mute, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->mutePublishStreamVideo(mute, zego_publish_channel(channel));
            }
#endif

            void enableTrafficControl(bool enable, int property, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->enableTrafficControl(enable, property, zego_publish_channel(channel));
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->setMinVideoBitrateForTrafficControl(bitrate, zego_traffic_control_min_video_bitrate_mode(mode), zego_publish_channel(channel));
            }
#endif

            void setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->setTrafficControlFocusOn(zego_traffic_control_focus_on_mode(mode), zego_publish_channel(channel));
            }

            void setCaptureVolume(int volume) override
            {
                oInternalOriginBridge->setCaptureVolume(volume);
            }

            void setAudioCaptureStereoMode(ZegoAudioCaptureStereoMode mode) override
            {
                oInternalOriginBridge->setAudioCaptureStereoMode(zego_audio_capture_stereo_mode(mode));
            }

            void addPublishCdnUrl(const std::string &streamID, const std::string &targetURL, ZegoPublisherUpdateCdnUrlCallback callback) override
            {
                const char *stream_id = streamID.c_str();
                const char *target_url = targetURL.c_str();

                int seq = oInternalOriginBridge->addPublishCdnUrl(stream_id, target_url);
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoPublisherUpdateCdnUrlCallback(seq, callback);
                }
            }

            void removePublishCdnUrl(const std::string &streamID, const std::string &targetURL, ZegoPublisherUpdateCdnUrlCallback callback) override
            {
                const char *stream_id = streamID.c_str();
                const char *target_url = targetURL.c_str();

                int seq = oInternalOriginBridge->removePublishCdnUrl(stream_id, target_url);
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoPublisherUpdateCdnUrlCallback(seq, callback);
                }
            }

            void enablePublishDirectToCDN(bool enable, ZegoCDNConfig *config, ZegoPublishChannel channel) override
            {
                if (config == nullptr)
                {
                    oInternalOriginBridge->enablePublishDirectToCDN(enable, nullptr, zego_publish_channel(channel));
                }
                else
                {
                    zego_cdn_config _config = ZegoExpressConvert::O2ICDNConfig(*config);
                    oInternalOriginBridge->enablePublishDirectToCDN(enable, &_config, zego_publish_channel(channel));
                }
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void setPublishWatermark(ZegoWatermark *watermark, bool isPreviewVisible, ZegoPublishChannel channel) override
            {
                if (watermark != nullptr)
                {
                    zego_watermark _watermark = ZegoExpressConvert::O2IWatermark(*watermark);
                    oInternalOriginBridge->setPublishWatermark(&_watermark, isPreviewVisible, zego_publish_channel(channel));
                }
                else
                {
                    oInternalOriginBridge->setPublishWatermark(nullptr, isPreviewVisible, zego_publish_channel(channel));
                }
            }
#endif

            void setSEIConfig(ZegoSEIConfig config) override
            {
                zego_sei_config _config = ZegoExpressConvert::O2SEIConfig(config);
                oInternalOriginBridge->setSEIConfig(_config);
            }

            void sendSEI(const unsigned char *buffer, unsigned int buffer_length, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->sendSEI(buffer, buffer_length, zego_publish_channel(channel));
            }

            void enableHardwareEncoder(bool enable) override
            {
                oInternalOriginBridge->enableHardwareEncoder(enable);
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) override
            {
                zego_capture_pipeline_scale_mode _mode = zego_capture_pipeline_scale_mode(mode);
                oInternalOriginBridge->setCapturePipelineScaleMode(_mode);
            }
#endif

            ///===================================================================================================
#if ZEGO_EXPRESS_VIDEO_SDK
            void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas) override
            {
                const char *stream_id = streamID.c_str();
                if (canvas == nullptr)
                {
                    oInternalOriginBridge->startPlayingStream(stream_id, nullptr);
                }
                else
                {
                    zego_canvas _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
                    oInternalOriginBridge->startPlayingStream(stream_id, &_canvas);
                }
            }

            // void setPlayStreamView(const std::string &streamID, ZegoCanvas *canvas) override
            // {
            //     const char *stream_id = streamID.c_str();
            //     if (canvas == nullptr)
            //     {
            //         oInternalOriginBridge->setPlayStreamView(stream_id, nullptr);
            //     }
            //     else
            //     {
            //         zego_canvas _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
            //         oInternalOriginBridge->setPlayStreamView(stream_id, &_canvas);
            //     }
            // }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void startPlayingStream(const std::string &streamID, ZegoCanvas *canvas, ZegoPlayerConfig config) override
            {
                const char *stream_id = streamID.c_str();
                zego_canvas _canvas;
                if (canvas)
                {
                    _canvas = ZegoExpressConvert::O2ICanvas(*canvas);
                }
                zego_player_config _config;
                memset(&_config, 0, sizeof(zego_player_config));
                zego_cdn_config cdn_cofig;
                memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
                if (config.cdnConfig)
                {
                    cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
                    _config.cdn_config = &cdn_cofig;
                }
                else
                {
                    _config.cdn_config = nullptr;
                }
                _config.video_layer = zego_player_video_layer(config.videoLayer);
                _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
                oInternalOriginBridge->startPlayingStreamWithConfig(stream_id, canvas ? &_canvas : nullptr, _config);
            }
#endif

#if ZEGO_EXPRESS_AUDIO_SDK
            void startPlayingStream(const std::string &streamID) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->startPlayingStream(stream_id, nullptr);
            }
#endif

#if ZEGO_EXPRESS_AUDIO_SDK
            void startPlayingStream(const std::string &streamID, ZegoPlayerConfig config) override
            {
                const char *stream_id = streamID.c_str();
                zego_player_config _config;
                memset(&_config, 0, sizeof(zego_player_config));
                zego_cdn_config cdn_cofig;
                memset(&cdn_cofig, 0, sizeof(zego_cdn_config));
                if (config.cdnConfig)
                {
                    cdn_cofig = ZegoExpressConvert::O2ICDNConfig(*config.cdnConfig);
                    _config.cdn_config = &cdn_cofig;
                }
                else
                {
                    _config.cdn_config = nullptr;
                }
                _config.video_layer = zego_player_video_layer(config.videoLayer);
                _config.resource_mode = zego_stream_resource_mode(config.resourceMode);
                oInternalOriginBridge->startPlayingStreamWithConfig(stream_id, nullptr, _config);
            }
#endif

            void stopPlayingStream(const std::string &streamID) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->stopPlayingStream(stream_id);
            }

            void setPlayStreamDecryptionKey(const std::string &streamID, const std::string &key) override
            {
                const char *stream_id = streamID.c_str();
                const char *decryption_key = key.c_str();
                oInternalOriginBridge->setPlayStreamDecryptionKey(stream_id, decryption_key);
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void takePlayStreamSnapshot(const std::string &streamID, ZegoPlayerTakeSnapshotCallback callback) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->takePlayStreamSnapshot(stream_id);
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoPlayerTakeSnapshotCallback(streamID, callback);
                }
            }
#endif

            void setPlayVolume(const std::string &streamID, int volume) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->setPlayVolume(stream_id, volume);
            }

            void setAllPlayStreamVolume(int volume) override
            {
                oInternalOriginBridge->setAllPlayStreamVolume(volume);
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void setPlayStreamVideoLayer(const std::string &streamID, ZegoPlayerVideoLayer videoLayer) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->setPlayStreamVideoLayer(stream_id, zego_player_video_layer(videoLayer));
            }

            void setPlayStreamVideoType(const std::string &streamID, ZegoVideoStreamType type) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->setPlayStreamVideoType(stream_id, zego_video_stream_type(type));
            }
#endif

            void setPlayStreamBufferIntervalRange(const std::string &streamID, unsigned int minBufferInterval, unsigned int maxBufferInterval) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->setPlayStreamBufferIntervalRange(stream_id, minBufferInterval, maxBufferInterval);
            }

            void setPlayStreamFocusOn(const std::string& streamID) override
            {
                oInternalOriginBridge->setPlayStreamFocusOn(streamID.c_str());
            }

            void mutePlayStreamAudio(const std::string &streamID, bool mute) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->mutePlayStreamAudio(stream_id, mute);
            }

            void muteAllPlayStreamAudio(bool mute) override
            {
                oInternalOriginBridge->muteAllPlayStreamAudio(mute);
            }



#if ZEGO_EXPRESS_VIDEO_SDK
            void mutePlayStreamVideo(const std::string &streamID, bool mute) override
            {
                const char *stream_id = streamID.c_str();
                oInternalOriginBridge->mutePlayStreamVideo(stream_id, mute);
            }

             void muteAllPlayStreamVideo(bool mute) override
            {
                oInternalOriginBridge->muteAllPlayStreamVideo(mute);
            }
#endif

            void enableHardwareDecoder(bool enable) override
            {
                oInternalOriginBridge->enableHardwareDecoder(enable);
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void enableCheckPoc(bool enable) override
            {
                oInternalOriginBridge->enableCheckPoc(enable);
            }
#endif

            //===================================================================================================

            void muteMicrophone(bool enable) override
            {
                oInternalOriginBridge->muteMicrophone(enable);
            }

            bool isMicrophoneMuted() override
            {
                return oInternalOriginBridge->isMicrophoneMuted();
            }

            void muteSpeaker(bool mute) override
            {
                oInternalOriginBridge->muteSpeaker(mute);
            }

            bool isSpeakerMuted() override
            {
                return oInternalOriginBridge->isSpeakerMuted();
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            void enableCamera(bool enable, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->enableCamera(enable, zego_publish_channel(channel));
            }
#endif

            void enableAudioCaptureDevice(bool enable) override
            {
                oInternalOriginBridge->enableAudioCaptureDevice(enable);
            }

#if TARGET_OS_IPHONE || defined(ANDROID)
            ZegoAudioRoute getAudioRouteType() override
            {
                return ZegoAudioRoute(oInternalOriginBridge->getAudioRouteType());
            }

            void setAudioRouteToSpeaker(bool defaultToSpeaker) override
            {
                oInternalOriginBridge->setAudioRouteToSpeaker(defaultToSpeaker);
            }

            void setBuiltInSpeakerOn(bool enable) override
            {
                oInternalOriginBridge->setBuiltInSpeakerOn(enable);
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
#if (TARGET_OS_IPHONE || defined(ANDROID))
            void useFrontCamera(bool enable, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->useFrontCamera(enable, zego_publish_channel(channel));
            }

            void setCameraZoomFactor(float factor, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->setCameraZoomFactor(factor, zego_publish_channel(channel));
            }

            float getCameraMaxZoomFactor(ZegoPublishChannel channel) override
            {
                return oInternalOriginBridge->getCameraMaxZoomFactor(zego_publish_channel(channel));
            }
#endif
#endif

            void startSoundLevelMonitor(unsigned int millisecond) override
            {
                oInternalOriginBridge->startSoundLevelMonitor(millisecond);
            }

            void stopSoundLevelMonitor() override
            {
                oInternalOriginBridge->stopSoundLevelMonitor();
            }

            void startAudioSpectrumMonitor(unsigned int millisecond) override
            {
                oInternalOriginBridge->startAudioSpectrumMonitor(millisecond);
            }

            void stopAudioSpectrumMonitor() override
            {
                oInternalOriginBridge->stopAudioSpectrumMonitor();
            }

            void startPerformanceMonitor(unsigned int millisecond) override
            {
                oInternalOriginBridge->startPerformanceMonitor(millisecond);
            }

            void stopPerformanceMonitor() override
            {
                oInternalOriginBridge->stopPerformanceMonitor();
            }

            void enableHeadphoneMonitor(bool enable) override
            {
                oInternalOriginBridge->enableHeadphoneMonitor(enable);
            }

            void setHeadphoneMonitorVolume(int volume) override
            {
                oInternalOriginBridge->setHeadphoneMonitorVolume(volume);
            }

            void enableMixSystemPlayout(bool enable) override
            {
                oInternalOriginBridge->enableMixSystemPlayout(enable);
            }

            void setMixSystemPlayoutVolume(int volume) override
            {
                oInternalOriginBridge->setMixSystemPlayoutVolume(volume);
            }

            void enableMixEnginePlayout(bool enable) override
            {
                oInternalOriginBridge->enableMixEnginePlayout(enable);
            }

            void useAudioDevice(ZegoAudioDeviceType deviceType, const std::string &deviceID) override
            {
                oInternalOriginBridge->useAudioDevice(zego_audio_device_type(deviceType), deviceID.c_str());
            }

            int getAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string &deviceID) override
            {
                return oInternalOriginBridge->getAudioDeviceVolume(zego_audio_device_type(deviceType), deviceID.c_str());
            }

            void setAudioDeviceVolume(ZegoAudioDeviceType deviceType, const std::string &deviceID, int volume) override
            {
                oInternalOriginBridge->setAudioDeviceVolume(zego_audio_device_type(deviceType), deviceID.c_str(), volume);
            }

            void startAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType, const std::string& deviceID) override
            {
                oInternalOriginBridge->startAudioDeviceVolumeMonitor(zego_audio_device_type(deviceType), deviceID.c_str());
            }

            void stopAudioDeviceVolumeMonitor(ZegoAudioDeviceType deviceType, const std::string& deviceID) override
            {
                oInternalOriginBridge->stopAudioDeviceVolumeMonitor(zego_audio_device_type(deviceType), deviceID.c_str());
            }

            void muteAudioDevice(ZegoAudioDeviceType deviceType, const std::string& deviceID, bool mute) override
            {
                oInternalOriginBridge->muteAudioDevice(zego_audio_device_type(deviceType), deviceID.c_str(), mute);
            }

            bool isAudioDeviceMuted(ZegoAudioDeviceType deviceType, const std::string& deviceID) override
            {
                return oInternalOriginBridge->isAudioDeviceMuted(zego_audio_device_type(deviceType), deviceID.c_str());
            }

            std::vector<ZegoDeviceInfo> getAudioDeviceList(ZegoAudioDeviceType deviceType) override
            {
                int count = 0;
                zego_device_info *_deviceInfoList = oInternalOriginBridge->getAudioDeviceList(zego_audio_device_type(deviceType), &count);

                std::vector<ZegoDeviceInfo> result;
                for (int i = 0; i < count; i++)
                {
                    ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
                    result.push_back(deviceInfo);
                }

                oInternalOriginBridge->freeAudioDeviceList(_deviceInfoList);
                return result;
            }

            virtual std::string getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) override
            {
                return oInternalOriginBridge->getDefaultAudioDeviceID(zego_audio_device_type(deviceType));
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            virtual std::string getDefaultVideoDeviceID() override
            {
                return oInternalOriginBridge->getDefaultVideoDeviceID();
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void useVideoDevice(const std::string &deviceID, ZegoPublishChannel channel) override
            {
                const char *device_id = deviceID.c_str();
                oInternalOriginBridge->useVideoDevice(device_id, zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            std::vector<ZegoDeviceInfo> getVideoDeviceList() override
            {
                int count = 0;
                zego_device_info *_deviceInfoList = oInternalOriginBridge->getVideoDeviceList(&count);

                std::vector<ZegoDeviceInfo> result;
                for (int i = 0; i < count; i++)
                {
                    ZegoDeviceInfo deviceInfo = ZegoExpressConvert::I2ODeviceInfo(_deviceInfoList[i]);
                    result.push_back(deviceInfo);
                }

                oInternalOriginBridge->freeVideoDeviceList(_deviceInfoList);
                return result;
            }
#endif

            //===================================================================================================

            void enableAEC(bool enable) override
            {
                oInternalOriginBridge->enableAEC(enable);
            }

#if TARGET_OS_IPHONE || defined(ANDROID)
            void enableHeadphoneAEC(bool enable) override
            {
                oInternalOriginBridge->enableHeadphoneAEC(enable);
            }
#endif

            void setAECMode(ZegoAECMode mode) override
            {
                oInternalOriginBridge->setAECMode(zego_aec_mode(mode));
            }

            void enableAGC(bool enable) override
            {
                oInternalOriginBridge->enableAGC(enable);
            }

            void setANSMode(ZegoANSMode mode) override
            {
                oInternalOriginBridge->setANSMode(zego_ans_mode(mode));
            }

            void enableANS(bool enable) override
            {
                oInternalOriginBridge->enableANS(enable);
            }

            void enableTransientANS(bool enable) override
            {
                oInternalOriginBridge->enableTransientANS(enable);
            }

            void enableAudioMixing(bool enable) override
            {
                oInternalOriginBridge->enableAudioMixing(enable);
            }

            void setAudioMixingHandler(std::shared_ptr<IZegoAudioMixingHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoAudioMixingHandler(handler);
            }

            void muteLocalAudioMixing(bool mute) override
            {
                oInternalOriginBridge->muteLocalAudioMixing(mute);
            }

            void setAudioMixingVolume(int volume, ZegoVolumeType type) override
            {
                oInternalOriginBridge->setAudioMixingVolume(volume, zego_volume_type(type));
            }

            void setAudioEqualizerGain(int bandIndex, float bandGain) override
            {
                oInternalOriginBridge->setAudioEqualizerGain(bandIndex, bandGain);
            }

            //===================================================================================================
            void sendBroadcastMessage(const std::string &roomID, const std::string &message, ZegoIMSendBroadcastMessageCallback callback) override
            {
                const char *_roomID = roomID.c_str();
                const char *_message = message.c_str();

                int seq = oInternalOriginBridge->sendBroadcastMessage(_roomID, _message);
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoIMSendBroadcastMessageCallback(seq, callback);
                }
            }

            void sendBarrageMessage(const std::string &roomID, const std::string &message, ZegoIMSendBarrageMessageCallback callback) override
            {
                const char *_roomID = roomID.c_str();
                const char *_message = message.c_str();

                int seq = oInternalOriginBridge->sendBarrageMessage(_roomID, _message);
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoIMSendBarrageMessageCallback(seq, callback);
                }
            }

            void sendCustomCommand(const std::string &roomID, const std::string &command, std::vector<ZegoUser> toUserList, ZegoIMSendCustomCommandCallback callback) override
            {
                std::vector<zego_user> _userList;
                for (auto user : toUserList)
                {
                    zego_user _user = ZegoExpressConvert::O2IUser(user);
                    _userList.push_back(_user);
                }
                zego_user *users = _userList.data();
                const char *_roomID = roomID.c_str();
                const char *_command = command.c_str();

                int seq = oInternalOriginBridge->sendCustomCommand(_roomID, _command, users, ZegoUInt(_userList.size()));
                if (callback != nullptr)
                {
                    oInternalCallbackCenter->insertZegoIMSendCustomCommandCallback(seq, callback);
                }
            }

            //===================================================================================================
            void StartOrStopMixerTask(bool isStart, ZegoMixerTask task, ZegoMixerStartCallback startCallback, ZegoMixerStopCallback stopCallback)
            {
                zego_mixer_task _task;

                memset(_task.task_id, 0, sizeof(_task.task_id));
                ZegoStrncpy(_task.task_id, task.taskID.c_str(), ZEGO_EXPRESS_MAX_MIXER_TASK_LEN);

                std::vector<zego_mixer_input> input_list;
                {
                    for (auto &input : task.inputList)
                    {
                        zego_mixer_input _input = ZegoExpressConvert::O2IMixerInput(input);
                        input_list.push_back(_input);
                    }
                }
                _task.input_list = input_list.data();
                _task.input_list_count = ZegoUInt(input_list.size());

                std::vector<zego_mixer_output> output_list;
                {
                    for (auto &output : task.outputList)
                    {
                        zego_mixer_output _output = ZegoExpressConvert::O2IMixerOutput(output);
                        output_list.push_back(_output);
                    }
                }
                _task.output_list = output_list.data();
                _task.output_list_count = ZegoUInt(output_list.size());

                _task.audio_config = ZegoExpressConvert::O2IMixerAudioConfig(task.audioConfig);
                _task.video_config = ZegoExpressConvert::O2IMixerVideoConfig(task.videoConfig);

                zego_watermark _watermark;
                if (task.watermark != nullptr)
                {
                    _watermark = ZegoExpressConvert::O2IWatermark(*task.watermark);
                    _task.watermark = &_watermark;
                }
                else
                {
                    _task.watermark = nullptr;
                }

                memset(_task.background_image_url, 0, sizeof(_task.background_image_url));
                ZegoStrncpy(_task.background_image_url, task.backgroundImageURL.c_str(), ZEGO_EXPRESS_MAX_URL_LEN);

                _task.enable_sound_level = task.enableSoundLevel;

                std::string advanceConfig;
                for (auto iter : task.advancedConfig)
                {
                    advanceConfig += iter.first + "=" + iter.second + ";";
                }
                memset(_task.advanced_config, 0, sizeof(_task.advanced_config));
                ZegoStrncpy(_task.advanced_config, advanceConfig.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);

                if (isStart)
                {
                    int seq = oInternalOriginBridge->startMixerTask(_task);
                    if (startCallback != nullptr)
                    {
                        oInternalCallbackCenter->insertZegoMixerStartCallback(seq, startCallback);
                    }
                }
                else
                {
                    int seq = oInternalOriginBridge->stopMixerTask(_task);
                    if (stopCallback != nullptr)
                    {
                        oInternalCallbackCenter->insertZegoMixerStopCallback(seq, stopCallback);
                    }
                }
            }

            void startMixerTask(ZegoMixerTask task, ZegoMixerStartCallback callback) override
            {
                StartOrStopMixerTask(true, task, callback, nullptr);
            }

            void stopMixerTask(ZegoMixerTask task, ZegoMixerStopCallback callback) override
            {
                StartOrStopMixerTask(false, task, nullptr, callback);
            }

            //===================================================================================================
            IZegoMediaPlayer *createMediaPlayer() override
            {
                auto index = oInternalOriginBridge->createMediaPlayer();
                if (index == zego_media_player_instance_index_null)
                {
                    return nullptr;
                }

                auto mediaPlayer = std::make_shared<ZegoExpressMediaPlayerImp>(index);
                oInternalCallbackCenter->insertZegoExpressMediaPlayerImp(index, mediaPlayer);
                return mediaPlayer.get();
            }

            void destroyMediaPlayer(IZegoMediaPlayer *&mediaPlayer) override
            {
                if (mediaPlayer && oInternalCallbackCenter->getZegoExpressMediaPlayerImp(mediaPlayer->getIndex()))
                {
                    oInternalOriginBridge->destroyMediaPlayer(zego_media_player_instance_index(mediaPlayer->getIndex()));
                    oInternalCallbackCenter->eraseZegoExpressMediaPlayerImp(mediaPlayer->getIndex());
                }
            }

            //===================================================================================================
            IZegoAudioEffectPlayer *createAudioEffectPlayer() override
            {
                auto index = oInternalOriginBridge->createAudioEffectPlayer();
                if (index == zego_audio_effect_player_instance_index_null)
                {
                    return nullptr;
                }

                auto audioEffectPlayer = std::make_shared<ZegoExpressAudioEffectPlayerImp>(index);
                oInternalCallbackCenter->insertZegoExpressAudioEffectPlayerImp(index, audioEffectPlayer);
                return audioEffectPlayer.get();
            }

            void destroyAudioEffectPlayer(IZegoAudioEffectPlayer *&audioEffectPlayer) override
            {
                if (audioEffectPlayer && oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(audioEffectPlayer->getIndex()))
                {
                    oInternalOriginBridge->destroyAudioEffectPlayer(zego_audio_effect_player_instance_index(audioEffectPlayer->getIndex()));
                    oInternalCallbackCenter->eraseZegoExpressAudioEffectPlayerImp(audioEffectPlayer->getIndex());
                }
            }

            //===================================================================================================
#if ZEGO_EXPRESS_VIDEO_SDK
            void enableCustomVideoCapture(bool enable, ZegoCustomVideoCaptureConfig *config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {

                if (config == nullptr)
                {
                    oInternalOriginBridge->enableCustomVideoCapture(enable, nullptr, zego_publish_channel(channel));
                }
                else
                {
                    zego_custom_video_capture_config _custom_video_capture_config;
                    _custom_video_capture_config.buffer_type = zego_video_buffer_type(config->bufferType);
                    oInternalOriginBridge->enableCustomVideoCapture(enable, &_custom_video_capture_config, zego_publish_channel(channel));
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void setCustomVideoCaptureHandler(std::shared_ptr<IZegoCustomVideoCaptureHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoCustomVideoCaptureHandler(handler);
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void setCustomVideoCaptureFillMode(ZegoViewMode mode, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->customVideoCaptureSetFillMode(zego_view_mode(mode), zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength, ZegoVideoFrameParam params, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel) override
            {
                auto _param = ZegoExpressConvert::O2IVideoFrameParam(params);
                oInternalOriginBridge->customVideoCaptureSendRawData(data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void sendCustomVideoCaptureEncodedData(const unsigned char *data, unsigned int dataLength, ZegoVideoEncodedFrameParam params, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                auto _param = ZegoExpressConvert::O2IVideoEncodedFrameParam(params);
                oInternalOriginBridge->customVideoCaptureSendEncodedFrameData(data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            // C++ 接口层暂不对外，等 VE 2021.3 发布之后需要重新添加 override
            void enableCustomVideoProcessing(bool enable, ZegoCustomVideoProcessConfig *config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {

                if (config == nullptr)
                {
                    oInternalOriginBridge->enableCustomVideoProcessing(enable, nullptr, zego_publish_channel(channel));
                }
                else
                {
                    zego_custom_video_process_config _custom_video_process_config;
                    memset(&_custom_video_process_config, 0, sizeof(zego_custom_video_process_config));
                    _custom_video_process_config.buffer_type = zego_video_buffer_type(config->bufferType);
                    oInternalOriginBridge->enableCustomVideoProcessing(enable, &_custom_video_process_config, zego_publish_channel(channel));
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            // C++ 接口层暂不对外，等 VE 2021.3 发布之后需要重新添加 override
            void setCustomVideoProcessHandler(std::shared_ptr<IZegoCustomVideoProcessHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoCustomVideoProcessHandler(handler);
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            // C++ 接口层暂不对外，等 VE 2021.3 发布之后需要重新添加 override
            void sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength, ZegoVideoFrameParam params, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel) override
            {
                auto _param = ZegoExpressConvert::O2IVideoFrameParam(params);
                oInternalOriginBridge->sendCustomVideoProcessedRawData(data, dataLength, _param, referenceTimeMillisecond, zego_publish_channel(channel));
            }
            
            void sendCustomVideoProcessedCVPixelBuffer(void * buffer, unsigned long long referenceTimeMillisecond, ZegoPublishChannel channel) override
            {
                oInternalOriginBridge->sendCustomVideoProcessedCVPixelBuffer(buffer, referenceTimeMillisecond, zego_publish_channel(channel));
            }
#endif

            void enableCustomAudioCaptureProcessing(bool enable, ZegoCustomAudioProcessConfig *config) override
            {
                if (config)
                {
                    auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
                    oInternalOriginBridge->enableCustomAudioCaptureProcessing(enable, &_config);
                }
                else
                {
                    oInternalOriginBridge->enableCustomAudioCaptureProcessing(enable, nullptr);
                }
            }

            void enableCustomAudioRemoteProcessing(bool enable, ZegoCustomAudioProcessConfig *config) override
            {
                if (config)
                {
                    auto _config = ZegoExpressConvert::O2ICustomAudioProcessConfig(*config);
                    oInternalOriginBridge->enableCustomAudioRemoteProcessing(enable, &_config);
                }
                else
                {
                    oInternalOriginBridge->enableCustomAudioRemoteProcessing(enable, nullptr);
                }
            }

            void setCustomAudioProcessHandler(std::shared_ptr<IZegoCustomAudioProcessHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoCustomAudioProcessHandler(handler);
            }

            //===================================================================================================
            void startRecordingCapturedData(ZegoDataRecordConfig config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                zego_data_record_config _config = ZegoExpressConvert::O2IDataRecordConfig(config);
                oInternalOriginBridge->startRecordingCapturedData(_config, zego_publish_channel(channel));
            }

            void stopRecordingCapturedData(ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                oInternalOriginBridge->stopRecordingCapturedData(zego_publish_channel(channel));
            }

            void setDataRecordEventHandler(std::shared_ptr<IZegoDataRecordEventHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoDataRecordEventHandler(handler);
            }

            //===================================================================================================
#if ZEGO_EXPRESS_VIDEO_SDK
            void enableCustomVideoRender(bool enable, ZegoCustomVideoRenderConfig *config) override
            {
                if (config == nullptr)
                {
                    oInternalOriginBridge->enableCustomVideoRender(enable, nullptr);
                }
                else
                {
                    zego_custom_video_render_config _custom_video_render_config;
                    _custom_video_render_config.buffer_type = zego_video_buffer_type(config->bufferType);
                    _custom_video_render_config.frame_format_series = zego_video_frame_format_series(config->frameFormatSeries);
                    _custom_video_render_config.enable_engine_render = config->enableEngineRender;
                    oInternalOriginBridge->enableCustomVideoRender(enable, &_custom_video_render_config);
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            void setCustomVideoRenderHandler(std::shared_ptr<IZegoCustomVideoRenderHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoCustomVideoRenderHandler(handler);
            }
#endif

            //===================================================================================================
            void setAudioDataHandler(std::shared_ptr<IZegoAudioDataHandler> handler) override
            {
                oInternalCallbackCenter->setIZegoAudioDataHandler(handler);
            }

            void enableAudioDataCallback(bool enable, unsigned int callbackBitMask, ZegoAudioFrameParam param) override
            {
                zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
                oInternalOriginBridge->enableAudioDataCallback(enable, callbackBitMask, _param);
            }
            
            void startAudioDataObserver(unsigned int observerBitMask, ZegoAudioFrameParam param) override
            {
                zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
                oInternalOriginBridge->startAudioDataObserver(observerBitMask, _param);
            }
            
            void stopAudioDataObserver() override
            {
                oInternalOriginBridge->stopAudioDataObserver();
            }

            //===================================================================================================

            void enableCustomAudioIO(bool enable, ZegoCustomAudioConfig *config, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                if (config == nullptr)
                {
                    oInternalOriginBridge->enableCustomAudioIO(enable, nullptr, zego_publish_channel(channel));
                }
                else
                {
                    zego_custom_audio_config _config;
                    _config.source_type = zego_audio_source_type(config->sourceType);
                    oInternalOriginBridge->enableCustomAudioIO(enable, &_config, zego_publish_channel(channel));
                }
            }

            void sendCustomAudioCaptureAACData(unsigned char *data, unsigned int dataLength, unsigned int configLength, unsigned long long referenceTimeMillisecond, ZegoAudioFrameParam param, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
                oInternalOriginBridge->sendCustomAudioCaptureAACData(data, dataLength, configLength, referenceTimeMillisecond, _param, zego_publish_channel(channel));
            }

            void sendCustomAudioCapturePCMData(unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param, ZegoPublishChannel channel = ZEGO_PUBLISH_CHANNEL_MAIN) override
            {
                zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
                oInternalOriginBridge->sendCustomAudioCapturePCMData(data, dataLength, _param, zego_publish_channel(channel));
            }

            void fetchCustomAudioRenderPCMData(unsigned char *data, unsigned int dataLength, ZegoAudioFrameParam param) override
            {
                zego_audio_frame_param _param = ZegoExpressConvert::O2IAudioFrameParam(param);
                oInternalOriginBridge->fetchCustomAudioRenderPCMData(data, dataLength, _param);
            }

            //===================================================================================================
            void startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config) override
            {
                zego_network_speed_test_config _config;
                _config.test_uplink = config.testUplink;
                _config.expected_uplink_bitrate = config.expectedUplinkBitrate;
                _config.test_downlink = config.testDownlink;
                _config.expected_downlink_bitrate = config.expectedDownlinkBitrate;
                oInternalOriginBridge->startNetworkSpeedTest(_config);
            }

            void stopNetworkSpeedTest() override
            {
                oInternalOriginBridge->stopNetworkSpeedTest();
            }

			void startNetworkProbe(ZegoNetworkProbeConfig config, ZegoNetworkProbeResultCallback callback) override
			{
				zego_network_probe_config _config;
				_config.enable_traceroute = config.enableTraceroute;
				int seq = oInternalOriginBridge->startNetworkProbe(_config);
				if (callback != nullptr)
				{
					oInternalCallbackCenter->insertZegoNetworkProbeResultCallback(seq, callback);
				}
			}

			void stopNetworkProbe() override
			{
				oInternalOriginBridge->stopNetworkProbe();
			}

        public:
            ///===================================================================================================
            static std::string getVersion()
            {
                std::string version = oInternalOriginBridge->getVersion();
                return version;
            }

            static void setAndroidEnv(void *jvm, void *ctx)
            {
                oInternalOriginBridge->setAndroidEnv(jvm, ctx);
            }

            static void setEngineConfig(ZegoEngineConfig engineConfig)
            {
                zego_engine_config _engineConfig;

                zego_log_config _logConfig;
                if (engineConfig.logConfig == nullptr)
                {
                    _engineConfig.log_config = nullptr;
                }
                else
                {
                    memset(_logConfig.log_path, 0, sizeof(_logConfig.log_path));
                    ZegoStrncpy(_logConfig.log_path, engineConfig.logConfig->logPath.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
                    _logConfig.log_size = engineConfig.logConfig->logSize;
                    _engineConfig.log_config = &_logConfig;
                }

                std::string advanceConfig;
                for (auto iter : engineConfig.advancedConfig)
                {
                    advanceConfig += iter.first + "=" + iter.second + ";";
                }
                memset(_engineConfig.advanced_config, 0, sizeof(_engineConfig.advanced_config));
                ZegoStrncpy(_engineConfig.advanced_config, advanceConfig.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);

                oInternalOriginBridge->setEngineConfig(_engineConfig);
            }
            static void setLogConfig(ZegoLogConfig config){
                zego_log_config _logConfig;
                ZegoStrncpy(_logConfig.log_path, config.logPath.c_str(), ZEGO_EXPRESS_MAX_COMMON_LEN);
                _logConfig.log_size = config.logSize;
                oInternalOriginBridge->setLogConfig(_logConfig);
            }
            static void setApiCalledCallback(std::shared_ptr<IZegoApiCalledEventHandler> callback){
                oInternalCallbackCenter->registerApiCalledResultCallback();
                oInternalCallbackCenter->setIZegoApiCalledEventHandler(callback);
            }

            bool init(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
            {
                oInternalCallbackCenter->registerCallback();
                oInternalCallbackCenter->setIZegoEventHandler(eventHandler);

                bool initSucceed = 0 == oInternalOriginBridge->init(appID, appSign.c_str(), isTestEnvironment, zego_scenario(scenario));
                if (!initSucceed)
                {
                    oInternalCallbackCenter->unregisterCallback();
                    oInternalCallbackCenter->clearHandlerData();
                }
                return initSucceed;
            }

            void uinitAsync(ZegoDestroyCompletionCallback afterDestroyed)
            {
                oInternalCallbackCenter->setZegoDestroyCompletionCallback(afterDestroyed);
                oInternalCallbackCenter->unregisterCallback();
                oInternalCallbackCenter->clearHandlerData();
                oInternalOriginBridge->uninitAsync();
                oInternalCallbackCenter->unRegisterApiCalledResultCallback();
            }
        };

        class ZegoExpressEngineContainer
        {
        public:
            std::recursive_mutex engineMutex;
            std::shared_ptr<ZegoExpressEngineImp> engineInstance = nullptr;
        };
#define oEngineContainer ZegoSingleton<ZegoExpressEngineContainer>::CreateInstance()

        class ZegoExpressSDKInternal
        {
        public:
            static void setEngineConfig(ZegoEngineConfig engineConfig)
            {
                std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
                ZegoExpressEngineImp::setEngineConfig(engineConfig);
            }
            static void setLogConfig(ZegoLogConfig config){
                std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
                ZegoExpressEngineImp::setLogConfig(config);
            }

            static IZegoExpressEngine *createEngine(unsigned int appID, std::string appSign, bool isTestEnvironment, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler)
            {
                std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
                if (oEngineContainer->engineInstance == nullptr)
                {
                    auto newEngineInstance = std::make_shared<ZegoExpressEngineImp>();
                    if (newEngineInstance->init(appID, appSign, isTestEnvironment, scenario, eventHandler))
                    {
                        oEngineContainer->engineInstance = newEngineInstance;
                    }
                }else{
                    oInternalOriginBridge ->printDebugInfoAndPopupWhenAlreadyInit();
                }
                return oEngineContainer->engineInstance.get();
            }

            static void destroyEngine(IZegoExpressEngine *&iEngine, ZegoDestroyCompletionCallback afterDestroyed)
            {
                std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
                if (iEngine != nullptr && iEngine == oEngineContainer->engineInstance.get())
                {
                    oEngineContainer->engineInstance->uinitAsync(afterDestroyed);
                    oEngineContainer->engineInstance = nullptr;
                    iEngine = nullptr;
                }
            }

            static IZegoExpressEngine *getEngine()
            {
                std::lock_guard<std::recursive_mutex> locker(oEngineContainer->engineMutex);
                return oEngineContainer->engineInstance.get();
            }

            static std::string getVersion()
            {
                return ZegoExpressEngineImp::getVersion();
            }

            static void setAndroidEnv(void *jvm, void *ctx)
            {
                return ZegoExpressEngineImp::setAndroidEnv(jvm, ctx);
            }
            static void setApiCalledCallback(std::shared_ptr<IZegoApiCalledEventHandler> callback){
                 ZegoExpressEngineImp::setApiCalledCallback(callback);
            }
        };

    } //namespace EXPRESS
} //namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
