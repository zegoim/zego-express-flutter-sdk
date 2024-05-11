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
    FTArgument &argument,
    FTResult result) {
    result->Success(EXPRESS::ZegoExpressSDK::getVersion());
}

void ZegoExpressEngineMethodHandler::isFeatureSupported(
    FTArgument &argument,
    FTResult result) {
    auto featureType = zego_value_get_int(argument[FTValue("featureType")]);
    result->Success(
        EXPRESS::ZegoExpressSDK::isFeatureSupported((EXPRESS::ZegoFeatureType)featureType));
}

void ZegoExpressEngineMethodHandler::setPluginVersion(
    FTArgument &argument,
    FTResult result) {
    std::string version = zego_value_get_string(argument[FTValue("version")]);

    version = "*** Plugin Version: " + version;

    ZF::logInfo(version.c_str());

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAssetAbsolutePath(
    FTArgument &argument,
    FTResult result) {
    std::string assetPath = zego_value_get_string(argument[FTValue("assetPath")]);
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
    FTArgument &argument,
    FTResult result) {
    initApiCalledCallback();

    EXPRESS::ZegoEngineConfig config;
    config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                             {"thirdparty_framework_info", "flutter"}};
    EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    // TODO: need to write getValue utils
    unsigned int appID = argument[FTValue("appID")].LongValue();
    std::string appSign = zego_value_get_string(argument[FTValue("appSign")]);
    bool isTestEnv = zego_value_get_bool(argument[FTValue("isTestEnv")]);
    int scenario = zego_value_get_int(argument[FTValue("scenario")]);

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
    FTArgument &argument,
    FTResult result) {
    FTMap profileMap = zego_value_get_map(argument[FTValue("profile")]);
    if (profileMap.size() > 0) {

        initApiCalledCallback();

        EXPRESS::ZegoEngineConfig config;
        config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                                 {"thirdparty_framework_info", "flutter"}};
        EXPRESS::ZegoExpressSDK::setEngineConfig(config);

        EXPRESS::ZegoEngineProfile profilePtr;
        profilePtr.appID = profileMap[FTValue("appID")].LongValue();
        if (!profileMap[FTValue("appSign")].IsNull()) {
            profilePtr.appSign = zego_value_get_string(profileMap[FTValue("appSign")]);
        }
        profilePtr.scenario =
            (EXPRESS::ZegoScenario)zego_value_get_int(profileMap[FTValue("scenario")]);
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
    FTArgument &argument,
    FTResult result) {
    auto engine = EXPRESS::ZegoExpressSDK::getEngine();

    if (engine) {
        ZegoTextureRendererController::getInstance()->uninit();
        auto sharedPtrResult =
            FTMoveResult(result);
        EXPRESS::ZegoExpressSDK::destroyEngine(engine, [=]() { sharedPtrResult->Success(); });
    } else {
        result->Success();
    }
}

