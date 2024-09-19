#include "include/zego_express_engine/zego_express_engine_plugin.h"

#include <windows.h>

#include <VersionHelpers.h>

#include <flutter/event_channel.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "internal/ZegoLog.h"
#include "internal/ZegoExpressEngineEventHandler.h"
#include "internal/ZegoExpressEngineMethodHandler.h"
#include "internal/ZegoExpressEngineInterface.h"

class ZegoExpressEnginePlugin : public flutter::Plugin,
                                public flutter::StreamHandler<flutter::EncodableValue> {

  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    ZegoExpressEnginePlugin();

    virtual ~ZegoExpressEnginePlugin();

  protected:
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListenInternal(
        const flutter::EncodableValue *arguments,
        std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events) override;
    // Implementation of the public interface, to be provided by subclasses.
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
    OnCancelInternal(const flutter::EncodableValue *arguments) override;

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call,
                          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  public:
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> methodChannel_;
    std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> eventChannel_;

  private:
    std::shared_ptr<ZegoExpressEngineEventHandler> eventHandler_;
};

// static
void ZegoExpressEnginePlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {

    ZegoExpressEngineMethodHandler::getInstance().setPluginBinaryMessenger(registrar->messenger());
    ZegoExpressEngineMethodHandler::getInstance().setPluginTexture(registrar->texture_registrar());

    auto plugin = std::make_unique<ZegoExpressEnginePlugin>();

    plugin->methodChannel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
        registrar->messenger(), "plugins.zego.im/zego_express_engine",
        &flutter::StandardMethodCodec::GetInstance());

    plugin->eventChannel_ = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
        registrar->messenger(), "plugins.zego.im/zego_express_event_handler",
        &flutter::StandardMethodCodec::GetInstance());

    plugin->methodChannel_->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
            plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    plugin->eventChannel_->SetStreamHandler(std::move(plugin));

    ZegoExpressEngineMethodHandler::getInstance().initApiCalledCallback();

    registrar->AddPlugin(std::move(plugin));
}

ZegoExpressEnginePlugin::ZegoExpressEnginePlugin() {}

ZegoExpressEnginePlugin::~ZegoExpressEnginePlugin() {
    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    ZegoExpressEngineMethodHandler::getInstance().clearPluginRegistrar();
}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
ZegoExpressEnginePlugin::OnListenInternal(
    const flutter::EncodableValue *arguments,
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events) {

    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    std::cout << "on listen event" << std::endl;

    return nullptr;
}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
ZegoExpressEnginePlugin::OnCancelInternal(const flutter::EncodableValue *arguments) {

    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    std::cout << "on cancel listen event" << std::endl;

    return nullptr;
}

void ZegoExpressEnginePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    ZF::logInfo("[DartCall][%s]", method_call.method_name().c_str());

    flutter::EncodableMap argument;
    if (std::holds_alternative<flutter::EncodableMap>(*method_call.arguments())) {
        argument = std::get<flutter::EncodableMap>(*method_call.arguments());
    }

    auto pair = G_MethodMap.find(method_call.method_name());
    if (pair != G_MethodMap.end()) {
        try {
            if (!pair->second.second &&
                !ZegoExpressEngineMethodHandler::getInstance().isEngineCreated()) {
                result->Error("Engine_not_created", "Please call createEngineWithProfile first");
            } else {
                pair->second.first(argument, std::move(result));
            }
        } catch (std::exception &e) {
            result->Error("method_call_error", e.what());
        }
    } else {
        result->NotImplemented();
    }
}

void ZegoExpressEnginePluginRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar) {
    ZegoExpressEnginePlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()
            ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
