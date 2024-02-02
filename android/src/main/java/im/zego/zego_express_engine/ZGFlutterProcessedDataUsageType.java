package im.zego.zego_express_engine;

/** Usage of Custom video processed data. */
public enum ZGFlutterProcessedDataUsageType {
    /** Preview. */
    PREVIEW(0),
    /** Publish. */
    PUBLISH(1),
    /** Both preview and publish. */
    BOTH(2);

    private int value;

    private ZGFlutterProcessedDataUsageType(int value) { this.value = value; }

    public int value() { return this.value; }

    public static ZGFlutterProcessedDataUsageType getZGFlutterProcessedDataUsageType(int value) {
        try {

            if (PREVIEW.value == value) {
                return PREVIEW;
            }

            if (PUBLISH.value == value) {
                return PUBLISH;
            }

            if (BOTH.value == value) {
                return BOTH;
            }

        } catch (Exception e) {
            throw new RuntimeException("The enumeration cannot be found");
        }
        return null;
    }
}