void ZegoExpressEngineMethodHandler::setEngineConfig(
    FTArgument &argument,
    FTResult result) {
    FTMap configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoEngineConfig config;

    if (configMap.size() > 0) {
        FTMap logConfigMap = zego_value_get_map(configMap[FTValue("logConfig")]);
        std::unique_ptr<EXPRESS::ZegoLogConfig> logConfigPtr = nullptr;
        if (logConfigMap.size() > 0) {
            logConfigPtr = std::make_unique<EXPRESS::ZegoLogConfig>();
            logConfigPtr->logPath = zego_value_get_string(logConfigMap[FTValue("logPath")]);
            logConfigPtr->logSize  = zego_value_get_int(logConfigMap[FTValue("logSize")]);
            logConfigPtr->logCount = zego_value_get_int(logConfigMap[FTValue("logCount")]);
        }

        config.logConfig = logConfigPtr.get();

        FTMap advancedConfigMap = zego_value_get_map(configMap[FTValue("advancedConfig")]);
        for (auto &cfg : advancedConfigMap) {
            std::string key = zego_value_get_string(cfg.first);
            std::string value = zego_value_get_string(cfg.second);
            config.advancedConfig[key] = value;
        }

        EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLogConfig(
    FTArgument &argument,
    FTResult result) {
    FTMap configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoLogConfig config;
    if (configMap.size() > 0) {
        config.logPath = zego_value_get_string(configMap[FTValue("logPath")]);
        config.logSize  = zego_value_get_int(configMap[FTValue("logSize")]);
        config.logCount = zego_value_get_int(configMap[FTValue("logCount")]);
        EXPRESS::ZegoExpressSDK::setLogConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLocalProxyConfig(
    FTArgument &argument,
    FTResult result) {
    auto proxyListArray = zego_value_get_list(argument[FTValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = zego_value_get_map(proxy_);
        proxy.ip = zego_value_get_string(proxyMap[FTValue("ip")]);
        proxy.port = zego_value_get_int(proxyMap[FTValue("port")]);
        proxy.hostName = zego_value_get_string(proxyMap[FTValue("hostName")]);
        proxy.userName = zego_value_get_string(proxyMap[FTValue("userName")]);
        proxy.password = zego_value_get_string(proxyMap[FTValue("password")]);
        proxyList.push_back(proxy);
    }
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::setLocalProxyConfig(proxyList, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCloudProxyConfig(
    FTArgument &argument,
    FTResult result) {
    auto proxyListArray = zego_value_get_list(argument[FTValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = zego_value_get_map(proxy_);
        proxy.ip = zego_value_get_string(proxyMap[FTValue("ip")]);
        proxy.port = zego_value_get_int(proxyMap[FTValue("port")]);
        proxy.hostName = zego_value_get_string(proxyMap[FTValue("hostName")]);
        proxy.userName = zego_value_get_string(proxyMap[FTValue("userName")]);
        proxy.password = zego_value_get_string(proxyMap[FTValue("password")]);
        proxyList.push_back(proxy);
    }
    auto token = zego_value_get_string(argument[FTValue("token")]);

    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::setCloudProxyConfig(proxyList, token, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomMode(
    FTArgument &argument,
    FTResult result) {
    auto mode = (EXPRESS::ZegoRoomMode)zego_value_get_int(argument[FTValue("mode")]);
    EXPRESS::ZegoExpressSDK::setRoomMode(mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLicense(
    FTArgument &argument,
    FTResult result) {
    auto license = zego_value_get_string(argument[FTValue("license")]);
    EXPRESS::ZegoExpressSDK::setLicense(license);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setGeoFence(
    FTArgument &argument,
    FTResult result) {
    auto type = (EXPRESS::ZegoGeoFenceType)zego_value_get_int(argument[FTValue("type")]);
    auto areaList_ = zego_value_get_list(argument[FTValue("areaList")]);
    std::vector<int> areaList;
    for (auto area : areaList_) {
        areaList.push_back(zego_value_get_int(area));
    }
    EXPRESS::ZegoExpressSDK::setGeoFence(type, areaList);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomScenario(
    FTArgument &argument,
    FTResult result) {
    auto scenario = (EXPRESS::ZegoScenario)zego_value_get_int(argument[FTValue("scenario")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomScenario(scenario);

    result->Success();
}

void ZegoExpressEngineMethodHandler::uploadLog(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->uploadLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::submitLog(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::submitLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableDebugAssistant(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableDebugAssistant(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::callExperimentalAPI(
    FTArgument &argument,
    FTResult result) {
    auto params = zego_value_get_string(argument[FTValue("params")]);
    auto resultStr = EXPRESS::ZegoExpressSDK::getEngine()->callExperimentalAPI(params);
    result->Success(resultStr);
}

void ZegoExpressEngineMethodHandler::setDummyCaptureImagePath(
    FTArgument &argument,
    FTResult result) {
    auto filePath = zego_value_get_string(argument[FTValue("filePath")]);
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
    auto channel = (EXPRESS::ZegoPublishChannel)zego_value_get_int(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setDummyCaptureImagePath(filePath, channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::loginRoom(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto userMap = zego_value_get_map(argument[FTValue("user")]);

    EXPRESS::ZegoUser user{zego_value_get_string(userMap[FTValue("userID")]),
                           zego_value_get_string(userMap[FTValue("userName")])};

    FTMap configMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("config")])) {
        configMap = zego_value_get_map(argument[FTValue("config")]);
    }

    EXPRESS::ZegoRoomConfig config;
    if (configMap.size() > 0) {
        config.maxMemberCount =
            (unsigned int)zego_value_get_int(configMap[FTValue("maxMemberCount")]);
        config.isUserStatusNotify = zego_value_get_bool(configMap[FTValue("isUserStatusNotify")]);
        config.token = zego_value_get_string(configMap[FTValue("token")]);
    }
    auto sharedPtrResult =
        FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto sharedPtrResult =
        FTMoveResult(result);
    if (!argument[FTValue("roomID")].IsNull()) {
        auto roomID = zego_value_get_string(argument[FTValue("roomID")]);

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
    FTArgument &argument,
    FTResult result) {
    auto fromRoomID = zego_value_get_string(argument[FTValue("fromRoomID")]);
    auto toRoomID = zego_value_get_string(argument[FTValue("toRoomID")]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (std::holds_alternative<FTMap>(argument[FTValue("config")])) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = configMap[FTValue("maxMemberCount")].LongValue();
            configPtr->isUserStatusNotify =
                zego_value_get_bool(configMap[FTValue("isUserStatusNotify")]);
            configPtr->token = zego_value_get_string(configMap[FTValue("token")]);
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());

    result->Success();
}

void ZegoExpressEngineMethodHandler::renewToken(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto token = zego_value_get_string(argument[FTValue("token")]);

    EXPRESS::ZegoExpressSDK::getEngine()->renewToken(roomID, token);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto key = zego_value_get_string(argument[FTValue("key")]);
    auto value = zego_value_get_string(argument[FTValue("value")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::getRoomStreamList(
    FTArgument &argument,
    FTResult result) {

    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);

    auto stream_list_type = zego_value_get_int(argument[FTValue("streamListType")]);

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
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    if (std::holds_alternative<FTMap>(argument[FTValue("config")])) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            EXPRESS::ZegoPublisherConfig config;

            config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
            config.forceSynchronousNetworkTime =
                zego_value_get_int(configMap[FTValue("forceSynchronousNetworkTime")]);
            config.streamCensorshipMode = (EXPRESS::ZegoStreamCensorshipMode)zego_value_get_int(
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
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream(
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(
    FTArgument &argument,
    FTResult result) {
    auto extraInfo = zego_value_get_string(argument[FTValue("extraInfo")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    auto sharedPtrResult =
        FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    FTMap canvasMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        auto alphaBlend = zego_value_get_bool(canvasMap[FTValue("alphaBlend")]);
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
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);
    ZegoTextureRendererController::getInstance()->removeCapturedRenderer(
        (EXPRESS::ZegoPublishChannel)channel);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoConfig(
    FTArgument &argument,
    FTResult result) {
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoVideoConfig config;
    config.bitrate = zego_value_get_int(configMap[FTValue("bitrate")]);
    config.captureHeight = zego_value_get_int(configMap[FTValue("captureHeight")]);
    config.captureWidth = zego_value_get_int(configMap[FTValue("captureWidth")]);
    auto codecIDIndex = zego_value_get_int(configMap[FTValue("codecID")]);
    config.codecID = (EXPRESS::ZegoVideoCodecID)(
        codecIDIndex > 4 ? EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN : codecIDIndex);
    config.encodeHeight = zego_value_get_int(configMap[FTValue("encodeHeight")]);
    config.encodeWidth = zego_value_get_int(configMap[FTValue("encodeWidth")]);
    config.fps = zego_value_get_int(configMap[FTValue("fps")]);
    config.keyFrameInterval = zego_value_get_int(configMap[FTValue("keyFrameInterval")]);

    auto channel = zego_value_get_int(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setVideoConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoConfig(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);
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
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    std::vector<EXPRESS::ZegoPublishDualStreamConfig> configList;
    auto configListMap = zego_value_get_list(argument[FTValue("configList")]);
    for (auto config_ : configListMap) {
        FTMap configMap = zego_value_get_map(config_);
        EXPRESS::ZegoPublishDualStreamConfig config;

        config.encodeWidth = zego_value_get_int(configMap[FTValue("encodeWidth")]);
        config.encodeHeight = zego_value_get_int(configMap[FTValue("encodeHeight")]);
        config.fps = zego_value_get_int(configMap[FTValue("fps")]);
        config.bitrate = zego_value_get_int(configMap[FTValue("bitrate")]);
        config.streamType = (EXPRESS::ZegoVideoStreamType) zego_value_get_int(configMap[FTValue("streamType")]);

        configList.push_back(config);
    }
    
    EXPRESS::ZegoExpressSDK::getEngine()->setPublishDualStreamConfig(configList, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoMirrorMode(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    auto mirrorMode =
        (EXPRESS::ZegoVideoMirrorMode)zego_value_get_int(argument[FTValue("mirrorMode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVideoMirrorMode(mirrorMode,
                                                             (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = zego_value_get_int(configMap[FTValue("bitrate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[FTValue("channel")]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)zego_value_get_int(configMap[FTValue("codecID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    auto config =
        EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig((EXPRESS::ZegoPublishChannel)channel);
    FTMap retMap;
    retMap[FTValue("bitrate")] = FTValue(config.bitrate);
    retMap[FTValue("channel")] = FTValue(config.channel);
    retMap[FTValue("codecID")] = FTValue(config.codecID);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey(
    FTArgument &argument,
    FTResult result) {
    auto key = zego_value_get_string(argument[FTValue("key")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishStreamEncryptionKey(
        key, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePublishStreamSnapshot(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    auto sharedPtrResult =
        FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePublishStreamVideo(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamVideo(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(
    FTArgument &argument,
    FTResult result) {
    auto volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode(
        (EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(
    FTArgument &argument,
    FTResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(),
                                                  (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendAudioSideInfo(
    FTArgument &argument,
    FTResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
    auto timeStampMs = zego_value_get_double(argument[FTValue("timeStampMs")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendAudioSideInfo(
        byteData.data(), (unsigned int)byteData.size(), timeStampMs,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCapturePipelineScaleMode(
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCapturePipelineScaleMode(
        (EXPRESS::ZegoCapturePipelineScaleMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableH265EncodeFallback(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableH265EncodeFallback(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoEncoderSupported(
    FTArgument &argument,
    FTResult result) {
    auto codecID = zego_value_get_int(argument[FTValue("codecID")]);
    if (codecID > 4) {
        codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
    }

    int ret = 0;
    if (!argument[FTValue("codecBackend")].IsNull()) {
        auto codecBackend = zego_value_get_int(argument[FTValue("codecBackend")]);
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
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setLowlightEnhancement(
        (EXPRESS::ZegoLowlightEnhancementMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoSource(
    FTArgument &argument,
    FTResult result) {
    auto source = zego_value_get_int(argument[FTValue("source")]);

    bool hasInstanceID = false;
    int instanceID = -1;
    if (!argument[FTValue("instanceID")].IsNull()) {
        hasInstanceID = true;
        instanceID = zego_value_get_int(argument[FTValue("instanceID")]);
    }

    bool hasChannel = false;
    int channel = 0;
    if (!argument[FTValue("channel")].IsNull()) {
        hasChannel = true;
        channel = zego_value_get_int(argument[FTValue("channel")]);
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
    FTArgument &argument,
    FTResult result) {
    auto source = zego_value_get_int(argument[FTValue("source")]);

    bool hasConfig = false;
    EXPRESS::ZegoAudioSourceMixConfig config;
    if (!argument[FTValue("config")].IsNull()) {
        hasConfig = true;
        auto configMap = zego_value_get_map(argument[FTValue("config")]);

        auto audioEffectPlayerIndexList =
            zego_value_get_list(configMap[FTValue("audioEffectPlayerIndexList")]);
        config.audioEffectPlayerCount = audioEffectPlayerIndexList.size();
        int *audioEffectPlayers = nullptr;
        if (config.audioEffectPlayerCount > 0) {
            audioEffectPlayers = new int[config.audioEffectPlayerCount];
        }
        config.audioEffectPlayerIndexList = audioEffectPlayers;
        for (auto audioEffectPlayerIndex : audioEffectPlayerIndexList) {
            *audioEffectPlayers = zego_value_get_int(audioEffectPlayerIndex);
            audioEffectPlayers += 1;
        }

        auto mediaPlayerIndexList = zego_value_get_list(configMap[FTValue("mediaPlayerIndexList")]);
        config.mediaPlayerCount = mediaPlayerIndexList.size();
        int *mediaPlayers = nullptr;
        if (config.mediaPlayerCount > 0) {
            mediaPlayers = new int[config.mediaPlayerCount];
        }
        config.mediaPlayerIndexList = mediaPlayers;
        for (auto mediaPlayerIndex : mediaPlayerIndexList) {
            *mediaPlayers = zego_value_get_int(mediaPlayerIndex);
            mediaPlayers += 1;
        }

        config.enableMixEnginePlayout =
            zego_value_get_bool(configMap[FTValue("enableMixEnginePlayout")]);
        config.enableMixSystemPlayout =
            zego_value_get_bool(configMap[FTValue("enableMixSystemPlayout")]);
    }

    bool hasChannel = false;
    int channel = -1;
    if (!argument[FTValue("channel")].IsNull()) {
        hasChannel = true;
        channel = zego_value_get_int(argument[FTValue("channel")]);
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
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoObjectSegmentationConfig config;
    if (!argument[FTValue("config")].IsNull()) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);

        auto type = zego_value_get_int(configMap[FTValue("objectSegmentationType")]);
        config.objectSegmentationType = (EXPRESS::ZegoObjectSegmentationType)type;

        if (!configMap[FTValue("backgroundConfig")].IsNull()) {
            auto backgroundConfigMap = zego_value_get_map(configMap[FTValue("backgroundConfig")]);

            EXPRESS::ZegoBackgroundConfig backgroundConfig;
            backgroundConfig.processType = (EXPRESS::ZegoBackgroundProcessType)zego_value_get_int(backgroundConfigMap[FTValue("processType")]);
            backgroundConfig.blurLevel = (EXPRESS::ZegoBackgroundBlurLevel)zego_value_get_int(backgroundConfigMap[FTValue("blurLevel")]);
            // Note：注意这里巨坑，如果这里使用std::get<int64_t>处理传过来的0时会导致crash, 怀疑是flutter内部的bug
            backgroundConfig.color = backgroundConfigMap[FTValue("color")].LongValue();
            backgroundConfig.imageURL = zego_value_get_string(backgroundConfigMap[FTValue("imageURL")]);
            backgroundConfig.videoURL = zego_value_get_string(backgroundConfigMap[FTValue("videoURL")]);

            config.backgroundConfig = backgroundConfig;
        }
    }
    

    EXPRESS::ZegoExpressSDK::getEngine()->enableVideoObjectSegmentation(
        enable, config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlphaChannelVideoEncoder(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto alphaLayout = zego_value_get_int(argument[FTValue("alphaLayout")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAlphaChannelVideoEncoder(
        enable, (EXPRESS::ZegoAlphaLayoutType)alphaLayout, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    FTMap canvasMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        auto alphaBlend = zego_value_get_bool(canvasMap[FTValue("alphaBlend")]);

        ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
        ZegoTextureRendererController::getInstance()->addRemoteRenderer(viewID, streamID, viewMode);
        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }

    FTMap configMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("config")])) {
        configMap = zego_value_get_map(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode =
            (EXPRESS::ZegoStreamResourceMode)zego_value_get_int(configMap[FTValue("resourceMode")]);
        auto videoCodecIDIndex = zego_value_get_int(configMap[FTValue("videoCodecID")]);
        if (videoCodecIDIndex > 4) {
            config.videoCodecID = EXPRESS::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
        } else {
            config.videoCodecID = (EXPRESS::ZegoVideoCodecID)videoCodecIDIndex;
        }

        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.sourceResourceType =
            (EXPRESS::ZegoResourceType)zego_value_get_int(configMap[FTValue("sourceResourceType")]);
        config.codecTemplateID =
            (EXPRESS::ZegoResourceType)zego_value_get_int(configMap[FTValue("codecTemplateID")]);
        config.resourceSwitchMode = (EXPRESS::ZegoStreamResourceSwitchMode)zego_value_get_int(configMap[FTValue("resourceSwitchMode")]);
        config.resourceWhenStopPublish = (EXPRESS::ZegoStreamResourceType)zego_value_get_int(configMap[FTValue("resourceWhenStopPublish")]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (std::holds_alternative<FTMap>(configMap[FTValue("cdnConfig")])) {
            auto cdnConfigMap = zego_value_get_map(configMap[FTValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = zego_value_get_string(cdnConfigMap[FTValue("url")]);
                cdnConfigPtr->authParam = zego_value_get_string(cdnConfigMap[FTValue("authParam")]);
                cdnConfigPtr->protocol = zego_value_get_string(cdnConfigMap[FTValue("protocol")]);
                cdnConfigPtr->quicVersion =
                    zego_value_get_string(cdnConfigMap[FTValue("quicVersion")]);
                cdnConfigPtr->httpdns =
                    (EXPRESS::ZegoHttpDNSType)zego_value_get_int(cdnConfigMap[FTValue("httpdns")]);

                cdnConfigPtr->quicConnectMode = (int)zego_value_get_int(cdnConfigMap[FTValue("quicConnectMode")]);
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
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    FTMap infoMap = zego_value_get_map(argument[FTValue("info")]);
    EXPRESS::ZegoCrossAppInfo info;
    info.appID = infoMap[FTValue("appID")].LongValue();
    info.token = zego_value_get_string(infoMap[FTValue("token")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamCrossAppInfo(streamID, info);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePlayStreamSnapshot(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    auto sharedPtrResult =
        FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(
    FTArgument &argument,
    FTResult result) {
    auto volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayAudioStreams(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayAudioStreams(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(
    FTArgument &argument,
    FTResult result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(
    FTArgument &argument,
    FTResult result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(
    FTArgument &argument,
    FTResult result) {
    auto type = zego_value_get_int(argument[FTValue("type")]);

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
    FTArgument &argument,
    FTResult result) {
    auto type = zego_value_get_int(argument[FTValue("type")]);
    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultAudioDeviceID(
        (EXPRESS::ZegoAudioDeviceType)type);

    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::useAudioDevice(
    FTArgument &argument,
    FTResult result) {
    auto type = zego_value_get_int(argument[FTValue("type")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);
    EXPRESS::ZegoExpressSDK::getEngine()->useAudioDevice((EXPRESS::ZegoAudioDeviceType)type,
                                                         deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(
    FTArgument &argument,
    FTResult result) {
    FTMap configMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("config")])) {
        configMap = zego_value_get_map(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {
        EXPRESS::ZegoSoundLevelConfig config;
        config.millisecond = zego_value_get_int(configMap[FTValue("millisecond")]);
        config.enableVAD = zego_value_get_bool(configMap[FTValue("enableVAD")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startSoundLevelMonitor(config);
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopSoundLevelMonitor(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopSoundLevelMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneMonitor(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(
    FTArgument &argument,
    FTResult result) {
    int volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(
    FTArgument &argument,
    FTResult result) {
    int mode = zego_value_get_int(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableSpeechEnhance(
    FTArgument &argument,
    FTResult result) {

    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    int level = zego_value_get_int(argument[FTValue("level")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableSpeechEnhance(enable, level);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(
    FTArgument &argument,
    FTResult result) {
    auto bandIndex = zego_value_get_int(argument[FTValue("bandIndex")]);
    auto bandGain = zego_value_get_double(argument[FTValue("bandGain")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(
    FTArgument &argument,
    FTResult result) {
    auto preset = zego_value_get_int(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset(
        (EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(
    FTArgument &argument,
    FTResult result) {
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)zego_value_get_double(paramMap[FTValue("pitch")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(
    FTArgument &argument,
    FTResult result) {
    auto preset = zego_value_get_int(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(
    FTArgument &argument,
    FTResult result) {
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoReverbAdvancedParam param;
    param.damping = (float)zego_value_get_double(paramMap[FTValue("damping")]);
    param.roomSize = (float)zego_value_get_double(paramMap[FTValue("roomSize")]);
    param.reverberance = (float)zego_value_get_double(paramMap[FTValue("reverberance")]);
    param.wetOnly = zego_value_get_bool(paramMap[FTValue("wetOnly")]);
    param.wetGain = (float)zego_value_get_double(paramMap[FTValue("wetGain")]);
    param.dryGain = (float)zego_value_get_double(paramMap[FTValue("dryGain")]);
    param.toneLow = (float)zego_value_get_double(paramMap[FTValue("toneLow")]);
    param.toneHigh = (float)zego_value_get_double(paramMap[FTValue("toneHigh")]);
    param.preDelay = (float)zego_value_get_double(paramMap[FTValue("preDelay")]);
    param.stereoWidth = (float)zego_value_get_double(paramMap[FTValue("stereoWidth")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbAdvancedParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbEchoParam(
    FTArgument &argument,
    FTResult result) {
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)zego_value_get_double(paramMap[FTValue("inGain")]);
    param.outGain = (float)zego_value_get_double(paramMap[FTValue("outGain")]);
    param.numDelays = zego_value_get_int(paramMap[FTValue("numDelays")]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    FTArray delayVec = zego_value_get_list(paramMap[FTValue("delay")]);
    FTArray decayVec = zego_value_get_list(paramMap[FTValue("decay")]);

    for (int i = 0; i < param.numDelays; i++) {
        param.delay[i] = zego_value_get_int(delayVec.at(i));
        param.decay[i] = (float)zego_value_get_double(decayVec.at(i));
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbEchoParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVirtualStereo(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto angle = zego_value_get_int(argument[FTValue("angle")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto angle = zego_value_get_int(argument[FTValue("angle")]);
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePlayStreamVirtualStereo(enable, angle, streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setElectronicEffects(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto mode = zego_value_get_int(argument[FTValue("mode")]);
    auto tonal = zego_value_get_int(argument[FTValue("tonal")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setElectronicEffects(
        enable, (ZEGO::EXPRESS::ZegoElectronicEffectsMode)mode, tonal);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioDataObserver(
    FTArgument &argument,
    FTResult result) {
    auto bitmask = zego_value_get_int(argument[FTValue("observerBitMask")]);
    auto param = zego_value_get_map(argument[FTValue("param")]);

    EXPRESS::ZegoAudioFrameParam nativeParam;
    nativeParam.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(param[FTValue("sampleRate")]);
    nativeParam.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(param[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDataObserver(bitmask, nativeParam);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDataObserver(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDataObserver();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAudioEffectPlayer(
    FTArgument &argument,
    FTResult result) {
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        std::string path = zego_value_get_string(argument[FTValue("path")]);

        FTMap configMap = zego_value_get_map(argument[FTValue("config")]);
        std::unique_ptr<EXPRESS::ZegoAudioEffectPlayConfig> configPtr = nullptr;

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoAudioEffectPlayConfig>();
            configPtr->playCount = zego_value_get_int(configMap[FTValue("playCount")]);
            configPtr->isPublishOut = zego_value_get_bool(configMap[FTValue("isPublishOut")]);
        }

        player->start(audioEffectID, path, configPtr.get());

        result->Success();
    } else {
        result->Error("audioEffectPlayerStart_Can_not_find_player",
                      "Invoke `audioEffectPlayerStart` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStop(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        player->stop(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerStop_Can_not_find_player",
                      "Invoke `audioEffectPlayerStop` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPause(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        player->pause(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerPause_Can_not_find_player",
                      "Invoke `audioEffectPlayerPause` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        player->resume(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerResume_Can_not_find_player",
                      "Invoke `audioEffectPlayerResume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setPlayVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setPublishVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPublishVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPublishVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolumeAll(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setPlayVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolumeAll(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[FTValue("volume")]);
        player->setPublishVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPublishVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPublishVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        auto totalDuration = player->getTotalDuration(audioEffectID);

        result->Success(FTValue((int64_t)totalDuration));
    } else {
        result->Error("audioEffectPlayerGetTotalDuration_Can_not_find_player",
                      "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        auto currentProgress = player->getCurrentProgress(audioEffectID);

        result->Success(FTValue((int64_t)currentProgress));
    } else {
        result->Error(
            "audioEffectPlayerGetCurrentProgress_Can_not_find_player",
            "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        std::string path = zego_value_get_string(argument[FTValue("path")]);

        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        player->unloadResource(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerUnLoadResource_Can_not_find_player",
                      "Invoke `audioEffectPlayerUnloadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        auto speed = zego_value_get_double(argument[FTValue("speed")]);
        player->setPlaySpeed(audioEffectID, speed);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlaySpeed_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUpdatePosition(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[FTValue("audioEffectID")]);
        auto position = zego_value_get_vector_float(argument[FTValue("position")]);
        player->updatePosition(audioEffectID, position.data());

        result->Success();
    } else {
        result->Error("audioEffectPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `audioEffectPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaPlayer(
    FTArgument &argument,
    FTResult result) {
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = zego_value_get_string(argument[FTValue("path")]);

        auto sharedPtrResult =
            FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto mediaData = zego_value_get_vector_uint8(argument[FTValue("mediaData")]);

        auto sharedPtrResult =
            FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = zego_value_get_string(argument[FTValue("path")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult =
            FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->start();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerStop(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->stop();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerPause(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->pause();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerResume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->resume();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSeekTo(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[FTValue("enable")]);

        mediaPlayer->enableRepeat(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAux(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[FTValue("enable")]);

        mediaPlayer->enableAux(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool mute = zego_value_get_bool(argument[FTValue("mute")]);

        mediaPlayer->muteLocal(mute);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableViewMirror(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[FTValue("enable")]);

        mediaPlayer->enableViewMirror(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[FTValue("volume")]);

        mediaPlayer->setVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[FTValue("volume")]);

        mediaPlayer->setPlayVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[FTValue("volume")]);

        mediaPlayer->setPublishVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int millsecond = zego_value_get_int(argument[FTValue("millisecond")]);

        mediaPlayer->setProgressInterval((unsigned long long)millsecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPlayVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPublishVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioTrackCount = mediaPlayer->getAudioTrackCount();
        result->Success(FTValue((int32_t)audioTrackCount));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto trackIndex = zego_value_get_int(argument[FTValue("trackIndex")]);
        mediaPlayer->setAudioTrackIndex(trackIndex);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        FTMap paramMap = zego_value_get_map(argument[FTValue("param")]);
        auto pitch = zego_value_get_double(paramMap[FTValue("pitch")]);

        auto audioChannel = zego_value_get_int(argument[FTValue("audioChannel")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->setVoiceChangerParam((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel,
                                          param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto state = mediaPlayer->getCurrentState();
        result->Success(FTValue((int32_t)state));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto speed = zego_value_get_double(argument[FTValue("speed")]);
        mediaPlayer->setPlaySpeed(speed);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableSoundLevelMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableFrequencySpectrumMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto time = argument[FTValue("time")].LongValue();
        auto size = argument[FTValue("size")].LongValue();
        mediaPlayer->setNetWorkResourceMaxCache(time, size);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto threshold = argument[FTValue("threshold")].LongValue();
        mediaPlayer->setNetWorkBufferThreshold(threshold);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        FTMap configMap = zego_value_get_map(argument[FTValue("config")]);
        auto timeout = zego_value_get_int(configMap[FTValue("timeout")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult =
            FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioChannel = zego_value_get_int(argument[FTValue("audioChannel")]);
        mediaPlayer->setActiveAudioChannel((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel);

        result->Success();
    } else {
        result->Error("mediaPlayerSetActiveAudioChannel_Can_not_find_player",
                      "Invoke `mediaPlayerSetActiveAudioChannel` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {

        FTMap canvasMap =
            zego_value_get_map(argument[FTValue("canvas")]);

        EXPRESS::ZegoCanvas canvas;
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[FTValue("viewMode")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto pFrame =
            ZegoTextureRendererController::getInstance()->getMediaPlayerFrame(mediaPlayer);
        auto size = ZegoTextureRendererController::getInstance()->getMediaPlayerSize(mediaPlayer);
        auto stride = ZegoTextureRendererController::getInstance()->getMediaPlayerFrameStride(mediaPlayer);
        FTMap resultMap;
        if (pFrame && size != std::pair(0, 0)) {
            auto tmpData = makeBtimap(pFrame, size, stride);
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
    FTArgument &argument,
    FTResult result) {
    // auto index = zego_value_get_int(argument[FTValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto mode = zego_value_get_int(argument[FTValue("mode")]);
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
    FTArgument &argument,
    FTResult result) {
    // auto index = zego_value_get_int(argument[FTValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto index_ = zego_value_get_int(argument[FTValue("index_")]);
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
    FTArgument &argument,
    FTResult result) {

    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
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
    FTArgument &argument,
    FTResult result) {

    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
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
    FTArgument &argument,
    FTResult result) {

    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto resourceMap = zego_value_get_map(argument[FTValue("resource")]);
        EXPRESS::ZegoMediaPlayerResource resource;
        resource.resourceID = zego_value_get_string(resourceMap[FTValue("resourceID")]);
        resource.loadType =
            (EXPRESS::ZegoMultimediaLoadType)zego_value_get_int(resourceMap[FTValue("loadType")]);
        resource.alphaLayout =
            (EXPRESS::ZegoAlphaLayoutType)zego_value_get_int(resourceMap[FTValue("alphaLayout")]);
        resource.startPosition = resourceMap[FTValue("startPosition")].LongValue();
        resource.filePath = zego_value_get_string(resourceMap[FTValue("filePath")]);
        auto memory = zego_value_get_vector_uint8(resourceMap[FTValue("memory")]);
        resource.memory = memory.data();
        resource.memoryLength = memory.size();

        auto sharedPtrResult =
            FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto position = zego_value_get_vector_float(argument[FTValue("position")]);
        mediaPlayer->updatePosition(position.data());

        result->Success();
    } else {
        result->Error("mediaPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `mediaPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetMediaInfo(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {

    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto headersMap = zego_value_get_map(argument[FTValue("headers")]);
        std::unordered_map<std::string, std::string> headers;
        for (auto &header : headersMap) {
            std::string key = zego_value_get_string(header.first);
            std::string value = zego_value_get_string(header.second);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        auto mode = zego_value_get_int(argument[FTValue("mode")]);

        mediaPlayer->enableLiveAudioEffect(enable, (EXPRESS::ZegoLiveAudioEffectMode) mode);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLiveAudioEffect_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLiveAudioEffect` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayMediaStreamType(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto streamType = zego_value_get_int(argument[FTValue("streamType")]);

        mediaPlayer->setPlayMediaStreamType((EXPRESS::ZegoMediaStreamType)streamType);

        result->Success();
    } else {
        result->Error("mediaPlayerSetPlayMediaStreamType_Can_not_find_player",
                      "Invoke `mediaPlayerSetPlayMediaStreamType` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableLocalCache(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        std::string cacheDir = zego_value_get_string(argument[FTValue("cacheDir")]);

        mediaPlayer->enableLocalCache(enable, cacheDir);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLocalCache_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLocalCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlaybackStatistics(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {

    FTMap config_map = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoMediaDataPublisherConfig config{};
    config.channel = zego_value_get_int(config_map[FTValue("channel")]);
    config.mode =
        (EXPRESS::ZegoMediaDataPublisherMode)zego_value_get_int(config_map[FTValue("mode")]);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        bool is_clear = zego_value_get_bool(argument[FTValue("isClear")]);
        std::string path = zego_value_get_string(argument[FTValue("path")]);
        publisher->addMediaFilePath(path, is_clear);
        result->Success();
    } else {
        result->Error("mediaDataPublisherAddMediaFilePath_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherAddMediaFilePath` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherGetCurrentDuration(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        int delay_time = zego_value_get_int(argument[FTValue("delay_time")]);
        publisher->setVideoSendDelayTime(delay_time);
        result->Success();
    } else {
        result->Error("mediaDataPublisherSetVideoSendDelayTime_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherSetVideoSendDelayTime` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::startMixerTask(
    FTArgument &argument,
    FTResult result) {

    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = zego_value_get_map(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = zego_value_get_map(argument[FTValue("videoConfig")]);
    FTMap advancedConfigMap = zego_value_get_map(argument[FTValue("advancedConfig")]);

    for (auto &cfg : advancedConfigMap) {
        std::string key = zego_value_get_string(cfg.first);
        std::string value = zego_value_get_string(cfg.second);
        task.advancedConfig[key] = value;
    }

    task.taskID = zego_value_get_string(argument[FTValue("taskID")]);

    auto inputFlutterList = zego_value_get_list(argument[FTValue("inputList")]);
    auto outputFlutterList = zego_value_get_list(argument[FTValue("outputList")]);

    task.backgroundImageURL = zego_value_get_string(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = zego_value_get_bool(argument[FTValue("enableSoundLevel")]);

    // backgroundColor
    task.backgroundColor = zego_value_get_int(argument[FTValue("backgroundColor")]);

    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)zego_value_get_int(
        argument[FTValue("streamAlignmentMode")]);

    task.mixImageCheckMode = (EXPRESS::ZegoMixImageCheckMode)zego_value_get_int(
        argument[FTValue("mixImageCheckMode")]);

    // userData
    auto userData = zego_value_get_vector_uint8(argument[FTValue("userData")]);
    task.userData = userData.data();
    task.userDataLength = userData.size();

    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        zego_value_get_int(argument[FTValue("minPlayStreamBufferLength")]);

    task.audioConfig.bitrate = zego_value_get_int(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)zego_value_get_int(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)zego_value_get_int(taskAudioConfig[FTValue("codecID")]);
    task.audioConfig.mixMode =
        (EXPRESS::ZegoAudioMixMode)zego_value_get_int(taskAudioConfig[FTValue("mixMode")]);

    task.videoConfig.bitrate = zego_value_get_int(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = zego_value_get_int(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = zego_value_get_int(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = zego_value_get_int(taskVideoConfig[FTValue("width")]);
    task.videoConfig.quality = zego_value_get_int(taskVideoConfig[FTValue("quality")]);
    task.videoConfig.rateControlMode = (EXPRESS::ZegoVideoRateControlMode)zego_value_get_int(
        taskVideoConfig[FTValue("rateControlMode")]);

    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = zego_value_get_map(inputIter);

        input.streamID = zego_value_get_string(inputMap[FTValue("streamID")]);
        input.contentType =
            (EXPRESS::ZegoMixerInputContentType)zego_value_get_int(inputMap[FTValue("contentType")]);
        input.layout.height = zego_value_get_int(inputMap[FTValue("bottom")]) -
                              zego_value_get_int(inputMap[FTValue("top")]);
        input.layout.width = zego_value_get_int(inputMap[FTValue("right")]) -
                             zego_value_get_int(inputMap[FTValue("left")]);
        input.layout.x = zego_value_get_int(inputMap[FTValue("left")]);
        input.layout.y = zego_value_get_int(inputMap[FTValue("top")]);
        input.soundLevelID = zego_value_get_int(inputMap[FTValue("soundLevelID")]);

        input.volume = zego_value_get_int(inputMap[FTValue("volume")]);
        input.isAudioFocus = zego_value_get_bool(inputMap[FTValue("isAudioFocus")]);
        input.audioDirection = zego_value_get_int(inputMap[FTValue("audioDirection")]);

        if (!inputMap[FTValue("label")].IsNull()) {
            auto labelMap = zego_value_get_map(inputMap[FTValue("label")]);
            auto text = zego_value_get_string(labelMap[FTValue("text")]);
            EXPRESS::ZegoLabelInfo labelInfo(text);
            labelInfo.left = zego_value_get_int(labelMap[FTValue("left")]);
            labelInfo.top = zego_value_get_int(labelMap[FTValue("top")]);
            auto fontMap = zego_value_get_map(labelMap[FTValue("font")]);
            EXPRESS::ZegoFontStyle fontStyle;
            fontStyle.type = (EXPRESS::ZegoFontType)zego_value_get_int(fontMap[FTValue("type")]);
            fontStyle.size = zego_value_get_int(fontMap[FTValue("size")]);
            fontStyle.color = zego_value_get_int(fontMap[FTValue("color")]);
            fontStyle.transparency = zego_value_get_int(fontMap[FTValue("transparency")]);
            fontStyle.border = zego_value_get_bool(fontMap[FTValue("border")]);
            fontStyle.borderColor = zego_value_get_int(fontMap[FTValue("borderColor")]);
            labelInfo.font = fontStyle;
            input.label = labelInfo;
        }

        if (!inputMap[FTValue("renderMode")].IsNull()) {
            input.renderMode =
                (EXPRESS::ZegoMixRenderMode)zego_value_get_int(inputMap[FTValue("renderMode")]);
        }

        if (!inputMap[FTValue("imageInfo")].IsNull()) {
            auto imageInfoMap = zego_value_get_map(inputMap[FTValue("imageInfo")]);
            auto url = zego_value_get_string(imageInfoMap[FTValue("url")]);
            int32_t displayMode = 0;
            if (!imageInfoMap[FTValue("displayMode")].IsNull()) {
                displayMode = zego_value_get_int(imageInfoMap[FTValue("displayMode")]);
            }
            input.imageInfo = EXPRESS::ZegoMixerImageInfo(url, displayMode);
        }

        if (!inputMap[FTValue("cornerRadius")].IsNull()) {
            input.cornerRadius = zego_value_get_int(inputMap[FTValue("cornerRadius")]);
        }

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = zego_value_get_map(outputIter);
        output.target = zego_value_get_string(outputMap[FTValue("target")]);

        if (!outputMap[FTValue("videoConfig")].IsNull()) {
            auto videoConfigMap =
                zego_value_get_map(outputMap[FTValue("videoConfig")]);
            output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[FTValue("bitrate")]);
            output.videoConfig.encodeLatency =
                zego_value_get_int(videoConfigMap[FTValue("encodeLatency")]);
            output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                videoConfigMap[FTValue("encodeProfile")]);
            output.videoConfig.enableLowBitrateHD = zego_value_get_bool(
                videoConfigMap[FTValue("enableLowBitrateHD")]);

            auto codecID = zego_value_get_int(videoConfigMap[FTValue("videoCodecID")]);
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
        auto watermarkMap = zego_value_get_map(argument[FTValue("watermark")]);
        std::string imageURL = zego_value_get_string(watermarkMap[FTValue("imageURL")]);
        if (!imageURL.empty()) {    
            watermark.imageURL = imageURL;
            watermark.layout.x = zego_value_get_int(watermarkMap[FTValue("left")]);
            watermark.layout.y = zego_value_get_int(watermarkMap[FTValue("top")]);
            watermark.layout.width = zego_value_get_int(watermarkMap[FTValue("right")]) -
                                     zego_value_get_int(watermarkMap[FTValue("left")]);
            watermark.layout.height = zego_value_get_int(watermarkMap[FTValue("bottom")]) -
                                      zego_value_get_int(watermarkMap[FTValue("top")]);

            task.watermark = &watermark;
        }
    }

    // whiteboard
    EXPRESS::ZegoMixerWhiteboard whiteboard;
    if (!argument[FTValue("whiteboard")].IsNull()) {
        auto whiteboardMap = zego_value_get_map(argument[FTValue("whiteboard")]);
        int64_t whiteboardID = whiteboardMap[FTValue("whiteboardID")].LongValue();
        if (whiteboardID != 0) {
            whiteboard.whiteboardID = whiteboardID;
            whiteboard.horizontalRatio =
                zego_value_get_int(whiteboardMap[FTValue("horizontalRatio")]);
            whiteboard.verticalRatio = zego_value_get_int(whiteboardMap[FTValue("verticalRatio")]);
            whiteboard.isPPTAnimation = zego_value_get_bool(whiteboardMap[FTValue("isPPTAnimation")]);
            whiteboard.zOrder = zego_value_get_int(whiteboardMap[FTValue("zOrder")]);

            auto layoutMap = zego_value_get_map(whiteboardMap[FTValue("layout")]);
            whiteboard.layout.x = zego_value_get_int(layoutMap[FTValue("left")]);
            whiteboard.layout.y = zego_value_get_int(layoutMap[FTValue("top")]);
            whiteboard.layout.width = zego_value_get_int(layoutMap[FTValue("right")]) -
                                      zego_value_get_int(layoutMap[FTValue("left")]);
            whiteboard.layout.height = zego_value_get_int(layoutMap[FTValue("bottom")]) -
                                       zego_value_get_int(layoutMap[FTValue("top")]);

            task.whiteboard = &whiteboard;
        }
    }

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->startMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            ZF::logInfo("*** Plugin onMixerStartResult");
            FTMap retMap;

            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("extendedData")] = FTValue(extendedData);

            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopMixerTask(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = zego_value_get_map(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = zego_value_get_map(argument[FTValue("videoConfig")]);
    auto taskWatermark = zego_value_get_map(argument[FTValue("watermark")]);

    task.taskID = zego_value_get_string(argument[FTValue("taskID")]);

    task.audioConfig.bitrate = zego_value_get_int(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)zego_value_get_int(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)zego_value_get_int(taskAudioConfig[FTValue("codecID")]);

    task.videoConfig.bitrate = zego_value_get_int(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = zego_value_get_int(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = zego_value_get_int(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = zego_value_get_int(taskVideoConfig[FTValue("width")]);

    auto inputFlutterList = zego_value_get_list(argument[FTValue("inputList")]);
    auto outputFlutterList = zego_value_get_list(argument[FTValue("outputList")]);

    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = zego_value_get_map(inputIter);

        input.streamID = zego_value_get_string(inputMap[FTValue("streamID")]);
        input.contentType =
            (EXPRESS::ZegoMixerInputContentType)zego_value_get_int(inputMap[FTValue("contentType")]);
        input.layout.height = zego_value_get_int(inputMap[FTValue("bottom")]) -
                              zego_value_get_int(inputMap[FTValue("top")]);
        input.layout.width = zego_value_get_int(inputMap[FTValue("right")]) -
                             zego_value_get_int(inputMap[FTValue("left")]);
        input.layout.x = zego_value_get_int(inputMap[FTValue("left")]);
        input.layout.y = zego_value_get_int(inputMap[FTValue("top")]);
        input.soundLevelID = zego_value_get_int(inputMap[FTValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = zego_value_get_map(outputIter);
        output.target = zego_value_get_string(outputMap[FTValue("target")]);

        if (!outputMap[FTValue("videoConfig")].IsNull()) {
            auto videoConfigMap =
                zego_value_get_map(outputMap[FTValue("videoConfig")]);
            output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[FTValue("bitrate")]);
            output.videoConfig.encodeLatency =
                zego_value_get_int(videoConfigMap[FTValue("encodeLatency")]);
            output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                videoConfigMap[FTValue("encodeProfile")]);
            output.videoConfig.enableLowBitrateHD = zego_value_get_bool(
                videoConfigMap[FTValue("enableLowBitrateHD")]);

            auto codecID = zego_value_get_int(videoConfigMap[FTValue("videoCodecID")]);
            if (codecID > 4) {
                codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
            }
            output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecID;
        }

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;
    task.backgroundImageURL = zego_value_get_string(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = zego_value_get_bool(argument[FTValue("enableSoundLevel")]);

    FTMap advancedConfigMap = zego_value_get_map(argument[FTValue("advancedConfig")]);
    for (auto &cfg : advancedConfigMap) {
        std::string key = zego_value_get_string(cfg.first);
        std::string value = zego_value_get_string(cfg.second);
        task.advancedConfig[key] = value;
    }

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->stopMixerTask(task, [=](int errorCode) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setSEIConfig(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoSEIConfig config;
    FTMap configMap = zego_value_get_map(argument[FTValue("config")]);

    config.type = (EXPRESS::ZegoSEIType)zego_value_get_int(configMap[FTValue("type")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSEIConfig(config);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioDeviceVolume(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);
    auto volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setSpeakerVolumeInAPP(
    FTArgument &argument,
    FTResult result) {
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);
    auto volume = zego_value_get_int(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSpeakerVolumeInAPP(deviceID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getSpeakerVolumeInAPP(
    FTArgument &argument,
    FTResult result) {
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);

    int volume = EXPRESS::ZegoExpressSDK::getEngine()->getSpeakerVolumeInAPP(deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::startAudioDeviceVolumeMonitor(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDeviceVolumeMonitor(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAudioDevice(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAudioDevice((EXPRESS::ZegoAudioDeviceType)deviceType,
                                                          deviceID.c_str(), mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAudioDeviceMuted(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);

    auto ret = EXPRESS::ZegoExpressSDK::getEngine()->isAudioDeviceMuted(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioDeviceMode(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioDeviceVolume(
    FTArgument &argument,
    FTResult result) {
    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);

    auto volume = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureDevice(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    std::thread tmpThread([=]() {
        EXPRESS::ZegoExpressSDK::getEngine()->enableAudioCaptureDevice(enable);
        sharedPtrResult->Success();
    });

    tmpThread.detach();
}

void ZegoExpressEngineMethodHandler::enableTrafficControl(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto property = zego_value_get_int(argument[FTValue("property")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTrafficControl(enable, property, 
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startRecordingCapturedData(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoDataRecordConfig config;

    FTMap configMap = zego_value_get_map(argument[FTValue("config")]);

    config.filePath = zego_value_get_string(configMap[FTValue("filePath")]);
    config.recordType =
        (EXPRESS::ZegoDataRecordType)zego_value_get_int(configMap[FTValue("recordType")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startRecordingCapturedData(
        config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopRecordingCapturedData(
    FTArgument &argument,
    FTResult result) {
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopRecordingCapturedData(
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCamera(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCamera(enable,
                                                       (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto minFPS = zego_value_get_int(argument[FTValue("minFPS")]);
    auto maxFPS = zego_value_get_int(argument[FTValue("maxFPS")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCameraAdaptiveFPS(
        enable, minFPS, maxFPS, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::useVideoDevice(
    FTArgument &argument,
    FTResult result) {
    auto deviceID = zego_value_get_string(argument[FTValue("deviceID")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->useVideoDevice(deviceID,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoDeviceList(
    FTArgument &argument,
    FTResult result) {

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
    FTArgument &argument,
    FTResult result) {

    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultVideoDeviceID();
    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::enableMixSystemPlayout(
    FTArgument &argument,
    FTResult result) {

    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixSystemPlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setMixSystemPlayoutVolume(
    FTArgument &argument,
    FTResult result) {

    auto volume = zego_value_get_int(argument[FTValue("volume")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setMixSystemPlayoutVolume(volume);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableMixEnginePlayout(
    FTArgument &argument,
    FTResult result) {

    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixEnginePlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioVADStableStateMonitor(
    FTArgument &argument,
    FTResult result) {

    auto type = zego_value_get_int(argument[FTValue("type")]);
    if (argument[FTValue("millisecond")].IsNull()) {
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type);
    } else {
        auto millisecond = zego_value_get_int(argument[FTValue("millisecond")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioVADStableStateMonitor(
    FTArgument &argument,
    FTResult result) {

    auto type = zego_value_get_int(argument[FTValue("type")]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioVADStableStateMonitor(
        (EXPRESS::ZegoAudioVADStableStateMonitorType)type);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getCurrentAudioDevice(
    FTArgument &argument,
    FTResult result) {

    auto deviceType = zego_value_get_int(argument[FTValue("deviceType")]);
    auto deviceInfo = EXPRESS::ZegoExpressSDK::getEngine()->getCurrentAudioDevice(
        (EXPRESS::ZegoAudioDeviceType)deviceType);

    FTMap resultMap;
    resultMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
    resultMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);
    result->Success(resultMap);
}

void ZegoExpressEngineMethodHandler::createCopyrightedMusic(
    FTArgument &argument,
    FTResult result) {
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
    FTArgument &argument,
    FTResult result) {
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        copyrightedMusic_->clearCache();
        result->Success();
    } else {
        result->Error("copyrightedMusicClearCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicClearCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicDownload(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        copyrightedMusic_->cancelDownload(resourceID);
        result->Success();
    } else {
        result->Error("copyrightedMusicCancelDownload_Can_not_find_instance",
                      "Invoke `copyrightedMusicCancelDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getAverageScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetAverageScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetAverageScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto ret = copyrightedMusic_->getCacheSize();
        result->Success(FTValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetCacheSize_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCacheSize` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getCurrentPitch(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetCurrentPitch_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getDuration(resourceID);
        result->Success(FTValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetDuration_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetDuration` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto krcToken = zego_value_get_string(argument[FTValue("krcToken")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto songID = zego_value_get_string(argument[FTValue("songID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
        if (argument[FTValue("vendorID")].IsNull()) {
            copyrightedMusic_->getLrcLyric(songID, [=](int errorCode, std::string lyrics) {
                FTMap retMap;
                retMap[FTValue("errorCode")] = FTValue(errorCode);
                retMap[FTValue("lyrics")] = FTValue(lyrics);
                sharedPtrResult->Success(retMap);
            });
        } else {
            int vendorID = zego_value_get_int(argument[FTValue("vendorID")]);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetLyricConfig config;
        
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[FTValue("vendorID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto shareToken = zego_value_get_string(argument[FTValue("shareToken")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getPreviousScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetPreviousScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getTotalScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetTotalScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetTotalScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicConfig config;
        auto userMap = zego_value_get_map(configMap[FTValue("user")]);
        config.user.userID = zego_value_get_string(userMap[FTValue("userID")]);
        config.user.userName = zego_value_get_string(userMap[FTValue("userName")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->pauseScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicPauseScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicPauseScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto songID = zego_value_get_string(argument[FTValue("songID")]);
        auto type = zego_value_get_int(argument[FTValue("type")]);
        bool ret = false;
        if (argument[FTValue("vendorID")].IsNull()) {
            ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type);
        } else {
            int vendorID = zego_value_get_int(argument[FTValue("vendorID")]);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfig config;
        
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID) zego_value_get_int(configMap[FTValue("vendorID")]);
        config.resourceType = (EXPRESS::ZegoCopyrightedMusicResourceType) zego_value_get_int(configMap[FTValue("resourceType")]);
        config.resourceQualityType = (EXPRESS::ZegoCopyrightedMusicResourceQualityType) zego_value_get_int(configMap[FTValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCacheWithConfig` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCacheWithConfigV2(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfigV2 config;
        
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[FTValue("vendorID")]);
        config.resourceType = zego_value_get_int(configMap[FTValue("resourceType")]);
        config.resourceQualityType = zego_value_get_int(configMap[FTValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCacheWithConfigV2` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[FTValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[FTValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[FTValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[FTValue("sceneID")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resetScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicResetScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResetScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resumeScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicResumeScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResumeScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto command = zego_value_get_string(argument[FTValue("command")]);
        auto params = zego_value_get_string(argument[FTValue("params")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto level = zego_value_get_int(argument[FTValue("level")]);
        copyrightedMusic_->setScoringLevel(level);
        result->Success();
    } else {
        result->Error("copyrightedMusicSetScoringLevel_Can_not_find_instance",
                      "Invoke `copyrightedMusicSetScoringLevel` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStartScore(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto pitchValueInterval = zego_value_get_int(argument[FTValue("pitchValueInterval")]);
        auto ret = copyrightedMusic_->startScore(resourceID, pitchValueInterval);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicStartScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStartScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStopScore(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->stopScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicStopScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStopScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetFullScore(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getFullScore(resourceID);
        result->Success(FTValue(ret));
    } else {
        result->Error("copyrightedMusicGetFullScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetFullScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetSharedResource(
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfig config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        auto type =
            (EXPRESS::ZegoCopyrightedMusicResourceType)zego_value_get_int(argument[FTValue("type")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfigV2 config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.resourceType = zego_value_get_int(configMap[FTValue("resourceType")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.mode =
            (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(configMap[FTValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[FTValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[FTValue("sceneID")]);
        auto type =
            (EXPRESS::ZegoCopyrightedMusicResourceType)zego_value_get_int(argument[FTValue("type")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfigV2 config;
        config.songID = zego_value_get_string(configMap[FTValue("songID")]);
        config.mode = zego_value_get_int(configMap[FTValue("mode")]);
        config.vendorID = zego_value_get_int(configMap[FTValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[FTValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[FTValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[FTValue("sceneID")]);
        config.resourceType = zego_value_get_int(configMap[FTValue("resourceType")]);
        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto width = zego_value_get_int(argument[FTValue("width")]);
    auto height = zego_value_get_int(argument[FTValue("height")]);

    auto textureID = ZegoTextureRendererController::getInstance()->createTextureRenderer(
        registrar_->texture_registrar(), width, height);

    result->Success(FTValue(textureID));
}

void ZegoExpressEngineMethodHandler::destroyTextureRenderer(
    FTArgument &argument,
    FTResult result) {
    auto textureID = argument[FTValue("textureID")].LongValue();
    bool state = ZegoTextureRendererController::getInstance()->destroyTextureRenderer(textureID);

    result->Success(FTValue(state));
}

void ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl(
    FTArgument &argument,
    FTResult result) {
    auto bitrate = zego_value_get_int(argument[FTValue("bitrate")]);
    auto mode = zego_value_get_int(argument[FTValue("mode")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoBitrateForTrafficControl(
        bitrate, (EXPRESS::ZegoTrafficControlMinVideoBitrateMode)mode,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl(
    FTArgument &argument,
    FTResult result) {
    auto fps = zego_value_get_int(argument[FTValue("fps")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoFpsForTrafficControl(
        fps, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl(
    FTArgument &argument,
    FTResult result) {
    auto width = zego_value_get_int(argument[FTValue("width")]);
    auto height = zego_value_get_int(argument[FTValue("height")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoResolutionForTrafficControl(
        width, height, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setTrafficControlFocusOn(
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setTrafficControlFocusOn(
        (EXPRESS::ZegoTrafficControlFocusOnMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::addPublishCdnUrl(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto targetURL = zego_value_get_string(argument[FTValue("targetURL")]);
    auto timeout = zego_value_get_int(argument[FTValue("timeout")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->addPublishCdnUrl(streamID, targetURL, timeout, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::removePublishCdnUrl(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto targetURL = zego_value_get_string(argument[FTValue("targetURL")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->removePublishCdnUrl(
        streamID, targetURL, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::enablePublishDirectToCDN(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCDNConfig config;
    if (configMap.size() > 0) {
        config.url = zego_value_get_string(configMap[FTValue("url")]);
        config.authParam = zego_value_get_string(configMap[FTValue("authParam")]);
        config.protocol = zego_value_get_string(configMap[FTValue("protocol")]);
        config.quicVersion = zego_value_get_string(configMap[FTValue("quicVersion")]);
        config.httpdns = (EXPRESS::ZegoHttpDNSType)zego_value_get_int(configMap[FTValue("httpdns")]);
        config.quicConnectMode = (int)zego_value_get_int(configMap[FTValue("quicConnectMode")]);
    }

    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePublishDirectToCDN(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPublishWatermark(
    FTArgument &argument,
    FTResult result) {
    auto watermarkMap = zego_value_get_map(argument[FTValue("watermark")]);
    EXPRESS::ZegoWatermark *watermark = nullptr;
    EXPRESS::ZegoWatermark watermarkTemp;
    if (watermarkMap.size() > 0) {
        watermarkTemp.imageURL = zego_value_get_string(watermarkMap[FTValue("imageURL")]);
        watermarkTemp.layout.x = zego_value_get_int(watermarkMap[FTValue("left")]);
        watermarkTemp.layout.y = zego_value_get_int(watermarkMap[FTValue("top")]);
        watermarkTemp.layout.width =
            zego_value_get_int(watermarkMap[FTValue("right")]) - watermarkTemp.layout.x;
        watermarkTemp.layout.height =
            zego_value_get_int(watermarkMap[FTValue("bottom")]) - watermarkTemp.layout.y;

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
    auto isPreviewVisible = zego_value_get_bool(argument[FTValue("isPreviewVisible")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPublishWatermark(watermark, isPreviewVisible,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey(
    FTArgument &argument,
    FTResult result) {
    auto key = zego_value_get_string(argument[FTValue("key")]);
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamDecryptionKey(streamID, key);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamVideoType(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto streamType = zego_value_get_int(argument[FTValue("streamType")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamVideoType(
        streamID, (EXPRESS::ZegoVideoStreamType)streamType);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto minBufferInterval = zego_value_get_int(argument[FTValue("minBufferInterval")]);
    auto maxBufferInterval = zego_value_get_int(argument[FTValue("maxBufferInterval")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamBufferIntervalRange(
        streamID, minBufferInterval, maxBufferInterval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamFocusOn(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamFocusOn(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamVideo(
    FTArgument &argument,
    FTResult result) {
    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamVideo(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamVideo(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayVideoStreams(
    FTArgument &argument,
    FTResult result) {
    auto mute = zego_value_get_bool(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayVideoStreams(mute);
    
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCheckPoc(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCheckPoc(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoDecoderSupported(
    FTArgument &argument,
    FTResult result) {
    auto codecID = zego_value_get_int(argument[FTValue("codecID")]);
    if (codecID > 4) {
        codecID = 100;
    }

    int ret = 0;
    if (!argument[FTValue("codecBackend")].IsNull()) {
        auto codecBackend = zego_value_get_int(argument[FTValue("codecBackend")]);
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
    FTArgument &argument,
    FTResult result) {
    auto mode = zego_value_get_int(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamsAlignmentProperty(
        (EXPRESS::ZegoStreamAlignmentMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVideoSuperResolution(
    FTArgument &argument,
    FTResult result) {
    // auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    // auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    // EXPRESS::ZegoExpressSDK::getEngine()->enableVideoSuperResolution(streamID, enable);

    // result->Success();

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::initVideoSuperResolution(
    FTArgument &argument,
    FTResult result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::uninitVideoSuperResolution(
    FTArgument &argument,
    FTResult result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::updatePlayingCanvas(
    FTArgument &argument,
    FTResult result) {

    auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

    FTMap canvasMap;
    if (std::holds_alternative<FTMap>(argument[FTValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    bool isSuccess = false;
    int64_t viewID = 0;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[FTValue("viewMode")]);
        auto alphaBlend = zego_value_get_bool(canvasMap[FTValue("alphaBlend")]);

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
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = zego_value_get_string(argument[FTValue("taskID")]);
    task.roomID = zego_value_get_string(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = zego_value_get_list(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = zego_value_get_map(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = zego_value_get_string(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = zego_value_get_map(outputMap[FTValue("videoConfig")]);

                auto codecIDIndex = zego_value_get_int(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                        videoConfigMap[FTValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD = zego_value_get_bool(
                    videoConfigMap[FTValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }
    // AudioConfig
    auto audioConfigMap = zego_value_get_map(argument[FTValue("audioConfig")]);
    if (audioConfigMap.size() > 0) {
        task.audioConfig.bitrate = zego_value_get_int(audioConfigMap[FTValue("bitrate")]);
        task.audioConfig.channel =
            (EXPRESS::ZegoAudioChannel)zego_value_get_int(audioConfigMap[FTValue("channel")]);
        task.audioConfig.codecID =
            (EXPRESS::ZegoAudioCodecID)zego_value_get_int(audioConfigMap[FTValue("codecID")]);
    }
    // Enable SoundLevel
    task.enableSoundLevel = zego_value_get_bool(argument[FTValue("enableSoundLevel")]);
    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)zego_value_get_int(
        argument[FTValue("streamAlignmentMode")]);
    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        zego_value_get_int(argument[FTValue("minPlayStreamBufferLength")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->startAutoMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("extendedData")] = FTValue(extendedData);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopAutoMixerTask(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = zego_value_get_string(argument[FTValue("taskID")]);
    task.roomID = zego_value_get_string(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = zego_value_get_list(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = zego_value_get_map(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = zego_value_get_string(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = zego_value_get_map(outputMap[FTValue("videoConfig")]);

                auto codecIDIndex = zego_value_get_int(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                        videoConfigMap[FTValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD = zego_value_get_bool(
                    videoConfigMap[FTValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }

    // no need to set audio config
    // no need to set video config
    // no need to set enable sound level

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->stopAutoMixerTask(task, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioRouteType(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::useFrontCamera(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::isCameraFocusSupported(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusMode(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureMode(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureCompensation(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraZoomFactor(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor(
    FTArgument &argument,
    FTResult result) {
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioSpectrumMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioSpectrumMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneAEC(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startEffectsEnv(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->startEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopEffectsEnv(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableEffectsBeauty(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableEffectsBeauty(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setEffectsBeautyParam(
    FTArgument &argument,
    FTResult result) {
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoEffectsBeautyParam param;
    param.rosyIntensity = zego_value_get_int(paramMap[FTValue("rosyIntensity")]);
    param.sharpenIntensity = zego_value_get_int(paramMap[FTValue("sharpenIntensity")]);
    param.smoothIntensity = zego_value_get_int(paramMap[FTValue("smoothIntensity")]);
    param.whitenIntensity = zego_value_get_int(paramMap[FTValue("whitenIntensity")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setEffectsBeautyParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableBeautify(
    FTArgument &argument,
    FTResult result) {
    result->Error("Not recommended", "Not recommended, please use startEffectsEnv instead");
}

void ZegoExpressEngineMethodHandler::setBeautifyOption(
    FTArgument &argument,
    FTResult result) {
    result->Error("Not recommended",
                  "Not recommended, please use startEffectsEnv and setEffectsBeautyParam instead");
}

void ZegoExpressEngineMethodHandler::sendBroadcastMessage(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto message = zego_value_get_string(argument[FTValue("message")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendBroadcastMessage(
        roomID, message, [=](int errorCode, unsigned long long messageID) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("messageID")] = FTValue((int64_t)messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendBarrageMessage(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto message = zego_value_get_string(argument[FTValue("message")]);

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendBarrageMessage(
        roomID, message, [=](int errorCode, std::string messageID) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("messageID")] = FTValue(messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendCustomCommand(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto command = zego_value_get_string(argument[FTValue("command")]);
    auto toUserListArray = zego_value_get_list(argument[FTValue("toUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = zego_value_get_map(user_);
        user.userID = zego_value_get_string(userMap[FTValue("userID")]);
        user.userName = zego_value_get_string(userMap[FTValue("userName")]);
        toUserList.push_back(user);
    }

    auto sharedPtrResult =
        FTMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomCommand(
        roomID, command, toUserList, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}


void ZegoExpressEngineMethodHandler::sendTransparentMessage(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);
    auto sendMode = zego_value_get_int(argument[FTValue("sendMode")]);
    auto sendType = zego_value_get_int(argument[FTValue("sendType")]);
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("content")]);

    auto toUserListArray = zego_value_get_list(argument[FTValue("recvUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = zego_value_get_map(user_);
        user.userID = zego_value_get_string(userMap[FTValue("userID")]);
        user.userName = zego_value_get_string(userMap[FTValue("userName")]);
        toUserList.push_back(user);
    }

    auto timeout = zego_value_get_int(argument[FTValue("timeOut")]);

    auto sharedPtrResult =
        FTMoveResult(result);

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
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoCaptureConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[FTValue("bufferType")]);
    }

    auto channel = zego_value_get_int(argument[FTValue("channel")]);

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
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoProcessConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[FTValue("bufferType")]);
    }

    auto channel = zego_value_get_int(argument[FTValue("channel")]);

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
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);

    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoRenderConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[FTValue("bufferType")]);
        config.frameFormatSeries = (EXPRESS::ZegoVideoFrameFormatSeries)zego_value_get_int(
            configMap[FTValue("frameFormatSeries")]);
        config.enableEngineRender = zego_value_get_bool(configMap[FTValue("enableEngineRender")]);
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
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlignedAudioAuxData(
    FTArgument &argument,
    FTResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioRemoteProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[FTValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioPlaybackProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioIO(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioConfig config;
    config.sourceType =
        (EXPRESS::ZegoAudioSourceType)zego_value_get_int(configMap[FTValue("sourceType")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioIO(enable, &config,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData(
    FTArgument &argument,
    FTResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto configLength = argument[FTValue("configLength")].LongValue();
    auto referenceTimeMillisecond = argument[FTValue("configLength")].LongValue();
    auto samples = argument[FTValue("samples")].LongValue();
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[FTValue("sampleRate")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCaptureAACData(
        byteData.data(), dataLength, configLength, referenceTimeMillisecond, samples, param,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData(
    FTArgument &argument,
    FTResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[FTValue("sampleRate")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCapturePCMData(
        byteData.data(), dataLength, param, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData(
    FTArgument &argument,
    FTResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = zego_value_get_map(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[FTValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[FTValue("sampleRate")]);

    EXPRESS::ZegoExpressSDK::getEngine()->fetchCustomAudioRenderPCMData(byteData.data(), dataLength,
                                                                        param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPerformanceMonitor(
    FTArgument &argument,
    FTResult result) {
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startPerformanceMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPerformanceMonitor(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopPerformanceMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkProbe(
    FTArgument &argument,
    FTResult result) {
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkProbeConfig config;
    config.enableTraceroute = zego_value_get_bool(configMap[FTValue("enableTraceroute")]);

    auto sharedPtrResult =
        FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkProbe();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkSpeedTest(
    FTArgument &argument,
    FTResult result) {
    auto configMap = zego_value_get_map(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkSpeedTestConfig config;
    config.testUplink = zego_value_get_bool(configMap[FTValue("testUplink")]);
    config.expectedUplinkBitrate = zego_value_get_int(configMap[FTValue("expectedUplinkBitrate")]);
    config.testDownlink = zego_value_get_bool(configMap[FTValue("testDownlink")]);
    config.expectedDownlinkBitrate =
        zego_value_get_int(configMap[FTValue("expectedDownlinkBitrate")]);
    auto interval = argument[FTValue("interval")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkSpeedTest(config, interval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopNetworkSpeedTest(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkSpeedTest();

    result->Success();
}

void ZegoExpressEngineMethodHandler::getNetworkTimeInfo(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoNetworkTimeInfo info = EXPRESS::ZegoExpressSDK::getEngine()->getNetworkTimeInfo();

    FTMap retMap;
    retMap[FTValue("timestamp")] = FTValue((int64_t)info.timestamp);
    retMap[FTValue("maxDeviation")] = FTValue((int32_t)info.maxDeviation);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::startDumpData(
    FTArgument &argument,
    FTResult result) {
        auto configMap = zego_value_get_map(argument[FTValue("config")]);
        EXPRESS::ZegoDumpDataConfig config;
        config.dataType = (EXPRESS::ZegoDumpDataType)zego_value_get_int(configMap[FTValue("dataType")]);

        EXPRESS::ZegoExpressSDK::getEngine()->startDumpData(config);

        result->Success();
    }

void ZegoExpressEngineMethodHandler::stopDumpData(
    FTArgument &argument,
    FTResult result) {
        
        EXPRESS::ZegoExpressSDK::getEngine()->stopDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::uploadDumpData(
    FTArgument &argument,
    FTResult result) {

        EXPRESS::ZegoExpressSDK::getEngine()->uploadDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::removeDumpData(
    FTArgument &argument,
    FTResult result) {

        EXPRESS::ZegoExpressSDK::getEngine()->removeDumpData();

        result->Success();
    }

void ZegoExpressEngineMethodHandler::createRangeAudio(
    FTArgument &argument,
    FTResult result) {
    rangeAudio_ = EXPRESS::ZegoExpressSDK::getEngine()->createRangeAudio();
    if (rangeAudio_) {
        rangeAudio_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(FTValue(0));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyRangeAudio(
    FTArgument &argument,
    FTResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->destroyRangeAudio(rangeAudio_);
    rangeAudio_ = nullptr;
    result->Success();
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);

        rangeAudio_->enableMicrophone(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableMicrophone_Can_not_find_instance",
                      "Invoke `rangeAudioEnableMicrophone` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        rangeAudio_->enableSpatializer(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpatializer_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpatializer` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);

        rangeAudio_->enableSpeaker(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpeaker_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpeaker` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto paramMap = zego_value_get_map(argument[FTValue("param")]);

        EXPRESS::ZegoReceiveRangeParam param;
        param.min = (float)zego_value_get_double(paramMap[FTValue("min")]);
        param.max = (float)zego_value_get_double(paramMap[FTValue("max")]);

        int ret = rangeAudio_->setAudioReceiveRange(param);

        result->Success(FTValue(ret));
    } else {
        result->Error("rangeAudioSetAudioReceiveRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetMode(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto mode = zego_value_get_int(argument[FTValue("mode")]);

        rangeAudio_->setRangeAudioMode((EXPRESS::ZegoRangeAudioMode)mode);

        result->Success();
    } else {
        result->Error("rangeAudioSetMode_Can_not_find_instance",
                      "Invoke `rangeAudioSetMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetTeamID(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto teamID = zego_value_get_string(argument[FTValue("teamID")]);

        rangeAudio_->setTeamID(teamID);

        result->Success();
    } else {
        result->Error("rangeAudioSetTeamID_Can_not_find_instance",
                      "Invoke `rangeAudioSetTeamID` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto userID = zego_value_get_string(argument[FTValue("userID")]);
        auto position = zego_value_get_vector_float(argument[FTValue("position")]);

        rangeAudio_->updateAudioSource(userID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto position = zego_value_get_vector_float(argument[FTValue("position")]);
        auto axisForward = zego_value_get_vector_float(argument[FTValue("axisForward")]);
        auto axisRight = zego_value_get_vector_float(argument[FTValue("axisRight")]);
        auto axisUp = zego_value_get_vector_float(argument[FTValue("axisUp")]);

        rangeAudio_->updateSelfPosition(position.data(), axisForward.data(), axisRight.data(),
                                        axisUp.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioMuteUser(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto userID = zego_value_get_string(argument[FTValue("userID")]);
        auto mute = zego_value_get_bool(argument[FTValue("mute")]);

        rangeAudio_->muteUser(userID, mute);

        result->Success();
    } else {
        result->Error("rangeAudioMuteUser_Can_not_find_instance",
                      "Invoke `rangeAudioMuteUser` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto frequency = zego_value_get_int(argument[FTValue("frequency")]);

        rangeAudio_->setPositionUpdateFrequency(frequency);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetPositionUpdateFrequency_Can_not_find_instance",
            "Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioVolume(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto volume = zego_value_get_int(argument[FTValue("volume")]);

        rangeAudio_->setRangeAudioVolume(volume);

        result->Success();
    } else {
        result->Error("rangeAudioSetRangeAudioVolume_Can_not_find_instance",
                      "Invoke `rangeAudioSetRangeAudioVolume` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetStreamVocalRange(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);
        auto paramMap = zego_value_get_map(argument[FTValue("param")]);

        EXPRESS::ZegoVocalRangeParam param;
        param.min = (float)zego_value_get_double(paramMap[FTValue("min")]);
        param.max = (float)zego_value_get_double(paramMap[FTValue("max")]);

        int ret = rangeAudio_->setStreamVocalRange(streamID, param);

        result->Success(FTValue(ret));
    } else {
        result->Error("rangeAudioSetStreamVocalRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateStreamPosition(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        auto position = zego_value_get_vector_float(argument[FTValue("position")]);

        rangeAudio_->updateStreamPosition(streamID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateStreamPosition_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateStreamPosition` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioCustomMode(
    FTArgument &argument,
    FTResult result) {
    if (rangeAudio_) {
        auto speakMode =
            (EXPRESS::ZegoRangeAudioSpeakMode)zego_value_get_int(argument[FTValue("speakMode")]);

        auto listenMode =
            (EXPRESS::ZegoRangeAudioListenMode)zego_value_get_int(argument[FTValue("listenMode")]);

        rangeAudio_->setRangeAudioCustomMode(speakMode, listenMode);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetRangeAudioCustomMode_Can_not_find_instance",
            "Invoke `rangeAudioSetRangeAudioCustomMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager(
    FTArgument &argument,
    FTResult result) {
    auto roomID = zego_value_get_string(argument[FTValue("roomID")]);

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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyRealTimeSequentialDataManager(
            dataManagerMap_[index]);
        dataManagerMap_.erase(index);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto byteData = zego_value_get_vector_uint8(argument[FTValue("data")]);
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        auto sharedPtrResult =
            FTMoveResult(result);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStartBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartSubscribing(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStartSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStopBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopSubscribing(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStopSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::getScreenCaptureSources(
    FTArgument &argument,
    FTResult result) {
    auto thumbnailWidth = zego_value_get_int(argument[FTValue("thumbnailWidth")]);
    auto thumbnailHeight = zego_value_get_int(argument[FTValue("thumbnailHeight")]);
    auto iconWidth = zego_value_get_int(argument[FTValue("iconWidth")]);
    auto iconHeight = zego_value_get_int(argument[FTValue("iconHeight")]);

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
    FTArgument &argument,
    FTResult result) {
    void *sourceId =
        reinterpret_cast<void *>(static_cast<intptr_t>(argument[FTValue("sourceId")].LongValue()));
    auto sourceType = zego_value_get_int(argument[FTValue("sourceType")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyScreenCaptureSource(screenCaptureSource);
    }

    screenCaptureSourceMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCursorVisibleScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto visible = zego_value_get_bool(argument[FTValue("visible")]);
        screenCaptureSource->enableCursorVisible(visible);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableWindowActivateScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto active = zego_value_get_bool(argument[FTValue("active")]);
        screenCaptureSource->enableWindowActivate(active);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setExcludeWindowListScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto list = zego_value_get_list(argument[FTValue("list")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = zego_value_get_map(argument[FTValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = zego_value_get_double(rectMap[FTValue("x")]);
        rect.y = zego_value_get_double(rectMap[FTValue("y")]);
        rect.width = zego_value_get_double(rectMap[FTValue("width")]);
        rect.height = zego_value_get_double(rectMap[FTValue("height")]);
        screenCaptureSource->updateCaptureRegion(rect);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::updatePublishRegionScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = zego_value_get_map(argument[FTValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = zego_value_get_double(rectMap[FTValue("x")]);
        rect.y = zego_value_get_double(rectMap[FTValue("y")]);
        rect.width = zego_value_get_double(rectMap[FTValue("width")]);
        rect.height = zego_value_get_double(rectMap[FTValue("height")]);
        screenCaptureSource->updatePublishRegion(rect);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::updateCaptureSourceScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        void *sourceId = reinterpret_cast<void *>(
            static_cast<intptr_t>(argument[FTValue("sourceId")].LongValue()));
        auto sourceType = zego_value_get_int(argument[FTValue("sourceType")]);
        screenCaptureSource->updateCaptureSource(sourceId,
                                                 (EXPRESS::ZegoScreenCaptureSourceType)sourceType);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::startCaptureScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->startCapture();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopCaptureScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->stopCapture();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::getCaptureSourceRectScreenCaptureSource(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto enable = zego_value_get_bool(argument[FTValue("enable")]);
        auto param = zego_value_get_map(argument[FTValue("audioParam")]);

        EXPRESS::ZegoAudioFrameParam nativeParam;
        nativeParam.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(param[FTValue("sampleRate")]);
        nativeParam.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(param[FTValue("channel")]);

        screenCaptureSource->enableAudioCapture(enable, nativeParam);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAIVoiceChanger(
    FTArgument &argument,
    FTResult result) {
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
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->setEventHandler(nullptr);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyAIVoiceChanger(aiVoiceChanger);
    }

    aiVoiceChangerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAIVoiceChangerSupported(
    FTArgument &argument,
    FTResult result) {
    bool supported = EXPRESS::ZegoExpressSDK::getEngine()->isAIVoiceChangerSupported();
    result->Success(FTValue(supported));
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerInitEngine(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->initEngine();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerUpdate(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->update();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerSetSpeaker(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        auto speakerID = zego_value_get_int(argument[FTValue("speakerID")]);
        aiVoiceChanger->setSpeaker(speakerID);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerGetSpeakerList(
    FTArgument &argument,
    FTResult result) {
    auto index = zego_value_get_int(argument[FTValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->getSpeakerList();
    }

    result->Success();
}
void ZegoExpressEngineMethodHandler::enableColorEnhancement(
    FTArgument &argument,
    FTResult result) {
    auto enable = zego_value_get_bool(argument[FTValue("enable")]);
    auto params = zego_value_get_map(argument[FTValue("params")]);
    auto channel = zego_value_get_int(argument[FTValue("channel")]);

    EXPRESS::ZegoColorEnhancementParams p;
    p.intensity = zego_value_get_double(params[FTValue("intensity")]);
    p.skinToneProtectionLevel = zego_value_get_double(params[FTValue("skinToneProtectionLevel")]);
    p.lipColorProtectionLevel = zego_value_get_double(params[FTValue("lipColorProtectionLevel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableColorEnhancement(enable, p, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}
