package im.zego.zego_express_engine.internal;

public interface IZegoCustomVideoCaptureCallback {
    void onStart(ZegoCustomVideoCaptureClient client);
    void onStop();
}
