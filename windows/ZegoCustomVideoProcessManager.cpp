#include "include/zego_express_engine/ZegoCustomVideoProcessManager.h"

#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoCustomVideoProcessManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoCustomVideoProcessHandler : public ZEGO::EXPRESS::IZegoCustomVideoProcessHandler {
public:
    void setCustomVideoProcessHandler(std::shared_ptr<IZegoFlutterCustomVideoProcessHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterCustomVideoProcessHandler> handler_ = nullptr;

    void onCapturedUnprocessedRawData(const unsigned char ** data,
                                              unsigned int * dataLength,
                                              ZEGO::EXPRESS::ZegoVideoFrameParam param,
                                              unsigned long long referenceTimeMillisecond,
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
            
            handler_->onCapturedUnprocessedRawData(data, dataLength, frameParam, referenceTimeMillisecond, (ZGFlutterPublishChannel)channel);
        }
    }
};

std::shared_ptr<ZegoCustomVideoProcessManager> ZegoCustomVideoProcessManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoCustomVideoProcessManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoProcessHandler> ZegoCustomVideoProcessManager::getHandler() {
    return zegoHandler_;
}

void ZegoCustomVideoProcessManager::setCustomVideoProcessHandler(std::shared_ptr<IZegoFlutterCustomVideoProcessHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoCustomVideoProcessHandler>();
    }
    dynamic_cast<ZegoCustomVideoProcessHandler*>(zegoHandler_.get())->setCustomVideoProcessHandler(handler_);
}

void ZegoCustomVideoProcessManager::sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength,
                                    ZGFlutterVideoFrameParam params,
                                    unsigned long long referenceTimeMillisecond,
                                    ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoVideoFrameParam zegoParam;
    zegoParam.format = (ZEGO::EXPRESS::ZegoVideoFrameFormat)params.format;
    zegoParam.height = params.height;
    zegoParam.width = params.width;
    zegoParam.rotation = params.rotation;
    for (size_t i = 0; i < 4; i++)
    {
        zegoParam.strides[i] = params.strides[i];
    }
    
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->sendCustomVideoProcessedRawData(data, dataLength, zegoParam, referenceTimeMillisecond, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

void ZegoCustomVideoProcessManager::sendCustomVideoProcessedRawData(
        const unsigned char **data, unsigned int *dataLength, ZGFlutterVideoFrameParam params,
        unsigned long long referenceTimeMillisecond, ZGFlutterProcessedDataUsageType usage,
        ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoVideoFrameParam zegoParam;
    zegoParam.format = (ZEGO::EXPRESS::ZegoVideoFrameFormat)params.format;
    zegoParam.height = params.height;
    zegoParam.width = params.width;
    zegoParam.rotation = params.rotation;
    for (size_t i = 0; i < 4; i++)
    {
        zegoParam.strides[i] = params.strides[i];
    }
    
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->sendCustomVideoProcessedRawData(data, dataLength, zegoParam, referenceTimeMillisecond,
        (ZEGO::EXPRESS::ZegoProcessedDataUsageType)usage, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}