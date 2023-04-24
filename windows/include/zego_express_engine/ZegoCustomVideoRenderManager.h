#ifndef ZEGO_CUSTOM_VIDEO_RENDER_MANAGER_H_
#define ZEGO_CUSTOM_VIDEO_RENDER_MANAGER_H_

#include "ZegoCustomVideoDefine.h"

class FLUTTER_PLUGIN_EXPORT IZegoFlutterCustomVideoRenderHandler {
  protected:
    virtual ~IZegoFlutterCustomVideoRenderHandler() {}

  public:
    /// When custom video rendering is enabled, the original video frame data collected by the local preview is called back.
    ///
    /// Available since: 1.1.0
    /// Description: When using custom video rendering, the SDK callbacks the original video frame data collected by the local preview, which is rendered by the developer.
    /// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
    /// When to Trigger: When the local preview is turned on, when the SDK collects the local preview video frame data.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param Video frame parameters.
    /// @param flipMode video flip mode.
    /// @param channel Publishing stream channel.
    virtual void onCapturedVideoFrameRawData(unsigned char ** /*data*/,
                                             unsigned int * /*dataLength*/,
                                             ZGFlutterVideoFrameParam /*param*/,
                                             ZGFlutterVideoFlipMode /*flipMode*/,
                                             ZGFlutterPublishChannel /*channel*/) {}

    /// When custom video rendering is enabled, the remote end pulls the original video frame data to call back, and distinguishes different streams by streamID.
    ///
    /// Available since: 1.1.0
    /// Description: When custom video rendering is enabled, the SDK calls back the remote end to pull the original video frame data, distinguishes different streams by streamID, and renders them by the developer.
    /// Use cases: Use a cross-platform interface framework or game engine; need to obtain the video frame data collected or streamed by the SDK for special processing.
    /// When to Trigger: After starting to stream, when the SDK receives the video frame data of the remote stream.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Raw video frame data (eg: RGBA only needs to consider data[0], I420 needs to consider data[0,1,2]).
    /// @param dataLength Data length (eg: RGBA only needs to consider dataLength[0], I420 needs to consider dataLength[0,1,2]).
    /// @param param Video frame parameters.
    /// @param streamID Stream ID.
    virtual void onRemoteVideoFrameRawData(unsigned char ** /*data*/, unsigned int * /*dataLength*/,
                                           ZGFlutterVideoFrameParam /*param*/,
                                           const std::string & /*streamID*/) {}

    /// Custom video rendering remote playing stream video frame undecoded data callback.
    ///
    /// Available since: 1.10.0
    /// Description: When [enableCustomVideoRender] is called to enable custom video rendering and the specified data type is [EncodedData], this function will be called back with the remote playing stream undecoded video frame data.
    /// When to Trigger: After calling [startPlayingStream], the SDK will call back when the remote streaming video frame data is received.
    /// Related APIs: Call [setCustomVideoRenderHandler] to set custom video rendering callback.
    ///
    /// @param data Encoded data of video frames.
    /// @param dataLength Data length.
    /// @param param Video frame parameters.
    /// @param referenceTimeMillisecond video frame reference time, UNIX timestamp, in milliseconds.
    /// @param streamID Stream ID.
    virtual void onRemoteVideoFrameEncodedData(const unsigned char * /*data*/,
                                               unsigned int /*dataLength*/,
                                               ZGFlutterVideoEncodedFrameParam /*param*/,
                                               unsigned long long /*referenceTimeMillisecond*/,
                                               const std::string & /*streamID*/) {}
};

namespace ZEGO::EXPRESS {
    class IZegoCustomVideoRenderHandler;
};
class ZegoCustomVideoRenderHandler;

class FLUTTER_PLUGIN_EXPORT ZegoCustomVideoRenderManager{
public:
    static std::shared_ptr<ZegoCustomVideoRenderManager> getInstance();

    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoRenderHandler> getHandler();
    
    /// Sets up the event callback handler for custom video rendering.
    ///
    /// Available since: 1.9.0
    /// Description: Custom video rendering, where the developer is responsible for rendering the video data onto the UI components. With the change callback set, a callback that throws video data to the developer will be triggered when the developer calls startPreview, startPublishingStream, and startPlayingStream.
    /// Use case: Use beauty features or apps that use a cross-platform UI framework (for example, Qt requires a complex hierarchical UI to achieve high-experience interaction) or game engine (e.g. Unity, Unreal Engine, Cocos)
    /// When to call: After [createEngine].
    /// Related callbacks: [onCapturedVideoFrameRawData] local preview video frame data callback, [onRemoteVideoFrameRawData] remote playing stream video frame data callback.
    /// Note: This function is only available in ZegoExpressVideo SDK!
    ///
    /// @param handler Custom video render handler
    void
    setCustomVideoRenderHandler(std::shared_ptr<IZegoFlutterCustomVideoRenderHandler> handler);

private:
    friend class ZegoCustomVideoRenderHandler;
    std::shared_ptr<IZegoFlutterCustomVideoRenderHandler> handler_ = nullptr;
    std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoRenderHandler> zegoHandler_ = nullptr;
};

#endif  // ZEGO_CUSTOM_VIDEO_RENDER_MANAGER_H_