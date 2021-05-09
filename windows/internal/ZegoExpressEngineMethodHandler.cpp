#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoExpressEngineEventHandler.h"

#include <variant>
#include <functional>
#include <flutter/encodable_value.h>

void ZegoExpressEngineMethodHandler::getVersion(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->Success(EXPRESS::ZegoExpressSDK::getVersion());
}

void ZegoExpressEngineMethodHandler::createEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    //auto it = argument.find(FTValue("appID"));
    /*auto value = argument[FTValue("appID")];
    if (std::holds_alternative<int32_t>(value)) {
        auto var = std::get<int64_t>(value);
        std::cout << "print test value: " << var << std::endl;
    }
    else {
        auto appSign = std::get<int32_t>(value);
        std::cout << "print test value: " << appSign << std::endl;
    }*/

    // TODO: need to write getValue utils
    unsigned int appID = (unsigned int)std::get<int32_t>(argument[FTValue("appID")]);
    std::string appSign = std::get<std::string>(argument[FTValue("appSign")]);
    bool isTestEnv = std::get<bool>(argument[FTValue("isTestEnv")]);
    int scenario = std::get<int32_t>(argument[FTValue("scenario")]);

    //auto eventInstance = ZegoExpressEngineEventHandler::getInstance();

    EXPRESS::ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario, ZegoExpressEngineEventHandler::getInstance());

    result->Success();
}

void ZegoExpressEngineMethodHandler::destroyEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto engine = EXPRESS::ZegoExpressSDK::getEngine(); 
    EXPRESS::ZegoExpressSDK::destroyEngine(engine, [&]() {
        result->Success();
    });
}

void ZegoExpressEngineMethodHandler::setEngineConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

}

void ZegoExpressEngineMethodHandler::setLogConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

}

void ZegoExpressEngineMethodHandler::uploadLog(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->uploadLog(/*[&](int errorCode) mutable {
        flutter::EncodableMap retMap;
        retMap.insert(flutter::EncodableValue("errorCode"), flutter::EncodableValue(errorCode));
        result->Success(retMap);
    }*/);
    result->Success();
}

void ZegoExpressEngineMethodHandler::loginRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto userMap = std::get<flutter::EncodableMap>(argument[FTValue("user")]);

    EXPRESS::ZegoUser user{ std::get<std::string>(userMap[FTValue("userID")]), std::get<std::string>(userMap[FTValue("userName")]) };

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }
    
    if (configMap.size() > 0) {
   
        EXPRESS::ZegoRoomConfig config;
        config.maxMemberCount = (unsigned int)std::get<int32_t>(configMap[FTValue("maxMemberCount")]);
        config.isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
        config.token = std::get<std::string>(configMap[FTValue("token")]);

        EXPRESS::ZegoExpressSDK::getEngine()->loginRoom(roomID, user, config);
    }
    else {
        EXPRESS::ZegoExpressSDK::getEngine()->loginRoom(roomID, user);
    }

    result->Success();

}

