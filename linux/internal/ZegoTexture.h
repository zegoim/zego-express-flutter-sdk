#pragma once

#include <flutter_linux/flutter_linux.h>

#define ZEGO_TEXTURE_TYPE (zego_texture_get_type())

G_DECLARE_FINAL_TYPE(ZegoTexture, zego_texture, ZEGO_TEXTURE, ZEGO_TEXTURE, FlPixelBufferTexture)

#define ZEGO_TEXTURE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), zego_texture_get_type(), ZegoTexture))

struct VideoOutput {
    uint8_t *buffer = nullptr;
    int32_t video_width = 0;
    int32_t video_height = 0;
};

ZegoTexture *zego_texture_new(VideoOutput* video_output);

gboolean zego_texture_copy_pixels(FlPixelBufferTexture *texture, const uint8_t **buffer,
                                  uint32_t *width, uint32_t *height, GError **error);
