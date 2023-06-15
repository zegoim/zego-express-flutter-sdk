#include "include/zego_express_engine/ZegoMediaPlayerBlockDataManager.h"
#include "ZegoLog.h"
#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoMediaPlayerBlockDataManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoMediaPlayerBlockDataHandler : public ZEGO::EXPRESS::IZegoMediaPlayerBlockDataHandler {
public:
    void setBlockDataHandler(std::shared_ptr<IZegoFlutterMediaPlayerBlockDataHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterMediaPlayerBlockDataHandler> handler_ = nullptr;

    void onBlockBegin(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer, const std::string &path) override {
        ZF::logInfo("[MediaPlayer][onBlockBegin] path: %s", path.c_str());

        if (handler_) {
            handler_->onBlockBegin(mediaPlayer->getIndex(), path);
        } 
    }

    unsigned int onBlockData(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer, unsigned char *const buffer,
                                     unsigned int bufferSize) override {
        if (handler_) {
            return handler_->onBlockData(mediaPlayer->getIndex(), buffer, bufferSize);
        }
        return -1;
    }
};

std::shared_ptr<ZegoMediaPlayerBlockDataManager> ZegoMediaPlayerBlockDataManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoMediaPlayerBlockDataManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerBlockDataHandler> ZegoMediaPlayerBlockDataManager::getHandler() {
    return zegoHandler_;
}

void ZegoMediaPlayerBlockDataManager::setBlockDataHandler(std::shared_ptr<IZegoFlutterMediaPlayerBlockDataHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoMediaPlayerBlockDataHandler>();
    }
    dynamic_cast<ZegoMediaPlayerBlockDataHandler*>(zegoHandler_.get())->setBlockDataHandler(handler_);
}