#pragma once

#include <functional>
#include <map>
#include <string>

#include "ZegoDataUtils.h"

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

static const std::map<std::string,
                      std::pair<std::function<void(ZFArgument argument, ZFResult result)>, bool>>

    G_MethodMap = {
        EngineStaticMethodHandler(getVersion),
        EngineStaticMethodHandler(isFeatureSupported),
        EngineStaticMethodHandler(setPluginVersion),
        EngineStaticMethodHandler(sendCustomLogMessage),
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
        EngineMethodHandler(setDummyCaptureImageParams),

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
        EngineMethodHandler(setLowlightEnhancementParams),
        EngineMethodHandler(setVideoSource),
        EngineMethodHandler(setAudioSource),
        EngineMethodHandler(enableVideoObjectSegmentation),
        EngineMethodHandler(enableAlphaChannelVideoEncoder),
        EngineMethodHandler(enableAuxBgmBalance),

        EngineMethodHandler(startPlayingStream),
        EngineMethodHandler(switchPlayingStream),
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
        EngineMethodHandler(mediaPlayerEnableVoiceChanger),
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