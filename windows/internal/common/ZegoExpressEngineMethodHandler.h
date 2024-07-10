#pragma once

#include <ZegoExpressSDK.h>

#ifdef _WIN32
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#else
#include <flutter_linux/flutter_linux.h>
#endif

#include "../ZegoDataUtils.h"

using namespace ZEGO;

class ZegoExpressEngineMethodHandler {
  public:
    ~ZegoExpressEngineMethodHandler() {}

    static ZegoExpressEngineMethodHandler &getInstance() {
        static ZegoExpressEngineMethodHandler m_instance;
        return m_instance;
    }

    void setPluginBinaryMessenger(FTBinaryMessenger *messenger) { messenger_ = messenger; }

    void setPluginTexture(FTTextureRegistrar *texture) { texture_ = texture; }

    void clearPluginRegistrar();

    void initApiCalledCallback();

    bool isEngineCreated();

  public:
    void getVersion(FTArgument argument, FTResult result);
    void isFeatureSupported(FTArgument argument, FTResult result);
    void setPluginVersion(FTArgument argument, FTResult result);
    void sendCustomLogMessage(FTArgument argument, FTResult result);
    void getAssetAbsolutePath(FTArgument argument, FTResult result);
    void createEngine(FTArgument argument, FTResult result);
    void createEngineWithProfile(FTArgument argument, FTResult result);
    void destroyEngine(FTArgument argument, FTResult result);
    void setEngineConfig(FTArgument argument, FTResult result);
    void setLogConfig(FTArgument argument, FTResult result);
    void setLocalProxyConfig(FTArgument argument, FTResult result);
    void setCloudProxyConfig(FTArgument argument, FTResult result);
    void setRoomMode(FTArgument argument, FTResult result);
    void setLicense(FTArgument argument, FTResult result);
    void setGeoFence(FTArgument argument, FTResult result);
    void setRoomScenario(FTArgument argument, FTResult result);
    void uploadLog(FTArgument argument, FTResult result);
    void submitLog(FTArgument argument, FTResult result);
    void enableDebugAssistant(FTArgument argument, FTResult result);
    void callExperimentalAPI(FTArgument argument, FTResult result);
    void setDummyCaptureImagePath(FTArgument argument, FTResult result);

    void loginRoom(FTArgument argument, FTResult result);
    void logoutRoom(FTArgument argument, FTResult result);
    void switchRoom(FTArgument argument, FTResult result);
    void renewToken(FTArgument argument, FTResult result);
    void setRoomExtraInfo(FTArgument argument, FTResult result);
    void getRoomStreamList(FTArgument argument, FTResult result);

    void startPublishingStream(FTArgument argument, FTResult result);
    void stopPublishingStream(FTArgument argument, FTResult result);
    void setStreamExtraInfo(FTArgument argument, FTResult result);
    void startPreview(FTArgument argument, FTResult result);
    void stopPreview(FTArgument argument, FTResult result);
    void setVideoConfig(FTArgument argument, FTResult result);
    void getVideoConfig(FTArgument argument, FTResult result);
    void setPublishDualStreamConfig(FTArgument argument, FTResult result);
    void setVideoMirrorMode(FTArgument argument, FTResult result);
    void setAudioConfig(FTArgument argument, FTResult result);
    void getAudioConfig(FTArgument argument, FTResult result);
    void setPublishStreamEncryptionKey(FTArgument argument, FTResult result);
    void takePublishStreamSnapshot(FTArgument argument, FTResult result);

