#pragma once

#include <ZegoExpressSDK.h>

#ifdef _WIN32
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#else
#include <flutter_linux/flutter_linux.h>
#endif

#include "ZegoDataUtils.h"

using namespace ZEGO;

class ZegoExpressEngineMethodHandler {
  public:
    ~ZegoExpressEngineMethodHandler() {}

    static ZegoExpressEngineMethodHandler &getInstance() {
        static ZegoExpressEngineMethodHandler m_instance;
        return m_instance;
    }

    void setPluginBinaryMessenger(ZFBinaryMessenger *messenger) { messenger_ = messenger; }

    void setPluginTexture(ZFTextureRegistrar *texture) { texture_ = texture; }

    void clearPluginRegistrar();

    void initApiCalledCallback();

    bool isEngineCreated();

  public:
    void getVersion(ZFArgument argument, ZFResult result);
    void isFeatureSupported(ZFArgument argument, ZFResult result);
    void setPluginVersion(ZFArgument argument, ZFResult result);
    void sendCustomLogMessage(ZFArgument argument, ZFResult result);
    void getAssetAbsolutePath(ZFArgument argument, ZFResult result);
    void createEngine(ZFArgument argument, ZFResult result);
    void createEngineWithProfile(ZFArgument argument, ZFResult result);
    void destroyEngine(ZFArgument argument, ZFResult result);
    void setEngineConfig(ZFArgument argument, ZFResult result);
    void setLogConfig(ZFArgument argument, ZFResult result);
    void setLocalProxyConfig(ZFArgument argument, ZFResult result);
    void setCloudProxyConfig(ZFArgument argument, ZFResult result);
    void setRoomMode(ZFArgument argument, ZFResult result);
    void setLicense(ZFArgument argument, ZFResult result);
    void setGeoFence(ZFArgument argument, ZFResult result);
    void setRoomScenario(ZFArgument argument, ZFResult result);
    void uploadLog(ZFArgument argument, ZFResult result);
    void submitLog(ZFArgument argument, ZFResult result);
    void enableDebugAssistant(ZFArgument argument, ZFResult result);
    void callExperimentalAPI(ZFArgument argument, ZFResult result);
    void setDummyCaptureImagePath(ZFArgument argument, ZFResult result);

    void loginRoom(ZFArgument argument, ZFResult result);
    void logoutRoom(ZFArgument argument, ZFResult result);
    void switchRoom(ZFArgument argument, ZFResult result);
    void renewToken(ZFArgument argument, ZFResult result);
    void setRoomExtraInfo(ZFArgument argument, ZFResult result);
    void getRoomStreamList(ZFArgument argument, ZFResult result);

    void startPublishingStream(ZFArgument argument, ZFResult result);
    void stopPublishingStream(ZFArgument argument, ZFResult result);
    void setStreamExtraInfo(ZFArgument argument, ZFResult result);
    void startPreview(ZFArgument argument, ZFResult result);
    void stopPreview(ZFArgument argument, ZFResult result);
    void setVideoConfig(ZFArgument argument, ZFResult result);
    void getVideoConfig(ZFArgument argument, ZFResult result);
    void setPublishDualStreamConfig(ZFArgument argument, ZFResult result);
    void setVideoMirrorMode(ZFArgument argument, ZFResult result);
    void setAudioConfig(ZFArgument argument, ZFResult result);
    void getAudioConfig(ZFArgument argument, ZFResult result);
    void setPublishStreamEncryptionKey(ZFArgument argument, ZFResult result);
    void takePublishStreamSnapshot(ZFArgument argument, ZFResult result);

