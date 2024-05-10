#ifndef ZEGO_CUSTOM_VIDEO_PROCESS_MANAGER_H_
#define ZEGO_CUSTOM_VIDEO_PROCESS_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterCustomVideoProcessHandler {
  protected:
    virtual ~IZegoFlutterCustomVideoProcessHandler() {}

  public:
    /// Call back when the original video data is obtained.
    ///
    /// Available since: 2.2.0
    /// Description: When the custom video pre-processing is turned on, after calling [setCustomVideoProcessHandler] to set the callback, the SDK receives the original video data and calls back to the developer. After the developer has processed the original image, he must call [sendCustomVideoProcessedRawData] to send the processed data back to the SDK, otherwise it will cause frame loss.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to Trigger: When the custom video pre-processing is enabled, the SDK collects the original video data.
    /// Restrictions: This interface takes effect when [enableCustomVideoProcessing] is called to enable custom video pre-processing and the bufferType of config is passed in [ZegoVideoBufferTypeRawData].
    /// Platform differences: It only takes effect on the Windows platform.
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param param Video frame parameters.
    /// @param referenceTimeMillisecond Video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel.
    virtual void onCapturedUnprocessedRawData(const unsigned char ** /*data*/,
                                              unsigned int * /*dataLength*/,
                                              ZGFlutterVideoFrameParam /*param*/,
                                              unsigned long long /*referenceTimeMillisecond*/,
                                              ZGFlutterPublishChannel /*channel*/) {}
};

namespace ZEGO::EXPRESS {
    class IZegoCustomVideoProcessHandler;
};
class ZegoCustomVideoProcessHandler;

class FLUTTER_PLUGIN_EXPORT ZegoCustomVideoProcessManager{
public:
    static std::shared_ptr<ZegoCustomVideoProcessManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoProcessHandler> getHandler();
    
    /// Sets up the event callback handler for custom video processing.
    ///
    /// Available since: 2.2.0 (Android/iOS/macOS native), 2.4.0 (Windows/macOS C++).
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// Restrictions: None.
    /// Related APIs: Call [enableCustomVideoProcessing] function to enable custom video pre-processing callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video process handler.Required: Yes.
    void setCustomVideoProcessHandler(std::shared_ptr<IZegoFlutterCustomVideoProcessHandler> handler);

    /// Send the original video data after the pre-processing of the custom video to the SDK, and support other channels to push the stream.
    ///
    /// Available since: 2.4.0
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: None.
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param params video frame param.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param channel Publishing stream channel.Required: No.Default value: Main publish channel.
    void sendCustomVideoProcessedRawData(const unsigned char **data, unsigned int *dataLength,
                                    ZGFlutterVideoFrameParam params,
                                    unsigned long long referenceTimeMillisecond,
                                    ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

    /// Send the original video data after the pre-processing of the custom video to the SDK, support other channels to push the stream, and support specified data usage.
    ///
    /// Available since: 3.14.0
    /// Description: When the developer opens the custom pre-processing, by calling [setCustomVideoProcessHandler], you can set the custom video pre-processing callback to obtain the original video data.
    /// Use cases: After the developer collects the video data by himself or obtains the video data collected by the SDK, if the basic beauty and watermark functions of the SDK cannot meet the needs of the developer (for example, the beauty effect cannot meet the expectations), the ZegoEffects SDK can be used to perform the video Some special processing, such as beautifying, adding pendants, etc., this process is the pre-processing of custom video.
    /// When to call: Must be called in the [onCapturedUnprocessedCVPixelbuffer] callback.
    /// Restrictions: None.
    /// Platform differences: Only valid on Windows platform.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param data Raw video data. RGB format data storage location is data[0], YUV format data storage location is Y component：data[0], U component：data[1], V component：data[2].
    /// @param dataLength Raw video data length. RGB format data length storage location is dataLength[0], YUV format data storage location respectively Y component length：dataLength[0], U component length：dataLength[1], V component length：dataLength[2].
    /// @param params video frame param.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param usage Data usage
    /// @param channel Publishing stream channel.Required: No.Default value: Main publish channel.
    void sendCustomVideoProcessedRawData(
        const unsigned char **data, unsigned int *dataLength, ZGFlutterVideoFrameParam params,
        unsigned long long referenceTimeMillisecond, ZGFlutterProcessedDataUsageType usage,
        ZGFlutterPublishChannel channel = ZG_FLUTTER_PUBLISH_CHANNEL_MAIN);

private:
    friend class ZegoCustomVideoProcessHandler;
    std::shared_ptr<IZegoFlutterCustomVideoProcessHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoProcessHandler> zegoHandler_ = nullptr;
};

#endif  // ZEGO_CUSTOM_VIDEO_PROCESS_MANAGER_H_