    void setMinVideoBitrateForTrafficControl(FTArgument argument, FTResult result);
    void setMinVideoFpsForTrafficControl(FTArgument argument, FTResult result);
    void setMinVideoResolutionForTrafficControl(FTArgument argument, FTResult result);
    void setTrafficControlFocusOn(FTArgument argument, FTResult result);
    void addPublishCdnUrl(FTArgument argument, FTResult result);
    void removePublishCdnUrl(FTArgument argument, FTResult result);
    void enablePublishDirectToCDN(FTArgument argument, FTResult result);
    void setPublishWatermark(FTArgument argument, FTResult result);
    void setPlayStreamDecryptionKey(FTArgument argument, FTResult result);
    void setPlayStreamVideoType(FTArgument argument, FTResult result);
    void setPlayStreamBufferIntervalRange(FTArgument argument, FTResult result);
    void setPlayStreamFocusOn(FTArgument argument, FTResult result);
    void mutePlayStreamVideo(FTArgument argument, FTResult result);
    void muteAllPlayStreamVideo(FTArgument argument, FTResult result);
    void muteAllPlayVideoStreams(FTArgument argument, FTResult result);
    void enableCheckPoc(FTArgument argument, FTResult result);
    void isVideoDecoderSupported(FTArgument argument, FTResult result);
    void setPlayStreamsAlignmentProperty(FTArgument argument, FTResult result);
    void enableVideoSuperResolution(FTArgument argument, FTResult result);
    void initVideoSuperResolution(FTArgument argument, FTResult result);
    void uninitVideoSuperResolution(FTArgument argument, FTResult result);
    void updatePlayingCanvas(FTArgument argument, FTResult result);

    void startAutoMixerTask(FTArgument argument, FTResult result);
    void stopAutoMixerTask(FTArgument argument, FTResult result);
    void setAudioRouteToSpeaker(FTArgument argument, FTResult result);
    void getAudioRouteType(FTArgument argument, FTResult result);
    void useFrontCamera(FTArgument argument, FTResult result);
    void isCameraFocusSupported(FTArgument argument, FTResult result);
    void setCameraFocusMode(FTArgument argument, FTResult result);
    void setCameraFocusPointInPreview(FTArgument argument, FTResult result);
    void setCameraExposureMode(FTArgument argument, FTResult result);
    void setCameraExposurePointInPreview(FTArgument argument, FTResult result);
    void setCameraExposureCompensation(FTArgument argument, FTResult result);
    void setCameraZoomFactor(FTArgument argument, FTResult result);
    void getCameraMaxZoomFactor(FTArgument argument, FTResult result);
    void startAudioSpectrumMonitor(FTArgument argument, FTResult result);
    void stopAudioSpectrumMonitor(FTArgument argument, FTResult result);
    void enableHeadphoneAEC(FTArgument argument, FTResult result);
    void startEffectsEnv(FTArgument argument, FTResult result);
    void stopEffectsEnv(FTArgument argument, FTResult result);
    void enableEffectsBeauty(FTArgument argument, FTResult result);
    void setEffectsBeautyParam(FTArgument argument, FTResult result);
    void enableBeautify(FTArgument argument, FTResult result);
    void setBeautifyOption(FTArgument argument, FTResult result);
    void enableColorEnhancement(FTArgument argument, FTResult result);

    void sendBroadcastMessage(FTArgument argument, FTResult result);
    void sendBarrageMessage(FTArgument argument, FTResult result);
    void sendCustomCommand(FTArgument argument, FTResult result);
    void sendTransparentMessage(FTArgument argument, FTResult result);
    void enableCustomVideoCapture(FTArgument argument, FTResult result);
    void enableCustomVideoProcessing(FTArgument argument, FTResult result);
    void enableCustomVideoRender(FTArgument argument, FTResult result);
    void enableCustomAudioCaptureProcessing(FTArgument argument, FTResult result);
    void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(FTArgument argument,
                                                                 FTResult result);
    void enableAlignedAudioAuxData(FTArgument argument, FTResult result);
    void enableCustomAudioRemoteProcessing(FTArgument argument, FTResult result);
    void enableCustomAudioPlaybackProcessing(FTArgument argument, FTResult result);
    void enableCustomAudioIO(FTArgument argument, FTResult result);
    void sendCustomAudioCaptureAACData(FTArgument argument, FTResult result);
    void sendCustomAudioCapturePCMData(FTArgument argument, FTResult result);
    void fetchCustomAudioRenderPCMData(FTArgument argument, FTResult result);

    void startPerformanceMonitor(FTArgument argument, FTResult result);
    void stopPerformanceMonitor(FTArgument argument, FTResult result);
    void startNetworkProbe(FTArgument argument, FTResult result);
    void stopNetworkProbe(FTArgument argument, FTResult result);
    void startNetworkSpeedTest(FTArgument argument, FTResult result);
    void stopNetworkSpeedTest(FTArgument argument, FTResult result);
    void getNetworkTimeInfo(FTArgument argument, FTResult result);

