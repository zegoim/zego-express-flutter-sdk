#include "include/zego_express_engine/zego_express_engine_plugin.h"

#include <windows.h>

#include <VersionHelpers.h>

#include <flutter/event_channel.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "internal/ZegoExpressEngineEventHandler.h"
#include "internal/ZegoExpressEngineMethodHandler.h"

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

static const std::map<
    std::string,
    std::pair<
        std::function<void(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)>,
        bool>>
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
        EngineMethodHandler(startPublishingStream),
        EngineMethodHandler(stopPublishingStream),
        EngineMethodHandler(setStreamExtraInfo),
        EngineMethodHandler(startPreview),
        EngineMethodHandler(stopPreview),
        EngineMethodHandler(setVideoConfig),
        EngineMethodHandler(getVideoConfig),
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
        EngineMethodHandler(enableCheckPoc),
        EngineMethodHandler(isVideoDecoderSupported),
        EngineMethodHandler(setPlayStreamsAlignmentProperty),
        EngineMethodHandler(enableVideoSuperResolution),
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
        EngineMethodHandler(enableCustomVideoCapture),
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

        EngineMethodHandler(startPlayingStream),
        EngineMethodHandler(stopPlayingStream),
        EngineMethodHandler(setPlayStreamCrossAppInfo),
        EngineMethodHandler(takePlayStreamSnapshot),
        EngineMethodHandler(setPlayVolume),
        EngineMethodHandler(setAllPlayStreamVolume),
        EngineMethodHandler(mutePlayStreamAudio),
        EngineMethodHandler(muteAllPlayStreamAudio),
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
        EngineMethodHandler(audioEffectPlayerSetVolumeAll),
        EngineMethodHandler(audioEffectPlayerGetTotalDuration),
        EngineMethodHandler(audioEffectPlayerGetCurrentProgress),
        EngineMethodHandler(audioEffectPlayerLoadResource),
        EngineMethodHandler(audioEffectPlayerUnloadResource),
        EngineMethodHandler(audioEffectPlayerSetPlaySpeed),

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

        // Copyrighted Music
        EngineMethodHandler(createCopyrightedMusic),
        EngineMethodHandler(destroyCopyrightedMusic),
        EngineMethodHandler(copyrightedMusicClearCache),
        EngineMethodHandler(copyrightedMusicDownload),
        EngineMethodHandler(copyrightedMusicGetAverageScore),
        EngineMethodHandler(copyrightedMusicGetCacheSize),
        EngineMethodHandler(copyrightedMusicGetCurrentPitch),
        EngineMethodHandler(copyrightedMusicGetDuration),
        EngineMethodHandler(copyrightedMusicGetKrcLyricByToken),
        EngineMethodHandler(copyrightedMusicGetLrcLyric),
        EngineMethodHandler(copyrightedMusicGetMusicByToken),
        EngineMethodHandler(copyrightedMusicGetPreviousScore),
        EngineMethodHandler(copyrightedMusicGetStandardPitch),
        EngineMethodHandler(copyrightedMusicGetTotalScore),
        EngineMethodHandler(copyrightedMusicInitCopyrightedMusic),
        EngineMethodHandler(copyrightedMusicPauseScore),
        EngineMethodHandler(copyrightedMusicQueryCache),
        EngineMethodHandler(copyrightedMusicRequestAccompaniment),
        EngineMethodHandler(copyrightedMusicRequestAccompanimentClip),
        EngineMethodHandler(copyrightedMusicRequestSong),
        EngineMethodHandler(copyrightedMusicResetScore),
        EngineMethodHandler(copyrightedMusicResumeScore),
        EngineMethodHandler(copyrightedMusicSendExtendedRequest),
        EngineMethodHandler(copyrightedMusicStartScore),
        EngineMethodHandler(copyrightedMusicStopScore),
        EngineMethodHandler(copyrightedMusicGetFullScore),
        EngineMethodHandler(copyrightedMusicGetSharedResource),
        EngineMethodHandler(copyrightedMusicRequestResource),

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
        EngineMethodHandler(updateCaptureSourceScreenCaptureSource),
        EngineMethodHandler(startCaptureScreenCaptureSource),
        EngineMethodHandler(stopCaptureScreenCaptureSource),

        // textureRenderer
        EngineMethodHandler(createTextureRenderer),
        EngineMethodHandler(destroyTextureRenderer),
};

class ZegoExpressEnginePlugin : public flutter::Plugin,
                                public flutter::StreamHandler<flutter::EncodableValue> {

  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    ZegoExpressEnginePlugin();

    virtual ~ZegoExpressEnginePlugin();

  protected:
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListenInternal(
        const flutter::EncodableValue *arguments,
        std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events) override;
    // Implementation of the public interface, to be provided by subclasses.
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
    OnCancelInternal(const flutter::EncodableValue *arguments) override;

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  private:
  private:
    std::shared_ptr<ZegoExpressEngineEventHandler> eventHandler_;
};

// static
void ZegoExpressEnginePlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {
    ZegoExpressEngineMethodHandler::getInstance().setPluginRegistrar(registrar);

    auto methodChannel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
        registrar->messenger(), "plugins.zego.im/zego_express_engine",
        &flutter::StandardMethodCodec::GetInstance());

    auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
        registrar->messenger(), "plugins.zego.im/zego_express_event_handler",
        &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<ZegoExpressEnginePlugin>();

    eventChannel->SetStreamHandler(std::move(plugin));

    methodChannel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
            plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    ZegoExpressEngineMethodHandler::getInstance().initApiCalledCallback();

    registrar->AddPlugin(std::move(plugin));
}

ZegoExpressEnginePlugin::ZegoExpressEnginePlugin() {}

ZegoExpressEnginePlugin::~ZegoExpressEnginePlugin() {}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
ZegoExpressEnginePlugin::OnListenInternal(
    const flutter::EncodableValue *arguments,
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events) {

    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    std::cout << "on listen event" << std::endl;

    return nullptr;
}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
ZegoExpressEnginePlugin::OnCancelInternal(const flutter::EncodableValue *arguments) {

    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    std::cout << "on cancel listen event" << std::endl;

    return nullptr;
}

void ZegoExpressEnginePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    flutter::EncodableMap argument;
    if (std::holds_alternative<flutter::EncodableMap>(*method_call.arguments())) {
        argument = std::get<flutter::EncodableMap>(*method_call.arguments());
    }

    auto pair = G_MethodMap.find(method_call.method_name());
    if (pair != G_MethodMap.end()) {
        try {
            if (!pair->second.second &&
                !ZegoExpressEngineMethodHandler::getInstance().isEngineCreated()) {
                result->Error("Engine_not_created", "Please call createEngineWithProfile first");
            } else {
                pair->second.first(argument, std::move(result));
            }
        } catch (std::exception &e) {
            result->Error("method_call_error", e.what());
        }
    } else {
        result->NotImplemented();
    }
}

void ZegoExpressEnginePluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    ZegoExpressEnginePlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()
            ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