void ZegoExpressEngineMethodHandler::logoutRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument["roomID"]);

    EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(roomID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::switchRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto fromRoomID = std::get<std::string>(argument["fromRoomID"]);
    auto toRoomID = std::get<std::string>(argument["toRoomID"]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (std::holds_alternative<flutter::EncodableMap>(argument["config"])) {
        auto configMap = std::get<flutter::EncodableMap>(argument["config"]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = (unsigned int)std::get<int64_t>(configMap["maxMemberCount"]);
            configPtr->isUserStatusNotify = std::get<bool>(configMap["isUserStatusNotify"]);
            configPtr->token = std::get<std::string>(configMap["token"]);
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());
    
    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument["roomID"]);
    auto key = std::get<std::string>(argument["key"]);
    auto value = std::get<std::string>(argument["value"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [&](int errorCode) {
        flutter::EncodableMap retMap;
        retMap[flutter::EncodableValue("errorCode")] = flutter::EncodableValue(errorCode);
        //retMap.insert(flutter::EncodableValue("errorCode"), flutter::EncodableValue(errorCode));
        result->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::startPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument["streamID"]);
    auto channel = std::get<int32_t>(argument["channel"]);

    EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(streamID, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument["channel"]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream((EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto extraInfo = std::get<std::string>(argument["extraInfo"]);
    auto channel = std::get<int32_t>(argument["channel"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setStreamExtraInfo(extraInfo, [&](int errorCode) {
        
        flutter::EncodableMap retMap;
        retMap[flutter::EncodableValue("errorCode")] = flutter::EncodableValue(errorCode);
        //retMap.insert(flutter::EncodableValue("errorCode"), flutter::EncodableValue(errorCode));
        result->Success(retMap);

    }, (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::startPreivew(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument["channel"]);
    EXPRESS::ZegoExpressSDK::getEngine()->startPreview(nullptr, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument["channel"]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto configMap = std::get<flutter::EncodableMap>(argument["config"]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = std::get<int32_t>(configMap["bitrate"]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap["channel"]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(configMap["codecID"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto config = EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig();
    flutter::EncodableMap retMap;
    retMap[flutter::EncodableValue("bitrate")] = flutter::EncodableValue(config.bitrate);
    retMap[flutter::EncodableValue("channel")] = flutter::EncodableValue(config.channel);
    retMap[flutter::EncodableValue("codecID")] = flutter::EncodableValue(config.codecID);
    //retMap.insert(flutter::EncodableValue("bitrate"), flutter::EncodableValue(config.bitrate));
    //retMap.insert(flutter::EncodableValue("channel"), flutter::EncodableValue(config.channel));
    //retMap.insert(flutter::EncodableValue("codecID"), flutter::EncodableValue(config.codecID));

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument["mute"]);
    auto channel = std::get<int32_t>(argument["channel"]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument["volume"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument["mode"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode((EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument["byteData"]);
    auto channel = std::get<int32_t>(argument["channel"]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(), (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument["streamID"]);
    
    
    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument["config"])) {
        configMap = std::get<flutter::EncodableMap>(argument["config"]);
    }
    
    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode = (EXPRESS::ZegoStreamResourceMode)std::get<int32_t>(argument["resourceMode"]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (std::holds_alternative<flutter::EncodableMap>(configMap["cdnConfig"])) {
            auto cdnConfigMap = std::get<flutter::EncodableMap>(argument["cdnConfig"]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = std::get<std::string>(cdnConfigMap["url"]);
                cdnConfigPtr->authParam = std::get<std::string>(cdnConfigMap["authParam"]);
            }
            
        }

        config.cdnConfig = cdnConfigPtr.get();

        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr, config);
    }
    else {
        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPlayingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument["streamID"]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument["streamID"]);
    auto volume = std::get<int32_t>(argument["volume"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument["volume"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument["streamID"]);
    auto mute = std::get<bool>(argument["mute"]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument["mute"]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument["mute"]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(flutter::EncodableValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument["mute"]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(flutter::EncodableValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

}

void ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

}

void ZegoExpressEngineMethodHandler::useAudioDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto millisecond = std::get<int32_t>(argument["millisecond"]);
    EXPRESS::ZegoExpressSDK::getEngine()->startSoundLevelMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopSoundLevelMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopSoundLevelMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int volume = std::get<int32_t>(argument["volume"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument["mode"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int mode = std::get<int32_t>(argument["mode"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bandIndex = std::get<int32_t>(argument["bandIndex"]);
    auto bandGain = std::get<double>(argument["bandGain"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument["preset"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset((EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument["param"]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)std::get<double>(argument["pitch"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument["preset"]);
    
    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument["param"]);
    EXPRESS::ZegoReverbAdvancedParam param;
    param.damping = (float)std::get<double>(argument["damping"]);
    param.roomSize = (float)std::get<double>(argument["roomSize"]);
    param.reverberance = (float)std::get<double>(argument["reverberance"]);
    param.wetOnly = std::get<bool>(argument["wetOnly"]);
    param.wetGain = (float)std::get<double>(argument["wetGain"]);
    param.dryGain = (float)std::get<double>(argument["dryGain"]);
    param.toneLow = (float)std::get<double>(argument["toneLow"]);
    param.toneHigh = (float)std::get<double>(argument["toneHigh"]);
    param.preDelay = (float)std::get<double>(argument["preDelay"]);
    param.stereoWidth = (float)std::get<double>(argument["stereoWidth"]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbAdvancedParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbEchoParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument["param"]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)std::get<double>(argument["inGain"]);
    param.outGain = (float)std::get<double>(argument["outGain"]);
    param.numDelays = std::get<int>(argument["numDelays"]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    std::vector<int> delayVec = std::get<std::vector<int>>(argument["delay"]);
    std::vector<double> decayVec = std::get<std::vector<double>>(argument["decay"]);

    for (int i = 0; i < param.numDelays; i++) {
        param.delay[i] = delayVec.at(i);
        param.decay[i] = (float)decayVec.at(i);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbEchoParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVirtualStereo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument["enable"]);
    auto angle = std::get<int32_t>(argument["angle"]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

