package im.zego.zego_express_engine;

public enum ZGFlutterVideoEncodedFrameFormat {
    /** AVC AVCC format */
    AVCC(0),
    /** AVC Annex-B format */
    AnnexB(1),
    /** VP8 format */
    VP8(2),
    /** HEVC AVCC format */
    HEVCAVCC(3),
    /** HEVC Annex-B format */
    HEVCAnnexB(4);

    private int value;

    private ZGFlutterVideoEncodedFrameFormat(int value) { this.value = value; }

    public int value() { return this.value; }

    public static ZGFlutterVideoEncodedFrameFormat getZegoVideoEncodedFrameFormat(int value) {
        try {

            if (AVCC.value == value) {
                return AVCC;
            }

            if (AnnexB.value == value) {
                return AnnexB;
            }

            if (VP8.value == value) {
                return VP8;
            }

            if (HEVCAVCC.value == value) {
                return HEVCAVCC;
            }

            if (HEVCAnnexB.value == value) {
                return HEVCAnnexB;
            }

        } catch (Exception e) {
            throw new RuntimeException("The enumeration cannot be found");
        }
        return null;
    }
}
