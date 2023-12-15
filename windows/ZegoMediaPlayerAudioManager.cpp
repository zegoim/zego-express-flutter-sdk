#include "include/zego_express_engine/ZegoMediaPlayerAudioManager.h"

#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoMediaPlayerAudioManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoMediaPlayerAudioHandler : public ZEGO::EXPRESS::IZegoMediaPlayerAudioHandler {
public:
    void setAudioHandler(std::shared_ptr<IZegoFlutterMediaPlayerAudioHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterMediaPlayerAudioHandler> handler_ = nullptr;

    void onAudioFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char * data,
                              unsigned int dataLength, ZEGO::EXPRESS::ZegoAudioFrameParam param) override {
        if (handler_) {
            ZGFlutterAudioFrameParam frameParam;
            frameParam.sampleRate = (ZGFlutterAudioSampleRate)param.sampleRate;
            frameParam.channel = (ZGFlutterAudioChannel)param.channel;
            handler_->onAudioFrame(mediaPlayer->getIndex(), data, dataLength, frameParam);
        }
    }
};

std::shared_ptr<ZegoMediaPlayerAudioManager> ZegoMediaPlayerAudioManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoMediaPlayerAudioManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerAudioHandler> ZegoMediaPlayerAudioManager::getHandler() {
    return zegoHandler_;
}

void ZegoMediaPlayerAudioManager::setAudioHandler(std::shared_ptr<IZegoFlutterMediaPlayerAudioHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoMediaPlayerAudioHandler>();
    }
    dynamic_cast<ZegoMediaPlayerAudioHandler*>(zegoHandler_.get())->setAudioHandler(handler_);
}