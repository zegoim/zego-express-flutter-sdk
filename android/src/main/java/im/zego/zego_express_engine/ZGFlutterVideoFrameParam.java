package im.zego.zego_express_engine;

public class ZGFlutterVideoFrameParam {
    /** Video frame format */
    public ZGFlutterVideoFrameFormat format;

    /** Number of bytes per line (for example: RGBA only needs to consider strides [0], I420 needs to consider strides [0,1,2]) */
    final public int[] strides = new int[4];

    /** Video frame width */
    public int width;

    /** Video frame height */
    public int height;

    /** Video frame rotation */
    public int rotation;
}
