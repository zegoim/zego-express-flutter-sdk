package im.zego.zego_express_engine;

import android.graphics.SurfaceTexture;
import android.view.Surface;


import java.nio.ByteBuffer;

public abstract class ZegoCustomVideoCaptureClient {

    public enum VideoFrameFormat {
        /** Unknown format, will take platform default */
        Unknown(0),
        /** I420 (YUV420Planar) format */
        I420(1),
        /** NV12 (YUV420SemiPlanar) format */
        NV12(2),
        /** NV21 (YUV420SemiPlanar) format */
        NV21(3),
        /** BGRA32 format */
        BGRA32(4),
        /** RGBA32 format */
        RGBA32(5),
        /** ARGB32 format */
        ARGB32(6),
        /** ABGR32 format */
        ABGR32(7),
        /** I422 (YUV422Planar) format */
        I422(8);

        private int value;

        private VideoFrameFormat(int value) {
            this.value = value;
        }

        public int value() {
            return this.value;
        }

        public static VideoFrameFormat getVideoFrameFormat(int value) {
            try {

                if (Unknown.value == value) {
                    return Unknown;
                }

                if (I420.value == value) {
                    return I420;
                }

                if (NV12.value == value) {
                    return NV12;
                }

                if (NV21.value == value) {
                    return NV21;
                }

                if (BGRA32.value == value) {
                    return BGRA32;
                }

                if (RGBA32.value == value) {
                    return RGBA32;
                }

                if (ARGB32.value == value) {
                    return ARGB32;
                }

                if (ABGR32.value == value) {
                    return ABGR32;
                }

                if (I422.value == value) {
                    return I422;
                }

            } catch (Exception e) {
                throw new RuntimeException("The enumeration cannot be found");
            }
            return null;
        }
    }

    public static class VideoFrameParam {

        /** Video frame format */
        public VideoFrameFormat format;

        /** Number of bytes per line (for example: RGBA only needs to consider strides [0], I420 needs to consider strides [0,1,2]) */
        final public int[] strides = new int[4];

        /** Video frame width */
        public int width;

        /** Video frame height */
        public int height;

        /** Video frame rotation */
        public int rotation;

    }

    public abstract void setVideoMirrorMode(int mode);

    public abstract void sendByteBuffer(ByteBuffer data, int dataLength, ZegoCustomVideoCaptureClient.VideoFrameParam param, long referenceTimeMillisecond);

    public abstract SurfaceTexture getSurfaceTexture();

    public abstract void sendGLTextureData(int textureID, int width, int height, long referenceTimeMillisecond);

}
