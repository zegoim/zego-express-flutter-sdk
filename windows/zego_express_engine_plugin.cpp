#include "include/zego_express_engine/zego_express_engine_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/event_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "internal/ZegoExpressEngineMethodHandler.h"
#include "internal/ZegoExpressEngineEventHandler.h"

//namespace {

class ZegoExpressEnginePlugin : public flutter::Plugin, public flutter::StreamHandler<flutter::EncodableValue> {
 
public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    ZegoExpressEnginePlugin();

  
    virtual ~ZegoExpressEnginePlugin();

protected:
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListenInternal(
        const flutter::EncodableValue* arguments,
        std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events) override;

    // Implementation of the public interface, to be provided by subclasses.
    virtual std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnCancelInternal(
        const flutter::EncodableValue* arguments) override;


private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

private:

private:
    std::shared_ptr<ZegoExpressEngineEventHandler> eventHandler_;

};

// static
void ZegoExpressEnginePlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar) {
    auto methodChannel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>
      (registrar->messenger(), "plugins.zego.im/zego_express_engine", &flutter::StandardMethodCodec::GetInstance());

    auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>
      (registrar->messenger(), "plugins.zego.im/zego_express_event_handler", &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<ZegoExpressEnginePlugin>();

    eventChannel->SetStreamHandler(std::move(plugin));

    methodChannel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
            plugin_pointer->HandleMethodCall(call, std::move(result));
        });


    registrar->AddPlugin(std::move(plugin));
}

ZegoExpressEnginePlugin::ZegoExpressEnginePlugin() {}

ZegoExpressEnginePlugin::~ZegoExpressEnginePlugin() {}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> ZegoExpressEnginePlugin::OnListenInternal(
    const flutter::EncodableValue* arguments,
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events) {

    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    std::cout << "on listen event" << std::endl;

    return nullptr;
}

std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> ZegoExpressEnginePlugin::OnCancelInternal(
    const flutter::EncodableValue* arguments) {

    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    std::cout << "on cancel listen event" << std::endl;

    return nullptr;
}

void ZegoExpressEnginePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

    flutter::EncodableMap argument;
    if (std::holds_alternative<flutter::EncodableMap>(*method_call.arguments())) {
        argument = std::get<flutter::EncodableMap>(*method_call.arguments());
    }

    if(method_call.method_name() == "getVersion") {
      ZegoExpressEngineMethodHandler::getInstance().getVersion(argument, std::move(result));
    }
    else if (method_call.method_name() == "createEngine") {
        ZegoExpressEngineMethodHandler::getInstance().createEngine(argument, std::move(result));
    }
    else if (method_call.method_name() == "destroyEngine") {
        ZegoExpressEngineMethodHandler::getInstance().destroyEngine(argument, std::move(result));
    }
    else if (method_call.method_name() == "setEngineConfig") {
        ZegoExpressEngineMethodHandler::getInstance().setEngineConfig(argument, std::move(result));
    }
    else if (method_call.method_name() == "uploadLog") {
        ZegoExpressEngineMethodHandler::getInstance().uploadLog(argument, std::move(result));
    }
    else if (method_call.method_name() == "loginRoom") {
        ZegoExpressEngineMethodHandler::getInstance().loginRoom(argument, std::move(result));
    }
    else if (method_call.method_name() == "logoutRoom") {
        ZegoExpressEngineMethodHandler::getInstance().logoutRoom(argument, std::move(result));
    }
    else if (method_call.method_name() == "switchRoom") {
        ZegoExpressEngineMethodHandler::getInstance().switchRoom(argument, std::move(result));
    }
    else if (method_call.method_name() == "setRoomExtraInfo") {
        ZegoExpressEngineMethodHandler::getInstance().setRoomExtraInfo(argument, std::move(result));
    }
    else if (method_call.method_name() == "startPublishingStream") {
        ZegoExpressEngineMethodHandler::getInstance().startPublishingStream(argument, std::move(result));
    }
    else if (method_call.method_name() == "stopPublishingStream") {
        ZegoExpressEngineMethodHandler::getInstance().stopPublishingStream(argument, std::move(result));
    }
    else if (method_call.method_name() == "setStreamExtraInfo") {
        ZegoExpressEngineMethodHandler::getInstance().setStreamExtraInfo(argument, std::move(result));
    }
    else if (method_call.method_name() == "startPreview") {
        ZegoExpressEngineMethodHandler::getInstance().startPreview(argument, std::move(result));
    }
    else if (method_call.method_name() == "stopPreview") {
        ZegoExpressEngineMethodHandler::getInstance().stopPreview(argument, std::move(result));
    }
    else if (method_call.method_name() == "setAudioConfig") {
        ZegoExpressEngineMethodHandler::getInstance().setAudioConfig(argument, std::move(result));
    }
    else if (method_call.method_name() == "getAudioConfig") {
        ZegoExpressEngineMethodHandler::getInstance().getAudioConfig(argument, std::move(result));
    }
    else if (method_call.method_name() == "mutePublishStreamAudio") {
        ZegoExpressEngineMethodHandler::getInstance().mutePublishStreamAudio(argument, std::move(result));
    }
    else if (method_call.method_name() == "setCaptureVolume") {
        ZegoExpressEngineMethodHandler::getInstance().setCaptureVolume(argument, std::move(result));
    }
    else if (method_call.method_name() == "setAudioCaptureStereoMode") {
        ZegoExpressEngineMethodHandler::getInstance().setAudioCaptureStereoMode(argument, std::move(result));
    }
    else if (method_call.method_name() == "sendSEI") {
        ZegoExpressEngineMethodHandler::getInstance().sendSEI(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableHardwareEncoder") {
        ZegoExpressEngineMethodHandler::getInstance().enableHardwareEncoder(argument, std::move(result));
    }
    else if (method_call.method_name() == "startPlayingStream") {
        ZegoExpressEngineMethodHandler::getInstance().startPlayingStream(argument, std::move(result));
    }
    else if (method_call.method_name() == "stopPlayingStream") {
        ZegoExpressEngineMethodHandler::getInstance().stopPlayingStream(argument, std::move(result));
    }
    else if (method_call.method_name() == "setPlayVolume") {
        ZegoExpressEngineMethodHandler::getInstance().setPlayVolume(argument, std::move(result));
    }
    else if (method_call.method_name() == "setAllPlayStreamVolume") {
        ZegoExpressEngineMethodHandler::getInstance().setAllPlayStreamVolume(argument, std::move(result));
    }
    else if (method_call.method_name() == "mutePlayStreamAudio") {
        ZegoExpressEngineMethodHandler::getInstance().mutePlayStreamAudio(argument, std::move(result));
    }
    else if (method_call.method_name() == "muteAllPlayStreamAudio") {
        ZegoExpressEngineMethodHandler::getInstance().muteAllPlayStreamAudio(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableHardwareDecoder") {
        ZegoExpressEngineMethodHandler::getInstance().enableHardwareDecoder(argument, std::move(result));
    }
    else if (method_call.method_name() == "muteMicrophone") {
        ZegoExpressEngineMethodHandler::getInstance().muteMicrophone(argument, std::move(result));
    }
    else if (method_call.method_name() == "isMicrophoneMuted") {
        ZegoExpressEngineMethodHandler::getInstance().isMicrophoneMuted(argument, std::move(result));
    }
    else if (method_call.method_name() == "muteSpeaker") {
        ZegoExpressEngineMethodHandler::getInstance().muteSpeaker(argument, std::move(result));
    }
    else if (method_call.method_name() == "isSpeakerMuted") {
        ZegoExpressEngineMethodHandler::getInstance().isSpeakerMuted(argument, std::move(result));
    }
    else if (method_call.method_name() == "getAudioDeviceList") {
        ZegoExpressEngineMethodHandler::getInstance().getAudioDeviceList(argument, std::move(result));
    }
    else if (method_call.method_name() == "getDefaultAudioDeviceID") {
        ZegoExpressEngineMethodHandler::getInstance().getDefaultAudioDeviceID(argument, std::move(result));
    }
    else if (method_call.method_name() == "useAudioDevice") {
        ZegoExpressEngineMethodHandler::getInstance().useAudioDevice(argument, std::move(result));
    }
    else if (method_call.method_name() == "startSoundLevelMonitor") {
        ZegoExpressEngineMethodHandler::getInstance().startSoundLevelMonitor(argument, std::move(result));
    }
    else if (method_call.method_name() == "stopSoundLevelMonitor") {
        ZegoExpressEngineMethodHandler::getInstance().stopSoundLevelMonitor(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableHeadphoneMonitor") {
        ZegoExpressEngineMethodHandler::getInstance().enableHeadphoneMonitor(argument, std::move(result));
    }
    else if (method_call.method_name() == "setHeadphoneMonitorVolume") {
        ZegoExpressEngineMethodHandler::getInstance().setHeadphoneMonitorVolume(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableAEC") {
        ZegoExpressEngineMethodHandler::getInstance().enableAEC(argument, std::move(result));
    }
    else if (method_call.method_name() == "setAECMode") {
        ZegoExpressEngineMethodHandler::getInstance().setAECMode(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableANS") {
        ZegoExpressEngineMethodHandler::getInstance().enableANS(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableTransientANS") {
        ZegoExpressEngineMethodHandler::getInstance().enableTransientANS(argument, std::move(result));
    }
    else if (method_call.method_name() == "setANSMode") {
        ZegoExpressEngineMethodHandler::getInstance().setANSMode(argument, std::move(result));
    }
    else if (method_call.method_name() == "setAudioEqualizerGain") {
        ZegoExpressEngineMethodHandler::getInstance().setAudioEqualizerGain(argument, std::move(result));
    }
    else if (method_call.method_name() == "setVoiceChangerPreset") {
        ZegoExpressEngineMethodHandler::getInstance().setVoiceChangerPreset(argument, std::move(result));
    }
    else if (method_call.method_name() == "setVoiceChangerParam") {
        ZegoExpressEngineMethodHandler::getInstance().setVoiceChangerParam(argument, std::move(result));
    }
    else if (method_call.method_name() == "setReverbPreset") {
        ZegoExpressEngineMethodHandler::getInstance().setReverbPreset(argument, std::move(result));
    }
    else if (method_call.method_name() == "setReverbAdvancedParam") {
        ZegoExpressEngineMethodHandler::getInstance().setReverbAdvancedParam(argument, std::move(result));
    }
    else if (method_call.method_name() == "setReverbEchoParam") {
        ZegoExpressEngineMethodHandler::getInstance().setReverbEchoParam(argument, std::move(result));
    }
    else if (method_call.method_name() == "enableVirtualStereo") {
        ZegoExpressEngineMethodHandler::getInstance().enableVirtualStereo(argument, std::move(result));
    }
    else {
      result->NotImplemented();
    }
}

//}  // namespace

void ZegoExpressEnginePluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  ZegoExpressEnginePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
