package im.zego.zego_express_engine;

import java.nio.ByteBuffer;

public class ZGFlutterAudioMixData {

    /** Audio PCM data that needs to be mixed into the stream */
    public ByteBuffer audioData;

    /** the length of the audio PCM data that needs to be mixed into the stream. If the data length is sufficient, it must be the same as expectedDataLength */
    public int audioDataLength;

    /** Audio data attributes, including sample rate and number of channels. Currently supports 16k, 32k, 44.1k, 48k sampling rate, mono or stereo, 16-bit deep PCM data. Developers need to explicitly specify audio data attributes, otherwise mixing will not take effect. */
    public ZGFlutterAudioFrameParam param = new ZGFlutterAudioFrameParam();

    /** SEI data, used to transfer custom data. When audioData is null, SEIData will not be sent */
    public ByteBuffer SEIData;

    /** SEI data length */
    public int SEIDataLength;
}
