package im.zego.zego_express_engine;

import java.nio.*;
import android.graphics.SurfaceTexture;

public interface IZegoFlutterCustomVideoProcessHandler {
    /**
     * The SDK informs the developer that it is about to start custom video processing.
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, the SDK informs the developer that the video pre-processing is about to start, and it is recommended to initialize other resources(E.g. Beauty SDK) in this callback.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to Trigger: Open custom video pre-processing, after calling [startPreview] or [startPublishingStream] successfully.
     * Related callbacks: Custom video pre-processing end notification [onStop].
     * Related APIs: Call [setCustomVideoProcessHandler] function to set custom video pre-processing callback.
     *
     * @param channel Publishing stream channel.
     */
    public void onStart(ZGFlutterPublishChannel channel);

    /**
     * The SDK informs the developer to stop custom video processing.
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, the SDK informs the developer that the video pre-processing is about to end, and it is recommended to destroy other resources(E.g. Beauty SDK) in this callback.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to Trigger: If you call [startPreview] to start the preview and [startPublishingStream] to start the push stream at the same time after you start the custom capture, you should call [stopPreview] to stop the preview and [stopPublishingStream] to stop the push stream before the callback will be triggered.
     * Related callbacks: Custom video pre-processing start notification [onStart].
     * Related APIs: Call [setCustomVideoProcessHandler] function to set custom video pre-processing callback.
     *
     * @param channel Publishing stream channel.
     */
    public void onStop(ZGFlutterPublishChannel channel);

    /**
     * Call back when the original video data of type [Texture] is obtained.
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedTextureData] to send the processed data back to the SDK, otherwise it will cause frame loss.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeGLTexture2D].
     * Platform differences: It only takes effect on the Android platform.
     *
     * @param textureID Texture ID.
     * @param width Texture width.
     * @param height Texture height.
     * @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
     * @param channel Publishing stream channel.
     */
    public void onCapturedUnprocessedTextureData(int textureID, int width, int height,
                                                 long referenceTimeMillisecond,
                                                 ZGFlutterPublishChannel channel);

    /**
     * Call back when the original video data is obtained.
     *
     * Available since: 2.2.0
     * Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedRawData] to send the processed data back to the SDK, otherwise it will cause frame loss.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeRawData].
     * Platform differences: It only takes effect on the Windows platform.
     *
     * @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
     * @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
     * @param param Video frame parameters.
     * @param referenceTimeMillisecond Video frame reference time, UNIX timestamp, in milliseconds.
     * @param channel Publishing stream channel.
     */
    public void onCapturedUnprocessedRawData(ByteBuffer data, int[] dataLength,
                                             ZGFlutterVideoFrameParam param,
                                             long referenceTimeMillisecond,
                                             ZGFlutterPublishChannel channel);

    /**
     * When the developer chooses the SurfaceTexture video pre-processing type, the SurfaceTexture used to input the original video data is obtained through this interface and passed to the SDK.
     *
     * Available since: 2.2.0
     * Description: To enable custom video pre-processing, developers need to create a SurfaceTexture object and listen to its member method [setOnFrameAvailableListener], and then return the corresponding SurfaceTexture. When the SDK raw video data input is completed, the developer will receive [onFrameAvailable]. After the developer passes the SurfaceTexture(input) object to the SDK, he can call [getCustomVideoProcessOutputSurfaceTexture] to get the SurfaceTexture used by the SDK to encode and push the stream, and then the developer can pass the video processed data into this SurfaceTexture(output) object.
     * Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
     * Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeSurfaceTexture].
     * Platform differences: It only takes effect on the Android platform.
     *
     * @param width The capture width of the original video.
     * @param height The capture height of the original video.
     * @param channel Publishing stream channel.
     * @return SurfaceTexture instance.
     */
    public SurfaceTexture getCustomVideoProcessInputSurfaceTexture(int width, int height,
                                                                   ZGFlutterPublishChannel channel);
}
