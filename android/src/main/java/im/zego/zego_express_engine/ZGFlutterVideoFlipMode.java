package im.zego.zego_express_engine;

public enum ZGFlutterVideoFlipMode {
    /** No flip */
    NONE(0),
    /** X-axis flip */
    X(1),
    /** Y-axis flip */
    Y(2),
    /** X-Y-axis flip */
    XY(3);

    private int value;

    private ZGFlutterVideoFlipMode(int value) { this.value = value; }

    public int value() { return this.value; }

    public static ZGFlutterVideoFlipMode getZegoVideoFlipMode(int value) {
        try {

            if (NONE.value == value) {
                return NONE;
            }

            if (X.value == value) {
                return X;
            }

            if (Y.value == value) {
                return Y;
            }

            if (XY.value == value) {
                return XY;
            }

        } catch (Exception e) {
            throw new RuntimeException("The enumeration cannot be found");
        }
        return null;
    }
}
