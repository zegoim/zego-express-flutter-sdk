#ifndef ZEGO_MEDIA_PLAYER_AUDIO_MANAGER_H_
#define ZEGO_MEDIA_PLAYER_AUDIO_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterMediaPlayerAudioHandler {
protected:
    virtual ~IZegoFlutterMediaPlayerAudioHandler() {}

public:
    /// The callback triggered when the media player throws out audio frame data.
    ///
    /// Available since: 1.3.4
    /// Description: The callback triggered when the media player throws out audio frame data.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setAudioHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayerIndex Callback player index.
    /// @param data Raw data of audio frames.
    /// @param dataLength Data length.
    /// @param param audio frame flip mode.
    virtual void onAudioFrame(int /*mediaPlayerIndex*/, const unsigned char * /*data*/,
                              unsigned int /*dataLength*/, ZGFlutterAudioFrameParam /*param*/) {}
};

namespace ZEGO::EXPRESS {
    class IZegoMediaPlayerAudioHandler;
};
class ZegoMediaPlayerAudioHandler;

class FLUTTER_PLUGIN_EXPORT ZegoMediaPlayerAudioManager{
public:
    static std::shared_ptr<ZegoMediaPlayerAudioManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerAudioHandler> getHandler();
    
    /// Set audio data callback handler of the media player.
    ///
    /// Available since: 2.1.0
    /// Description: By setting this callback, the audio data of the media resource file played by the media player can be called back.
    /// Restrictions: None.
    /// Caution: When you no longer need to get the audio frame data, please call this function again to clear the handler to stop the audio frame data callback.
    ///
    /// @param handler audio data callback handler
    void setAudioHandler(std::shared_ptr<IZegoFlutterMediaPlayerAudioHandler> handler);

private:
    friend class ZegoMediaPlayerAudioHandler;
    std::shared_ptr<IZegoFlutterMediaPlayerAudioHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerAudioHandler> zegoHandler_ = nullptr;
};

#endif  // ZEGO_MEDIA_PLAYER_AUDIO_MANAGER_H_