    void startDumpData(FTArgument argument, FTResult result);
    void stopDumpData(FTArgument argument, FTResult result);
    void uploadDumpData(FTArgument argument, FTResult result);
    void removeDumpData(FTArgument argument, FTResult result);

    void mutePublishStreamAudio(FTArgument argument, FTResult result);
    void mutePublishStreamVideo(FTArgument argument, FTResult result);
    void setCaptureVolume(FTArgument argument, FTResult result);
    void setAudioCaptureStereoMode(FTArgument argument, FTResult result);
    void sendSEI(FTArgument argument, FTResult result);
    void sendAudioSideInfo(FTArgument argument, FTResult result);
    void enableHardwareEncoder(FTArgument argument, FTResult result);
    void setCapturePipelineScaleMode(FTArgument argument, FTResult result);
    void enableH265EncodeFallback(FTArgument argument, FTResult result);
    void isVideoEncoderSupported(FTArgument argument, FTResult result);
    void setLowlightEnhancement(FTArgument argument, FTResult result);
    void setVideoSource(FTArgument argument, FTResult result);
    void setAudioSource(FTArgument argument, FTResult result);
    void enableVideoObjectSegmentation(FTArgument argument, FTResult result);
    void enableAlphaChannelVideoEncoder(FTArgument argument, FTResult result);

    void startPlayingStream(FTArgument argument, FTResult result);
    void switchPlayingStream(FTArgument argument, FTResult result);
    void stopPlayingStream(FTArgument argument, FTResult result);
    void setPlayStreamCrossAppInfo(FTArgument argument, FTResult result);
    void takePlayStreamSnapshot(FTArgument argument, FTResult result);
    void setPlayVolume(FTArgument argument, FTResult result);
    void setAllPlayStreamVolume(FTArgument argument, FTResult result);
    void mutePlayStreamAudio(FTArgument argument, FTResult result);
    void muteAllPlayStreamAudio(FTArgument argument, FTResult result);
    void muteAllPlayAudioStreams(FTArgument argument, FTResult result);
    void enableHardwareDecoder(FTArgument argument, FTResult result);

    void muteMicrophone(FTArgument argument, FTResult result);
    void isMicrophoneMuted(FTArgument argument, FTResult result);
    void muteSpeaker(FTArgument argument, FTResult result);
    void isSpeakerMuted(FTArgument argument, FTResult result);
    void getAudioDeviceList(FTArgument argument, FTResult result);
    void getDefaultAudioDeviceID(FTArgument argument, FTResult result);
    void useAudioDevice(FTArgument argument, FTResult result);

    void startSoundLevelMonitor(FTArgument argument, FTResult result);
    void stopSoundLevelMonitor(FTArgument argument, FTResult result);

    void enableHeadphoneMonitor(FTArgument argument, FTResult result);
    void setHeadphoneMonitorVolume(FTArgument argument, FTResult result);

    void enableAEC(FTArgument argument, FTResult result);
    void setAECMode(FTArgument argument, FTResult result);
    void enableAGC(FTArgument argument, FTResult result);
    void enableANS(FTArgument argument, FTResult result);
    void enableTransientANS(FTArgument argument, FTResult result);
    void setANSMode(FTArgument argument, FTResult result);
    void enableSpeechEnhance(FTArgument argument, FTResult result);

    void setAudioEqualizerGain(FTArgument argument, FTResult result);
    void setVoiceChangerPreset(FTArgument argument, FTResult result);
    void setVoiceChangerParam(FTArgument argument, FTResult result);
    void setReverbPreset(FTArgument argument, FTResult result);
    void setReverbAdvancedParam(FTArgument argument, FTResult result);
    void setReverbEchoParam(FTArgument argument, FTResult result);
    void enableVirtualStereo(FTArgument argument, FTResult result);
    void enablePlayStreamVirtualStereo(FTArgument argument, FTResult result);
    void setElectronicEffects(FTArgument argument, FTResult result);

