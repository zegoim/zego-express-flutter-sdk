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

#define EngineMethodHandler(funcName) std::bind(&ZegoExpressEngineMethodHandler::funcName, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)

std::map<std::string, std::function<void(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)> > G_MethodMap = {
       {"getVersion", std::bind(&ZegoExpressEngineMethodHandler::getVersion, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPluginVersion", std::bind(&ZegoExpressEngineMethodHandler::setPluginVersion, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"createEngine", std::bind(&ZegoExpressEngineMethodHandler::createEngine, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"createEngineWithProfile", std::bind(&ZegoExpressEngineMethodHandler::createEngineWithProfile, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyEngine", std::bind(&ZegoExpressEngineMethodHandler::destroyEngine, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setEngineConfig", std::bind(&ZegoExpressEngineMethodHandler::setEngineConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setLogConfig", std::bind(&ZegoExpressEngineMethodHandler::setLogConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setRoomMode", std::bind(&ZegoExpressEngineMethodHandler::setRoomMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"uploadLog", std::bind(&ZegoExpressEngineMethodHandler::uploadLog, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setDebugVerbose", std::bind(&ZegoExpressEngineMethodHandler::setDebugVerbose, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableDebugAssistant", std::bind(&ZegoExpressEngineMethodHandler::enableDebugAssistant, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"callExperimentalAPI", std::bind(&ZegoExpressEngineMethodHandler::callExperimentalAPI, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setDummyCaptureImagePath", std::bind(&ZegoExpressEngineMethodHandler::setDummyCaptureImagePath, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       {"loginRoom", std::bind(&ZegoExpressEngineMethodHandler::loginRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"loginMultiRoom", std::bind(&ZegoExpressEngineMethodHandler::loginMultiRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"logoutRoom", std::bind(&ZegoExpressEngineMethodHandler::logoutRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"switchRoom", std::bind(&ZegoExpressEngineMethodHandler::switchRoom, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"renewToken", std::bind(&ZegoExpressEngineMethodHandler::renewToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setRoomExtraInfo", std::bind(&ZegoExpressEngineMethodHandler::setRoomExtraInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startPublishingStream", std::bind(&ZegoExpressEngineMethodHandler::startPublishingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopPublishingStream", std::bind(&ZegoExpressEngineMethodHandler::stopPublishingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setStreamExtraInfo", std::bind(&ZegoExpressEngineMethodHandler::setStreamExtraInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startPreview", std::bind(&ZegoExpressEngineMethodHandler::startPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopPreview", std::bind(&ZegoExpressEngineMethodHandler::stopPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setVideoConfig", std::bind(&ZegoExpressEngineMethodHandler::setVideoConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getVideoConfig", std::bind(&ZegoExpressEngineMethodHandler::getVideoConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setVideoMirrorMode", std::bind(&ZegoExpressEngineMethodHandler::setVideoMirrorMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAudioConfig", std::bind(&ZegoExpressEngineMethodHandler::setAudioConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getAudioConfig", std::bind(&ZegoExpressEngineMethodHandler::getAudioConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPublishStreamEncryptionKey", std::bind(&ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"takePublishStreamSnapshot", std::bind(&ZegoExpressEngineMethodHandler::takePublishStreamSnapshot, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       {"setMinVideoBitrateForTrafficControl", std::bind(&ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setMinVideoFpsForTrafficControl", std::bind(&ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setMinVideoResolutionForTrafficControl", std::bind(&ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setTrafficControlFocusOn", std::bind(&ZegoExpressEngineMethodHandler::setTrafficControlFocusOn, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"addPublishCdnUrl", std::bind(&ZegoExpressEngineMethodHandler::addPublishCdnUrl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"removePublishCdnUrl", std::bind(&ZegoExpressEngineMethodHandler::removePublishCdnUrl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enablePublishDirectToCDN", std::bind(&ZegoExpressEngineMethodHandler::enablePublishDirectToCDN, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPublishWatermark", std::bind(&ZegoExpressEngineMethodHandler::setPublishWatermark, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamDecryptionKey", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamVideoLayer", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamVideoLayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamVideoType", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamVideoType, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamBufferIntervalRange", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamFocusOn", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamFocusOn, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mutePlayStreamVideo", std::bind(&ZegoExpressEngineMethodHandler::mutePlayStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"muteAllPlayStreamVideo", std::bind(&ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCheckPoc", std::bind(&ZegoExpressEngineMethodHandler::enableCheckPoc, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"isVideoDecoderSupported", std::bind(&ZegoExpressEngineMethodHandler::isVideoDecoderSupported, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamsAlignmentProperty", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamsAlignmentProperty, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startAutoMixerTask", std::bind(&ZegoExpressEngineMethodHandler::startAutoMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopAutoMixerTask", std::bind(&ZegoExpressEngineMethodHandler::stopAutoMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAudioRouteToSpeaker", std::bind(&ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getAudioRouteType", std::bind(&ZegoExpressEngineMethodHandler::getAudioRouteType, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"useFrontCamera", std::bind(&ZegoExpressEngineMethodHandler::useFrontCamera, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"isCameraFocusSupported", std::bind(&ZegoExpressEngineMethodHandler::isCameraFocusSupported, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraFocusMode", std::bind(&ZegoExpressEngineMethodHandler::setCameraFocusMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraFocusPointInPreview", std::bind(&ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraExposureMode", std::bind(&ZegoExpressEngineMethodHandler::setCameraExposureMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraExposurePointInPreview", std::bind(&ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraExposureCompensation", std::bind(&ZegoExpressEngineMethodHandler::setCameraExposureCompensation, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCameraZoomFactor", std::bind(&ZegoExpressEngineMethodHandler::setCameraZoomFactor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getCameraMaxZoomFactor", std::bind(&ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startAudioSpectrumMonitor", std::bind(&ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopAudioSpectrumMonitor", std::bind(&ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableHeadphoneAEC", std::bind(&ZegoExpressEngineMethodHandler::enableHeadphoneAEC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableAGC", std::bind(&ZegoExpressEngineMethodHandler::enableAGC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startEffectsEnv", std::bind(&ZegoExpressEngineMethodHandler::startEffectsEnv, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopEffectsEnv", std::bind(&ZegoExpressEngineMethodHandler::stopEffectsEnv, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableEffectsBeauty", std::bind(&ZegoExpressEngineMethodHandler::enableEffectsBeauty, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setEffectsBeautyParam", std::bind(&ZegoExpressEngineMethodHandler::setEffectsBeautyParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableBeautify", std::bind(&ZegoExpressEngineMethodHandler::enableBeautify, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setBeautifyOption", std::bind(&ZegoExpressEngineMethodHandler::setBeautifyOption, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       {"sendBroadcastMessage", std::bind(&ZegoExpressEngineMethodHandler::sendBroadcastMessage, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendBarrageMessage", std::bind(&ZegoExpressEngineMethodHandler::sendBarrageMessage, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendCustomCommand", std::bind(&ZegoExpressEngineMethodHandler::sendCustomCommand, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomVideoCapture", std::bind(&ZegoExpressEngineMethodHandler::enableCustomVideoCapture, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomAudioCaptureProcessing", std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomAudioCaptureProcessingAfterHeadphoneMonitor", std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomAudioRemoteProcessing", std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomAudioPlaybackProcessing", std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopAudioDataObserver", std::bind(&ZegoExpressEngineMethodHandler::stopAudioDataObserver, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCustomAudioIO", std::bind(&ZegoExpressEngineMethodHandler::enableCustomAudioIO, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendCustomAudioCaptureAACData", std::bind(&ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendCustomAudioCapturePCMData", std::bind(&ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"fetchCustomAudioRenderPCMData", std::bind(&ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       {"startPerformanceMonitor", std::bind(&ZegoExpressEngineMethodHandler::startPerformanceMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopPerformanceMonitor", std::bind(&ZegoExpressEngineMethodHandler::stopPerformanceMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startNetworkProbe", std::bind(&ZegoExpressEngineMethodHandler::startNetworkProbe, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopNetworkProbe", std::bind(&ZegoExpressEngineMethodHandler::stopNetworkProbe, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startNetworkSpeedTest", std::bind(&ZegoExpressEngineMethodHandler::startNetworkSpeedTest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopNetworkSpeedTest", std::bind(&ZegoExpressEngineMethodHandler::stopNetworkSpeedTest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getNetworkTimeInfo", std::bind(&ZegoExpressEngineMethodHandler::getNetworkTimeInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},

       {"mutePublishStreamAudio", std::bind(&ZegoExpressEngineMethodHandler::mutePublishStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mutePublishStreamVideo", std::bind(&ZegoExpressEngineMethodHandler::mutePublishStreamVideo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setCaptureVolume", std::bind(&ZegoExpressEngineMethodHandler::setCaptureVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAudioCaptureStereoMode", std::bind(&ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendSEI", std::bind(&ZegoExpressEngineMethodHandler::sendSEI, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"sendAudioSideInfo", std::bind(&ZegoExpressEngineMethodHandler::sendAudioSideInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableHardwareEncoder", std::bind(&ZegoExpressEngineMethodHandler::enableHardwareEncoder, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setLowlightEnhancement", std::bind(&ZegoExpressEngineMethodHandler::setLowlightEnhancement, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       
       {"startPlayingStream", std::bind(&ZegoExpressEngineMethodHandler::startPlayingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopPlayingStream", std::bind(&ZegoExpressEngineMethodHandler::stopPlayingStream, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayStreamCrossAppInfo", std::bind(&ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setPlayVolume", std::bind(&ZegoExpressEngineMethodHandler::setPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAllPlayStreamVolume", std::bind(&ZegoExpressEngineMethodHandler::setAllPlayStreamVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mutePlayStreamAudio", std::bind(&ZegoExpressEngineMethodHandler::mutePlayStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"muteAllPlayStreamAudio", std::bind(&ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableHardwareDecoder", std::bind(&ZegoExpressEngineMethodHandler::enableHardwareDecoder, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"muteMicrophone", std::bind(&ZegoExpressEngineMethodHandler::muteMicrophone, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"isMicrophoneMuted", std::bind(&ZegoExpressEngineMethodHandler::isMicrophoneMuted, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"muteSpeaker", std::bind(&ZegoExpressEngineMethodHandler::muteSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"isSpeakerMuted", std::bind(&ZegoExpressEngineMethodHandler::isSpeakerMuted, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getAudioDeviceList", std::bind(&ZegoExpressEngineMethodHandler::getAudioDeviceList, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getDefaultAudioDeviceID", std::bind(&ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"useAudioDevice", std::bind(&ZegoExpressEngineMethodHandler::useAudioDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startSoundLevelMonitor", std::bind(&ZegoExpressEngineMethodHandler::startSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopSoundLevelMonitor", std::bind(&ZegoExpressEngineMethodHandler::stopSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableHeadphoneMonitor", std::bind(&ZegoExpressEngineMethodHandler::enableHeadphoneMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setHeadphoneMonitorVolume", std::bind(&ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableAEC", std::bind(&ZegoExpressEngineMethodHandler::enableAEC, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAECMode", std::bind(&ZegoExpressEngineMethodHandler::setAECMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableANS", std::bind(&ZegoExpressEngineMethodHandler::enableANS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableTransientANS", std::bind(&ZegoExpressEngineMethodHandler::enableTransientANS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setANSMode", std::bind(&ZegoExpressEngineMethodHandler::setANSMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAudioEqualizerGain", std::bind(&ZegoExpressEngineMethodHandler::setAudioEqualizerGain, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setVoiceChangerPreset", std::bind(&ZegoExpressEngineMethodHandler::setVoiceChangerPreset, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setVoiceChangerParam", std::bind(&ZegoExpressEngineMethodHandler::setVoiceChangerParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setReverbPreset", std::bind(&ZegoExpressEngineMethodHandler::setReverbPreset, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setReverbAdvancedParam", std::bind(&ZegoExpressEngineMethodHandler::setReverbAdvancedParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setReverbEchoParam", std::bind(&ZegoExpressEngineMethodHandler::setReverbEchoParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableVirtualStereo", std::bind(&ZegoExpressEngineMethodHandler::enableVirtualStereo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enablePlayStreamVirtualStereo", std::bind(&ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setElectronicEffects", std::bind(&ZegoExpressEngineMethodHandler::setElectronicEffects, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startMixerTask", std::bind(&ZegoExpressEngineMethodHandler::startMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopMixerTask", std::bind(&ZegoExpressEngineMethodHandler::stopMixerTask, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setSEIConfig", std::bind(&ZegoExpressEngineMethodHandler::setSEIConfig, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"setAudioDeviceVolume", std::bind(&ZegoExpressEngineMethodHandler::setAudioDeviceVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"getAudioDeviceVolume", std::bind(&ZegoExpressEngineMethodHandler::getAudioDeviceVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableAudioCaptureDevice", std::bind(&ZegoExpressEngineMethodHandler::enableAudioCaptureDevice, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableTrafficControl", std::bind(&ZegoExpressEngineMethodHandler::enableTrafficControl, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startRecordingCapturedData", std::bind(&ZegoExpressEngineMethodHandler::startRecordingCapturedData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"stopRecordingCapturedData", std::bind(&ZegoExpressEngineMethodHandler::stopRecordingCapturedData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"startAudioDataObserver", std::bind(&ZegoExpressEngineMethodHandler::startAudioDataObserver, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCamera", std::bind(&ZegoExpressEngineMethodHandler::enableCamera, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"enableCameraAdaptiveFPS", std::bind(&ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       // Audio Effect Player
       {"createAudioEffectPlayer", std::bind(&ZegoExpressEngineMethodHandler::createAudioEffectPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyAudioEffectPlayer", std::bind(&ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerStart", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStart, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerStop", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStop, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerPause", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerPause, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerResume", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerResume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerStopAll", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerPauseAll", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerResumeAll", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerSeekTo", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerSetVolume", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerSetVolumeAll", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerGetTotalDuration", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerGetCurrentProgress", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerLoadResource", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerUnloadResource", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"audioEffectPlayerSetPlaySpeed", std::bind(&ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       // Media Player
       {"createMediaPlayer", std::bind(&ZegoExpressEngineMethodHandler::createMediaPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyMediaPlayer", std::bind(&ZegoExpressEngineMethodHandler::destroyMediaPlayer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerLoadResource", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerLoadResourceFromMediaData", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerLoadResourceWithPosition", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerStart", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerStart, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerStop", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerStop, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerPause", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerPause, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerResume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerResume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSeekTo", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSeekTo, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerEnableRepeat", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerEnableAux", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableAux, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerMuteLocal", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetVolume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetPlayVolume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetPublishVolume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetProgressInterval", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetPlayVolume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetPublishVolume", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetTotalDuration", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetCurrentProgress", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetAudioTrackCount", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetAudioTrackIndex", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetVoiceChangerParam", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetCurrentState", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerEnableAccurateSeek", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableAccurateSeek, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerGetNetWorkResourceCache", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetNetWorkBufferThreshold", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetNetWorkResourceMaxCache", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetPlaySpeed", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerEnableFrequencySpectrumMonitor", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerEnableSoundLevelMonitor", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerLoadCopyrightedMusicResourceWithPosition", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerLoadCopyrightedMusicResourceWithPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerClearView", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerClearView, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetActiveAudioChannel", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetActiveAudioChannel, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerSetPlayerCanvas", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"mediaPlayerTakeSnapshot", std::bind(&ZegoExpressEngineMethodHandler::mediaPlayerTakeSnapshot, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},       

       // Copyrighted Music
       {"createCopyrightedMusic", std::bind(&ZegoExpressEngineMethodHandler::createCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyCopyrightedMusic", std::bind(&ZegoExpressEngineMethodHandler::destroyCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicClearCache", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicClearCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicDownload", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicDownload, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetAverageScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetCacheSize", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetCurrentPitch", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetDuration", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetKrcLyricByToken", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetLrcLyric", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyric, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetMusicByToken", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetMusicByToken, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetPreviousScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetPreviousScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetStandardPitch", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicGetTotalScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicGetTotalScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicInitCopyrightedMusic", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicPauseScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicPauseScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicQueryCache", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicRequestAccompaniment", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicRequestAccompanimentClip", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompanimentClip, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicRequestSong", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicRequestSong, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicResetScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicResetScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicResumeScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicSendExtendedRequest", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicStartScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicStartScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"copyrightedMusicStopScore", std::bind(&ZegoExpressEngineMethodHandler::copyrightedMusicStopScore, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
    
        // rangeAudio
       {"createRangeAudio", std::bind(&ZegoExpressEngineMethodHandler::createRangeAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyRangeAudio", std::bind(&ZegoExpressEngineMethodHandler::destroyRangeAudio, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioEnableMicrophone", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioEnableSpatializer", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioEnableSpeaker", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioSetAudioReceiveRange", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioSetMode", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetMode, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioSetTeamID", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetTeamID, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioUpdateAudioSource", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioUpdateSelfPosition", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioMuteUser", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioMuteUser, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"rangeAudioSetPositionUpdateFrequency", std::bind(&ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
        // RealTimeSequentialDataManager
       {"createRealTimeSequentialDataManager", std::bind(&ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyRealTimeSequentialDataManager", std::bind(&ZegoExpressEngineMethodHandler::destroyRealTimeSequentialDataManager, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"dataManagerSendRealTimeSequentialData", std::bind(&ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"dataManagerStartBroadcasting", std::bind(&ZegoExpressEngineMethodHandler::dataManagerStartBroadcasting, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"dataManagerStartSubscribing", std::bind(&ZegoExpressEngineMethodHandler::dataManagerStartSubscribing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"dataManagerStopBroadcasting", std::bind(&ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"dataManagerStopSubscribing", std::bind(&ZegoExpressEngineMethodHandler::dataManagerStopSubscribing, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},

        // textureRenderer 
       {"createTextureRenderer", std::bind(&ZegoExpressEngineMethodHandler::createTextureRenderer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"updateTextureRendererSize", std::bind(&ZegoExpressEngineMethodHandler::updateTextureRendererSize, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)},
       {"destroyTextureRenderer", std::bind(&ZegoExpressEngineMethodHandler::destroyTextureRenderer, &ZegoExpressEngineMethodHandler::getInstance(), std::placeholders::_1, std::placeholders::_2)}
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
        pair->second(argument, std::move(result));
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

