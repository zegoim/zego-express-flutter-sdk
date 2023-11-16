#include "include/zego_express_engine/ZegoCustomVideoCaptureManager.h"
#include "ZegoLog.h"
#include <ZegoExpressSDK.h>

static std::shared_ptr<ZegoCustomVideoCaptureManager> instance_ = nullptr;
static std::once_flag singletonFlag;

class ZegoCustomVideoCaptureHandler : public ZEGO::EXPRESS::IZegoCustomVideoCaptureHandler {
public:
    void setCustomVideoCaptureHandler(std::shared_ptr<IZegoFlutterCustomVideoCaptureHandler> handler) {
        handler_ = handler;
    }
private:
    std::shared_ptr<IZegoFlutterCustomVideoCaptureHandler> handler_ = nullptr;
    void onStart(ZEGO::EXPRESS::ZegoPublishChannel channel) override {
        ZF::logInfo("[CustomVideoCapture][onStart] channel: %d", channel);

        if (handler_) {
            handler_->onStart((ZGFlutterPublishChannel)channel);
        }
    }

    void onStop(ZEGO::EXPRESS::ZegoPublishChannel channel) override {
        ZF::logInfo("[CustomVideoCapture][onStop] channel: %d", channel);

        if (handler_) {
            handler_->onStop((ZGFlutterPublishChannel)channel);
        }
    }

    void onEncodedDataTrafficControl(ZEGO::EXPRESS::ZegoTrafficControlInfo trafficControlInfo,
                                             ZEGO::EXPRESS::ZegoPublishChannel channel) override {
        ZF::logInfo("[CustomVideoCapture][onEncodedDataTrafficControl] channel:%d", channel);

        if (handler_) {
            ZGFlutterTrafficControlInfo info;
            info.bitrate = trafficControlInfo.bitrate;
            info.fps = trafficControlInfo.fps;
            info.height = trafficControlInfo.height;
            info.width = trafficControlInfo.width;
            handler_->onEncodedDataTrafficControl(info, (ZGFlutterPublishChannel)channel);
        }
    }
};

std::shared_ptr<ZegoCustomVideoCaptureManager> ZegoCustomVideoCaptureManager::getInstance() {
    std::call_once(singletonFlag, [&] {
        instance_ = std::make_shared<ZegoCustomVideoCaptureManager>();
    });
    return instance_;
}

std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoCaptureHandler> ZegoCustomVideoCaptureManager::getHandler() {
    return zegoHandler_;
}

void ZegoCustomVideoCaptureManager::setCustomVideoCaptureHandler(std::shared_ptr<IZegoFlutterCustomVideoCaptureHandler> handler) {
    handler_ = handler;
    if (handler_ && !zegoHandler_) {
        zegoHandler_ = std::make_shared<ZegoCustomVideoCaptureHandler>();
    }
    dynamic_cast<ZegoCustomVideoCaptureHandler*>(zegoHandler_.get())->setCustomVideoCaptureHandler(handler_);
}

void ZegoCustomVideoCaptureManager::sendCustomVideoCaptureRawData(const unsigned char *data, unsigned int dataLength,
                                ZGFlutterVideoFrameParam params,
                                unsigned long long referenceTimeMillisecond,
                                ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoVideoFrameParam zegoParams;
    zegoParams.format = (ZEGO::EXPRESS::ZegoVideoFrameFormat)params.format;
    zegoParams.height = params.height;
    zegoParams.rotation = params.rotation;
    zegoParams.width = params.width;
    for (uint32_t i = 0; i < 4; i++) {
        zegoParams.strides[i] = params.strides[i];
    }
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->sendCustomVideoCaptureRawData(data, dataLength, zegoParams, referenceTimeMillisecond, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

void ZegoCustomVideoCaptureManager::sendCustomVideoCaptureEncodedData(const unsigned char *data, unsigned int dataLength,
                                      ZGFlutterVideoEncodedFrameParam params,
                                      unsigned long long referenceTimeMillisecond,
                                      ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoVideoEncodedFrameParam zegoParams;
    zegoParams.format = (ZEGO::EXPRESS::ZegoVideoEncodedFrameFormat)params.format;
    zegoParams.height = params.height;
    zegoParams.isKeyFrame = params.isKeyFrame;
    zegoParams.rotation = params.rotation;
    zegoParams.SEIData = params.SEIData;
    zegoParams.SEIDataLength = params.SEIDataLength;
    zegoParams.width = params.width;
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->sendCustomVideoCaptureEncodedData(data, dataLength, zegoParams, referenceTimeMillisecond, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

void ZegoCustomVideoCaptureManager::setCustomVideoCaptureFillMode(ZGFlutterViewMode mode,
                                  ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureFillMode((ZEGO::EXPRESS::ZegoViewMode)mode, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

void ZegoCustomVideoCaptureManager::setCustomVideoCaptureDeviceState(bool isEnable, ZGFlutterRemoteDeviceState state,
                                     ZGFlutterPublishChannel channel) {
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureDeviceState(isEnable, (ZEGO::EXPRESS::ZegoRemoteDeviceState)state,
        (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

void ZegoCustomVideoCaptureManager::setCustomVideoCaptureRegionOfInterest(std::vector<ZGFlutterRoiRect> rectList,
        ZGFlutterPublishChannel channel) {
    std::vector<ZEGO::EXPRESS::ZegoRoiRect> zegoRectList;
    for (ZGFlutterRoiRect rect: rectList) {
        ZEGO::EXPRESS::ZegoRoiRect zegoRect;
        zegoRect.height = rect.height;
        zegoRect.width = rect.width;
        zegoRect.x = rect.x;
        zegoRect.y = rect.y;
        zegoRect.strength = rect.strength;
        zegoRectList.push_back(zegoRect);
    }
    ZEGO::EXPRESS::ZegoExpressSDK::getEngine()->setCustomVideoCaptureRegionOfInterest(zegoRectList, (ZEGO::EXPRESS::ZegoPublishChannel)channel);
}

ZEGO::EXPRESS::IZegoExpressEngine* ZegoCustomVideoCaptureManager::GetEngine() {
    return ZEGO::EXPRESS::ZegoExpressSDK::getEngine();
}