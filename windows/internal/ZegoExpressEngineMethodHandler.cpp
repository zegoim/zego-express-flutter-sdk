#include "ZegoExpressEngineMethodHandler.h"

#include <variant>
#include <flutter/encodable_value.h>

std::string ZegoExpressEngineMethodHandler::getVersion()
{
    return EXPRESS::ZegoExpressSDK::getVersion();
}

void ZegoExpressEngineMethodHandler::createEngine(const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto argument = std::get<flutter::EncodableMap>(*method_call.arguments());
    unsigned int appID = (unsigned int)std::get<int64_t>(argument["appID"]);
    std::string appSign = std::get<std::string>(argument["appSign"]);
    bool isTestEnv = std::get<bool>(argument["isTestEnv"]);
    int scenario = std::get<int32_t>(argument["scenario"]);

    EXPRESS::ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario, nullptr);
}