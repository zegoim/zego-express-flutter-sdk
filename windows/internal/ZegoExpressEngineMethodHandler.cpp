#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoTextureRendererController.h"
#include "ZegoExpressEngineEventHandler.h"

#ifdef _WIN32
#include "DataToImageTools.hpp"
#include "zego_express_engine/ZegoCustomVideoCaptureManager.h"
#include "zego_express_engine/ZegoCustomVideoProcessManager.h"
#include "zego_express_engine/ZegoCustomVideoRenderManager.h"
#include "zego_express_engine/ZegoMediaPlayerAudioManager.h"
#include "zego_express_engine/ZegoMediaPlayerBlockDataManager.h"
#include "zego_express_engine/ZegoMediaPlayerVideoManager.h"
#include <flutter/encodable_value.h>
#include <flutter/plugin_registrar_windows.h>
#endif

#include <functional>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <variant>

#include "ZegoLog.h"
#include "ZegoUtils.h"

void ZegoExpressEngineMethodHandler::clearPluginRegistrar() {
    ZegoTextureRendererController::getInstance()->uninit();
}

void ZegoExpressEngineMethodHandler::initApiCalledCallback() {
    EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());
}

bool ZegoExpressEngineMethodHandler::isEngineCreated() {
    return EXPRESS::ZegoExpressSDK::getEngine();
}

void ZegoExpressEngineMethodHandler::getVersion(ZFArgument argument, ZFResult result) {
    result->Success(ZFValue(EXPRESS::ZegoExpressSDK::getVersion()));
}

void ZegoExpressEngineMethodHandler::isFeatureSupported(ZFArgument argument, ZFResult result) {
    auto featureType = zego_value_get_int(argument[ZFValue("featureType")]);
    result->Success(ZFValue(
        EXPRESS::ZegoExpressSDK::isFeatureSupported((EXPRESS::ZegoFeatureType)featureType)));
}

