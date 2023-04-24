#include "include/zego_express_engine/ZegoCustomVideoRenderManager.h"

#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoCustomVideoRenderManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoCustomVideoRenderHandler : public ZEGO::EXPRESS::IZegoCustomVideoRenderHandler {
public:
    void setCustomVideoRenderHandler(std::shared_ptr<IZegoFlutterCustomVideoRenderHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterCustomVideoRenderHandler> handler_ = nullptr;

    void onCapturedVideoFrameRawData(unsigned char ** data,
                                             unsigned int * dataLength,
                                             ZEGO::EXPRESS::ZegoVideoFrameParam param,
                                             ZEGO::EXPRESS::ZegoVideoFlipMode flipMode,
                                             ZEGO::EXPRESS::ZegoPublishChannel channel) override {
        if (handler_) {
            ZGFlutterVideoFrameParam frameParam;
            frameParam.width = param.width;
            frameParam.height = param.height;
            frameParam.rotation = param.rotation;
            frameParam.format = (ZGFlutterVideoFrameFormat)param.format;
            for (size_t i = 0; i < 4; i++)
            {
                frameParam.strides[i] = param.strides[i];
            }
            handler_->onCapturedVideoFrameRawData(data, dataLength, frameParam, (ZGFlutterVideoFlipMode)flipMode, (ZGFlutterPublishChannel)channel);
        }
    }

    void onRemoteVideoFrameRawData(unsigned char ** data, unsigned int * dataLength,
                                           ZEGO::EXPRESS::ZegoVideoFrameParam param,
                                           const std::string & streamID) override {
        if (handler_) {
            ZGFlutterVideoFrameParam frameParam;
            frameParam.width = param.width;
            frameParam.height = param.height;
            frameParam.rotation = param.rotation;
            frameParam.format = (ZGFlutterVideoFrameFormat)param.format;
            for (size_t i = 0; i < 4; i++)
            {
                frameParam.strides[i] = param.strides[i];
            }
            handler_->onRemoteVideoFrameRawData(data, dataLength, frameParam, streamID);
        }
    }

    void onRemoteVideoFrameEncodedData(const unsigned char * data,
                                               unsigned int dataLength,
                                               ZEGO::EXPRESS::ZegoVideoEncodedFrameParam param,
                                               unsigned long long referenceTimeMillisecond,
                                               const std::string & streamID)override {
        if (handler_) {
            ZGFlutterVideoEncodedFrameParam frameParam;
            frameParam.width = param.width;
            frameParam.height = param.height;
            frameParam.rotation = param.rotation;
            frameParam.format = (ZGFlutterVideoEncodedFrameFormat)param.format;
            frameParam.isKeyFrame = param.isKeyFrame;
            frameParam.SEIData = param.SEIData;
            frameParam.SEIDataLength = param.SEIDataLength;
            handler_->onRemoteVideoFrameEncodedData(data, dataLength, frameParam, referenceTimeMillisecond, streamID);
        }
    }
};

std::shared_ptr<ZegoCustomVideoRenderManager> ZegoCustomVideoRenderManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoCustomVideoRenderManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoRenderHandler> ZegoCustomVideoRenderManager::getHandler() {
    return zegoHandler_;
}

void ZegoCustomVideoRenderManager::setCustomVideoRenderHandler(std::shared_ptr<IZegoFlutterCustomVideoRenderHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoCustomVideoRenderHandler>();
    }
    dynamic_cast<ZegoCustomVideoRenderHandler*>(zegoHandler_.get())->setCustomVideoRenderHandler(handler_);
}