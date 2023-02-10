package im.zego.zego_express_engine;

import java.nio.*;

public class ZGFlutterVideoEncodedFrameParam {
    /** Video encoded frame format */
    public ZGFlutterVideoEncodedFrameFormat format;

    /** Whether it is a keyframe */
    public boolean isKeyFrame;

    /** Video frame counterclockwise rotation angle */
    public int rotation;

    /** Video frame width */
    public int width;

    /** Video frame height */
    public int height;

    /** SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB] */
    public ByteBuffer SEIData;

    /** Length of the SEI data (Optional, if you don't need to send SEI, set it to 0. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB] */
    public int SEIDataLength;
}
