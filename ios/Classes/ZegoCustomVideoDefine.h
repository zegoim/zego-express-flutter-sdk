//
//  ZegoCustomVideoDefine.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoCustomVideoDefine_h
#define ZegoCustomVideoDefine_h

/// Publish channel.
typedef NS_ENUM(NSUInteger, PublishChannel) {
    /// The main (default/first) publish channel.
    PublishChannelMain = 0,
    /// The auxiliary (second) publish channel
    PublishChannelAux = 1,
    /// The third publish channel
    PublishChannelThird = 2,
    /// The fourth publish channel
    PublishChannelFourth = 3
};

/// Video frame flip mode.
typedef NS_ENUM(NSUInteger, VideoFlipMode) {
    /// No flip
    VideoFlipModeNone = 0,
    /// X-axis flip
    VideoFlipModeX = 1,
    /// Y-axis flip
    VideoFlipModeY = 2,
    /// X-Y-axis flip
    VideoFlipModeXY = 3
};

/// Video frame format.
typedef NS_ENUM(NSUInteger, VideoFrameFormat) {
    /// Unknown format, will take platform default
    VideoFrameFormatUnknown = 0,
    /// I420 (YUV420Planar) format
    VideoFrameFormatI420 = 1,
    /// NV12 (YUV420SemiPlanar) format
    VideoFrameFormatNV12 = 2,
    /// NV21 (YUV420SemiPlanar) format
    VideoFrameFormatNV21 = 3,
    /// BGRA32 format
    VideoFrameFormatBGRA32 = 4,
    /// RGBA32 format
    VideoFrameFormatRGBA32 = 5,
    /// ARGB32 format
    VideoFrameFormatARGB32 = 6,
    /// ABGR32 format
    VideoFrameFormatABGR32 = 7,
    /// I422 (YUV422Planar) format
    VideoFrameFormatI422 = 8
};

/// Video encoded frame format.
typedef NS_ENUM(NSUInteger, VideoEncodedFrameFormat) {
    /// AVC AVCC format
    VideoEncodedFrameFormatAVCC = 0,
    /// AVC Annex-B format
    VideoEncodedFrameFormatAnnexB = 1,
    /// VP8 format
    VideoEncodedFrameFormatVP8 = 2,
    /// HEVC AVCC format
    VideoEncodedFrameFormatHEVCAVCC = 3,
    /// HEVC Annex-B format
    VideoEncodedFrameFormatHEVCAnnexB = 4
};

/// Video rendering fill mode.
typedef NS_ENUM(NSUInteger, ViewMode) {
    /// The proportional scaling up, there may be black borders
    ViewModeAspectFit = 0,
    /// The proportional zoom fills the entire View and may be partially cut
    ViewModeAspectFill = 1,
    /// Fill the entire view, the image may be stretched
    ViewModeScaleToFill = 2
};

/// Object for video frame fieldeter.
///
/// Including video frame format, width and height, etc.
@interface VideoFrameParam : NSObject

/// Video frame format
@property (nonatomic, assign) VideoFrameFormat format;

/// Number of bytes per line (for example: BGRA only needs to consider strides [0], I420 needs to consider strides [0,1,2])
@property (nonatomic, assign) int *strides;

/// The rotation direction of the video frame, the SDK rotates clockwise
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

@end

/// Externally encoded data traffic control information.
@interface TrafficControlInfo : NSObject

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
@interface VideoEncodedFrameParam : NSObject

/// Video encoded frame format
@property (nonatomic, assign) VideoEncodedFrameFormat format;

/// Whether it is a keyframe
@property (nonatomic, assign) BOOL isKeyFrame;

/// Video frame counterclockwise rotation angle
@property (nonatomic, assign) int rotation;

/// Video frame size
@property (nonatomic, assign) CGSize size;

/// SEI data (Optional, if you don't need to send SEI, set it to null. Deprecated, use [sendSEI] instead). Useful when set format as [AVCC] or [AnnexB]
@property (nonatomic, strong, nullable) NSData *SEIData;

@end

#endif /* ZegoCustomVideoDefine_h */
