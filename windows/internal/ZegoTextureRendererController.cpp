#include "ZegoTextureRendererController.h"

#include <flutter/standard_method_codec.h>

using namespace ZEGO::EXPRESS;

ZegoTextureRendererController::ZegoTextureRendererController(/* args */)
{
}

ZegoTextureRendererController::~ZegoTextureRendererController()
{
    capturedRenderers_.clear();
    remoteRenderers_.clear();
    renderers_.clear();
    isInit = false;
}

void ZegoTextureRendererController::init(flutter::BinaryMessenger *message)
{
    if (!isInit)
    {
        auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>
            (message, "plugins.zego.im/zego_texture_renderer_controller_event_handler", &flutter::StandardMethodCodec::GetInstance());
        eventChannel->SetStreamHandler(std::make_unique<ZegoTextureRendererControllerEventChannel>());

        isInit = true;
        ZegoCustomVideoRenderConfig config;
        config.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
        config.frameFormatSeries = ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
        ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);

        ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(ZegoTextureRendererController::getInstance());
    }
}

void ZegoTextureRendererController::uninit()
{
    capturedRenderers_.clear();
    remoteRenderers_.clear();
    renderers_.clear();
    isInit = false;
}

int64_t ZegoTextureRendererController::createTextureRenderer(flutter::TextureRegistrar* texture_registrar, uint32_t width, uint32_t height)
{
    auto textureRenderer = std::make_shared<ZegoTextureRenderer>(texture_registrar, width, height);

    renderers_.insert(std::pair<int64_t , std::shared_ptr<ZegoTextureRenderer> >(textureRenderer->getTextureID(), textureRenderer));

    return textureRenderer->getTextureID();
}

bool ZegoTextureRendererController::destroyTextureRenderer(int64_t textureID)
{
    auto renderer = renderers_.find(textureID);
    if (renderer != renderers_.end()) {
        renderer->second.reset();
        renderers_.erase(textureID);
        return true;
    }
    return false;
}

/// Called when dart invoke `startPreview`
bool ZegoTextureRendererController::addCapturedRenderer(int64_t textureID, ZEGO::EXPRESS::ZegoPublishChannel channel, ZEGO::EXPRESS::ZegoViewMode viewMode)
{
    auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    capturedRenderers_.insert(std::pair(channel, renderer->second));

    return true;
}
/// Called when dart invoke `stopPreview`
void ZegoTextureRendererController::removeCapturedRenderer(ZEGO::EXPRESS::ZegoPublishChannel channel)
{
    capturedRenderers_.erase(channel);
}
/// Called when dart invoke `startPlayingStream`
bool ZegoTextureRendererController::addRemoteRenderer(int64_t textureID, std::string streamID, ZEGO::EXPRESS::ZegoViewMode viewMode)
{
     auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    remoteRenderers_.insert(std::pair(streamID, renderer->second));

    return true;
}
/// Called when dart invoke `stopPlayingStream`
void ZegoTextureRendererController::removeRemoteRenderer(std::string streamID)
{
    remoteRenderers_.erase(streamID);
}

/// Called when dart invoke `mediaPlayer.setPlayerCanvas`
bool ZegoTextureRendererController::addMediaPlayerRenderer(int64_t textureID, ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer, ZEGO::EXPRESS::ZegoViewMode viewMode)
{
     auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    mediaPlayerRenderers_.insert(std::pair(mediaPlayer, renderer->second));

    return true;
}
/// Called when dart invoke `destroyMediaPlayer`
void ZegoTextureRendererController::removeMediaPlayerRenderer(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    mediaPlayer->setVideoHandler(nullptr, ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32);
    mediaPlayerRenderers_.erase(mediaPlayer);
}

/// For video preview/play
void ZegoTextureRendererController::startRendering()
{

}
/// For video preview/play
void ZegoTextureRendererController::stopRendering()
{

}

void ZegoTextureRendererController::setMediaPlayerVideoHandler(std::shared_ptr<ZEGO::EXPRESS::IZegoMediaPlayerVideoHandler> handler) 
{
    mediaPlayerHandler_ = handler;
}

void ZegoTextureRendererController::setCustomVideoRenderHandler(std::shared_ptr<ZEGO::EXPRESS::IZegoCustomVideoRenderHandler> handler) 
{
    videoRenderHandler_ = handler;
}