    void setMinVideoBitrateForTrafficControl(ZFArgument argument, ZFResult result);
    void setMinVideoFpsForTrafficControl(ZFArgument argument, ZFResult result);
    void setMinVideoResolutionForTrafficControl(ZFArgument argument, ZFResult result);
    void setTrafficControlFocusOn(ZFArgument argument, ZFResult result);
    void addPublishCdnUrl(ZFArgument argument, ZFResult result);
    void removePublishCdnUrl(ZFArgument argument, ZFResult result);
    void enablePublishDirectToCDN(ZFArgument argument, ZFResult result);
    void setPublishWatermark(ZFArgument argument, ZFResult result);
    void setPlayStreamDecryptionKey(ZFArgument argument, ZFResult result);
    void setPlayStreamVideoType(ZFArgument argument, ZFResult result);
    void setPlayStreamBufferIntervalRange(ZFArgument argument, ZFResult result);
    void setPlayStreamFocusOn(ZFArgument argument, ZFResult result);
    void mutePlayStreamVideo(ZFArgument argument, ZFResult result);
    void muteAllPlayStreamVideo(ZFArgument argument, ZFResult result);
    void muteAllPlayVideoStreams(ZFArgument argument, ZFResult result);
    void enableCheckPoc(ZFArgument argument, ZFResult result);
    void isVideoDecoderSupported(ZFArgument argument, ZFResult result);
    void setPlayStreamsAlignmentProperty(ZFArgument argument, ZFResult result);
    void enableVideoSuperResolution(ZFArgument argument, ZFResult result);
    void initVideoSuperResolution(ZFArgument argument, ZFResult result);
    void uninitVideoSuperResolution(ZFArgument argument, ZFResult result);
    void updatePlayingCanvas(ZFArgument argument, ZFResult result);

    void startAutoMixerTask(ZFArgument argument, ZFResult result);
    void stopAutoMixerTask(ZFArgument argument, ZFResult result);
    void setAudioRouteToSpeaker(ZFArgument argument, ZFResult result);
    void getAudioRouteType(ZFArgument argument, ZFResult result);
    void useFrontCamera(ZFArgument argument, ZFResult result);
    void isCameraFocusSupported(ZFArgument argument, ZFResult result);
    void setCameraFocusMode(ZFArgument argument, ZFResult result);
    void setCameraFocusPointInPreview(ZFArgument argument, ZFResult result);
    void setCameraExposureMode(ZFArgument argument, ZFResult result);
    void setCameraExposurePointInPreview(ZFArgument argument, ZFResult result);
    void setCameraExposureCompensation(ZFArgument argument, ZFResult result);
    void setCameraZoomFactor(ZFArgument argument, ZFResult result);
    void getCameraMaxZoomFactor(ZFArgument argument, ZFResult result);
    void startAudioSpectrumMonitor(ZFArgument argument, ZFResult result);
    void stopAudioSpectrumMonitor(ZFArgument argument, ZFResult result);
    void enableHeadphoneAEC(ZFArgument argument, ZFResult result);
    void startEffectsEnv(ZFArgument argument, ZFResult result);
    void stopEffectsEnv(ZFArgument argument, ZFResult result);
    void enableEffectsBeauty(ZFArgument argument, ZFResult result);
    void setEffectsBeautyParam(ZFArgument argument, ZFResult result);
    void enableBeautify(ZFArgument argument, ZFResult result);
    void setBeautifyOption(ZFArgument argument, ZFResult result);
    void enableColorEnhancement(ZFArgument argument, ZFResult result);

    void sendBroadcastMessage(ZFArgument argument, ZFResult result);
    void sendBarrageMessage(ZFArgument argument, ZFResult result);
    void sendCustomCommand(ZFArgument argument, ZFResult result);
    void sendTransparentMessage(ZFArgument argument, ZFResult result);
    void enableCustomVideoCapture(ZFArgument argument, ZFResult result);
    void enableCustomVideoProcessing(ZFArgument argument, ZFResult result);
    void enableCustomVideoRender(ZFArgument argument, ZFResult result);
    void enableCustomAudioCaptureProcessing(ZFArgument argument, ZFResult result);
    void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(ZFArgument argument,
                                                                 ZFResult result);
    void enableAlignedAudioAuxData(ZFArgument argument, ZFResult result);
    void enableCustomAudioRemoteProcessing(ZFArgument argument, ZFResult result);
    void enableCustomAudioPlaybackProcessing(ZFArgument argument, ZFResult result);
    void enableCustomAudioIO(ZFArgument argument, ZFResult result);
    void sendCustomAudioCaptureAACData(ZFArgument argument, ZFResult result);
    void sendCustomAudioCapturePCMData(ZFArgument argument, ZFResult result);
    void fetchCustomAudioRenderPCMData(ZFArgument argument, ZFResult result);

