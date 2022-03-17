#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoExpressEngineEventHandler.h"

#include <variant>
#include <functional>
#include <flutter/encodable_value.h>

#include <Windows.h>

void ZegoExpressEngineMethodHandler::getVersion(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->Success(EXPRESS::ZegoExpressSDK::getVersion());
}

void ZegoExpressEngineMethodHandler::setPluginVersion(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    std::string version = std::get<std::string>(argument[FTValue("version")]);
    
    version = "*** Plugin Version: " + version;

    zego_express_custom_log(version.c_str(), "Flutter");
    
    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    // TODO: need to write getValue utils
    unsigned int appID = 0;
    if(std::holds_alternative<int32_t>(argument[FTValue("appID")])) {
        appID = (unsigned int)std::get<int32_t>(argument[FTValue("appID")]);
    } else {
        appID = (unsigned int)std::get<int64_t>(argument[FTValue("appID")]);
    }
    std::string appSign = std::get<std::string>(argument[FTValue("appSign")]);
    bool isTestEnv = std::get<bool>(argument[FTValue("isTestEnv")]);
    int scenario = std::get<int32_t>(argument[FTValue("scenario")]);

    auto engine = EXPRESS::ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario, ZegoExpressEngineEventHandler::getInstance());

    engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
    EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());

    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngineWithProfile(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap profileMap = std::get<FTMap>(argument[FTValue("profile")]);
    if(profileMap.size() > 0) {
        EXPRESS::ZegoEngineProfile profilePtr;
        if(std::holds_alternative<int32_t>(profileMap[FTValue("appID")])) {
            profilePtr.appID = (unsigned int)std::get<int32_t>(profileMap[FTValue("appID")]);
        } else {
            profilePtr.appID = (unsigned int)std::get<int64_t>(profileMap[FTValue("appID")]);
        }
        if (!profileMap[FTValue("appSign")].IsNull()) {
            profilePtr.appSign = std::get<std::string>(profileMap[FTValue("appSign")]);
        }  
        profilePtr.scenario = (EXPRESS::ZegoScenario)std::get<int32_t>(profileMap[FTValue("scenario")]);
        auto engine = EXPRESS::ZegoExpressSDK::createEngine(profilePtr, ZegoExpressEngineEventHandler::getInstance());

        engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
        EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::destroyEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto engine = EXPRESS::ZegoExpressSDK::getEngine();

    if (engine) {
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        EXPRESS::ZegoExpressSDK::destroyEngine(engine, [=]() {
            sharedPtrResult->Success();
        });
    }
    else {
        result->Success();
    }

}

void ZegoExpressEngineMethodHandler::setEngineConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoEngineConfig config;

    if(configMap.size() > 0) {
        FTMap logConfigMap = std::get<FTMap>(configMap[FTValue("logConfig")]);
        std::unique_ptr<EXPRESS::ZegoLogConfig> logConfigPtr = nullptr;
        if(logConfigMap.size() > 0) {
            logConfigPtr = std::make_unique<EXPRESS::ZegoLogConfig>();
            logConfigPtr->logPath = std::get<std::string>(logConfigMap[FTValue("logPath")]);
            logConfigPtr->logSize = std::get<int32_t>(logConfigMap[FTValue("logSize")]);
        }

        config.logConfig = logConfigPtr.get();

        FTMap advancedConfigMap = std::get<FTMap>(configMap[FTValue("advancedConfig")]);
        for (auto& cfg : advancedConfigMap) {
            std::string key = std::get<std::string>(cfg.first);
            std::string value = std::get<std::string>(cfg.second);
			config.advancedConfig[key] = value;
		}

        EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLogConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoLogConfig config;
    if(configMap.size() > 0) {
        config.logPath = std::get<std::string>(configMap[FTValue("logPath")]);
        config.logSize = std::get<int32_t>(configMap[FTValue("logSize")]);
        EXPRESS::ZegoExpressSDK::setLogConfig(config);
    }

    result->Success();
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
    if (!argument[FTValue("roomID")].IsNull()) {
        auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(roomID);
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::switchRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto fromRoomID = std::get<std::string>(argument[FTValue("fromRoomID")]);
    auto toRoomID = std::get<std::string>(argument[FTValue("toRoomID")]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = (unsigned int)std::get<int64_t>(configMap[FTValue("maxMemberCount")]);
            configPtr->isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
            configPtr->token = std::get<std::string>(configMap[FTValue("token")]);
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto value = std::get<std::string>(argument[FTValue("value")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::startPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            EXPRESS::ZegoPublisherConfig config;

            config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);

            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(streamID, config, (EXPRESS::ZegoPublishChannel)channel);
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(streamID, (EXPRESS::ZegoPublishChannel)channel);
        }
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream((EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto extraInfo = std::get<std::string>(argument[FTValue("extraInfo")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setStreamExtraInfo(extraInfo, [=](int errorCode) {

        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);

    }, (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::startPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startPreview(nullptr, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(configMap[FTValue("codecID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto config = EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig((EXPRESS::ZegoPublishChannel)channel);
    flutter::EncodableMap retMap;
    retMap[FTValue("bitrate")] = FTValue(config.bitrate);
    retMap[FTValue("channel")] = FTValue(config.channel);
    retMap[FTValue("codecID")] = FTValue(config.codecID);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePublishStreamVideo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamVideo(mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode((EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(), (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);


    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode = (EXPRESS::ZegoStreamResourceMode)std::get<int32_t>(configMap[FTValue("resourceMode")]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (std::holds_alternative<flutter::EncodableMap>(configMap[FTValue("cdnConfig")])) {
            auto cdnConfigMap = std::get<flutter::EncodableMap>(configMap[FTValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = std::get<std::string>(cdnConfigMap[FTValue("url")]);
                cdnConfigPtr->authParam = std::get<std::string>(cdnConfigMap[FTValue("authParam")]);
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
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);

    FTArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceList((EXPRESS::ZegoAudioDeviceType)type);
    for(auto &deviceInfo : deviceList) {
        FTMap deviceMap;
        deviceMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(FTValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultAudioDeviceID((EXPRESS::ZegoAudioDeviceType)type);

    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::useAudioDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    EXPRESS::ZegoExpressSDK::getEngine()->useAudioDevice((EXPRESS::ZegoAudioDeviceType)type, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {
         EXPRESS::ZegoSoundLevelConfig config;
        config.millisecond = std::get<int32_t>(configMap[FTValue("millisecond")]);
        config.enableVAD = std::get<bool>(configMap[FTValue("enableVAD")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startSoundLevelMonitor(config);
    }
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
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bandIndex = std::get<int32_t>(argument[FTValue("bandIndex")]);
    auto bandGain = std::get<double>(argument[FTValue("bandGain")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset((EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)std::get<double>(paramMap[FTValue("pitch")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoReverbAdvancedParam param;
    param.damping = (float)std::get<double>(paramMap[FTValue("damping")]);
    param.roomSize = (float)std::get<double>(paramMap[FTValue("roomSize")]);
    param.reverberance = (float)std::get<double>(paramMap[FTValue("reverberance")]);
    param.wetOnly = std::get<bool>(paramMap[FTValue("wetOnly")]);
    param.wetGain = (float)std::get<double>(paramMap[FTValue("wetGain")]);
    param.dryGain = (float)std::get<double>(paramMap[FTValue("dryGain")]);
    param.toneLow = (float)std::get<double>(paramMap[FTValue("toneLow")]);
    param.toneHigh = (float)std::get<double>(paramMap[FTValue("toneHigh")]);
    param.preDelay = (float)std::get<double>(paramMap[FTValue("preDelay")]);
    param.stereoWidth = (float)std::get<double>(paramMap[FTValue("stereoWidth")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbAdvancedParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbEchoParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)std::get<double>(paramMap[FTValue("inGain")]);
    param.outGain = (float)std::get<double>(paramMap[FTValue("outGain")]);
    param.numDelays = std::get<int>(paramMap[FTValue("numDelays")]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    std::vector<int> delayVec = std::get<std::vector<int>>(paramMap[FTValue("delay")]);
    std::vector<double> decayVec = std::get<std::vector<double>>(paramMap[FTValue("decay")]);

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
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePlayStreamVirtualStereo(enable, angle, streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setElectronicEffects(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto tonal = std::get<int32_t>(argument[FTValue("tonal")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setElectronicEffects(enable, (ZEGO::EXPRESS::ZegoElectronicEffectsMode)mode, tonal);

    result->Success();
}


void ZegoExpressEngineMethodHandler::startAudioDataObserver(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bitmask = std::get<int32_t>(argument[FTValue("observerBitMask")]);
    auto param = std::get<FTMap>(argument[FTValue("param")]);

    EXPRESS::ZegoAudioFrameParam nativeParam;
    nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_UNKNOWN;
    switch (std::get<int32_t>(param[FTValue("sampleRate")]))
    {
    case 0:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_UNKNOWN;
        break;
    case 1:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_8K;
        break;
    case 2:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_16K;
        break;
    case 3:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_22K;
        break;
    case 4:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_24K;
        break;
    case 5:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_32K;
        break;
    case 6:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_44K;
        break;
    case 7:
        nativeParam.sampleRate = EXPRESS::ZEGO_AUDIO_SAMPLE_RATE_48K;
        break;
    }
    nativeParam.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(param[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDataObserver(bitmask, nativeParam);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDataObserver(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDataObserver();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAudioEffectPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto player = EXPRESS::ZegoExpressSDK::getEngine()->createAudioEffectPlayer();
    if(player) {

        int index = player->getIndex();
        player->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        audioEffectPlayerMap_[index] = player;

        result->Success(FTValue(index));

    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if(player) {

        EXPRESS::ZegoExpressSDK::getEngine()->destroyAudioEffectPlayer(player);
        audioEffectPlayerMap_.erase(index);

        result->Success();
    } else {
        result->Error("destroyAudioEffectPlayer_Can_not_find_player", "Invoke `destroyAudioEffectPlayer` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStart(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
        std::unique_ptr<EXPRESS::ZegoAudioEffectPlayConfig> configPtr = nullptr;

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoAudioEffectPlayConfig>();
            configPtr->playCount = std::get<int32_t>(configMap[FTValue("playCount")]);
            configPtr->isPublishOut = std::get<bool>(configMap[FTValue("isPublishOut")]);
        }

        player->start(audioEffectID, path, configPtr.get());

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStart_Can_not_find_player", "Invoke `audioEffectPlayerStart` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStop(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->stop(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStop_Can_not_find_player", "Invoke `audioEffectPlayerStop` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPause(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->pause(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerPause_Can_not_find_player", "Invoke `audioEffectPlayerPause` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->resume(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerResume_Can_not_find_player", "Invoke `audioEffectPlayerResume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->stopAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStopAll_Can_not_find_player", "Invoke `audioEffectPlayerStopAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->pauseAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerPauseAll_Can_not_find_player", "Invoke `audioEffectPlayerPauseAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->resumeAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerResumeAll_Can_not_find_player", "Invoke `audioEffectPlayerResumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        unsigned long long millisecond = 0;
        if (std::holds_alternative<int32_t>(argument[FTValue("millisecond")])) {
            millisecond = (unsigned long long)std::get<int32_t>(argument[FTValue("millisecond")]);
        }
        else {
            millisecond = (unsigned long long)std::get<int64_t>(argument[FTValue("millisecond")]);
        }

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->seekTo(audioEffectID, millisecond, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }
    else {
        result->Error("audioEffectPlayerSeekTo_Can_not_find_player", "Invoke `audioEffectPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolume(audioEffectID, volume);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerSetVolume_Can_not_find_player", "Invoke `audioEffectPlayerSetVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolumeAll(volume);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerSetVolumeAll_Can_not_find_player", "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto totalDuration = player->getTotalDuration(audioEffectID);

        result->Success(FTValue((int64_t)totalDuration));
    }
    else {
        result->Error("audioEffectPlayerGetTotalDuration_Can_not_find_player", "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto currentProgress = player->getCurrentProgress(audioEffectID);

        result->Success(FTValue((int64_t)currentProgress));
    }
    else {
        result->Error("audioEffectPlayerGetCurrentProgress_Can_not_find_player", "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->loadResource(audioEffectID, path, [=](int errorCode) {

            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }
    else {
        result->Error("audioEffectPlayerLoadResource_Can_not_find_player", "Invoke `audioEffectPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->unloadResource(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerUnLoadResource_Can_not_find_player", "Invoke `audioEffectPlayerUnloadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mediaPlayer = EXPRESS::ZegoExpressSDK::getEngine()->createMediaPlayer();
    if(mediaPlayer) {
        auto index = mediaPlayer->getIndex();

        mediaPlayer->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaPlayerMap_[index] = mediaPlayer;

        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyMediaPlayer(mediaPlayer);
    }

    mediaPlayerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResource(path, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        uint64_t startPosition = 0;
        if (std::holds_alternative<int32_t>(argument[FTValue("startPosition")])) {
            startPosition = (unsigned long long)std::get<int32_t>(argument[FTValue("startPosition")]);
        }
        else {
            startPosition = (unsigned long long)std::get<int64_t>(argument[FTValue("startPosition")]);
        }

        auto mediaData = std::get<std::vector<uint8_t>>(argument[FTValue("mediaData")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceFromMediaData(mediaData.data(), (int)mediaData.size(),startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);
        uint64_t startPosition = 0;
        if (std::holds_alternative<int32_t>(argument[FTValue("startPosition")])) {
            startPosition = (unsigned long long)std::get<int32_t>(argument[FTValue("startPosition")]);
        }
        else {
            startPosition = (unsigned long long)std::get<int64_t>(argument[FTValue("startPosition")]);
        }

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceWithPosition(path, startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerStart(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->start();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerStop(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->stop();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerPause(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->pause();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerResume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->resume();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSeekTo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        unsigned long long millisecond = 0;
        if (std::holds_alternative<int32_t>(argument[FTValue("millisecond")])) {
            millisecond = (unsigned long long)std::get<int32_t>(argument[FTValue("millisecond")]);
        }
        else {
            millisecond = (unsigned long long)std::get<int64_t>(argument[FTValue("millisecond")]);
        }

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        mediaPlayer->seekTo(millisecond, [=](int errorCode){
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableRepeat(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAux(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableAux(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool mute = std::get<bool>(argument[FTValue("mute")]);

        mediaPlayer->muteLocal(mute);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPlayVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPublishVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int millsecond = std::get<int32_t>(argument[FTValue("millisecond")]);

        mediaPlayer->setProgressInterval((unsigned long long)millsecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = mediaPlayer->getPlayVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = mediaPlayer->getPublishVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto totalDuration = mediaPlayer->getTotalDuration();
        // TODO: need test
        result->Success(FTValue((int64_t)totalDuration));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto currentProgress = mediaPlayer->getCurrentProgress();
        // TODO: need test
        result->Success(FTValue((int64_t)currentProgress));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto audioTrackCount = mediaPlayer->getAudioTrackCount();
        result->Success(FTValue((int32_t)audioTrackCount));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto trackIndex = std::get<int32_t>(argument[FTValue("trackIndex")]);
        mediaPlayer->setAudioTrackIndex(trackIndex);

    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        FTMap paramMap = std::get<FTMap>(argument[FTValue("param")]);
        auto pitch = std::get<double>(paramMap[FTValue("pitch")]);

        auto audioChannel = std::get<int32_t>(argument[FTValue("audioChannel")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->setVoiceChangerParam((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel, param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto state = mediaPlayer->getCurrentState();
        result->Success(FTValue((int32_t)state));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto speed = std::get<double>(argument[FTValue("speed")]);
        mediaPlayer->setPlaySpeed(speed);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::startMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);

    for (auto& cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    auto inputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("inputList")]);
    auto outputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("outputList")]);

    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);

    task.videoConfig.bitrate = std::get<int32_t>(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = std::get<int32_t>(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = std::get<int32_t>(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = std::get<int32_t>(taskVideoConfig[FTValue("width")]);

    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = std::get<FTMap>(inputIter);

        input.streamID = std::get<std::string>(inputMap[FTValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) - std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) - std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startMixerTask(task, [=](int errorCode, std::string extendedData) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("extendedData")] = FTValue(extendedData);

        sharedPtrResult->Success(retMap);
    });

}

void ZegoExpressEngineMethodHandler::stopMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    auto taskWatermark = std::get<flutter::EncodableMap>(argument[FTValue("watermark")]);

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);

    task.videoConfig.bitrate = std::get<int32_t>(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = std::get<int32_t>(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = std::get<int32_t>(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = std::get<int32_t>(taskVideoConfig[FTValue("width")]);

    auto inputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("inputList")]);
    auto outputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("outputList")]);


    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = std::get<FTMap>(inputIter);

        input.streamID = std::get<std::string>(inputMap[FTValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) - std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) - std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;
    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);
    for (auto& cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->stopMixerTask(task, [=](int errorCode) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });

}

void ZegoExpressEngineMethodHandler::setSEIConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoSEIConfig config;
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.type = (EXPRESS::ZegoSEIType)std::get<int32_t>(configMap[FTValue("type")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSEIConfig(config);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioDeviceVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioDeviceVolume((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioDeviceVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    auto volume = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceVolume((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableAudioCaptureDevice(enable);
    result->Success();
}

/*
void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(mute, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}
void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);
    result->Success();
}
*/

void ZegoExpressEngineMethodHandler::enableTrafficControl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto property = std::get<int32_t>(argument[FTValue("property")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTrafficControl(enable, property);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startRecordingCapturedData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoDataRecordConfig config;

    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.filePath = std::get<std::string>(configMap[FTValue("filePath")]);
    config.recordType = (EXPRESS::ZegoDataRecordType)std::get<int32_t>(configMap[FTValue("recordType")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startRecordingCapturedData(config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopRecordingCapturedData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopRecordingCapturedData((EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCamera(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCamera(enable, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

