package im.zego.zego_express_engine.internal;

import android.graphics.SurfaceTexture;

import java.nio.ByteBuffer;

import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

//import im.zego.zegoexpress.entity.ZegoVideoFrameParam;

public abstract class ZegoCustomVideoCaptureClient {

    public abstract void setVideoMirrorMode(int mode);

    public abstract void sendByteBuffer(ByteBuffer data, int dataLength, ZegoVideoFrameParam param, long referenceTimeMillisecond);
}
