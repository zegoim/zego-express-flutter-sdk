package im.zego.zego_express_engine;

import org.json.JSONObject;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoMediaPlayerAudioHandler;
import im.zego.zegoexpress.entity.ZegoAudioFrameParam;

public class ZegoMediaPlayerAudioManager implements IZegoMediaPlayerAudioHandler {

    private volatile static ZegoMediaPlayerAudioManager singleton;

    private static IZegoFlutterMediaPlayerAudioHandler mHander;

    public ZegoMediaPlayerAudioManager() {

    }

    /**
     * Get the custom audio capture manager instance
     */
    public static synchronized ZegoMediaPlayerAudioManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoMediaPlayerAudioManager();
        }
        return singleton;
    }

    /**
     * Set audio data callback handler of the media player.
     *
     * Available since: 2.1.0
     * Description: By setting this callback, the audio data of the media resource file played by the media player can be called back.
     * When to call: After the [ZegoMediaPlayer] instance created.
     * Restrictions: None.
     * Caution: When you no longer need to get the audio frame data, please call this function again to clear the handler to stop the audio frame data callback.
     *
     * @param handler audio data callback handler for media player
     */
    public void setAudioHandler(IZegoFlutterMediaPlayerAudioHandler handler) {
        mHander = handler;
    }

    @Override
    public void onAudioFrame(ZegoMediaPlayer mediaPlayer, ByteBuffer data, int dataLength, ZegoAudioFrameParam param) {
        if (mHander != null) {
            ZGFlutterAudioFrameParam audioFrameParam = new ZGFlutterAudioFrameParam();
            audioFrameParam.channel = ZGFlutterAudioChannel.getZGFlutterAudioChannel(param.channel.value());
            audioFrameParam.sampleRate = ZGFlutterAudioSampleRate.getZGFlutterAudioSampleRate(param.sampleRate.value());
            mHander.onAudioFrame(mediaPlayer.getIndex(), data, dataLength, audioFrameParam);
        }
    }
}
