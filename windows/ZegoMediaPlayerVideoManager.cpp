#include "include/zego_express_engine/ZegoMediaPlayerVideoManager.h"

#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoMediaPlayerVideoManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoMediaPlayerVideoHandler : public ZEGO::EXPRESS::IZegoMediaPlayerVideoHandler {
public:
    void setVideoHandler(std::shared_ptr<IZegoFlutterMediaPlayerVideoHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterMediaPlayerVideoHandler> handler_ = nullptr;

    void onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char ** data,
                              unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param) override {
        if (handler_) {
            ZGFlutterVideoFrameParam frameParam;
            frameParam.format = (ZGFlutterVideoFrameFormat)param.format;
            frameParam.height = param.height;
            frameParam.width = param.width;
            frameParam.rotation = param.rotation;
            for (size_t i = 0; i < 4; i++)
            {
                frameParam.strides[i] = param.strides[i];
            }
             
            handler_->onVideoFrame(mediaPlayer->getIndex(), data, dataLength, frameParam);
        }
    }

    void onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char ** data,
                              unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param,
                              const char * extraInfo) override {
        if (handler_) {
            ZGFlutterVideoFrameParam frameParam;
            frameParam.format = (ZGFlutterVideoFrameFormat)param.format;
            frameParam.height = param.height;
            frameParam.width = param.width;
            frameParam.rotation = param.rotation;
            for (size_t i = 0; i < 4; i++)
            {
                frameParam.strides[i] = param.strides[i];
            }
             
            handler_->onVideoFrame(mediaPlayer->getIndex(), data, dataLength, frameParam, extraInfo);
        }
    }
};

std::shared_ptr<ZegoMediaPlayerVideoManager> ZegoMediaPlayerVideoManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoMediaPlayerVideoManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerVideoHandler> ZegoMediaPlayerVideoManager::getHandler() {
    return zegoHandler_;
}

void ZegoMediaPlayerVideoManager::setVideoHandler(std::shared_ptr<IZegoFlutterMediaPlayerVideoHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoMediaPlayerVideoHandler>();
    }
    dynamic_cast<ZegoMediaPlayerVideoHandler*>(zegoHandler_.get())->setVideoHandler(handler_);
}