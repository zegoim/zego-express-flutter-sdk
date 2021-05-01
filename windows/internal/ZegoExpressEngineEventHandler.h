#pragma once

#include <flutter/event_channel.h>

#include <ZegoExpressSDK.h>
using namespace ZEGO;

class ZegoExpressEngineEventHandler 
{
public:
    ~ZegoExpressEngineEventHandler(){}

    static ZegoExpressEngineEventHandler& getInstance()
    {
        static ZegoExpressEngineEventHandler m_instance;
        return m_instance;
    }

    void setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&eventSink);
    void clearEventSink();

private:
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> eventSink_;

private:
    ZegoExpressEngineEventHandler() = default;
};