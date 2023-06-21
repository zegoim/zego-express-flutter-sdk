package im.zego.zego_express_engine;

import org.json.JSONObject;

import java.nio.*;

public interface IZegoFlutterMediaPlayerVideoHandler {
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
    public void onVideoFrame(int mediaPlayerIndex, ByteBuffer[] data, int[] dataLength,
                      ZGFlutterVideoFrameParam param, JSONObject extraInfo);
}