    /* Custom Audio IO */
    void startAudioDataObserver(FTArgument argument, FTResult result);
    void stopAudioDataObserver(FTArgument argument, FTResult result);
    /* Audio Effects Player */
    void createAudioEffectPlayer(FTArgument argument, FTResult result);
    void destroyAudioEffectPlayer(FTArgument argument, FTResult result);
    void audioEffectPlayerStart(FTArgument argument, FTResult result);
    void audioEffectPlayerStop(FTArgument argument, FTResult result);
    void audioEffectPlayerPause(FTArgument argument, FTResult result);
    void audioEffectPlayerResume(FTArgument argument, FTResult result);
    void audioEffectPlayerStopAll(FTArgument argument, FTResult result);
    void audioEffectPlayerPauseAll(FTArgument argument, FTResult result);
    void audioEffectPlayerResumeAll(FTArgument argument, FTResult result);
    void audioEffectPlayerSeekTo(FTArgument argument, FTResult result);
    void audioEffectPlayerSetVolume(FTArgument argument, FTResult result);
    void audioEffectPlayerSetPlayVolume(FTArgument argument, FTResult result);
    void audioEffectPlayerSetPublishVolume(FTArgument argument, FTResult result);
    void audioEffectPlayerSetVolumeAll(FTArgument argument, FTResult result);
    void audioEffectPlayerSetPlayVolumeAll(FTArgument argument, FTResult result);
    void audioEffectPlayerSetPublishVolumeAll(FTArgument argument, FTResult result);
    void audioEffectPlayerGetTotalDuration(FTArgument argument, FTResult result);
    void audioEffectPlayerGetCurrentProgress(FTArgument argument, FTResult result);
    void audioEffectPlayerLoadResource(FTArgument argument, FTResult result);
    void audioEffectPlayerUnloadResource(FTArgument argument, FTResult result);
    void audioEffectPlayerSetPlaySpeed(FTArgument argument, FTResult result);
    void audioEffectPlayerUpdatePosition(FTArgument argument, FTResult result);

    /* Media Player */
    void createMediaPlayer(FTArgument argument, FTResult result);
    void destroyMediaPlayer(FTArgument argument, FTResult result);
    void mediaPlayerLoadResource(FTArgument argument, FTResult result);
    void mediaPlayerLoadResourceFromMediaData(FTArgument argument, FTResult result);
    void mediaPlayerLoadResourceWithPosition(FTArgument argument, FTResult result);
    void mediaPlayerStart(FTArgument argument, FTResult result);
    void mediaPlayerStop(FTArgument argument, FTResult result);
    void mediaPlayerPause(FTArgument argument, FTResult result);
    void mediaPlayerResume(FTArgument argument, FTResult result);
    void mediaPlayerSeekTo(FTArgument argument, FTResult result);
    void mediaPlayerEnableRepeat(FTArgument argument, FTResult result);
    void mediaPlayerEnableAux(FTArgument argument, FTResult result);
    void mediaPlayerMuteLocal(FTArgument argument, FTResult result);
    void mediaPlayerEnableViewMirror(FTArgument argument, FTResult result);
    void mediaPlayerSetVolume(FTArgument argument, FTResult result);
    void mediaPlayerSetPlayVolume(FTArgument argument, FTResult result);
    void mediaPlayerSetPublishVolume(FTArgument argument, FTResult result);
    void mediaPlayerSetProgressInterval(FTArgument argument, FTResult result);
    void mediaPlayerGetPlayVolume(FTArgument argument, FTResult result);
    void mediaPlayerGetPublishVolume(FTArgument argument, FTResult result);
    void mediaPlayerGetTotalDuration(FTArgument argument, FTResult result);
    void mediaPlayerGetCurrentProgress(FTArgument argument, FTResult result);
    void mediaPlayerGetAudioTrackCount(FTArgument argument, FTResult result);
    void mediaPlayerSetAudioTrackIndex(FTArgument argument, FTResult result);
    void mediaPlayerSetVoiceChangerParam(FTArgument argument, FTResult result);
    void mediaPlayerEnableVoiceChanger(FTArgument argument, FTResult result);
    void mediaPlayerGetCurrentState(FTArgument argument, FTResult result);
    void mediaPlayerSetPlaySpeed(FTArgument argument, FTResult result);

