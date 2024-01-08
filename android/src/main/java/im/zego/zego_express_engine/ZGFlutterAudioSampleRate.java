package im.zego.zego_express_engine;

/** audio sample rate. */
public enum ZGFlutterAudioSampleRate {
    /** Unknown */
    UNKNOWN(0),
    /** 8K */
    AUDIO_SAMPLE_RATE_8K(8000),
    /** 16K */
    AUDIO_SAMPLE_RATE_16K(16000),
    /** 22.05K */
    AUDIO_SAMPLE_RATE_22K(22050),
    /** 24K */
    AUDIO_SAMPLE_RATE_24K(24000),
    /** 32K */
    AUDIO_SAMPLE_RATE_32K(32000),
    /** 44.1K */
    AUDIO_SAMPLE_RATE_44K(44100),
    /** 48K */
    AUDIO_SAMPLE_RATE_48K(48000);

    private int value;

    private ZGFlutterAudioSampleRate(int value) { this.value = value; }

    public int value() { return this.value; }

    public static ZGFlutterAudioSampleRate getZGFlutterAudioSampleRate(int value) {
        try {

            if (UNKNOWN.value == value) {
                return UNKNOWN;
            }

            if (AUDIO_SAMPLE_RATE_8K.value == value) {
                return AUDIO_SAMPLE_RATE_8K;
            }

            if (AUDIO_SAMPLE_RATE_16K.value == value) {
                return AUDIO_SAMPLE_RATE_16K;
            }

            if (AUDIO_SAMPLE_RATE_22K.value == value) {
                return AUDIO_SAMPLE_RATE_22K;
            }

            if (AUDIO_SAMPLE_RATE_24K.value == value) {
                return AUDIO_SAMPLE_RATE_24K;
            }

            if (AUDIO_SAMPLE_RATE_32K.value == value) {
                return AUDIO_SAMPLE_RATE_32K;
            }

            if (AUDIO_SAMPLE_RATE_44K.value == value) {
                return AUDIO_SAMPLE_RATE_44K;
            }

            if (AUDIO_SAMPLE_RATE_48K.value == value) {
                return AUDIO_SAMPLE_RATE_48K;
            }

        } catch (Exception e) {
            throw new RuntimeException("The enumeration cannot be found");
        }
        return null;
    }
}