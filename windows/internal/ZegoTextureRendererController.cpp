#include "ZegoTextureRendererController.h"
#include "ZegoExpressEngineEventHandler.h"
#include "../ZegoLog.h"
#include <flutter/standard_method_codec.h>

using namespace ZEGO::EXPRESS;

ZegoTextureRendererController::ZegoTextureRendererController(/* args */)
{
}

ZegoTextureRendererController::~ZegoTextureRendererController()
{
    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        capturedRenderers_.clear();
        remoteRenderers_.clear();
        mediaPlayerRenderers_.clear();
        alphaRenders_.clear();
        videoSourceChannels_.clear();
        capturedRenderFirstFrameMap_.clear();
        meidaPlayerRenderFirstFrameMap_.clear();
    }
    
    renderers_.clear();
    isInit = false;
}

void ZegoTextureRendererController::init(flutter::BinaryMessenger *message)
{
    if (!isInit)
    {
        eventChannel_ = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>
            (message, "plugins.zego.im/zego_texture_renderer_controller_event_handler", &flutter::StandardMethodCodec::GetInstance());
        eventChannel_->SetStreamHandler(std::make_unique<ZegoTextureRendererControllerEventChannel>());

        isInit = true;
        ZegoCustomVideoRenderConfig config{};
        config.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
        config.frameFormatSeries = ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
        ZegoExpressSDK::getEngine()->enableCustomVideoRender(true, &config);

        ZegoExpressSDK::getEngine()->setCustomVideoRenderHandler(ZegoTextureRendererController::getInstance());
    }
}

void ZegoTextureRendererController::uninit()
{
    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        capturedRenderers_.clear();
        remoteRenderers_.clear();
        mediaPlayerRenderers_.clear();
        alphaRenders_.clear();
        videoSourceChannels_.clear();
        capturedRenderFirstFrameMap_.clear();
        meidaPlayerRenderFirstFrameMap_.clear();
    }
    
    renderers_.clear();
    isInit = false;
}

int64_t ZegoTextureRendererController::createTextureRenderer(flutter::TextureRegistrar* texture_registrar, uint32_t width, uint32_t height)
{
    auto textureRenderer = std::make_shared<ZegoTextureRenderer>(texture_registrar, width, height);

    ZF::logInfo("[createTextureRenderer] textureID: %d, width: %d, height: %d", textureRenderer->getTextureID(), width, height);

    renderers_.insert(std::pair<int64_t , std::shared_ptr<ZegoTextureRenderer> >(textureRenderer->getTextureID(), textureRenderer));

    return textureRenderer->getTextureID();
}

bool ZegoTextureRendererController::destroyTextureRenderer(int64_t textureID)
{
    ZF::logInfo("[destroyTextureRenderer] textureID: %d", textureID);

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
    ZF::logInfo("[addCapturedRenderer] textureID: %d, channel: %d, viewMode: %d", textureID, channel, viewMode);

    auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        capturedRenderers_.insert(std::pair(channel, renderer->second));
    }

    return true;
}
/// Called when dart invoke `stopPreview`
void ZegoTextureRendererController::removeCapturedRenderer(ZEGO::EXPRESS::ZegoPublishChannel channel)
{
    ZF::logInfo("[removeCapturedRenderer] channel: %d", channel);

    std::lock_guard<std::mutex> lock(rendersMutex_);
    capturedRenderers_.erase(channel);
    capturedRenderFirstFrameMap_.erase(channel);
}
/// Called when dart invoke `startPlayingStream`
bool ZegoTextureRendererController::addRemoteRenderer(int64_t textureID, std::string streamID, ZEGO::EXPRESS::ZegoViewMode viewMode)
{
    ZF::logInfo("[addRemoteRenderer] textureID: %d, streamID: %s, viewMode: %d", textureID, streamID.c_str(), viewMode);

    auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        remoteRenderers_.insert(std::pair(streamID, renderer->second));
    }

    return true;
}
/// Called when dart invoke `stopPlayingStream`
void ZegoTextureRendererController::removeRemoteRenderer(std::string streamID)
{
    ZF::logInfo("[removeRemoteRenderer] streamID: %s", streamID.c_str());

    std::lock_guard<std::mutex> lock(rendersMutex_);
    remoteRenderers_.erase(streamID);
}

