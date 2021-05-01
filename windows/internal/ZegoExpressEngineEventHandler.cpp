#include "ZegoExpressEngineEventHandler.h"

#include <memory>

void ZegoExpressEngineEventHandler::setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& eventSink)
{
	eventSink_ = std::move(eventSink);
}

void ZegoExpressEngineEventHandler::clearEventSink()
{
	eventSink_.reset();
}