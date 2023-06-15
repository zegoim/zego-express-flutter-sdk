package im.zego.zego_express_engine;

import java.nio.ByteBuffer;

public interface IZegoFlutterMediaPlayerBlockDataHandler {
    /**
     * The callback triggered when the media player is about to throw the block data of the media resource.
     *
     * Available since: 3.4.0
     * Description: The callback triggered when the media player is about to throw the block data of the media resource.
     * Trigger: The callback is generated when the media player starts playing.
     * Caution: The callback does not actually take effect until call [setBlockDataHandler] to set.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.
     * @param path The path of the media resource.
     */
    public void onBlockBegin(int mediaPlayerIndex, String path);

    /**
     * The callback triggered when the media player throws the block data of the media resource.
     *
     * Available since: 3.4.0
     * Description: The callback triggered when the media player throws the block data of the media resource.
     * Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
     * Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.ect.
     * @param buffer The block data of the media resource.
     * @return The size of the buffer, -1 is returned for failure.
     */
    public int onBlockData(int mediaPlayerIndex, ByteBuffer buffer);
}