/// Called when dart invoke `mediaPlayer.setPlayerCanvas`
bool ZegoTextureRendererController::addMediaPlayerRenderer(int64_t textureID, ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer, ZEGO::EXPRESS::ZegoViewMode viewMode)
{
    ZF::logInfo("[addMediaPlayerRenderer] textureID: %d, index: %d, viewMode: %d", textureID, mediaPlayer->getIndex(), viewMode);

    auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return false;
    }

    renderer->second->setViewMode(viewMode);

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        mediaPlayerRenderers_.insert(std::pair(mediaPlayer, renderer->second));
    }

    return true;
}
/// Called when dart invoke `destroyMediaPlayer`
void ZegoTextureRendererController::removeMediaPlayerRenderer(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    ZF::logInfo("[removeMediaPlayerRenderer] index: %d", mediaPlayer->getIndex());

    mediaPlayer->setVideoHandler(nullptr, ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32);
    
    std::lock_guard<std::mutex> lock(rendersMutex_);
    mediaPlayerRenderers_.erase(mediaPlayer);
    meidaPlayerRenderFirstFrameMap_.erase(mediaPlayer);
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

void ZegoTextureRendererController::enableTextureAlpha(bool enable, int64_t textureID) {
    ZF::logInfo("[enableTextureAlpha] textureID: %d, enable: %d", textureID, enable);

    auto renderer = renderers_.find(textureID);

    if (renderer == renderers_.end()) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        alphaRenders_.insert(std::pair<int64_t , bool>(textureID, enable));
    }
}

void ZegoTextureRendererController::setVideoSourceChannel(ZEGO::EXPRESS::ZegoPublishChannel channel, ZEGO::EXPRESS::ZegoVideoSourceType sourceType)
{
    ZF::logInfo("[setVideoSourceChannel] channel: %d, sourceType: %d", channel, sourceType);

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        videoSourceChannels_.insert(std::pair<ZEGO::EXPRESS::ZegoPublishChannel , ZEGO::EXPRESS::ZegoVideoSourceType>(channel, sourceType));
    }
}

void ZegoTextureRendererController::resetMediaPlayerRenderFirstFrame(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer) {
    ZF::logInfo("[resetMediaPlayerRenderFirstFrame] index: %d", mediaPlayer->getIndex());

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        meidaPlayerRenderFirstFrameMap_.erase(mediaPlayer);
    }
}

void ZegoTextureRendererController::resetAllRenderFirstFrame() {
    ZF::logInfo("[resetAllRenderFirstFrame]");

    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
        capturedRenderFirstFrameMap_.clear();
        meidaPlayerRenderFirstFrameMap_.clear();
    }
}


void ZegoTextureRendererController::sendScreenCapturedVideoFrameRawData(unsigned char ** data,
                                        unsigned int * dataLength,
                                        ZEGO::EXPRESS::ZegoVideoFrameParam param,
                                        ZEGO::EXPRESS::ZegoVideoFlipMode flipMode)
{
    int key = -1;
    for (auto const& pair : videoSourceChannels_) {
        if (pair.second == ZEGO::EXPRESS::ZegoVideoSourceType::ZEGO_VIDEO_SOURCE_TYPE_SCREEN_CAPTURE) {
            key = pair.first;
            break;
        }
    }

    if (key != -1) {
        onCapturedVideoFrameRawData(data, dataLength, param, flipMode, (ZEGO::EXPRESS::ZegoPublishChannel)key);
    }
}

