#include "include/zego_express_engine/zego_express_engine_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/event_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "internal/ZegoExpressEngineMethodHandler.h"
#include "internal/ZegoExpressEngineEventHandler.h"

#define EngineMethodHandler(funcName)                                                              \
    {                                                                                              \
#funcName, {std::bind(&ZegoExpressEngineMethodHandler::funcName,                                      \
                   &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1,          \
                   std::placeholders::_2),false }}

#define EngineStaticMethodHandler(funcName)                                                              \
    {                                                                                              \
#funcName, {std::bind(&ZegoExpressEngineMethodHandler::funcName,                                      \
                   &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1,          \
                   std::placeholders::_2), true}}

static const std::map<std::string, std::pair< std::function<void(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)>, bool> > G_MethodMap = {
       EngineStaticMethodHandler(getVersion),
       EngineStaticMethodHandler(isFeatureSupported), //std::bind(&ZegoExpressEngineMethodHandler::isFeatureSupported, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(setPluginVersion), //std::bind(&ZegoExpressEngineMethodHandler::setPluginVersion, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(getAssetAbsolutePath), //std::bind(&ZegoExpressEngineMethodHandler::getAssetAbsolutePath, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(createEngine), //std::bind(&ZegoExpressEngineMethodHandler::createEngine, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(createEngineWithProfile), //std::bind(&ZegoExpressEngineMethodHandler::createEngineWithProfile, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(destroyEngine), //std::bind(&ZegoExpressEngineMethodHandler::destroyEngine, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(setEngineConfig), //std::bind(&ZegoExpressEngineMethodHandler::setEngineConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(setLogConfig), //std::bind(&ZegoExpressEngineMethodHandler::setLogConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineStaticMethodHandler(setRoomMode), //std::bind(&ZegoExpressEngineMethodHandler::setRoomMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(uploadLog), //std::bind(&ZegoExpressEngineMethodHandler::uploadLog, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setDebugVerbose), //std::bind(&ZegoExpressEngineMethodHandler::setDebugVerbose, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableDebugAssistant), //std::bind(&ZegoExpressEngineMethodHandler::enableDebugAssistant, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(callExperimentalAPI), //std::bind(&ZegoExpressEngineMethodHandler::callExperimentalAPI, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setDummyCaptureImagePath), //std::bind(&ZegoExpressEngineMethodHandler::setDummyCaptureImagePath, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       EngineMethodHandler(loginRoom), //std::bind(&ZegoExpressEngineMethodHandler::loginRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(loginMultiRoom), //std::bind(&ZegoExpressEngineMethodHandler::loginMultiRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(logoutRoom), //std::bind(&ZegoExpressEngineMethodHandler::logoutRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(switchRoom), //std::bind(&ZegoExpressEngineMethodHandler::switchRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(renewToken), //std::bind(&ZegoExpressEngineMethodHandler::renewToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setRoomExtraInfo), //std::bind(&ZegoExpressEngineMethodHandler::setRoomExtraInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startPublishingStream), //std::bind(&ZegoExpressEngineMethodHandler::startPublishingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopPublishingStream), //std::bind(&ZegoExpressEngineMethodHandler::stopPublishingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setStreamExtraInfo), //std::bind(&ZegoExpressEngineMethodHandler::setStreamExtraInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startPreview), //std::bind(&ZegoExpressEngineMethodHandler::startPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopPreview), //std::bind(&ZegoExpressEngineMethodHandler::stopPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setVideoConfig), //std::bind(&ZegoExpressEngineMethodHandler::setVideoConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getVideoConfig), //std::bind(&ZegoExpressEngineMethodHandler::getVideoConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setVideoMirrorMode), //std::bind(&ZegoExpressEngineMethodHandler::setVideoMirrorMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioConfig), //std::bind(&ZegoExpressEngineMethodHandler::setAudioConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getAudioConfig), //std::bind(&ZegoExpressEngineMethodHandler::getAudioConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPublishStreamEncryptionKey), //std::bind(&ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(takePublishStreamSnapshot), //std::bind(&ZegoExpressEngineMethodHandler::takePublishStreamSnapshot, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       EngineMethodHandler(setMinVideoBitrateForTrafficControl), //std::bind(&ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setMinVideoFpsForTrafficControl), //std::bind(&ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setMinVideoResolutionForTrafficControl), //std::bind(&ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setTrafficControlFocusOn), //std::bind(&ZegoExpressEngineMethodHandler::setTrafficControlFocusOn, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(addPublishCdnUrl), //std::bind(&ZegoExpressEngineMethodHandler::addPublishCdnUrl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(removePublishCdnUrl), //std::bind(&ZegoExpressEngineMethodHandler::removePublishCdnUrl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enablePublishDirectToCDN), //std::bind(&ZegoExpressEngineMethodHandler::enablePublishDirectToCDN, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPublishWatermark), //std::bind(&ZegoExpressEngineMethodHandler::setPublishWatermark, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamDecryptionKey), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamVideoLayer), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamVideoLayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamVideoType), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamVideoType, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamBufferIntervalRange), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamFocusOn), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamFocusOn, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mutePlayStreamVideo), //std::bind(&ZegoExpressEngineMethodHandler::mutePlayStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(muteAllPlayStreamVideo), //std::bind(&ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCheckPoc), //std::bind(&ZegoExpressEngineMethodHandler::enableCheckPoc, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(isVideoDecoderSupported), //std::bind(&ZegoExpressEngineMethodHandler::isVideoDecoderSupported, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamsAlignmentProperty), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamsAlignmentProperty, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startAutoMixerTask), //std::bind(&ZegoExpressEngineMethodHandler::startAutoMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopAutoMixerTask), //std::bind(&ZegoExpressEngineMethodHandler::stopAutoMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioRouteToSpeaker), //std::bind(&ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getAudioRouteType), //std::bind(&ZegoExpressEngineMethodHandler::getAudioRouteType, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(useFrontCamera), //std::bind(&ZegoExpressEngineMethodHandler::useFrontCamera, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(isCameraFocusSupported), //std::bind(&ZegoExpressEngineMethodHandler::isCameraFocusSupported, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraFocusMode), //std::bind(&ZegoExpressEngineMethodHandler::setCameraFocusMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraFocusPointInPreview), //std::bind(&ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraExposureMode), //std::bind(&ZegoExpressEngineMethodHandler::setCameraExposureMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraExposurePointInPreview), //std::bind(&ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraExposureCompensation), //std::bind(&ZegoExpressEngineMethodHandler::setCameraExposureCompensation, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCameraZoomFactor), //std::bind(&ZegoExpressEngineMethodHandler::setCameraZoomFactor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getCameraMaxZoomFactor), //std::bind(&ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startAudioSpectrumMonitor), //std::bind(&ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopAudioSpectrumMonitor), //std::bind(&ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableHeadphoneAEC), //std::bind(&ZegoExpressEngineMethodHandler::enableHeadphoneAEC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableAGC), //std::bind(&ZegoExpressEngineMethodHandler::enableAGC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startEffectsEnv), //std::bind(&ZegoExpressEngineMethodHandler::startEffectsEnv, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopEffectsEnv), //std::bind(&ZegoExpressEngineMethodHandler::stopEffectsEnv, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableEffectsBeauty), //std::bind(&ZegoExpressEngineMethodHandler::enableEffectsBeauty, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setEffectsBeautyParam), //std::bind(&ZegoExpressEngineMethodHandler::setEffectsBeautyParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableBeautify), //std::bind(&ZegoExpressEngineMethodHandler::enableBeautify, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setBeautifyOption), //std::bind(&ZegoExpressEngineMethodHandler::setBeautifyOption, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       EngineMethodHandler(sendBroadcastMessage), //std::bind(&ZegoExpressEngineMethodHandler::sendBroadcastMessage, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendBarrageMessage), //std::bind(&ZegoExpressEngineMethodHandler::sendBarrageMessage, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendCustomCommand), //std::bind(&ZegoExpressEngineMethodHandler::sendCustomCommand, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomVideoCapture), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomVideoCapture, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomAudioCaptureProcessing), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomAudioCaptureProcessingAfterHeadphoneMonitor), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableAlignedAudioAuxData), //std::bind(&ZegoExpressEngineMethodHandler::enableAlignedAudioAuxData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomAudioRemoteProcessing), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomAudioPlaybackProcessing), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopAudioDataObserver), //std::bind(&ZegoExpressEngineMethodHandler::stopAudioDataObserver, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCustomAudioIO), //std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioIO, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendCustomAudioCaptureAACData), //std::bind(&ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendCustomAudioCapturePCMData), //std::bind(&ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(fetchCustomAudioRenderPCMData), //std::bind(&ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       EngineMethodHandler(startPerformanceMonitor), //std::bind(&ZegoExpressEngineMethodHandler::startPerformanceMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopPerformanceMonitor), //std::bind(&ZegoExpressEngineMethodHandler::stopPerformanceMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startNetworkProbe), //std::bind(&ZegoExpressEngineMethodHandler::startNetworkProbe, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopNetworkProbe), //std::bind(&ZegoExpressEngineMethodHandler::stopNetworkProbe, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startNetworkSpeedTest), //std::bind(&ZegoExpressEngineMethodHandler::startNetworkSpeedTest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopNetworkSpeedTest), //std::bind(&ZegoExpressEngineMethodHandler::stopNetworkSpeedTest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getNetworkTimeInfo), //std::bind(&ZegoExpressEngineMethodHandler::getNetworkTimeInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},

       EngineMethodHandler(mutePublishStreamAudio), //std::bind(&ZegoExpressEngineMethodHandler::mutePublishStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mutePublishStreamVideo), //std::bind(&ZegoExpressEngineMethodHandler::mutePublishStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCaptureVolume), //std::bind(&ZegoExpressEngineMethodHandler::setCaptureVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioCaptureStereoMode), //std::bind(&ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendSEI), //std::bind(&ZegoExpressEngineMethodHandler::sendSEI, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(sendAudioSideInfo), //std::bind(&ZegoExpressEngineMethodHandler::sendAudioSideInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableHardwareEncoder), //std::bind(&ZegoExpressEngineMethodHandler::enableHardwareEncoder, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setCapturePipelineScaleMode),
       EngineMethodHandler(enableH265EncodeFallback),
       EngineMethodHandler(isVideoEncoderSupported),
       EngineMethodHandler(setLowlightEnhancement), //std::bind(&ZegoExpressEngineMethodHandler::setLowlightEnhancement, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       EngineMethodHandler(startPlayingStream), //std::bind(&ZegoExpressEngineMethodHandler::startPlayingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopPlayingStream), //std::bind(&ZegoExpressEngineMethodHandler::stopPlayingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayStreamCrossAppInfo), //std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(takePlayStreamSnapshot), //std::bind(&ZegoExpressEngineMethodHandler::takePlayStreamSnapshot, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setPlayVolume), //std::bind(&ZegoExpressEngineMethodHandler::setPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAllPlayStreamVolume), //std::bind(&ZegoExpressEngineMethodHandler::setAllPlayStreamVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mutePlayStreamAudio), //std::bind(&ZegoExpressEngineMethodHandler::mutePlayStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(muteAllPlayStreamAudio), //std::bind(&ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableHardwareDecoder), //std::bind(&ZegoExpressEngineMethodHandler::enableHardwareDecoder, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(muteMicrophone), //std::bind(&ZegoExpressEngineMethodHandler::muteMicrophone, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(isMicrophoneMuted), //std::bind(&ZegoExpressEngineMethodHandler::isMicrophoneMuted, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(muteSpeaker), //std::bind(&ZegoExpressEngineMethodHandler::muteSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(isSpeakerMuted), //std::bind(&ZegoExpressEngineMethodHandler::isSpeakerMuted, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getAudioDeviceList), //std::bind(&ZegoExpressEngineMethodHandler::getAudioDeviceList, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getDefaultAudioDeviceID), //std::bind(&ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(useAudioDevice), //std::bind(&ZegoExpressEngineMethodHandler::useAudioDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startSoundLevelMonitor), //std::bind(&ZegoExpressEngineMethodHandler::startSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopSoundLevelMonitor), //std::bind(&ZegoExpressEngineMethodHandler::stopSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableHeadphoneMonitor), //std::bind(&ZegoExpressEngineMethodHandler::enableHeadphoneMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setHeadphoneMonitorVolume), //std::bind(&ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setMixSystemPlayoutVolume), //std::bind(&ZegoExpressEngineMethodHandler::setMixSystemPlayoutVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableMixEnginePlayout), //std::bind(&ZegoExpressEngineMethodHandler::enableMixEnginePlayout, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableMixSystemPlayout), //std::bind(&ZegoExpressEngineMethodHandler::enableMixSystemPlayout, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableAEC), //std::bind(&ZegoExpressEngineMethodHandler::enableAEC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAECMode), //std::bind(&ZegoExpressEngineMethodHandler::setAECMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableANS), //std::bind(&ZegoExpressEngineMethodHandler::enableANS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableTransientANS), //std::bind(&ZegoExpressEngineMethodHandler::enableTransientANS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setANSMode), //std::bind(&ZegoExpressEngineMethodHandler::setANSMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioEqualizerGain), //std::bind(&ZegoExpressEngineMethodHandler::setAudioEqualizerGain, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setVoiceChangerPreset), //std::bind(&ZegoExpressEngineMethodHandler::setVoiceChangerPreset, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setVoiceChangerParam), //std::bind(&ZegoExpressEngineMethodHandler::setVoiceChangerParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setReverbPreset), //std::bind(&ZegoExpressEngineMethodHandler::setReverbPreset, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setReverbAdvancedParam), //std::bind(&ZegoExpressEngineMethodHandler::setReverbAdvancedParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setReverbEchoParam), //std::bind(&ZegoExpressEngineMethodHandler::setReverbEchoParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableVirtualStereo), //std::bind(&ZegoExpressEngineMethodHandler::enableVirtualStereo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enablePlayStreamVirtualStereo), //std::bind(&ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setElectronicEffects), //std::bind(&ZegoExpressEngineMethodHandler::setElectronicEffects, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startMixerTask), //std::bind(&ZegoExpressEngineMethodHandler::startMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopMixerTask), //std::bind(&ZegoExpressEngineMethodHandler::stopMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setSEIConfig), //std::bind(&ZegoExpressEngineMethodHandler::setSEIConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioDeviceVolume), //std::bind(&ZegoExpressEngineMethodHandler::setAudioDeviceVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startAudioDeviceVolumeMonitor), //std::bind(&ZegoExpressEngineMethodHandler::startAudioDeviceVolumeMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopAudioDeviceVolumeMonitor), //std::bind(&ZegoExpressEngineMethodHandler::stopAudioDeviceVolumeMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(muteAudioDevice), //std::bind(&ZegoExpressEngineMethodHandler::muteAudioDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(isAudioDeviceMuted), //std::bind(&ZegoExpressEngineMethodHandler::isAudioDeviceMuted, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(setAudioDeviceMode), //std::bind(&ZegoExpressEngineMethodHandler::setAudioDeviceMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getAudioDeviceVolume), //std::bind(&ZegoExpressEngineMethodHandler::getAudioDeviceVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableAudioCaptureDevice), //std::bind(&ZegoExpressEngineMethodHandler::enableAudioCaptureDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableTrafficControl), //std::bind(&ZegoExpressEngineMethodHandler::enableTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startRecordingCapturedData), //std::bind(&ZegoExpressEngineMethodHandler::startRecordingCapturedData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopRecordingCapturedData), //std::bind(&ZegoExpressEngineMethodHandler::stopRecordingCapturedData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startAudioDataObserver), //std::bind(&ZegoExpressEngineMethodHandler::startAudioDataObserver, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCamera), //std::bind(&ZegoExpressEngineMethodHandler::enableCamera, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableCameraAdaptiveFPS), //std::bind(&ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(useVideoDevice), //std::bind(&ZegoExpressEngineMethodHandler::useVideoDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getVideoDeviceList), //std::bind(&ZegoExpressEngineMethodHandler::getVideoDeviceList, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getDefaultVideoDeviceID), //std::bind(&ZegoExpressEngineMethodHandler::getDefaultVideoDeviceID, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(enableMixSystemPlayout), //std::bind(&ZegoExpressEngineMethodHandler::enableMixSystemPlayout, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(startAudioVADStableStateMonitor), //std::bind(&ZegoExpressEngineMethodHandler::startAudioVADStableStateMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(stopAudioVADStableStateMonitor), //std::bind(&ZegoExpressEngineMethodHandler::stopAudioVADStableStateMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(getCurrentAudioDevice), //std::bind(&ZegoExpressEngineMethodHandler::getCurrentAudioDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       // Audio Effect Player
       EngineMethodHandler(createAudioEffectPlayer), //std::bind(&ZegoExpressEngineMethodHandler::createAudioEffectPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyAudioEffectPlayer), //std::bind(&ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerStart), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStart, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerStop), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStop, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerPause), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerPause, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerResume), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerResume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerStopAll), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerPauseAll), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerResumeAll), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerSeekTo), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerSetVolume), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerSetVolumeAll), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerGetTotalDuration), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerGetCurrentProgress), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerLoadResource), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerUnloadResource), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(audioEffectPlayerSetPlaySpeed), //std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       // Media Player
       EngineMethodHandler(createMediaPlayer), //std::bind(&ZegoExpressEngineMethodHandler::createMediaPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyMediaPlayer), //std::bind(&ZegoExpressEngineMethodHandler::destroyMediaPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerLoadResource), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerLoadResourceFromMediaData), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerLoadResourceWithPosition), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerStart), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerStart, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerStop), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerStop, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerPause), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerPause, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerResume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerResume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSeekTo), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSeekTo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerEnableRepeat), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerEnableAux), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableAux, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerMuteLocal), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetVolume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetPlayVolume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetPublishVolume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetProgressInterval), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetPlayVolume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetPublishVolume), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetTotalDuration), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetCurrentProgress), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetAudioTrackCount), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetAudioTrackIndex), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetVoiceChangerParam), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetCurrentState), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerEnableAccurateSeek), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableAccurateSeek, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerGetNetWorkResourceCache), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetNetWorkBufferThreshold), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetNetWorkResourceMaxCache), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetPlaySpeed), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerEnableFrequencySpectrumMonitor), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerEnableSoundLevelMonitor), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerLoadCopyrightedMusicResourceWithPosition), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadCopyrightedMusicResourceWithPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerClearView), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerClearView, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetActiveAudioChannel), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetActiveAudioChannel, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerSetPlayerCanvas), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(mediaPlayerTakeSnapshot), //std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerTakeSnapshot, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},       

       // Copyrighted Music
       EngineMethodHandler(createCopyrightedMusic), //std::bind(&ZegoExpressEngineMethodHandler::createCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyCopyrightedMusic), //std::bind(&ZegoExpressEngineMethodHandler::destroyCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicClearCache), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicClearCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicDownload), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicDownload, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetAverageScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetCacheSize), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetCurrentPitch), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetDuration), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetKrcLyricByToken), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetLrcLyric), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyric, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetMusicByToken), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetMusicByToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetPreviousScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetPreviousScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetStandardPitch), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicGetTotalScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetTotalScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicInitCopyrightedMusic), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicPauseScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicPauseScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicQueryCache), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicRequestAccompaniment), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicRequestAccompanimentClip), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompanimentClip, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicRequestSong), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestSong, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicResetScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicResetScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicResumeScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicSendExtendedRequest), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicStartScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicStartScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(copyrightedMusicStopScore), //std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicStopScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
    
        // rangeAudio
       EngineMethodHandler(createRangeAudio), //std::bind(&ZegoExpressEngineMethodHandler::createRangeAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyRangeAudio), //std::bind(&ZegoExpressEngineMethodHandler::destroyRangeAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioEnableMicrophone), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioEnableSpatializer), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioEnableSpeaker), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioSetAudioReceiveRange), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioSetMode), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioSetTeamID), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetTeamID, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioUpdateAudioSource), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioUpdateSelfPosition), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioMuteUser), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioMuteUser, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioSetPositionUpdateFrequency), //std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(rangeAudioSetRangeAudioVolume),
       EngineMethodHandler(rangeAudioSetStreamVocalRange),
       EngineMethodHandler(rangeAudioUpdateStreamPosition),
        // RealTimeSequentialDataManager
       EngineMethodHandler(createRealTimeSequentialDataManager), //std::bind(&ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyRealTimeSequentialDataManager), //std::bind(&ZegoExpressEngineMethodHandler::destroyRealTimeSequentialDataManager, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(dataManagerSendRealTimeSequentialData), //std::bind(&ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(dataManagerStartBroadcasting), //std::bind(&ZegoExpressEngineMethodHandler::dataManagerStartBroadcasting, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(dataManagerStartSubscribing), //std::bind(&ZegoExpressEngineMethodHandler::dataManagerStartSubscribing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(dataManagerStopBroadcasting), //std::bind(&ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(dataManagerStopSubscribing), //std::bind(&ZegoExpressEngineMethodHandler::dataManagerStopSubscribing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},

        // textureRenderer 
       EngineMethodHandler(createTextureRenderer), //std::bind(&ZegoExpressEngineMethodHandler::createTextureRenderer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       EngineMethodHandler(destroyTextureRenderer), //std::bind(&ZegoExpressEngineMethodHandler::destroyTextureRenderer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)}
    };