    void mediaPlayerEnableSoundLevelMonitor(FTArgument argument, FTResult result);
    void mediaPlayerEnableFrequencySpectrumMonitor(FTArgument argument, FTResult result);
    void mediaPlayerSetNetWorkResourceMaxCache(FTArgument argument, FTResult result);
    void mediaPlayerSetNetWorkBufferThreshold(FTArgument argument, FTResult result);
    void mediaPlayerGetNetWorkResourceCache(FTArgument argument, FTResult result);
    void mediaPlayerEnableAccurateSeek(FTArgument argument, FTResult result);
    void mediaPlayerLoadCopyrightedMusicResourceWithPosition(FTArgument argument, FTResult result);
    void mediaPlayerClearView(FTArgument argument, FTResult result);
    void mediaPlayerSetActiveAudioChannel(FTArgument argument, FTResult result);
    void mediaPlayerSetPlayerCanvas(FTArgument argument, FTResult result);
    void mediaPlayerTakeSnapshot(FTArgument argument, FTResult result);
    void mediaPlayerSetAudioTrackMode(FTArgument argument, FTResult result);
    void mediaPlayerSetAudioTrackPublishIndex(FTArgument argument, FTResult result);
    void mediaPlayerEnableAudioData(FTArgument argument, FTResult result);
    void mediaPlayerEnableVideoData(FTArgument argument, FTResult result);
    void mediaPlayerEnableBlockData(FTArgument argument, FTResult result);
    void mediaPlayerLoadResourceWithConfig(FTArgument argument, FTResult result);
    void mediaPlayerUpdatePosition(FTArgument argument, FTResult result);
    void mediaPlayerGetMediaInfo(FTArgument argument, FTResult result);
    void mediaPlayerSetHttpHeader(FTArgument argument, FTResult result);
    void mediaPlayerGetCurrentRenderingProgress(FTArgument argument, FTResult result);
    void mediaPlayerEnableLiveAudioEffect(FTArgument argument, FTResult result);
    void mediaPlayerSetPlayMediaStreamType(FTArgument argument, FTResult result);
    void mediaPlayerEnableLocalCache(FTArgument argument, FTResult result);
    void mediaPlayerGetPlaybackStatistics(FTArgument argument, FTResult result);

    /* Media Data Publisher */
    void createMediaDataPublisher(FTArgument argument, FTResult result);
    void destroyMediaDataPublisher(FTArgument argument, FTResult result);
    void mediaDataPublisherAddMediaFilePath(FTArgument argument, FTResult result);
    void mediaDataPublisherGetCurrentDuration(FTArgument argument, FTResult result);
    void mediaDataPublisherGetTotalDuration(FTArgument argument, FTResult result);
    void mediaDataPublisherReset(FTArgument argument, FTResult result);
    void mediaDataPublisherSeekTo(FTArgument argument, FTResult result);
    void mediaDataPublisherSetVideoSendDelayTime(FTArgument argument, FTResult result);

    void startMixerTask(FTArgument argument, FTResult result);
    void stopMixerTask(FTArgument argument, FTResult result);
    void setSEIConfig(FTArgument argument, FTResult result);
    void setAudioDeviceVolume(FTArgument argument, FTResult result);
    void setSpeakerVolumeInAPP(FTArgument argument, FTResult result);
    void getSpeakerVolumeInAPP(FTArgument argument, FTResult result);
    void startAudioDeviceVolumeMonitor(FTArgument argument, FTResult result);
    void stopAudioDeviceVolumeMonitor(FTArgument argument, FTResult result);
    void muteAudioDevice(FTArgument argument, FTResult result);
    void isAudioDeviceMuted(FTArgument argument, FTResult result);
    void setAudioDeviceMode(FTArgument argument, FTResult result);
    void getAudioDeviceVolume(FTArgument argument, FTResult result);
    void enableAudioCaptureDevice(FTArgument argument, FTResult result);
    void enableTrafficControl(FTArgument argument, FTResult result);
    void startRecordingCapturedData(FTArgument argument, FTResult result);
    void stopRecordingCapturedData(FTArgument argument, FTResult result);

    void enableCamera(FTArgument argument, FTResult result);
    void enableCameraAdaptiveFPS(FTArgument argument, FTResult result);

