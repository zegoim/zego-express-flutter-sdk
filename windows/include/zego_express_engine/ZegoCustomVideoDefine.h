#ifndef ZEGO_CUSTOM_VIDEO_DEFINE_H_
#define ZEGO_CUSTOM_VIDEO_DEFINE_H_

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

/// Publish channel.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterPublishChannel {
    /// The main (default/first) publish channel.
    ZG_FLUTTER_PUBLISH_CHANNEL_MAIN = 0,

    /// The auxiliary (second) publish channel
    ZG_FLUTTER_PUBLISH_CHANNEL_AUX = 1,

    /// The third publish channel
    ZG_FLUTTER_PUBLISH_CHANNEL_THIRD = 2,

    /// The fourth publish channel
    ZG_FLUTTER_PUBLISH_CHANNEL_FOURTH = 3

};

/// Externally encoded data traffic control information.
struct FLUTTER_PLUGIN_EXPORT ZGFlutterTrafficControlInfo {
    /// Video resolution width to be adjusted
    int width;

    /// Video resolution height to be adjusted
    int height;

    /// Video FPS to be adjusted
    int fps;

    /// Video bitrate in kbps to be adjusted
    int bitrate;
};

/// Video frame format.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterVideoFrameFormat {
    /// Unknown format, will take platform default
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_UNKNOWN = 0,

    /// I420 (YUV420Planar) format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_I420 = 1,

    /// NV12 (YUV420SemiPlanar) format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_NV12 = 2,

    /// NV21 (YUV420SemiPlanar) format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_NV21 = 3,

    /// BGRA32 format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_BGRA32 = 4,

    /// RGBA32 format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_RGBA32 = 5,

    /// ARGB32 format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_ARGB32 = 6,

    /// ABGR32 format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_ABGR32 = 7,

    /// I422 (YUV422Planar) format
    ZG_FLUTTER_VIDEO_FRAME_FORMAT_I422 = 8

};

/// Object for video frame fieldeter.
///
/// Including video frame format, width and height, etc.
struct FLUTTER_PLUGIN_EXPORT ZGFlutterVideoFrameParam {
    /// Video frame format
    ZGFlutterVideoFrameFormat format;

    /// Number of bytes per line (for example: BGRA only needs to consider strides [0], I420 needs to consider strides [0,1,2])
    int strides[4];

    /// Video frame width. When use custom video capture, the video data meeting the 32-bit alignment can obtain the maximum performance. Taking BGRA as an example, width * 4 is expected to be multiple of 32.
    int width;

    /// Video frame height
    int height;

    /// The rotation direction of the video frame, the SDK rotates clockwise
    int rotation;
};

/// Video encoded frame format.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterVideoEncodedFrameFormat {
    /// AVC AVCC format
    ZG_FLUTTER_VIDEO_ENCODED_FRAME_FORMAT_AVCC = 0,

    /// AVC Annex-B format
    ZG_FLUTTER_VIDEO_ENCODED_FRAME_FORMAT_ANNEXB = 1,

    /// VP8 format
    ZG_FLUTTER_VIDEO_ENCODED_FRAME_FORMAT_VP8 = 2,

    /// HEVC AVCC format
    ZG_FLUTTER_VIDEO_ENCODED_FRAME_FORMAT_HEVC_AVCC = 3,

    /// HEVC Annex-B format
    ZG_FLUTTER_VIDEO_ENCODED_FRAME_FORMAT_HEVC_ANNEXB = 4

};

/// Object for video encoded frame fieldeter.
///
/// Including video encoded frame format, width and height, etc.
struct FLUTTER_PLUGIN_EXPORT ZGFlutterVideoEncodedFrameParam {
    /// Video encoded frame format
    ZGFlutterVideoEncodedFrameFormat format;

    /// Whether it is a keyframe
    bool isKeyFrame;

    /// Video frame counterclockwise rotation angle
    int rotation;

    /// Video frame width
    int width;

    /// Video frame height
    int height;

    /// SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
    const unsigned char *SEIData;

    /// Length of the SEI data (Optional, if you don't need to send SEI, set it to 0. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
    unsigned int SEIDataLength;
};

/// Video rendering fill mode.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterViewMode {
    /// The proportional scaling up, there may be black borders
    ZG_FLUTTER_VIEW_MODE_ASPECT_FIT = 0,

    /// The proportional zoom fills the entire View and may be partially cut
    ZG_FLUTTER_VIEW_MODE_ASPECT_FILL = 1,

    /// Fill the entire view, the image may be stretched
    ZG_FLUTTER_VIEW_MODE_SCALE_TO_FILL = 2

};

