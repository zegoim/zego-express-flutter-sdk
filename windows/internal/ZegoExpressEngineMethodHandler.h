#pragma once

#include <ZegoExpressSDK.h>
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

using namespace ZEGO;

class ZegoExpressEngineMethodHandler {
  public:
    ~ZegoExpressEngineMethodHandler() {}

    static ZegoExpressEngineMethodHandler &getInstance() {
        static ZegoExpressEngineMethodHandler m_instance;
        return m_instance;
    }

    void setPluginRegistrar(flutter::PluginRegistrarWindows *registrar) { registrar_ = registrar; }

    void clearPluginRegistrar();

    void initApiCalledCallback();

    bool isEngineCreated();

  public:
    void getVersion(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void isFeatureSupported(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPluginVersion(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getAssetAbsolutePath(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void createEngine(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    createEngineWithProfile(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyEngine(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setEngineConfig(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setLogConfig(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setLocalProxyConfig(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCloudProxyConfig(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setRoomMode(flutter::EncodableMap &argument,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setLicense(flutter::EncodableMap &argument,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);                     
    void setGeoFence(flutter::EncodableMap &argument,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setRoomScenario(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void uploadLog(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void submitLog(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableDebugAssistant(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    callExperimentalAPI(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setDummyCaptureImagePath(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void loginRoom(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void logoutRoom(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void switchRoom(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void renewToken(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setRoomExtraInfo(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getRoomStreamList(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    startPublishingStream(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    stopPublishingStream(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setStreamExtraInfo(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startPreview(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopPreview(flutter::EncodableMap &argument,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setVideoConfig(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getVideoConfig(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPublishDualStreamConfig(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setVideoMirrorMode(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setAudioConfig(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getAudioConfig(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPublishStreamEncryptionKey(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void takePublishStreamSnapshot(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void setMinVideoBitrateForTrafficControl(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setMinVideoFpsForTrafficControl(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setMinVideoResolutionForTrafficControl(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setTrafficControlFocusOn(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void addPublishCdnUrl(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    removePublishCdnUrl(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enablePublishDirectToCDN(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setPublishWatermark(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPlayStreamDecryptionKey(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setPlayStreamVideoType(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPlayStreamBufferIntervalRange(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setPlayStreamFocusOn(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mutePlayStreamVideo(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    muteAllPlayStreamVideo(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    muteAllPlayVideoStreams(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCheckPoc(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    isVideoDecoderSupported(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPlayStreamsAlignmentProperty(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableVideoSuperResolution(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void initVideoSuperResolution(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void uninitVideoSuperResolution(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void updatePlayingCanvas(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void startAutoMixerTask(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopAutoMixerTask(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setAudioRouteToSpeaker(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getAudioRouteType(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void useFrontCamera(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    isCameraFocusSupported(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCameraFocusMode(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCameraFocusPointInPreview(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setCameraExposureMode(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCameraExposurePointInPreview(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCameraExposureCompensation(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setCameraZoomFactor(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getCameraMaxZoomFactor(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startAudioSpectrumMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopAudioSpectrumMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableHeadphoneAEC(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startEffectsEnv(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopEffectsEnv(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableEffectsBeauty(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setEffectsBeautyParam(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableBeautify(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setBeautifyOption(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableColorEnhancement(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    sendBroadcastMessage(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendBarrageMessage(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendCustomCommand(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void ZegoExpressEngineMethodHandler::sendTransparentMessage(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result); 
    void enableCustomVideoCapture(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomVideoProcessing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomVideoRender(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomAudioCaptureProcessing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableAlignedAudioAuxData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomAudioRemoteProcessing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCustomAudioPlaybackProcessing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableCustomAudioIO(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendCustomAudioCaptureAACData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendCustomAudioCapturePCMData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void fetchCustomAudioRenderPCMData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    startPerformanceMonitor(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    stopPerformanceMonitor(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startNetworkProbe(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopNetworkProbe(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    startNetworkSpeedTest(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    stopNetworkSpeedTest(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getNetworkTimeInfo(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void startDumpData(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopDumpData(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void uploadDumpData(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void removeDumpData(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
        

    void
    mutePublishStreamAudio(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mutePublishStreamVideo(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCaptureVolume(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setAudioCaptureStereoMode(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendSEI(flutter::EncodableMap &argument,
                 std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void sendAudioSideInfo(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableHardwareEncoder(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setCapturePipelineScaleMode(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableH265EncodeFallback(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    isVideoEncoderSupported(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setLowlightEnhancement(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setVideoSource(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setAudioSource(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableVideoObjectSegmentation(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableAlphaChannelVideoEncoder(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void startPlayingStream(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopPlayingStream(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPlayStreamCrossAppInfo(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    takePlayStreamSnapshot(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setPlayVolume(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setAllPlayStreamVolume(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mutePlayStreamAudio(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    muteAllPlayStreamAudio(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    muteAllPlayAudioStreams(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableHardwareDecoder(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void muteMicrophone(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void isMicrophoneMuted(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void muteSpeaker(flutter::EncodableMap &argument,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void isSpeakerMuted(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getAudioDeviceList(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getDefaultAudioDeviceID(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void useAudioDevice(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    startSoundLevelMonitor(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    stopSoundLevelMonitor(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    enableHeadphoneMonitor(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setHeadphoneMonitorVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void enableAEC(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setAECMode(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableAGC(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableANS(flutter::EncodableMap &argument,
                   std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableTransientANS(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setANSMode(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableSpeechEnhance(flutter::EncodableMap &argument,
                    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    setAudioEqualizerGain(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setVoiceChangerPreset(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setVoiceChangerParam(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setReverbPreset(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setReverbAdvancedParam(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setReverbEchoParam(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableVirtualStereo(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enablePlayStreamVirtualStereo(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setElectronicEffects(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    /* Custom Audio IO */
    void
    startAudioDataObserver(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    stopAudioDataObserver(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    /* Audio Effects Player */
    void
    createAudioEffectPlayer(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyAudioEffectPlayer(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    audioEffectPlayerStart(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    audioEffectPlayerStop(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    audioEffectPlayerPause(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    audioEffectPlayerResume(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerStopAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerPauseAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerResumeAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    audioEffectPlayerSeekTo(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetPlayVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetPublishVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetVolumeAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetPlayVolumeAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetPublishVolumeAll(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerGetTotalDuration(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerGetCurrentProgress(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerLoadResource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerUnloadResource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerSetPlaySpeed(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void audioEffectPlayerUpdatePosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    /* Media Player */
    void createMediaPlayer(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyMediaPlayer(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerLoadResource(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerLoadResourceFromMediaData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerLoadResourceWithPosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerStart(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerStop(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerPause(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerResume(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSeekTo(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerEnableRepeat(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerEnableAux(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerMuteLocal(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerSetVolume(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetPlayVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetPublishVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetProgressInterval(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetPlayVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetPublishVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetTotalDuration(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetCurrentProgress(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetAudioTrackCount(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetAudioTrackIndex(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetVoiceChangerParam(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetCurrentState(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerSetPlaySpeed(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void mediaPlayerEnableSoundLevelMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableFrequencySpectrumMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetNetWorkResourceMaxCache(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetNetWorkBufferThreshold(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetNetWorkResourceCache(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableAccurateSeek(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerLoadCopyrightedMusicResourceWithPosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerClearView(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetActiveAudioChannel(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetPlayerCanvas(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    mediaPlayerTakeSnapshot(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetAudioTrackMode(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetAudioTrackPublishIndex(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableAudioData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableVideoData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableBlockData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerLoadResourceWithConfig(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerUpdatePosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetMediaInfo(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetHttpHeader(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetCurrentRenderingProgress(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableLiveAudioEffect(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerSetPlayMediaStreamType(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerEnableLocalCache(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaPlayerGetPlaybackStatistics(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
        
    /* Media Data Publisher */
    void createMediaDataPublisher(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyMediaDataPublisher(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherAddMediaFilePath(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherGetCurrentDuration(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherGetTotalDuration(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherReset(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherSeekTo(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void mediaDataPublisherSetVideoSendDelayTime(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void startMixerTask(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopMixerTask(flutter::EncodableMap &argument,
                       std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setSEIConfig(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setAudioDeviceVolume(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    setSpeakerVolumeInAPP(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getSpeakerVolumeInAPP(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startAudioDeviceVolumeMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopAudioDeviceVolumeMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void muteAudioDevice(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void isAudioDeviceMuted(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setAudioDeviceMode(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getAudioDeviceVolume(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableAudioCaptureDevice(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableTrafficControl(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startRecordingCapturedData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopRecordingCapturedData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void enableCamera(flutter::EncodableMap &argument,
                      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableCameraAdaptiveFPS(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void useVideoDevice(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getVideoDeviceList(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getDefaultVideoDeviceID(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableMixSystemPlayout(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setMixSystemPlayoutVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    enableMixEnginePlayout(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startAudioVADStableStateMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopAudioVADStableStateMonitor(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    getCurrentAudioDevice(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    /* Copyrighted Music*/
    void
    createCopyrightedMusic(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    destroyCopyrightedMusic(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicClearCache(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicDownload(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicCancelDownload(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetAverageScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetCacheSize(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetCurrentPitch(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetDuration(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetKrcLyricByToken(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetLrcLyric(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetLrcLyricWithConfig(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetMusicByToken(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetPreviousScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetStandardPitch(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetTotalScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicInitCopyrightedMusic(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicPauseScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicQueryCache(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicQueryCacheWithConfig(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicQueryCacheWithConfigV2(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicRequestAccompaniment(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicRequestAccompanimentClip(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicRequestSong(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicResetScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicResumeScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicSendExtendedRequest(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicSetScoringLevel(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicStartScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicStopScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetFullScore(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetSharedResource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicGetSharedResourceV2(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicRequestResource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void copyrightedMusicRequestResourceV2(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    // rangeAudio
    void createRangeAudio(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyRangeAudio(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioEnableMicrophone(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioEnableSpatializer(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    rangeAudioEnableSpeaker(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetAudioReceiveRange(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetMode(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    rangeAudioSetTeamID(flutter::EncodableMap &argument,
                        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioUpdateAudioSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioUpdateSelfPosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioMuteUser(flutter::EncodableMap &argument,
                            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetPositionUpdateFrequency(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetRangeAudioVolume(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetStreamVocalRange(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioUpdateStreamPosition(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void rangeAudioSetRangeAudioCustomMode(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    // RealTimeSequentialDataManager
    void createRealTimeSequentialDataManager(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyRealTimeSequentialDataManager(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void dataManagerSendRealTimeSequentialData(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void dataManagerStartBroadcasting(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void dataManagerStartSubscribing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void dataManagerStopBroadcasting(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void dataManagerStopSubscribing(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    // ScreenCapture
    void getScreenCaptureSources(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void createScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void destroyScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableCursorVisibleScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableWindowActivateScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void setExcludeWindowListScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void updateCaptureRegionScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void updatePublishRegionScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void updateCaptureSourceScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void startCaptureScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void stopCaptureScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void getCaptureSourceRectScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void enableAudioCaptureScreenCaptureSource(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    // AIVoiceChanger
    void createAIVoiceChanger(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    destroyAIVoiceChanger(flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void aiVoiceChangerGetSpeakerList(
        flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void aiVoiceChangerInitEngine(
        flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void aiVoiceChangerSetSpeaker(
        flutter::EncodableMap &argument,
                         std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    aiVoiceChangerUpdate(
        flutter::EncodableMap &argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    void
    createTextureRenderer(flutter::EncodableMap &argument,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    void
    destroyTextureRenderer(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

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

    flutter::PluginRegistrarWindows *registrar_;
};