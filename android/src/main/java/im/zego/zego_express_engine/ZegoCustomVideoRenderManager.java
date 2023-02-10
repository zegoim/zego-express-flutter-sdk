package im.zego.zego_express_engine;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.callback.IZegoCustomVideoRenderHandler;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoVideoFlipMode;
import im.zego.zegoexpress.entity.ZegoVideoEncodedFrameParam;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public class ZegoCustomVideoRenderManager extends IZegoCustomVideoRenderHandler {

    private volatile static ZegoCustomVideoRenderManager singleton;

    private static IZegoFlutterCustomVideoRenderHandler mHander;

    public ZegoCustomVideoRenderManager() {
    }

    /**
     * Get the custom video capture manager instance
     */
    public static synchronized ZegoCustomVideoRenderManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoCustomVideoRenderManager();
        }
        return singleton;
    }

    /**
     * Sets up the event callback handler for custom video rendering.
     *
     * Available since: 1.9.0
     * Description: Custom video rendering, where the developer is responsible for rendering the video data onto the UI components. With the change callback set, a callback that throws video data to the developer will be triggered when the developer calls startPreview, startPublishingStream, and startPlayingStream.
     * Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
     * Related callbacks: [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
     * Note: This function is only available in ZegoExpressVideo SDK!
     *
     * @param handler Custom video render handler
     */
    public void setCustomVideoRenderHandler(IZegoFlutterCustomVideoRenderHandler handler) {
        mHander = handler;
    }

    @Override
    public void onCapturedVideoFrameRawData(ByteBuffer[] data, int[] dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode, ZegoPublishChannel channel) {
        if (mHander != null) {
            ZGFlutterVideoFrameParam videoFrameParam = new ZGFlutterVideoFrameParam();
            videoFrameParam.format = ZGFlutterVideoFrameFormat.getVideoFrameFormat(param.format.value());
            videoFrameParam.height = param.height;
            videoFrameParam.width = param.width;
            videoFrameParam.rotation = param.rotation;
            for (int i = 0; i < 4; i++) {
                videoFrameParam.strides[i] = param.strides[i];
            }

            mHander.onCapturedVideoFrameRawData(data, dataLength, videoFrameParam, ZGFlutterVideoFlipMode.getZegoVideoFlipMode(flipMode.value()), ZGFlutterPublishChannel.getZegoPublishChannel(channel.value()));
        }
    }

    @Override
    public void onRemoteVideoFrameRawData(ByteBuffer[] data, int[] dataLength, ZegoVideoFrameParam param, String streamID) {
        if (mHander != null) {
            ZGFlutterVideoFrameParam videoFrameParam = new ZGFlutterVideoFrameParam();
            videoFrameParam.format = ZGFlutterVideoFrameFormat.getVideoFrameFormat(param.format.value());
            videoFrameParam.height = param.height;
            videoFrameParam.width = param.width;
            videoFrameParam.rotation = param.rotation;
            for (int i = 0; i < 4; i++) {
                videoFrameParam.strides[i] = param.strides[i];
            }

            mHander.onRemoteVideoFrameRawData(data, dataLength, videoFrameParam, streamID);
        }
    }

    @Override
    public void onRemoteVideoFrameEncodedData(ByteBuffer data, int dataLength, ZegoVideoEncodedFrameParam param, long referenceTimeMillisecond, String streamID) {
        if (mHander != null) {
            ZGFlutterVideoEncodedFrameParam videoEncodedFrameParam = new ZGFlutterVideoEncodedFrameParam();
            videoEncodedFrameParam.isKeyFrame = param.isKeyFrame;
            videoEncodedFrameParam.SEIData = param.SEIData;
            videoEncodedFrameParam.SEIDataLength = param.SEIDataLength;
            videoEncodedFrameParam.format = ZGFlutterVideoEncodedFrameFormat.getZegoVideoEncodedFrameFormat(param.format.value());
            videoEncodedFrameParam.height = param.height;
            videoEncodedFrameParam.width = param.width;
            videoEncodedFrameParam.rotation = param.rotation;
            mHander.onRemoteVideoFrameEncodedData(data,dataLength, videoEncodedFrameParam, referenceTimeMillisecond, streamID);
        }
    }
}
