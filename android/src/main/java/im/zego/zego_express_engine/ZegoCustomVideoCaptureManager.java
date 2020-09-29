package im.zego.zego_express_engine;

import java.util.HashMap;

import im.zego.zego_express_engine.IZegoCustomVideoCaptureCallback;
import im.zego.zego_express_engine.ZegoCustomVideoCaptureClient;
import im.zego.zego_express_engine.internal.ZegoCustomVideoCaptureClientImpl;

import im.zego.zegoexpress.callback.IZegoCustomVideoCaptureHandler;
import im.zego.zegoexpress.constants.ZegoPublishChannel;

public class ZegoCustomVideoCaptureManager extends IZegoCustomVideoCaptureHandler {
    private static ZegoCustomVideoCaptureManager singleton;
    private static HashMap<Integer, IZegoCustomVideoCaptureCallback> mHandlers = new HashMap<>();
    private static HashMap<Integer, ZegoCustomVideoCaptureClient> mClients = new HashMap<>();

    private ZegoCustomVideoCaptureManager() {
    }

    public static synchronized ZegoCustomVideoCaptureManager getInstance() {
        if (singleton == null) {
            singleton = new ZegoCustomVideoCaptureManager();
        }
        return singleton;
    }

    public void setCustomVideoCaptureCallback(IZegoCustomVideoCaptureCallback handler, int channel) {
        mHandlers.put(channel, handler);
    }

    public void onStart(ZegoPublishChannel channel) {
        ZegoCustomVideoCaptureClient client = new ZegoCustomVideoCaptureClientImpl(channel);
        mClients.put(channel.value(), client);

        IZegoCustomVideoCaptureCallback callback = mHandlers.get(channel.value());
        if(callback != null) {
            callback.onStart(client);
        }
    }

    public void onStop(ZegoPublishChannel channel) {
        mClients.remove(channel.value());
        IZegoCustomVideoCaptureCallback callback = mHandlers.get(channel.value());
        if(callback != null) {
            callback.onStop();
        }
    }
}
