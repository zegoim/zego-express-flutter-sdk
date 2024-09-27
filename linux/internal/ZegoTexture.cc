#include "ZegoTexture.h"
#include "ZegoLog.h"

struct _ZegoTexture {
    FlPixelBufferTexture parent_instance;
    VideoOutput *video_output;
};

G_DEFINE_TYPE(ZegoTexture, zego_texture, fl_pixel_buffer_texture_get_type())

static void zego_texture_init(ZegoTexture *self) {}

static void zego_texture_dispose(GObject *object) {
    G_OBJECT_CLASS(zego_texture_parent_class)->dispose(object);
}

static void zego_texture_class_init(ZegoTextureClass *klass) {
    FL_PIXEL_BUFFER_TEXTURE_CLASS(klass)->copy_pixels = zego_texture_copy_pixels;
    G_OBJECT_CLASS(klass)->dispose = zego_texture_dispose;
}

ZegoTexture *zego_texture_new(VideoOutput* video_output) {
    ZegoTexture* self = ZEGO_TEXTURE(g_object_new(zego_texture_get_type(), NULL));
    self->video_output = video_output;
    return self;
}

gboolean zego_texture_copy_pixels(FlPixelBufferTexture *texture, const uint8_t **buffer,
                                  uint32_t *width, uint32_t *height, GError **error) {

    ZegoTexture *self = ZEGO_TEXTURE(texture);
    VideoOutput *video_output = self->video_output;

    *buffer = video_output->buffer;
    *width = video_output->video_width;
    *height = video_output->video_height;

    return TRUE;
}