class ZegoExpressEnginePlugin : public flutter::Plugin, public flutter::StreamHandler<flutter::EncodableValue> {
 
public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    ZegoExpressEnginePlugin();

  
    virtual ~ZegoExpressEnginePlugin();

protected:
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListenInternal(
        const flutter::EncodableValue* arguments,
        std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events) override;

    // Implementation of the public interface, to be provided by subclasses.
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnCancelInternal(
        const flutter::EncodableValue* arguments) override;


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

    auto methodChannel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>
      (registrar->messenger(), "plugins.zego.im/zego_express_engine", &flutter::StandardMethodCodec::GetInstance());

    auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>
      (registrar->messenger(), "plugins.zego.im/zego_express_event_handler", &flutter::StandardMethodCodec::GetInstance());

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

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> ZegoExpressEnginePlugin::OnListenInternal(
    const flutter::EncodableValue* arguments,
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events) {

    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    std::cout << "on listen event" << std::endl;

    return nullptr;
}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> ZegoExpressEnginePlugin::OnCancelInternal(
    const flutter::EncodableValue* arguments) {

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
            if (!pair->second.second && !ZegoExpressEngineMethodHandler::getInstance().isEngineCreated()) {
                result->Error("Engine_not_created", "Please call createEngineWithProfile first");
            } else {
                pair->second.first(argument, std::move(result));
            }
        } catch (std::exception &e) {
            result->Error("method_call_error", e.what());
        }
    }
    else {
        result->NotImplemented();
    }
}

void ZegoExpressEnginePluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  ZegoExpressEnginePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

