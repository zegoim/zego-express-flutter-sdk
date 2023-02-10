package im.zego.zego_express_engine;

import java.nio.*;

public interface IZegoFlutterCustomVideoRenderHandler {
    /**
     * When custom video rendering is enabled, the original video frame data collected by the local preview is called back.
     * <p>
     * Available since: 1.1.0
     * Description: When using custom video rendering, the SDK callbacks the original video frame data collected by the local preview, which is rendered by the developer.
     * Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
     * When to Trigger: When the local preview is turned on, when the SDK collects the local preview video frame data.
     * Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
     *
     * @param data       Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
     * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
     * @param param      Video frame parameters.
     * @param flipMode   video flip mode.
     * @param channel    Publishing stream channel.
     */
    public void onCapturedVideoFrameRawData(ByteBuffer[] data, int[] dataLength,
                                            ZGFlutterVideoFrameParam param, ZGFlutterVideoFlipMode flipMode,
                                            ZGFlutterPublishChannel channel);

    /**
     * When custom video rendering is enabled, the remote end pulls the original video frame data to call back, and distinguishes different streams by streamID.
     * <p>
     * Available since: 1.1.0
     * Description: When custom video rendering is enabled, the SDK calls back the remote end to pull the original video frame data, distinguishes different streams by streamID, and renders them by the developer.
     * Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
     * When to Trigger: After starting to stream, when the SDK receives the video frame data of the remote stream.
     * Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
     *
     * @param data       Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
     * @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
     * @param param      Video frame parameters.
     * @param streamID   Stream ID.
     */
    public void onRemoteVideoFrameRawData(ByteBuffer[] data, int[] dataLength,
                                          ZGFlutterVideoFrameParam param, String streamID);

    /**
     * Custom video rendering remote playing stream video frame undecoded data callback.
     * <p>
     * Available since: 1.10.0
     * Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [EncodedData], this function will be called back with the remote playing stream undecoded video frame data.
     * When to Trigger: After calling [startPlayingStream], the SDK will call back when the remote streaming video frame data is received.
     * Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
     *
     * @param data                     Encoded data of video frames.
     * @param dataLength               Data length.
     * @param param                    Video frame parameters.
     * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
     * @param streamID                 Stream ID.
     */
    public void onRemoteVideoFrameEncodedData(ByteBuffer data, int dataLength,
                                              ZGFlutterVideoEncodedFrameParam param,
                                              long referenceTimeMillisecond, String streamID);
}