void ZegoTextureRendererController::onCapturedVideoFrameRawData(unsigned char ** data,
                                             unsigned int * dataLength,
                                             ZegoVideoFrameParam param,
                                             ZegoVideoFlipMode flipMode,
                                             ZegoPublishChannel channel)
{
    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
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

            /// alpha premultiply
            auto alphaBlend = alphaRenders_.find(renderer->second->getTextureID());
            if (alphaBlend != alphaRenders_.end() && alphaBlend->second) {
                UINT cbStride = param.width * 4;
                UINT cbBufferSize = cbStride * param.height;

                for (UINT y = 0; y < param.height; y++) {
                    BYTE* pRow = *data + y * cbStride;
                    for (UINT x = 0; x < param.width; x++) {
                        BYTE* pPixel = pRow + x * 4;
                        BYTE alpha = pPixel[3];
                        pPixel[0] = static_cast<BYTE>((pPixel[0] * alpha + 127) / 255);
                        pPixel[1] = static_cast<BYTE>((pPixel[1] * alpha + 127) / 255);
                        pPixel[2] = static_cast<BYTE>((pPixel[2] * alpha + 127) / 255);
                    }
                }
            }

            renderer->second->setUseMirrorEffect(isMirror);

            auto firstFrameRender = capturedRenderFirstFrameMap_.find(channel);
            if (firstFrameRender == capturedRenderFirstFrameMap_.end() || !firstFrameRender->second) {
                ZegoExpressEngineEventHandler::getInstance()->onPublisherRenderVideoFirstFrame(
                    channel);
                capturedRenderFirstFrameMap_.insert(std::pair<ZEGO::EXPRESS::ZegoPublishChannel, bool>(channel, true));
            }
            renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
        }
    }

    if (videoRenderHandler_) {
        videoRenderHandler_->onCapturedVideoFrameRawData(data, dataLength, param, flipMode, channel);
    }
}

void ZegoTextureRendererController::onRemoteVideoFrameRawData(unsigned char ** data, unsigned int * dataLength,
                                           ZegoVideoFrameParam param,
                                           const std::string & streamID)
{
    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
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

            /// alpha premultiply
            auto alphaBlend = alphaRenders_.find(renderer->second->getTextureID());
            if (alphaBlend != alphaRenders_.end() && alphaBlend->second) {
                UINT cbStride = param.width * 4;
                UINT cbBufferSize = cbStride * param.height;

                for (UINT y = 0; y < param.height; y++) {
                    BYTE* pRow = *data + y * cbStride;
                    for (UINT x = 0; x < param.width; x++) {
                        BYTE* pPixel = pRow + x * 4;
                        BYTE alpha = pPixel[3];
                        pPixel[0] = static_cast<BYTE>((pPixel[0] * alpha + 127) / 255);
                        pPixel[1] = static_cast<BYTE>((pPixel[1] * alpha + 127) / 255);
                        pPixel[2] = static_cast<BYTE>((pPixel[2] * alpha + 127) / 255);
                    }
                }
            }

            renderer->second->updateSrcFrameBuffer(data[0], dataLength[0], param);
        }
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
    {
        std::lock_guard<std::mutex> lock(rendersMutex_);
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

            auto firstFrameRender = meidaPlayerRenderFirstFrameMap_.find(mediaPlayer);
            if (firstFrameRender == meidaPlayerRenderFirstFrameMap_.end() ||
                !firstFrameRender->second) {
                ZegoExpressEngineEventHandler::getInstance()->onMediaPlayerFirstFrameEvent(
                    mediaPlayer, EXPRESS::ZegoMediaPlayerFirstFrameEvent::ZEGO_MEDIA_PLAYER_FIRST_FRAME_EVENT_VIDEO_RENDERED);
                meidaPlayerRenderFirstFrameMap_.insert(
                    std::pair<ZEGO::EXPRESS::IZegoMediaPlayer *, bool>(mediaPlayer, true));
            }
            renderer->second->updateSrcFrameBuffer((uint8_t *)data[0], dataLength[0], param);
        }
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
    std::lock_guard<std::mutex> lock(rendersMutex_);
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        return renderer->second->getSize();
    }
    return std::pair(0, 0);
}

const std::vector<uint8_t> *ZegoTextureRendererController::getMediaPlayerFrame(ZEGO::EXPRESS::IZegoMediaPlayer *mediaPlayer)
{
    std::lock_guard<std::mutex> lock(rendersMutex_);
    auto renderer = mediaPlayerRenderers_.find(mediaPlayer);
    if (renderer != mediaPlayerRenderers_.end()) {
        return renderer->second->getFrame();
    }
    return nullptr;
}