#pragma once

#include <flutter/method_channel.h>

#include <ZegoExpressSDK.h>
using namespace ZEGO;

class ZegoExpressEngineMethodHandler 
{
public:
    ~ZegoExpressEngineMethodHandler(){}

    static ZegoExpressEngineMethodHandler & getInstance()
    {
        static ZegoExpressEngineMethodHandler m_instance;
        return m_instance;
    }

public:
    std::string getVersion();
    void createEngine(const flutter::MethodCall<flutter::EncodableValue>& method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);


private:
    ZegoExpressEngineMethodHandler() = default;
};