/// Remote device status.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterRemoteDeviceState {
    /// Device on
    ZG_FLUTTER_REMOTE_DEVICE_STATE_OPEN = 0,

    /// General device error
    ZG_FLUTTER_REMOTE_DEVICE_STATE_GENERIC_ERROR = 1,

    /// Invalid device ID
    ZG_FLUTTER_REMOTE_DEVICE_STATE_INVALID_ID = 2,

    /// No permission
    ZG_FLUTTER_REMOTE_DEVICE_STATE_NO_AUTHORIZATION = 3,

    /// Captured frame rate is 0
    ZG_FLUTTER_REMOTE_DEVICE_STATE_ZERO_FPS = 4,

    /// The device is occupied
    ZG_FLUTTER_REMOTE_DEVICE_STATE_IN_USE_BY_OTHER = 5,

    /// The device is not plugged in or unplugged
    ZG_FLUTTER_REMOTE_DEVICE_STATE_UNPLUGGED = 6,

    /// The system needs to be restarted
    ZG_FLUTTER_REMOTE_DEVICE_STATE_REBOOT_REQUIRED = 7,

    /// System media services stop, such as under the iOS platform, when the system detects that the current pressure is huge (such as playing a lot of animation), it is possible to disable all media related services.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_SYSTEM_MEDIA_SERVICES_LOST = 8,

    /// The remote user calls [enableCamera] or [muteMicrophone] to disable the camera or microphone.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_DISABLE = 9,

    /// The remote user actively calls [mutePublishStreamAudio] or [mutePublishStreamVideo] to stop publish the audio or video stream.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_MUTE = 10,

    /// The device is interrupted, such as a phone call interruption, etc.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_INTERRUPTION = 11,

    /// There are multiple apps at the same time in the foreground, such as the iPad app split screen, the system will prohibit all apps from using the camera.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_IN_BACKGROUND = 12,

    /// CDN server actively disconnected
    ZG_FLUTTER_REMOTE_DEVICE_STATE_MULTI_FOREGROUND_APP = 13,

    /// The system is under high load pressure and may cause abnormal equipment.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_BY_SYSTEM_PRESSURE = 14,

    /// The remote device is not supported to publish the device state.
    ZG_FLUTTER_REMOTE_DEVICE_STATE_NOT_SUPPORT = 15

};

/// Coordinates used by the ROI function.
struct FLUTTER_PLUGIN_EXPORT ZGFlutterRoiRect {
    /// The horizontal offset from the top-left corner
    int x;

    /// The vertical offset from the top-left corner
    int y;

    /// The width of the rectangle
    int width;

    /// The height of the rectangle
    int height;

    /// ROI strength, currently supported value range is [0, 4], 0 is no effect, 4 is the strongest.
    int strength;
};

/// Video frame flip mode.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterVideoFlipMode {
    /// No flip
    ZG_FLUTTER_VIDEO_FLIP_MODE_NONE = 0,

    /// X-axis flip
    ZG_FLUTTER_VIDEO_FLIP_MODE_X = 1,

    /// Y-axis flip
    ZG_FLUTTER_VIDEO_FLIP_MODE_Y = 2,

    /// X-Y-axis flip
    ZG_FLUTTER_VIDEO_FLIP_MODE_XY = 3

};

/// audio sample rate.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterAudioSampleRate {
    /// Unknown
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_UNKNOWN = 0,

    /// 8K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_8K = 8000,

    /// 16K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_16K = 16000,

    /// 22.05K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_22K = 22050,

    /// 24K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_24K = 24000,

    /// 32K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_32K = 32000,

    /// 44.1K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_44K = 44100,

    /// 48K
    ZG_FLUTTER_AUDIO_SAMPLE_RATE_48K = 48000

};

/// Audio channel type.
enum FLUTTER_PLUGIN_EXPORT ZGFlutterAudioChannel {
    /// Unknown
    ZG_FLUTTER_AUDIO_CHANNEL_UNKNOWN = 0,

    /// Mono
    ZG_FLUTTER_AUDIO_CHANNEL_MONO = 1,

    /// Stereo
    ZG_FLUTTER_AUDIO_CHANNEL_STEREO = 2

};

/// Parameter object for audio frame.
///
/// Including the sampling rate and channel of the audio frame
struct FLUTTER_PLUGIN_EXPORT ZGFlutterAudioFrameParam {
    /// Sampling Rate
    ZGFlutterAudioSampleRate sampleRate;

    /// Audio channel, default is Mono
    ZGFlutterAudioChannel channel;
};

/// Usage of Custom video processed data.
enum ZGFlutterProcessedDataUsageType {
    /// Preview.
    ZG_FLUTTER_PROCESSED_DATA_USAGE_TYPE_PREVIEW = 0,

    /// Publish.
    ZG_FLUTTER_PROCESSED_DATA_USAGE_TYPE_PUBLISH = 1,

    /// Both preview and publish.
    ZG_FLUTTER_PROCESSED_DATA_USAGE_TYPE_BOTH = 2

};

#endif  // ZEGO_CUSTOM_VIDEO_DEFINE_H_