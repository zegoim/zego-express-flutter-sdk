#include "ZegoTextureRendererController.h"

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

void ZegoTextureRendererController::init()
{
    if (!isInit)
    {
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

    mediaPlayer->setVideoHandler(ZegoTextureRendererController::getInstance(), ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_BGRA32);

    renderer->second->setViewMode(viewMode);

    mediaPlayerRenderers_.insert(std::pair(mediaPlayer, renderer->second));

    return true;
}
/// Called when dart invoke `destroyMediaPlayer`
void ZegoTextureRendererController::removeMediaPlayerRenderer(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    mediaPlayer->setVideoHandler(nullptr, ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_BGRA32);
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

void ZegoTextureRendererController::onCapturedVideoFrameRawData(unsigned char ** data,
                                             unsigned int * dataLength,
                                             ZegoVideoFrameParam param,
                                             ZegoVideoFlipMode flipMode,
                                             ZegoPublishChannel channel)
{
    auto renderer = capturedRenderers_.find(channel);
    if (renderer != capturedRenderers_.end()) {
        renderer->second->setUseMirrorEffect(flipMode == ZEGO_VIDEO_FLIP_MODE_X);
        renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
    }
}

void ZegoTextureRendererController::onRemoteVideoFrameRawData(unsigned char ** data, unsigned int * dataLength,
                                           ZegoVideoFrameParam param,
                                           const std::string & streamID)
{
    auto renderer = remoteRenderers_.find(streamID);
    if (renderer != remoteRenderers_.end()) {
        renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
    }
}

void ZegoTextureRendererController::onVideoFrame(ZEGO::EXPRESS::IZegoMediaPlayer * mediaPlayer, const unsigned char ** data,
                              unsigned int * dataLength, ZEGO::EXPRESS::ZegoVideoFrameParam param)
{
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        renderer->second->updateSrcFrameBuffer((uint8_t *)data[0], dataLength[0], param);
    }
}