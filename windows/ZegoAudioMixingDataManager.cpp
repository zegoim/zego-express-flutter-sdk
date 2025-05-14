#include "include/zego_express_engine/ZegoAudioMixingDataManager.h"
#include "internal/ZegoLog.h"
#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoAudioMixingDataManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoAudioMixingHandler : public ZEGO::EXPRESS::IZegoAudioMixingHandler {
public:
    void setHandler(std::shared_ptr<IZegoFlutterAudioMixingHandler> handler) {
        handler_ = handler;
    }

private:
    std::shared_ptr<IZegoFlutterAudioMixingHandler> handler_ = nullptr;

    void onAudioMixingCopyData(ZEGO::EXPRESS::ZegoAudioMixingData *data) override {
        if (handler_ == nullptr) {
            return;
        }

        ZGFlutterAudioMixingData result;
        result.audioData = data->audioData;
        result.audioDataLength = data->audioDataLength;
        result.SEIData = data->SEIData;
        result.SEIDataLength = data->SEIDataLength;
        result.param.sampleRate = (ZGFlutterAudioSampleRate)data->param.sampleRate;
        result.param.channel = (ZGFlutterAudioChannel)data->param.channel;

        handler_->onAudioMixingCopyData(&result);

        data->audioDataLength = result.audioDataLength;
        data->param.sampleRate = ZEGO::EXPRESS::ZegoAudioSampleRate(result.param.sampleRate);
        data->param.channel = ZEGO::EXPRESS::ZegoAudioChannel(result.param.channel);
        data->SEIDataLength = result.SEIDataLength;
    }
};

std::shared_ptr<ZegoAudioMixingDataManager> ZegoAudioMixingDataManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoAudioMixingDataManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoAudioMixingHandler> ZegoAudioMixingDataManager::getHandler() {
    return zegoHandler_;
}

void ZegoAudioMixingDataManager::setAudioMixingHandler(std::shared_ptr<IZegoFlutterAudioMixingHandler> handler) {
    ZF::logInfo("[AudioMixingDataManager][setAudioMixingHandler] handler: %p", handler.get());
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoAudioMixingHandler>();
    }
    dynamic_cast<ZegoAudioMixingHandler*>(zegoHandler_.get())->setHandler(handler_);
}