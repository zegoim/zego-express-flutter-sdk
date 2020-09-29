package im.zego.zego_express_engine.internal;

import android.graphics.SurfaceTexture;

import java.nio.ByteBuffer;

import im.zego.zego_express_engine.ZegoCustomVideoCaptureClient;
import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoVideoFrameFormat;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public class ZegoCustomVideoCaptureClientImpl extends ZegoCustomVideoCaptureClient {

    private ZegoPublishChannel mChannel;
    private ZegoVideoFrameParam mParam;
    private ZegoVideoMirrorMode mMirrorMode;

    public ZegoCustomVideoCaptureClientImpl(ZegoPublishChannel channel) {
        this.mChannel = channel;
    }

    @Override
    public void setVideoMirrorMode(int mode) {
        mMirrorMode = ZegoVideoMirrorMode.getZegoVideoMirrorMode(mode);
        ZegoExpressEngine.getEngine().setVideoMirrorMode(mMirrorMode, mChannel);
    }

    @Override
    public void sendByteBuffer(ByteBuffer data, int dataLength, VideoFrameParam param, long referenceTimeMillisecond) {
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

        ZegoExpressEngine.getEngine().sendCustomVideoCaptureRawData(data, dataLength, mParam, referenceTimeMillisecond, mChannel);

        // Android 使用 Texture Renderer 和 PlatformView 行为一致
    }

    @Override
    public SurfaceTexture getSurfaceTexture() {
        return ZegoExpressEngine.getEngine().getCustomVideoCaptureSurfaceTexture(mChannel);
    }

    @Override
    public void sendGLTextureData(int textureID, int width, int height, long referenceTimeMillisecond) {
        ZegoExpressEngine.getEngine().sendCustomVideoCaptureTextureData(textureID, width, height, referenceTimeMillisecond, mChannel);
    }

}
