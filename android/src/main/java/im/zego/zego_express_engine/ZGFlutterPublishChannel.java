package im.zego.zego_express_engine;

public enum ZGFlutterPublishChannel {
    /** The main (default/first) publish channel. */
    MAIN(0),
    /** The auxiliary (second) publish channel */
    AUX(1),
    /** The third publish channel */
    THIRD(2),
    /** The fourth publish channel */
    FOURTH(3);

    private int value;

    private ZGFlutterPublishChannel(int value) { this.value = value; }

    public int value() { return this.value; }

    public static ZGFlutterPublishChannel getZegoPublishChannel(int value) {
        try {

            if (MAIN.value == value) {
                return MAIN;
            }

            if (AUX.value == value) {
                return AUX;
            }

            if (THIRD.value == value) {
                return THIRD;
            }

            if (FOURTH.value == value) {
                return FOURTH;
            }

        } catch (Exception e) {
            throw new RuntimeException("The enumeration cannot be found");
        }
        return null;
    }
}
