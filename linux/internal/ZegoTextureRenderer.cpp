#include "ZegoTextureRenderer.h"

#include <cassert>
#include <iostream>

ZegoTextureRenderer::ZegoTextureRenderer() {}

ZegoTextureRenderer::~ZegoTextureRenderer() {}

void ZegoTextureRenderer::CreateTexture(ZFTextureRegistrar *texture_registrar, uint32_t width,
                                        uint32_t height) {

    textureRegistrar_ = texture_registrar;
    width_ = width;
    height_ = height;

    video_output_ = new VideoOutput();
    texture_ = zego_texture_new(video_output_);
    FL_PIXEL_BUFFER_TEXTURE_GET_CLASS(texture_)->copy_pixels = zego_texture_copy_pixels;
    fl_texture_registrar_register_texture(texture_registrar, FL_TEXTURE(texture_));
    textureID_ = reinterpret_cast<int64_t>(FL_TEXTURE(texture_));
}

void ZegoTextureRenderer::DestroyTexture() {
    const std::lock_guard<std::mutex> lock(bufferMutex_);
    delete video_output_;
    texture_ = nullptr;
    textureRegistrar_ = nullptr;
}

bool ZegoTextureRenderer::updateSrcFrameBuffer(uint8_t *data, uint32_t data_length,
                                               ZEGO::EXPRESS::ZegoVideoFrameParam frameParam) {
    // Scoped lock guard.
    {
        const std::lock_guard<std::mutex> lock(bufferMutex_);
        if (!TextureRegistered()) {
            return false;
        }

        updateRenderSize(frameParam.width, frameParam.height);
        srcStride_ = frameParam.strides[0];

        if (srcBuffer_.size() != data_length) {
            // Update source buffer size.
            srcBuffer_.resize(data_length);
        }

        if (destBuffer_.size() != srcBuffer_.size()) {
            destBuffer_.resize(srcBuffer_.size());
        }

        std::copy(data, data + data_length, srcBuffer_.data());

        switch (frameParam.format) {
        case ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32:
            video_output_->buffer = srcBuffer_.data();
            break;
        case ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_BGRA32:
            srcFrameFormatToFlutterFormat<VideoFormatBGRAPixel>();
            video_output_->buffer = destBuffer_.data();
            break;
        case ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_ARGB32:
            srcFrameFormatToFlutterFormat<VideoFormatARGBPixel>();
            video_output_->buffer = destBuffer_.data();
            break;
        case ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_ABGR32:
            srcFrameFormatToFlutterFormat<VideoFormatABGRPixel>();
            video_output_->buffer = destBuffer_.data();
            break;
        default:
            video_output_->buffer = srcBuffer_.data();
            break;
        }

        video_output_->video_width = frameParam.width;
        video_output_->video_height = frameParam.height;

        fl_texture_registrar_mark_texture_frame_available(textureRegistrar_, FL_TEXTURE(texture_));
    }

    return true;
};

bool ZegoTextureRenderer::TextureRegistered() {
    return textureRegistrar_ && texture_ && textureID_ > -1;
}

template <typename T> void ZegoTextureRenderer::srcFrameFormatToFlutterFormat() {

    T *src = reinterpret_cast<T *>(srcBuffer_.data());
    FlutterDesktopPixel *dst = reinterpret_cast<FlutterDesktopPixel *>(destBuffer_.data());

    for (uint32_t y = 0; y < height_; y++) {
        for (uint32_t x = 0; x < width_; x++) {
            uint32_t sp = (y * width_) + x;
            if (isUseMirror_) {
                // Software mirror mode.
                // IMFCapturePreviewSink also has the SetMirrorState setting,
                // but if enabled, samples will not be processed.

                // Calculates mirrored pixel position.
                uint32_t tp = (y * width_) + ((width_ - 1) - x);
                dst[tp].r = src[sp].r;
                dst[tp].g = src[sp].g;
                dst[tp].b = src[sp].b;
                dst[tp].a = 255;
            } else {
                dst[sp].r = src[sp].r;
                dst[sp].g = src[sp].g;
                dst[sp].b = src[sp].b;
                dst[sp].a = 255;
            }
        }
    }
}