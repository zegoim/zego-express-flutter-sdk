
#pragma once

#include "../ZegoExpressInterface.h"
#include "../ZegoExpressDefines.h"
#include "../ZegoExpressEventHandler.h"

#include "ZegoInternalBase.h"
#include "ZegoInternalBridge.h"
#include "ZegoInternalMediaPlayer.hpp"
#include "ZegoInternalAudioEffectPlayer.hpp"

ZEGO_DISABLE_DEPRECATION_WARNINGS

namespace ZEGO
{
    namespace EXPRESS
    {

#define oInternalCallbackCenter ZegoSingleton<ZegoInternalCallbackCenter>::CreateInstance()
        class ZegoInternalCallbackCenter
        {
        public:
            declearSingleShareMember(IZegoEventHandler);
            declearSingleShareMember(IZegoApiCalledEventHandler);
            declearMultiRawMember(zego_seq, ZegoRoomSetRoomExtraInfoCallback);
            declearMultiRawMember(zego_seq, ZegoPublisherSetStreamExtraInfoCallback);
            declearMultiRawMember(zego_seq, ZegoPublisherUpdateCdnUrlCallback);
#if ZEGO_EXPRESS_VIDEO_SDK
            declearMultiRawMember(zego_seq, ZegoPublisherTakeSnapshotCallback);
            declearMultiRawMember(std::string, ZegoPlayerTakeSnapshotCallback);
#endif
            declearMultiRawMember(zego_seq, ZegoIMSendBroadcastMessageCallback);
            declearMultiRawMember(zego_seq, ZegoIMSendBarrageMessageCallback);
            declearMultiRawMember(zego_seq, ZegoIMSendCustomCommandCallback);
            declearMultiRawMember(zego_seq, ZegoMixerStartCallback);
            declearMultiRawMember(zego_seq, ZegoMixerStopCallback);
            declearMultiShareMember(ZegoExpressMediaPlayerImp);
            declearMultiShareMember(ZegoExpressAudioEffectPlayerImp);
#if ZEGO_EXPRESS_VIDEO_SDK
            declearSingleShareMember(IZegoCustomVideoRenderHandler);
            declearSingleShareMember(IZegoCustomVideoProcessHandler);
            declearSingleShareMember(IZegoCustomVideoCaptureHandler);
#endif
            declearSingleShareMember(IZegoAudioDataHandler);
            declearSingleShareMember(IZegoAudioMixingHandler);
            declearSingleShareMember(IZegoDataRecordEventHandler);
            declearSingleShareMember(IZegoCustomAudioProcessHandler);
            declearSingleRawMember(ZegoDestroyCompletionCallback);
            declearMultiRawMember(zego_seq, ZegoTestNetworkConnectivityCallback);
            declearMultiRawMember(zego_seq, ZegoNetworkProbeResultCallback);
            declearMultiRawMember(zego_seq, ZegoUploadLogResultCallback);

            void clearHandlerData()
            {
                mIZegoEventHandler = nullptr;
                mIZegoApiCalledEventHandler = nullptr;
                mZegoRoomSetRoomExtraInfoCallback.clear();
                mZegoPublisherSetStreamExtraInfoCallback.clear();
                mZegoPublisherUpdateCdnUrlCallback.clear();
#if ZEGO_EXPRESS_VIDEO_SDK
                mZegoPublisherTakeSnapshotCallback.clear();
                mZegoPlayerTakeSnapshotCallback.clear();
#endif
                mZegoIMSendBroadcastMessageCallback.clear();
                mZegoIMSendBarrageMessageCallback.clear();
                mZegoIMSendCustomCommandCallback.clear();
                mZegoMixerStartCallback.clear();
                mZegoMixerStopCallback.clear();
                mZegoExpressMediaPlayerImp.clear();
                mZegoExpressAudioEffectPlayerImp.clear();
#if ZEGO_EXPRESS_VIDEO_SDK
                mIZegoCustomVideoRenderHandler = nullptr;
                mIZegoCustomVideoProcessHandler = nullptr;
                mIZegoCustomVideoCaptureHandler = nullptr;
#endif
                mIZegoAudioDataHandler = nullptr;
                mIZegoAudioMixingHandler = nullptr;
                mIZegoDataRecordEventHandler = nullptr;
                mIZegoCustomAudioProcessHandler = nullptr;
                mZegoTestNetworkConnectivityCallback.clear();
                mZegoNetworkProbeResultCallback.clear();
                mZegoUploadLogResultCallback.clear();
            }
            void registerApiCalledResultCallback(){
                oInternalOriginBridge->registerOnApiCalledResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_api_called_result), ZegoVoidPtr(this));
            }
            void unRegisterApiCalledResultCallback(){
                oInternalOriginBridge->registerOnApiCalledResultCallback(nullptr, nullptr);
            }

            void registerCallback()
            {
#ifdef WIN32
                oInternalOriginBridge->registerRecvWindowsMessageCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_recv_win_massage), nullptr);
#endif
                oInternalOriginBridge->registerEngineUninitCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_engine_uninit), nullptr);

                oInternalOriginBridge->registerOnCallErrorCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_debug_error), ZegoVoidPtr(this));
                oInternalOriginBridge->registerEngineStateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_engine_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerUploadLogResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_upload_log_result), ZegoVoidPtr(this));

                oInternalOriginBridge->registerRoomStreamUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_stream_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomStreamExtraInfoUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_stream_extra_info_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomUserStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_user_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomOnlineUserCountUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_online_user_count_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomExtraInfoUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_extra_info_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRoomSetRoomExtraInfoResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_room_set_room_extra_info_result), ZegoVoidPtr(this));

                oInternalOriginBridge->registerPublisherStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherQualityUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_quality_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_captured_audio_first_frame), ZegoVoidPtr(this));
#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_captured_video_first_frame), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherRenderVideoFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_render_video_first_frame), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherVideoSizeChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_video_size_changed), ZegoVoidPtr(this));
#endif
                oInternalOriginBridge->registerPublisherUpdatePublishCdnUrlCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_update_cdn_url_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherRelayCDNStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_relay_cdn_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPublisherUpdateStreamExtraInfoResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_update_stream_extra_info_result), ZegoVoidPtr(this));
#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerPublisherTakeSnapshotResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_publisher_take_snapshot_result), ZegoVoidPtr(this));
#endif

                oInternalOriginBridge->registerPlayerStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPlayerQualityUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_quality_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPlayerMediaEventCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_media_event), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPlayerRecvAudioFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_audio_first_frame), ZegoVoidPtr(this));
#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerPlayerRecvVideoFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_video_first_frame), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPlayerRenderVideoFirstFrameCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_render_video_first_frame), ZegoVoidPtr(this));
                oInternalOriginBridge->registerPlayerVideoSizeChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_video_size_changed), ZegoVoidPtr(this));
#endif
                oInternalOriginBridge->registerPlayerRecvSEICallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_recv_sei), ZegoVoidPtr(this));
#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerPlayerTakeSnapshotResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_take_snapshot_result), ZegoVoidPtr(this));
#endif

                oInternalOriginBridge->registerDeviceErrorCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_device_error), ZegoVoidPtr(this));
                oInternalOriginBridge->registerAudioDeviceStateChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_device_state_changed), ZegoVoidPtr(this));
                oInternalOriginBridge->registerAudioDeviceVolumeChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_device_volume_changed), ZegoVoidPtr(this));

#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerVideoDeviceStateChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_video_device_state_changed), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRemoteCameraStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_camera_state_update), ZegoVoidPtr(this));
#endif

#if TARGET_OS_IPHONE || defined(ANDROID)
                oInternalOriginBridge->registerAudioRouteChangeCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_route_change), ZegoVoidPtr(this));
#endif
                oInternalOriginBridge->registerRemoteMicStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_mic_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCapturedSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_sound_level_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRemoteSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_sound_level_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCapturedAudioSpectrumUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_audio_spectrum_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRemoteAudioSpectrumUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_remote_audio_spectrum_update), ZegoVoidPtr(this));

                oInternalOriginBridge->registerIMSendBroadcastMessageResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_broadcast_message_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerIMRecvBroadcasMessageCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_broadcast_message), ZegoVoidPtr(this));
                oInternalOriginBridge->registerIMSendBarrageMessageResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_barrage_message_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerIMRecvBarrageMessageResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_barrage_message), ZegoVoidPtr(this));
                oInternalOriginBridge->registerIMSendCustomCommandResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_send_custom_command_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerIMRecvCustomCommandCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_im_recv_custom_command), ZegoVoidPtr(this));

                oInternalOriginBridge->registerMixerStartResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_start_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMixerStopResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_stop_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMixerSoundLevelUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_sound_level_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMixerRelayCDNStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixer_relay_cdn_state_update), ZegoVoidPtr(this));

                oInternalOriginBridge->registerMediaPlayerStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerNetworkEventCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_network_event), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerPlayingProgressCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_playing_progress), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerRecvSEICallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_recv_sei), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerSeekToResult(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_seek_to), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerLoadFileResult(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mediaplayer_load_file_result), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_audio_frame), ZegoVoidPtr(this));

                oInternalOriginBridge->registerAudioEffectPlayStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_play_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerAudioEffectPlayerLoadResourceCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_player_load_resource), ZegoVoidPtr(this));
                oInternalOriginBridge->registerAudioEffectPlayerSeekToCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_effect_player_seek_to), ZegoVoidPtr(this));

#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerMediaPlayerVideoDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_video_frame), ZegoVoidPtr(this));
                oInternalOriginBridge->registerMediaPlayerTakeSnapshotResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_media_player_take_snapshot_result), ZegoVoidPtr(this));
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerCustomVideoRenderLocalFrameDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_render_captured_frame_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoRenderRemoteFrameDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_render_remote_frame_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoRenderLocalFrameEncodedDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_render_captured_frame_encoded_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoRenderRemoteFrameEncodedDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_render_remote_frame_encoded_data), ZegoVoidPtr(this));
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerCustomVideoCaptureStartCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_capture_start), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoCaptureStopCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_capture_stop), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoCaptureEncodedDataTrafficControlCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_capture_encoded_data_traffic_control), ZegoVoidPtr(this));
#endif
                
#if ZEGO_EXPRESS_VIDEO_SDK
                oInternalOriginBridge->registerCustomVideoProcessCapturedRawDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_process_captured_unprocessed_raw_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerCustomVideoProcessCapturedCVPixelBufferCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer), ZegoVoidPtr(this));
