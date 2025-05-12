#include "include/zego_express_engine/ZegoAudioMixingDataManager.h"
#include "ZegoLog.h"
#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoAudioMixingDataManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoAudioMixingHandler : public ZEGO::EXPRESS::IZegoAudioMixingHandler {
public:
    void setHandler(std::shared_ptr<IZegoFlutterAudioMixingHandler> handler) {
        handler_ = handler;
    }

    std::shared_ptr<ZEGO::EXPRESS::IZegoAudioMixingHandler> getHandler() {
        return zegoHandler_;
    }

private:
    std::shared_ptr<IZegoFlutterAudioMixingHandler> handler_ = nullptr;

    ZegoAudioMixingData onAudioMixingCopyData(unsigned int expectedDataLength) override {
        if (handler_ == nullptr) {
            return nullptr;
        }

        auto data = handler_->onAudioMixingCopyData(expectedDataLength);

        if (data == nullptr) {
            return nullptr;
        }

        ZegoAudioMixingData result;
        result.audioData = data.audioData;
        result.audioDataLength = data.audioDataLength;
        result.SEIData = data.SEIData;
        ZegoAudioFrameParam param;
        param.sampleRate = data.param.sampleRate;
        param.channel = data.param.channel;
        result.param = param;
        result.SEIDataLength = data.SEIDataLength;
        return result;
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

void ZegoAudioMixingDataManager::setHandler(std::shared_ptr<IZegoFlutterAudioMixingHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoAudioMixingHandler>();
    }
    dynamic_cast<ZegoAudioMixingHandler*>(zegoHandler_.get())->setHandler(handler_);
}