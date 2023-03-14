//
//  ZegoMediaPlayerVideoManager.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoMediaPlayerVideoManager_h
#define ZegoMediaPlayerVideoManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterMediaPlayerVideoHandler <NSObject>

@optional

/// The callback triggered when the media player throws out video frame data (in CVPixelBuffer format).
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mediaPlayerIndex Callback player index
/// @param buffer video data of CVPixelBuffer format
/// @param param video data frame param
- (void)mediaPlayer:(int)mediaPlayerIndex
    videoFramePixelBuffer:(CVPixelBufferRef)buffer
                    param:(ZGFlutterVideoFrameParam *)param;

/// The callback triggered when the media player throws out video frame data (in CVPixelBuffer format).
/// Note: This function is only available in ZegoExpressVideo SDK!
///
/// @param mediaPlayerIndex Callback player index
/// @param buffer video data of CVPixelBuffer format
/// @param param video data frame param
/// @param extraInfo video data frame extra info.
- (void)mediaPlayer:(int)mediaPlayerIndex
    videoFramePixelBuffer:(CVPixelBufferRef)buffer
                    param:(ZGFlutterVideoFrameParam *)param
                extraInfo:(NSDictionary *)extraInfo;

@end

@interface ZegoMediaPlayerVideoManager : NSObject

/// Get the custom video capture manager instance
+ (instancetype)sharedInstance;

/// Set video data callback handler of the media player.
///
/// Available since: 2.1.0
/// Description: By setting this callback, the video data of the media resource file played by the media player can be called back.
/// When to call: After the [ZegoMediaPlayer] instance created.
/// Restrictions: None.
/// Caution: When you no longer need to get the video frame data, please call this function again to clear the handler to stop the video frame data callback.
///
/// @param handler Video data callback handler for media player
- (void)setVideoHandler:(id<ZegoFlutterMediaPlayerVideoHandler>)handler;

-(id<ZegoFlutterMediaPlayerVideoHandler>_Nullable)getMediaPlayerVideoHandler;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoMediaPlayerVideoManager_h */
