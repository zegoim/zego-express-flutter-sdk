#include "include/zego_express_engine/zego_express_engine_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>
#include <map>

#include "internal/ZegoExpressEngineEventHandler.h"
#include "internal/ZegoExpressEngineMethodHandler.h"
#include "zego_express_engine_plugin_private.h"

#define ZEGO_EXPRESS_ENGINE_PLUGIN(obj)                                                            \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), zego_express_engine_plugin_get_type(),                      \
                                ZegoExpressEnginePlugin))

struct _ZegoExpressEnginePlugin {
    GObject parent_instance;
};

G_DEFINE_TYPE(ZegoExpressEnginePlugin, zego_express_engine_plugin, g_object_get_type())

#define EngineMethodHandler(funcName)                                                              \
    {                                                                                              \
        #funcName, {                                                                               \
            std::bind(&ZegoExpressEngineMethodHandler::funcName,                                   \
                      &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1,       \
                      std::placeholders::_2),                                                      \
                false                                                                              \
        }                                                                                          \
    }

#define EngineStaticMethodHandler(funcName)                                                        \
    {                                                                                              \
        #funcName, {                                                                               \
            std::bind(&ZegoExpressEngineMethodHandler::funcName,                                   \
                      &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1,       \
                      std::placeholders::_2),                                                      \
                true                                                                               \
        }                                                                                          \
    }

static gboolean send_events = FALSE;
static const std::map<std::string,
                      std::pair<std::function<void(FTArgument argument, FTResult result)>, bool>>

    G_MethodMap = {
        EngineStaticMethodHandler(getVersion),
        EngineStaticMethodHandler(isFeatureSupported),
        EngineStaticMethodHandler(setPluginVersion),
        EngineStaticMethodHandler(getAssetAbsolutePath),
        EngineStaticMethodHandler(createEngine),
        EngineStaticMethodHandler(createEngineWithProfile),
        EngineStaticMethodHandler(destroyEngine),
        EngineStaticMethodHandler(setEngineConfig),
        EngineStaticMethodHandler(setLogConfig),
        EngineStaticMethodHandler(setLocalProxyConfig),
        EngineStaticMethodHandler(setCloudProxyConfig),
        EngineStaticMethodHandler(setRoomMode),
        EngineStaticMethodHandler(setLicense),
        EngineStaticMethodHandler(setGeoFence),
        EngineStaticMethodHandler(submitLog),
        EngineMethodHandler(setRoomScenario),
        EngineMethodHandler(uploadLog),
        EngineMethodHandler(enableDebugAssistant),
        EngineMethodHandler(callExperimentalAPI),
        EngineMethodHandler(setDummyCaptureImagePath),

        EngineMethodHandler(loginRoom),
        EngineMethodHandler(logoutRoom),
        EngineMethodHandler(switchRoom),
        EngineMethodHandler(renewToken),
        EngineMethodHandler(setRoomExtraInfo),
        EngineMethodHandler(getRoomStreamList),
        EngineMethodHandler(startPublishingStream),
        EngineMethodHandler(stopPublishingStream),
        EngineMethodHandler(setStreamExtraInfo),
        EngineMethodHandler(startPreview),
        EngineMethodHandler(stopPreview),
        EngineMethodHandler(setVideoConfig),
        EngineMethodHandler(getVideoConfig),
        EngineMethodHandler(setPublishDualStreamConfig),
        EngineMethodHandler(setVideoMirrorMode),
        EngineMethodHandler(setAudioConfig),
        EngineMethodHandler(getAudioConfig),
        EngineMethodHandler(setPublishStreamEncryptionKey),
        EngineMethodHandler(takePublishStreamSnapshot),

        EngineMethodHandler(setMinVideoBitrateForTrafficControl),
        EngineMethodHandler(setMinVideoFpsForTrafficControl),
        EngineMethodHandler(setMinVideoResolutionForTrafficControl),
        EngineMethodHandler(setTrafficControlFocusOn),
        EngineMethodHandler(addPublishCdnUrl),
        EngineMethodHandler(removePublishCdnUrl),
        EngineMethodHandler(enablePublishDirectToCDN),
        EngineMethodHandler(setPublishWatermark),
        EngineMethodHandler(setPlayStreamDecryptionKey),
        EngineMethodHandler(setPlayStreamVideoType),
        EngineMethodHandler(setPlayStreamBufferIntervalRange),
        EngineMethodHandler(setPlayStreamFocusOn),
        EngineMethodHandler(mutePlayStreamVideo),
        EngineMethodHandler(muteAllPlayStreamVideo),
        EngineMethodHandler(muteAllPlayVideoStreams),
        EngineMethodHandler(enableCheckPoc),
        EngineMethodHandler(isVideoDecoderSupported),
        EngineMethodHandler(setPlayStreamsAlignmentProperty),
        EngineMethodHandler(enableVideoSuperResolution),
        EngineMethodHandler(initVideoSuperResolution),
        EngineMethodHandler(uninitVideoSuperResolution),
        EngineMethodHandler(updatePlayingCanvas),

        EngineMethodHandler(startAutoMixerTask),
        EngineMethodHandler(stopAutoMixerTask),
        EngineMethodHandler(setAudioRouteToSpeaker),
        EngineMethodHandler(getAudioRouteType),
        EngineMethodHandler(useFrontCamera),
        EngineMethodHandler(isCameraFocusSupported),
        EngineMethodHandler(setCameraFocusMode),
        EngineMethodHandler(setCameraFocusPointInPreview),
        EngineMethodHandler(setCameraExposureMode),
        EngineMethodHandler(setCameraExposurePointInPreview),
        EngineMethodHandler(setCameraExposureCompensation),
        EngineMethodHandler(setCameraZoomFactor),
        EngineMethodHandler(getCameraMaxZoomFactor),
        EngineMethodHandler(startAudioSpectrumMonitor),
        EngineMethodHandler(stopAudioSpectrumMonitor),
        EngineMethodHandler(enableHeadphoneAEC),
        EngineMethodHandler(enableAGC),
        EngineMethodHandler(startEffectsEnv),
        EngineMethodHandler(stopEffectsEnv),
        EngineMethodHandler(enableEffectsBeauty),
        EngineMethodHandler(setEffectsBeautyParam),
        EngineMethodHandler(enableBeautify),
        EngineMethodHandler(setBeautifyOption),

        EngineMethodHandler(sendBroadcastMessage),
        EngineMethodHandler(sendBarrageMessage),
        EngineMethodHandler(sendCustomCommand),
        EngineMethodHandler(sendTransparentMessage),
        EngineMethodHandler(enableCustomVideoCapture),
        EngineMethodHandler(enableCustomVideoProcessing),
        EngineMethodHandler(enableCustomVideoRender),
        EngineMethodHandler(enableCustomAudioCaptureProcessing),
        EngineMethodHandler(enableCustomAudioCaptureProcessingAfterHeadphoneMonitor),
        EngineMethodHandler(enableAlignedAudioAuxData),
        EngineMethodHandler(enableCustomAudioRemoteProcessing),
        EngineMethodHandler(enableCustomAudioPlaybackProcessing),
        EngineMethodHandler(stopAudioDataObserver),
        EngineMethodHandler(enableCustomAudioIO),
        EngineMethodHandler(sendCustomAudioCaptureAACData),
        EngineMethodHandler(sendCustomAudioCapturePCMData),
        EngineMethodHandler(fetchCustomAudioRenderPCMData),

        EngineMethodHandler(startPerformanceMonitor),
        EngineMethodHandler(stopPerformanceMonitor),
        EngineMethodHandler(startNetworkProbe),
        EngineMethodHandler(stopNetworkProbe),
        EngineMethodHandler(startNetworkSpeedTest),
        EngineMethodHandler(stopNetworkSpeedTest),
        EngineMethodHandler(getNetworkTimeInfo),

        EngineMethodHandler(mutePublishStreamAudio),
        EngineMethodHandler(mutePublishStreamVideo),
        EngineMethodHandler(setCaptureVolume),
        EngineMethodHandler(setAudioCaptureStereoMode),
        EngineMethodHandler(sendSEI),
        EngineMethodHandler(sendAudioSideInfo),
        EngineMethodHandler(enableHardwareEncoder),
        EngineMethodHandler(setCapturePipelineScaleMode),
        EngineMethodHandler(enableH265EncodeFallback),
        EngineMethodHandler(isVideoEncoderSupported),
        EngineMethodHandler(setLowlightEnhancement),
        EngineMethodHandler(setVideoSource),
        EngineMethodHandler(setAudioSource),
        EngineMethodHandler(enableVideoObjectSegmentation),
        EngineMethodHandler(enableAlphaChannelVideoEncoder),

        EngineMethodHandler(startPlayingStream),
        EngineMethodHandler(stopPlayingStream),
        EngineMethodHandler(setPlayStreamCrossAppInfo),
        EngineMethodHandler(takePlayStreamSnapshot),
        EngineMethodHandler(setPlayVolume),
        EngineMethodHandler(setAllPlayStreamVolume),
        EngineMethodHandler(mutePlayStreamAudio),
        EngineMethodHandler(muteAllPlayStreamAudio),
        EngineMethodHandler(muteAllPlayAudioStreams),
        EngineMethodHandler(enableHardwareDecoder),
        EngineMethodHandler(muteMicrophone),
        EngineMethodHandler(isMicrophoneMuted),
        EngineMethodHandler(muteSpeaker),
        EngineMethodHandler(isSpeakerMuted),
        EngineMethodHandler(getAudioDeviceList),
        EngineMethodHandler(getDefaultAudioDeviceID),
        EngineMethodHandler(useAudioDevice),
        EngineMethodHandler(startSoundLevelMonitor),
        EngineMethodHandler(stopSoundLevelMonitor),
        EngineMethodHandler(enableHeadphoneMonitor),
        EngineMethodHandler(setHeadphoneMonitorVolume),
        EngineMethodHandler(setMixSystemPlayoutVolume),
        EngineMethodHandler(enableMixEnginePlayout),
        EngineMethodHandler(enableMixSystemPlayout),
        EngineMethodHandler(enableAEC),
        EngineMethodHandler(setAECMode),
        EngineMethodHandler(enableANS),
        EngineMethodHandler(enableTransientANS),
        EngineMethodHandler(setANSMode),
        EngineMethodHandler(enableSpeechEnhance),
        EngineMethodHandler(setAudioEqualizerGain),
        EngineMethodHandler(setVoiceChangerPreset),
        EngineMethodHandler(setVoiceChangerParam),
        EngineMethodHandler(setReverbPreset),
        EngineMethodHandler(setReverbAdvancedParam),
        EngineMethodHandler(setReverbEchoParam),
        EngineMethodHandler(enableVirtualStereo),
        EngineMethodHandler(enablePlayStreamVirtualStereo),
        EngineMethodHandler(setElectronicEffects),
        EngineMethodHandler(startMixerTask),
        EngineMethodHandler(stopMixerTask),
        EngineMethodHandler(setSEIConfig),
        EngineMethodHandler(setAudioDeviceVolume),
        EngineMethodHandler(setSpeakerVolumeInAPP),
        EngineMethodHandler(getSpeakerVolumeInAPP),
        EngineMethodHandler(startAudioDeviceVolumeMonitor),
        EngineMethodHandler(stopAudioDeviceVolumeMonitor),
        EngineMethodHandler(muteAudioDevice),
        EngineMethodHandler(isAudioDeviceMuted),
        EngineMethodHandler(setAudioDeviceMode),
        EngineMethodHandler(getAudioDeviceVolume),
        EngineMethodHandler(enableAudioCaptureDevice),
        EngineMethodHandler(enableTrafficControl),
        EngineMethodHandler(startRecordingCapturedData),
        EngineMethodHandler(stopRecordingCapturedData),
        EngineMethodHandler(startAudioDataObserver),
        EngineMethodHandler(enableCamera),
        EngineMethodHandler(enableCameraAdaptiveFPS),
        EngineMethodHandler(useVideoDevice),
        EngineMethodHandler(getVideoDeviceList),
        EngineMethodHandler(getDefaultVideoDeviceID),
        EngineMethodHandler(enableMixSystemPlayout),
        EngineMethodHandler(startAudioVADStableStateMonitor),
        EngineMethodHandler(stopAudioVADStableStateMonitor),
        EngineMethodHandler(getCurrentAudioDevice),
        EngineMethodHandler(startDumpData),
        EngineMethodHandler(stopDumpData),
        EngineMethodHandler(uploadDumpData),
        EngineMethodHandler(removeDumpData),
        EngineMethodHandler(enableColorEnhancement),

        EngineMethodHandler(createAudioEffectPlayer),
        EngineMethodHandler(destroyAudioEffectPlayer),
        EngineMethodHandler(audioEffectPlayerStart),
        EngineMethodHandler(audioEffectPlayerStop),
        EngineMethodHandler(audioEffectPlayerPause),
        EngineMethodHandler(audioEffectPlayerResume),
        EngineMethodHandler(audioEffectPlayerStopAll),
        EngineMethodHandler(audioEffectPlayerPauseAll),
        EngineMethodHandler(audioEffectPlayerResumeAll),
        EngineMethodHandler(audioEffectPlayerSeekTo),
        EngineMethodHandler(audioEffectPlayerSetVolume),
        EngineMethodHandler(audioEffectPlayerSetPlayVolume),
        EngineMethodHandler(audioEffectPlayerSetPublishVolume),
        EngineMethodHandler(audioEffectPlayerSetVolumeAll),
        EngineMethodHandler(audioEffectPlayerSetPlayVolumeAll),
        EngineMethodHandler(audioEffectPlayerSetPublishVolumeAll),
        EngineMethodHandler(audioEffectPlayerGetTotalDuration),
        EngineMethodHandler(audioEffectPlayerGetCurrentProgress),
        EngineMethodHandler(audioEffectPlayerLoadResource),
        EngineMethodHandler(audioEffectPlayerUnloadResource),
        EngineMethodHandler(audioEffectPlayerSetPlaySpeed),
        EngineMethodHandler(audioEffectPlayerUpdatePosition),

        EngineMethodHandler(createMediaPlayer),
        EngineMethodHandler(destroyMediaPlayer),
        EngineMethodHandler(mediaPlayerLoadResource),
        EngineMethodHandler(mediaPlayerLoadResourceFromMediaData),
        EngineMethodHandler(mediaPlayerLoadResourceWithPosition),
        EngineMethodHandler(mediaPlayerStart),
        EngineMethodHandler(mediaPlayerStop),
        EngineMethodHandler(mediaPlayerPause),
        EngineMethodHandler(mediaPlayerResume),
        EngineMethodHandler(mediaPlayerSeekTo),
        EngineMethodHandler(mediaPlayerEnableRepeat),
        EngineMethodHandler(mediaPlayerEnableAux),
        EngineMethodHandler(mediaPlayerMuteLocal),
        EngineMethodHandler(mediaPlayerSetVolume),
        EngineMethodHandler(mediaPlayerSetPlayVolume),
        EngineMethodHandler(mediaPlayerSetPublishVolume),
        EngineMethodHandler(mediaPlayerSetProgressInterval),
        EngineMethodHandler(mediaPlayerGetPlayVolume),
        EngineMethodHandler(mediaPlayerGetPublishVolume),
        EngineMethodHandler(mediaPlayerGetTotalDuration),
        EngineMethodHandler(mediaPlayerGetCurrentProgress),
        EngineMethodHandler(mediaPlayerGetAudioTrackCount),
        EngineMethodHandler(mediaPlayerSetAudioTrackIndex),
        EngineMethodHandler(mediaPlayerSetVoiceChangerParam),
        EngineMethodHandler(mediaPlayerGetCurrentState),
        EngineMethodHandler(mediaPlayerEnableAccurateSeek),
        EngineMethodHandler(mediaPlayerGetNetWorkResourceCache),
        EngineMethodHandler(mediaPlayerSetNetWorkBufferThreshold),
        EngineMethodHandler(mediaPlayerSetNetWorkResourceMaxCache),
        EngineMethodHandler(mediaPlayerSetPlaySpeed),
        EngineMethodHandler(mediaPlayerEnableFrequencySpectrumMonitor),
        EngineMethodHandler(mediaPlayerEnableSoundLevelMonitor),
        EngineMethodHandler(mediaPlayerLoadCopyrightedMusicResourceWithPosition),
        EngineMethodHandler(mediaPlayerClearView),
        EngineMethodHandler(mediaPlayerSetActiveAudioChannel),
        EngineMethodHandler(mediaPlayerSetPlayerCanvas),
        EngineMethodHandler(mediaPlayerTakeSnapshot),
        EngineMethodHandler(mediaPlayerSetAudioTrackMode),
        EngineMethodHandler(mediaPlayerSetAudioTrackPublishIndex),
        EngineMethodHandler(mediaPlayerEnableAudioData),
        EngineMethodHandler(mediaPlayerEnableVideoData),
        EngineMethodHandler(mediaPlayerEnableBlockData),
        EngineMethodHandler(mediaPlayerLoadResourceWithConfig),
        EngineMethodHandler(mediaPlayerUpdatePosition),
        EngineMethodHandler(mediaPlayerGetMediaInfo),
        EngineMethodHandler(mediaPlayerSetHttpHeader),
        EngineMethodHandler(mediaPlayerGetCurrentRenderingProgress),
        EngineMethodHandler(mediaPlayerEnableLiveAudioEffect),
        EngineMethodHandler(mediaPlayerEnableViewMirror),
        EngineMethodHandler(mediaPlayerSetPlayMediaStreamType),
        EngineMethodHandler(mediaPlayerGetPlaybackStatistics),

        // MediaDataPublihser
        EngineMethodHandler(createMediaDataPublisher),
        EngineMethodHandler(destroyMediaDataPublisher),
        EngineMethodHandler(mediaDataPublisherAddMediaFilePath),
        EngineMethodHandler(mediaDataPublisherGetCurrentDuration),
        EngineMethodHandler(mediaDataPublisherGetTotalDuration),
        EngineMethodHandler(mediaDataPublisherReset),
        EngineMethodHandler(mediaDataPublisherSeekTo),
        EngineMethodHandler(mediaDataPublisherSetVideoSendDelayTime),

        // Copyrighted Music
        EngineMethodHandler(createCopyrightedMusic),
        EngineMethodHandler(destroyCopyrightedMusic),
        EngineMethodHandler(copyrightedMusicClearCache),
        EngineMethodHandler(copyrightedMusicDownload),
        EngineMethodHandler(copyrightedMusicCancelDownload),
        EngineMethodHandler(copyrightedMusicGetAverageScore),
        EngineMethodHandler(copyrightedMusicGetCacheSize),
        EngineMethodHandler(copyrightedMusicGetCurrentPitch),
        EngineMethodHandler(copyrightedMusicGetDuration),
        EngineMethodHandler(copyrightedMusicGetKrcLyricByToken),
        EngineMethodHandler(copyrightedMusicGetLrcLyric),
        EngineMethodHandler(copyrightedMusicGetLrcLyricWithConfig),
        EngineMethodHandler(copyrightedMusicGetMusicByToken),
        EngineMethodHandler(copyrightedMusicGetPreviousScore),
        EngineMethodHandler(copyrightedMusicGetStandardPitch),
        EngineMethodHandler(copyrightedMusicGetTotalScore),
        EngineMethodHandler(copyrightedMusicInitCopyrightedMusic),
        EngineMethodHandler(copyrightedMusicPauseScore),
        EngineMethodHandler(copyrightedMusicQueryCache),
        EngineMethodHandler(copyrightedMusicQueryCacheWithConfig),
        EngineMethodHandler(copyrightedMusicQueryCacheWithConfigV2),
        EngineMethodHandler(copyrightedMusicRequestAccompaniment),
        EngineMethodHandler(copyrightedMusicRequestAccompanimentClip),
        EngineMethodHandler(copyrightedMusicRequestSong),
        EngineMethodHandler(copyrightedMusicResetScore),
        EngineMethodHandler(copyrightedMusicResumeScore),
        EngineMethodHandler(copyrightedMusicSendExtendedRequest),
        EngineMethodHandler(copyrightedMusicSetScoringLevel),
        EngineMethodHandler(copyrightedMusicStartScore),
        EngineMethodHandler(copyrightedMusicStopScore),
        EngineMethodHandler(copyrightedMusicGetFullScore),
        EngineMethodHandler(copyrightedMusicGetSharedResource),
        EngineMethodHandler(copyrightedMusicGetSharedResourceV2),
        EngineMethodHandler(copyrightedMusicRequestResource),
        EngineMethodHandler(copyrightedMusicRequestResourceV2),

        // rangeAudio
        EngineMethodHandler(createRangeAudio),
        EngineMethodHandler(destroyRangeAudio),
        EngineMethodHandler(rangeAudioEnableMicrophone),
        EngineMethodHandler(rangeAudioEnableSpatializer),
        EngineMethodHandler(rangeAudioEnableSpeaker),
        EngineMethodHandler(rangeAudioSetAudioReceiveRange),
        EngineMethodHandler(rangeAudioSetMode),
        EngineMethodHandler(rangeAudioSetTeamID),
        EngineMethodHandler(rangeAudioUpdateAudioSource),
        EngineMethodHandler(rangeAudioUpdateSelfPosition),
        EngineMethodHandler(rangeAudioMuteUser),
        EngineMethodHandler(rangeAudioSetPositionUpdateFrequency),
        EngineMethodHandler(rangeAudioSetRangeAudioVolume),
        EngineMethodHandler(rangeAudioSetStreamVocalRange),
        EngineMethodHandler(rangeAudioUpdateStreamPosition),
        EngineMethodHandler(rangeAudioSetRangeAudioCustomMode),

        // RealTimeSequentialDataManager
        EngineMethodHandler(createRealTimeSequentialDataManager),
        EngineMethodHandler(destroyRealTimeSequentialDataManager),
        EngineMethodHandler(dataManagerSendRealTimeSequentialData),
        EngineMethodHandler(dataManagerStartBroadcasting),
        EngineMethodHandler(dataManagerStartSubscribing),
        EngineMethodHandler(dataManagerStopBroadcasting),
        EngineMethodHandler(dataManagerStopSubscribing),

        // ScreenCapture
        EngineMethodHandler(getScreenCaptureSources),
        EngineMethodHandler(createScreenCaptureSource),
        EngineMethodHandler(destroyScreenCaptureSource),
        EngineMethodHandler(enableCursorVisibleScreenCaptureSource),
        EngineMethodHandler(enableWindowActivateScreenCaptureSource),
        EngineMethodHandler(setExcludeWindowListScreenCaptureSource),
        EngineMethodHandler(updateCaptureRegionScreenCaptureSource),
        EngineMethodHandler(updatePublishRegionScreenCaptureSource),
        EngineMethodHandler(updateCaptureSourceScreenCaptureSource),
        EngineMethodHandler(startCaptureScreenCaptureSource),
        EngineMethodHandler(stopCaptureScreenCaptureSource),
        EngineMethodHandler(getCaptureSourceRectScreenCaptureSource),
        EngineMethodHandler(enableAudioCaptureScreenCaptureSource),

        // AIVoiceChanger
        EngineMethodHandler(createAIVoiceChanger),
        EngineMethodHandler(destroyAIVoiceChanger),
        EngineMethodHandler(isAIVoiceChangerSupported),
        EngineMethodHandler(aiVoiceChangerGetSpeakerList),
        EngineMethodHandler(aiVoiceChangerInitEngine),
        EngineMethodHandler(aiVoiceChangerSetSpeaker),
        EngineMethodHandler(aiVoiceChangerUpdate),

        // textureRenderer
        EngineMethodHandler(createTextureRenderer),
        EngineMethodHandler(destroyTextureRenderer),
};

// Called when a method call is received from Flutter.
static void zego_express_engine_plugin_handle_method_call(ZegoExpressEnginePlugin *self,
                                                          FlMethodCall *method_call) {

    std::unique_ptr<FlResult> result(new FlResult(method_call));

    const gchar *method = fl_method_call_get_name(method_call);
    FlValue *args = fl_method_call_get_args(method_call);
    FlValueMap argument(args);

    auto pair = G_MethodMap.find(std::string(method));
    if (pair != G_MethodMap.end()) {
        try {
            if (!pair->second.second &&
                !ZegoExpressEngineMethodHandler::getInstance().isEngineCreated()) {
                fl_method_call_respond_error(method_call, "Engine_not_created",
                                             "Please call createEngineWithProfile first", nullptr,
                                             nullptr);
            } else {
                pair->second.first(argument, std::move(result));
            }
        } catch (std::exception &e) {
            fl_method_call_respond_error(method_call, "method_call_error", e.what(), nullptr,
                                         nullptr);
        }
    } else {
        fl_method_call_respond_not_implemented(method_call, nullptr);
    }
}

FlMethodResponse *get_platform_version() {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
}

static void zego_express_engine_plugin_dispose(GObject *object) {
    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    // ZegoExpressEngineMethodHandler::getInstance().clearPluginRegistrar();
    G_OBJECT_CLASS(zego_express_engine_plugin_parent_class)->dispose(object);
}

