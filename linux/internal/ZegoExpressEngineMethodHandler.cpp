#include "ZegoExpressEngineMethodHandler.h"

// #include <flutter/plugin_registrar_windows.h>
#include <functional>
#include <variant>

// #include <Windows.h>
// #include "../ZegoLog.h"
// #include "DataToImageTools.hpp"

void ZegoExpressEngineMethodHandler::clearPluginRegistrar() {
    // ZegoTextureRendererController::getInstance()->uninit();
}

void ZegoExpressEngineMethodHandler::initApiCalledCallback() {
    // EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());
}

bool ZegoExpressEngineMethodHandler::isEngineCreated() {
    // return EXPRESS::ZegoExpressSDK::getEngine();
    return true;
}

void ZegoExpressEngineMethodHandler::getVersion(
    FTArgument argument,
    FTResult result) {
    result->Success(FTValue("3.14.0"));
}

void ZegoExpressEngineMethodHandler::isFeatureSupported(
    FTArgument argument,
    FTResult result) {
    FlValue *type = argument[FTValue("featureType")];
    auto featureType = zego_value_get_int(type);
    FTMap map;
    map[FTValue("123")] = FTValue(featureType);
    map[FTValue("111")] = FTValue(true);
    result->Success(map[FTValue("111")]);
}
