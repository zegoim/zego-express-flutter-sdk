package im.zego.zego_express_engine;

import org.json.JSONObject;

import java.nio.*;

import im.zego.zegoexpress.ZegoMediaPlayer;

public abstract class IZegoFlutterMediaPlayerVideoHandler {
    /**
     * The callback triggered when the media player throws out video frame data.
     *
     * Available since: 1.3.4
     * Description: The callback triggered when the media player throws out video frame data.
     * Trigger: The callback is generated when the media player starts playing.
     * Caution: The callback does not actually take effect until call [setVideoHandler] to set.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.
     * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
     * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
     * @param param Video frame flip mode.
     */
     public abstract void onVideoFrame(int mediaPlayerIndex, ByteBuffer[] data, int[] dataLength,
                             ZGFlutterVideoFrameParam param);

    /**
     * The callback triggered when the media player throws out video frame data.
     *
     * Available since: 2.16.0
     * Description: The callback triggered when the media player throws out video frame data.
     * Trigger: The callback is generated when the media player starts playing.
     * Caution: The callback does not actually take effect until call [setVideoHandler] to set.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.
     * @param data Raw data of video frames (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
     * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
     * @param param Video frame flip mode.
     * @param extraInfo Video frame extra info.
     */
    public abstract void onVideoFrame(int mediaPlayerIndex, ByteBuffer[] data, int[] dataLength,
                      ZGFlutterVideoFrameParam param, JSONObject extraInfo);

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
    void onBlockBegin(int mediaPlayerIndex, String path) {}

    /**
     * The callback triggered when the media player throws the block data of the media resource.
     *
     * Available since: 3.4.0
     * Description: The callback triggered when the media player throws the block data of the media resource.
     * Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
     * Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.
     * @param buffer The block data of the media resource.
     * @return The size of the buffer, -1 is returned for failure.
     */
    int onBlockData(int mediaPlayerIndex, ByteBuffer buffer) {
        return -1;
    }
}