    void useVideoDevice(FTArgument argument, FTResult result);
    void getVideoDeviceList(FTArgument argument, FTResult result);
    void getDefaultVideoDeviceID(FTArgument argument, FTResult result);
    void enableMixSystemPlayout(FTArgument argument, FTResult result);
    void setMixSystemPlayoutVolume(FTArgument argument, FTResult result);
    void enableMixEnginePlayout(FTArgument argument, FTResult result);
    void startAudioVADStableStateMonitor(FTArgument argument, FTResult result);
    void stopAudioVADStableStateMonitor(FTArgument argument, FTResult result);
    void getCurrentAudioDevice(FTArgument argument, FTResult result);

    /* Copyrighted Music*/
    void createCopyrightedMusic(FTArgument argument, FTResult result);
    void destroyCopyrightedMusic(FTArgument argument, FTResult result);
    void copyrightedMusicClearCache(FTArgument argument, FTResult result);
    void copyrightedMusicDownload(FTArgument argument, FTResult result);
    void copyrightedMusicCancelDownload(FTArgument argument, FTResult result);
    void copyrightedMusicGetAverageScore(FTArgument argument, FTResult result);
    void copyrightedMusicGetCacheSize(FTArgument argument, FTResult result);
    void copyrightedMusicGetCurrentPitch(FTArgument argument, FTResult result);
    void copyrightedMusicGetDuration(FTArgument argument, FTResult result);
    void copyrightedMusicGetKrcLyricByToken(FTArgument argument, FTResult result);
    void copyrightedMusicGetLrcLyric(FTArgument argument, FTResult result);
    void copyrightedMusicGetLrcLyricWithConfig(FTArgument argument, FTResult result);
    void copyrightedMusicGetMusicByToken(FTArgument argument, FTResult result);
    void copyrightedMusicGetPreviousScore(FTArgument argument, FTResult result);
    void copyrightedMusicGetStandardPitch(FTArgument argument, FTResult result);
    void copyrightedMusicGetTotalScore(FTArgument argument, FTResult result);
    void copyrightedMusicInitCopyrightedMusic(FTArgument argument, FTResult result);
    void copyrightedMusicPauseScore(FTArgument argument, FTResult result);
    void copyrightedMusicQueryCache(FTArgument argument, FTResult result);
    void copyrightedMusicQueryCacheWithConfig(FTArgument argument, FTResult result);
    void copyrightedMusicQueryCacheWithConfigV2(FTArgument argument, FTResult result);
    void copyrightedMusicRequestAccompaniment(FTArgument argument, FTResult result);
    void copyrightedMusicRequestAccompanimentClip(FTArgument argument, FTResult result);
    void copyrightedMusicRequestSong(FTArgument argument, FTResult result);
    void copyrightedMusicResetScore(FTArgument argument, FTResult result);
    void copyrightedMusicResumeScore(FTArgument argument, FTResult result);
    void copyrightedMusicSendExtendedRequest(FTArgument argument, FTResult result);
    void copyrightedMusicSetScoringLevel(FTArgument argument, FTResult result);
    void copyrightedMusicStartScore(FTArgument argument, FTResult result);
    void copyrightedMusicStopScore(FTArgument argument, FTResult result);
    void copyrightedMusicGetFullScore(FTArgument argument, FTResult result);
    void copyrightedMusicGetSharedResource(FTArgument argument, FTResult result);
    void copyrightedMusicGetSharedResourceV2(FTArgument argument, FTResult result);
    void copyrightedMusicRequestResource(FTArgument argument, FTResult result);
    void copyrightedMusicRequestResourceV2(FTArgument argument, FTResult result);

