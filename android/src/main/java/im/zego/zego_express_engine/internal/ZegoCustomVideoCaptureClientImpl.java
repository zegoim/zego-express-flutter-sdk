package im.zego.zego_express_engine.internal;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public class ZegoCustomVideoCaptureClientImpl extends ZegoCustomVideoCaptureClient {

    private ZegoPublishChannel mChannel;
    public ZegoCustomVideoCaptureClientImpl(ZegoPublishChannel channel) {
        this.mChannel = channel;
    }

    @Override
    public void setVideoMirrorMode(int mode) {

    }

    @Override
    public void sendByteBuffer(ByteBuffer data, int dataLength, ZegoVideoFrameParam param, long referenceTimeMillisecond) {
        ZegoExpressEngine.getEngine().sendCustomVideoCaptureRawData(data, dataLength, param, referenceTimeMillisecond, mChannel);
    }
}
