//
//  ZegoCustomVideoDefine.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoCustomVideoDefine_h
#define ZegoCustomVideoDefine_h

/// Publish channel.
typedef NS_ENUM(NSUInteger, ZGFlutterPublishChannel) {
    /// The main (default/first) publish channel.
    ZGFlutterPublishChannelMain = 0,
    /// The auxiliary (second) publish channel
    ZGFlutterPublishChannelAux = 1,
    /// The third publish channel
    ZGFlutterPublishChannelThird = 2,
    /// The fourth publish channel
    ZGFlutterPublishChannelFourth = 3
};

/// Video frame flip mode.
typedef NS_ENUM(NSUInteger, ZGFlutterVideoFlipMode) {
    /// No flip
    ZGFlutterVideoFlipModeNone = 0,
    /// X-axis flip
    ZGFlutterVideoFlipModeX = 1,
    /// Y-axis flip
    ZGFlutterVideoFlipModeY = 2,
    /// X-Y-axis flip
    ZGFlutterVideoFlipModeXY = 3
};

/// Video frame format.
typedef NS_ENUM(NSUInteger, ZGFlutterVideoFrameFormat) {
    /// Unknown format, will take platform default
    ZGFlutterVideoFrameFormatUnknown = 0,
    /// I420 (YUV420Planar) format
    ZGFlutterVideoFrameFormatI420 = 1,
    /// NV12 (YUV420SemiPlanar) format
    ZGFlutterVideoFrameFormatNV12 = 2,
    /// NV21 (YUV420SemiPlanar) format
    ZGFlutterVideoFrameFormatNV21 = 3,
    /// BGRA32 format
    ZGFlutterVideoFrameFormatBGRA32 = 4,
    /// RGBA32 format
    ZGFlutterVideoFrameFormatRGBA32 = 5,
    /// ARGB32 format
    ZGFlutterVideoFrameFormatARGB32 = 6,
    /// ABGR32 format
    ZGFlutterVideoFrameFormatABGR32 = 7,
    /// I422 (YUV422Planar) format
    ZGFlutterVideoFrameFormatI422 = 8
};

/// Video encoded frame format.
typedef NS_ENUM(NSUInteger, ZGFlutterVideoEncodedFrameFormat) {
    /// AVC AVCC format
    ZGFlutterVideoEncodedFrameFormatAVCC = 0,
    /// AVC Annex-B format
    ZGFlutterVideoEncodedFrameFormatAnnexB = 1,
    /// VP8 format
    ZGFlutterVideoEncodedFrameFormatVP8 = 2,
    /// HEVC AVCC format
    ZGFlutterVideoEncodedFrameFormatHEVCAVCC = 3,
    /// HEVC Annex-B format
    ZGFlutterVideoEncodedFrameFormatHEVCAnnexB = 4
};

/// Video rendering fill mode.
typedef NS_ENUM(NSUInteger, ZGFlutterViewMode) {
    /// The proportional scaling up, there may be black borders
    ZGFlutterViewModeAspectFit = 0,
    /// The proportional zoom fills the entire View and may be partially cut
    ZGFlutterViewModeAspectFill = 1,
    /// Fill the entire view, the image may be stretched
    ZGFlutterViewModeScaleToFill = 2
};

/// audio sample rate.
typedef NS_ENUM(NSUInteger, ZGFlutterAudioSampleRate) {
    /// Unknown
    ZGFlutterAudioSampleRateUnknown = 0,
    /// 8K
    ZGFlutterAudioSampleRate8K = 8000,
    /// 16K
    ZGFlutterAudioSampleRate16K = 16000,
    /// 22.05K
    ZGFlutterAudioSampleRate22K = 22050,
    /// 24K
    ZGFlutterAudioSampleRate24K = 24000,
    /// 32K
    ZGFlutterAudioSampleRate32K = 32000,
    /// 44.1K
    ZGFlutterAudioSampleRate44K = 44100,
    /// 48K
    ZGFlutterAudioSampleRate48K = 48000
};

/// Audio channel type.
typedef NS_ENUM(NSUInteger, ZGFlutterAudioChannel) {
    /// Unknown
    ZGFlutterAudioChannelUnknown = 0,
    /// Mono
    ZGFlutterAudioChannelMono = 1,
    /// Stereo
    ZGFlutterAudioChannelStereo = 2
};

/// Object for video frame fieldeter.
///
/// Including video frame format, width and height, etc.
@interface ZGFlutterVideoFrameParam : NSObject

/// Video frame format
@property (nonatomic, assign) ZGFlutterVideoFrameFormat format;

/// Number of bytes per line (for example: BGRA only needs to consider strides [0], I420 needs to consider strides [0,1,2])
@property (nonatomic, assign, nonnull) int *strides;

/// The rotation direction of the video frame, the SDK rotates clockwise
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

@end

/// Externally encoded data traffic control information.
@interface ZGFlutterTrafficControlInfo : NSObject

/// Video FPS to be adjusted
@property (nonatomic, assign) int fps;

/// Video bitrate in kbps to be adjusted
@property (nonatomic, assign) int bitrate;

/// Video resolution to be adjusted
@property (nonatomic, assign) CGSize resolution;

@end

/// Object for video encoded frame fieldeter.
///
/// Including video encoded frame format, width and height, etc.
@interface ZGFlutterVideoEncodedFrameParam : NSObject

/// Video encoded frame format
@property (nonatomic, assign) ZGFlutterVideoEncodedFrameFormat format;

/// Whether it is a keyframe
@property (nonatomic, assign) BOOL isKeyFrame;

/// Video frame counterclockwise rotation angle
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

/// SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
@property (nonatomic, strong, nullable) NSData *SEIData;

@end

/// Parameter object for audio frame.
///
/// Including the sampling rate and channel of the audio frame
@interface ZGFlutterAudioFrameParam : NSObject

/// Sampling Rate
@property (nonatomic, assign) ZGFlutterAudioSampleRate sampleRate;

/// Audio channel, default is Mono
@property (nonatomic, assign) ZGFlutterAudioChannel channel;

@end

#endif /* ZegoCustomVideoDefine_h */