void ZegoTextureRendererController::onCapturedVideoFrameRawData(unsigned char ** data,
                                             unsigned int * dataLength,
                                             ZegoVideoFrameParam param,
                                             ZegoVideoFlipMode flipMode,
                                             ZegoPublishChannel channel)
{
    auto renderer = capturedRenderers_.find(channel);
    if (renderer != capturedRenderers_.end()) {
        bool isMirror = flipMode == ZEGO_VIDEO_FLIP_MODE_X;
        if (eventSink_) {
            auto size = renderer->second->getSize();
            
            if (size.first != param.width || size.second != param.height || renderer->second->getUseMirrorEffect() != isMirror) {
                flutter::EncodableMap map;
                map[flutter::EncodableValue("type")] =  flutter::EncodableValue("update");
                map[flutter::EncodableValue("textureID")] =  flutter::EncodableValue(renderer->second->getTextureID());
                map[flutter::EncodableValue("width")] =  flutter::EncodableValue(param.width);
                map[flutter::EncodableValue("height")] =  flutter::EncodableValue(param.height);
                map[flutter::EncodableValue("isMirror")] =  flutter::EncodableValue(isMirror ? 1 : 0);
                eventSink_->Success(map);
            }
        }
        renderer->second->setUseMirrorEffect(isMirror);
        renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
    }

    if (videoRenderHandler_) {
        videoRenderHandler_->onCapturedVideoFrameRawData(data, dataLength, param, flipMode, channel);
    }
}

void ZegoTextureRendererController::onRemoteVideoFrameRawData(unsigned char ** data, unsigned int * dataLength,
                                           ZegoVideoFrameParam param,
                                           const std::string & streamID)
{
    auto renderer = remoteRenderers_.find(streamID);
    if (renderer != remoteRenderers_.end()) {
        if (eventSink_) {
            auto size = renderer->second->getSize();
            if (size.first != param.width || size.second != param.height) {
                flutter::EncodableMap map;
                map[flutter::EncodableValue("type")] =  flutter::EncodableValue("update");
                map[flutter::EncodableValue("textureID")] =  flutter::EncodableValue(renderer->second->getTextureID());
                map[flutter::EncodableValue("width")] =  flutter::EncodableValue(param.width);
                map[flutter::EncodableValue("height")] =  flutter::EncodableValue(param.height);
                eventSink_->Success(map);
            }
        }
        renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
    }

    if (videoRenderHandler_) {
        videoRenderHandler_->onRemoteVideoFrameRawData(data, dataLength, param, streamID);
    }
}

void ZegoTextureRendererController::onRemoteVideoFrameEncodedData(const unsigned char * data,
                                               unsigned int dataLength,
                                               ZEGO::EXPRESS::ZegoVideoEncodedFrameParam param,
                                               unsigned long long referenceTimeMillisecond,
                                               const std::string & streamID)
{
    if (videoRenderHandler_) {
        videoRenderHandler_->onRemoteVideoFrameEncodedData(data, dataLength, param, referenceTimeMillisecond, streamID);
    }
}

void ZegoTextureRendererController::onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char ** data,
                              unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param)
{
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        if (eventSink_) {
            auto size = renderer->second->getSize();
            if (size.first != param.width || size.second != param.height) {
                flutter::EncodableMap map;
                map[flutter::EncodableValue("type")] =  flutter::EncodableValue("update");
                map[flutter::EncodableValue("textureID")] =  flutter::EncodableValue(renderer->second->getTextureID());
                map[flutter::EncodableValue("width")] =  flutter::EncodableValue(param.width);
                map[flutter::EncodableValue("height")] =  flutter::EncodableValue(param.height);
                eventSink_->Success(map);
            }
        }
        renderer->second->updateSrcFrameBuffer((uint8_t *)data[0], dataLength[0], param);
    }
    if (mediaPlayerHandler_) {
        mediaPlayerHandler_->onVideoFrame(mediaPlayer, data, dataLength, param);
    }
}

void ZegoTextureRendererController::onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char ** data,
                              unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param,
                              const char * extraInfo) {
    if (mediaPlayerHandler_) {
        mediaPlayerHandler_->onVideoFrame(mediaPlayer, data, dataLength, param, extraInfo);
    }
}

/// Called when dart invoke `mediaPlayerTakeSnapshot`
std::pair<int32_t, int32_t> ZegoTextureRendererController::getMediaPlayerSize(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        return renderer->second->getSize();
    }
    return std::pair(0, 0);
}

const std::vector<uint8_t> *ZegoTextureRendererController::getMediaPlayerFrame(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        return renderer->second->getFrame();
    }
    return nullptr;
}