#endif

                oInternalOriginBridge->registerAudioMixingCopyDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_audio_mixing_copy_data), ZegoVoidPtr(this));

                oInternalOriginBridge->registerOnCapturedAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_audio_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerOnPlaybackAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_playback_audio_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerOnMixedAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_mixed_audio_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerOnPlayerAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_player_audio_data), ZegoVoidPtr(this));

                oInternalOriginBridge->registerRecordingCapturedDataStateUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_data_record_state_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerRecordingCapturedDataProgressUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_captured_data_record_progress_update), ZegoVoidPtr(this));

                oInternalOriginBridge->registerProcessCapturedAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_process_captured_audio_data), ZegoVoidPtr(this));
                oInternalOriginBridge->registerProcessRemoteAudioDataCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_process_remote_audio_data), ZegoVoidPtr(this));

                oInternalOriginBridge->registerPerformanceStatueUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_performance_status_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerNetworkModeChangedCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_mode_changed), ZegoVoidPtr(this));
                oInternalOriginBridge->registerTestNetworkConnectivityCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_test_network_connectivity), ZegoVoidPtr(this));
                oInternalOriginBridge->registerNetworkSpeedTestQualityUpdateCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_speed_test_quality_update), ZegoVoidPtr(this));
                oInternalOriginBridge->registerNetworkSpeedTestErrorCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_speed_test_error), ZegoVoidPtr(this));
				oInternalOriginBridge->registerNetworkProbeResultCallback(ZegoVoidPtr(&ZegoInternalCallbackCenter::zego_on_network_probe_result), ZegoVoidPtr(this));
            }

            void unregisterCallback()
            {
                oInternalOriginBridge->registerOnCallErrorCallback(nullptr, nullptr);
                oInternalOriginBridge->registerEngineStateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerUploadLogResultCallback(nullptr, nullptr);

                oInternalOriginBridge->registerRoomStreamUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomStreamExtraInfoUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomUserStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomOnlineUserCountUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomExtraInfoUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRoomSetRoomExtraInfoResultCallback(nullptr, nullptr);

                oInternalOriginBridge->registerPublisherStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherQualityUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherRecvAudioCapturedFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherRecvVideoCapturedFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherRenderVideoFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherVideoSizeChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherUpdatePublishCdnUrlCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherRelayCDNStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherUpdateStreamExtraInfoResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPublisherTakeSnapshotResultCallback(nullptr, nullptr);

                oInternalOriginBridge->registerPlayerStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerQualityUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerMediaEventCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerRecvAudioFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerRecvVideoFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerRenderVideoFirstFrameCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerVideoSizeChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerRecvSEICallback(nullptr, nullptr);
                oInternalOriginBridge->registerPlayerTakeSnapshotResultCallback(nullptr, nullptr);

                oInternalOriginBridge->registerAudioRouteChangeCallback(nullptr, nullptr);
                oInternalOriginBridge->registerAudioDeviceStateChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerAudioDeviceVolumeChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerVideoDeviceStateChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerDeviceErrorCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRemoteCameraStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRemoteMicStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCapturedSoundLevelUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRemoteSoundLevelUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCapturedAudioSpectrumUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRemoteAudioSpectrumUpdateCallback(nullptr, nullptr);

                oInternalOriginBridge->registerIMSendBroadcastMessageResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerIMRecvBroadcasMessageCallback(nullptr, nullptr);
                oInternalOriginBridge->registerIMSendBarrageMessageResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerIMRecvBarrageMessageResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerIMSendCustomCommandResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerIMRecvCustomCommandCallback(nullptr, nullptr);

                oInternalOriginBridge->registerMixerStartResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMixerStopResultCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMixerSoundLevelUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerAutoMixerSoundLevelUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMixerRelayCDNStateUpdateCallback(nullptr, nullptr);

                oInternalOriginBridge->registerMediaPlayerStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerNetworkEventCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerPlayingProgressCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerRecvSEICallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerSeekToResult(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerLoadFileResult(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerAudioDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerVideoDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerMediaPlayerTakeSnapshotResultCallback(nullptr, nullptr);


                oInternalOriginBridge->registerAudioEffectPlayStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerAudioEffectPlayerLoadResourceCallback(nullptr, nullptr);
                oInternalOriginBridge->registerAudioEffectPlayerSeekToCallback(nullptr, nullptr);

                oInternalOriginBridge->registerCustomVideoRenderLocalFrameDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoRenderRemoteFrameDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoRenderLocalFrameEncodedDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoRenderRemoteFrameEncodedDataCallback(nullptr, nullptr);
                
                oInternalOriginBridge->registerCustomVideoProcessCapturedRawDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoProcessCapturedCVPixelBufferCallback(nullptr, nullptr);

                oInternalOriginBridge->registerCustomVideoCaptureStartCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoCaptureStopCallback(nullptr, nullptr);
                oInternalOriginBridge->registerCustomVideoCaptureEncodedDataTrafficControlCallback(nullptr, nullptr);

                oInternalOriginBridge->registerAudioMixingCopyDataCallback(nullptr, nullptr);

                oInternalOriginBridge->registerOnCapturedAudioDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerOnPlaybackAudioDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerOnMixedAudioDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerOnPlayerAudioDataCallback(nullptr, nullptr);

                oInternalOriginBridge->registerRecordingCapturedDataStateUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerRecordingCapturedDataProgressUpdateCallback(nullptr, nullptr);

                oInternalOriginBridge->registerProcessCapturedAudioDataCallback(nullptr, nullptr);
                oInternalOriginBridge->registerProcessRemoteAudioDataCallback(nullptr, nullptr);

                oInternalOriginBridge->registerPerformanceStatueUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerNetworkModeChangedCallback(nullptr, nullptr);
                oInternalOriginBridge->registerTestNetworkConnectivityCallback(nullptr, nullptr);
                oInternalOriginBridge->registerNetworkSpeedTestQualityUpdateCallback(nullptr, nullptr);
                oInternalOriginBridge->registerNetworkSpeedTestErrorCallback(nullptr, nullptr);
				oInternalOriginBridge->registerNetworkProbeResultCallback(nullptr, nullptr);
            }

            static void zego_on_engine_uninit(void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto afterDestroy = oInternalCallbackCenter->getZegoDestroyCompletionCallback();
                if (afterDestroy)
                {
                    afterDestroy();
                    oInternalCallbackCenter->setZegoDestroyCompletionCallback(nullptr);
                }
            }

#ifdef WIN32
            static void zego_on_recv_win_massage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
            {
                ZEGO_UNUSED_VARIABLE(hWnd);
                ZEGO_UNUSED_VARIABLE(lParam);
                if (uMsg == (WM_APP + 10086))
                {
                    std::function<void(void)> *pFunc = (std::function<void(void)> *)wParam;
                    (*pFunc)();
                    delete pFunc;
                }
            }
#endif

            // DEBUG CALLBACK
            static void zego_on_debug_error(int error_code, const char *func_name, const char *_info, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string funcName = func_name;
                std::string info = _info;
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onDebugError(error_code, funcName, info);
                ZEGO_SWITCH_THREAD_ING
            }
            
            static void zego_on_api_called_result(int error_code, const char *func_name, const char *_info, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoApiCalledEventHandler();
                std::string funcName = func_name;
                std::string info = _info;
                auto weakHandler = std::weak_ptr<IZegoApiCalledEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onApiCalledResult(error_code, funcName, info);
                ZEGO_SWITCH_THREAD_ING
            }

            // ENEING CALLBACK
            static void zego_on_engine_state_update(zego_engine_state state, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onEngineStateUpdate(ZegoEngineState(state));
                ZEGO_SWITCH_THREAD_ING
            }

            // ROOM CALLBACK
            static void zego_on_room_state_update(const char *room_id, zego_room_state state, zego_error error_code, const char *extend_data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::string extendData = extend_data;
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRoomStateUpdate(roomID, ZegoRoomState(state), error_code, extendData);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_room_stream_update(const char *room_id, enum zego_update_type update_type, const struct zego_stream *stream_info_list, unsigned int stream_info_count, const char *extended_data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::string extendedData = extended_data;
                std::vector<ZegoStream> streamList;
                for (unsigned int i = 0; i < stream_info_count; i++)
                {
                    ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
                    streamList.push_back(stream);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                {
                    handlerInMain->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList, extendedData);
                    handlerInMain->onRoomStreamUpdate(roomID, ZegoUpdateType(update_type), streamList); // Deprecated
                }

                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_room_stream_extra_info_update(const char *room_id, const struct zego_stream *stream_info_list, unsigned int stream_info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::vector<ZegoStream> streamList;
                for (unsigned int i = 0; i < stream_info_count; i++)
                {
                    ZegoStream stream = ZegoExpressConvert::I2OStream(stream_info_list[i]);
                    streamList.push_back(stream);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRoomStreamExtraInfoUpdate(roomID, streamList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_user_update(const char *room_id, zego_update_type update_type, const struct zego_user *user_list, unsigned int user_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::vector<ZegoUser> userList;
                for (unsigned int i = 0; i < user_count; i++)
                {
                    ZegoUser user = ZegoExpressConvert::I2OUser(user_list[i]);
                    userList.push_back(user);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRoomUserUpdate(roomID, ZegoUpdateType(update_type), userList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_room_online_user_count_update(const char *room_id, int online_user_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRoomOnlineUserCountUpdate(roomID, online_user_count);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_room_extra_info_update(const char *room_id, const struct zego_room_extra_info *room_extra_info_list, unsigned int room_extra_info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::vector<ZegoRoomExtraInfo> infoList;
                for (unsigned int i = 0; i < room_extra_info_count; i++)
                {
                    ZegoRoomExtraInfo info = ZegoExpressConvert::I2ORoomExtraInfo(room_extra_info_list[i]);
                    infoList.push_back(info);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRoomExtraInfoUpdate(roomID, infoList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_upload_log_result(zego_seq seq, zego_error error_code, void * user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoUploadLogResultCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_room_set_room_extra_info_result(zego_error error_code, const char *room_id, const char *key, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(room_id);
                ZEGO_UNUSED_VARIABLE(key);
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoRoomSetRoomExtraInfoCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            // PUBLISHER CALLBACK
            static void zego_on_publisher_state_update(const char *stream_id, zego_publisher_state state, zego_error error_code, const char *extend_data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;
                std::string extendData = extend_data;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherStateUpdate(streamID, ZegoPublisherState(state), error_code, extendData);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_publisher_quality_update(const char *stream_id, zego_publish_stream_quality quality, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                ZegoPublishStreamQuality pushlishQuality = ZegoExpressConvert::I2OPushlishQuality(quality);
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherQualityUpdate(streamID, pushlishQuality);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_publisher_captured_audio_first_frame(void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherCapturedAudioFirstFrame();
                ZEGO_SWITCH_THREAD_ING
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_publisher_captured_video_first_frame(enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherCapturedVideoFirstFrame(ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_publisher_render_video_first_frame(enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherRenderVideoFirstFrame(ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_publisher_video_size_changed(int width, int height, enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherVideoSizeChanged(width, height, ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            static void zego_on_publisher_update_cdn_url_result(const char *stream_id, zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(stream_id);
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoPublisherUpdateCdnUrlCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_publisher_relay_cdn_state_update(const char *stream_id, const zego_stream_relay_cdn_info *state_info_list, unsigned int info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;
                std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

                for (unsigned int i = 0; i < info_count; i++)
                {
                    zego_stream_relay_cdn_info _info = state_info_list[i];
                    ZegoStreamRelayCDNInfo info;
                    info.url = _info.url;
                    info.stateTime = _info.state_time;
                    info.state = ZegoStreamRelayCDNState(_info.state);
                    info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
                    streamInfoList.push_back(info);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_publisher_update_stream_extra_info_result(zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoPublisherSetStreamExtraInfoCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_publisher_take_snapshot_result(zego_error error_code, enum zego_publish_channel channel, void *image, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoPublisherTakeSnapshotCallback((int)channel);
                if (callback)
                {
                    callback(error_code, image);
                }
            }
#endif

            // PLAYER CALLBACK
            static void zego_on_player_state_update(const char *stream_id, zego_player_state state, zego_error error_code, const char *extend_data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;
                std::string extendData = extend_data;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerStateUpdate(streamID, ZegoPlayerState(state), error_code, extendData);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_player_quality_update(const char *stream_id, zego_play_stream_quality quality, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;
                ZegoPlayStreamQuality playQuality = ZegoExpressConvert::I2OPlayQuality(quality);

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerQualityUpdate(streamID, playQuality);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_player_media_event(const char *stream_id, zego_player_media_event media_event, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerMediaEvent(streamID, ZegoPlayerMediaEvent(media_event));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_player_recv_audio_first_frame(const char *stream_id, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerRecvAudioFirstFrame(streamID);
                ZEGO_SWITCH_THREAD_ING
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_player_recv_video_first_frame(const char *stream_id, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerRecvVideoFirstFrame(streamID);
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_player_render_video_first_frame(const char *stream_id, void *user_context)
            {

                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerRenderVideoFirstFrame(streamID);
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_player_video_size_changed(const char *stream_id, int width, int height, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerVideoSizeChanged(streamID, width, height);
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            //#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_player_recv_sei(const char *stream_id, const unsigned char *data, unsigned int data_length, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;
                std::vector<unsigned char> seiData;
                seiData.reserve(data_length);
                for (unsigned int i = 0; i < data_length; i++)
                {
                    seiData.push_back(data[i]);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPlayerRecvSEI(streamID, seiData.data(), ZegoUInt(seiData.size()));
                ZEGO_SWITCH_THREAD_ING
            }
            //#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_player_take_snapshot_result(zego_error error_code, const char *stream_id, void *image, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                std::string streamID = stream_id;
                auto callback = oInternalCallbackCenter->eraseZegoPlayerTakeSnapshotCallback(streamID);
                if (callback)
                {
                    callback(error_code, image);
                }
            }
#endif

            // DEVICE CALLBACK
            static void zego_on_audio_device_state_changed(enum zego_update_type update_type, enum zego_audio_device_type device_type, struct zego_device_info _device, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onAudioDeviceStateChanged(ZegoUpdateType(update_type), ZegoAudioDeviceType(device_type), device);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_audio_device_volume_changed(enum zego_audio_device_type device_type, const char * _device_id, int volume, void * user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string deviceID = _device_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onAudioDeviceVolumeChanged(ZegoAudioDeviceType(device_type), deviceID, volume);
                ZEGO_SWITCH_THREAD_ING
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_video_device_state_changed(enum zego_update_type update_type, struct zego_device_info _device, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                ZegoDeviceInfo device = ZegoExpressConvert::I2ODeviceInfo(_device);

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onVideoDeviceStateChanged(ZegoUpdateType(update_type), device);
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            static void zego_on_device_error(zego_error error_code, const char *device_name, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string deviceName = device_name;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onDeviceError(error_code, deviceName);
                ZEGO_SWITCH_THREAD_ING
            }

#if TARGET_OS_IPHONE || defined(ANDROID)
            static void zego_on_audio_route_change(zego_audio_route audio_route, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onAudioRouteChange(ZegoAudioRoute(audio_route));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            static void zego_on_remote_mic_state_update(const char *stream_id, zego_remote_device_state state, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRemoteMicStateUpdate(streamID, ZegoRemoteDeviceState(state));
                ZEGO_SWITCH_THREAD_ING
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_remote_camera_state_update(const char *stream_id, zego_remote_device_state state, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string streamID = stream_id;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRemoteCameraStateUpdate(streamID, ZegoRemoteDeviceState(state));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            static void zego_on_captured_sound_level_update(const zego_sound_level_info *sound_level_info, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                float soundLevel = sound_level_info->sound_level;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onCapturedSoundLevelUpdate(soundLevel);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_remote_sound_level_update(const zego_sound_level_info *sound_level_info_list, unsigned int info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                std::unordered_map<std::string, float> soundLevels;
                for (unsigned int i = 0; i < info_count; i++)
                {
                    zego_sound_level_info sound_level_info = sound_level_info_list[i];
                    std::string streamID = sound_level_info.stream_id;
                    float soundLevel = sound_level_info.sound_level;
                    soundLevels.insert({streamID, soundLevel});
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRemoteSoundLevelUpdate(soundLevels);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_captured_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                ZegoAudioSpectrum audioSpectrum;
                for (unsigned int i = 0; i < audio_spectrum_info->spectrum_count; i++)
                {
                    audioSpectrum.push_back(audio_spectrum_info->spectrum_list[i]);
                }
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onCapturedAudioSpectrumUpdate(audioSpectrum);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_remote_audio_spectrum_update(const zego_audio_spectrum_info *audio_spectrum_info_list, unsigned int info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::unordered_map<std::string, ZegoAudioSpectrum> audioSpectrums;
                for (unsigned int j = 0; j < info_count; j++)
                {
                    zego_audio_spectrum_info frequency_spectrum_info = audio_spectrum_info_list[j];

                    std::string streamID = frequency_spectrum_info.stream_id;
                    ZegoAudioSpectrum frequencySpectrum;
                    for (unsigned int i = 0; i < frequency_spectrum_info.spectrum_count; i++)
                    {
                        frequencySpectrum.push_back(frequency_spectrum_info.spectrum_list[i]);
                    }
                    audioSpectrums.insert({streamID, frequencySpectrum});
                }
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onRemoteAudioSpectrumUpdate(audioSpectrums);
                ZEGO_SWITCH_THREAD_ING
            }

            // IM CALLBACK
            static void zego_on_im_send_broadcast_message_result(const char *room_id, unsigned long long message_id, zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(room_id);
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoIMSendBroadcastMessageCallback(seq);
                if (callback)
                {
                    unsigned long long messageID = message_id;
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code, messageID);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_im_recv_broadcast_message(const char *room_id, const struct zego_broadcast_message_info *message_info_list, unsigned int message_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string roomID = room_id;
                std::vector<ZegoBroadcastMessageInfo> messageInfoList;
                for (unsigned int i = 0; i < message_count; i++)
                {
                    zego_broadcast_message_info message_info = message_info_list[i];
                    ZegoBroadcastMessageInfo messageInfo;
                    messageInfo.messageID = message_info.message_id;
                    messageInfo.sendTime = message_info.send_time;
                    messageInfo.message = message_info.message;
                    messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
                    messageInfoList.push_back(messageInfo);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onIMRecvBroadcastMessage(roomID, messageInfoList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_im_send_barrage_message_result(const char *room_id, const char *message_id, zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(room_id);
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoIMSendBarrageMessageCallback(seq);
                if (callback)
                {
                    std::string messageID = message_id;
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code, messageID);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_im_recv_barrage_message(const char *room_id, const struct zego_barrage_message_info *message_info_list, unsigned int message_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                std::string roomID = room_id;
                std::vector<ZegoBarrageMessageInfo> messageInfoList;
                for (unsigned int i = 0; i < message_count; i++)
                {
                    zego_barrage_message_info message_info = message_info_list[i];
                    ZegoBarrageMessageInfo messageInfo;
                    messageInfo.messageID = message_info.message_id;
                    messageInfo.sendTime = message_info.send_time;
                    messageInfo.message = message_info.message;
                    messageInfo.fromUser = ZegoExpressConvert::I2OUser(message_info.from_user);
                    messageInfoList.push_back(messageInfo);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onIMRecvBarrageMessage(roomID, messageInfoList);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_im_send_custom_command_result(const char *room_id, zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(room_id);
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoIMSendCustomCommandCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_im_recv_custom_command(const char *room_id, struct zego_user from_user, const char *_content, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                std::string roomID = room_id;
                ZegoUser user = ZegoExpressConvert::I2OUser(from_user);
                std::string content = _content;

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onIMRecvCustomCommand(roomID, user, content);
                ZEGO_SWITCH_THREAD_ING
            }

            // MIXER CALLBACK
            static void zego_on_mixer_start_result(zego_error error_code, zego_seq seq, const char *extended_data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoMixerStartCallback(seq);
                if (callback)
                {
                    int errorCode = error_code;
                    std::string extendData = extended_data;
                    ZEGO_SWITCH_THREAD_PRE
                    callback(errorCode, extendData);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_mixer_stop_result(zego_error error_code, zego_seq seq, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto callback = oInternalCallbackCenter->eraseZegoMixerStopCallback(seq);
                if (callback)
                {
                    int errorCode = error_code;
                    ZEGO_SWITCH_THREAD_PRE
                    callback(errorCode);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

            static void zego_on_mixer_sound_level_update(const struct zego_mixer_sound_level_info *sound_level_info_list, unsigned int info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();

                std::unordered_map<unsigned int, float> soundLevels;
                for (unsigned int i = 0; i < info_count; i++)
                {
                    zego_mixer_sound_level_info sound_level_info = sound_level_info_list[i];
                    unsigned int soundLevelID = sound_level_info.sound_level_id;
                    float soundLevel = sound_level_info.sound_level;
                    soundLevels.insert({soundLevelID, soundLevel});
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onMixerSoundLevelUpdate(soundLevels);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_mixer_relay_cdn_state_update(const char *task_id, const struct zego_stream_relay_cdn_info *cdn_info_list, unsigned int info_count, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                std::string taskID = task_id;
                std::vector<ZegoStreamRelayCDNInfo> streamInfoList;

                for (unsigned int i = 0; i < info_count; i++)
                {
                    zego_stream_relay_cdn_info _info = cdn_info_list[i];
                    ZegoStreamRelayCDNInfo info;
                    info.url = _info.url;
                    info.stateTime = _info.state_time;
                    info.state = ZegoStreamRelayCDNState(_info.state);
                    info.updateReason = ZegoStreamRelayCDNUpdateReason(_info.update_reason);
                    streamInfoList.push_back(info);
                }

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onMixerRelayCDNStateUpdate(taskID, streamInfoList);
                ZEGO_SWITCH_THREAD_ING
            }

            // MediaPlayer Callback
            static void zego_on_media_player_state_update(zego_media_player_state state, zego_error error_code, zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_state_update(state, error_code);
                }
            }

            static void zego_on_media_player_network_event(zego_media_player_network_event event, zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_network_event(event);
                }
            }

            static void zego_on_media_player_playing_progress(unsigned long long millisecond, zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_playing_progress(millisecond);
                }
            }

            static void zego_on_media_player_recv_sei(const unsigned char* data, unsigned int data_length, enum zego_media_player_instance_index instance_index, void * user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_recv_sei(data, data_length);
                }
            }

            static void zego_on_media_player_seek_to(zego_seq seq, zego_error error_code, enum zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_mediaplayer_seek_to_result(seq, error_code);
                }
            }

            static void zego_on_mediaplayer_load_file_result(zego_error error_code, zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_mediaplayer_load_file_result(error_code);
                }
            }

            static void zego_on_media_player_audio_frame(const unsigned char *data, unsigned int data_length, const struct zego_audio_frame_param param, enum zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_audio_frame(data, data_length, param);
                }
            }

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_media_player_video_frame(const unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param param, enum zego_media_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_video_frame(data, data_length, param);
                }
            }


            static void zego_on_media_player_take_snapshot_result(zego_error error_code, void * image, enum zego_media_player_instance_index instance_index, void * user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto mediaPlayer = oInternalCallbackCenter->getZegoExpressMediaPlayerImp(instance_index);
                if (mediaPlayer)
                {
                    mediaPlayer->zego_on_media_player_take_snapshot_result(error_code, image, instance_index);
                }
            } 
#endif

            static void zego_on_audio_effect_play_state_update(unsigned int audio_effect_id, enum zego_audio_effect_play_state state, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto audioEffectPlayer = oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
                if (audioEffectPlayer)
                {
                    audioEffectPlayer->zego_on_audio_effect_play_state_update(audio_effect_id, state, error_code);
                }
            }

            static void zego_on_audio_effect_player_load_resource(zego_seq seq, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto audioEffectPlayer = oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
                if (audioEffectPlayer)
                {
                    audioEffectPlayer->zego_on_audio_effect_player_load_resource(seq, error_code);
                }
            }

            static void zego_on_audio_effect_player_seek_to(zego_seq seq, zego_error error_code, enum zego_audio_effect_player_instance_index instance_index, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto audioEffectPlayer = oInternalCallbackCenter->getZegoExpressAudioEffectPlayerImp(instance_index);
                if (audioEffectPlayer)
                {
                    audioEffectPlayer->zego_on_audio_effect_player_seek_to(seq, error_code);
                }
            }

            // CustomVideoRender
#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_render_captured_frame_data(unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param, enum zego_video_flip_mode flip_mode, enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
                if (handler)
                {
                    auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
                    ZegoVideoFlipMode flipMode = ZegoVideoFlipMode(flip_mode);
                    handler->onCapturedVideoFrameRawData(data, data_length, param, flipMode, ZegoPublishChannel(channel));
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_render_remote_frame_data(const char *stream_id, unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
                if (handler)
                {
                    std::string streamID = stream_id;
                    auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
                    handler->onRemoteVideoFrameRawData(data, data_length, param, streamID);
                }
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_render_captured_frame_encoded_data(const unsigned char *data, unsigned int data_length, const struct zego_video_encoded_frame_param _param, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(data);
                ZEGO_UNUSED_VARIABLE(data_length);
                ZEGO_UNUSED_VARIABLE(_param);
                ZEGO_UNUSED_VARIABLE(reference_time_millisecond);
                ZEGO_UNUSED_VARIABLE(channel);
                ZEGO_UNUSED_VARIABLE(user_context);
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_render_remote_frame_encoded_data(const unsigned char *data, unsigned int data_length, const struct zego_video_encoded_frame_param _param, unsigned long long reference_time_millisecond, const char *stream_id, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoRenderHandler();
                if (handler)
                {
                    std::string streamID = stream_id;
                    auto param = ZegoExpressConvert::I2OVideoEncodedFrameParam(_param);
                    handler->onRemoteVideoFrameEncodedData(data, data_length, param, reference_time_millisecond, streamID);
                }
            }
#endif
            
#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_process_captured_unprocessed_raw_data(const unsigned char **data, unsigned int *data_length, const struct zego_video_frame_param _param, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoProcessHandler();
                if (handler)
                {
                    auto param = ZegoExpressConvert::I2OVideoFrameParam(_param);
                    handler->onCapturedUnprocessedRawData(data, data_length, param, reference_time_millisecond, ZegoPublishChannel(channel));
                }
            }
#endif
            
#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_process_captured_unprocessed_cvpixelbuffer(void * buffer, unsigned long long reference_time_millisecond, enum zego_publish_channel channel, void * user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoProcessHandler();
                if (handler)
                {
                    handler->onCapturedUnprocessedCVPixelBuffer(buffer, reference_time_millisecond, ZegoPublishChannel(channel));
                }
            }
#endif

            // CustomVideoCapture
#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_capture_start(enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler();
                auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onStart(ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_capture_stop(enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler();
                auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onStop(ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

#if ZEGO_EXPRESS_VIDEO_SDK
            static void zego_on_custom_video_capture_encoded_data_traffic_control(struct zego_traffic_control_info traffic_control_info, enum zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomVideoCaptureHandler();
                auto weakHandler = std::weak_ptr<IZegoCustomVideoCaptureHandler>(handler);
                ZegoTrafficControlInfo trafficControlInfo;
                trafficControlInfo.bitrate = traffic_control_info.bitrate;
                trafficControlInfo.fps = traffic_control_info.fps;
                trafficControlInfo.height = traffic_control_info.height;
                trafficControlInfo.width = traffic_control_info.width;
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onEncodedDataTrafficControl(trafficControlInfo, ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }
#endif

            static void zego_on_audio_mixing_copy_data(struct zego_audio_mixing_data *data, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoAudioMixingHandler();
                if (handler)
                {
                    ZegoAudioMixingData audioMixingData;
                    audioMixingData.audioData = data->audio_data;
                    audioMixingData.audioDataLength = data->audio_data_length;
                    audioMixingData.param.channel = ZegoAudioChannel(data->param.channel);
                    audioMixingData.param.sampleRate = ZegoAudioSampleRate(data->param.sample_rate);
                    audioMixingData.SEIData = data->sei_data;
                    audioMixingData.SEIDataLength = data->sei_data_length;
                    handler->onAudioMixingCopyData(&audioMixingData);
                    data->audio_data_length = audioMixingData.audioDataLength;
                    data->param.channel = zego_audio_channel(audioMixingData.param.channel);
                    data->param.sample_rate = zego_audio_sample_rate(audioMixingData.param.sampleRate);
                    data->sei_data_length = audioMixingData.SEIDataLength;
                }
            }

            static void zego_on_captured_audio_data(const unsigned char *data, unsigned int data_length, zego_audio_frame_param _param, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
                    handler->onCapturedAudioData(data, data_length, param);
                }
            }

            static void zego_on_playback_audio_data(const unsigned char *data, unsigned int data_length, zego_audio_frame_param _param, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
                    handler->onPlaybackAudioData(data, data_length, param);
                }
            }

            static void zego_on_mixed_audio_data(const unsigned char *data, unsigned int data_length, zego_audio_frame_param _param, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
                    handler->onMixedAudioData(data, data_length, param);
                }
            }

            static void zego_on_player_audio_data(const unsigned char *data, unsigned int data_length, struct zego_audio_frame_param _param, const char *stream_id, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoAudioDataHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(_param);
                    handler->onPlayerAudioData(data, data_length, param, stream_id);
                }
            }

            static void zego_on_captured_data_record_state_update(zego_data_record_state state, zego_error error_code, zego_data_record_config _config, zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoDataRecordEventHandler();
                auto weakHandler = std::weak_ptr<IZegoDataRecordEventHandler>(handler);
                ZegoDataRecordConfig config = ZegoExpressConvert::I2ODataRecordConfig(_config);
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onCapturedDataRecordStateUpdate(ZegoDataRecordState(state), error_code, config, ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_captured_data_record_progress_update(zego_data_record_progress _progress, zego_data_record_config _config, zego_publish_channel channel, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoDataRecordEventHandler();
                auto weakHandler = std::weak_ptr<IZegoDataRecordEventHandler>(handler);
                ZegoDataRecordConfig config = ZegoExpressConvert::I2ODataRecordConfig(_config);
                ZegoDataRecordProgress progress;
                progress.duration = _progress.duration;
                progress.currentFileSize = _progress.current_file_size;
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onCapturedDataRecordProgressUpdate(progress, config, ZegoPublishChannel(channel));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_process_captured_audio_data(unsigned char *data, unsigned int data_length, struct zego_audio_frame_param *_param, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
                    handler->onProcessCapturedAudioData(data, data_length, &param);
                    *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
                }
            }

            static void zego_on_process_remote_audio_data(unsigned char *data, unsigned int data_length, struct zego_audio_frame_param *_param, const char *stream_id, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoCustomAudioProcessHandler();
                if (handler)
                {
                    ZegoAudioFrameParam param = ZegoExpressConvert::I2OAudioFrameParam(*_param);
                    handler->onProcessRemoteAudioData(data, data_length, &param, stream_id);
                    *_param = ZegoExpressConvert::O2IAudioFrameParam(param);
                }
            }

            // Utilities callback
            static void zego_on_performance_status_update(zego_performance_status _status, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                ZegoPerformanceStatus status = ZegoExpressConvert::I2OPerformanceStatus(_status);

                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handlerInMain = weakHandler.lock();
                if (handlerInMain)
                    handlerInMain->onPerformanceStatusUpdate(status);
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_network_mode_changed(enum zego_network_mode mode, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onNetworkModeChanged(ZegoNetworkMode(mode));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_network_speed_test_error(int error_code, enum zego_network_speed_test_type type, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onNetworkSpeedTestError(error_code, ZegoNetworkSpeedTestType(type));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_network_speed_test_quality_update(const struct zego_network_speed_test_quality _quality, enum zego_network_speed_test_type type, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                auto handler = oInternalCallbackCenter->getIZegoEventHandler();
                auto weakHandler = std::weak_ptr<IZegoEventHandler>(handler);
                ZegoNetworkSpeedTestQuality quality;
                quality.connectCost = _quality.connect_cost;
                quality.packetLostRate = _quality.packet_lost_rate;
                quality.rtt = _quality.rtt;
                ZEGO_SWITCH_THREAD_PRE
                auto handler = weakHandler.lock();
                if (handler)
                    handler->onNetworkSpeedTestQualityUpdate(quality, ZegoNetworkSpeedTestType(type));
                ZEGO_SWITCH_THREAD_ING
            }

            static void zego_on_test_network_connectivity(zego_seq seq, zego_error error_code, const struct zego_test_network_connectivity_result _result, void *user_context)
            {
                ZEGO_UNUSED_VARIABLE(user_context);
                ZegoTestNetworkConnectivityResult result;
                result.connectCost = _result.connect_cost;
                auto callback = oInternalCallbackCenter->eraseZegoTestNetworkConnectivityCallback(seq);
                if (callback)
                {
                    ZEGO_SWITCH_THREAD_PRE
                    callback(error_code, result);
                    ZEGO_SWITCH_THREAD_ING
                }
            }

			static void zego_on_network_probe_result(zego_seq seq, zego_error error_code, const struct zego_network_probe_result _result, void * user_context)
			{
				ZEGO_UNUSED_VARIABLE(user_context);
				ZegoNetworkProbeHttpResult httpResult;
				ZegoNetworkProbeTcpResult tcpResult;
				ZegoNetworkProbeUdpResult udpResult;
				ZegoNetworkProbeTracerouteResult tracerouteResult;
				bool httpResultValid = false;
				bool tcpResultValid = false;
				bool udpResultValid = false;
				bool tracerouteResultValid = false;
				if (_result.http_probe_result) {
					httpResultValid = true;
					httpResult.errorCode = _result.http_probe_result->error_code;
					httpResult.requestCostTime = _result.http_probe_result->request_cost_time;
				}
				if (_result.tcp_probe_result) {
					tcpResultValid = true;
					tcpResult.connectCostTime = _result.tcp_probe_result->connect_cost_time;
					tcpResult.rtt = _result.tcp_probe_result->rtt;
					tcpResult.errorCode = _result.tcp_probe_result->error_code;
				}
				if (_result.udp_probe_result) {
					udpResultValid = true;
					udpResult.errorCode = _result.udp_probe_result->error_code;
					udpResult.rtt = _result.udp_probe_result->rtt;
				}
				if (_result.traceroute_result) {
					tracerouteResultValid = true;
					tracerouteResult.errorCode = _result.traceroute_result->error_code;
					tracerouteResult.tracerouteCostTime = _result.traceroute_result->traceroute_cost_time;
				}
				
				auto callback = oInternalCallbackCenter->eraseZegoNetworkProbeResultCallback(seq);
				if (callback)
				{
					ZEGO_SWITCH_THREAD_PRE
					auto httpResultTemp = httpResult;
					auto tcpResultTemp = tcpResult;
					auto udpResultTemp = udpResult;
					auto traceRouteResultTemp = tracerouteResult;
					ZegoNetworkProbeResult result;
					result.httpProbeResult = httpResultValid ? &httpResultTemp : nullptr;
					result.tcpProbeResult = tcpResultValid ? &tcpResultTemp : nullptr;
					result.udpProbeResult = udpResultValid ? &udpResultTemp : nullptr;
					result.tracerouteResult = tracerouteResultValid ? &traceRouteResultTemp : nullptr;
					callback(error_code, result);
					ZEGO_SWITCH_THREAD_ING
				}
			}

        };

    } // namespace EXPRESS
} // namespace ZEGO

ZEGO_ENABLE_DEPRECATION_WARNINGS