    void startPerformanceMonitor(ZFArgument argument, ZFResult result);
    void stopPerformanceMonitor(ZFArgument argument, ZFResult result);
    void startNetworkProbe(ZFArgument argument, ZFResult result);
    void stopNetworkProbe(ZFArgument argument, ZFResult result);
    void startNetworkSpeedTest(ZFArgument argument, ZFResult result);
    void stopNetworkSpeedTest(ZFArgument argument, ZFResult result);
    void getNetworkTimeInfo(ZFArgument argument, ZFResult result);

    void startDumpData(ZFArgument argument, ZFResult result);
    void stopDumpData(ZFArgument argument, ZFResult result);
    void uploadDumpData(ZFArgument argument, ZFResult result);
    void removeDumpData(ZFArgument argument, ZFResult result);

    void mutePublishStreamAudio(ZFArgument argument, ZFResult result);
    void mutePublishStreamVideo(ZFArgument argument, ZFResult result);
    void setCaptureVolume(ZFArgument argument, ZFResult result);
    void setAudioCaptureStereoMode(ZFArgument argument, ZFResult result);
    void sendSEI(ZFArgument argument, ZFResult result);
    void sendAudioSideInfo(ZFArgument argument, ZFResult result);
    void enableHardwareEncoder(ZFArgument argument, ZFResult result);
    void setCapturePipelineScaleMode(ZFArgument argument, ZFResult result);
    void enableH265EncodeFallback(ZFArgument argument, ZFResult result);
    void isVideoEncoderSupported(ZFArgument argument, ZFResult result);
    void setLowlightEnhancement(ZFArgument argument, ZFResult result);
    void setVideoSource(ZFArgument argument, ZFResult result);
    void setAudioSource(ZFArgument argument, ZFResult result);
    void enableVideoObjectSegmentation(ZFArgument argument, ZFResult result);
    void enableAlphaChannelVideoEncoder(ZFArgument argument, ZFResult result);

    void startPlayingStream(ZFArgument argument, ZFResult result);
    void switchPlayingStream(ZFArgument argument, ZFResult result);
    void stopPlayingStream(ZFArgument argument, ZFResult result);
    void setPlayStreamCrossAppInfo(ZFArgument argument, ZFResult result);
    void takePlayStreamSnapshot(ZFArgument argument, ZFResult result);
    void setPlayVolume(ZFArgument argument, ZFResult result);
    void setAllPlayStreamVolume(ZFArgument argument, ZFResult result);
    void mutePlayStreamAudio(ZFArgument argument, ZFResult result);
    void muteAllPlayStreamAudio(ZFArgument argument, ZFResult result);
    void muteAllPlayAudioStreams(ZFArgument argument, ZFResult result);
    void enableHardwareDecoder(ZFArgument argument, ZFResult result);

    void muteMicrophone(ZFArgument argument, ZFResult result);
    void isMicrophoneMuted(ZFArgument argument, ZFResult result);
    void muteSpeaker(ZFArgument argument, ZFResult result);
    void isSpeakerMuted(ZFArgument argument, ZFResult result);
    void getAudioDeviceList(ZFArgument argument, ZFResult result);
    void getDefaultAudioDeviceID(ZFArgument argument, ZFResult result);
    void useAudioDevice(ZFArgument argument, ZFResult result);

    void startSoundLevelMonitor(ZFArgument argument, ZFResult result);
    void stopSoundLevelMonitor(ZFArgument argument, ZFResult result);

