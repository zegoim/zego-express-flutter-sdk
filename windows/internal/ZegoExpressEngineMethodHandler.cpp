#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoExpressEngineEventHandler.h"
#include "ZegoTextureRendererController.h"

#include <flutter/encodable_value.h>
#include <flutter/plugin_registrar_windows.h>
#include <functional>
#include <variant>

// #include <Windows.h>
#include "../ZegoLog.h"
#include "DataToImageTools.hpp"
#include "zego_express_engine/ZegoCustomVideoCaptureManager.h"
#include "zego_express_engine/ZegoCustomVideoProcessManager.h"
#include "zego_express_engine/ZegoCustomVideoRenderManager.h"
#include "zego_express_engine/ZegoMediaPlayerVideoManager.h"
#include "zego_express_engine/ZegoMediaPlayerAudioManager.h"
#include "zego_express_engine/ZegoMediaPlayerBlockDataManager.h"

void ZegoExpressEngineMethodHandler::clearPluginRegistrar() {
    ZegoTextureRendererController::getInstance()->uninit();
}

void ZegoExpressEngineMethodHandler::initApiCalledCallback() {
    EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());
}

bool ZegoExpressEngineMethodHandler::isEngineCreated() {
    return EXPRESS::ZegoExpressSDK::getEngine();
}

void ZegoExpressEngineMethodHandler::getVersion(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->Success(EXPRESS::ZegoExpressSDK::getVersion());
}

void ZegoExpressEngineMethodHandler::isFeatureSupported(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto featureType = std::get<int32_t>(argument[FTValue("featureType")]);
    result->Success(
        EXPRESS::ZegoExpressSDK::isFeatureSupported((EXPRESS::ZegoFeatureType)featureType));
}

void ZegoExpressEngineMethodHandler::setPluginVersion(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    std::string version = std::get<std::string>(argument[FTValue("version")]);

    version = "*** Plugin Version: " + version;

    ZF::logInfo(version.c_str());

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAssetAbsolutePath(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    std::string assetPath = std::get<std::string>(argument[FTValue("assetPath")]);
    wchar_t exePath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exePathStrW{exePath};
    std::string exePathStr(exePathStrW.begin(), exePathStrW.end());
    exePathStr = std::string(exePathStr, 0, exePathStr.find_last_of("\\"));
    if (!exePathStr.empty()) {
        assetPath = exePathStr + "\\data\\flutter_assets\\" + assetPath;
    } else {
        result->Error("getAssetAbsolutePath_get_exe_path_fail",
                      "Failed to get the directory where the application is located");
        return;
    }
    result->Success(FTValue(assetPath));
}

void ZegoExpressEngineMethodHandler::createEngine(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    initApiCalledCallback();

    EXPRESS::ZegoEngineConfig config;
    config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                             {"thirdparty_framework_info", "flutter"}};
    EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    // TODO: need to write getValue utils
    unsigned int appID = argument[FTValue("appID")].LongValue();
    std::string appSign = std::get<std::string>(argument[FTValue("appSign")]);
    bool isTestEnv = std::get<bool>(argument[FTValue("isTestEnv")]);
    int scenario = std::get<int32_t>(argument[FTValue("scenario")]);

    auto engine = EXPRESS::ZegoExpressSDK::createEngine(
        appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario,
        ZegoExpressEngineEventHandler::getInstance());

    engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());

    ZegoTextureRendererController::getInstance()->init(registrar_->messenger());

    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngineWithProfile(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    FTMap profileMap = std::get<FTMap>(argument[FTValue("profile")]);
    if (profileMap.size() > 0) {

        initApiCalledCallback();

        EXPRESS::ZegoEngineConfig config;
        config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                                 {"thirdparty_framework_info", "flutter"}};
        EXPRESS::ZegoExpressSDK::setEngineConfig(config);

        EXPRESS::ZegoEngineProfile profilePtr;
        profilePtr.appID = profileMap[FTValue("appID")].LongValue();
        if (!profileMap[FTValue("appSign")].IsNull()) {
            profilePtr.appSign = std::get<std::string>(profileMap[FTValue("appSign")]);
        }
        profilePtr.scenario =
            (EXPRESS::ZegoScenario)std::get<int32_t>(profileMap[FTValue("scenario")]);
        auto engine = EXPRESS::ZegoExpressSDK::createEngine(
            profilePtr, ZegoExpressEngineEventHandler::getInstance());

        engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());

        ZegoTextureRendererController::getInstance()->init(registrar_->messenger());
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::destroyEngine(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto engine = EXPRESS::ZegoExpressSDK::getEngine();

    if (engine) {
        ZegoTextureRendererController::getInstance()->uninit();
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        EXPRESS::ZegoExpressSDK::destroyEngine(engine, [=]() { sharedPtrResult->Success(); });
    } else {
        result->Success();
    }
}

