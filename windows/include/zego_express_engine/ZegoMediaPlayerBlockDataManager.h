
#ifndef ZEGO_MEDIA_PLAYER_BLOCK_DATA_MANAGER_H_
#define ZEGO_MEDIA_PLAYER_BLOCK_DATA_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterMediaPlayerBlockDataHandler {
protected:
    virtual ~IZegoFlutterMediaPlayerBlockDataHandler() {}

public:
    /// The callback triggered when the media player is about to throw the block data of the media resource.
    ///
    /// Available since: 3.4.0
    /// Description: The callback triggered when the media player is about to throw the block data of the media resource.
    /// Trigger: The callback is generated when the media player starts playing.
    /// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayerIndex Callback player index.
    /// @param path The path of the media resource.
    virtual void onBlockBegin(int mediaPlayerIndex, const std::string &path) = 0;

    /// The callback triggered when the media player throws the block data of the media resource.
    ///
    /// Available since: 3.4.0
    /// Description: The callback triggered when the media player throws the block data of the media resource.
    /// Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
    /// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
    /// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
    ///
    /// @param mediaPlayerIndex Callback player index.
    /// @param buffer The block data of the media resource.
    /// @param bufferSize Length of media resource block data.
    /// @return The size of the buffer, -1 is returned for failure.
    virtual unsigned int onBlockData(int mediaPlayerIndex, unsigned char *const buffer,
                                     unsigned int bufferSize) = 0;
};

namespace ZEGO::EXPRESS {
    class IZegoMediaPlayerBlockDataHandler;
};
class ZegoMediaPlayerBlockDataHandler;

class FLUTTER_PLUGIN_EXPORT ZegoMediaPlayerBlockDataManager {
public:
    static std::shared_ptr<ZegoMediaPlayerBlockDataManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerBlockDataHandler> getHandler();
    
    /// Set the media resource block data callback handler of the media player.
    ///
    /// Available since: 3.4.0
    /// Description: This callback can be set to listen to and decrypt the media resource block data for playing the user's own ciphertext media resources.
    /// When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
    /// Restrictions: None.
    /// Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
    ///
    /// @param handler The media resource block data callback handler of the media player.
    void setBlockDataHandler(std::shared_ptr<IZegoFlutterMediaPlayerBlockDataHandler> handler);

private:
    friend class ZegoMediaPlayerBlockDataHandler;
    std::shared_ptr<IZegoFlutterMediaPlayerBlockDataHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerBlockDataHandler> zegoHandler_ = nullptr;
};

#endif  // ZEGO_MEDIA_PLAYER_BLOCK_DATA_MANAGER_H_
