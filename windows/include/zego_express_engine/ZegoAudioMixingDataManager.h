#ifndef ZEGO_AUDIO_MIXING_DATA_MANAGER_H_
#define ZEGO_AUDIO_MIXING_DATA_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterAudioMixingHandler {
protected:
    virtual ~IZegoFlutterAudioMixingHandler() {}
public:
    virtual void onAudioMixingCopyData(ZGFlutterAudioMixingData *data) = 0;
};

namespace ZEGO::EXPRESS {
    class IZegoAudioMixingHandler;
};
class ZegoAudioMixingHandler;

class FLUTTER_PLUGIN_EXPORT ZegoAudioMixingDataManager {
public:
    static std::shared_ptr<ZegoAudioMixingDataManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoAudioMixingHandler> getHandler();

    void setAudioMixingHandler(std::shared_ptr<IZegoFlutterAudioMixingHandler> handler);

private:
    friend class ZegoAudioMixingHandler;
    std::shared_ptr<IZegoFlutterAudioMixingHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoAudioMixingHandler> zegoHandler_ = nullptr;
};

#endif
