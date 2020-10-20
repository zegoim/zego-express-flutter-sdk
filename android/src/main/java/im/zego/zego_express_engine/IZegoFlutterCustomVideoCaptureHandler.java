package im.zego.zego_express_engine;

public interface IZegoFlutterCustomVideoCaptureHandler {
    /**
     * The callback triggered when the SDK is ready to receive captured video data. Only those video data that are sent to the SDK after this callback is received are valid.
     * 
     * @param channel publish channel, It is consistent with Dart API
     */
    void onStart(int channel);

    /**
     * The callback triggered when SDK stops receiving captured video data.
     *
     * After receiving this callback, the developer is advised to set the saved client object to nil, otherwise the memory occupied by the client object will always exist
     * @param channel publish channel, It is consistent with Dart API
     */
    void onStop(int channel);
}