    // rangeAudio
    void createRangeAudio(FTArgument argument, FTResult result);
    void destroyRangeAudio(FTArgument argument, FTResult result);
    void rangeAudioEnableMicrophone(FTArgument argument, FTResult result);
    void rangeAudioEnableSpatializer(FTArgument argument, FTResult result);
    void rangeAudioEnableSpeaker(FTArgument argument, FTResult result);
    void rangeAudioSetAudioReceiveRange(FTArgument argument, FTResult result);
    void rangeAudioSetMode(FTArgument argument, FTResult result);
    void rangeAudioSetTeamID(FTArgument argument, FTResult result);
    void rangeAudioUpdateAudioSource(FTArgument argument, FTResult result);
    void rangeAudioUpdateSelfPosition(FTArgument argument, FTResult result);
    void rangeAudioMuteUser(FTArgument argument, FTResult result);
    void rangeAudioSetPositionUpdateFrequency(FTArgument argument, FTResult result);
    void rangeAudioSetRangeAudioVolume(FTArgument argument, FTResult result);
    void rangeAudioSetStreamVocalRange(FTArgument argument, FTResult result);
    void rangeAudioUpdateStreamPosition(FTArgument argument, FTResult result);
    void rangeAudioSetRangeAudioCustomMode(FTArgument argument, FTResult result);

    // RealTimeSequentialDataManager
    void createRealTimeSequentialDataManager(FTArgument argument, FTResult result);
    void destroyRealTimeSequentialDataManager(FTArgument argument, FTResult result);
    void dataManagerSendRealTimeSequentialData(FTArgument argument, FTResult result);
    void dataManagerStartBroadcasting(FTArgument argument, FTResult result);
    void dataManagerStartSubscribing(FTArgument argument, FTResult result);
    void dataManagerStopBroadcasting(FTArgument argument, FTResult result);
    void dataManagerStopSubscribing(FTArgument argument, FTResult result);

    // ScreenCapture
    void getScreenCaptureSources(FTArgument argument, FTResult result);
    void createScreenCaptureSource(FTArgument argument, FTResult result);
    void destroyScreenCaptureSource(FTArgument argument, FTResult result);
    void enableCursorVisibleScreenCaptureSource(FTArgument argument, FTResult result);
    void enableWindowActivateScreenCaptureSource(FTArgument argument, FTResult result);
    void setExcludeWindowListScreenCaptureSource(FTArgument argument, FTResult result);
    void updateCaptureRegionScreenCaptureSource(FTArgument argument, FTResult result);
    void updatePublishRegionScreenCaptureSource(FTArgument argument, FTResult result);
    void updateCaptureSourceScreenCaptureSource(FTArgument argument, FTResult result);
    void startCaptureScreenCaptureSource(FTArgument argument, FTResult result);
    void stopCaptureScreenCaptureSource(FTArgument argument, FTResult result);
    void getCaptureSourceRectScreenCaptureSource(FTArgument argument, FTResult result);
    void enableAudioCaptureScreenCaptureSource(FTArgument argument, FTResult result);

    // AIVoiceChanger
    void createAIVoiceChanger(FTArgument argument, FTResult result);
    void destroyAIVoiceChanger(FTArgument argument, FTResult result);
    void isAIVoiceChangerSupported(FTArgument argument, FTResult result);
    void aiVoiceChangerGetSpeakerList(FTArgument argument, FTResult result);
    void aiVoiceChangerInitEngine(FTArgument argument, FTResult result);
    void aiVoiceChangerSetSpeaker(FTArgument argument, FTResult result);
    void aiVoiceChangerUpdate(FTArgument argument, FTResult result);

    void createTextureRenderer(FTArgument argument, FTResult result);
    void destroyTextureRenderer(FTArgument argument, FTResult result);

  private:
    ZegoExpressEngineMethodHandler() = default;

  private:
    std::unordered_map<int, EXPRESS::IZegoAudioEffectPlayer *> audioEffectPlayerMap_;
    std::unordered_map<int, EXPRESS::IZegoMediaPlayer *> mediaPlayerMap_;
    std::unordered_map<int, EXPRESS::IZegoMediaDataPublisher *> mediaDataPublisherMap_;
    std::unordered_map<int, EXPRESS::IZegoScreenCaptureSource *> screenCaptureSourceMap_;
    std::unordered_map<int, EXPRESS::IZegoRealTimeSequentialDataManager *> dataManagerMap_;
    std::unordered_map<int, EXPRESS::IZegoAIVoiceChanger *> aiVoiceChangerMap_;
    EXPRESS::IZegoCopyrightedMusic *copyrightedMusic_ = nullptr;
    EXPRESS::IZegoRangeAudio *rangeAudio_ = nullptr;

    FTBinaryMessenger *messenger_;
    FTTextureRegistrar *texture_;
};