void ZegoExpressEngineMethodHandler::setEngineConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoEngineConfig config;

    if (configMap.size() > 0) {
        FTMap logConfigMap = std::get<FTMap>(configMap[FTValue("logConfig")]);
        std::unique_ptr<EXPRESS::ZegoLogConfig> logConfigPtr = nullptr;
        if (logConfigMap.size() > 0) {
            logConfigPtr = std::make_unique<EXPRESS::ZegoLogConfig>();
            logConfigPtr->logPath = std::get<std::string>(logConfigMap[FTValue("logPath")]);
            logConfigPtr->logSize  = std::get<int32_t>(logConfigMap[FTValue("logSize")]);
            logConfigPtr->logCount = std::get<int32_t>(logConfigMap[FTValue("logCount")]);
        }

        config.logConfig = logConfigPtr.get();

        FTMap advancedConfigMap = std::get<FTMap>(configMap[FTValue("advancedConfig")]);
        for (auto &cfg : advancedConfigMap) {
            std::string key = std::get<std::string>(cfg.first);
            std::string value = std::get<std::string>(cfg.second);
            config.advancedConfig[key] = value;
        }

        EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLogConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoLogConfig config;
    if (configMap.size() > 0) {
        config.logPath = std::get<std::string>(configMap[FTValue("logPath")]);
        config.logSize  = std::get<int32_t>(configMap[FTValue("logSize")]);
        config.logCount = std::get<int32_t>(configMap[FTValue("logCount")]);
        EXPRESS::ZegoExpressSDK::setLogConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLocalProxyConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto proxyListArray = std::get<FTArray>(argument[FTValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = std::get<FTMap>(proxy_);
        proxy.ip = std::get<std::string>(proxyMap[FTValue("ip")]);
        proxy.port = std::get<int32_t>(proxyMap[FTValue("port")]);
        proxy.hostName = std::get<std::string>(proxyMap[FTValue("hostName")]);
        proxy.userName = std::get<std::string>(proxyMap[FTValue("userName")]);
        proxy.password = std::get<std::string>(proxyMap[FTValue("password")]);
        proxyList.push_back(proxy);
    }
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::setLocalProxyConfig(proxyList, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCloudProxyConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto proxyListArray = std::get<FTArray>(argument[FTValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = std::get<FTMap>(proxy_);
        proxy.ip = std::get<std::string>(proxyMap[FTValue("ip")]);
        proxy.port = std::get<int32_t>(proxyMap[FTValue("port")]);
        proxy.hostName = std::get<std::string>(proxyMap[FTValue("hostName")]);
        proxy.userName = std::get<std::string>(proxyMap[FTValue("userName")]);
        proxy.password = std::get<std::string>(proxyMap[FTValue("password")]);
        proxyList.push_back(proxy);
    }
    auto token = std::get<std::string>(argument[FTValue("token")]);

    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::setCloudProxyConfig(proxyList, token, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = (EXPRESS::ZegoRoomMode)std::get<int32_t>(argument[FTValue("mode")]);
    EXPRESS::ZegoExpressSDK::setRoomMode(mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLicense(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto license = std::get<std::string>(argument[FTValue("license")]);
    EXPRESS::ZegoExpressSDK::setLicense(license);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setGeoFence(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto type = (EXPRESS::ZegoGeoFenceType)std::get<int32_t>(argument[FTValue("type")]);
    auto areaList_ = std::get<FTArray>(argument[FTValue("areaList")]);
    std::vector<int> areaList;
    for (auto area : areaList_) {
        areaList.push_back(std::get<int32_t>(area));
    }
    EXPRESS::ZegoExpressSDK::setGeoFence(type, areaList);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomScenario(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto scenario = (EXPRESS::ZegoScenario)std::get<int32_t>(argument[FTValue("scenario")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomScenario(scenario);

    result->Success();
}

void ZegoExpressEngineMethodHandler::uploadLog(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->uploadLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::submitLog(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::submitLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableDebugAssistant(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableDebugAssistant(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::callExperimentalAPI(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto params = std::get<std::string>(argument[FTValue("params")]);
    auto resultStr = EXPRESS::ZegoExpressSDK::getEngine()->callExperimentalAPI(params);
    result->Success(resultStr);
}

void ZegoExpressEngineMethodHandler::setDummyCaptureImagePath(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto filePath = std::get<std::string>(argument[FTValue("filePath")]);
    const std::string flutterAssertTaget = "flutter-asset://";
    if (filePath.compare(0, flutterAssertTaget.size(), flutterAssertTaget) == 0) {
        filePath.replace(0, flutterAssertTaget.size(), "");
        wchar_t exePath[MAX_PATH] = {0};
        ::GetModuleFileName(NULL, exePath, MAX_PATH);
        std::wstring exePathStrW{exePath};
        std::string exePathStr(exePathStrW.begin(), exePathStrW.end());
        exePathStr = std::string(exePathStr, 0, exePathStr.find_last_of("\\"));
        if (!exePathStr.empty()) {
            filePath = exePathStr + "\\data\\flutter_assets\\" + filePath;
        } else {
            result->Error("setDummyCaptureImagePath_get_exe_path_fail",
                          "Failed to get the directory where the application is located");
            return;
        }
    }
    auto channel = (EXPRESS::ZegoPublishChannel)std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setDummyCaptureImagePath(filePath, channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::loginRoom(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto userMap = std::get<flutter::EncodableMap>(argument[FTValue("user")]);

    EXPRESS::ZegoUser user{std::get<std::string>(userMap[FTValue("userID")]),
                           std::get<std::string>(userMap[FTValue("userName")])};

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    EXPRESS::ZegoRoomConfig config;
    if (configMap.size() > 0) {
        config.maxMemberCount =
            (unsigned int)std::get<int32_t>(configMap[FTValue("maxMemberCount")]);
        config.isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
        config.token = std::get<std::string>(configMap[FTValue("token")]);
    }
    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->loginRoom(
        roomID, user, config, [=](int errorCode, std::string extendedData) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            if (extendedData.empty()) {
                retMap[FTValue("extendedData")] = FTValue("{}");
            } else {
                retMap[FTValue("extendedData")] = FTValue(extendedData);
            }
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::logoutRoom(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    if (!argument[FTValue("roomID")].IsNull()) {
        auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(
            roomID, [=](int errorCode, std::string extendedData) {
                FTMap retMap;

                retMap[FTValue("errorCode")] = FTValue(errorCode);
                if (extendedData.empty()) {
                    retMap[FTValue("extendedData")] = FTValue("{}");
                } else {
                    retMap[FTValue("extendedData")] = FTValue(extendedData);
                }

                sharedPtrResult->Success(retMap);
            });
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(
            [=](int errorCode, std::string extendedData) {
                FTMap retMap;

                retMap[FTValue("errorCode")] = FTValue(errorCode);
                if (extendedData.empty()) {
                    retMap[FTValue("extendedData")] = FTValue("{}");
                } else {
                    retMap[FTValue("extendedData")] = FTValue(extendedData);
                }

                sharedPtrResult->Success(retMap);
            });
    }
}

void ZegoExpressEngineMethodHandler::switchRoom(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto fromRoomID = std::get<std::string>(argument[FTValue("fromRoomID")]);
    auto toRoomID = std::get<std::string>(argument[FTValue("toRoomID")]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = configMap[FTValue("maxMemberCount")].LongValue();
            configPtr->isUserStatusNotify =
                std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
            configPtr->token = std::get<std::string>(configMap[FTValue("token")]);
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());

    result->Success();
}

void ZegoExpressEngineMethodHandler::renewToken(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto token = std::get<std::string>(argument[FTValue("token")]);

    EXPRESS::ZegoExpressSDK::getEngine()->renewToken(roomID, token);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto value = std::get<std::string>(argument[FTValue("value")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::getRoomStreamList(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

    auto stream_list_type = std::get<int32_t>(argument[FTValue("streamListType")]);

    EXPRESS::ZegoRoomStreamList streamList = EXPRESS::ZegoExpressSDK::getEngine()->getRoomStreamList(
        roomID, (EXPRESS::ZegoRoomStreamListType)stream_list_type);
    
    FTMap retMap;

    FTArray streamListPlayArray;
    for (auto &stream : streamList.playStreamList) {
        FTMap streamMap;
        FTMap userMap;
        userMap[FTValue("userID")] = FTValue(stream.user.userID);
        userMap[FTValue("userName")] = FTValue(stream.user.userName);

        streamMap[FTValue("streamID")] = FTValue(stream.streamID);
        streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
        streamMap[FTValue("user")] = FTValue(userMap);

        streamListPlayArray.emplace_back(FTValue(streamMap));
    }

    retMap[FTValue("playStreamList")] = FTValue(streamListPlayArray);

    FTArray streamListPublishArray;
    for (auto &stream : streamList.publishStreamList) {
        FTMap streamMap;
        FTMap userMap;
        userMap[FTValue("userID")] = FTValue(stream.user.userID);
        userMap[FTValue("userName")] = FTValue(stream.user.userName);

        streamMap[FTValue("streamID")] = FTValue(stream.streamID);
        streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
        streamMap[FTValue("user")] = FTValue(userMap);

        streamListPublishArray.emplace_back(FTValue(streamMap));
    }

    retMap[FTValue("publishStreamList")] = FTValue(streamListPublishArray);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::startPublishingStream(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            EXPRESS::ZegoPublisherConfig config;

            config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
            config.forceSynchronousNetworkTime =
                std::get<int32_t>(configMap[FTValue("forceSynchronousNetworkTime")]);
            config.streamCensorshipMode = (EXPRESS::ZegoStreamCensorshipMode)std::get<int32_t>(
                configMap[FTValue("streamCensorshipMode")]);

            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(
                streamID, config, (EXPRESS::ZegoPublishChannel)channel);
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(
                streamID, (EXPRESS::ZegoPublishChannel)channel);
        }
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPublishingStream(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream(
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto extraInfo = std::get<std::string>(argument[FTValue("extraInfo")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setStreamExtraInfo(
        extraInfo,
        [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        },
        (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::startPreview(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    flutter::EncodableMap canvasMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("canvas")])) {
        canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        auto alphaBlend = std::get<bool>(canvasMap[FTValue("alphaBlend")]);
        ZegoTextureRendererController::getInstance()->removeCapturedRenderer(
            (EXPRESS::ZegoPublishChannel)channel);
        ZegoTextureRendererController::getInstance()->addCapturedRenderer(
            viewID, (EXPRESS::ZegoPublishChannel)channel, viewMode);
        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }
    EXPRESS::ZegoExpressSDK::getEngine()->startPreview(nullptr,
                                                       (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPreview(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    ZegoTextureRendererController::getInstance()->removeCapturedRenderer(
        (EXPRESS::ZegoPublishChannel)channel);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoVideoConfig config;
    config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
    config.captureHeight = std::get<int32_t>(configMap[FTValue("captureHeight")]);
    config.captureWidth = std::get<int32_t>(configMap[FTValue("captureWidth")]);
    auto codecIDIndex = std::get<int32_t>(configMap[FTValue("codecID")]);
    config.codecID = (EXPRESS::ZegoVideoCodecID)(
        codecIDIndex > 4 ? EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN : codecIDIndex);
    config.encodeHeight = std::get<int32_t>(configMap[FTValue("encodeHeight")]);
    config.encodeWidth = std::get<int32_t>(configMap[FTValue("encodeWidth")]);
    config.fps = std::get<int32_t>(configMap[FTValue("fps")]);
    config.keyFrameInterval = std::get<int32_t>(configMap[FTValue("keyFrameInterval")]);

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setVideoConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    auto config =
        EXPRESS::ZegoExpressSDK::getEngine()->getVideoConfig((EXPRESS::ZegoPublishChannel)channel);

    FTMap configMap;
    configMap[FTValue("bitrate")] = FTValue(config.bitrate);
    configMap[FTValue("captureHeight")] = FTValue(config.captureHeight);
    configMap[FTValue("captureWidth")] = FTValue(config.captureWidth);
    configMap[FTValue("codecID")] = FTValue((int32_t)config.codecID);
    configMap[FTValue("encodeHeight")] = FTValue(config.encodeHeight);
    configMap[FTValue("encodeWidth")] = FTValue(config.encodeWidth);
    configMap[FTValue("fps")] = FTValue(config.fps);
    configMap[FTValue("keyFrameInterval")] = FTValue(config.keyFrameInterval);

    result->Success(configMap);
}

void ZegoExpressEngineMethodHandler::setPublishDualStreamConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    std::vector<EXPRESS::ZegoPublishDualStreamConfig> configList;
    auto configListMap = std::get<FTArray>(argument[FTValue("configList")]);
    for (auto config_ : configListMap) {
        FTMap configMap = std::get<FTMap>(config_);
        EXPRESS::ZegoPublishDualStreamConfig config;

        config.encodeWidth = std::get<int32_t>(configMap[FTValue("encodeWidth")]);
        config.encodeHeight = std::get<int32_t>(configMap[FTValue("encodeHeight")]);
        config.fps = std::get<int32_t>(configMap[FTValue("fps")]);
        config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
        config.streamType = (EXPRESS::ZegoVideoStreamType) std::get<int32_t>(configMap[FTValue("streamType")]);

        configList.push_back(config);
    }
    
    EXPRESS::ZegoExpressSDK::getEngine()->setPublishDualStreamConfig(configList, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoMirrorMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto mirrorMode =
        (EXPRESS::ZegoVideoMirrorMode)std::get<int32_t>(argument[FTValue("mirrorMode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVideoMirrorMode(mirrorMode,
                                                             (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(configMap[FTValue("codecID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto config =
        EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig((EXPRESS::ZegoPublishChannel)channel);
    flutter::EncodableMap retMap;
    retMap[FTValue("bitrate")] = FTValue(config.bitrate);
    retMap[FTValue("channel")] = FTValue(config.channel);
    retMap[FTValue("codecID")] = FTValue(config.codecID);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishStreamEncryptionKey(
        key, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePublishStreamSnapshot(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->takePublishStreamSnapshot(
        [=](int errorCode, void *image) {
            std::vector<uint8_t> raw_image;
            if(image)
            {
                auto tmpData = CreateFromHBITMAP((HBITMAP)image);
                raw_image.assign(tmpData.second, tmpData.second + tmpData.first);
                delete[] tmpData.second;
            }
        
            FTMap resultMap;
            resultMap[FTValue("errorCode")] = FTValue(errorCode);
            resultMap[FTValue("image")] = FTValue(raw_image);
            sharedPtrResult->Success(resultMap);
        },
        (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePublishStreamVideo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamVideo(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode(
        (EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(),
                                                  (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendAudioSideInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto timeStampMs = std::get<double>(argument[FTValue("timeStampMs")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendAudioSideInfo(
        byteData.data(), (unsigned int)byteData.size(), timeStampMs,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCapturePipelineScaleMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCapturePipelineScaleMode(
        (EXPRESS::ZegoCapturePipelineScaleMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableH265EncodeFallback(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableH265EncodeFallback(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoEncoderSupported(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto codecID = std::get<int32_t>(argument[FTValue("codecID")]);
    if (codecID > 4) {
        codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
    }

    int ret = 0;
    if (!argument[FTValue("codecBackend")].IsNull()) {
        auto codecBackend = std::get<int32_t>(argument[FTValue("codecBackend")]);
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoEncoderSupported(
            (EXPRESS::ZegoVideoCodecID)codecID, (EXPRESS::ZegoVideoCodecBackend)codecBackend);
    } else {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoEncoderSupported(
                  (EXPRESS::ZegoVideoCodecID)codecID)
                  ? 1
                  : 0;
    }

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setLowlightEnhancement(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setLowlightEnhancement(
        (EXPRESS::ZegoLowlightEnhancementMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto source = std::get<int32_t>(argument[FTValue("source")]);

    bool hasInstanceID = false;
    int instanceID = -1;
    if (!argument[FTValue("instanceID")].IsNull()) {
        hasInstanceID = true;
        instanceID = std::get<int32_t>(argument[FTValue("instanceID")]);
    }

    bool hasChannel = false;
    int channel = 0;
    if (!argument[FTValue("channel")].IsNull()) {
        hasChannel = true;
        channel = std::get<int32_t>(argument[FTValue("channel")]);
    }

    ZegoTextureRendererController::getInstance()->setVideoSourceChannel((EXPRESS::ZegoPublishChannel)channel, (EXPRESS::ZegoVideoSourceType)source);

    int ret = 0;
    if (!hasChannel && !hasInstanceID) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setVideoSource(
            (EXPRESS::ZegoVideoSourceType)source);
    } else if (hasChannel && !hasInstanceID) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setVideoSource(
            (EXPRESS::ZegoVideoSourceType)source, (EXPRESS::ZegoPublishChannel)channel);
    } else if (!hasChannel && hasInstanceID) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setVideoSource(
            (EXPRESS::ZegoVideoSourceType)source, instanceID);
    } else {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setVideoSource(
            (EXPRESS::ZegoVideoSourceType)source, instanceID, (EXPRESS::ZegoPublishChannel)channel);
    }

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto source = std::get<int32_t>(argument[FTValue("source")]);

    bool hasConfig = false;
    EXPRESS::ZegoAudioSourceMixConfig config;
    if (!argument[FTValue("config")].IsNull()) {
        hasConfig = true;
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);

        auto audioEffectPlayerIndexList =
            std::get<FTArray>(configMap[FTValue("audioEffectPlayerIndexList")]);
        config.audioEffectPlayerCount = audioEffectPlayerIndexList.size();
        int *audioEffectPlayers = nullptr;
        if (config.audioEffectPlayerCount > 0) {
            audioEffectPlayers = new int[config.audioEffectPlayerCount];
        }
        config.audioEffectPlayerIndexList = audioEffectPlayers;
        for (auto audioEffectPlayerIndex : audioEffectPlayerIndexList) {
            *audioEffectPlayers = std::get<int32_t>(audioEffectPlayerIndex);
            audioEffectPlayers += 1;
        }

        auto mediaPlayerIndexList = std::get<FTArray>(configMap[FTValue("mediaPlayerIndexList")]);
        config.mediaPlayerCount = mediaPlayerIndexList.size();
        int *mediaPlayers = nullptr;
        if (config.mediaPlayerCount > 0) {
            mediaPlayers = new int[config.mediaPlayerCount];
        }
        config.mediaPlayerIndexList = mediaPlayers;
        for (auto mediaPlayerIndex : mediaPlayerIndexList) {
            *mediaPlayers = std::get<int32_t>(mediaPlayerIndex);
            mediaPlayers += 1;
        }

        config.enableMixEnginePlayout =
            std::get<bool>(configMap[FTValue("enableMixEnginePlayout")]);
        config.enableMixSystemPlayout =
            std::get<bool>(configMap[FTValue("enableMixSystemPlayout")]);
    }

    bool hasChannel = false;
    int channel = -1;
    if (!argument[FTValue("channel")].IsNull()) {
        hasChannel = true;
        channel = std::get<int32_t>(argument[FTValue("channel")]);
    }

    int ret = -1;
    if (!hasChannel && !hasConfig) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setAudioSource(
            (EXPRESS::ZegoAudioSourceType)source);
    } else if (hasChannel && !hasConfig) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setAudioSource(
            (EXPRESS::ZegoAudioSourceType)source, (EXPRESS::ZegoPublishChannel)channel);
    } else if (!hasChannel && hasConfig) {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->setAudioSource(
            (EXPRESS::ZegoAudioSourceType)source, config);
        delete[] config.audioEffectPlayerIndexList;
        delete[] config.mediaPlayerIndexList;
    } else {
        if (channel == 0) {
            ret = EXPRESS::ZegoExpressSDK::getEngine()->setAudioSource(
                (EXPRESS::ZegoAudioSourceType)source, config);
        }
        delete[] config.audioEffectPlayerIndexList;
        delete[] config.mediaPlayerIndexList;
    }

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::enableVideoObjectSegmentation(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoObjectSegmentationConfig config;
    if (!argument[FTValue("config")].IsNull()) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);

        auto type = std::get<int32_t>(configMap[FTValue("objectSegmentationType")]);
        config.objectSegmentationType = (EXPRESS::ZegoObjectSegmentationType)type;

        if (!configMap[FTValue("backgroundConfig")].IsNull()) {
            auto backgroundConfigMap = std::get<FTMap>(configMap[FTValue("backgroundConfig")]);

            EXPRESS::ZegoBackgroundConfig backgroundConfig;
            backgroundConfig.processType = (EXPRESS::ZegoBackgroundProcessType)std::get<int32_t>(backgroundConfigMap[FTValue("processType")]);
            backgroundConfig.blurLevel = (EXPRESS::ZegoBackgroundBlurLevel)std::get<int32_t>(backgroundConfigMap[FTValue("blurLevel")]);
            backgroundConfig.color = std::get<int32_t>(backgroundConfigMap[FTValue("color")]);
            backgroundConfig.imageURL = std::get<std::string>(backgroundConfigMap[FTValue("imageURL")]);
            backgroundConfig.videoURL = std::get<std::string>(backgroundConfigMap[FTValue("videoURL")]);

            config.backgroundConfig = backgroundConfig;
        }
    }
    

    EXPRESS::ZegoExpressSDK::getEngine()->enableVideoObjectSegmentation(
        enable, config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlphaChannelVideoEncoder(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto alphaLayout = std::get<int32_t>(argument[FTValue("alphaLayout")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAlphaChannelVideoEncoder(
        enable, (EXPRESS::ZegoAlphaLayoutType)alphaLayout, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    flutter::EncodableMap canvasMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("canvas")])) {
        canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        auto alphaBlend = std::get<bool>(canvasMap[FTValue("alphaBlend")]);

        ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
        ZegoTextureRendererController::getInstance()->addRemoteRenderer(viewID, streamID, viewMode);
        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode =
            (EXPRESS::ZegoStreamResourceMode)std::get<int32_t>(configMap[FTValue("resourceMode")]);
        auto videoCodecIDIndex = std::get<int32_t>(configMap[FTValue("videoCodecID")]);
        if (videoCodecIDIndex > 4) {
            config.videoCodecID = EXPRESS::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
        } else {
            config.videoCodecID = (EXPRESS::ZegoVideoCodecID)videoCodecIDIndex;
        }

        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.sourceResourceType =
            (EXPRESS::ZegoResourceType)std::get<int32_t>(configMap[FTValue("sourceResourceType")]);
        config.codecTemplateID =
            (EXPRESS::ZegoResourceType)std::get<int32_t>(configMap[FTValue("codecTemplateID")]);
        config.resourceSwitchMode = (EXPRESS::ZegoStreamResourceSwitchMode)std::get<int32_t>(configMap[FTValue("resourceSwitchMode")]);
        config.resourceWhenStopPublish = (EXPRESS::ZegoStreamResourceType)std::get<int32_t>(configMap[FTValue("resourceWhenStopPublish")]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (std::holds_alternative<flutter::EncodableMap>(configMap[FTValue("cdnConfig")])) {
            auto cdnConfigMap = std::get<flutter::EncodableMap>(configMap[FTValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = std::get<std::string>(cdnConfigMap[FTValue("url")]);
                cdnConfigPtr->authParam = std::get<std::string>(cdnConfigMap[FTValue("authParam")]);
                cdnConfigPtr->protocol = std::get<std::string>(cdnConfigMap[FTValue("protocol")]);
                cdnConfigPtr->quicVersion =
                    std::get<std::string>(cdnConfigMap[FTValue("quicVersion")]);
                cdnConfigPtr->httpdns =
                    (EXPRESS::ZegoHttpDNSType)std::get<int32_t>(cdnConfigMap[FTValue("httpdns")]);

                cdnConfigPtr->quicConnectMode = (int)std::get<int32_t>(cdnConfigMap[FTValue("quicConnectMode")]);
            }
        }

        config.cdnConfig = cdnConfigPtr.get();

        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr, config);
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPlayingStream(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    flutter::EncodableMap infoMap = std::get<flutter::EncodableMap>(argument[FTValue("info")]);
    EXPRESS::ZegoCrossAppInfo info;
    info.appID = infoMap[FTValue("appID")].LongValue();
    info.token = std::get<std::string>(infoMap[FTValue("token")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamCrossAppInfo(streamID, info);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePlayStreamSnapshot(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->takePlayStreamSnapshot(
        streamID, [=](int errorCode, void *image) {
            // TODO : prevent crash
            std::vector<uint8_t> raw_image;
            if(image)
            {
                auto tmpData = CreateFromHBITMAP((HBITMAP)image);
                raw_image.assign(tmpData.second, tmpData.second + tmpData.first);
                delete[] tmpData.second;
            }

            FTMap resultMap;
            resultMap[FTValue("errorCode")] = FTValue(errorCode);
            resultMap[FTValue("image")] = FTValue(raw_image);
            sharedPtrResult->Success(resultMap);
        });
}

void ZegoExpressEngineMethodHandler::setPlayVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayAudioStreams(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayAudioStreams(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto type = std::get<int32_t>(argument[FTValue("type")]);

    FTArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceList(
        (EXPRESS::ZegoAudioDeviceType)type);
    for (auto &deviceInfo : deviceList) {
        FTMap deviceMap;
        deviceMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(FTValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultAudioDeviceID(
        (EXPRESS::ZegoAudioDeviceType)type);

    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::useAudioDevice(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    EXPRESS::ZegoExpressSDK::getEngine()->useAudioDevice((EXPRESS::ZegoAudioDeviceType)type,
                                                         deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
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

void ZegoExpressEngineMethodHandler::stopSoundLevelMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopSoundLevelMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    int volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    int mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableSpeechEnhance(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto enable = std::get<bool>(argument[FTValue("enable")]);
    int level = std::get<int32_t>(argument[FTValue("level")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableSpeechEnhance(enable, level);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto bandIndex = std::get<int32_t>(argument[FTValue("bandIndex")]);
    auto bandGain = std::get<double>(argument[FTValue("bandGain")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset(
        (EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)std::get<double>(paramMap[FTValue("pitch")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
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

void ZegoExpressEngineMethodHandler::setReverbEchoParam(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)std::get<double>(paramMap[FTValue("inGain")]);
    param.outGain = (float)std::get<double>(paramMap[FTValue("outGain")]);
    param.numDelays = std::get<int>(paramMap[FTValue("numDelays")]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    FTArray delayVec = std::get<FTArray>(paramMap[FTValue("delay")]);
    FTArray decayVec = std::get<FTArray>(paramMap[FTValue("decay")]);

    for (int i = 0; i < param.numDelays; i++) {
        param.delay[i] = std::get<int32_t>(delayVec.at(i));
        param.decay[i] = (float)std::get<double>(decayVec.at(i));
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbEchoParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVirtualStereo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePlayStreamVirtualStereo(enable, angle, streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setElectronicEffects(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto tonal = std::get<int32_t>(argument[FTValue("tonal")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setElectronicEffects(
        enable, (ZEGO::EXPRESS::ZegoElectronicEffectsMode)mode, tonal);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioDataObserver(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto bitmask = std::get<int32_t>(argument[FTValue("observerBitMask")]);
    auto param = std::get<FTMap>(argument[FTValue("param")]);

    EXPRESS::ZegoAudioFrameParam nativeParam;
    nativeParam.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(param[FTValue("sampleRate")]);
    nativeParam.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(param[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDataObserver(bitmask, nativeParam);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDataObserver(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDataObserver();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAudioEffectPlayer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto player = EXPRESS::ZegoExpressSDK::getEngine()->createAudioEffectPlayer();
    if (player) {

        int index = player->getIndex();
        player->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        audioEffectPlayerMap_[index] = player;

        result->Success(FTValue(index));

    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        EXPRESS::ZegoExpressSDK::getEngine()->destroyAudioEffectPlayer(player);
        audioEffectPlayerMap_.erase(index);

        result->Success();
    } else {
        result->Error("destroyAudioEffectPlayer_Can_not_find_player",
                      "Invoke `destroyAudioEffectPlayer` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStart(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
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
    } else {
        result->Error("audioEffectPlayerStart_Can_not_find_player",
                      "Invoke `audioEffectPlayerStart` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStop(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->stop(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerStop_Can_not_find_player",
                      "Invoke `audioEffectPlayerStop` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPause(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->pause(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerPause_Can_not_find_player",
                      "Invoke `audioEffectPlayerPause` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->resume(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerResume_Can_not_find_player",
                      "Invoke `audioEffectPlayerResume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->stopAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerStopAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerStopAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->pauseAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerPauseAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerPauseAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->resumeAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerResumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerResumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->seekTo(audioEffectID, millisecond, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("audioEffectPlayerSeekTo_Can_not_find_player",
                      "Invoke `audioEffectPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setPlayVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setPublishVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPublishVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPublishVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolumeAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setPlayVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolumeAll(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setPublishVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPublishVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPublishVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto totalDuration = player->getTotalDuration(audioEffectID);

        result->Success(FTValue((int64_t)totalDuration));
    } else {
        result->Error("audioEffectPlayerGetTotalDuration_Can_not_find_player",
                      "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto currentProgress = player->getCurrentProgress(audioEffectID);

        result->Success(FTValue((int64_t)currentProgress));
    } else {
        result->Error(
            "audioEffectPlayerGetCurrentProgress_Can_not_find_player",
            "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->loadResource(audioEffectID, path, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("audioEffectPlayerLoadResource_Can_not_find_player",
                      "Invoke `audioEffectPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->unloadResource(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerUnLoadResource_Can_not_find_player",
                      "Invoke `audioEffectPlayerUnloadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto speed = std::get<double>(argument[FTValue("speed")]);
        player->setPlaySpeed(audioEffectID, speed);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlaySpeed_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUpdatePosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);
        player->updatePosition(audioEffectID, position.data());

        result->Success();
    } else {
        result->Error("audioEffectPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `audioEffectPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaPlayer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mediaPlayer = EXPRESS::ZegoExpressSDK::getEngine()->createMediaPlayer();
    if (mediaPlayer) {
        auto index = mediaPlayer->getIndex();

        mediaPlayer->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaPlayer->setVideoHandler(ZegoTextureRendererController::getInstance(),
                                     ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32);
        mediaPlayerMap_[index] = mediaPlayer;

        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaPlayer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->setEventHandler(nullptr);
        ZegoTextureRendererController::getInstance()->removeMediaPlayerRenderer(mediaPlayer);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyMediaPlayer(mediaPlayer);
    }

    mediaPlayerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResource(path, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerLoadResource_Can_not_find_player",
                      "Invoke `mediaPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto mediaData = std::get<std::vector<uint8_t>>(argument[FTValue("mediaData")]);

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceFromMediaData(
            mediaData.data(), (int)mediaData.size(), startPosition, [=](int errorCode) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);

                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error(
            "mediaPlayerLoadResourceFromMediaData_Can_not_find_player",
            "Invoke `mediaPlayerLoadResourceFromMediaData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceWithPosition(path, startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "mediaPlayerLoadResourceWithPosition_Can_not_find_player",
            "Invoke `mediaPlayerLoadResourceWithPosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerStart(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->start();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerStop(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->stop();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerPause(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->pause();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerResume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->resume();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSeekTo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        mediaPlayer->seekTo(millisecond, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerSeekTo_Can_not_find_player",
                      "Invoke `mediaPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableRepeat(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAux(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableAux(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool mute = std::get<bool>(argument[FTValue("mute")]);

        mediaPlayer->muteLocal(mute);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPlayVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPublishVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int millsecond = std::get<int32_t>(argument[FTValue("millisecond")]);

        mediaPlayer->setProgressInterval((unsigned long long)millsecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPlayVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPublishVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto totalDuration = mediaPlayer->getTotalDuration();
        // TODO: need test
        result->Success(FTValue((int64_t)totalDuration));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto currentProgress = mediaPlayer->getCurrentProgress();
        // TODO: need test
        result->Success(FTValue((int64_t)currentProgress));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioTrackCount = mediaPlayer->getAudioTrackCount();
        result->Success(FTValue((int32_t)audioTrackCount));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto trackIndex = std::get<int32_t>(argument[FTValue("trackIndex")]);
        mediaPlayer->setAudioTrackIndex(trackIndex);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        FTMap paramMap = std::get<FTMap>(argument[FTValue("param")]);
        auto pitch = std::get<double>(paramMap[FTValue("pitch")]);

        auto audioChannel = std::get<int32_t>(argument[FTValue("audioChannel")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->setVoiceChangerParam((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel,
                                          param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto state = mediaPlayer->getCurrentState();
        result->Success(FTValue((int32_t)state));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto speed = std::get<double>(argument[FTValue("speed")]);
        mediaPlayer->setPlaySpeed(speed);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableSoundLevelMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableFrequencySpectrumMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto time = argument[FTValue("time")].LongValue();
        auto size = argument[FTValue("size")].LongValue();
        mediaPlayer->setNetWorkResourceMaxCache(time, size);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto threshold = argument[FTValue("threshold")].LongValue();
        mediaPlayer->setNetWorkBufferThreshold(threshold);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        FTMap resultMap;
        auto netWorkResourceCache = mediaPlayer->getNetWorkResourceCache();

        resultMap[FTValue("time")] = FTValue((int64_t)netWorkResourceCache->time);
        resultMap[FTValue("size")] = FTValue((int64_t)netWorkResourceCache->size);
        result->Success(resultMap);
    } else {
        result->Error("mediaPlayerGetNetWorkResourceCache_Can_not_find_player",
                      "Invoke `mediaPlayerGetNetWorkResourceCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAccurateSeek(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
        auto timeout = std::get<int>(configMap[FTValue("timeout")]);
        EXPRESS::ZegoAccurateSeekConfig config;
        config.timeout = timeout;
        mediaPlayer->enableAccurateSeek(enable, &config);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableAccurateSeek_Can_not_find_player",
                      "Invoke `mediaPlayerEnableAccurateSeek` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadCopyrightedMusicResourceWithPosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadCopyrightedMusicResourceWithPosition(
            resourceID, startPosition, [=](int errorCode) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);

                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("mediaPlayerLoadCopyrightedMusicResourceWithPosition_Can_not_find_player",
                      "Invoke `mediaPlayerLoadCopyrightedMusicResourceWithPosition` but can't find "
                      "specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerClearView(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->clearView();

        result->Success();
    } else {
        result->Error("mediaPlayerClearView_Can_not_find_player",
                      "Invoke `mediaPlayerClearView` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetActiveAudioChannel(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioChannel = std::get<int32_t>(argument[FTValue("audioChannel")]);
        mediaPlayer->setActiveAudioChannel((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel);

        result->Success();
    } else {
        result->Error("mediaPlayerSetActiveAudioChannel_Can_not_find_player",
                      "Invoke `mediaPlayerSetActiveAudioChannel` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {

        flutter::EncodableMap canvasMap =
            std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);

        EXPRESS::ZegoCanvas canvas;
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        if (ZegoTextureRendererController::getInstance()->addMediaPlayerRenderer(
                viewID, mediaPlayer, viewMode)) {
            result->Success();
        } else {
            result->Error(
                "mediaPlayerSetPlayerCanvas_Canvas_error",
                "Invoke `mediaPlayerSetPlayerCanvas` but canvas is abnormal, please check canvas");
        }
    } else {
        result->Error("mediaPlayerSetPlayerCanvas_Can_not_find_player",
                      "Invoke `mediaPlayerSetPlayerCanvas` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerTakeSnapshot(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto pFrame =
            ZegoTextureRendererController::getInstance()->getMediaPlayerFrame(mediaPlayer);
        auto size = ZegoTextureRendererController::getInstance()->getMediaPlayerSize(mediaPlayer);
        FTMap resultMap;
        if (pFrame && size != std::pair(0, 0)) {
            auto tmpData = makeBtimap(pFrame, size);
            std::vector<uint8_t> raw_image(tmpData.second, tmpData.second + tmpData.first);
            delete[] tmpData.second;

            resultMap[FTValue("image")] = FTValue(raw_image);
            resultMap[FTValue("errorCode")] = FTValue(0);
        } else {
            resultMap[FTValue("errorCode")] = FTValue(-1);
        }
        result->Success(resultMap);
    } else {
        result->Error("mediaPlayerTakeSnapshot_Can_not_find_player",
                      "Invoke `mediaPlayerTakeSnapshot` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    // auto index = std::get<int32_t>(argument[FTValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    //     mediaPlayer->setAudioTrackMode((EXPRESS::ZegoMediaPlayerAudioTrackMode)mode);

    //     result->Success();
    // }
    // else
    // {
    //     result->Error("mediaPlayerSetAudioTrackMode_Can_not_find_player", "Invoke `mediaPlayerSetAudioTrackMode` but can't find specific player");
    // }
    result->Error("not_support_feature", "windows platform not support feature");
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackPublishIndex(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    // auto index = std::get<int32_t>(argument[FTValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto index_ = std::get<int32_t>(argument[FTValue("index_")]);
    //     mediaPlayer->setAudioTrackPublishIndex(index_);

    //     result->Success();
    // }
    // else
    // {
    //     result->Error("mediaPlayerSetAudioTrackPublishIndex_Can_not_find_player", "Invoke `mediaPlayerSetAudioTrackPublishIndex` but can't find specific player");
    // }
    result->Error("not_support_feature", "windows platform not support feature");
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAudioData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        if (enable) {
            mediaPlayer->setAudioHandler(
                ZegoMediaPlayerAudioManager::getInstance()->getHandler());
        } else {
            mediaPlayer->setAudioHandler(nullptr);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableAudioData_Can_not_find_player",
                      "Invoke `mediaPlayerEnableAudioData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableVideoData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        if (enable) {
            ZegoTextureRendererController::getInstance()->setMediaPlayerVideoHandler(
                ZegoMediaPlayerVideoManager::getInstance()->getHandler());
        } else {
            ZegoTextureRendererController::getInstance()->setMediaPlayerVideoHandler(nullptr);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableVideoData_Can_not_find_player",
                      "Invoke `mediaPlayerEnableVideoData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableBlockData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        unsigned int blockSize = argument[FTValue("blockSize")].LongValue();
        if (enable) {
            mediaPlayer->setBlockDataHandler(ZegoMediaPlayerBlockDataManager::getInstance()->getHandler(), blockSize);
        } else {
            mediaPlayer->setBlockDataHandler(nullptr, blockSize);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableBlockData_Can_not_find_player",
                    "Invoke `mediaPlayerEnableBlockData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto resourceMap = std::get<FTMap>(argument[FTValue("resource")]);
        EXPRESS::ZegoMediaPlayerResource resource;
        resource.resourceID = std::get<std::string>(resourceMap[FTValue("resourceID")]);
        resource.loadType =
            (EXPRESS::ZegoMultimediaLoadType)std::get<int32_t>(resourceMap[FTValue("loadType")]);
        resource.alphaLayout =
            (EXPRESS::ZegoAlphaLayoutType)std::get<int32_t>(resourceMap[FTValue("alphaLayout")]);
        resource.startPosition = resourceMap[FTValue("startPosition")].LongValue();
        resource.filePath = std::get<std::string>(resourceMap[FTValue("filePath")]);
        auto memory = std::get<std::vector<uint8_t>>(resourceMap[FTValue("memory")]);
        resource.memory = memory.data();
        resource.memoryLength = memory.size();

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceWithConfig(resource, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerLoadResourceWithConfig_Can_not_find_player",
                      "Invoke `mediaPlayerLoadResourceWithConfig` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerUpdatePosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);
        mediaPlayer->updatePosition(position.data());

        result->Success();
    } else {
        result->Error("mediaPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `mediaPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetMediaInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto info = mediaPlayer->getMediaInfo();

        FTMap resultMap;

        resultMap[FTValue("width")] = FTValue((int32_t)info.width);
        resultMap[FTValue("height")] = FTValue((int32_t)info.height);
        resultMap[FTValue("frameRate")] = FTValue((int32_t)info.frameRate);
        result->Success(resultMap);

    } else {
        result->Error("mediaPlayerGetMediaInfo_Can_not_find_player",
                      "Invoke `mediaPlayerGetMediaInfo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetHttpHeader(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto headersMap = std::get<FTMap>(argument[FTValue("headers")]);
        std::unordered_map<std::string, std::string> headers;
        for (auto &header : headersMap) {
            std::string key = std::get<std::string>(header.first);
            std::string value = std::get<std::string>(header.second);
            headers[key] = value;
        }

        mediaPlayer->setHttpHeader(headers);

        result->Success();
    } else {
        result->Error("mediaPlayerSetHttpHeader_Can_not_find_player",
                      "Invoke `mediaPlayerSetHttpHeader` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentRenderingProgress(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto progress = mediaPlayer->getCurrentRenderingProgress();
        result->Success(FTValue((int64_t)progress));

    } else {
        result->Error("mediaPlayerGetCurrentRenderingProgress_Can_not_find_player",
                      "Invoke `mediaPlayerGetCurrentRenderingProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableLiveAudioEffect(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto mode = std::get<int32_t>(argument[FTValue("mode")]);

        mediaPlayer->enableLiveAudioEffect(enable, (EXPRESS::ZegoLiveAudioEffectMode) mode);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLiveAudioEffect_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLiveAudioEffect` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayMediaStreamType(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto streamType = std::get<int32_t>(argument[FTValue("streamType")]);

        mediaPlayer->setPlayMediaStreamType((EXPRESS::ZegoMediaStreamType)streamType);

        result->Success();
    } else {
        result->Error("mediaPlayerSetPlayMediaStreamType_Can_not_find_player",
                      "Invoke `mediaPlayerSetPlayMediaStreamType` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableLocalCache(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        std::string cacheDir = std::get<std::string>(argument[FTValue("cacheDir")]);

        mediaPlayer->enableLocalCache(enable, cacheDir);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLocalCache_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLocalCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlaybackStatistics(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto info = mediaPlayer->getPlaybackStatistics();
        FTMap retMap;
        retMap[FTValue("videoSourceFps")] = FTValue(info.videoSourceFps);
        retMap[FTValue("videoDecodeFps")] = FTValue(info.videoDecodeFps);
        retMap[FTValue("videoRenderFps")] = FTValue(info.videoRenderFps);
        retMap[FTValue("audioSourceFps")] = FTValue(info.audioSourceFps);
        retMap[FTValue("audioDecodeFps")] = FTValue(info.audioDecodeFps);
        retMap[FTValue("audioRenderFps")] = FTValue(info.audioRenderFps);
        result->Success(retMap);

    } else {
        result->Error("mediaPlayerGetPlaybackStatistics_Can_not_find_player",
                      "Invoke `mediaPlayerGetPlaybackStatistics` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaDataPublisher(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    FTMap config_map = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoMediaDataPublisherConfig config{};
    config.channel = std::get<int32_t>(config_map[FTValue("channel")]);
    config.mode =
        (EXPRESS::ZegoMediaDataPublisherMode)std::get<int32_t>(config_map[FTValue("mode")]);

    auto publisher = EXPRESS::ZegoExpressSDK::getEngine()->createMediaDataPublisher(config);
    if (publisher) {
        int index = publisher->getIndex();
        publisher->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaDataPublisherMap_[index] = publisher;
        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaDataPublisher(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyMediaDataPublisher(publisher);
        mediaDataPublisherMap_.erase(index);
        result->Success();
    } else {
        result->Error("destroyMediaDataPublisher_Can_not_find_publisher",
                      "Invoke `destroyMediaDataPublisher` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherAddMediaFilePath(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        bool is_clear = std::get<bool>(argument[FTValue("isClear")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);
        publisher->addMediaFilePath(path, is_clear);
        result->Success();
    } else {
        result->Error("mediaDataPublisherAddMediaFilePath_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherAddMediaFilePath` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherGetCurrentDuration(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        auto duration = publisher->getCurrentDuration();
        result->Success(FTValue((int64_t)duration));
    } else {
        result->Error("mediaDataPublisherGetCurrentDuration_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherGetCurrentDuration` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherGetTotalDuration(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        auto duration = publisher->getTotalDuration();
        result->Success(FTValue((int64_t)duration));
    } else {
        result->Error("mediaDataPublisherGetTotalDuration_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherGetTotalDuration` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherReset(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        publisher->reset();
        result->Success();
    } else {
        result->Error("mediaDataPublisherReset_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherReset` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherSeekTo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();
        publisher->seekTo(millisecond);
        result->Success();
    } else {
        result->Error("mediaDataPublisherSeekTo_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherSeekTo` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherSetVideoSendDelayTime(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        int delay_time = std::get<int32_t>(argument[FTValue("delay_time")]);
        publisher->setVideoSendDelayTime(delay_time);
        result->Success();
    } else {
        result->Error("mediaDataPublisherSetVideoSendDelayTime_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherSetVideoSendDelayTime` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::startMixerTask(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);

    for (auto &cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    auto inputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("inputList")]);
    auto outputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("outputList")]);

    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    // backgroundColor
    task.backgroundColor = std::get<int32_t>(argument[FTValue("backgroundColor")]);

    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)std::get<int32_t>(
        argument[FTValue("streamAlignmentMode")]);

    // userData
    auto userData = std::get<std::vector<uint8_t>>(argument[FTValue("userData")]);
    task.userData = userData.data();
    task.userDataLength = userData.size();

    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        std::get<int32_t>(argument[FTValue("minPlayStreamBufferLength")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);
    task.audioConfig.mixMode =
        (EXPRESS::ZegoAudioMixMode)std::get<int32_t>(taskAudioConfig[FTValue("mixMode")]);

    task.videoConfig.bitrate = std::get<int32_t>(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = std::get<int32_t>(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = std::get<int32_t>(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = std::get<int32_t>(taskVideoConfig[FTValue("width")]);
    task.videoConfig.quality = std::get<int32_t>(taskVideoConfig[FTValue("quality")]);
    task.videoConfig.rateControlMode = (EXPRESS::ZegoVideoRateControlMode)std::get<int32_t>(
        taskVideoConfig[FTValue("rateControlMode")]);

    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = std::get<FTMap>(inputIter);

        input.streamID = std::get<std::string>(inputMap[FTValue("streamID")]);
        input.contentType =
            (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) -
                              std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) -
                             std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        input.volume = std::get<int32_t>(inputMap[FTValue("volume")]);
        input.isAudioFocus = std::get<bool>(inputMap[FTValue("isAudioFocus")]);
        input.audioDirection = std::get<int32_t>(inputMap[FTValue("audioDirection")]);

        if (!inputMap[FTValue("label")].IsNull()) {
            auto labelMap = std::get<flutter::EncodableMap>(inputMap[FTValue("label")]);
            auto text = std::get<std::string>(labelMap[FTValue("text")]);
            EXPRESS::ZegoLabelInfo labelInfo(text);
            labelInfo.left = std::get<int32_t>(labelMap[FTValue("left")]);
            labelInfo.top = std::get<int32_t>(labelMap[FTValue("top")]);
            auto fontMap = std::get<flutter::EncodableMap>(labelMap[FTValue("font")]);
            EXPRESS::ZegoFontStyle fontStyle;
            fontStyle.type = (EXPRESS::ZegoFontType)std::get<int32_t>(fontMap[FTValue("type")]);
            fontStyle.size = std::get<int32_t>(fontMap[FTValue("size")]);
            fontStyle.color = std::get<int32_t>(fontMap[FTValue("color")]);
            fontStyle.transparency = std::get<int32_t>(fontMap[FTValue("transparency")]);
            fontStyle.border = std::get<bool>(fontMap[FTValue("border")]);
            fontStyle.borderColor = std::get<int32_t>(fontMap[FTValue("borderColor")]);
            labelInfo.font = fontStyle;
            input.label = labelInfo;
        }

        if (!inputMap[FTValue("renderMode")].IsNull()) {
            input.renderMode =
                (EXPRESS::ZegoMixRenderMode)std::get<int32_t>(inputMap[FTValue("renderMode")]);
        }

        if (!inputMap[FTValue("imageInfo")].IsNull()) {
            auto imageInfoMap = std::get<flutter::EncodableMap>(inputMap[FTValue("imageInfo")]);
            auto url = std::get<std::string>(imageInfoMap[FTValue("url")]);
            int32_t displayMode = 0;
            if (!imageInfoMap[FTValue("displayMode")].IsNull()) {
                displayMode = std::get<int32_t>(imageInfoMap[FTValue("displayMode")]);
            }
            input.imageInfo = EXPRESS::ZegoMixerImageInfo(url, displayMode);
        }

        if (!inputMap[FTValue("cornerRadius")].IsNull()) {
            input.cornerRadius = std::get<int32_t>(inputMap[FTValue("cornerRadius")]);
        }

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        if (!outputMap[FTValue("videoConfig")].IsNull()) {
            auto videoConfigMap =
                std::get<flutter::EncodableMap>(outputMap[FTValue("videoConfig")]);
            output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
            output.videoConfig.encodeLatency =
                std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
            output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)std::get<int32_t>(
                videoConfigMap[FTValue("encodeProfile")]);
            output.videoConfig.enableLowBitrateHD = std::get<bool>(
                videoConfigMap[FTValue("enableLowBitrateHD")]);

            auto codecID = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
            if (codecID > 4) {
                codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
            }
            output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecID;
        }

        task.outputList.push_back(output);
    }

    // Water mark
    EXPRESS::ZegoWatermark watermark;
    if (!argument[FTValue("watermark")].IsNull()) {
        auto watermarkMap = std::get<flutter::EncodableMap>(argument[FTValue("watermark")]);
        std::string imageURL = std::get<std::string>(watermarkMap[FTValue("imageURL")]);
        if (!imageURL.empty()) {    
            watermark.imageURL = imageURL;
            watermark.layout.x = std::get<int32_t>(watermarkMap[FTValue("left")]);
            watermark.layout.y = std::get<int32_t>(watermarkMap[FTValue("top")]);
            watermark.layout.width = std::get<int32_t>(watermarkMap[FTValue("right")]) -
                                     std::get<int32_t>(watermarkMap[FTValue("left")]);
            watermark.layout.height = std::get<int32_t>(watermarkMap[FTValue("bottom")]) -
                                      std::get<int32_t>(watermarkMap[FTValue("top")]);

            task.watermark = &watermark;
        }
    }

    // whiteboard
    EXPRESS::ZegoMixerWhiteboard whiteboard;
    if (!argument[FTValue("whiteboard")].IsNull()) {
        auto whiteboardMap = std::get<flutter::EncodableMap>(argument[FTValue("whiteboard")]);
        int64_t whiteboardID = whiteboardMap[FTValue("whiteboardID")].LongValue();
        if (whiteboardID != 0) {
            whiteboard.whiteboardID = whiteboardID;
            whiteboard.horizontalRatio =
                std::get<int32_t>(whiteboardMap[FTValue("horizontalRatio")]);
            whiteboard.verticalRatio = std::get<int32_t>(whiteboardMap[FTValue("verticalRatio")]);
            whiteboard.isPPTAnimation = std::get<bool>(whiteboardMap[FTValue("isPPTAnimation")]);
            whiteboard.zOrder = std::get<int32_t>(whiteboardMap[FTValue("zOrder")]);

            auto layoutMap = std::get<flutter::EncodableMap>(whiteboardMap[FTValue("layout")]);
            whiteboard.layout.x = std::get<int32_t>(layoutMap[FTValue("left")]);
            whiteboard.layout.y = std::get<int32_t>(layoutMap[FTValue("top")]);
            whiteboard.layout.width = std::get<int32_t>(layoutMap[FTValue("right")]) -
                                      std::get<int32_t>(layoutMap[FTValue("left")]);
            whiteboard.layout.height = std::get<int32_t>(layoutMap[FTValue("bottom")]) -
                                       std::get<int32_t>(layoutMap[FTValue("top")]);

            task.whiteboard = &whiteboard;
        }
    }

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            FTMap retMap;

            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("extendedData")] = FTValue(extendedData);

            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopMixerTask(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    auto taskWatermark = std::get<flutter::EncodableMap>(argument[FTValue("watermark")]);

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);

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
        input.contentType =
            (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) -
                              std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) -
                             std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        if (!outputMap[FTValue("videoConfig")].IsNull()) {
            auto videoConfigMap =
                std::get<flutter::EncodableMap>(outputMap[FTValue("videoConfig")]);
            output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
            output.videoConfig.encodeLatency =
                std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
            output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)std::get<int32_t>(
                videoConfigMap[FTValue("encodeProfile")]);
            output.videoConfig.enableLowBitrateHD = std::get<bool>(
                videoConfigMap[FTValue("enableLowBitrateHD")]);

            auto codecID = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
            if (codecID > 4) {
                codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
            }
            output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecID;
        }

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;
    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);
    for (auto &cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->stopMixerTask(task, [=](int errorCode) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setSEIConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoSEIConfig config;
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.type = (EXPRESS::ZegoSEIType)std::get<int32_t>(configMap[FTValue("type")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSEIConfig(config);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioDeviceVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setSpeakerVolumeInAPP(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSpeakerVolumeInAPP(deviceID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getSpeakerVolumeInAPP(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    int volume = EXPRESS::ZegoExpressSDK::getEngine()->getSpeakerVolumeInAPP(deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::startAudioDeviceVolumeMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDeviceVolumeMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAudioDevice(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAudioDevice((EXPRESS::ZegoAudioDeviceType)deviceType,
                                                          deviceID.c_str(), mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAudioDeviceMuted(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    auto ret = EXPRESS::ZegoExpressSDK::getEngine()->isAudioDeviceMuted(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioDeviceMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioDeviceVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    auto volume = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureDevice(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    std::thread tmpThread([=]() {
        EXPRESS::ZegoExpressSDK::getEngine()->enableAudioCaptureDevice(enable);
        sharedPtrResult->Success();
    });

    tmpThread.detach();
}

void ZegoExpressEngineMethodHandler::enableTrafficControl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto property = std::get<int32_t>(argument[FTValue("property")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTrafficControl(enable, property, 
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startRecordingCapturedData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoDataRecordConfig config;

    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.filePath = std::get<std::string>(configMap[FTValue("filePath")]);
    config.recordType =
        (EXPRESS::ZegoDataRecordType)std::get<int32_t>(configMap[FTValue("recordType")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startRecordingCapturedData(
        config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopRecordingCapturedData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopRecordingCapturedData(
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCamera(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCamera(enable,
                                                       (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto minFPS = std::get<int32_t>(argument[FTValue("minFPS")]);
    auto maxFPS = std::get<int32_t>(argument[FTValue("maxFPS")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCameraAdaptiveFPS(
        enable, minFPS, maxFPS, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::useVideoDevice(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->useVideoDevice(deviceID,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoDeviceList(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    FTArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getVideoDeviceList();
    for (auto &deviceInfo : deviceList) {
        FTMap deviceMap;
        deviceMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(FTValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultVideoDeviceID(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultVideoDeviceID();
    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::enableMixSystemPlayout(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixSystemPlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setMixSystemPlayoutVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto volume = std::get<int32_t>(argument[FTValue("volume")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setMixSystemPlayoutVolume(volume);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableMixEnginePlayout(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixEnginePlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioVADStableStateMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto type = std::get<int32_t>(argument[FTValue("type")]);
    if (argument[FTValue("millisecond")].IsNull()) {
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type);
    } else {
        auto millisecond = std::get<int32_t>(argument[FTValue("millisecond")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioVADStableStateMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto type = std::get<int32_t>(argument[FTValue("type")]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioVADStableStateMonitor(
        (EXPRESS::ZegoAudioVADStableStateMonitorType)type);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getCurrentAudioDevice(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceInfo = EXPRESS::ZegoExpressSDK::getEngine()->getCurrentAudioDevice(
        (EXPRESS::ZegoAudioDeviceType)deviceType);

    FTMap resultMap;
    resultMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
    resultMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);
    result->Success(resultMap);
}

void ZegoExpressEngineMethodHandler::createCopyrightedMusic(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto tmpCopyrightedMusic = EXPRESS::ZegoExpressSDK::getEngine()->createCopyrightedMusic();
    if (tmpCopyrightedMusic != nullptr) {
        copyrightedMusic_ = tmpCopyrightedMusic;
        copyrightedMusic_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(FTValue(0));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyCopyrightedMusic(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyCopyrightedMusic(copyrightedMusic_);
        copyrightedMusic_ = nullptr;
        result->Success();
    } else {
        result->Error("destroyCopyrightedMusic_Can_not_find_instance",
                      "Invoke `destroyCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicClearCache(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        copyrightedMusic_->clearCache();
        result->Success();
    } else {
        result->Error("copyrightedMusicClearCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicClearCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicDownload(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->download(resourceID, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicDownload_Can_not_find_instance",
                      "Invoke `copyrightedMusicDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicCancelDownload(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        copyrightedMusic_->cancelDownload(resourceID);
        result->Success();
    } else {
        result->Error("copyrightedMusicCancelDownload_Can_not_find_instance",
                      "Invoke `copyrightedMusicCancelDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getAverageScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetAverageScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetAverageScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto ret = copyrightedMusic_->getCacheSize();
        result->Success(FTValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetCacheSize_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCacheSize` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getCurrentPitch(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetCurrentPitch_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getDuration(resourceID);
        result->Success(FTValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetDuration_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetDuration` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto krcToken = std::get<std::string>(argument[FTValue("krcToken")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getKrcLyricByToken(krcToken, [=](int errorCode, std::string lyrics) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("lyrics")] = FTValue(lyrics);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicGetKrcLyricByToken_Can_not_find_instance",
            "Invoke `copyrightedMusicGetKrcLyricByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyric(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto songID = std::get<std::string>(argument[FTValue("songID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        if (argument[FTValue("vendorID")].IsNull()) {
            copyrightedMusic_->getLrcLyric(songID, [=](int errorCode, std::string lyrics) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);
                retMap[FTValue("lyrics")] = FTValue(lyrics);
                sharedPtrResult->Success(retMap);
            });
        } else {
            int vendorID = std::get<int>(argument[FTValue("vendorID")]);
            copyrightedMusic_->getLrcLyric(songID, (EXPRESS::ZegoCopyrightedMusicVendorID)vendorID,
                                           [=](int errorCode, std::string lyrics) {
                                               FTMap retMap;
                                               retMap[FTValue("errorCode")] = FTValue(errorCode);
                                               retMap[FTValue("lyrics")] = FTValue(lyrics);
                                               sharedPtrResult->Success(retMap);
                                           });
        }

    } else {
        result->Error("copyrightedMusicGetLrcLyric_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetLrcLyric` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyricWithConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetLyricConfig config;
        
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.vendorID = std::get<int32_t>(configMap[FTValue("vendorID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getLrcLyric(config, [=](int errorCode, std::string lyrics) {
                                               FTMap retMap;
                                               retMap[FTValue("errorCode")] = FTValue(errorCode);
                                               retMap[FTValue("lyrics")] = FTValue(lyrics);
                                               sharedPtrResult->Success(retMap);
                                           });
    } else {
        result->Error("copyrightedMusicGetLrcLyric_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetLrcLyricWithConfig` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetMusicByToken(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto shareToken = std::get<std::string>(argument[FTValue("shareToken")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getMusicByToken(shareToken, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicGetMusicByToken_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetMusicByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetPreviousScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getPreviousScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetPreviousScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getStandardPitch(resourceID, [=](int errorCode, std::string pitch) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("pitch")] = FTValue(pitch);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicGetStandardPitch_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetStandardPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetTotalScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getTotalScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetTotalScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetTotalScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicConfig config;
        auto userMap = std::get<FTMap>(configMap[FTValue("user")]);
        config.user.userID = std::get<std::string>(userMap[FTValue("userID")]);
        config.user.userName = std::get<std::string>(userMap[FTValue("userName")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->initCopyrightedMusic(config, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicInitCopyrightedMusic_Can_not_find_instance",
            "Invoke `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicPauseScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->pauseScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicPauseScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicPauseScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto songID = std::get<std::string>(argument[FTValue("songID")]);
        auto type = std::get<int32_t>(argument[FTValue("type")]);
        bool ret = false;
        if (argument[FTValue("vendorID")].IsNull()) {
            ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type);
        } else {
            int vendorID = std::get<int>(argument[FTValue("vendorID")]);
            ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type,
                                                (EXPRESS::ZegoCopyrightedMusicVendorID)vendorID);
        }
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCacheWithConfig(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfig config;
        
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID) std::get<int32_t>(configMap[FTValue("vendorID")]);
        config.resourceType = (EXPRESS::ZegoCopyrightedMusicResourceType) std::get<int32_t>(configMap[FTValue("resourceType")]);
        config.resourceQualityType = (EXPRESS::ZegoCopyrightedMusicResourceQualityType) std::get<int32_t>(configMap[FTValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCacheWithConfig` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCacheWithConfigV2(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfigV2 config;
        
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.vendorID = std::get<int32_t>(configMap[FTValue("vendorID")]);
        config.resourceType = std::get<int32_t>(configMap[FTValue("resourceType")]);
        config.resourceQualityType = std::get<int32_t>(configMap[FTValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCacheWithConfigV2` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)std::get<int32_t>(
            configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.masterID = std::get<std::string>(configMap[FTValue("masterID")]);
        config.sceneID = std::get<int32_t>(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestAccompaniment(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestAccompaniment_Can_not_find_instance",
                      "copyrightedMusicRequestAccompaniment `copyrightedMusicInitCopyrightedMusic` "
                      "but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompanimentClip(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)std::get<int32_t>(
            configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.masterID = std::get<std::string>(configMap[FTValue("masterID")]);
        config.sceneID = std::get<int32_t>(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestAccompanimentClip(
            config, [=](int errorCode, std::string resource) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);
                retMap[FTValue("resource")] = FTValue(resource);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("copyrightedMusicRequestAccompanimentClip_Can_not_find_instance",
                      "copyrightedMusicRequestAccompanimentClip "
                      "`copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestSong(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)std::get<int32_t>(
            configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.masterID = std::get<std::string>(configMap[FTValue("masterID")]);
        config.sceneID = std::get<int32_t>(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestSong(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestSong_Can_not_find_instance",
                      "copyrightedMusicRequestSong `copyrightedMusicInitCopyrightedMusic` but "
                      "can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResetScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resetScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicResetScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResetScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resumeScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicResumeScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResumeScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto command = std::get<std::string>(argument[FTValue("command")]);
        auto params = std::get<std::string>(argument[FTValue("params")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->sendExtendedRequest(
            command, params, [=](int errorCode, std::string command, std::string result) {
                ZF::logInfo("*** Plugin OnSendExtendedRequestCallback");
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);
                retMap[FTValue("command")] = FTValue(command);
                retMap[FTValue("result")] = FTValue(result);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("copyrightedMusicSendExtendedRequest_Can_not_find_instance",
                      "copyrightedMusicSendExtendedRequest `copyrightedMusicInitCopyrightedMusic` "
                      "but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSetScoringLevel(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto level = std::get<int32_t>(argument[FTValue("level")]);
        copyrightedMusic_->setScoringLevel(level);
        result->Success();
    } else {
        result->Error("copyrightedMusicSetScoringLevel_Can_not_find_instance",
                      "Invoke `copyrightedMusicSetScoringLevel` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStartScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto pitchValueInterval = std::get<int32_t>(argument[FTValue("pitchValueInterval")]);
        auto ret = copyrightedMusic_->startScore(resourceID, pitchValueInterval);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicStartScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStartScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStopScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->stopScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicStopScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStopScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetFullScore(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getFullScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetFullScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetFullScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetSharedResource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)std::get<int32_t>(
            configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        auto type =
            (EXPRESS::ZegoCopyrightedMusicResourceType)std::get<int32_t>(argument[FTValue("type")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getSharedResource(config, type,
                                             [=](int errorCode, std::string resource) {
                                                 FTMap retMap;
                                                 retMap[FTValue("errorCode")] = FTValue(errorCode);
                                                 retMap[FTValue("resource")] = FTValue(resource);
                                                 sharedPtrResult->Success(retMap);
                                             });
    } else {
        result->Error(
            "copyrightedMusicGetSharedResource_Can_not_find_instance",
            "Invoke `copyrightedMusicGetSharedResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetSharedResourceV2(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfigV2 config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.vendorID = std::get<int32_t>(configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.resourceType = std::get<int32_t>(configMap[FTValue("resourceType")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getSharedResource(config, 
                                             [=](int errorCode, std::string resource) {
                                                 FTMap retMap;
                                                 retMap[FTValue("errorCode")] = FTValue(errorCode);
                                                 retMap[FTValue("resource")] = FTValue(resource);
                                                 sharedPtrResult->Success(retMap);
                                             });
    } else {
        result->Error(
            "copyrightedMusicGetSharedResource_Can_not_find_instance",
            "Invoke `copyrightedMusicGetSharedResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestResource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)std::get<int32_t>(
            configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.masterID = std::get<std::string>(configMap[FTValue("masterID")]);
        config.sceneID = std::get<int32_t>(configMap[FTValue("sceneID")]);
        auto type =
            (EXPRESS::ZegoCopyrightedMusicResourceType)std::get<int32_t>(argument[FTValue("type")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestResource(config, type, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestResource_Can_not_find_instance",
                      "Invoke `copyrightedMusicRequestResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestResourceV2(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfigV2 config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode = std::get<int32_t>(configMap[FTValue("mode")]);
        config.vendorID = std::get<int32_t>(configMap[FTValue("vendorID")]);
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.masterID = std::get<std::string>(configMap[FTValue("masterID")]);
        config.sceneID = std::get<int32_t>(configMap[FTValue("sceneID")]);
        config.resourceType = std::get<int32_t>(configMap[FTValue("resourceType")]);
        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestResource(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestResource_Can_not_find_instance",
                      "Invoke `copyrightedMusicRequestResourceV2` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createTextureRenderer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto width = std::get<int32_t>(argument[FTValue("width")]);
    auto height = std::get<int32_t>(argument[FTValue("height")]);

    auto textureID = ZegoTextureRendererController::getInstance()->createTextureRenderer(
        registrar_->texture_registrar(), width, height);

    result->Success(FTValue(textureID));
}

void ZegoExpressEngineMethodHandler::destroyTextureRenderer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto textureID = argument[FTValue("textureID")].LongValue();
    bool state = ZegoTextureRendererController::getInstance()->destroyTextureRenderer(textureID);

    result->Success(FTValue(state));
}

void ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto bitrate = std::get<int32_t>(argument[FTValue("bitrate")]);
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoBitrateForTrafficControl(
        bitrate, (EXPRESS::ZegoTrafficControlMinVideoBitrateMode)mode,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto fps = std::get<int32_t>(argument[FTValue("fps")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoFpsForTrafficControl(
        fps, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto width = std::get<int32_t>(argument[FTValue("width")]);
    auto height = std::get<int32_t>(argument[FTValue("height")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoResolutionForTrafficControl(
        width, height, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setTrafficControlFocusOn(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setTrafficControlFocusOn(
        (EXPRESS::ZegoTrafficControlFocusOnMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::addPublishCdnUrl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto targetURL = std::get<std::string>(argument[FTValue("targetURL")]);
    auto timeout = std::get<int32_t>(argument[FTValue("timeout")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->addPublishCdnUrl(streamID, targetURL, timeout, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::removePublishCdnUrl(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto targetURL = std::get<std::string>(argument[FTValue("targetURL")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->removePublishCdnUrl(
        streamID, targetURL, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::enablePublishDirectToCDN(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCDNConfig config;
    if (configMap.size() > 0) {
        config.url = std::get<std::string>(configMap[FTValue("url")]);
        config.authParam = std::get<std::string>(configMap[FTValue("authParam")]);
        config.protocol = std::get<std::string>(configMap[FTValue("protocol")]);
        config.quicVersion = std::get<std::string>(configMap[FTValue("quicVersion")]);
        config.httpdns = (EXPRESS::ZegoHttpDNSType)std::get<int32_t>(configMap[FTValue("httpdns")]);
        config.quicConnectMode = (int)std::get<int32_t>(configMap[FTValue("quicConnectMode")]);
    }

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePublishDirectToCDN(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPublishWatermark(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto watermarkMap = std::get<FTMap>(argument[FTValue("watermark")]);
    EXPRESS::ZegoWatermark *watermark = nullptr;
    EXPRESS::ZegoWatermark watermarkTemp;
    if (watermarkMap.size() > 0) {
        watermarkTemp.imageURL = std::get<std::string>(watermarkMap[FTValue("imageURL")]);
        watermarkTemp.layout.x = std::get<int32_t>(watermarkMap[FTValue("left")]);
        watermarkTemp.layout.y = std::get<int32_t>(watermarkMap[FTValue("top")]);
        watermarkTemp.layout.width =
            std::get<int32_t>(watermarkMap[FTValue("right")]) - watermarkTemp.layout.x;
        watermarkTemp.layout.height =
            std::get<int32_t>(watermarkMap[FTValue("bottom")]) - watermarkTemp.layout.y;

        const std::string flutterAssertTaget = "flutter-asset://";
        if (watermarkTemp.imageURL.compare(0, flutterAssertTaget.size(), flutterAssertTaget) == 0) {
            watermarkTemp.imageURL.replace(0, flutterAssertTaget.size(), "");

            wchar_t exePath[MAX_PATH] = {0};
            ::GetModuleFileName(NULL, exePath, MAX_PATH);
            std::wstring exePathStrW{exePath};
            std::string exePathStr(exePathStrW.begin(), exePathStrW.end());
            exePathStr = std::string(exePathStr, 0, exePathStr.find_last_of("\\"));
            if (!exePathStr.empty()) {
                watermarkTemp.imageURL =
                    "file:///" + exePathStr + "\\data\\flutter_assets\\" + watermarkTemp.imageURL;
            } else {
                result->Error("setPublishWatermark_get_exe_path_fail",
                              "Failed to get the directory where the application is located");
                return;
            }
        }
        watermark = &watermarkTemp;
    }
    auto isPreviewVisible = std::get<bool>(argument[FTValue("isPreviewVisible")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPublishWatermark(watermark, isPreviewVisible,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamDecryptionKey(streamID, key);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamVideoType(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto streamType = std::get<int32_t>(argument[FTValue("streamType")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamVideoType(
        streamID, (EXPRESS::ZegoVideoStreamType)streamType);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto minBufferInterval = std::get<int32_t>(argument[FTValue("minBufferInterval")]);
    auto maxBufferInterval = std::get<int32_t>(argument[FTValue("maxBufferInterval")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamBufferIntervalRange(
        streamID, minBufferInterval, maxBufferInterval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamFocusOn(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamFocusOn(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamVideo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamVideo(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamVideo(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayVideoStreams(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayVideoStreams(mute);
    
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCheckPoc(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCheckPoc(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoDecoderSupported(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto codecID = std::get<int32_t>(argument[FTValue("codecID")]);
    if (codecID > 4) {
        codecID = 100;
    }

    int ret = 0;
    if (!argument[FTValue("codecBackend")].IsNull()) {
        auto codecBackend = std::get<int32_t>(argument[FTValue("codecBackend")]);
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoDecoderSupported(
            (EXPRESS::ZegoVideoCodecID)codecID, (EXPRESS::ZegoVideoCodecBackend)codecBackend);
    } else {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoDecoderSupported(
                  (EXPRESS::ZegoVideoCodecID)codecID)
                  ? 1
                  : 0;
    }

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setPlayStreamsAlignmentProperty(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamsAlignmentProperty(
        (EXPRESS::ZegoStreamAlignmentMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVideoSuperResolution(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    // auto enable = std::get<bool>(argument[FTValue("enable")]);
    // auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    // EXPRESS::ZegoExpressSDK::getEngine()->enableVideoSuperResolution(streamID, enable);

    // result->Success();

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::initVideoSuperResolution(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::uninitVideoSuperResolution(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::updatePlayingCanvas(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    flutter::EncodableMap canvasMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("canvas")])) {
        canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    bool isSuccess = false;
    int64_t viewID = 0;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto alphaBlend = std::get<bool>(canvasMap[FTValue("alphaBlend")]);

        viewID = canvasMap[FTValue("view")].LongValue();
        ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
        isSuccess = ZegoTextureRendererController::getInstance()->addRemoteRenderer(
            viewID, streamID, viewMode);

        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }

    if (isSuccess) {
        result->Success(FTValue(0));
    } else {
        std::ostringstream oss;
        oss << "The TextureRenderer for textureID: " << viewID
            << "cannot be found, developer should call `createCanvasView` first and get the "
               "textureID";
        result->Error("updatePlayingCanvas_No_TextureRenderer", oss.str());
    }
}

void ZegoExpressEngineMethodHandler::startAutoMixerTask(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);
    task.roomID = std::get<std::string>(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = std::get<FTArray>(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = std::get<FTMap>(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = std::get<std::string>(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = std::get<FTMap>(outputMap[FTValue("videoConfig")]);

                auto codecIDIndex = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)std::get<int32_t>(
                        videoConfigMap[FTValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD = std::get<bool>(
                    videoConfigMap[FTValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }
    // AudioConfig
    auto audioConfigMap = std::get<FTMap>(argument[FTValue("audioConfig")]);
    if (audioConfigMap.size() > 0) {
        task.audioConfig.bitrate = std::get<int32_t>(audioConfigMap[FTValue("bitrate")]);
        task.audioConfig.channel =
            (EXPRESS::ZegoAudioChannel)std::get<int32_t>(audioConfigMap[FTValue("channel")]);
        task.audioConfig.codecID =
            (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(audioConfigMap[FTValue("codecID")]);
    }
    // Enable SoundLevel
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);
    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)std::get<int32_t>(
        argument[FTValue("streamAlignmentMode")]);
    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        std::get<int32_t>(argument[FTValue("minPlayStreamBufferLength")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startAutoMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("extendedData")] = FTValue(extendedData);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopAutoMixerTask(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);
    task.roomID = std::get<std::string>(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = std::get<FTArray>(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = std::get<FTMap>(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = std::get<std::string>(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = std::get<FTMap>(outputMap[FTValue("videoConfig")]);

                auto codecIDIndex = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)std::get<int32_t>(
                        videoConfigMap[FTValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD = std::get<bool>(
                    videoConfigMap[FTValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }

    // no need to set audio config
    // no need to set video config
    // no need to set enable sound level

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->stopAutoMixerTask(task, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioRouteType(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::useFrontCamera(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::isCameraFocusSupported(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureCompensation(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraZoomFactor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioSpectrumMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioSpectrumMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneAEC(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startEffectsEnv(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->startEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopEffectsEnv(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableEffectsBeauty(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableEffectsBeauty(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setEffectsBeautyParam(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoEffectsBeautyParam param;
    param.rosyIntensity = std::get<int32_t>(paramMap[FTValue("rosyIntensity")]);
    param.sharpenIntensity = std::get<int32_t>(paramMap[FTValue("sharpenIntensity")]);
    param.smoothIntensity = std::get<int32_t>(paramMap[FTValue("smoothIntensity")]);
    param.whitenIntensity = std::get<int32_t>(paramMap[FTValue("whitenIntensity")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setEffectsBeautyParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableBeautify(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->Error("Not recommended", "Not recommended, please use startEffectsEnv instead");
}

void ZegoExpressEngineMethodHandler::setBeautifyOption(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->Error("Not recommended",
                  "Not recommended, please use startEffectsEnv and setEffectsBeautyParam instead");
}

void ZegoExpressEngineMethodHandler::sendBroadcastMessage(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto message = std::get<std::string>(argument[FTValue("message")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendBroadcastMessage(
        roomID, message, [=](int errorCode, unsigned long long messageID) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("messageID")] = FTValue((int64_t)messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendBarrageMessage(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto message = std::get<std::string>(argument[FTValue("message")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendBarrageMessage(
        roomID, message, [=](int errorCode, std::string messageID) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("messageID")] = FTValue(messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendCustomCommand(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto command = std::get<std::string>(argument[FTValue("command")]);
    auto toUserListArray = std::get<FTArray>(argument[FTValue("toUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = std::get<FTMap>(user_);
        user.userID = std::get<std::string>(userMap[FTValue("userID")]);
        user.userName = std::get<std::string>(userMap[FTValue("userName")]);
        toUserList.push_back(user);
    }

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomCommand(
        roomID, command, toUserList, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}


void ZegoExpressEngineMethodHandler::sendTransparentMessage(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto sendMode = std::get<std::int32_t>(argument[FTValue("sendMode")]);
    auto sendType = std::get<std::int32_t>(argument[FTValue("sendType")]);
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("content")]);

    auto toUserListArray = std::get<FTArray>(argument[FTValue("recvUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = std::get<FTMap>(user_);
        user.userID = std::get<std::string>(userMap[FTValue("userID")]);
        user.userName = std::get<std::string>(userMap[FTValue("userName")]);
        toUserList.push_back(user);
    }

    auto timeout = std::get<std::int32_t>(argument[FTValue("timeOut")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

    ZEGO::EXPRESS::ZegoRoomSendTransparentMessage message;
    message.sendMode = (ZEGO::EXPRESS::ZegoRoomTransparentMessageMode)sendMode;
    message.sendType = (ZEGO::EXPRESS::ZegoRoomTransparentMessageType)sendType;
    message.content.assign((const char*)byteData.data(), (unsigned int)byteData.size());
    message.recvUserList = std::move(toUserList);
    message.timeOut = timeout;

    EXPRESS::ZegoExpressSDK::getEngine()->sendTransparentMessage(
        roomID, message, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}


void ZegoExpressEngineMethodHandler::enableCustomVideoCapture(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoCaptureConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)std::get<int32_t>(configMap[FTValue("bufferType")]);
    }

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    if (enable) {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureHandler(
            ZegoCustomVideoCaptureManager::getInstance()->getHandler());
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureHandler(nullptr);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoCapture(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomVideoProcessing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoProcessConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)std::get<int32_t>(configMap[FTValue("bufferType")]);
    }

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    if (enable) {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoProcessHandler(
            ZegoCustomVideoProcessManager::getInstance()->getHandler());
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoProcessHandler(nullptr);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoProcessing(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomVideoRender(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoRenderConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)std::get<int32_t>(configMap[FTValue("bufferType")]);
        config.frameFormatSeries = (EXPRESS::ZegoVideoFrameFormatSeries)std::get<int32_t>(
            configMap[FTValue("frameFormatSeries")]);
        config.enableEngineRender = std::get<bool>(configMap[FTValue("enableEngineRender")]);
    }

    if (enable) {
        if (config.enableEngineRender) {
            config.bufferType = EXPRESS::ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
            config.frameFormatSeries = EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
            EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);

            EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(
                ZegoTextureRendererController::getInstance());
            ZegoTextureRendererController::getInstance()->setCustomVideoRenderHandler(
                ZegoCustomVideoRenderManager::getInstance()->getHandler());
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);
            EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(
                ZegoCustomVideoRenderManager::getInstance()->getHandler());
        }
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(false, &config);
        ZegoTextureRendererController::getInstance()->setCustomVideoRenderHandler(nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlignedAudioAuxData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioRemoteProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioPlaybackProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioIO(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioConfig config;
    config.sourceType =
        (EXPRESS::ZegoAudioSourceType)std::get<int32_t>(configMap[FTValue("sourceType")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioIO(enable, &config,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto configLength = argument[FTValue("configLength")].LongValue();
    auto referenceTimeMillisecond = argument[FTValue("configLength")].LongValue();
    auto samples = argument[FTValue("samples")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCaptureAACData(
        byteData.data(), dataLength, configLength, referenceTimeMillisecond, samples, param,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCapturePCMData(
        byteData.data(), dataLength, param, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);

    EXPRESS::ZegoExpressSDK::getEngine()->fetchCustomAudioRenderPCMData(byteData.data(), dataLength,
                                                                        param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPerformanceMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startPerformanceMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPerformanceMonitor(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopPerformanceMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkProbe(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkProbeConfig config;
    config.enableTraceroute = std::get<bool>(configMap[FTValue("enableTraceroute")]);

    auto sharedPtrResult =
        std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkProbe(
        config, [=](int errorCode, const EXPRESS::ZegoNetworkProbeResult &result) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            if (result.httpProbeResult) {
                FTMap httpProbeResultMap;
                httpProbeResultMap[FTValue("errorCode")] =
                    FTValue(result.httpProbeResult->errorCode);
                httpProbeResultMap[FTValue("requestCostTime")] =
                    FTValue((int64_t)result.httpProbeResult->requestCostTime);
                retMap[FTValue("httpProbeResult")] = FTValue(httpProbeResultMap);
            }
            if (result.tcpProbeResult) {
                FTMap tcpProbeResultMap;
                tcpProbeResultMap[FTValue("errorCode")] = FTValue(result.tcpProbeResult->errorCode);
                tcpProbeResultMap[FTValue("rtt")] = FTValue((int64_t)result.tcpProbeResult->rtt);
                tcpProbeResultMap[FTValue("connectCostTime")] =
                    FTValue((int64_t)result.tcpProbeResult->connectCostTime);
                retMap[FTValue("tcpProbeResult")] = FTValue(tcpProbeResultMap);
            }
            if (result.udpProbeResult) {
                FTMap udpProbeResultMap;
                udpProbeResultMap[FTValue("errorCode")] = FTValue(result.udpProbeResult->errorCode);
                udpProbeResultMap[FTValue("rtt")] = FTValue((int64_t)result.udpProbeResult->rtt);
                retMap[FTValue("udpProbeResult")] = FTValue(udpProbeResultMap);
            }
            if (result.tracerouteResult) {
                FTMap tracerouteResultMap;
                tracerouteResultMap[FTValue("errorCode")] =
                    FTValue(result.tracerouteResult->errorCode);
                tracerouteResultMap[FTValue("tracerouteCostTime")] =
                    FTValue((int64_t)result.tracerouteResult->tracerouteCostTime);
                retMap[FTValue("tracerouteResult")] = FTValue(tracerouteResultMap);
            }
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopNetworkProbe(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkProbe();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkSpeedTest(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkSpeedTestConfig config;
    config.testUplink = std::get<bool>(configMap[FTValue("testUplink")]);
    config.expectedUplinkBitrate = std::get<int32_t>(configMap[FTValue("expectedUplinkBitrate")]);
    config.testDownlink = std::get<bool>(configMap[FTValue("testDownlink")]);
    config.expectedDownlinkBitrate =
        std::get<int32_t>(configMap[FTValue("expectedDownlinkBitrate")]);
    auto interval = argument[FTValue("interval")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkSpeedTest(config, interval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopNetworkSpeedTest(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkSpeedTest();

    result->Success();
}

void ZegoExpressEngineMethodHandler::getNetworkTimeInfo(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoNetworkTimeInfo info = EXPRESS::ZegoExpressSDK::getEngine()->getNetworkTimeInfo();

    flutter::EncodableMap retMap;
    retMap[FTValue("timestamp")] = FTValue((int64_t)info.timestamp);
    retMap[FTValue("maxDeviation")] = FTValue((int32_t)info.maxDeviation);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::startDumpData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoDumpDataConfig config;
        config.dataType = (EXPRESS::ZegoDumpDataType)std::get<int32_t>(configMap[FTValue("dataType")]);

        EXPRESS::ZegoExpressSDK::getEngine()->startDumpData(config);

        result->Success();
    }

void ZegoExpressEngineMethodHandler::stopDumpData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        
        EXPRESS::ZegoExpressSDK::getEngine()->stopDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::uploadDumpData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

        EXPRESS::ZegoExpressSDK::getEngine()->uploadDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::removeDumpData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

        EXPRESS::ZegoExpressSDK::getEngine()->removeDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::createRangeAudio(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    rangeAudio_ = EXPRESS::ZegoExpressSDK::getEngine()->createRangeAudio();
    if (rangeAudio_) {
        rangeAudio_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(FTValue(0));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyRangeAudio(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    EXPRESS::ZegoExpressSDK::getEngine()->destroyRangeAudio(rangeAudio_);
    rangeAudio_ = nullptr;
    result->Success();
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);

        rangeAudio_->enableMicrophone(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableMicrophone_Can_not_find_instance",
                      "Invoke `rangeAudioEnableMicrophone` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        rangeAudio_->enableSpatializer(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpatializer_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpatializer` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);

        rangeAudio_->enableSpeaker(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpeaker_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpeaker` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto paramMap = std::get<FTMap>(argument[FTValue("param")]);

        EXPRESS::ZegoReceiveRangeParam param;
        param.min = (float)std::get<double>(paramMap[FTValue("min")]);
        param.max = (float)std::get<double>(paramMap[FTValue("max")]);

        int ret = rangeAudio_->setAudioReceiveRange(param);

        result->Success(FTValue(ret));
    } else {
        result->Error("rangeAudioSetAudioReceiveRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto mode = std::get<int32_t>(argument[FTValue("mode")]);

        rangeAudio_->setRangeAudioMode((EXPRESS::ZegoRangeAudioMode)mode);

        result->Success();
    } else {
        result->Error("rangeAudioSetMode_Can_not_find_instance",
                      "Invoke `rangeAudioSetMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetTeamID(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto teamID = std::get<std::string>(argument[FTValue("teamID")]);

        rangeAudio_->setTeamID(teamID);

        result->Success();
    } else {
        result->Error("rangeAudioSetTeamID_Can_not_find_instance",
                      "Invoke `rangeAudioSetTeamID` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto userID = std::get<std::string>(argument[FTValue("userID")]);
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);

        rangeAudio_->updateAudioSource(userID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);
        auto axisForward = std::get<std::vector<float>>(argument[FTValue("axisForward")]);
        auto axisRight = std::get<std::vector<float>>(argument[FTValue("axisRight")]);
        auto axisUp = std::get<std::vector<float>>(argument[FTValue("axisUp")]);

        rangeAudio_->updateSelfPosition(position.data(), axisForward.data(), axisRight.data(),
                                        axisUp.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioMuteUser(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto userID = std::get<std::string>(argument[FTValue("userID")]);
        auto mute = std::get<bool>(argument[FTValue("mute")]);

        rangeAudio_->muteUser(userID, mute);

        result->Success();
    } else {
        result->Error("rangeAudioMuteUser_Can_not_find_instance",
                      "Invoke `rangeAudioMuteUser` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto frequency = std::get<int32_t>(argument[FTValue("frequency")]);

        rangeAudio_->setPositionUpdateFrequency(frequency);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetPositionUpdateFrequency_Can_not_find_instance",
            "Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioVolume(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto volume = std::get<int32_t>(argument[FTValue("volume")]);

        rangeAudio_->setRangeAudioVolume(volume);

        result->Success();
    } else {
        result->Error("rangeAudioSetRangeAudioVolume_Can_not_find_instance",
                      "Invoke `rangeAudioSetRangeAudioVolume` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetStreamVocalRange(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
        auto paramMap = std::get<FTMap>(argument[FTValue("param")]);

        EXPRESS::ZegoVocalRangeParam param;
        param.min = (float)std::get<double>(paramMap[FTValue("min")]);
        param.max = (float)std::get<double>(paramMap[FTValue("max")]);

        int ret = rangeAudio_->setStreamVocalRange(streamID, param);

        result->Success(FTValue(ret));
    } else {
        result->Error("rangeAudioSetStreamVocalRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateStreamPosition(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);

        rangeAudio_->updateStreamPosition(streamID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateStreamPosition_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateStreamPosition` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioCustomMode(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (rangeAudio_) {
        auto speakMode =
            (EXPRESS::ZegoRangeAudioSpeakMode)std::get<int>(argument[FTValue("speakMode")]);

        auto listenMode =
            (EXPRESS::ZegoRangeAudioListenMode)std::get<int>(argument[FTValue("listenMode")]);

        rangeAudio_->setRangeAudioCustomMode(speakMode, listenMode);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetRangeAudioCustomMode_Can_not_find_instance",
            "Invoke `rangeAudioSetRangeAudioCustomMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

    auto dataManager =
        EXPRESS::ZegoExpressSDK::getEngine()->createRealTimeSequentialDataManager(roomID);
    if (dataManager) {
        dataManager->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        dataManagerMap_[dataManager->getIndex()] = dataManager;
        result->Success(FTValue(dataManager->getIndex()));
    } else {
        result->Success(-1);
    }
}

void ZegoExpressEngineMethodHandler::destroyRealTimeSequentialDataManager(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyRealTimeSequentialDataManager(
            dataManagerMap_[index]);
        dataManagerMap_.erase(index);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        auto sharedPtrResult =
            std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        dataManagerMap_[index]->sendRealTimeSequentialData(
            byteData.data(), byteData.size(), streamID, [=](int errorCode) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error(
            "dataManagerSendRealTimeSequentialData_Can_not_find_instance",
            "Invoke `dataManagerSendRealTimeSequentialData` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartBroadcasting(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStartBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartSubscribing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStartSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStopBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopSubscribing(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStopSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::getScreenCaptureSources(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto thumbnailWidth = std::get<int32_t>(argument[FTValue("thumbnailWidth")]);
    auto thumbnailHeight = std::get<int32_t>(argument[FTValue("thumbnailHeight")]);
    auto iconWidth = std::get<int32_t>(argument[FTValue("iconWidth")]);
    auto iconHeight = std::get<int32_t>(argument[FTValue("iconHeight")]);

    auto infos = EXPRESS::ZegoExpressSDK::getEngine()->getScreenCaptureSources(
        thumbnailWidth, thumbnailHeight, iconWidth, iconHeight);
    FTArray resultArray;
    for (auto &info : infos) {
        FTMap infoMap;
        infoMap[FTValue("sourceType")] = FTValue(static_cast<int32_t>(info.sourceType));
        infoMap[FTValue("sourceID")] = FTValue(reinterpret_cast<intptr_t>(info.sourceID));
        infoMap[FTValue("sourceName")] = FTValue(info.sourceName);

        if (info.thumbnailImage.length <= 0 || info.thumbnailImage.buffer == nullptr) {
            infoMap[FTValue("thumbnailImage")] = FTValue(std::monostate());
        } else {
            auto thumbnailImageData = makeBtimapByBGRABuffer(
                info.thumbnailImage.buffer, info.thumbnailImage.length,
                std::pair(info.thumbnailImage.width, info.thumbnailImage.height));
            std::vector<uint8_t> thumbnailImage(
                thumbnailImageData.second, thumbnailImageData.second + thumbnailImageData.first);
            delete[] thumbnailImageData.second;
            infoMap[FTValue("thumbnailImage")] = FTValue(thumbnailImage);
        }

        if (info.iconImage.length <= 0 || info.iconImage.buffer == nullptr) {
            infoMap[FTValue("iconImage")] = FTValue(std::monostate());
        } else {
            auto iconImageData =
                makeIconByBGRABuffer(info.iconImage.buffer, info.iconImage.length,
                                     std::pair(info.iconImage.width, info.iconImage.height));
            std::vector<uint8_t> iconImage(iconImageData.second,
                                           iconImageData.second + iconImageData.first);
            delete[] iconImageData.second;
            infoMap[FTValue("iconImage")] = FTValue(iconImage);
        }

        resultArray.emplace_back(FTValue(infoMap));
    }

    result->Success(resultArray);
}

void ZegoExpressEngineMethodHandler::createScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    void *sourceId =
        reinterpret_cast<void *>(static_cast<intptr_t>(argument[FTValue("sourceId")].LongValue()));
    auto sourceType = std::get<int32_t>(argument[FTValue("sourceType")]);
    auto screenCaptureSource = EXPRESS::ZegoExpressSDK::getEngine()->createScreenCaptureSource(
        sourceId, (EXPRESS::ZegoScreenCaptureSourceType)sourceType);
    if (screenCaptureSource) {
        auto index = screenCaptureSource->getIndex();

        screenCaptureSource->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        screenCaptureSourceMap_[index] = screenCaptureSource;

        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyScreenCaptureSource(screenCaptureSource);
    }

    screenCaptureSourceMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCursorVisibleScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto visible = std::get<bool>(argument[FTValue("visible")]);
        screenCaptureSource->enableCursorVisible(visible);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableWindowActivateScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto active = std::get<bool>(argument[FTValue("active")]);
        screenCaptureSource->enableWindowActivate(active);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setExcludeWindowListScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto list = std::get<FTArray>(argument[FTValue("list")]);
        intptr_t *c_list = new intptr_t[list.size()];
        intptr_t *tmpPtr = c_list;
        for (auto win : list) {
            *tmpPtr = static_cast<intptr_t>(win.LongValue());
            tmpPtr += 1;
        }

        screenCaptureSource->setExcludeWindowList((void **)c_list, list.size());
        delete[] c_list;
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::updateCaptureRegionScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = std::get<FTMap>(argument[FTValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = std::get<double>(rectMap[FTValue("x")]);
        rect.y = std::get<double>(rectMap[FTValue("y")]);
        rect.width = std::get<double>(rectMap[FTValue("width")]);
        rect.height = std::get<double>(rectMap[FTValue("height")]);
        screenCaptureSource->updateCaptureRegion(rect);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::updatePublishRegionScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = std::get<FTMap>(argument[FTValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = std::get<double>(rectMap[FTValue("x")]);
        rect.y = std::get<double>(rectMap[FTValue("y")]);
        rect.width = std::get<double>(rectMap[FTValue("width")]);
        rect.height = std::get<double>(rectMap[FTValue("height")]);
        screenCaptureSource->updatePublishRegion(rect);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::updateCaptureSourceScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        void *sourceId = reinterpret_cast<void *>(
            static_cast<intptr_t>(argument[FTValue("sourceId")].LongValue()));
        auto sourceType = std::get<int32_t>(argument[FTValue("sourceType")]);
        screenCaptureSource->updateCaptureSource(sourceId,
                                                 (EXPRESS::ZegoScreenCaptureSourceType)sourceType);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::startCaptureScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->startCapture();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopCaptureScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->stopCapture();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::getCaptureSourceRectScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rect = screenCaptureSource->getCaptureSourceRect();
        FTMap resultMap;
        resultMap[FTValue("x")] = FTValue((int32_t)rect.x);
        resultMap[FTValue("y")] = FTValue((int32_t)rect.y);
        resultMap[FTValue("width")] = FTValue((int32_t)rect.width);
        resultMap[FTValue("height")] = FTValue((int32_t)rect.height);
        result->Success(resultMap);
    }

    result->Success();
}


void ZegoExpressEngineMethodHandler::enableAudioCaptureScreenCaptureSource(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto param = std::get<FTMap>(argument[FTValue("audioParam")]);

        EXPRESS::ZegoAudioFrameParam nativeParam;
        nativeParam.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(param[FTValue("sampleRate")]);
        nativeParam.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(param[FTValue("channel")]);

        screenCaptureSource->enableAudioCapture(enable, nativeParam);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAIVoiceChanger(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto aiVoiceChanger = EXPRESS::ZegoExpressSDK::getEngine()->createAIVoiceChanger();
    if (aiVoiceChanger) {
        auto index = aiVoiceChanger->getIndex();

        aiVoiceChanger->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        aiVoiceChangerMap_[index] = aiVoiceChanger;

        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}


void ZegoExpressEngineMethodHandler::destroyAIVoiceChanger(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->setEventHandler(nullptr);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyAIVoiceChanger(aiVoiceChanger);
    }

    aiVoiceChangerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerInitEngine(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->initEngine();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerUpdate(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->update();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerSetSpeaker(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        auto speakerID = std::get<int32_t>(argument[FTValue("speakerID")]);
        aiVoiceChanger->setSpeaker(speakerID);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerGetSpeakerList(
    flutter::EncodableMap &argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->getSpeakerList();
    }

    result->Success();
}
void ZegoExpressEngineMethodHandler::enableColorEnhancement(flutter::EncodableMap &argument,
                           std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result){
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto params = std::get<FTMap>(argument[FTValue("params")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoColorEnhancementParams p;
    p.intensity = std::get<double>(params[FTValue("intensity")]);
    p.skinToneProtectionLevel = std::get<double>(params[FTValue("skinToneProtectionLevel")]);
    p.lipColorProtectionLevel = std::get<double>(params[FTValue("lipColorProtectionLevel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableColorEnhancement(enable, p, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}
