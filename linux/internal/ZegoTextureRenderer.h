#pragma once

#include <flutter_linux/flutter_linux.h>

#include <memory>
#include <mutex>
#include <string>

#include <ZegoExpressSDK.h>

#include "ZegoDataUtils.h"
#include "ZegoTexture.h"

// Describes flutter desktop pixelbuffers pixel data order.
struct FlutterDesktopPixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

// Describes MFVideoFormat_RGB32 data order.
struct VideoFormatBGRAPixel {
    uint8_t b = 0;
    uint8_t g = 0;
    uint8_t r = 0;
    uint8_t a = 0;
};

struct VideoFormatRGBAPixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

struct VideoFormatARGBPixel {
    uint8_t a = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

struct VideoFormatABGRPixel {
    uint8_t a = 0;
    uint8_t b = 0;
    uint8_t g = 0;
    uint8_t r = 0;
};

// Handles the registration of Flutter textures, pixel buffers, and the
// conversion of texture formats.
class ZegoTextureRenderer {
  public:
    ZegoTextureRenderer();

    virtual ~ZegoTextureRenderer();

    //must call after construct ZegoTextureRenderer
    void CreateTexture(ZFTextureRegistrar *texture_registrar, uint32_t width, uint32_t height);

    void DestroyTexture();

    // Prevent copying.
    ZegoTextureRenderer(ZegoTextureRenderer const &) = delete;
    ZegoTextureRenderer &operator=(ZegoTextureRenderer const &) = delete;

    // Updates source data buffer with given data.
    bool updateSrcFrameBuffer(uint8_t *data, uint32_t data_length,
                              ZEGO::EXPRESS::ZegoVideoFrameParam frameParam);

    // Registers texture and updates given texture_id pointer value.
    int64_t getTextureID() { return textureID_; }

    // Updates current preview texture size.
    void updateRenderSize(uint32_t width, uint32_t height) {
        width_ = width;
        height_ = height;
    }

    inline std::pair<int32_t, int32_t> getSize() {
        return std::pair<int32_t, int32_t>(width_, height_);
    }

    inline const std::vector<uint8_t> *getFrame() { return &srcBuffer_; }

    inline uint32_t getFrameStride() { return srcStride_; }

    void setBackgroundColor(int colode) {}

    void setViewMode(ZEGO::EXPRESS::ZegoViewMode mode) { viewMode_ = mode; }

    inline bool getUseMirrorEffect() { return isUseMirror_; }

    void setUseMirrorEffect(bool mirror) { isUseMirror_ = mirror; }

  private:
    // Checks if texture registrar, texture id and texture are available.
    bool TextureRegistered();

    template <typename T> void srcFrameFormatToFlutterFormat();

    ZFTextureRegistrar *textureRegistrar_ = nullptr;

    std::vector<uint8_t> srcBuffer_;
    std::vector<uint8_t> destBuffer_;

    bool isUseMirror_ = true;
    int64_t textureID_ = -1;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    ZEGO::EXPRESS::ZegoViewMode viewMode_ = ZEGO::EXPRESS::ZegoViewMode::ZEGO_VIEW_MODE_ASPECT_FIT;

    uint32_t srcStride_ = 0;

    ZegoTexture *texture_;
    VideoOutput *video_output_;

    std::mutex bufferMutex_;
};
