#include "include/zego_express_engine/ZegoMediaPlayerVideoManager.h"

static std::shared_ptr<ZegoMediaPlayerVideoManager> instance_ = nullptr;
static std::once_flag singletonFlag;

void ZegoMediaPlayerVideoManager::onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                            const unsigned char **data,
                            unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param) {
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

void ZegoMediaPlayerVideoManager::onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                const unsigned char **data,
                            unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param,
                            const char * extraInfo) {
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

void ZegoMediaPlayerVideoManager::onBlockBegin(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer,
                    const std::string &path) {
    if (handler_) {
        handler_->onBlockBegin(mediaPlayer->getIndex(), path);
    }
}

unsigned int ZegoMediaPlayerVideoManager::onBlockData(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer,
                            unsigned char *const buffer,
                                    unsigned int bufferSize) {
    if (handler_) {
        return handler_->onBlockData(mediaPlayer->getIndex(), buffer, bufferSize);
    }
    return -1;                                 
}

std::shared_ptr<ZegoMediaPlayerVideoManager> ZegoMediaPlayerVideoManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoMediaPlayerVideoManager>();
    });
    return instance_;
}

void ZegoMediaPlayerVideoManager::setVideoHandler(std::shared_ptr<IZegoFlutterMediaPlayerVideoHandler> handler) {
    handler_ = handler;
}