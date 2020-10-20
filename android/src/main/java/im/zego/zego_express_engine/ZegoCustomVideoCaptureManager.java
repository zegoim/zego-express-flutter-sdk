package im.zego.zego_express_engine;

import android.graphics.SurfaceTexture;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.logging.Handler;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.callback.IZegoCustomVideoCaptureHandler;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoVideoFrameFormat;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public class ZegoCustomVideoCaptureManager extends IZegoCustomVideoCaptureHandler {

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

        public static ZegoCustomVideoCaptureManager.VideoFrameFormat getVideoFrameFormat(int value) {
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
        public ZegoCustomVideoCaptureManager.VideoFrameFormat format;

        /** Number of bytes per line (for example: RGBA only needs to consider strides [0], I420 needs to consider strides [0,1,2]) */
        final public int[] strides = new int[4];

        /** Video frame width */
        public int width;

        /** Video frame height */
        public int height;

        /** Video frame rotation */
        public int rotation;

    }

    private static ZegoCustomVideoCaptureManager singleton;

    private ZegoVideoFrameParam mParam;
    private ZegoVideoMirrorMode mMirrorMode;

    private static IZegoFlutterCustomVideoCaptureHandler mHander;

    public ZegoCustomVideoCaptureManager() {

    }

    /**
     * Get the custom video capture manager instance
     */
    public static synchronized ZegoCustomVideoCaptureManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoCustomVideoCaptureManager();
        }
        return singleton;
    }

    /**
     * Sets the event callback handler for custom video capture.
     * 
     * Developers need to pass in the callback object that implements [ZegoCustomVideoCaptureDelegate] and open [enableCustomVideoCapture] in the Dart API to make the custom capture module take effect.
     * When the developer calls [startPreview]/[stopPreview] or [startPublishingStream]/[stopPublishingStream] in the Dart API, the SDK will notify the developer  the start/stop of the custom video capture, and the developer can receive [onStart] and [onStop] Start and stop the external input source after the notification.
     * This API call is set at any time before [enableCustomVideoCapture] is enabled in Dart
     * @param handler the callback object that implements the [ZegoCustomVideoCaptureDelegate]
     */
    public void setCustomVideoCaptureHandler(IZegoFlutterCustomVideoCaptureHandler handler) {
        mHander = handler;
    }

    /**
     * Sets the video mirroring mode.
     *
     * This function can be called to set whether the local preview video and the published video have mirror mode enabled.
     *
     * @param mode Mirror mode for previewing or publishing the stream. 0: Only preview mirror. 1: Both preview and publish mirror. 2: No Mirror. 3. Only publish mirror. No mirror by default.
     * @param channel publish channel, It is consistent with Dart API
     */
    public void setVideoMirrorMode(int mode, int channel) {
        mMirrorMode = ZegoVideoMirrorMode.getZegoVideoMirrorMode(mode);
        ZegoExpressEngine.getEngine().setVideoMirrorMode(mMirrorMode, ZegoPublishChannel.getZegoPublishChannel(channel));
    }

    /**
     * Sends the video frames (Raw Data) produced by custom video capture to the SDK.
     *
     * This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
     *
     * @param data video frame data
     * @param dataLength video frame data length
     * @param param video frame param
     * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
     * @param channel publish channel, It is consistent with Dart API
     */
    public void sendRawData(ByteBuffer data, int dataLength, ZegoCustomVideoCaptureManager.VideoFrameParam param, long referenceTimeMillisecond, int channel) {
        if(mParam == null) {
            mParam = new ZegoVideoFrameParam();
        }
        mParam.format = ZegoVideoFrameFormat.getZegoVideoFrameFormat(param.format.value());
        mParam.width = param.width;
        mParam.height = param.height;
        mParam.rotation = param.rotation;
        mParam.strides[0] = param.strides[0];
        mParam.strides[1] = param.strides[1];
        mParam.strides[2] = param.strides[2];
        mParam.strides[3] = param.strides[3];

        ZegoExpressEngine.getEngine().sendCustomVideoCaptureRawData(data, dataLength, mParam, referenceTimeMillisecond, ZegoPublishChannel.getZegoPublishChannel(channel));

        // Android 使用 Texture Renderer 和 PlatformView 行为一致
    }

    /**
     * Gets the SurfaceTexture instance.
     *
     * @param channel publish channel, It is consistent with Dart API
     * @return SurfaceTexture instance
     */
    public SurfaceTexture getSurfaceTexture(int channel) {
        return ZegoExpressEngine.getEngine().getCustomVideoCaptureSurfaceTexture(ZegoPublishChannel.getZegoPublishChannel(channel));
    }

    /**
     * Sends the video frames (Texture Data) produced by custom video capture to the SDK.
     *
     * This api need to be start called after the [onStart] callback notification and to be stop called call after the [onStop] callback notification.
     *
     * @param textureID texture ID
     * @param width Video frame width
     * @param height Video frame height
     * @param referenceTimeMillisecond Timestamp of this video frame
     * @param channel publish channel, It is consistent with Dart API
     */
    public void sendGLTextureData(int textureID, int width, int height, long referenceTimeMillisecond, int channel) {
        ZegoExpressEngine.getEngine().sendCustomVideoCaptureTextureData(textureID, width, height, referenceTimeMillisecond, ZegoPublishChannel.getZegoPublishChannel(channel));
    }

    /**
     * Internal function. Ignore this.
     */
    public void onStart(ZegoPublishChannel channel) {
        //ZegoCustomVideoCaptureClient client = new ZegoCustomVideoCaptureClientImpl(channel);
        //mClients.put(channel.value(), client);

        //IZegoCustomVideoCaptureCallback callback = mHandlers.get(channel.value());
        if(mHander != null) {
            mHander.onStart(channel.value());
        }
    }

    /**
     * Internal function. Ignore this.
     */
    public void onStop(ZegoPublishChannel channel) {
        //mClients.remove(channel.value());
        //IZegoCustomVideoCaptureCallback callback = mHandlers.get(channel.value());
        if(mHander != null) {
            mHander.onStop(channel.value());
        }
    }


}