    void enableHeadphoneMonitor(ZFArgument argument, ZFResult result);
    void setHeadphoneMonitorVolume(ZFArgument argument, ZFResult result);

    void enableAEC(ZFArgument argument, ZFResult result);
    void setAECMode(ZFArgument argument, ZFResult result);
    void enableAGC(ZFArgument argument, ZFResult result);
    void enableANS(ZFArgument argument, ZFResult result);
    void enableTransientANS(ZFArgument argument, ZFResult result);
    void setANSMode(ZFArgument argument, ZFResult result);
    void enableSpeechEnhance(ZFArgument argument, ZFResult result);

    void setAudioEqualizerGain(ZFArgument argument, ZFResult result);
    void setVoiceChangerPreset(ZFArgument argument, ZFResult result);
    void setVoiceChangerParam(ZFArgument argument, ZFResult result);
    void setReverbPreset(ZFArgument argument, ZFResult result);
    void setReverbAdvancedParam(ZFArgument argument, ZFResult result);
    void setReverbEchoParam(ZFArgument argument, ZFResult result);
    void enableVirtualStereo(ZFArgument argument, ZFResult result);
    void enablePlayStreamVirtualStereo(ZFArgument argument, ZFResult result);
    void setElectronicEffects(ZFArgument argument, ZFResult result);

