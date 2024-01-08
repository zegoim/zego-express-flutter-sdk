package im.zego.zego_express_engine;

import java.nio.*;

public interface IZegoFlutterMediaPlayerAudioHandler {
    /**
     * The callback triggered when the media player throws out audio frame data.
     *
     * Description: The callback triggered when the media player throws out audio frame data.
     * Trigger: The callback is generated when the media player starts playing.
     * Caution: The callback does not actually take effect until call [setAudioHandler] to set.
     * Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
     *
     * @param mediaPlayerIndex Callback player index.
     * @param data Raw data of audio frames.
     * @param dataLength Data length.
     * @param param audio frame flip mode.
     */
    public void onAudioFrame(int mediaPlayerIndex, ByteBuffer data, int dataLength,
                             ZGFlutterAudioFrameParam param);
}