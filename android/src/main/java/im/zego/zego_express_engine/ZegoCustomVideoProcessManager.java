package im.zego.zego_express_engine;

import android.graphics.SurfaceTexture;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zego_express_engine.internal.ZegoLog;
import im.zego.zegoexpress.callback.IZegoCustomVideoProcessHandler;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;
import im.zego.zegoexpress.constants.ZegoProcessedDataUsageType;

public class ZegoCustomVideoProcessManager extends IZegoCustomVideoProcessHandler {
    private volatile static ZegoCustomVideoProcessManager singleton;

    private static IZegoFlutterCustomVideoProcessHandler mHander;

    public ZegoCustomVideoProcessManager() {
    }

    /**
     * Get the custom video capture manager instance
     */
    public static synchronized ZegoCustomVideoProcessManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoCustomVideoProcessManager();
        }
        return singleton;
    }

    /**
     * Sets up the event callback handler for custom video processing.
     *
     * Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
     * Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to call: Called before [startPreview], [startPublishingStream] and [createRealTimeSequentialDataManager], otherwise it may cause the timing of obtaining video data to be too slow.
     * Restrictions: None.
     * Related APIs: Call [enableCustomVideoProcessing] function to enable custom video pre-processing callback.
     * Note: This function is only available in ZegoExpressVideo SDK!
     *
     * @param handler Custom video process handler.Required: Yes.
     */
    public void setCustomVideoProcessHandler(IZegoFlutterCustomVideoProcessHandler handler) {
        mHander = handler;
    }

    /**
     * Send the [Texture] type video data after the pre-processing of the custom video to the SDK.
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, the [Texture] format video data after the custom video pre-processing is sent to the SDK, and other channels are supported.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to call: Must be called in the [onCapturedUnprocessedTextureData] callback.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeGLTexture2D].
     * Platform differences: Only valid on Android platform.
     * Note: This function is only available in ZegoExpressVideo SDK!
     *
     * @param textureID texture ID.
     * @param width Texture width.
     * @param height Texture height.
     * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
     */
    public void sendProcessedTextureData(int textureID, int width, int height,
                                                             long referenceTimeMillisecond, ZGFlutterPublishChannel channel) {
        ZegoExpressEngine.getEngine().sendCustomVideoProcessedTextureData(textureID, width, height, referenceTimeMillisecond, ZegoPublishChannel.getZegoPublishChannel(channel.value()));
    }

    /**
     * Send the [Texture] type video data after the pre-processing of the custom video to the SDK (for the specified channel), and support specified data usage.
     *
     * Available since: 3.14.0
     * Description: When the custom video pre-processing is turned on, the [Texture] format video data after the custom video pre-processing is sent to the SDK, and other channels are supported.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to call: Must be called in the [onCapturedUnprocessedTextureData] callback.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeGLTexture2D].
     * Platform differences: Only valid on Android platform.
     * Note: This function is only available in ZegoExpressVideo SDK!
     *
     * @param textureID texture ID.
     * @param width Texture width.
     * @param height Texture height.
     * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
     * @param usage Data usage
     * @param channel Publishing stream channel.
     */
    public void sendCustomVideoProcessedTextureData(int textureID, int width, int height,
                                                             long referenceTimeMillisecond,
                                                             ZGFlutterProcessedDataUsageType usage,
                                                             ZGFlutterPublishChannel channel) {
        ZegoExpressEngine.getEngine().sendCustomVideoProcessedTextureData(textureID, width, height, referenceTimeMillisecond, 
            ZegoProcessedDataUsageType.getZegoProcessedDataUsageType(usage.value()),
            ZegoPublishChannel.getZegoPublishChannel(channel.value()));
    }

    /**
     * Get SurfaceTexture for output data (for the specified channel).
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, after the developer obtains the raw data of the SurfaceTexture type, this interface is called to obtain the SurfaceTexture used to output the video data, as the producer of the output data. After the developer completes the video pre-processing, send the processed video data to the SDK through this SurfaceTexture.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to call: It is recommended to call in the [onGetSurfaceTexture] callback function.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeSurfaceTexture].
     * Platform differences: It only takes effect on the Android platform.
     * Note: This function is only available in ZegoExpressVideo SDK!
     *
     * @param width Specify the width of the output video data.
     * @param height Specify the height of the output video data.
     * @param channel Publishing stream channel.
     * @return SurfaceTexture instance.
     */
    public SurfaceTexture
    getProcessOutputSurfaceTexture(int width, int height, ZGFlutterPublishChannel channel) {
        return ZegoExpressEngine.getEngine().getCustomVideoProcessOutputSurfaceTexture(width, height, ZegoPublishChannel.getZegoPublishChannel(channel.value()));
    }

    @Override
    public void onStart(ZegoPublishChannel channel) {
        ZegoLog.log("[CustomVideoProcess] [onStart] channel:%s", channel);
        if (mHander != null) {
            mHander.onStart(ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
    }

    @Override
    public void onStop(ZegoPublishChannel channel) {
        ZegoLog.log("[CustomVideoProcess] [onStop] channel:%s", channel);
        if (mHander != null) {
            mHander.onStop(ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
    }

    @Override
    public void onCapturedUnprocessedTextureData(int textureID, int width, int height, long referenceTimeMillisecond, ZegoPublishChannel channel) {
        if (mHander != null) {
            mHander.onCapturedUnprocessedTextureData(textureID, width, height, referenceTimeMillisecond, ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
    }

    @Override
    public void onCapturedUnprocessedRawData(ByteBuffer data, int[] dataLength, ZegoVideoFrameParam param, long referenceTimeMillisecond, ZegoPublishChannel channel) {
        if (mHander != null) {
            ZGFlutterVideoFrameParam videoFrameParam = new ZGFlutterVideoFrameParam();
            videoFrameParam.format = ZGFlutterVideoFrameFormat.getVideoFrameFormat(param.format.value());
            videoFrameParam.height = param.height;
            videoFrameParam.width = param.width;
            videoFrameParam.rotation = param.rotation;
            for (int i = 0; i < 4; i++) {
                videoFrameParam.strides[i] = param.strides[i];
            }

            mHander.onCapturedUnprocessedRawData(data, dataLength, videoFrameParam, referenceTimeMillisecond, ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
    }

    @Override
    public SurfaceTexture getCustomVideoProcessInputSurfaceTexture(int width, int height, ZegoPublishChannel channel) {
        if (mHander != null) {
            return mHander.getCustomVideoProcessInputSurfaceTexture(width, height, ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
        return null;
    }
}