static void zego_express_engine_plugin_class_init(ZegoExpressEnginePluginClass *klass) {
    G_OBJECT_CLASS(klass)->dispose = zego_express_engine_plugin_dispose;
}

static void zego_express_engine_plugin_init(ZegoExpressEnginePlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data) {
    ZegoExpressEnginePlugin *plugin = ZEGO_EXPRESS_ENGINE_PLUGIN(user_data);
    zego_express_engine_plugin_handle_method_call(plugin, method_call);
}

static FlMethodErrorResponse *listen_cb(FlEventChannel *channel, FlValue *args,
                                        gpointer user_data) {
    send_events = TRUE;
    std::unique_ptr<FlEventSink> events(new FlEventSink(channel));
    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    return NULL;
}

static FlMethodErrorResponse *cancel_cb(FlEventChannel *channel, FlValue *args,
                                        gpointer user_data) {
    send_events = FALSE;
    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    return NULL;
}

void zego_express_engine_plugin_register_with_registrar(FlPluginRegistrar *registrar) {
    ZegoExpressEnginePlugin *plugin =
        ZEGO_EXPRESS_ENGINE_PLUGIN(g_object_new(zego_express_engine_plugin_get_type(), nullptr));

    g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
    g_autoptr(FlBinaryMessenger) messenger = fl_plugin_registrar_get_messenger(registrar);
    g_autoptr(FlMethodChannel) method_channel = fl_method_channel_new(
        messenger, "plugins.zego.im/zego_express_engine", FL_METHOD_CODEC(codec));
    fl_method_channel_set_method_call_handler(method_channel, method_call_cb, g_object_ref(plugin),
                                              g_object_unref);
    FlEventChannel *event_channel = fl_event_channel_new(
        messenger, "plugins.zego.im/zego_express_event_handler", FL_METHOD_CODEC(codec));
    fl_event_channel_set_stream_handlers(event_channel, listen_cb, cancel_cb, NULL, NULL);

    FlTextureRegistrar* texture_registrar = fl_plugin_registrar_get_texture_registrar(registrar);
    FlView* view = fl_plugin_registrar_get_view(registrar);

    g_object_unref(plugin);
}

gboolean is_platform_thread() {
    GMainContext* current_context = g_main_context_get_thread_default();
    return current_context == g_main_context_default();
}