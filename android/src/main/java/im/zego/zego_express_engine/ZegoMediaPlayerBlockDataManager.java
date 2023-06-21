package im.zego.zego_express_engine;

import java.nio.ByteBuffer;

import im.zego.zego_express_engine.internal.ZegoLog;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoMediaPlayerBlockDataHandler;

public class ZegoMediaPlayerBlockDataManager implements IZegoMediaPlayerBlockDataHandler {

    private volatile static ZegoMediaPlayerBlockDataManager singleton;

    private static IZegoFlutterMediaPlayerBlockDataHandler mHander;

    /**
     * Get the custom video capture manager instance
     */
    public static synchronized ZegoMediaPlayerBlockDataManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoMediaPlayerBlockDataManager();
        }
        return singleton;
    }

    /**
     * Set the media resource block data callback handler of the media player.
     *
     * Available since: 3.4.0
     * Description: This callback can be set to listen to and decrypt the media resource block data for playing the user's own ciphertext media resources.
     * When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
     * Restrictions: None.
     * Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
     *
     * @param handler The media resource block data callback handler of the media player.
     */
    public void setBlockDataHandler(IZegoFlutterMediaPlayerBlockDataHandler handler) {
        mHander = handler;
    }

    @Override
    public void onBlockBegin(ZegoMediaPlayer mediaPlayer, String path) {
        ZegoLog.log("[MediaPlayer] [onBlockBegin] path:%s", path);
        if (mHander != null) {
            mHander.onBlockBegin(mediaPlayer.getIndex(), path);
        }
    }

    @Override
    public int onBlockData(ZegoMediaPlayer mediaPlayer, ByteBuffer buffer) {
        if (mHander != null) {
            return mHander.onBlockData(mediaPlayer.getIndex(), buffer);
        }
        return -1;
    }
}
