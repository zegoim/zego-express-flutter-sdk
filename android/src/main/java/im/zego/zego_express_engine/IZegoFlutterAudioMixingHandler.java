package im.zego.zego_express_engine;

public interface IZegoFlutterAudioMixingHandler {

    /**
     * Audio mixing callback.
     *
     * Available since: 1.9.0
     * Description: The callback for copying audio data to the SDK for audio mixing. This function should be used together with [enableAudioMixing].
     * Use cases: Developers can use this function when they need to mix their own songs, sound effects or other audio data into the publishing stream.
     * When to trigger: It will triggered after [createEngine], and call [enableAudioMixing] turn on audio mixing.
     * Restrictions: Supports 16k 32k 44.1k 48k sample rate, mono or dual channel, 16-bit deep PCM audio data.
     * Caution: This callback is a high frequency callback. To ensure the quality of the mixing data, please do not handle time-consuming operations in this callback.
     *
     * @param expectedDataLength Expected length of incoming audio mixing data.
     * @return The audio data provided by the developer that is expected to be mixed into the publishing stream.
     */
    public ZGFlutterAudioMixData onAudioMixingCopyData(int expectedDataLength);
}
