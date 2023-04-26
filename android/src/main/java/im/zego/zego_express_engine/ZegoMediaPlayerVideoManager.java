package im.zego.zego_express_engine;

import org.json.JSONObject;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoMediaPlayerVideoHandler;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public class ZegoMediaPlayerVideoManager extends IZegoMediaPlayerVideoHandler {

    private volatile static ZegoMediaPlayerVideoManager singleton;

    private ZegoVideoFrameParam mParam;
    private ZegoVideoMirrorMode mMirrorMode;

    private static IZegoFlutterMediaPlayerVideoHandler mHander;

    public ZegoMediaPlayerVideoManager() {

    }

    /**
     * Get the custom video capture manager instance
     */
    public static synchronized ZegoMediaPlayerVideoManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoMediaPlayerVideoManager();
        }
        return singleton;
    }

    /**
     * Set video data callback handler of the media player.
     *
     * Available since: 2.1.0
     * Description: By setting this callback, the video data of the media resource file played by the media player can be called back.
     * When to call: After the [ZegoMediaPlayer] instance created.
     * Restrictions: None.
     * Caution: When you no longer need to get the video frame data, please call this function again to clear the handler to stop the video frame data callback.
     *
     * @param handler Video data callback handler for media player
     */
    public void setVideoHandler(IZegoFlutterMediaPlayerVideoHandler handler) {
        mHander = handler;
    }

    @Override
    public void onVideoFrame(ZegoMediaPlayer mediaPlayer, ByteBuffer[] data, int[] dataLength,
                             ZegoVideoFrameParam param, JSONObject extraInfo) {
        if (mHander != null) {
            ZGFlutterVideoFrameParam videoFrameParam = new ZGFlutterVideoFrameParam();
            videoFrameParam.width = param.width;
            videoFrameParam.height = param.height;
            videoFrameParam.rotation = param.rotation;
            videoFrameParam.format = ZGFlutterVideoFrameFormat.getVideoFrameFormat(param.format.value());
            for (int i = 0; i < 4; i++) {
                videoFrameParam.strides[i] = param.strides[i];
            }
            mHander.onVideoFrame(mediaPlayer.getIndex(), data, dataLength, videoFrameParam, extraInfo);
        }
    }
}
