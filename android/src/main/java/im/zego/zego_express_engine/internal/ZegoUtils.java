package im.zego.zego_express_engine.internal;

public class ZegoUtils {
    /* Utils */

    public static boolean boolValue(Boolean number) {
        return number != null && number;
    }

    public static int intValue(Number number) {
        return number != null ? number.intValue() : 0;
    }

    public static long longValue(Number number) {
        return number != null ? number.longValue() : 0;
    }

    public static float floatValue(Number number) {
        return number != null ? number.floatValue() : .0f;
    }

    public static double doubleValue(Number number) {
        return number != null ? number.doubleValue() : .0f;
    }
}
