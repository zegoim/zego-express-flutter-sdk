#pragma once

// #include <ZegoExpressSDK.h>
// #define MSVN
// #include <flutter/encodable_value.h>
// #include <flutter/method_channel.h>
// #include <flutter/plugin_registrar_windows.h>
// #else
// #endif

#include "../ZegoDataTypeConvert.h"

// using namespace ZEGO;

class ZegoExpressEngineMethodHandler {
  public:
    ~ZegoExpressEngineMethodHandler() {}

    static ZegoExpressEngineMethodHandler &getInstance() {
        static ZegoExpressEngineMethodHandler m_instance;
        return m_instance;
    }

    // void setPluginRegistrar(flutter::PluginRegistrarWindows *registrar) { registrar_ = registrar; }

    void clearPluginRegistrar();

    void initApiCalledCallback();

    bool isEngineCreated();

  public:
    void getVersion(FTArgument argument,
                    FTResult result);
    void isFeatureSupported(FTArgument argument,
                            FTResult result);

  private:
    ZegoExpressEngineMethodHandler() = default;
};