void ZegoExpressEngineMethodHandler::setPluginVersion(ZFArgument argument, ZFResult result) {
    std::string version = zego_value_get_string(argument[ZFValue("version")]);

    version = "*** Plugin Version: " + version;

    ZF::logInfo(version.c_str());

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomLogMessage(ZFArgument argument, ZFResult result) {
    std::string version = zego_value_get_string(argument[ZFValue("message")]);

    ZF::logInfo(version.c_str());

    result->Success();
}


void ZegoExpressEngineMethodHandler::getAssetAbsolutePath(ZFArgument argument, ZFResult result) {
    std::string assetPath = zego_value_get_string(argument[ZFValue("assetPath")]);
    std::string flutterAssetsPath = GetFlutterAssetsPath();
    if (!flutterAssetsPath.empty()) {
        assetPath = flutterAssetsPath + assetPath;
    } else {
        result->Error("getAssetAbsolutePath_get_exe_path_fail",
                      "Failed to get the directory where the application is located");
        return;
    }
    result->Success(ZFValue(assetPath));
}

void ZegoExpressEngineMethodHandler::createEngine(ZFArgument argument, ZFResult result) {
    initApiCalledCallback();

    EXPRESS::ZegoEngineConfig config;
    config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                             {"thirdparty_framework_info", "flutter"}};
    EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    // TODO: need to write getValue utils
    unsigned int appID = zego_value_get_long(argument[ZFValue("appID")]);
    std::string appSign = zego_value_get_string(argument[ZFValue("appSign")]);
    bool isTestEnv = zego_value_get_bool(argument[ZFValue("isTestEnv")]);
    int scenario = zego_value_get_int(argument[ZFValue("scenario")]);

    auto engine = EXPRESS::ZegoExpressSDK::createEngine(
        appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario,
        ZegoExpressEngineEventHandler::getInstance());

    engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());

    ZegoTextureRendererController::getInstance()->init(messenger_);

    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngineWithProfile(ZFArgument argument, ZFResult result) {
    ZFMap profileMap = zego_value_get_map(argument[ZFValue("profile")]);
    if (profileMap.size() > 0) {

        initApiCalledCallback();

        EXPRESS::ZegoEngineConfig config;
        config.advancedConfig = {{"video_data_image_colorspace", "rgba"},
                                 {"thirdparty_framework_info", "flutter"}};
        EXPRESS::ZegoExpressSDK::setEngineConfig(config);

        EXPRESS::ZegoEngineProfile profilePtr;
        profilePtr.appID = zego_value_get_long(profileMap[ZFValue("appID")]);
        if (!zego_value_is_null(profileMap[ZFValue("appSign")])) {
            profilePtr.appSign = zego_value_get_string(profileMap[ZFValue("appSign")]);
        }
        profilePtr.scenario =
            (EXPRESS::ZegoScenario)zego_value_get_int(profileMap[ZFValue("scenario")]);
        auto engine = EXPRESS::ZegoExpressSDK::createEngine(
            profilePtr, ZegoExpressEngineEventHandler::getInstance());

        engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());

        ZegoTextureRendererController::getInstance()->init(messenger_);
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::destroyEngine(ZFArgument argument, ZFResult result) {
    auto engine = EXPRESS::ZegoExpressSDK::getEngine();

    if (engine) {
        ZegoTextureRendererController::getInstance()->uninit();
        auto sharedPtrResult = ZFMoveResult(result);
        EXPRESS::ZegoExpressSDK::destroyEngine(engine, [=]() { sharedPtrResult->Success(); });
    } else {
        result->Success();
    }
}

void ZegoExpressEngineMethodHandler::setEngineConfig(ZFArgument argument, ZFResult result) {
    ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoEngineConfig config;

    if (configMap.size() > 0) {
        ZFMap logConfigMap = zego_value_get_map(configMap[ZFValue("logConfig")]);
        std::unique_ptr<EXPRESS::ZegoLogConfig> logConfigPtr = nullptr;
        if (logConfigMap.size() > 0) {
            logConfigPtr = std::make_unique<EXPRESS::ZegoLogConfig>();
            logConfigPtr->logPath = zego_value_get_string(logConfigMap[ZFValue("logPath")]);
            logConfigPtr->logSize = zego_value_get_int(logConfigMap[ZFValue("logSize")]);
            logConfigPtr->logCount = zego_value_get_int(logConfigMap[ZFValue("logCount")]);
        }

        config.logConfig = logConfigPtr.get();

        ZFMap advancedConfigMap = zego_value_get_map(configMap[ZFValue("advancedConfig")]);

        for (const auto &cfg : advancedConfigMap) {
            std::string key = zego_value_get_string(cfg.first);
            std::string value = zego_value_get_string(cfg.second);
            config.advancedConfig[key] = value;
        }

        EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLogConfig(ZFArgument argument, ZFResult result) {
    ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoLogConfig config;
    if (configMap.size() > 0) {
        config.logPath = zego_value_get_string(configMap[ZFValue("logPath")]);
        config.logSize = zego_value_get_int(configMap[ZFValue("logSize")]);
        config.logCount = zego_value_get_int(configMap[ZFValue("logCount")]);
        EXPRESS::ZegoExpressSDK::setLogConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLocalProxyConfig(ZFArgument argument, ZFResult result) {
    auto proxyListArray = zego_value_get_list(argument[ZFValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (const auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = zego_value_get_map(proxy_);
        proxy.ip = zego_value_get_string(proxyMap[ZFValue("ip")]);
        proxy.port = zego_value_get_int(proxyMap[ZFValue("port")]);
        proxy.hostName = zego_value_get_string(proxyMap[ZFValue("hostName")]);
        proxy.userName = zego_value_get_string(proxyMap[ZFValue("userName")]);
        proxy.password = zego_value_get_string(proxyMap[ZFValue("password")]);
        proxyList.push_back(proxy);
    }
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::setLocalProxyConfig(proxyList, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCloudProxyConfig(ZFArgument argument, ZFResult result) {
    auto proxyListArray = zego_value_get_list(argument[ZFValue("proxyList")]);
    std::vector<EXPRESS::ZegoProxyInfo> proxyList;
    for (const auto proxy_ : proxyListArray) {
        EXPRESS::ZegoProxyInfo proxy;
        auto proxyMap = zego_value_get_map(proxy_);
        proxy.ip = zego_value_get_string(proxyMap[ZFValue("ip")]);
        proxy.port = zego_value_get_int(proxyMap[ZFValue("port")]);
        proxy.hostName = zego_value_get_string(proxyMap[ZFValue("hostName")]);
        proxy.userName = zego_value_get_string(proxyMap[ZFValue("userName")]);
        proxy.password = zego_value_get_string(proxyMap[ZFValue("password")]);
        proxyList.push_back(proxy);
    }
    auto token = zego_value_get_string(argument[ZFValue("token")]);

    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::setCloudProxyConfig(proxyList, token, enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomMode(ZFArgument argument, ZFResult result) {
    auto mode = (EXPRESS::ZegoRoomMode)zego_value_get_int(argument[ZFValue("mode")]);
    EXPRESS::ZegoExpressSDK::setRoomMode(mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLicense(ZFArgument argument, ZFResult result) {
    auto license = zego_value_get_string(argument[ZFValue("license")]);
    EXPRESS::ZegoExpressSDK::setLicense(license);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setGeoFence(ZFArgument argument, ZFResult result) {
    auto type = (EXPRESS::ZegoGeoFenceType)zego_value_get_int(argument[ZFValue("type")]);
    auto areaList_ = zego_value_get_list(argument[ZFValue("areaList")]);
    std::vector<int> areaList;
    for (auto area : areaList_) {
        areaList.push_back(zego_value_get_int(area));
    }
    EXPRESS::ZegoExpressSDK::setGeoFence(type, areaList);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomScenario(ZFArgument argument, ZFResult result) {
    auto scenario = (EXPRESS::ZegoScenario)zego_value_get_int(argument[ZFValue("scenario")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomScenario(scenario);

    result->Success();
}

void ZegoExpressEngineMethodHandler::uploadLog(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->uploadLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::submitLog(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::submitLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableDebugAssistant(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableDebugAssistant(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::callExperimentalAPI(ZFArgument argument, ZFResult result) {
    auto params = zego_value_get_string(argument[ZFValue("params")]);
    auto resultStr = EXPRESS::ZegoExpressSDK::getEngine()->callExperimentalAPI(params);
    result->Success(ZFValue(resultStr));
}

void ZegoExpressEngineMethodHandler::setDummyCaptureImagePath(ZFArgument argument,
                                                              ZFResult result) {
    auto filePath = zego_value_get_string(argument[ZFValue("filePath")]);
    const std::string flutterAssertTaget = "flutter-asset://";
    if (filePath.compare(0, flutterAssertTaget.size(), flutterAssertTaget) == 0) {
        filePath.replace(0, flutterAssertTaget.size(), "");
        std::string flutterAssetsPath = GetFlutterAssetsPath();
        if (!flutterAssetsPath.empty()) {
            filePath = flutterAssetsPath + filePath;
        } else {
            result->Error("setDummyCaptureImagePath_get_exe_path_fail",
                          "Failed to get the directory where the application is located");
            return;
        }
    }
    auto channel = (EXPRESS::ZegoPublishChannel)zego_value_get_int(argument[ZFValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setDummyCaptureImagePath(filePath, channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::loginRoom(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto userMap = zego_value_get_map(argument[ZFValue("user")]);

    EXPRESS::ZegoUser user{zego_value_get_string(userMap[ZFValue("userID")]),
                           zego_value_get_string(userMap[ZFValue("userName")])};

    ZFMap configMap;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        configMap = zego_value_get_map(argument[ZFValue("config")]);
    }

    EXPRESS::ZegoRoomConfig config;
    if (configMap.size() > 0) {
        config.maxMemberCount =
            (unsigned int)zego_value_get_int(configMap[ZFValue("maxMemberCount")]);
        config.isUserStatusNotify = zego_value_get_bool(configMap[ZFValue("isUserStatusNotify")]);
        config.token = zego_value_get_string(configMap[ZFValue("token")]);
        if (!zego_value_is_null(configMap[ZFValue("capabilityNegotiationTypes")])) {
            config.capabilityNegotiationTypes = zego_value_get_long(configMap[ZFValue("capabilityNegotiationTypes")]);
        }
    }
    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->loginRoom(
        roomID, user, config, [=](int errorCode, std::string extendedData) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            if (extendedData.empty()) {
                retMap[ZFValue("extendedData")] = ZFValue("{}");
            } else {
                retMap[ZFValue("extendedData")] = ZFValue(extendedData);
            }
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::logoutRoom(ZFArgument argument, ZFResult result) {
    auto sharedPtrResult = ZFMoveResult(result);
    if (!zego_value_is_null(argument[ZFValue("roomID")])) {
        auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);

        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(
            roomID, [=](int errorCode, std::string extendedData) {
                ZFMap retMap;

                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                if (extendedData.empty()) {
                    retMap[ZFValue("extendedData")] = ZFValue("{}");
                } else {
                    retMap[ZFValue("extendedData")] = ZFValue(extendedData);
                }

                sharedPtrResult->Success(retMap);
            });
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(
            [=](int errorCode, std::string extendedData) {
                ZFMap retMap;

                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                if (extendedData.empty()) {
                    retMap[ZFValue("extendedData")] = ZFValue("{}");
                } else {
                    retMap[ZFValue("extendedData")] = ZFValue(extendedData);
                }

                sharedPtrResult->Success(retMap);
            });
    }
}

void ZegoExpressEngineMethodHandler::switchRoom(ZFArgument argument, ZFResult result) {
    auto fromRoomID = zego_value_get_string(argument[ZFValue("fromRoomID")]);
    auto toRoomID = zego_value_get_string(argument[ZFValue("toRoomID")]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = zego_value_get_long(configMap[ZFValue("maxMemberCount")]);
            configPtr->isUserStatusNotify =
                zego_value_get_bool(configMap[ZFValue("isUserStatusNotify")]);
            configPtr->token = zego_value_get_string(configMap[ZFValue("token")]);
            if (!zego_value_is_null(configMap[ZFValue("capabilityNegotiationTypes")])) {
                configPtr->capabilityNegotiationTypes = zego_value_get_long(configMap[ZFValue("capabilityNegotiationTypes")]);
            }
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());

    result->Success();
}

void ZegoExpressEngineMethodHandler::renewToken(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto token = zego_value_get_string(argument[ZFValue("token")]);

    EXPRESS::ZegoExpressSDK::getEngine()->renewToken(roomID, token);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto key = zego_value_get_string(argument[ZFValue("key")]);
    auto value = zego_value_get_string(argument[ZFValue("value")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [=](int errorCode) {
        ZFMap retMap;
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::getRoomStreamList(ZFArgument argument, ZFResult result) {

    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);

    auto stream_list_type = zego_value_get_int(argument[ZFValue("streamListType")]);

    EXPRESS::ZegoRoomStreamList streamList =
        EXPRESS::ZegoExpressSDK::getEngine()->getRoomStreamList(
            roomID, (EXPRESS::ZegoRoomStreamListType)stream_list_type);

    ZFMap retMap;

    ZFArray streamListPlayArray;
    for (auto &stream : streamList.playStreamList) {
        ZFMap streamMap;
        ZFMap userMap;
        userMap[ZFValue("userID")] = ZFValue(stream.user.userID);
        userMap[ZFValue("userName")] = ZFValue(stream.user.userName);

        streamMap[ZFValue("streamID")] = ZFValue(stream.streamID);
        streamMap[ZFValue("extraInfo")] = ZFValue(stream.extraInfo);
        streamMap[ZFValue("user")] = ZFValue(userMap);

        streamListPlayArray.emplace_back(ZFValue(streamMap));
    }

    retMap[ZFValue("playStreamList")] = ZFValue(streamListPlayArray);

    ZFArray streamListPublishArray;
    for (auto &stream : streamList.publishStreamList) {
        ZFMap streamMap;
        ZFMap userMap;
        userMap[ZFValue("userID")] = ZFValue(stream.user.userID);
        userMap[ZFValue("userName")] = ZFValue(stream.user.userName);

        streamMap[ZFValue("streamID")] = ZFValue(stream.streamID);
        streamMap[ZFValue("extraInfo")] = ZFValue(stream.extraInfo);
        streamMap[ZFValue("user")] = ZFValue(userMap);

        streamListPublishArray.emplace_back(ZFValue(streamMap));
    }

    retMap[ZFValue("publishStreamList")] = ZFValue(streamListPublishArray);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::startPublishingStream(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    if (!zego_value_is_null(argument[ZFValue("config")])) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);

        if (configMap.size() > 0) {
            EXPRESS::ZegoPublisherConfig config;

            config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
            config.forceSynchronousNetworkTime =
                zego_value_get_int(configMap[ZFValue("forceSynchronousNetworkTime")]);
            config.streamCensorshipMode = (EXPRESS::ZegoStreamCensorshipMode)zego_value_get_int(
                configMap[ZFValue("streamCensorshipMode")]);

            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(
                streamID, config, (EXPRESS::ZegoPublishChannel)channel);
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(
                streamID, (EXPRESS::ZegoPublishChannel)channel);
        }
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPublishingStream(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream(
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(ZFArgument argument, ZFResult result) {
    auto extraInfo = zego_value_get_string(argument[ZFValue("extraInfo")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->setStreamExtraInfo(
        extraInfo,
        [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);

            sharedPtrResult->Success(retMap);
        },
        (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::startPreview(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    ZFMap canvasMap;
    if (!zego_value_is_null(argument[ZFValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[ZFValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[ZFValue("viewMode")]);
        int64_t viewID = zego_value_get_long(canvasMap[ZFValue("view")]);
        auto alphaBlend = zego_value_get_bool(canvasMap[ZFValue("alphaBlend")]);
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

void ZegoExpressEngineMethodHandler::stopPreview(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);
    ZegoTextureRendererController::getInstance()->removeCapturedRenderer(
        (EXPRESS::ZegoPublishChannel)channel);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoConfig(ZFArgument argument, ZFResult result) {
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoVideoConfig config;
    config.bitrate = zego_value_get_int(configMap[ZFValue("bitrate")]);
    config.captureHeight = zego_value_get_int(configMap[ZFValue("captureHeight")]);
    config.captureWidth = zego_value_get_int(configMap[ZFValue("captureWidth")]);
    auto codecIDIndex = zego_value_get_int(configMap[ZFValue("codecID")]);
    config.codecID = (EXPRESS::ZegoVideoCodecID)(
        codecIDIndex > 4 ? EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN : codecIDIndex);
    config.encodeHeight = zego_value_get_int(configMap[ZFValue("encodeHeight")]);
    config.encodeWidth = zego_value_get_int(configMap[ZFValue("encodeWidth")]);
    config.fps = zego_value_get_int(configMap[ZFValue("fps")]);
    config.keyFrameInterval = zego_value_get_int(configMap[ZFValue("keyFrameInterval")]);

    auto channel = zego_value_get_int(argument[ZFValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setVideoConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoConfig(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);
    auto config =
        EXPRESS::ZegoExpressSDK::getEngine()->getVideoConfig((EXPRESS::ZegoPublishChannel)channel);

    ZFMap configMap;
    configMap[ZFValue("bitrate")] = ZFValue(config.bitrate);
    configMap[ZFValue("captureHeight")] = ZFValue(config.captureHeight);
    configMap[ZFValue("captureWidth")] = ZFValue(config.captureWidth);
    configMap[ZFValue("codecID")] = ZFValue((int32_t)config.codecID);
    configMap[ZFValue("encodeHeight")] = ZFValue(config.encodeHeight);
    configMap[ZFValue("encodeWidth")] = ZFValue(config.encodeWidth);
    configMap[ZFValue("fps")] = ZFValue(config.fps);
    configMap[ZFValue("keyFrameInterval")] = ZFValue(config.keyFrameInterval);

    result->Success(configMap);
}

void ZegoExpressEngineMethodHandler::setPublishDualStreamConfig(ZFArgument argument,
                                                                ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    std::vector<EXPRESS::ZegoPublishDualStreamConfig> configList;
    auto configListMap = zego_value_get_list(argument[ZFValue("configList")]);
    for (auto config_ : configListMap) {
        ZFMap configMap = zego_value_get_map(config_);
        EXPRESS::ZegoPublishDualStreamConfig config;

        config.encodeWidth = zego_value_get_int(configMap[ZFValue("encodeWidth")]);
        config.encodeHeight = zego_value_get_int(configMap[ZFValue("encodeHeight")]);
        config.fps = zego_value_get_int(configMap[ZFValue("fps")]);
        config.bitrate = zego_value_get_int(configMap[ZFValue("bitrate")]);
        config.streamType =
            (EXPRESS::ZegoVideoStreamType)zego_value_get_int(configMap[ZFValue("streamType")]);

        configList.push_back(config);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishDualStreamConfig(
        configList, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoMirrorMode(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    auto mirrorMode =
        (EXPRESS::ZegoVideoMirrorMode)zego_value_get_int(argument[ZFValue("mirrorMode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVideoMirrorMode(mirrorMode,
                                                             (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = zego_value_get_int(configMap[ZFValue("bitrate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[ZFValue("channel")]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)zego_value_get_int(configMap[ZFValue("codecID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(ZFArgument argument, ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    auto config =
        EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig((EXPRESS::ZegoPublishChannel)channel);
    ZFMap retMap;
    retMap[ZFValue("bitrate")] = ZFValue(config.bitrate);
    retMap[ZFValue("channel")] = ZFValue(config.channel);
    retMap[ZFValue("codecID")] = ZFValue(config.codecID);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey(ZFArgument argument,
                                                                   ZFResult result) {
    auto key = zego_value_get_string(argument[ZFValue("key")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishStreamEncryptionKey(
        key, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePublishStreamSnapshot(ZFArgument argument,
                                                               ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->takePublishStreamSnapshot(
        [=](int errorCode, void *image) {
            std::vector<uint8_t> raw_image;
            if (image) {
#ifdef _WIN32
                auto tmpData = CreateFromHBITMAP((HBITMAP)image);
                raw_image.assign(tmpData.second, tmpData.second + tmpData.first);
                delete[] tmpData.second;
#endif
            }

            ZFMap resultMap;
            resultMap[ZFValue("errorCode")] = ZFValue(errorCode);
            resultMap[ZFValue("image")] = ZFValue(raw_image);
            sharedPtrResult->Success(resultMap);
        },
        (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePublishStreamVideo(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamVideo(
        mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(ZFArgument argument, ZFResult result) {
    auto volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(ZFArgument argument,
                                                               ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode(
        (EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(ZFArgument argument, ZFResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(),
                                                  (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendAudioSideInfo(ZFArgument argument, ZFResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
    auto timeStampMs = zego_value_get_double(argument[ZFValue("timeStampMs")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendAudioSideInfo(
        byteData.data(), (unsigned int)byteData.size(), timeStampMs,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCapturePipelineScaleMode(ZFArgument argument,
                                                                 ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCapturePipelineScaleMode(
        (EXPRESS::ZegoCapturePipelineScaleMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableH265EncodeFallback(ZFArgument argument,
                                                              ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableH265EncodeFallback(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoEncoderSupported(ZFArgument argument, ZFResult result) {
    auto codecID = zego_value_get_int(argument[ZFValue("codecID")]);
    if (codecID > 4) {
        codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
    }

    int ret = 0;
    if (!zego_value_is_null(argument[ZFValue("codecBackend")])) {
        auto codecBackend = zego_value_get_int(argument[ZFValue("codecBackend")]);
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoEncoderSupported(
            (EXPRESS::ZegoVideoCodecID)codecID, (EXPRESS::ZegoVideoCodecBackend)codecBackend);
    } else {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoEncoderSupported(
                  (EXPRESS::ZegoVideoCodecID)codecID)
                  ? 1
                  : 0;
    }

    result->Success(ZFValue(ret));
}

void ZegoExpressEngineMethodHandler::setLowlightEnhancement(ZFArgument argument, ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setLowlightEnhancement(
        (EXPRESS::ZegoLowlightEnhancementMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoSource(ZFArgument argument, ZFResult result) {
    auto source = zego_value_get_int(argument[ZFValue("source")]);

    bool hasInstanceID = false;
    int instanceID = -1;
    if (!zego_value_is_null(argument[ZFValue("instanceID")])) {
        hasInstanceID = true;
        instanceID = zego_value_get_int(argument[ZFValue("instanceID")]);
    }

    bool hasChannel = false;
    int channel = 0;
    if (!zego_value_is_null(argument[ZFValue("channel")])) {
        hasChannel = true;
        channel = zego_value_get_int(argument[ZFValue("channel")]);
    }

    ZegoTextureRendererController::getInstance()->setVideoSourceChannel(
        (EXPRESS::ZegoPublishChannel)channel, (EXPRESS::ZegoVideoSourceType)source);

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

    result->Success(ZFValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioSource(ZFArgument argument, ZFResult result) {
    auto source = zego_value_get_int(argument[ZFValue("source")]);

    bool hasConfig = false;
    EXPRESS::ZegoAudioSourceMixConfig config;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        hasConfig = true;
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);

        auto audioEffectPlayerIndexList =
            zego_value_get_list(configMap[ZFValue("audioEffectPlayerIndexList")]);
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

        auto mediaPlayerIndexList = zego_value_get_list(configMap[ZFValue("mediaPlayerIndexList")]);
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
            zego_value_get_bool(configMap[ZFValue("enableMixEnginePlayout")]);
        config.enableMixSystemPlayout =
            zego_value_get_bool(configMap[ZFValue("enableMixSystemPlayout")]);
    }

    bool hasChannel = false;
    int channel = -1;
    if (!zego_value_is_null(argument[ZFValue("channel")])) {
        hasChannel = true;
        channel = zego_value_get_int(argument[ZFValue("channel")]);
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

    result->Success(ZFValue(ret));
}

void ZegoExpressEngineMethodHandler::enableVideoObjectSegmentation(ZFArgument argument,
                                                                   ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoObjectSegmentationConfig config;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);

        auto type = zego_value_get_int(configMap[ZFValue("objectSegmentationType")]);
        config.objectSegmentationType = (EXPRESS::ZegoObjectSegmentationType)type;

        if (!zego_value_is_null(configMap[ZFValue("backgroundConfig")])) {
            auto backgroundConfigMap = zego_value_get_map(configMap[ZFValue("backgroundConfig")]);

            EXPRESS::ZegoBackgroundConfig backgroundConfig;
            backgroundConfig.processType = (EXPRESS::ZegoBackgroundProcessType)zego_value_get_int(
                backgroundConfigMap[ZFValue("processType")]);
            backgroundConfig.blurLevel = (EXPRESS::ZegoBackgroundBlurLevel)zego_value_get_int(
                backgroundConfigMap[ZFValue("blurLevel")]);
            // Note：注意这里巨坑，如果这里使用std::get<int64_t>处理传过来的0时会导致crash, 怀疑是flutter内部的bug
            backgroundConfig.color = zego_value_get_long(backgroundConfigMap[ZFValue("color")]);
            backgroundConfig.imageURL =
                zego_value_get_string(backgroundConfigMap[ZFValue("imageURL")]);
            backgroundConfig.videoURL =
                zego_value_get_string(backgroundConfigMap[ZFValue("videoURL")]);

            config.backgroundConfig = backgroundConfig;
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->enableVideoObjectSegmentation(
        enable, config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlphaChannelVideoEncoder(ZFArgument argument,
                                                                    ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto alphaLayout = zego_value_get_int(argument[ZFValue("alphaLayout")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAlphaChannelVideoEncoder(
        enable, (EXPRESS::ZegoAlphaLayoutType)alphaLayout, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    ZFMap canvasMap;
    if (!zego_value_is_null(argument[ZFValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[ZFValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[ZFValue("viewMode")]);
        auto viewID = zego_value_get_long(canvasMap[ZFValue("view")]);
        auto alphaBlend = zego_value_get_bool(canvasMap[ZFValue("alphaBlend")]);

        ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
        ZegoTextureRendererController::getInstance()->addRemoteRenderer(viewID, streamID, viewMode);
        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }

    ZFMap configMap;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        configMap = zego_value_get_map(argument[ZFValue("config")]);
    }

    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode =
            (EXPRESS::ZegoStreamResourceMode)zego_value_get_int(configMap[ZFValue("resourceMode")]);
        auto videoCodecIDIndex = zego_value_get_int(configMap[ZFValue("videoCodecID")]);
        if (videoCodecIDIndex > 4) {
            config.videoCodecID = EXPRESS::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
        } else {
            config.videoCodecID = (EXPRESS::ZegoVideoCodecID)videoCodecIDIndex;
        }

        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.sourceResourceType =
            (EXPRESS::ZegoResourceType)zego_value_get_int(configMap[ZFValue("sourceResourceType")]);
        config.codecTemplateID =
            (EXPRESS::ZegoResourceType)zego_value_get_int(configMap[ZFValue("codecTemplateID")]);
        config.resourceSwitchMode = (EXPRESS::ZegoStreamResourceSwitchMode)zego_value_get_int(
            configMap[ZFValue("resourceSwitchMode")]);
        config.resourceWhenStopPublish = (EXPRESS::ZegoStreamResourceType)zego_value_get_int(
            configMap[ZFValue("resourceWhenStopPublish")]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (!zego_value_is_null(configMap[ZFValue("cdnConfig")])) {
            auto cdnConfigMap = zego_value_get_map(configMap[ZFValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = zego_value_get_string(cdnConfigMap[ZFValue("url")]);
                cdnConfigPtr->authParam = zego_value_get_string(cdnConfigMap[ZFValue("authParam")]);
                cdnConfigPtr->customParams = zego_value_get_string(cdnConfigMap[ZFValue("customParams")]);
                cdnConfigPtr->protocol = zego_value_get_string(cdnConfigMap[ZFValue("protocol")]);
                cdnConfigPtr->quicVersion =
                    zego_value_get_string(cdnConfigMap[ZFValue("quicVersion")]);
                cdnConfigPtr->httpdns =
                    (EXPRESS::ZegoHttpDNSType)zego_value_get_int(cdnConfigMap[ZFValue("httpdns")]);

                cdnConfigPtr->quicConnectMode =
                    (int)zego_value_get_int(cdnConfigMap[ZFValue("quicConnectMode")]);
            }
        }

        config.cdnConfig = cdnConfigPtr.get();

        config.adaptiveSwitch = zego_value_get_int(configMap[ZFValue("adaptiveSwitch")]);
        auto adaptiveList = zego_value_get_list(configMap[ZFValue("adaptiveTemplateIDList")]);
        for (const auto adaptive : adaptiveList) {
            config.adaptiveTemplateIDList.push_back(zego_value_get_int(adaptive));
        }

        if (!zego_value_is_null(configMap[ZFValue("customResourceConfig")])) {
            auto customResourceConfigMap = zego_value_get_map(configMap[ZFValue("customResourceConfig")]);
            if (customResourceConfigMap.size() > 0) {
                config.customResourceConfig.beforePublish = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("beforePublish")]);
                config.customResourceConfig.publishing = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("publishing")]);
                config.customResourceConfig.afterPublish = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("afterPublish")]);
            }
        }

        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr, config);
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::switchPlayingStream(ZFArgument argument, ZFResult result) {
    auto fromStreamID = zego_value_get_string(argument[ZFValue("fromStreamID")]);
    auto toStreamID = zego_value_get_string(argument[ZFValue("toStreamID")]);

    ZFMap configMap;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        configMap = zego_value_get_map(argument[ZFValue("config")]);
    }

    EXPRESS::ZegoPlayerConfig config;
    std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr;
    if (configMap.size() > 0) {
        config.resourceMode =
            (EXPRESS::ZegoStreamResourceMode)zego_value_get_int(configMap[ZFValue("resourceMode")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.resourceSwitchMode = (EXPRESS::ZegoStreamResourceSwitchMode)zego_value_get_int(configMap[ZFValue("resourceSwitchMode")]);

        if (!zego_value_is_null(configMap[ZFValue("cdnConfig")])) {
            auto cdnConfigMap = zego_value_get_map(configMap[ZFValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = zego_value_get_string(cdnConfigMap[ZFValue("url")]);
                cdnConfigPtr->authParam = zego_value_get_string(cdnConfigMap[ZFValue("authParam")]);
                cdnConfigPtr->customParams = zego_value_get_string(cdnConfigMap[ZFValue("customParams")]);
                cdnConfigPtr->protocol = zego_value_get_string(cdnConfigMap[ZFValue("protocol")]);
                cdnConfigPtr->quicVersion =
                    zego_value_get_string(cdnConfigMap[ZFValue("quicVersion")]);
                cdnConfigPtr->httpdns =
                    (EXPRESS::ZegoHttpDNSType)zego_value_get_int(cdnConfigMap[ZFValue("httpdns")]);

                cdnConfigPtr->quicConnectMode = (int)zego_value_get_int(cdnConfigMap[ZFValue("quicConnectMode")]);

                config.cdnConfig = cdnConfigPtr.get();
            }
        }

        if (!zego_value_is_null(configMap[ZFValue("customResourceConfig")])) {
            auto customResourceConfigMap = zego_value_get_map(configMap[ZFValue("customResourceConfig")]);
            if (customResourceConfigMap.size() > 0) {
                config.customResourceConfig.beforePublish = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("beforePublish")]);
                config.customResourceConfig.publishing = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("publishing")]);
                config.customResourceConfig.afterPublish = (EXPRESS::ZegoResourceType)zego_value_get_int(customResourceConfigMap[ZFValue("afterPublish")]);
            }
        } 
    } 

    EXPRESS::ZegoExpressSDK::getEngine()->switchPlayingStream(fromStreamID, toStreamID, config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPlayingStream(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo(ZFArgument argument,
                                                               ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    ZFMap infoMap = zego_value_get_map(argument[ZFValue("info")]);
    EXPRESS::ZegoCrossAppInfo info;
    info.appID = zego_value_get_long(infoMap[ZFValue("appID")]);
    info.token = zego_value_get_string(infoMap[ZFValue("token")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamCrossAppInfo(streamID, info);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePlayStreamSnapshot(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->takePlayStreamSnapshot(
        streamID, [=](int errorCode, void *image) {
            // TODO : prevent crash
            std::vector<uint8_t> raw_image;
            if (image) {
#ifdef _WIN32
                auto tmpData = CreateFromHBITMAP((HBITMAP)image);
                raw_image.assign(tmpData.second, tmpData.second + tmpData.first);
                delete[] tmpData.second;
#endif
            }

            ZFMap resultMap;
            resultMap[ZFValue("errorCode")] = ZFValue(errorCode);
            resultMap[ZFValue("image")] = ZFValue(raw_image);
            sharedPtrResult->Success(resultMap);
        });
}

void ZegoExpressEngineMethodHandler::setPlayVolume(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(ZFArgument argument, ZFResult result) {
    auto volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayAudioStreams(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayAudioStreams(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(ZFArgument argument, ZFResult result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(ZFValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(ZFArgument argument, ZFResult result) {
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(ZFValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(ZFArgument argument, ZFResult result) {
    auto type = zego_value_get_int(argument[ZFValue("type")]);

    ZFArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceList(
        (EXPRESS::ZegoAudioDeviceType)type);
    for (auto &deviceInfo : deviceList) {
        ZFMap deviceMap;
        deviceMap[ZFValue("deviceID")] = ZFValue(deviceInfo.deviceID);
        deviceMap[ZFValue("deviceName")] = ZFValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(ZFValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID(ZFArgument argument, ZFResult result) {
    auto type = zego_value_get_int(argument[ZFValue("type")]);
    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultAudioDeviceID(
        (EXPRESS::ZegoAudioDeviceType)type);

    result->Success(ZFValue(deviceID));
}

void ZegoExpressEngineMethodHandler::useAudioDevice(ZFArgument argument, ZFResult result) {
    auto type = zego_value_get_int(argument[ZFValue("type")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);
    EXPRESS::ZegoExpressSDK::getEngine()->useAudioDevice((EXPRESS::ZegoAudioDeviceType)type,
                                                         deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(ZFArgument argument, ZFResult result) {
    ZFMap configMap;
    if (!zego_value_is_null(argument[ZFValue("config")])) {
        configMap = zego_value_get_map(argument[ZFValue("config")]);
    }

    if (configMap.size() > 0) {
        EXPRESS::ZegoSoundLevelConfig config;
        config.millisecond = zego_value_get_int(configMap[ZFValue("millisecond")]);
        config.enableVAD = zego_value_get_bool(configMap[ZFValue("enableVAD")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startSoundLevelMonitor(config);
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopSoundLevelMonitor(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopSoundLevelMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneMonitor(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(ZFArgument argument,
                                                               ZFResult result) {
    int volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(ZFArgument argument, ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(ZFArgument argument, ZFResult result) {
    int mode = zego_value_get_int(argument[ZFValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableSpeechEnhance(ZFArgument argument, ZFResult result) {

    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    int level = zego_value_get_int(argument[ZFValue("level")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableSpeechEnhance(enable, level);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(ZFArgument argument, ZFResult result) {
    auto bandIndex = zego_value_get_int(argument[ZFValue("bandIndex")]);
    auto bandGain = zego_value_get_double(argument[ZFValue("bandGain")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(ZFArgument argument, ZFResult result) {
    auto preset = zego_value_get_int(argument[ZFValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset(
        (EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(ZFArgument argument, ZFResult result) {
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)zego_value_get_double(paramMap[ZFValue("pitch")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(ZFArgument argument, ZFResult result) {
    auto preset = zego_value_get_int(argument[ZFValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(ZFArgument argument, ZFResult result) {
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoReverbAdvancedParam param;
    param.damping = (float)zego_value_get_double(paramMap[ZFValue("damping")]);
    param.roomSize = (float)zego_value_get_double(paramMap[ZFValue("roomSize")]);
    param.reverberance = (float)zego_value_get_double(paramMap[ZFValue("reverberance")]);
    param.wetOnly = zego_value_get_bool(paramMap[ZFValue("wetOnly")]);
    param.wetGain = (float)zego_value_get_double(paramMap[ZFValue("wetGain")]);
    param.dryGain = (float)zego_value_get_double(paramMap[ZFValue("dryGain")]);
    param.toneLow = (float)zego_value_get_double(paramMap[ZFValue("toneLow")]);
    param.toneHigh = (float)zego_value_get_double(paramMap[ZFValue("toneHigh")]);
    param.preDelay = (float)zego_value_get_double(paramMap[ZFValue("preDelay")]);
    param.stereoWidth = (float)zego_value_get_double(paramMap[ZFValue("stereoWidth")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbAdvancedParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbEchoParam(ZFArgument argument, ZFResult result) {
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)zego_value_get_double(paramMap[ZFValue("inGain")]);
    param.outGain = (float)zego_value_get_double(paramMap[ZFValue("outGain")]);
    param.numDelays = zego_value_get_int(paramMap[ZFValue("numDelays")]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    ZFArray delayVec = zego_value_get_list(paramMap[ZFValue("delay")]);
    ZFArray decayVec = zego_value_get_list(paramMap[ZFValue("decay")]);

    for (int i = 0; i < param.numDelays; i++) {
        param.delay[i] = zego_value_get_int(delayVec.at(i));
        param.decay[i] = (float)zego_value_get_double(decayVec.at(i));
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbEchoParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVirtualStereo(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto angle = zego_value_get_int(argument[ZFValue("angle")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo(ZFArgument argument,
                                                                   ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto angle = zego_value_get_int(argument[ZFValue("angle")]);
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePlayStreamVirtualStereo(enable, angle, streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setElectronicEffects(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);
    auto tonal = zego_value_get_int(argument[ZFValue("tonal")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setElectronicEffects(
        enable, (ZEGO::EXPRESS::ZegoElectronicEffectsMode)mode, tonal);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioDataObserver(ZFArgument argument, ZFResult result) {
    auto bitmask = zego_value_get_int(argument[ZFValue("observerBitMask")]);
    auto param = zego_value_get_map(argument[ZFValue("param")]);

    EXPRESS::ZegoAudioFrameParam nativeParam;
    nativeParam.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(param[ZFValue("sampleRate")]);
    nativeParam.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(param[ZFValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDataObserver(bitmask, nativeParam);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDataObserver(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDataObserver();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAudioEffectPlayer(ZFArgument argument, ZFResult result) {
    auto player = EXPRESS::ZegoExpressSDK::getEngine()->createAudioEffectPlayer();
    if (player) {

        int index = player->getIndex();
        player->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        audioEffectPlayerMap_[index] = player;

        result->Success(ZFValue(index));

    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
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

void ZegoExpressEngineMethodHandler::audioEffectPlayerStart(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        std::string path = zego_value_get_string(argument[ZFValue("path")]);

        ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);
        std::unique_ptr<EXPRESS::ZegoAudioEffectPlayConfig> configPtr = nullptr;

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoAudioEffectPlayConfig>();
            configPtr->playCount = zego_value_get_int(configMap[ZFValue("playCount")]);
            configPtr->isPublishOut = zego_value_get_bool(configMap[ZFValue("isPublishOut")]);
        }

        player->start(audioEffectID, path, configPtr.get());

        result->Success();
    } else {
        result->Error("audioEffectPlayerStart_Can_not_find_player",
                      "Invoke `audioEffectPlayerStart` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStop(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        player->stop(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerStop_Can_not_find_player",
                      "Invoke `audioEffectPlayerStop` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPause(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        player->pause(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerPause_Can_not_find_player",
                      "Invoke `audioEffectPlayerPause` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResume(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        player->resume(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerResume_Can_not_find_player",
                      "Invoke `audioEffectPlayerResume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->stopAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerStopAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerStopAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll(ZFArgument argument,
                                                               ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->pauseAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerPauseAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerPauseAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->resumeAll();

        result->Success();
    } else {
        result->Error("audioEffectPlayerResumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerResumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        unsigned long long millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);

        auto sharedPtrResult = ZFMoveResult(result);
        player->seekTo(audioEffectID, millisecond, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("audioEffectPlayerSeekTo_Can_not_find_player",
                      "Invoke `audioEffectPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolume(ZFArgument argument,
                                                                    ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setPlayVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolume(ZFArgument argument,
                                                                       ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setPublishVolume(audioEffectID, volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPublishVolume_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPublishVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlayVolumeAll(ZFArgument argument,
                                                                       ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setPlayVolumeAll(volume);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlayVolumeAll_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlayVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPublishVolumeAll(ZFArgument argument,
                                                                          ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = zego_value_get_int(argument[ZFValue("volume")]);
        player->setPublishVolumeAll(volume);

        result->Success();
    } else {
        result->Error(
            "audioEffectPlayerSetPublishVolumeAll_Can_not_find_player",
            "Invoke `audioEffectPlayerSetPublishVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration(ZFArgument argument,
                                                                       ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        auto totalDuration = player->getTotalDuration(audioEffectID);

        result->Success(ZFValue((int64_t)totalDuration));
    } else {
        result->Error("audioEffectPlayerGetTotalDuration_Can_not_find_player",
                      "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress(ZFArgument argument,
                                                                         ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        auto currentProgress = player->getCurrentProgress(audioEffectID);

        result->Success(ZFValue((int64_t)currentProgress));
    } else {
        result->Error(
            "audioEffectPlayerGetCurrentProgress_Can_not_find_player",
            "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        std::string path = zego_value_get_string(argument[ZFValue("path")]);

        auto sharedPtrResult = ZFMoveResult(result);
        player->loadResource(audioEffectID, path, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("audioEffectPlayerLoadResource_Can_not_find_player",
                      "Invoke `audioEffectPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource(ZFArgument argument,
                                                                     ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        player->unloadResource(audioEffectID);

        result->Success();
    } else {
        result->Error("audioEffectPlayerUnLoadResource_Can_not_find_player",
                      "Invoke `audioEffectPlayerUnloadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        auto speed = zego_value_get_double(argument[ZFValue("speed")]);
        player->setPlaySpeed(audioEffectID, speed);

        result->Success();
    } else {
        result->Error("audioEffectPlayerSetPlaySpeed_Can_not_find_player",
                      "Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUpdatePosition(ZFArgument argument,
                                                                     ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID =
            (unsigned int)zego_value_get_int(argument[ZFValue("audioEffectID")]);
        auto position = zego_value_get_vector_float(argument[ZFValue("position")]);
        player->updatePosition(audioEffectID, position.data());

        result->Success();
    } else {
        result->Error("audioEffectPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `audioEffectPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaPlayer(ZFArgument argument, ZFResult result) {
    auto mediaPlayer = EXPRESS::ZegoExpressSDK::getEngine()->createMediaPlayer();
    if (mediaPlayer) {
        auto index = mediaPlayer->getIndex();

        mediaPlayer->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaPlayer->setVideoHandler(ZegoTextureRendererController::getInstance(),
                                     ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32);
        mediaPlayerMap_[index] = mediaPlayer;

        result->Success(ZFValue(index));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaPlayer(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->setEventHandler(nullptr);
        ZegoTextureRendererController::getInstance()->removeMediaPlayerRenderer(mediaPlayer);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyMediaPlayer(mediaPlayer);
    }

    mediaPlayerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResource(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = zego_value_get_string(argument[ZFValue("path")]);

        auto sharedPtrResult = ZFMoveResult(result);

        mediaPlayer->loadResource(path, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerLoadResource_Can_not_find_player",
                      "Invoke `mediaPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData(ZFArgument argument,
                                                                          ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        uint64_t startPosition = zego_value_get_long(argument[ZFValue("startPosition")]);

        auto mediaData = zego_value_get_vector_uint8(argument[ZFValue("mediaData")]);

        auto sharedPtrResult = ZFMoveResult(result);

        mediaPlayer->loadResourceFromMediaData(
            mediaData.data(), (int)mediaData.size(), startPosition, [=](int errorCode) {
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);

                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error(
            "mediaPlayerLoadResourceFromMediaData_Can_not_find_player",
            "Invoke `mediaPlayerLoadResourceFromMediaData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition(ZFArgument argument,
                                                                         ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string path = zego_value_get_string(argument[ZFValue("path")]);
        uint64_t startPosition = zego_value_get_long(argument[ZFValue("startPosition")]);

        auto sharedPtrResult = ZFMoveResult(result);

        mediaPlayer->loadResourceWithPosition(path, startPosition, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "mediaPlayerLoadResourceWithPosition_Can_not_find_player",
            "Invoke `mediaPlayerLoadResourceWithPosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerStart(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->start();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerStop(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->stop();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerPause(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->pause();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerResume(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->resume();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSeekTo(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        unsigned long long millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);

        auto sharedPtrResult = ZFMoveResult(result);
        mediaPlayer->seekTo(millisecond, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerSeekTo_Can_not_find_player",
                      "Invoke `mediaPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[ZFValue("enable")]);

        mediaPlayer->enableRepeat(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAux(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[ZFValue("enable")]);

        mediaPlayer->enableAux(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool mute = zego_value_get_bool(argument[ZFValue("mute")]);

        mediaPlayer->muteLocal(mute);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableViewMirror(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        bool enable = zego_value_get_bool(argument[ZFValue("enable")]);

        mediaPlayer->enableViewMirror(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVolume(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[ZFValue("volume")]);

        mediaPlayer->setVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[ZFValue("volume")]);

        mediaPlayer->setPlayVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = zego_value_get_int(argument[ZFValue("volume")]);

        mediaPlayer->setPublishVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval(ZFArgument argument,
                                                                    ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int millsecond = zego_value_get_int(argument[ZFValue("millisecond")]);

        mediaPlayer->setProgressInterval((unsigned long long)millsecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPlayVolume();
        result->Success(ZFValue(volume));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        int volume = mediaPlayer->getPublishVolume();
        result->Success(ZFValue(volume));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto totalDuration = mediaPlayer->getTotalDuration();
        // TODO: need test
        result->Success(ZFValue((int64_t)totalDuration));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto currentProgress = mediaPlayer->getCurrentProgress();
        // TODO: need test
        result->Success(ZFValue((int64_t)currentProgress));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioTrackCount = mediaPlayer->getAudioTrackCount();
        result->Success(ZFValue((int32_t)audioTrackCount));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto trackIndex = zego_value_get_int(argument[ZFValue("trackIndex")]);
        mediaPlayer->setAudioTrackIndex(trackIndex);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam(ZFArgument argument,
                                                                     ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        ZFMap paramMap = zego_value_get_map(argument[ZFValue("param")]);
        auto pitch = zego_value_get_double(paramMap[ZFValue("pitch")]);

        auto audioChannel = zego_value_get_int(argument[ZFValue("audioChannel")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->setVoiceChangerParam((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel,
                                          param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableVoiceChanger(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        ZFMap paramMap = zego_value_get_map(argument[ZFValue("param")]);
        auto pitch = zego_value_get_double(paramMap[ZFValue("pitch")]);

        auto audioChannel = zego_value_get_int(argument[ZFValue("audioChannel")]);

        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->enableVoiceChanger((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel,
                                        enable, param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto state = mediaPlayer->getCurrentState();
        result->Success(ZFValue((int32_t)state));
    } else {
        result->Success(ZFValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto speed = zego_value_get_double(argument[ZFValue("speed")]);
        mediaPlayer->setPlaySpeed(speed);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor(ZFArgument argument,
                                                                        ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        auto millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);
        mediaPlayer->enableSoundLevelMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor(ZFArgument argument,
                                                                               ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        auto millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);
        mediaPlayer->enableFrequencySpectrumMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache(ZFArgument argument,
                                                                           ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto time = zego_value_get_long(argument[ZFValue("time")]);
        auto size = zego_value_get_long(argument[ZFValue("size")]);
        mediaPlayer->setNetWorkResourceMaxCache(time, size);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold(ZFArgument argument,
                                                                          ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto threshold = zego_value_get_long(argument[ZFValue("threshold")]);
        mediaPlayer->setNetWorkBufferThreshold(threshold);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache(ZFArgument argument,
                                                                        ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        ZFMap resultMap;
        auto netWorkResourceCache = mediaPlayer->getNetWorkResourceCache();

        resultMap[ZFValue("time")] = ZFValue((int64_t)netWorkResourceCache->time);
        resultMap[ZFValue("size")] = ZFValue((int64_t)netWorkResourceCache->size);
        result->Success(resultMap);
    } else {
        result->Error("mediaPlayerGetNetWorkResourceCache_Can_not_find_player",
                      "Invoke `mediaPlayerGetNetWorkResourceCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAccurateSeek(ZFArgument argument,
                                                                   ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);
        auto timeout = zego_value_get_int(configMap[ZFValue("timeout")]);
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
    ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        std::string resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        uint64_t startPosition = zego_value_get_long(argument[ZFValue("startPosition")]);

        auto sharedPtrResult = ZFMoveResult(result);

        mediaPlayer->loadCopyrightedMusicResourceWithPosition(
            resourceID, startPosition, [=](int errorCode) {
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);

                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("mediaPlayerLoadCopyrightedMusicResourceWithPosition_Can_not_find_player",
                      "Invoke `mediaPlayerLoadCopyrightedMusicResourceWithPosition` but can't find "
                      "specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerClearView(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->clearView();

        result->Success();
    } else {
        result->Error("mediaPlayerClearView_Can_not_find_player",
                      "Invoke `mediaPlayerClearView` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetActiveAudioChannel(ZFArgument argument,
                                                                      ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioChannel = zego_value_get_int(argument[ZFValue("audioChannel")]);
        mediaPlayer->setActiveAudioChannel((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel);

        result->Success();
    } else {
        result->Error("mediaPlayerSetActiveAudioChannel_Can_not_find_player",
                      "Invoke `mediaPlayerSetActiveAudioChannel` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {

        ZFMap canvasMap = zego_value_get_map(argument[ZFValue("canvas")]);

        EXPRESS::ZegoCanvas canvas;
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[ZFValue("viewMode")]);
        auto viewID = zego_value_get_long(canvasMap[ZFValue("view")]);
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

void ZegoExpressEngineMethodHandler::mediaPlayerTakeSnapshot(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        ZFMap resultMap;
        resultMap[ZFValue("errorCode")] = ZFValue(-1);
#ifdef _WIN32
        auto pFrame =
            ZegoTextureRendererController::getInstance()->getMediaPlayerFrame(mediaPlayer);
        auto size = ZegoTextureRendererController::getInstance()->getMediaPlayerSize(mediaPlayer);
        auto stride =
            ZegoTextureRendererController::getInstance()->getMediaPlayerFrameStride(mediaPlayer);
        if (pFrame && size != std::pair<int, int>(0, 0)) {
            auto tmpData = makeBtimap(pFrame, size, stride);
            std::vector<uint8_t> raw_image(tmpData.second, tmpData.second + tmpData.first);
            delete[] tmpData.second;

            resultMap[ZFValue("image")] = ZFValue(raw_image);
            resultMap[ZFValue("errorCode")] = ZFValue(0);
        } else {
            resultMap[ZFValue("errorCode")] = ZFValue(-1);
        }
#endif
        result->Success(resultMap);
    } else {
        result->Error("mediaPlayerTakeSnapshot_Can_not_find_player",
                      "Invoke `mediaPlayerTakeSnapshot` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackMode(ZFArgument argument,
                                                                  ZFResult result) {
    // auto index = zego_value_get_int(argument[ZFValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto mode = zego_value_get_int(argument[ZFValue("mode")]);
    //     mediaPlayer->setAudioTrackMode((EXPRESS::ZegoMediaPlayerAudioTrackMode)mode);

    //     result->Success();
    // }
    // else
    // {
    //     result->Error("mediaPlayerSetAudioTrackMode_Can_not_find_player", "Invoke `mediaPlayerSetAudioTrackMode` but can't find specific player");
    // }
    result->Error("not_support_feature", "windows platform not support feature");
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackPublishIndex(ZFArgument argument,
                                                                          ZFResult result) {
    // auto index = zego_value_get_int(argument[ZFValue("index")]);
    // auto mediaPlayer = mediaPlayerMap_[index];

    // if (mediaPlayer) {
    //     auto index_ = zego_value_get_int(argument[ZFValue("index_")]);
    //     mediaPlayer->setAudioTrackPublishIndex(index_);

    //     result->Success();
    // }
    // else
    // {
    //     result->Error("mediaPlayerSetAudioTrackPublishIndex_Can_not_find_player", "Invoke `mediaPlayerSetAudioTrackPublishIndex` but can't find specific player");
    // }
    result->Error("not_support_feature", "windows platform not support feature");
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAudioData(ZFArgument argument,
                                                                ZFResult result) {

    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        if (enable) {
#ifdef _WIN32
            mediaPlayer->setAudioHandler(ZegoMediaPlayerAudioManager::getInstance()->getHandler());
#endif
        } else {
            mediaPlayer->setAudioHandler(nullptr);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableAudioData_Can_not_find_player",
                      "Invoke `mediaPlayerEnableAudioData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableVideoData(ZFArgument argument,
                                                                ZFResult result) {

    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        if (enable) {
#ifdef _WIN32
            ZegoTextureRendererController::getInstance()->setMediaPlayerVideoHandler(
                ZegoMediaPlayerVideoManager::getInstance()->getHandler());
#endif
        } else {
            ZegoTextureRendererController::getInstance()->setMediaPlayerVideoHandler(nullptr);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableVideoData_Can_not_find_player",
                      "Invoke `mediaPlayerEnableVideoData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableBlockData(ZFArgument argument,
                                                                ZFResult result) {

    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        unsigned int blockSize = zego_value_get_long(argument[ZFValue("blockSize")]);
        if (enable) {
#ifdef _WIN32
            mediaPlayer->setBlockDataHandler(
                ZegoMediaPlayerBlockDataManager::getInstance()->getHandler(), blockSize);
#endif
        } else {
            mediaPlayer->setBlockDataHandler(nullptr, blockSize);
        }
        result->Success();
    } else {
        result->Error("mediaPlayerEnableBlockData_Can_not_find_player",
                      "Invoke `mediaPlayerEnableBlockData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithConfig(ZFArgument argument,
                                                                       ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto resourceMap = zego_value_get_map(argument[ZFValue("resource")]);
        EXPRESS::ZegoMediaPlayerResource resource;
        resource.resourceID = zego_value_get_string(resourceMap[ZFValue("resourceID")]);
        resource.loadType =
            (EXPRESS::ZegoMultimediaLoadType)zego_value_get_int(resourceMap[ZFValue("loadType")]);
        resource.alphaLayout =
            (EXPRESS::ZegoAlphaLayoutType)zego_value_get_int(resourceMap[ZFValue("alphaLayout")]);
        resource.startPosition = zego_value_get_long(resourceMap[ZFValue("startPosition")]);
        resource.filePath = zego_value_get_string(resourceMap[ZFValue("filePath")]);
        auto memory = zego_value_get_vector_uint8(resourceMap[ZFValue("memory")]);
        resource.memory = memory.data();
        resource.memoryLength = memory.size();

        auto sharedPtrResult = ZFMoveResult(result);

        mediaPlayer->loadResourceWithConfig(resource, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("mediaPlayerLoadResourceWithConfig_Can_not_find_player",
                      "Invoke `mediaPlayerLoadResourceWithConfig` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerUpdatePosition(ZFArgument argument,
                                                               ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto position = zego_value_get_vector_float(argument[ZFValue("position")]);
        mediaPlayer->updatePosition(position.data());

        result->Success();
    } else {
        result->Error("mediaPlayerUpdatePosition_Can_not_find_player",
                      "Invoke `mediaPlayerUpdatePosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetMediaInfo(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto info = mediaPlayer->getMediaInfo();

        ZFMap resultMap;

        resultMap[ZFValue("width")] = ZFValue((int32_t)info.width);
        resultMap[ZFValue("height")] = ZFValue((int32_t)info.height);
        resultMap[ZFValue("frameRate")] = ZFValue((int32_t)info.frameRate);
        result->Success(resultMap);

    } else {
        result->Error("mediaPlayerGetMediaInfo_Can_not_find_player",
                      "Invoke `mediaPlayerGetMediaInfo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetHttpHeader(ZFArgument argument,
                                                              ZFResult result) {

    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto headersMap = zego_value_get_map(argument[ZFValue("headers")]);
        std::unordered_map<std::string, std::string> headers;
        for (const auto &header : headersMap) {
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

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentRenderingProgress(ZFArgument argument,
                                                                            ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto progress = mediaPlayer->getCurrentRenderingProgress();
        result->Success(ZFValue((int64_t)progress));

    } else {
        result->Error(
            "mediaPlayerGetCurrentRenderingProgress_Can_not_find_player",
            "Invoke `mediaPlayerGetCurrentRenderingProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableLiveAudioEffect(ZFArgument argument,
                                                                      ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        auto mode = zego_value_get_int(argument[ZFValue("mode")]);

        mediaPlayer->enableLiveAudioEffect(enable, (EXPRESS::ZegoLiveAudioEffectMode)mode);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLiveAudioEffect_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLiveAudioEffect` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayMediaStreamType(ZFArgument argument,
                                                                       ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto streamType = zego_value_get_int(argument[ZFValue("streamType")]);

        mediaPlayer->setPlayMediaStreamType((EXPRESS::ZegoMediaStreamType)streamType);

        result->Success();
    } else {
        result->Error("mediaPlayerSetPlayMediaStreamType_Can_not_find_player",
                      "Invoke `mediaPlayerSetPlayMediaStreamType` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableLocalCache(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        std::string cacheDir = zego_value_get_string(argument[ZFValue("cacheDir")]);

        mediaPlayer->enableLocalCache(enable, cacheDir);

        result->Success();
    } else {
        result->Error("mediaPlayerEnableLocalCache_Can_not_find_player",
                      "Invoke `mediaPlayerEnableLocalCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlaybackStatistics(ZFArgument argument,
                                                                      ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto info = mediaPlayer->getPlaybackStatistics();
        ZFMap retMap;
        retMap[ZFValue("videoSourceFps")] = ZFValue(info.videoSourceFps);
        retMap[ZFValue("videoDecodeFps")] = ZFValue(info.videoDecodeFps);
        retMap[ZFValue("videoRenderFps")] = ZFValue(info.videoRenderFps);
        retMap[ZFValue("audioSourceFps")] = ZFValue(info.audioSourceFps);
        retMap[ZFValue("audioDecodeFps")] = ZFValue(info.audioDecodeFps);
        retMap[ZFValue("audioRenderFps")] = ZFValue(info.audioRenderFps);
        result->Success(retMap);

    } else {
        result->Error("mediaPlayerGetPlaybackStatistics_Can_not_find_player",
                      "Invoke `mediaPlayerGetPlaybackStatistics` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaDataPublisher(ZFArgument argument,
                                                              ZFResult result) {

    ZFMap config_map = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoMediaDataPublisherConfig config{};
    config.channel = zego_value_get_int(config_map[ZFValue("channel")]);
    config.mode =
        (EXPRESS::ZegoMediaDataPublisherMode)zego_value_get_int(config_map[ZFValue("mode")]);

    auto publisher = EXPRESS::ZegoExpressSDK::getEngine()->createMediaDataPublisher(config);
    if (publisher) {
        int index = publisher->getIndex();
        publisher->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaDataPublisherMap_[index] = publisher;
        result->Success(ZFValue(index));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaDataPublisher(ZFArgument argument,
                                                               ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
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

void ZegoExpressEngineMethodHandler::mediaDataPublisherAddMediaFilePath(ZFArgument argument,
                                                                        ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        bool is_clear = zego_value_get_bool(argument[ZFValue("isClear")]);
        std::string path = zego_value_get_string(argument[ZFValue("path")]);
        publisher->addMediaFilePath(path, is_clear);
        result->Success();
    } else {
        result->Error(
            "mediaDataPublisherAddMediaFilePath_Can_not_find_publisher",
            "Invoke `mediaDataPublisherAddMediaFilePath` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherGetCurrentDuration(ZFArgument argument,
                                                                          ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        auto duration = publisher->getCurrentDuration();
        result->Success(ZFValue((int64_t)duration));
    } else {
        result->Error(
            "mediaDataPublisherGetCurrentDuration_Can_not_find_publisher",
            "Invoke `mediaDataPublisherGetCurrentDuration` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherGetTotalDuration(ZFArgument argument,
                                                                        ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        auto duration = publisher->getTotalDuration();
        result->Success(ZFValue((int64_t)duration));
    } else {
        result->Error(
            "mediaDataPublisherGetTotalDuration_Can_not_find_publisher",
            "Invoke `mediaDataPublisherGetTotalDuration` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherReset(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        publisher->reset();
        result->Success();
    } else {
        result->Error("mediaDataPublisherReset_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherReset` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherSeekTo(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        unsigned long long millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);
        publisher->seekTo(millisecond);
        result->Success();
    } else {
        result->Error("mediaDataPublisherSeekTo_Can_not_find_publisher",
                      "Invoke `mediaDataPublisherSeekTo` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::mediaDataPublisherSetVideoSendDelayTime(ZFArgument argument,
                                                                             ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto publisher = mediaDataPublisherMap_[index];
    if (publisher) {
        int delay_time = zego_value_get_int(argument[ZFValue("delay_time")]);
        publisher->setVideoSendDelayTime(delay_time);
        result->Success();
    } else {
        result->Error(
            "mediaDataPublisherSetVideoSendDelayTime_Can_not_find_publisher",
            "Invoke `mediaDataPublisherSetVideoSendDelayTime` but can't find specific publisher");
    }
}

void ZegoExpressEngineMethodHandler::startMixerTask(ZFArgument argument, ZFResult result) {

    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = zego_value_get_map(argument[ZFValue("audioConfig")]);
    auto taskVideoConfig = zego_value_get_map(argument[ZFValue("videoConfig")]);
    ZFMap advancedConfigMap = zego_value_get_map(argument[ZFValue("advancedConfig")]);

    for (const auto &cfg : advancedConfigMap) {
        std::string key = zego_value_get_string(cfg.first);
        std::string value = zego_value_get_string(cfg.second);
        task.advancedConfig[key] = value;
    }

    task.taskID = zego_value_get_string(argument[ZFValue("taskID")]);

    auto inputFlutterList = zego_value_get_list(argument[ZFValue("inputList")]);
    auto outputFlutterList = zego_value_get_list(argument[ZFValue("outputList")]);

    task.backgroundImageURL = zego_value_get_string(argument[ZFValue("backgroundImageURL")]);
    task.enableSoundLevel = zego_value_get_bool(argument[ZFValue("enableSoundLevel")]);

    // backgroundColor
    task.backgroundColor = zego_value_get_int(argument[ZFValue("backgroundColor")]);

    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)zego_value_get_int(
        argument[ZFValue("streamAlignmentMode")]);

    task.mixImageCheckMode =
        (EXPRESS::ZegoMixImageCheckMode)zego_value_get_int(argument[ZFValue("mixImageCheckMode")]);

    // userData
    auto userData = zego_value_get_vector_uint8(argument[ZFValue("userData")]);
    task.userData = userData.data();
    task.userDataLength = userData.size();

    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        zego_value_get_int(argument[ZFValue("minPlayStreamBufferLength")]);

    task.audioConfig.bitrate = zego_value_get_int(taskAudioConfig[ZFValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)zego_value_get_int(taskAudioConfig[ZFValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)zego_value_get_int(taskAudioConfig[ZFValue("codecID")]);
    task.audioConfig.mixMode =
        (EXPRESS::ZegoAudioMixMode)zego_value_get_int(taskAudioConfig[ZFValue("mixMode")]);

    task.videoConfig.bitrate = zego_value_get_int(taskVideoConfig[ZFValue("bitrate")]);
    task.videoConfig.fps = zego_value_get_int(taskVideoConfig[ZFValue("fps")]);
    task.videoConfig.height = zego_value_get_int(taskVideoConfig[ZFValue("height")]);
    task.videoConfig.width = zego_value_get_int(taskVideoConfig[ZFValue("width")]);
    task.videoConfig.quality = zego_value_get_int(taskVideoConfig[ZFValue("quality")]);
    task.videoConfig.rateControlMode = (EXPRESS::ZegoVideoRateControlMode)zego_value_get_int(
        taskVideoConfig[ZFValue("rateControlMode")]);

    for (const auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = zego_value_get_map(inputIter);

        input.streamID = zego_value_get_string(inputMap[ZFValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)zego_value_get_int(
            inputMap[ZFValue("contentType")]);
        input.layout.height = zego_value_get_int(inputMap[ZFValue("bottom")]) -
                              zego_value_get_int(inputMap[ZFValue("top")]);
        input.layout.width = zego_value_get_int(inputMap[ZFValue("right")]) -
                             zego_value_get_int(inputMap[ZFValue("left")]);
        input.layout.x = zego_value_get_int(inputMap[ZFValue("left")]);
        input.layout.y = zego_value_get_int(inputMap[ZFValue("top")]);
        input.soundLevelID = zego_value_get_int(inputMap[ZFValue("soundLevelID")]);

        input.volume = zego_value_get_int(inputMap[ZFValue("volume")]);
        input.isAudioFocus = zego_value_get_bool(inputMap[ZFValue("isAudioFocus")]);
        input.audioDirection = zego_value_get_int(inputMap[ZFValue("audioDirection")]);

        if (!zego_value_is_null(inputMap[ZFValue("label")])) {
            auto labelMap = zego_value_get_map(inputMap[ZFValue("label")]);
            auto text = zego_value_get_string(labelMap[ZFValue("text")]);
            EXPRESS::ZegoLabelInfo labelInfo(text);
            labelInfo.left = zego_value_get_int(labelMap[ZFValue("left")]);
            labelInfo.top = zego_value_get_int(labelMap[ZFValue("top")]);
            auto fontMap = zego_value_get_map(labelMap[ZFValue("font")]);
            EXPRESS::ZegoFontStyle fontStyle;
            fontStyle.type = (EXPRESS::ZegoFontType)zego_value_get_int(fontMap[ZFValue("type")]);
            fontStyle.size = zego_value_get_int(fontMap[ZFValue("size")]);
            fontStyle.color = zego_value_get_int(fontMap[ZFValue("color")]);
            fontStyle.transparency = zego_value_get_int(fontMap[ZFValue("transparency")]);
            fontStyle.border = zego_value_get_bool(fontMap[ZFValue("border")]);
            fontStyle.borderColor = zego_value_get_int(fontMap[ZFValue("borderColor")]);
            labelInfo.font = fontStyle;
            input.label = labelInfo;
        }

        if (!zego_value_is_null(inputMap[ZFValue("renderMode")])) {
            input.renderMode =
                (EXPRESS::ZegoMixRenderMode)zego_value_get_int(inputMap[ZFValue("renderMode")]);
        }

        if (!zego_value_is_null(inputMap[ZFValue("imageInfo")])) {
            auto imageInfoMap = zego_value_get_map(inputMap[ZFValue("imageInfo")]);
            auto url = zego_value_get_string(imageInfoMap[ZFValue("url")]);
            int32_t displayMode = 0;
            if (!zego_value_is_null(imageInfoMap[ZFValue("displayMode")])) {
                displayMode = zego_value_get_int(imageInfoMap[ZFValue("displayMode")]);
            }
            input.imageInfo = EXPRESS::ZegoMixerImageInfo(url, displayMode);
        }

        if (!zego_value_is_null(inputMap[ZFValue("cornerRadius")])) {
            input.cornerRadius = zego_value_get_int(inputMap[ZFValue("cornerRadius")]);
        }

        task.inputList.push_back(input);
    }
    for (const auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        ZFMap outputMap = zego_value_get_map(outputIter);
        output.target = zego_value_get_string(outputMap[ZFValue("target")]);

        if (!zego_value_is_null(outputMap[ZFValue("videoConfig")])) {
            auto videoConfigMap = zego_value_get_map(outputMap[ZFValue("videoConfig")]);
            if (videoConfigMap.size() > 0) {
                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[ZFValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[ZFValue("encodeLatency")]);
                output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                    videoConfigMap[ZFValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD =
                    zego_value_get_bool(videoConfigMap[ZFValue("enableLowBitrateHD")]);

                auto codecID = zego_value_get_int(videoConfigMap[ZFValue("videoCodecID")]);
                if (codecID > 4) {
                    codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecID;
            }
        }

        if (!zego_value_is_null(outputMap[ZFValue("targetRoom")])) {
            auto targetRoomMap = zego_value_get_map(outputMap[ZFValue("targetRoom")]);
            if (targetRoomMap.size() > 0) {
                output.targetRoom.roomID = zego_value_get_string(targetRoomMap[ZFValue("roomID")]);
                output.targetRoom.userID = zego_value_get_string(targetRoomMap[ZFValue("userID")]);
            }
        }

        task.outputList.push_back(output);
    }

    // Water mark
    EXPRESS::ZegoWatermark watermark;
    if (!zego_value_is_null(argument[ZFValue("watermark")])) {
        auto watermarkMap = zego_value_get_map(argument[ZFValue("watermark")]);
        std::string imageURL = zego_value_get_string(watermarkMap[ZFValue("imageURL")]);
        if (!imageURL.empty()) {
            watermark.imageURL = imageURL;
            watermark.layout.x = zego_value_get_int(watermarkMap[ZFValue("left")]);
            watermark.layout.y = zego_value_get_int(watermarkMap[ZFValue("top")]);
            watermark.layout.width = zego_value_get_int(watermarkMap[ZFValue("right")]) -
                                     zego_value_get_int(watermarkMap[ZFValue("left")]);
            watermark.layout.height = zego_value_get_int(watermarkMap[ZFValue("bottom")]) -
                                      zego_value_get_int(watermarkMap[ZFValue("top")]);

            task.watermark = &watermark;
        }
    }

    // whiteboard
    EXPRESS::ZegoMixerWhiteboard whiteboard;
    if (!zego_value_is_null(argument[ZFValue("whiteboard")])) {
        auto whiteboardMap = zego_value_get_map(argument[ZFValue("whiteboard")]);
        int64_t whiteboardID = zego_value_get_long(whiteboardMap[ZFValue("whiteboardID")]);
        if (whiteboardID != 0) {
            whiteboard.whiteboardID = whiteboardID;
            whiteboard.horizontalRatio =
                zego_value_get_int(whiteboardMap[ZFValue("horizontalRatio")]);
            whiteboard.verticalRatio = zego_value_get_int(whiteboardMap[ZFValue("verticalRatio")]);
            whiteboard.isPPTAnimation =
                zego_value_get_bool(whiteboardMap[ZFValue("isPPTAnimation")]);
            whiteboard.zOrder = zego_value_get_int(whiteboardMap[ZFValue("zOrder")]);
            if (!zego_value_is_null(whiteboardMap[ZFValue("backgroundColor")])) {
                whiteboard.backgroundColor = zego_value_get_int(whiteboardMap[ZFValue("backgroundColor")]);
            }

            auto layoutMap = zego_value_get_map(whiteboardMap[ZFValue("layout")]);
            whiteboard.layout.x = zego_value_get_int(layoutMap[ZFValue("left")]);
            whiteboard.layout.y = zego_value_get_int(layoutMap[ZFValue("top")]);
            whiteboard.layout.width = zego_value_get_int(layoutMap[ZFValue("right")]) -
                                      zego_value_get_int(layoutMap[ZFValue("left")]);
            whiteboard.layout.height = zego_value_get_int(layoutMap[ZFValue("bottom")]) -
                                       zego_value_get_int(layoutMap[ZFValue("top")]);

            task.whiteboard = &whiteboard;
        }
    }

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->startMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            ZF::logInfo("*** Plugin onMixerStartResult");
            ZFMap retMap;

            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("extendedData")] = ZFValue(extendedData);

            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopMixerTask(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = zego_value_get_map(argument[ZFValue("audioConfig")]);
    auto taskVideoConfig = zego_value_get_map(argument[ZFValue("videoConfig")]);
    // auto taskWatermark = zego_value_get_map(argument[ZFValue("watermark")]);

    task.taskID = zego_value_get_string(argument[ZFValue("taskID")]);

    task.audioConfig.bitrate = zego_value_get_int(taskAudioConfig[ZFValue("bitrate")]);
    task.audioConfig.channel =
        (EXPRESS::ZegoAudioChannel)zego_value_get_int(taskAudioConfig[ZFValue("channel")]);
    task.audioConfig.codecID =
        (EXPRESS::ZegoAudioCodecID)zego_value_get_int(taskAudioConfig[ZFValue("codecID")]);

    task.videoConfig.bitrate = zego_value_get_int(taskVideoConfig[ZFValue("bitrate")]);
    task.videoConfig.fps = zego_value_get_int(taskVideoConfig[ZFValue("fps")]);
    task.videoConfig.height = zego_value_get_int(taskVideoConfig[ZFValue("height")]);
    task.videoConfig.width = zego_value_get_int(taskVideoConfig[ZFValue("width")]);

    auto inputFlutterList = zego_value_get_list(argument[ZFValue("inputList")]);
    auto outputFlutterList = zego_value_get_list(argument[ZFValue("outputList")]);

    for (const auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = zego_value_get_map(inputIter);

        input.streamID = zego_value_get_string(inputMap[ZFValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)zego_value_get_int(
            inputMap[ZFValue("contentType")]);
        input.layout.height = zego_value_get_int(inputMap[ZFValue("bottom")]) -
                              zego_value_get_int(inputMap[ZFValue("top")]);
        input.layout.width = zego_value_get_int(inputMap[ZFValue("right")]) -
                             zego_value_get_int(inputMap[ZFValue("left")]);
        input.layout.x = zego_value_get_int(inputMap[ZFValue("left")]);
        input.layout.y = zego_value_get_int(inputMap[ZFValue("top")]);
        input.soundLevelID = zego_value_get_int(inputMap[ZFValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (const auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        ZFMap outputMap = zego_value_get_map(outputIter);
        output.target = zego_value_get_string(outputMap[ZFValue("target")]);

        if (!zego_value_is_null(outputMap[ZFValue("videoConfig")])) {
            auto videoConfigMap = zego_value_get_map(outputMap[ZFValue("videoConfig")]);
            if (videoConfigMap.size() > 0) {
                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[ZFValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[ZFValue("encodeLatency")]);
                output.videoConfig.encodeProfile = (EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                    videoConfigMap[ZFValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD =
                    zego_value_get_bool(videoConfigMap[ZFValue("enableLowBitrateHD")]);

                auto codecID = zego_value_get_int(videoConfigMap[ZFValue("videoCodecID")]);
                if (codecID > 4) {
                    codecID = (int32_t)EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecID;
            }
        }

        if (!zego_value_is_null(outputMap[ZFValue("targetRoom")])) {
            auto targetRoomMap = zego_value_get_map(outputMap[ZFValue("targetRoom")]);
            if (targetRoomMap.size() > 0) {
                output.targetRoom.roomID = zego_value_get_string(targetRoomMap[ZFValue("roomID")]);
                output.targetRoom.userID = zego_value_get_string(targetRoomMap[ZFValue("userID")]);
            }
        }

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;
    task.backgroundImageURL = zego_value_get_string(argument[ZFValue("backgroundImageURL")]);
    task.enableSoundLevel = zego_value_get_bool(argument[ZFValue("enableSoundLevel")]);

    ZFMap advancedConfigMap = zego_value_get_map(argument[ZFValue("advancedConfig")]);
    for (const auto &cfg : advancedConfigMap) {
        std::string key = zego_value_get_string(cfg.first);
        std::string value = zego_value_get_string(cfg.second);
        task.advancedConfig[key] = value;
    }

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->stopMixerTask(task, [=](int errorCode) {
        ZFMap retMap;

        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setSEIConfig(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoSEIConfig config;
    ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);

    config.type = (EXPRESS::ZegoSEIType)zego_value_get_int(configMap[ZFValue("type")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSEIConfig(config);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioDeviceVolume(ZFArgument argument, ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);
    auto volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setSpeakerVolumeInAPP(ZFArgument argument, ZFResult result) {
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);
    auto volume = zego_value_get_int(argument[ZFValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSpeakerVolumeInAPP(deviceID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getSpeakerVolumeInAPP(ZFArgument argument, ZFResult result) {
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);

    int volume = EXPRESS::ZegoExpressSDK::getEngine()->getSpeakerVolumeInAPP(deviceID.c_str());

    result->Success(ZFValue(volume));
}

void ZegoExpressEngineMethodHandler::startAudioDeviceVolumeMonitor(ZFArgument argument,
                                                                   ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDeviceVolumeMonitor(ZFArgument argument,
                                                                  ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDeviceVolumeMonitor(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAudioDevice(ZFArgument argument, ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAudioDevice((EXPRESS::ZegoAudioDeviceType)deviceType,
                                                          deviceID.c_str(), mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAudioDeviceMuted(ZFArgument argument, ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);

    auto ret = EXPRESS::ZegoExpressSDK::getEngine()->isAudioDeviceMuted(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);

    result->Success(ZFValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioDeviceMode(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioDeviceVolume(ZFArgument argument, ZFResult result) {
    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);

    auto volume = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceVolume(
        (EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str());

    result->Success(ZFValue(volume));
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureDevice(ZFArgument argument,
                                                              ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    auto sharedPtrResult = ZFMoveResult(result);
    std::thread tmpThread([=]() {
        EXPRESS::ZegoExpressSDK::getEngine()->enableAudioCaptureDevice(enable);
        sharedPtrResult->Success();
    });

    tmpThread.detach();
}

void ZegoExpressEngineMethodHandler::enableTrafficControl(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto property = zego_value_get_int(argument[ZFValue("property")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTrafficControl(
        enable, property, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startRecordingCapturedData(ZFArgument argument,
                                                                ZFResult result) {
    EXPRESS::ZegoDataRecordConfig config;

    ZFMap configMap = zego_value_get_map(argument[ZFValue("config")]);

    config.filePath = zego_value_get_string(configMap[ZFValue("filePath")]);
    config.recordType =
        (EXPRESS::ZegoDataRecordType)zego_value_get_int(configMap[ZFValue("recordType")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startRecordingCapturedData(
        config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopRecordingCapturedData(ZFArgument argument,
                                                               ZFResult result) {
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopRecordingCapturedData(
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCamera(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCamera(enable,
                                                       (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto minFPS = zego_value_get_int(argument[ZFValue("minFPS")]);
    auto maxFPS = zego_value_get_int(argument[ZFValue("maxFPS")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCameraAdaptiveFPS(
        enable, minFPS, maxFPS, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::useVideoDevice(ZFArgument argument, ZFResult result) {
    auto deviceID = zego_value_get_string(argument[ZFValue("deviceID")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->useVideoDevice(deviceID,
                                                         (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoDeviceList(ZFArgument argument, ZFResult result) {

    ZFArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getVideoDeviceList();
    for (auto &deviceInfo : deviceList) {
        ZFMap deviceMap;
        deviceMap[ZFValue("deviceID")] = ZFValue(deviceInfo.deviceID);
        deviceMap[ZFValue("deviceName")] = ZFValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(ZFValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultVideoDeviceID(ZFArgument argument, ZFResult result) {

    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultVideoDeviceID();
    result->Success(ZFValue(deviceID));
}

void ZegoExpressEngineMethodHandler::enableMixSystemPlayout(ZFArgument argument, ZFResult result) {

    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixSystemPlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setMixSystemPlayoutVolume(ZFArgument argument,
                                                               ZFResult result) {

    auto volume = zego_value_get_int(argument[ZFValue("volume")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setMixSystemPlayoutVolume(volume);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableMixEnginePlayout(ZFArgument argument, ZFResult result) {

    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableMixEnginePlayout(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startAudioVADStableStateMonitor(ZFArgument argument,
                                                                     ZFResult result) {

    auto type = zego_value_get_int(argument[ZFValue("type")]);
    if (zego_value_is_null(argument[ZFValue("millisecond")])) {
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type);
    } else {
        auto millisecond = zego_value_get_int(argument[ZFValue("millisecond")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startAudioVADStableStateMonitor(
            (EXPRESS::ZegoAudioVADStableStateMonitorType)type, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioVADStableStateMonitor(ZFArgument argument,
                                                                    ZFResult result) {

    auto type = zego_value_get_int(argument[ZFValue("type")]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioVADStableStateMonitor(
        (EXPRESS::ZegoAudioVADStableStateMonitorType)type);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getCurrentAudioDevice(ZFArgument argument, ZFResult result) {

    auto deviceType = zego_value_get_int(argument[ZFValue("deviceType")]);
    auto deviceInfo = EXPRESS::ZegoExpressSDK::getEngine()->getCurrentAudioDevice(
        (EXPRESS::ZegoAudioDeviceType)deviceType);

    ZFMap resultMap;
    resultMap[ZFValue("deviceID")] = ZFValue(deviceInfo.deviceID);
    resultMap[ZFValue("deviceName")] = ZFValue(deviceInfo.deviceName);
    result->Success(resultMap);
}

void ZegoExpressEngineMethodHandler::createCopyrightedMusic(ZFArgument argument, ZFResult result) {
    auto tmpCopyrightedMusic = EXPRESS::ZegoExpressSDK::getEngine()->createCopyrightedMusic();
    if (tmpCopyrightedMusic != nullptr) {
        copyrightedMusic_ = tmpCopyrightedMusic;
        copyrightedMusic_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(ZFValue(0));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyCopyrightedMusic(ZFArgument argument, ZFResult result) {
    if (copyrightedMusic_) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyCopyrightedMusic(copyrightedMusic_);
        copyrightedMusic_ = nullptr;
        result->Success();
    } else {
        result->Error("destroyCopyrightedMusic_Can_not_find_instance",
                      "Invoke `destroyCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicClearCache(ZFArgument argument,
                                                                ZFResult result) {
    if (copyrightedMusic_) {
        copyrightedMusic_->clearCache();
        result->Success();
    } else {
        result->Error("copyrightedMusicClearCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicClearCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicDownload(ZFArgument argument,
                                                              ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->download(resourceID, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicDownload_Can_not_find_instance",
                      "Invoke `copyrightedMusicDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicCancelDownload(ZFArgument argument,
                                                                    ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        copyrightedMusic_->cancelDownload(resourceID);
        result->Success();
    } else {
        result->Error("copyrightedMusicCancelDownload_Can_not_find_instance",
                      "Invoke `copyrightedMusicCancelDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore(ZFArgument argument,
                                                                     ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getAverageScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicGetAverageScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetAverageScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize(ZFArgument argument,
                                                                  ZFResult result) {
    if (copyrightedMusic_) {
        auto ret = copyrightedMusic_->getCacheSize();
        result->Success(ZFValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetCacheSize_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCacheSize` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch(ZFArgument argument,
                                                                     ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getCurrentPitch(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicGetCurrentPitch_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration(ZFArgument argument,
                                                                 ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getDuration(resourceID);
        result->Success(ZFValue((int64_t)ret));
    } else {
        result->Error("copyrightedMusicGetDuration_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetDuration` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken(ZFArgument argument,
                                                                        ZFResult result) {
    if (copyrightedMusic_) {
        auto krcToken = zego_value_get_string(argument[ZFValue("krcToken")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getKrcLyricByToken(krcToken, [=](int errorCode, std::string lyrics) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("lyrics")] = ZFValue(lyrics);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicGetKrcLyricByToken_Can_not_find_instance",
            "Invoke `copyrightedMusicGetKrcLyricByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyric(ZFArgument argument,
                                                                 ZFResult result) {
    if (copyrightedMusic_) {
        auto songID = zego_value_get_string(argument[ZFValue("songID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        if (zego_value_is_null(argument[ZFValue("vendorID")])) {
            copyrightedMusic_->getLrcLyric(songID, [=](int errorCode, std::string lyrics) {
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                retMap[ZFValue("lyrics")] = ZFValue(lyrics);
                sharedPtrResult->Success(retMap);
            });
        } else {
            int vendorID = zego_value_get_int(argument[ZFValue("vendorID")]);
            copyrightedMusic_->getLrcLyric(songID, (EXPRESS::ZegoCopyrightedMusicVendorID)vendorID,
                                           [=](int errorCode, std::string lyrics) {
                                               ZFMap retMap;
                                               retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                                               retMap[ZFValue("lyrics")] = ZFValue(lyrics);
                                               sharedPtrResult->Success(retMap);
                                           });
        }

    } else {
        result->Error("copyrightedMusicGetLrcLyric_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetLrcLyric` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyricWithConfig(ZFArgument argument,
                                                                           ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetLyricConfig config;

        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[ZFValue("vendorID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getLrcLyric(config, [=](int errorCode, std::string lyrics) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("lyrics")] = ZFValue(lyrics);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicGetLrcLyric_Can_not_find_instance",
            "Invoke `copyrightedMusicGetLrcLyricWithConfig` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetMusicByToken(ZFArgument argument,
                                                                     ZFResult result) {
    if (copyrightedMusic_) {
        auto shareToken = zego_value_get_string(argument[ZFValue("shareToken")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getMusicByToken(shareToken, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicGetMusicByToken_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetMusicByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetPreviousScore(ZFArgument argument,
                                                                      ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getPreviousScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicGetPreviousScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch(ZFArgument argument,
                                                                      ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getStandardPitch(resourceID, [=](int errorCode, std::string pitch) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("pitch")] = ZFValue(pitch);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicGetStandardPitch_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetStandardPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetTotalScore(ZFArgument argument,
                                                                   ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getTotalScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicGetTotalScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetTotalScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic(ZFArgument argument,
                                                                          ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicConfig config;
        auto userMap = zego_value_get_map(configMap[ZFValue("user")]);
        config.user.userID = zego_value_get_string(userMap[ZFValue("userID")]);
        config.user.userName = zego_value_get_string(userMap[ZFValue("userName")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->initCopyrightedMusic(config, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicInitCopyrightedMusic_Can_not_find_instance",
            "Invoke `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicPauseScore(ZFArgument argument,
                                                                ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->pauseScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicPauseScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicPauseScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache(ZFArgument argument,
                                                                ZFResult result) {
    if (copyrightedMusic_) {
        auto songID = zego_value_get_string(argument[ZFValue("songID")]);
        auto type = zego_value_get_int(argument[ZFValue("type")]);
        bool ret = false;
        if (zego_value_is_null(argument[ZFValue("vendorID")])) {
            ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type);
        } else {
            int vendorID = zego_value_get_int(argument[ZFValue("vendorID")]);
            ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type,
                                                (EXPRESS::ZegoCopyrightedMusicVendorID)vendorID);
        }
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance",
                      "Invoke `copyrightedMusicQueryCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCacheWithConfig(ZFArgument argument,
                                                                          ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfig config;

        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.resourceType = (EXPRESS::ZegoCopyrightedMusicResourceType)zego_value_get_int(
            configMap[ZFValue("resourceType")]);
        config.resourceQualityType =
            (EXPRESS::ZegoCopyrightedMusicResourceQualityType)zego_value_get_int(
                configMap[ZFValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(ZFValue(ret));
    } else {
        result->Error(
            "copyrightedMusicQueryCache_Can_not_find_instance",
            "Invoke `copyrightedMusicQueryCacheWithConfig` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCacheWithConfigV2(ZFArgument argument,
                                                                            ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicQueryCacheConfigV2 config;

        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[ZFValue("vendorID")]);
        config.resourceType = zego_value_get_int(configMap[ZFValue("resourceType")]);
        config.resourceQualityType = zego_value_get_int(configMap[ZFValue("resourceQualityType")]);

        bool ret = copyrightedMusic_->queryCache(config);
        result->Success(ZFValue(ret));
    } else {
        result->Error(
            "copyrightedMusicQueryCache_Can_not_find_instance",
            "Invoke `copyrightedMusicQueryCacheWithConfigV2` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment(ZFArgument argument,
                                                                          ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(
            configMap[ZFValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[ZFValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[ZFValue("sceneID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->requestAccompaniment(config, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestAccompaniment_Can_not_find_instance",
                      "copyrightedMusicRequestAccompaniment `copyrightedMusicInitCopyrightedMusic` "
                      "but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompanimentClip(ZFArgument argument,
                                                                              ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(
            configMap[ZFValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[ZFValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[ZFValue("sceneID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->requestAccompanimentClip(
            config, [=](int errorCode, std::string resource) {
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                retMap[ZFValue("resource")] = ZFValue(resource);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("copyrightedMusicRequestAccompanimentClip_Can_not_find_instance",
                      "copyrightedMusicRequestAccompanimentClip "
                      "`copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestSong(ZFArgument argument,
                                                                 ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(
            configMap[ZFValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[ZFValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[ZFValue("sceneID")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->requestSong(config, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestSong_Can_not_find_instance",
                      "copyrightedMusicRequestSong `copyrightedMusicInitCopyrightedMusic` but "
                      "can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResetScore(ZFArgument argument,
                                                                ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->resetScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicResetScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResetScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore(ZFArgument argument,
                                                                 ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->resumeScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicResumeScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicResumeScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest(ZFArgument argument,
                                                                         ZFResult result) {
    if (copyrightedMusic_) {
        auto command = zego_value_get_string(argument[ZFValue("command")]);
        auto params = zego_value_get_string(argument[ZFValue("params")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->sendExtendedRequest(
            command, params, [=](int errorCode, std::string command, std::string result) {
                ZF::logInfo("*** Plugin OnSendExtendedRequestCallback");
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                retMap[ZFValue("command")] = ZFValue(command);
                retMap[ZFValue("result")] = ZFValue(result);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error("copyrightedMusicSendExtendedRequest_Can_not_find_instance",
                      "copyrightedMusicSendExtendedRequest `copyrightedMusicInitCopyrightedMusic` "
                      "but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSetScoringLevel(ZFArgument argument,
                                                                     ZFResult result) {
    if (copyrightedMusic_) {
        auto level = zego_value_get_int(argument[ZFValue("level")]);
        copyrightedMusic_->setScoringLevel(level);
        result->Success();
    } else {
        result->Error("copyrightedMusicSetScoringLevel_Can_not_find_instance",
                      "Invoke `copyrightedMusicSetScoringLevel` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStartScore(ZFArgument argument,
                                                                ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto pitchValueInterval = zego_value_get_int(argument[ZFValue("pitchValueInterval")]);
        auto ret = copyrightedMusic_->startScore(resourceID, pitchValueInterval);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicStartScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStartScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStopScore(ZFArgument argument,
                                                               ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->stopScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicStopScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicStopScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetFullScore(ZFArgument argument,
                                                                  ZFResult result) {
    if (copyrightedMusic_) {
        auto resourceID = zego_value_get_string(argument[ZFValue("resourceID")]);
        auto ret = copyrightedMusic_->getFullScore(resourceID);
        result->Success(ZFValue(ret));
    } else {
        result->Error("copyrightedMusicGetFullScore_Can_not_find_instance",
                      "Invoke `copyrightedMusicGetFullScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetSharedResource(ZFArgument argument,
                                                                       ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfig config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        auto type = (EXPRESS::ZegoCopyrightedMusicResourceType)zego_value_get_int(
            argument[ZFValue("type")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getSharedResource(config, type,
                                             [=](int errorCode, std::string resource) {
                                                 ZFMap retMap;
                                                 retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                                                 retMap[ZFValue("resource")] = ZFValue(resource);
                                                 sharedPtrResult->Success(retMap);
                                             });
    } else {
        result->Error(
            "copyrightedMusicGetSharedResource_Can_not_find_instance",
            "Invoke `copyrightedMusicGetSharedResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetSharedResourceV2(ZFArgument argument,
                                                                         ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicGetSharedConfigV2 config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.vendorID = zego_value_get_int(configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.resourceType = zego_value_get_int(configMap[ZFValue("resourceType")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->getSharedResource(config, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicGetSharedResource_Can_not_find_instance",
            "Invoke `copyrightedMusicGetSharedResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestResource(ZFArgument argument,
                                                                     ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)zego_value_get_int(
            configMap[ZFValue("mode")]);
        config.vendorID = (EXPRESS::ZegoCopyrightedMusicVendorID)zego_value_get_int(
            configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[ZFValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[ZFValue("sceneID")]);
        auto type = (EXPRESS::ZegoCopyrightedMusicResourceType)zego_value_get_int(
            argument[ZFValue("type")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->requestResource(config, type, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("copyrightedMusicRequestResource_Can_not_find_instance",
                      "Invoke `copyrightedMusicRequestResource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestResourceV2(ZFArgument argument,
                                                                       ZFResult result) {
    if (copyrightedMusic_) {
        auto configMap = zego_value_get_map(argument[ZFValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfigV2 config;
        config.songID = zego_value_get_string(configMap[ZFValue("songID")]);
        config.mode = zego_value_get_int(configMap[ZFValue("mode")]);
        config.vendorID = zego_value_get_int(configMap[ZFValue("vendorID")]);
        config.roomID = zego_value_get_string(configMap[ZFValue("roomID")]);
        config.masterID = zego_value_get_string(configMap[ZFValue("masterID")]);
        config.sceneID = zego_value_get_int(configMap[ZFValue("sceneID")]);
        config.resourceType = zego_value_get_int(configMap[ZFValue("resourceType")]);
        auto sharedPtrResult = ZFMoveResult(result);
        copyrightedMusic_->requestResource(config, [=](int errorCode, std::string resource) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("resource")] = ZFValue(resource);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error(
            "copyrightedMusicRequestResource_Can_not_find_instance",
            "Invoke `copyrightedMusicRequestResourceV2` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createTextureRenderer(ZFArgument argument, ZFResult result) {
    auto width = zego_value_get_int(argument[ZFValue("width")]);
    auto height = zego_value_get_int(argument[ZFValue("height")]);

    auto textureID = ZegoTextureRendererController::getInstance()->createTextureRenderer(
        texture_, width, height);

    result->Success(ZFValue(textureID));
}

void ZegoExpressEngineMethodHandler::destroyTextureRenderer(ZFArgument argument, ZFResult result) {
    auto textureID = zego_value_get_long(argument[ZFValue("textureID")]);
    bool state = ZegoTextureRendererController::getInstance()->destroyTextureRenderer(textureID);

    result->Success(ZFValue(state));
}

void ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl(ZFArgument argument,
                                                                         ZFResult result) {
    auto bitrate = zego_value_get_int(argument[ZFValue("bitrate")]);
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoBitrateForTrafficControl(
        bitrate, (EXPRESS::ZegoTrafficControlMinVideoBitrateMode)mode,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl(ZFArgument argument,
                                                                     ZFResult result) {
    auto fps = zego_value_get_int(argument[ZFValue("fps")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoFpsForTrafficControl(
        fps, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl(ZFArgument argument,
                                                                            ZFResult result) {
    auto width = zego_value_get_int(argument[ZFValue("width")]);
    auto height = zego_value_get_int(argument[ZFValue("height")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoResolutionForTrafficControl(
        width, height, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setTrafficControlFocusOn(ZFArgument argument,
                                                              ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setTrafficControlFocusOn(
        (EXPRESS::ZegoTrafficControlFocusOnMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::addPublishCdnUrl(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto targetURL = zego_value_get_string(argument[ZFValue("targetURL")]);
    auto timeout = zego_value_get_int(argument[ZFValue("timeout")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->addPublishCdnUrl(
        streamID, targetURL, timeout, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::removePublishCdnUrl(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto targetURL = zego_value_get_string(argument[ZFValue("targetURL")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->removePublishCdnUrl(
        streamID, targetURL, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::enablePublishDirectToCDN(ZFArgument argument,
                                                              ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCDNConfig config;
    if (configMap.size() > 0) {
        config.url = zego_value_get_string(configMap[ZFValue("url")]);
        config.authParam = zego_value_get_string(configMap[ZFValue("authParam")]);
        config.customParams = zego_value_get_string(configMap[ZFValue("customParams")]);
        config.protocol = zego_value_get_string(configMap[ZFValue("protocol")]);
        config.quicVersion = zego_value_get_string(configMap[ZFValue("quicVersion")]);
        config.httpdns =
            (EXPRESS::ZegoHttpDNSType)zego_value_get_int(configMap[ZFValue("httpdns")]);
        config.quicConnectMode = (int)zego_value_get_int(configMap[ZFValue("quicConnectMode")]);
    }

    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePublishDirectToCDN(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPublishWatermark(ZFArgument argument, ZFResult result) {
    auto watermarkMap = zego_value_get_map(argument[ZFValue("watermark")]);
    EXPRESS::ZegoWatermark *watermark = nullptr;
    EXPRESS::ZegoWatermark watermarkTemp;
    if (watermarkMap.size() > 0) {
        watermarkTemp.imageURL = zego_value_get_string(watermarkMap[ZFValue("imageURL")]);
        watermarkTemp.layout.x = zego_value_get_int(watermarkMap[ZFValue("left")]);
        watermarkTemp.layout.y = zego_value_get_int(watermarkMap[ZFValue("top")]);
        watermarkTemp.layout.width =
            zego_value_get_int(watermarkMap[ZFValue("right")]) - watermarkTemp.layout.x;
        watermarkTemp.layout.height =
            zego_value_get_int(watermarkMap[ZFValue("bottom")]) - watermarkTemp.layout.y;

        const std::string flutterAssertTaget = "flutter-asset://";
        if (watermarkTemp.imageURL.compare(0, flutterAssertTaget.size(), flutterAssertTaget) == 0) {
            watermarkTemp.imageURL.replace(0, flutterAssertTaget.size(), "");
            std::string flutterAsssetsPath = GetFlutterAssetsPath();
            if (!flutterAsssetsPath.empty()) {
                watermarkTemp.imageURL = "file:///" + flutterAsssetsPath + watermarkTemp.imageURL;
            } else {
                result->Error("setPublishWatermark_get_exe_path_fail",
                              "Failed to get the directory where the application is located");
                return;
            }
        }
        watermark = &watermarkTemp;
    }
    auto isPreviewVisible = zego_value_get_bool(argument[ZFValue("isPreviewVisible")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPublishWatermark(watermark, isPreviewVisible,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey(ZFArgument argument,
                                                                ZFResult result) {
    auto key = zego_value_get_string(argument[ZFValue("key")]);
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamDecryptionKey(streamID, key);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamVideoType(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto streamType = zego_value_get_int(argument[ZFValue("streamType")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamVideoType(
        streamID, (EXPRESS::ZegoVideoStreamType)streamType);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange(ZFArgument argument,
                                                                      ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto minBufferInterval = zego_value_get_int(argument[ZFValue("minBufferInterval")]);
    auto maxBufferInterval = zego_value_get_int(argument[ZFValue("maxBufferInterval")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamBufferIntervalRange(
        streamID, minBufferInterval, maxBufferInterval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamFocusOn(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamFocusOn(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamVideo(ZFArgument argument, ZFResult result) {
    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamVideo(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamVideo(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayVideoStreams(ZFArgument argument, ZFResult result) {
    auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayVideoStreams(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCheckPoc(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCheckPoc(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoDecoderSupported(ZFArgument argument, ZFResult result) {
    auto codecID = zego_value_get_int(argument[ZFValue("codecID")]);
    if (codecID > 4) {
        codecID = 100;
    }

    int ret = 0;
    if (!zego_value_is_null(argument[ZFValue("codecBackend")])) {
        auto codecBackend = zego_value_get_int(argument[ZFValue("codecBackend")]);
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoDecoderSupported(
            (EXPRESS::ZegoVideoCodecID)codecID, (EXPRESS::ZegoVideoCodecBackend)codecBackend);
    } else {
        ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoDecoderSupported(
                  (EXPRESS::ZegoVideoCodecID)codecID)
                  ? 1
                  : 0;
    }

    result->Success(ZFValue(ret));
}

void ZegoExpressEngineMethodHandler::setPlayStreamsAlignmentProperty(ZFArgument argument,
                                                                     ZFResult result) {
    auto mode = zego_value_get_int(argument[ZFValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamsAlignmentProperty(
        (EXPRESS::ZegoStreamAlignmentMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVideoSuperResolution(ZFArgument argument,
                                                                ZFResult result) {
    // auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    // auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    // EXPRESS::ZegoExpressSDK::getEngine()->enableVideoSuperResolution(streamID, enable);

    // result->Success();

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::initVideoSuperResolution(ZFArgument argument,
                                                              ZFResult result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::uninitVideoSuperResolution(ZFArgument argument,
                                                                ZFResult result) {

    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::updatePlayingCanvas(ZFArgument argument, ZFResult result) {

    auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

    ZFMap canvasMap;
    if (!zego_value_is_null(argument[ZFValue("canvas")])) {
        canvasMap = zego_value_get_map(argument[ZFValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    bool isSuccess = false;
    int64_t viewID = 0;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)zego_value_get_int(canvasMap[ZFValue("viewMode")]);
        auto alphaBlend = zego_value_get_bool(canvasMap[ZFValue("alphaBlend")]);

        viewID = zego_value_get_long(canvasMap[ZFValue("view")]);
        ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);
        isSuccess = ZegoTextureRendererController::getInstance()->addRemoteRenderer(
            viewID, streamID, viewMode);

        ZegoTextureRendererController::getInstance()->enableTextureAlpha(alphaBlend, viewID);
    }

    if (isSuccess) {
        result->Success(ZFValue(0));
    } else {
        std::ostringstream oss;
        oss << "The TextureRenderer for textureID: " << viewID
            << "cannot be found, developer should call `createCanvasView` first and get the "
               "textureID";
        result->Error("updatePlayingCanvas_No_TextureRenderer", oss.str());
    }
}

void ZegoExpressEngineMethodHandler::startAutoMixerTask(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = zego_value_get_string(argument[ZFValue("taskID")]);
    task.roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    // MixerOutput
    auto outputList = zego_value_get_list(argument[ZFValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            ZFMap outputMap = zego_value_get_map(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = zego_value_get_string(outputMap[ZFValue("target")]);
            if (!zego_value_is_null(outputMap[ZFValue("videoConfig")])) {
                auto videoConfigMap = zego_value_get_map(outputMap[ZFValue("videoConfig")]);

                auto codecIDIndex = zego_value_get_int(videoConfigMap[ZFValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[ZFValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[ZFValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                        videoConfigMap[ZFValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD =
                    zego_value_get_bool(videoConfigMap[ZFValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }
    // AudioConfig
    auto audioConfigMap = zego_value_get_map(argument[ZFValue("audioConfig")]);
    if (audioConfigMap.size() > 0) {
        task.audioConfig.bitrate = zego_value_get_int(audioConfigMap[ZFValue("bitrate")]);
        task.audioConfig.channel =
            (EXPRESS::ZegoAudioChannel)zego_value_get_int(audioConfigMap[ZFValue("channel")]);
        task.audioConfig.codecID =
            (EXPRESS::ZegoAudioCodecID)zego_value_get_int(audioConfigMap[ZFValue("codecID")]);
    }
    // Enable SoundLevel
    task.enableSoundLevel = zego_value_get_bool(argument[ZFValue("enableSoundLevel")]);
    // streamAlignmentMode
    task.streamAlignmentMode = (EXPRESS::ZegoStreamAlignmentMode)zego_value_get_int(
        argument[ZFValue("streamAlignmentMode")]);
    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength =
        zego_value_get_int(argument[ZFValue("minPlayStreamBufferLength")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->startAutoMixerTask(
        task, [=](int errorCode, std::string extendedData) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("extendedData")] = ZFValue(extendedData);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopAutoMixerTask(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = zego_value_get_string(argument[ZFValue("taskID")]);
    task.roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    // MixerOutput
    auto outputList = zego_value_get_list(argument[ZFValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            ZFMap outputMap = zego_value_get_map(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = zego_value_get_string(outputMap[ZFValue("target")]);
            if (!zego_value_is_null(outputMap[ZFValue("videoConfig")])) {
                auto videoConfigMap = zego_value_get_map(outputMap[ZFValue("videoConfig")]);

                auto codecIDIndex = zego_value_get_int(videoConfigMap[ZFValue("videoCodecID")]);
                if (codecIDIndex > 4) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = zego_value_get_int(videoConfigMap[ZFValue("bitrate")]);
                output.videoConfig.encodeLatency =
                    zego_value_get_int(videoConfigMap[ZFValue("encodeLatency")]);
                output.videoConfig.encodeProfile =
                    (ZEGO::EXPRESS::ZegoEncodeProfile)zego_value_get_int(
                        videoConfigMap[ZFValue("encodeProfile")]);
                output.videoConfig.enableLowBitrateHD =
                    zego_value_get_bool(videoConfigMap[ZFValue("enableLowBitrateHD")]);
            }
            task.outputList.push_back(output);
        }
    }

    // no need to set audio config
    // no need to set video config
    // no need to set enable sound level

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->stopAutoMixerTask(task, [=](int errorCode) {
        ZFMap retMap;
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioRouteType(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::useFrontCamera(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::isCameraFocusSupported(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusMode(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview(ZFArgument argument,
                                                                  ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureMode(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview(ZFArgument argument,
                                                                     ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureCompensation(ZFArgument argument,
                                                                   ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraZoomFactor(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor(ZFArgument argument,
                                                               ZFResult result) {
    auto millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioSpectrumMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor(ZFArgument argument,
                                                              ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioSpectrumMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneAEC(ZFArgument argument, ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startEffectsEnv(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->startEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopEffectsEnv(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableEffectsBeauty(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableEffectsBeauty(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setEffectsBeautyParam(ZFArgument argument, ZFResult result) {
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoEffectsBeautyParam param;
    param.rosyIntensity = zego_value_get_int(paramMap[ZFValue("rosyIntensity")]);
    param.sharpenIntensity = zego_value_get_int(paramMap[ZFValue("sharpenIntensity")]);
    param.smoothIntensity = zego_value_get_int(paramMap[ZFValue("smoothIntensity")]);
    param.whitenIntensity = zego_value_get_int(paramMap[ZFValue("whitenIntensity")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setEffectsBeautyParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableBeautify(ZFArgument argument, ZFResult result) {
    result->Error("Not recommended", "Not recommended, please use startEffectsEnv instead");
}

void ZegoExpressEngineMethodHandler::setBeautifyOption(ZFArgument argument, ZFResult result) {
    result->Error("Not recommended",
                  "Not recommended, please use startEffectsEnv and setEffectsBeautyParam instead");
}

void ZegoExpressEngineMethodHandler::sendBroadcastMessage(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto message = zego_value_get_string(argument[ZFValue("message")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendBroadcastMessage(
        roomID, message, [=](int errorCode, unsigned long long messageID) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("messageID")] = ZFValue((int64_t)messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendBarrageMessage(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto message = zego_value_get_string(argument[ZFValue("message")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendBarrageMessage(
        roomID, message, [=](int errorCode, std::string messageID) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            retMap[ZFValue("messageID")] = ZFValue(messageID);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendCustomCommand(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto command = zego_value_get_string(argument[ZFValue("command")]);
    auto toUserListArray = zego_value_get_list(argument[ZFValue("toUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = zego_value_get_map(user_);
        user.userID = zego_value_get_string(userMap[ZFValue("userID")]);
        user.userName = zego_value_get_string(userMap[ZFValue("userName")]);
        toUserList.push_back(user);
    }

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomCommand(
        roomID, command, toUserList, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::sendTransparentMessage(ZFArgument argument, ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);
    auto sendMode = zego_value_get_int(argument[ZFValue("sendMode")]);
    auto sendType = zego_value_get_int(argument[ZFValue("sendType")]);
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("content")]);

    auto toUserListArray = zego_value_get_list(argument[ZFValue("recvUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = zego_value_get_map(user_);
        user.userID = zego_value_get_string(userMap[ZFValue("userID")]);
        user.userName = zego_value_get_string(userMap[ZFValue("userName")]);
        toUserList.push_back(user);
    }

    auto timeout = zego_value_get_int(argument[ZFValue("timeOut")]);

    auto sharedPtrResult = ZFMoveResult(result);

    ZEGO::EXPRESS::ZegoRoomSendTransparentMessage message;
    message.sendMode = (ZEGO::EXPRESS::ZegoRoomTransparentMessageMode)sendMode;
    message.sendType = (ZEGO::EXPRESS::ZegoRoomTransparentMessageType)sendType;
    message.content.assign((const char *)byteData.data(), (unsigned int)byteData.size());
    message.recvUserList = std::move(toUserList);
    message.timeOut = timeout;

    EXPRESS::ZegoExpressSDK::getEngine()->sendTransparentMessage(
        roomID, message, [=](int errorCode) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::enableCustomVideoCapture(ZFArgument argument,
                                                              ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomVideoCaptureConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[ZFValue("bufferType")]);
    }

    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    if (enable) {
#ifdef _WIN32
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureHandler(
            ZegoCustomVideoCaptureManager::getInstance()->getHandler());
#endif
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureHandler(nullptr);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoCapture(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomVideoProcessing(ZFArgument argument,
                                                                 ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomVideoProcessConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[ZFValue("bufferType")]);
    }

    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    if (enable) {
#ifdef _WIN32
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoProcessHandler(
            ZegoCustomVideoProcessManager::getInstance()->getHandler());
#endif
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoProcessHandler(nullptr);
    }

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoProcessing(
        enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomVideoRender(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomVideoRenderConfig config;
    if (configMap.size() > 0) {
        config.bufferType =
            (EXPRESS::ZegoVideoBufferType)zego_value_get_int(configMap[ZFValue("bufferType")]);
        config.frameFormatSeries = (EXPRESS::ZegoVideoFrameFormatSeries)zego_value_get_int(
            configMap[ZFValue("frameFormatSeries")]);
        config.enableEngineRender = zego_value_get_bool(configMap[ZFValue("enableEngineRender")]);
    }

    if (enable) {
        if (config.enableEngineRender) {
            config.bufferType = EXPRESS::ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
            config.frameFormatSeries = EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
            EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);

            EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(
                ZegoTextureRendererController::getInstance());
#ifdef _WIN32
            ZegoTextureRendererController::getInstance()->setCustomVideoRenderHandler(
                ZegoCustomVideoRenderManager::getInstance()->getHandler());
#endif
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);
#ifdef _WIN32
            EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(
                ZegoCustomVideoRenderManager::getInstance()->getHandler());
#endif
        }
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoRender(false, &config);
        ZegoTextureRendererController::getInstance()->setCustomVideoRenderHandler(nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessing(ZFArgument argument,
                                                                        ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[ZFValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[ZFValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
    ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[ZFValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[ZFValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
        enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlignedAudioAuxData(ZFArgument argument,
                                                               ZFResult result) {
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing(ZFArgument argument,
                                                                       ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[ZFValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[ZFValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioRemoteProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing(ZFArgument argument,
                                                                         ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = zego_value_get_int(configMap[ZFValue("samples")]);
    config.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(configMap[ZFValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(configMap[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioPlaybackProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioIO(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoCustomAudioConfig config;
    config.sourceType =
        (EXPRESS::ZegoAudioSourceType)zego_value_get_int(configMap[ZFValue("sourceType")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioIO(enable, &config,
                                                              (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData(ZFArgument argument,
                                                                   ZFResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
    auto dataLength = zego_value_get_long(argument[ZFValue("dataLength")]);
    auto configLength = zego_value_get_long(argument[ZFValue("configLength")]);
    auto referenceTimeMillisecond = zego_value_get_long(argument[ZFValue("configLength")]);
    auto samples = zego_value_get_long(argument[ZFValue("samples")]);
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[ZFValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[ZFValue("sampleRate")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCaptureAACData(
        byteData.data(), dataLength, configLength, referenceTimeMillisecond, samples, param,
        (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData(ZFArgument argument,
                                                                   ZFResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
    auto dataLength = zego_value_get_long(argument[ZFValue("dataLength")]);
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[ZFValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[ZFValue("sampleRate")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCapturePCMData(
        byteData.data(), dataLength, param, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData(ZFArgument argument,
                                                                   ZFResult result) {
    auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
    auto dataLength = zego_value_get_long(argument[ZFValue("dataLength")]);
    auto paramMap = zego_value_get_map(argument[ZFValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)zego_value_get_int(paramMap[ZFValue("channel")]);
    param.sampleRate =
        (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(paramMap[ZFValue("sampleRate")]);

    EXPRESS::ZegoExpressSDK::getEngine()->fetchCustomAudioRenderPCMData(byteData.data(), dataLength,
                                                                        param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPerformanceMonitor(ZFArgument argument, ZFResult result) {
    auto millisecond = zego_value_get_long(argument[ZFValue("millisecond")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startPerformanceMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPerformanceMonitor(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopPerformanceMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkProbe(ZFArgument argument, ZFResult result) {
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoNetworkProbeConfig config;
    config.enableTraceroute = zego_value_get_bool(configMap[ZFValue("enableTraceroute")]);

    auto sharedPtrResult = ZFMoveResult(result);
    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkProbe(
        config, [=](int errorCode, const EXPRESS::ZegoNetworkProbeResult &result) {
            ZFMap retMap;
            retMap[ZFValue("errorCode")] = ZFValue(errorCode);
            if (result.httpProbeResult) {
                ZFMap httpProbeResultMap;
                httpProbeResultMap[ZFValue("errorCode")] =
                    ZFValue(result.httpProbeResult->errorCode);
                httpProbeResultMap[ZFValue("requestCostTime")] =
                    ZFValue((int64_t)result.httpProbeResult->requestCostTime);
                retMap[ZFValue("httpProbeResult")] = ZFValue(httpProbeResultMap);
            }
            if (result.tcpProbeResult) {
                ZFMap tcpProbeResultMap;
                tcpProbeResultMap[ZFValue("errorCode")] = ZFValue(result.tcpProbeResult->errorCode);
                tcpProbeResultMap[ZFValue("rtt")] = ZFValue((int64_t)result.tcpProbeResult->rtt);
                tcpProbeResultMap[ZFValue("connectCostTime")] =
                    ZFValue((int64_t)result.tcpProbeResult->connectCostTime);
                retMap[ZFValue("tcpProbeResult")] = ZFValue(tcpProbeResultMap);
            }
            if (result.udpProbeResult) {
                ZFMap udpProbeResultMap;
                udpProbeResultMap[ZFValue("errorCode")] = ZFValue(result.udpProbeResult->errorCode);
                udpProbeResultMap[ZFValue("rtt")] = ZFValue((int64_t)result.udpProbeResult->rtt);
                retMap[ZFValue("udpProbeResult")] = ZFValue(udpProbeResultMap);
            }
            if (result.tracerouteResult) {
                ZFMap tracerouteResultMap;
                tracerouteResultMap[ZFValue("errorCode")] =
                    ZFValue(result.tracerouteResult->errorCode);
                tracerouteResultMap[ZFValue("tracerouteCostTime")] =
                    ZFValue((int64_t)result.tracerouteResult->tracerouteCostTime);
                retMap[ZFValue("tracerouteResult")] = ZFValue(tracerouteResultMap);
            }
            sharedPtrResult->Success(retMap);
        });
}

void ZegoExpressEngineMethodHandler::stopNetworkProbe(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkProbe();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkSpeedTest(ZFArgument argument, ZFResult result) {
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoNetworkSpeedTestConfig config;
    config.testUplink = zego_value_get_bool(configMap[ZFValue("testUplink")]);
    config.expectedUplinkBitrate = zego_value_get_int(configMap[ZFValue("expectedUplinkBitrate")]);
    config.testDownlink = zego_value_get_bool(configMap[ZFValue("testDownlink")]);
    config.expectedDownlinkBitrate =
        zego_value_get_int(configMap[ZFValue("expectedDownlinkBitrate")]);
    auto interval = zego_value_get_long(argument[ZFValue("interval")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkSpeedTest(config, interval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopNetworkSpeedTest(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkSpeedTest();

    result->Success();
}

void ZegoExpressEngineMethodHandler::getNetworkTimeInfo(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoNetworkTimeInfo info = EXPRESS::ZegoExpressSDK::getEngine()->getNetworkTimeInfo();

    ZFMap retMap;
    retMap[ZFValue("timestamp")] = ZFValue((int64_t)info.timestamp);
    retMap[ZFValue("maxDeviation")] = ZFValue((int32_t)info.maxDeviation);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::startDumpData(ZFArgument argument, ZFResult result) {
    auto configMap = zego_value_get_map(argument[ZFValue("config")]);
    EXPRESS::ZegoDumpDataConfig config;
    config.dataType = (EXPRESS::ZegoDumpDataType)zego_value_get_int(configMap[ZFValue("dataType")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startDumpData(config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopDumpData(ZFArgument argument, ZFResult result) {

    EXPRESS::ZegoExpressSDK::getEngine()->stopDumpData();

    result->Success();
}

void ZegoExpressEngineMethodHandler::uploadDumpData(ZFArgument argument, ZFResult result) {

    EXPRESS::ZegoExpressSDK::getEngine()->uploadDumpData();

    result->Success();
}

void ZegoExpressEngineMethodHandler::removeDumpData(ZFArgument argument, ZFResult result) {

    EXPRESS::ZegoExpressSDK::getEngine()->removeDumpData();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createRangeAudio(ZFArgument argument, ZFResult result) {
    rangeAudio_ = EXPRESS::ZegoExpressSDK::getEngine()->createRangeAudio();
    if (rangeAudio_) {
        rangeAudio_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(ZFValue(0));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyRangeAudio(ZFArgument argument, ZFResult result) {
    EXPRESS::ZegoExpressSDK::getEngine()->destroyRangeAudio(rangeAudio_);
    rangeAudio_ = nullptr;
    result->Success();
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone(ZFArgument argument,
                                                                ZFResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

        rangeAudio_->enableMicrophone(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableMicrophone_Can_not_find_instance",
                      "Invoke `rangeAudioEnableMicrophone` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer(ZFArgument argument,
                                                                 ZFResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        rangeAudio_->enableSpatializer(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpatializer_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpatializer` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker(ZFArgument argument, ZFResult result) {
    if (rangeAudio_) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);

        rangeAudio_->enableSpeaker(enable);

        result->Success();
    } else {
        result->Error("rangeAudioEnableSpeaker_Can_not_find_instance",
                      "Invoke `rangeAudioEnableSpeaker` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange(ZFArgument argument,
                                                                    ZFResult result) {
    if (rangeAudio_) {
        auto paramMap = zego_value_get_map(argument[ZFValue("param")]);

        EXPRESS::ZegoReceiveRangeParam param;
        param.min = (float)zego_value_get_double(paramMap[ZFValue("min")]);
        param.max = (float)zego_value_get_double(paramMap[ZFValue("max")]);

        int ret = rangeAudio_->setAudioReceiveRange(param);

        result->Success(ZFValue(ret));
    } else {
        result->Error("rangeAudioSetAudioReceiveRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetMode(ZFArgument argument, ZFResult result) {
    if (rangeAudio_) {
        auto mode = zego_value_get_int(argument[ZFValue("mode")]);

        rangeAudio_->setRangeAudioMode((EXPRESS::ZegoRangeAudioMode)mode);

        result->Success();
    } else {
        result->Error("rangeAudioSetMode_Can_not_find_instance",
                      "Invoke `rangeAudioSetMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetTeamID(ZFArgument argument, ZFResult result) {
    if (rangeAudio_) {
        auto teamID = zego_value_get_string(argument[ZFValue("teamID")]);

        rangeAudio_->setTeamID(teamID);

        result->Success();
    } else {
        result->Error("rangeAudioSetTeamID_Can_not_find_instance",
                      "Invoke `rangeAudioSetTeamID` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource(ZFArgument argument,
                                                                 ZFResult result) {
    if (rangeAudio_) {
        auto userID = zego_value_get_string(argument[ZFValue("userID")]);
        auto position = zego_value_get_vector_float(argument[ZFValue("position")]);

        rangeAudio_->updateAudioSource(userID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition(ZFArgument argument,
                                                                  ZFResult result) {
    if (rangeAudio_) {
        auto position = zego_value_get_vector_float(argument[ZFValue("position")]);
        auto axisForward = zego_value_get_vector_float(argument[ZFValue("axisForward")]);
        auto axisRight = zego_value_get_vector_float(argument[ZFValue("axisRight")]);
        auto axisUp = zego_value_get_vector_float(argument[ZFValue("axisUp")]);

        rangeAudio_->updateSelfPosition(position.data(), axisForward.data(), axisRight.data(),
                                        axisUp.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioMuteUser(ZFArgument argument, ZFResult result) {
    if (rangeAudio_) {
        auto userID = zego_value_get_string(argument[ZFValue("userID")]);
        auto mute = zego_value_get_bool(argument[ZFValue("mute")]);

        rangeAudio_->muteUser(userID, mute);

        result->Success();
    } else {
        result->Error("rangeAudioMuteUser_Can_not_find_instance",
                      "Invoke `rangeAudioMuteUser` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency(ZFArgument argument,
                                                                          ZFResult result) {
    if (rangeAudio_) {
        auto frequency = zego_value_get_int(argument[ZFValue("frequency")]);

        rangeAudio_->setPositionUpdateFrequency(frequency);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetPositionUpdateFrequency_Can_not_find_instance",
            "Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioVolume(ZFArgument argument,
                                                                   ZFResult result) {
    if (rangeAudio_) {
        auto volume = zego_value_get_int(argument[ZFValue("volume")]);

        rangeAudio_->setRangeAudioVolume(volume);

        result->Success();
    } else {
        result->Error("rangeAudioSetRangeAudioVolume_Can_not_find_instance",
                      "Invoke `rangeAudioSetRangeAudioVolume` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetStreamVocalRange(ZFArgument argument,
                                                                   ZFResult result) {
    if (rangeAudio_) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);
        auto paramMap = zego_value_get_map(argument[ZFValue("param")]);

        EXPRESS::ZegoVocalRangeParam param;
        param.min = (float)zego_value_get_double(paramMap[ZFValue("min")]);
        param.max = (float)zego_value_get_double(paramMap[ZFValue("max")]);

        int ret = rangeAudio_->setStreamVocalRange(streamID, param);

        result->Success(ZFValue(ret));
    } else {
        result->Error("rangeAudioSetStreamVocalRange_Can_not_find_instance",
                      "Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateStreamPosition(ZFArgument argument,
                                                                    ZFResult result) {
    if (rangeAudio_) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        auto position = zego_value_get_vector_float(argument[ZFValue("position")]);

        rangeAudio_->updateStreamPosition(streamID, position.data());

        result->Success();
    } else {
        result->Error("rangeAudioUpdateStreamPosition_Can_not_find_instance",
                      "Invoke `rangeAudioUpdateStreamPosition` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetRangeAudioCustomMode(ZFArgument argument,
                                                                       ZFResult result) {
    if (rangeAudio_) {
        auto speakMode =
            (EXPRESS::ZegoRangeAudioSpeakMode)zego_value_get_int(argument[ZFValue("speakMode")]);

        auto listenMode =
            (EXPRESS::ZegoRangeAudioListenMode)zego_value_get_int(argument[ZFValue("listenMode")]);

        rangeAudio_->setRangeAudioCustomMode(speakMode, listenMode);

        result->Success();
    } else {
        result->Error(
            "rangeAudioSetRangeAudioCustomMode_Can_not_find_instance",
            "Invoke `rangeAudioSetRangeAudioCustomMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager(ZFArgument argument,
                                                                         ZFResult result) {
    auto roomID = zego_value_get_string(argument[ZFValue("roomID")]);

    auto dataManager =
        EXPRESS::ZegoExpressSDK::getEngine()->createRealTimeSequentialDataManager(roomID);
    if (dataManager) {
        dataManager->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        dataManagerMap_[dataManager->getIndex()] = dataManager;
        result->Success(ZFValue(dataManager->getIndex()));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyRealTimeSequentialDataManager(ZFArgument argument,
                                                                          ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyRealTimeSequentialDataManager(
            dataManagerMap_[index]);
        dataManagerMap_.erase(index);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData(ZFArgument argument,
                                                                           ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto byteData = zego_value_get_vector_uint8(argument[ZFValue("data")]);
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        auto sharedPtrResult = ZFMoveResult(result);
        dataManagerMap_[index]->sendRealTimeSequentialData(
            byteData.data(), byteData.size(), streamID, [=](int errorCode) {
                ZFMap retMap;
                retMap[ZFValue("errorCode")] = ZFValue(errorCode);
                sharedPtrResult->Success(retMap);
            });
    } else {
        result->Error(
            "dataManagerSendRealTimeSequentialData_Can_not_find_instance",
            "Invoke `dataManagerSendRealTimeSequentialData` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartBroadcasting(ZFArgument argument,
                                                                  ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        dataManagerMap_[index]->startBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStartBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartSubscribing(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        dataManagerMap_[index]->startSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStartSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting(ZFArgument argument,
                                                                 ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        dataManagerMap_[index]->stopBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopBroadcasting_Can_not_find_instance",
                      "Invoke `dataManagerStopBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopSubscribing(ZFArgument argument,
                                                                ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = zego_value_get_string(argument[ZFValue("streamID")]);

        dataManagerMap_[index]->stopSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopSubscribing_Can_not_find_instance",
                      "Invoke `dataManagerStopSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::getScreenCaptureSources(ZFArgument argument, ZFResult result) {
#ifdef _WIN32
    auto thumbnailWidth = zego_value_get_int(argument[ZFValue("thumbnailWidth")]);
    auto thumbnailHeight = zego_value_get_int(argument[ZFValue("thumbnailHeight")]);
    auto iconWidth = zego_value_get_int(argument[ZFValue("iconWidth")]);
    auto iconHeight = zego_value_get_int(argument[ZFValue("iconHeight")]);

    auto infos = EXPRESS::ZegoExpressSDK::getEngine()->getScreenCaptureSources(
        thumbnailWidth, thumbnailHeight, iconWidth, iconHeight);
    ZFArray resultArray;
    for (auto &info : infos) {
        ZFMap infoMap;
        infoMap[ZFValue("sourceType")] = ZFValue(static_cast<int32_t>(info.sourceType));
        infoMap[ZFValue("sourceID")] = ZFValue(reinterpret_cast<intptr_t>(info.sourceID));
        infoMap[ZFValue("sourceName")] = ZFValue(info.sourceName);

        if (info.thumbnailImage.length >= 0 && info.thumbnailImage.buffer != nullptr) {
            auto thumbnailImageData = makeBtimapByBGRABuffer(
                info.thumbnailImage.buffer, info.thumbnailImage.length,
                std::pair(info.thumbnailImage.width, info.thumbnailImage.height));
            std::vector<uint8_t> thumbnailImage(
                thumbnailImageData.second, thumbnailImageData.second + thumbnailImageData.first);
            delete[] thumbnailImageData.second;
            infoMap[ZFValue("thumbnailImage")] = ZFValue(thumbnailImage);
        }

        if (info.iconImage.length >= 0 && info.iconImage.buffer != nullptr) {
            auto iconImageData =
                makeIconByBGRABuffer(info.iconImage.buffer, info.iconImage.length,
                                     std::pair(info.iconImage.width, info.iconImage.height));
            std::vector<uint8_t> iconImage(iconImageData.second,
                                           iconImageData.second + iconImageData.first);
            delete[] iconImageData.second;
            infoMap[ZFValue("iconImage")] = ZFValue(iconImage);
        }

        resultArray.emplace_back(ZFValue(infoMap));
    }

    result->Success(resultArray);
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::createScreenCaptureSource(ZFArgument argument,
                                                               ZFResult result) {
#ifdef _WIN32
    void *sourceId = reinterpret_cast<void *>(
        static_cast<intptr_t>(zego_value_get_long(argument[ZFValue("sourceId")])));
    auto sourceType = zego_value_get_int(argument[ZFValue("sourceType")]);
    auto screenCaptureSource = EXPRESS::ZegoExpressSDK::getEngine()->createScreenCaptureSource(
        sourceId, (EXPRESS::ZegoScreenCaptureSourceType)sourceType);
    if (screenCaptureSource) {
        auto index = screenCaptureSource->getIndex();

        screenCaptureSource->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        screenCaptureSourceMap_[index] = screenCaptureSource;

        result->Success(ZFValue(index));
    } else {
        result->Success(ZFValue(-1));
    }
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::destroyScreenCaptureSource(ZFArgument argument,
                                                                ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyScreenCaptureSource(screenCaptureSource);
    }

    screenCaptureSourceMap_.erase(index);

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::enableCursorVisibleScreenCaptureSource(ZFArgument argument,
                                                                            ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto visible = zego_value_get_bool(argument[ZFValue("visible")]);
        screenCaptureSource->enableCursorVisible(visible);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::enableWindowActivateScreenCaptureSource(ZFArgument argument,
                                                                             ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto active = zego_value_get_bool(argument[ZFValue("active")]);
        screenCaptureSource->enableWindowActivate(active);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::setExcludeWindowListScreenCaptureSource(ZFArgument argument,
                                                                             ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto list = zego_value_get_list(argument[ZFValue("list")]);
        intptr_t *c_list = new intptr_t[list.size()];
        intptr_t *tmpPtr = c_list;
        for (auto win : list) {
            *tmpPtr = static_cast<intptr_t>(zego_value_get_long(win));
            tmpPtr += 1;
        }

        screenCaptureSource->setExcludeWindowList((void **)c_list, list.size());
        delete[] c_list;
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::updateCaptureRegionScreenCaptureSource(ZFArgument argument,
                                                                            ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = zego_value_get_map(argument[ZFValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = zego_value_get_double(rectMap[ZFValue("x")]);
        rect.y = zego_value_get_double(rectMap[ZFValue("y")]);
        rect.width = zego_value_get_double(rectMap[ZFValue("width")]);
        rect.height = zego_value_get_double(rectMap[ZFValue("height")]);
        screenCaptureSource->updateCaptureRegion(rect);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::updatePublishRegionScreenCaptureSource(ZFArgument argument,
                                                                            ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rectMap = zego_value_get_map(argument[ZFValue("rect")]);
        EXPRESS::ZegoRect rect;
        rect.x = zego_value_get_double(rectMap[ZFValue("x")]);
        rect.y = zego_value_get_double(rectMap[ZFValue("y")]);
        rect.width = zego_value_get_double(rectMap[ZFValue("width")]);
        rect.height = zego_value_get_double(rectMap[ZFValue("height")]);
        screenCaptureSource->updatePublishRegion(rect);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::updateCaptureSourceScreenCaptureSource(ZFArgument argument,
                                                                            ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        void *sourceId = reinterpret_cast<void *>(
            static_cast<intptr_t>(zego_value_get_long(argument[ZFValue("sourceId")])));
        auto sourceType = zego_value_get_int(argument[ZFValue("sourceType")]);
        screenCaptureSource->updateCaptureSource(sourceId,
                                                 (EXPRESS::ZegoScreenCaptureSourceType)sourceType);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::startCaptureScreenCaptureSource(ZFArgument argument,
                                                                     ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->startCapture();
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::stopCaptureScreenCaptureSource(ZFArgument argument,
                                                                    ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        screenCaptureSource->stopCapture();
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::getCaptureSourceRectScreenCaptureSource(ZFArgument argument,
                                                                             ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto rect = screenCaptureSource->getCaptureSourceRect();
        ZFMap resultMap;
        resultMap[ZFValue("x")] = ZFValue((int32_t)rect.x);
        resultMap[ZFValue("y")] = ZFValue((int32_t)rect.y);
        resultMap[ZFValue("width")] = ZFValue((int32_t)rect.width);
        resultMap[ZFValue("height")] = ZFValue((int32_t)rect.height);
        result->Success(resultMap);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureScreenCaptureSource(ZFArgument argument,
                                                                           ZFResult result) {
#ifdef _WIN32
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto screenCaptureSource = screenCaptureSourceMap_[index];

    if (screenCaptureSource) {
        auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
        auto param = zego_value_get_map(argument[ZFValue("audioParam")]);

        EXPRESS::ZegoAudioFrameParam nativeParam;
        nativeParam.sampleRate =
            (EXPRESS::ZegoAudioSampleRate)zego_value_get_int(param[ZFValue("sampleRate")]);
        nativeParam.channel =
            (EXPRESS::ZegoAudioChannel)zego_value_get_int(param[ZFValue("channel")]);

        screenCaptureSource->enableAudioCapture(enable, nativeParam);
    }

    result->Success();
#else
    result->Error("not_support_feature", "linux platform not support feature");
#endif
}

void ZegoExpressEngineMethodHandler::createAIVoiceChanger(ZFArgument argument, ZFResult result) {
    auto aiVoiceChanger = EXPRESS::ZegoExpressSDK::getEngine()->createAIVoiceChanger();
    if (aiVoiceChanger) {
        auto index = aiVoiceChanger->getIndex();

        aiVoiceChanger->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        aiVoiceChangerMap_[index] = aiVoiceChanger;

        result->Success(ZFValue(index));
    } else {
        result->Success(ZFValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyAIVoiceChanger(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->setEventHandler(nullptr);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyAIVoiceChanger(aiVoiceChanger);
    }

    aiVoiceChangerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAIVoiceChangerSupported(ZFArgument argument,
                                                               ZFResult result) {
    bool supported = EXPRESS::ZegoExpressSDK::getEngine()->isAIVoiceChangerSupported();
    result->Success(ZFValue(supported));
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerInitEngine(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->initEngine();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerUpdate(ZFArgument argument, ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->update();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerSetSpeaker(ZFArgument argument,
                                                              ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        auto speakerID = zego_value_get_int(argument[ZFValue("speakerID")]);
        aiVoiceChanger->setSpeaker(speakerID);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::aiVoiceChangerGetSpeakerList(ZFArgument argument,
                                                                  ZFResult result) {
    auto index = zego_value_get_int(argument[ZFValue("index")]);
    auto aiVoiceChanger = aiVoiceChangerMap_[index];

    if (aiVoiceChanger) {
        aiVoiceChanger->getSpeakerList();
    }

    result->Success();
}
void ZegoExpressEngineMethodHandler::enableColorEnhancement(ZFArgument argument, ZFResult result) {
    auto enable = zego_value_get_bool(argument[ZFValue("enable")]);
    auto params = zego_value_get_map(argument[ZFValue("params")]);
    auto channel = zego_value_get_int(argument[ZFValue("channel")]);

    EXPRESS::ZegoColorEnhancementParams p;
    p.intensity = zego_value_get_double(params[ZFValue("intensity")]);
    p.skinToneProtectionLevel = zego_value_get_double(params[ZFValue("skinToneProtectionLevel")]);
    p.lipColorProtectionLevel = zego_value_get_double(params[ZFValue("lipColorProtectionLevel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableColorEnhancement(
        enable, p, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}