    /* Custom Audio IO */
    void startAudioDataObserver(ZFArgument argument, ZFResult result);
    void stopAudioDataObserver(ZFArgument argument, ZFResult result);
    /* Audio Effects Player */
    void createAudioEffectPlayer(ZFArgument argument, ZFResult result);
    void destroyAudioEffectPlayer(ZFArgument argument, ZFResult result);
    void audioEffectPlayerStart(ZFArgument argument, ZFResult result);
    void audioEffectPlayerStop(ZFArgument argument, ZFResult result);
    void audioEffectPlayerPause(ZFArgument argument, ZFResult result);
    void audioEffectPlayerResume(ZFArgument argument, ZFResult result);
    void audioEffectPlayerStopAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerPauseAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerResumeAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSeekTo(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetVolume(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetPlayVolume(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetPublishVolume(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetVolumeAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetPlayVolumeAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetPublishVolumeAll(ZFArgument argument, ZFResult result);
    void audioEffectPlayerGetTotalDuration(ZFArgument argument, ZFResult result);
    void audioEffectPlayerGetCurrentProgress(ZFArgument argument, ZFResult result);
    void audioEffectPlayerLoadResource(ZFArgument argument, ZFResult result);
    void audioEffectPlayerUnloadResource(ZFArgument argument, ZFResult result);
    void audioEffectPlayerSetPlaySpeed(ZFArgument argument, ZFResult result);
    void audioEffectPlayerUpdatePosition(ZFArgument argument, ZFResult result);

    /* Media Player */
    void createMediaPlayer(ZFArgument argument, ZFResult result);
    void destroyMediaPlayer(ZFArgument argument, ZFResult result);
    void mediaPlayerLoadResource(ZFArgument argument, ZFResult result);
    void mediaPlayerLoadResourceFromMediaData(ZFArgument argument, ZFResult result);
    void mediaPlayerLoadResourceWithPosition(ZFArgument argument, ZFResult result);
    void mediaPlayerStart(ZFArgument argument, ZFResult result);
    void mediaPlayerStop(ZFArgument argument, ZFResult result);
    void mediaPlayerPause(ZFArgument argument, ZFResult result);
    void mediaPlayerResume(ZFArgument argument, ZFResult result);
    void mediaPlayerSeekTo(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableRepeat(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableAux(ZFArgument argument, ZFResult result);
    void mediaPlayerMuteLocal(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableViewMirror(ZFArgument argument, ZFResult result);
    void mediaPlayerSetVolume(ZFArgument argument, ZFResult result);
    void mediaPlayerSetPlayVolume(ZFArgument argument, ZFResult result);
    void mediaPlayerSetPublishVolume(ZFArgument argument, ZFResult result);
    void mediaPlayerSetProgressInterval(ZFArgument argument, ZFResult result);
    void mediaPlayerGetPlayVolume(ZFArgument argument, ZFResult result);
    void mediaPlayerGetPublishVolume(ZFArgument argument, ZFResult result);
    void mediaPlayerGetTotalDuration(ZFArgument argument, ZFResult result);
    void mediaPlayerGetCurrentProgress(ZFArgument argument, ZFResult result);
    void mediaPlayerGetAudioTrackCount(ZFArgument argument, ZFResult result);
    void mediaPlayerSetAudioTrackIndex(ZFArgument argument, ZFResult result);
    void mediaPlayerSetVoiceChangerParam(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableVoiceChanger(ZFArgument argument, ZFResult result);
    void mediaPlayerGetCurrentState(ZFArgument argument, ZFResult result);
    void mediaPlayerSetPlaySpeed(ZFArgument argument, ZFResult result);

    void mediaPlayerEnableSoundLevelMonitor(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableFrequencySpectrumMonitor(ZFArgument argument, ZFResult result);
    void mediaPlayerSetNetWorkResourceMaxCache(ZFArgument argument, ZFResult result);
    void mediaPlayerSetNetWorkBufferThreshold(ZFArgument argument, ZFResult result);
    void mediaPlayerGetNetWorkResourceCache(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableAccurateSeek(ZFArgument argument, ZFResult result);
    void mediaPlayerLoadCopyrightedMusicResourceWithPosition(ZFArgument argument, ZFResult result);
    void mediaPlayerClearView(ZFArgument argument, ZFResult result);
    void mediaPlayerSetActiveAudioChannel(ZFArgument argument, ZFResult result);
    void mediaPlayerSetPlayerCanvas(ZFArgument argument, ZFResult result);
    void mediaPlayerTakeSnapshot(ZFArgument argument, ZFResult result);
    void mediaPlayerSetAudioTrackMode(ZFArgument argument, ZFResult result);
    void mediaPlayerSetAudioTrackPublishIndex(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableAudioData(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableVideoData(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableBlockData(ZFArgument argument, ZFResult result);
    void mediaPlayerLoadResourceWithConfig(ZFArgument argument, ZFResult result);
    void mediaPlayerUpdatePosition(ZFArgument argument, ZFResult result);
    void mediaPlayerGetMediaInfo(ZFArgument argument, ZFResult result);
    void mediaPlayerSetHttpHeader(ZFArgument argument, ZFResult result);
    void mediaPlayerGetCurrentRenderingProgress(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableLiveAudioEffect(ZFArgument argument, ZFResult result);
    void mediaPlayerSetPlayMediaStreamType(ZFArgument argument, ZFResult result);
    void mediaPlayerEnableLocalCache(ZFArgument argument, ZFResult result);
    void mediaPlayerGetPlaybackStatistics(ZFArgument argument, ZFResult result);

    /* Media Data Publisher */
    void createMediaDataPublisher(ZFArgument argument, ZFResult result);
    void destroyMediaDataPublisher(ZFArgument argument, ZFResult result);
    void mediaDataPublisherAddMediaFilePath(ZFArgument argument, ZFResult result);
    void mediaDataPublisherGetCurrentDuration(ZFArgument argument, ZFResult result);
    void mediaDataPublisherGetTotalDuration(ZFArgument argument, ZFResult result);
    void mediaDataPublisherReset(ZFArgument argument, ZFResult result);
    void mediaDataPublisherSeekTo(ZFArgument argument, ZFResult result);
    void mediaDataPublisherSetVideoSendDelayTime(ZFArgument argument, ZFResult result);

    void startMixerTask(ZFArgument argument, ZFResult result);
    void stopMixerTask(ZFArgument argument, ZFResult result);
    void setSEIConfig(ZFArgument argument, ZFResult result);
    void setAudioDeviceVolume(ZFArgument argument, ZFResult result);
    void setSpeakerVolumeInAPP(ZFArgument argument, ZFResult result);
    void getSpeakerVolumeInAPP(ZFArgument argument, ZFResult result);
    void startAudioDeviceVolumeMonitor(ZFArgument argument, ZFResult result);
    void stopAudioDeviceVolumeMonitor(ZFArgument argument, ZFResult result);
    void muteAudioDevice(ZFArgument argument, ZFResult result);
    void isAudioDeviceMuted(ZFArgument argument, ZFResult result);
    void setAudioDeviceMode(ZFArgument argument, ZFResult result);
    void getAudioDeviceVolume(ZFArgument argument, ZFResult result);
    void enableAudioCaptureDevice(ZFArgument argument, ZFResult result);
    void enableTrafficControl(ZFArgument argument, ZFResult result);
    void startRecordingCapturedData(ZFArgument argument, ZFResult result);
    void stopRecordingCapturedData(ZFArgument argument, ZFResult result);

    void enableCamera(ZFArgument argument, ZFResult result);
    void enableCameraAdaptiveFPS(ZFArgument argument, ZFResult result);

    void useVideoDevice(ZFArgument argument, ZFResult result);
    void getVideoDeviceList(ZFArgument argument, ZFResult result);
    void getDefaultVideoDeviceID(ZFArgument argument, ZFResult result);
    void enableMixSystemPlayout(ZFArgument argument, ZFResult result);
    void setMixSystemPlayoutVolume(ZFArgument argument, ZFResult result);
    void enableMixEnginePlayout(ZFArgument argument, ZFResult result);
    void startAudioVADStableStateMonitor(ZFArgument argument, ZFResult result);
    void stopAudioVADStableStateMonitor(ZFArgument argument, ZFResult result);
    void getCurrentAudioDevice(ZFArgument argument, ZFResult result);

    /* Copyrighted Music*/
    void createCopyrightedMusic(ZFArgument argument, ZFResult result);
    void destroyCopyrightedMusic(ZFArgument argument, ZFResult result);
    void copyrightedMusicClearCache(ZFArgument argument, ZFResult result);
    void copyrightedMusicDownload(ZFArgument argument, ZFResult result);
    void copyrightedMusicCancelDownload(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetAverageScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetCacheSize(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetCurrentPitch(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetDuration(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetKrcLyricByToken(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetLrcLyric(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetLrcLyricWithConfig(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetMusicByToken(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetPreviousScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetStandardPitch(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetTotalScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicInitCopyrightedMusic(ZFArgument argument, ZFResult result);
    void copyrightedMusicPauseScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicQueryCache(ZFArgument argument, ZFResult result);
    void copyrightedMusicQueryCacheWithConfig(ZFArgument argument, ZFResult result);
    void copyrightedMusicQueryCacheWithConfigV2(ZFArgument argument, ZFResult result);
    void copyrightedMusicRequestAccompaniment(ZFArgument argument, ZFResult result);
    void copyrightedMusicRequestAccompanimentClip(ZFArgument argument, ZFResult result);
    void copyrightedMusicRequestSong(ZFArgument argument, ZFResult result);
    void copyrightedMusicResetScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicResumeScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicSendExtendedRequest(ZFArgument argument, ZFResult result);
    void copyrightedMusicSetScoringLevel(ZFArgument argument, ZFResult result);
    void copyrightedMusicStartScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicStopScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetFullScore(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetSharedResource(ZFArgument argument, ZFResult result);
    void copyrightedMusicGetSharedResourceV2(ZFArgument argument, ZFResult result);
    void copyrightedMusicRequestResource(ZFArgument argument, ZFResult result);
    void copyrightedMusicRequestResourceV2(ZFArgument argument, ZFResult result);

    // rangeAudio
    void createRangeAudio(ZFArgument argument, ZFResult result);
    void destroyRangeAudio(ZFArgument argument, ZFResult result);
    void rangeAudioEnableMicrophone(ZFArgument argument, ZFResult result);
    void rangeAudioEnableSpatializer(ZFArgument argument, ZFResult result);
    void rangeAudioEnableSpeaker(ZFArgument argument, ZFResult result);
    void rangeAudioSetAudioReceiveRange(ZFArgument argument, ZFResult result);
    void rangeAudioSetMode(ZFArgument argument, ZFResult result);
    void rangeAudioSetTeamID(ZFArgument argument, ZFResult result);
    void rangeAudioUpdateAudioSource(ZFArgument argument, ZFResult result);
    void rangeAudioUpdateSelfPosition(ZFArgument argument, ZFResult result);
    void rangeAudioMuteUser(ZFArgument argument, ZFResult result);
    void rangeAudioSetPositionUpdateFrequency(ZFArgument argument, ZFResult result);
    void rangeAudioSetRangeAudioVolume(ZFArgument argument, ZFResult result);
    void rangeAudioSetStreamVocalRange(ZFArgument argument, ZFResult result);
    void rangeAudioUpdateStreamPosition(ZFArgument argument, ZFResult result);
    void rangeAudioSetRangeAudioCustomMode(ZFArgument argument, ZFResult result);

    // RealTimeSequentialDataManager
    void createRealTimeSequentialDataManager(ZFArgument argument, ZFResult result);
    void destroyRealTimeSequentialDataManager(ZFArgument argument, ZFResult result);
    void dataManagerSendRealTimeSequentialData(ZFArgument argument, ZFResult result);
    void dataManagerStartBroadcasting(ZFArgument argument, ZFResult result);
    void dataManagerStartSubscribing(ZFArgument argument, ZFResult result);
    void dataManagerStopBroadcasting(ZFArgument argument, ZFResult result);
    void dataManagerStopSubscribing(ZFArgument argument, ZFResult result);

    // ScreenCapture
    void getScreenCaptureSources(ZFArgument argument, ZFResult result);
    void createScreenCaptureSource(ZFArgument argument, ZFResult result);
    void destroyScreenCaptureSource(ZFArgument argument, ZFResult result);
    void enableCursorVisibleScreenCaptureSource(ZFArgument argument, ZFResult result);
    void enableWindowActivateScreenCaptureSource(ZFArgument argument, ZFResult result);
    void setExcludeWindowListScreenCaptureSource(ZFArgument argument, ZFResult result);
    void updateCaptureRegionScreenCaptureSource(ZFArgument argument, ZFResult result);
    void updatePublishRegionScreenCaptureSource(ZFArgument argument, ZFResult result);
    void updateCaptureSourceScreenCaptureSource(ZFArgument argument, ZFResult result);
    void startCaptureScreenCaptureSource(ZFArgument argument, ZFResult result);
    void stopCaptureScreenCaptureSource(ZFArgument argument, ZFResult result);
    void getCaptureSourceRectScreenCaptureSource(ZFArgument argument, ZFResult result);
    void enableAudioCaptureScreenCaptureSource(ZFArgument argument, ZFResult result);

    // AIVoiceChanger
    void createAIVoiceChanger(ZFArgument argument, ZFResult result);
    void destroyAIVoiceChanger(ZFArgument argument, ZFResult result);
    void isAIVoiceChangerSupported(ZFArgument argument, ZFResult result);
    void aiVoiceChangerGetSpeakerList(ZFArgument argument, ZFResult result);
    void aiVoiceChangerInitEngine(ZFArgument argument, ZFResult result);
    void aiVoiceChangerSetSpeaker(ZFArgument argument, ZFResult result);
    void aiVoiceChangerUpdate(ZFArgument argument, ZFResult result);

    void createTextureRenderer(ZFArgument argument, ZFResult result);
    void destroyTextureRenderer(ZFArgument argument, ZFResult result);

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

    ZFBinaryMessenger *messenger_;
    ZFTextureRegistrar *texture_;
};