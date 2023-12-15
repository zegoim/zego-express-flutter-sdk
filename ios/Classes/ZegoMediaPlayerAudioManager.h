//
//  ZegoMediaPlayerAudioManager.h
//  Pods
//
//  Created by zego on 2023/11/28.
//

#ifndef ZegoMediaPlayerAudioManager_h
#define ZegoMediaPlayerAudioManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "ZegoCustomVideoDefine.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterMediaPlayerAudioHandler <NSObject>

@optional

/// The callback triggered when the media player throws out audio frame data.
///
/// Description: The callback triggered when the media player throws out audio frame data.
/// Trigger: The callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setAudioHandler] to set.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param mediaPlayerIndex Callback player index
/// @param data Raw data of audio frames.
/// @param dataLength Data length.
/// @param param audio frame flip mode.
- (void)mediaPlayer:(int)mediaPlayerIndex
     audioFrameData:(unsigned char *)data
         dataLength:(unsigned int)dataLength
              param:(ZGFlutterAudioFrameParam *)param;

@end

@interface ZegoMediaPlayerAudioManager : NSObject

/// Get the custom audio capture manager instance
+ (instancetype)sharedInstance;

/// Set audio data callback handler of the media player.
///
/// Available since: 2.1.0
/// Description: By setting this callback, the audio data of the media resource file played by the media player can be called back.
/// When to call: After the [ZegoMediaPlayer] instance created.
/// Restrictions: None.
/// Caution: When you no longer need to get the audio frame data, please call this function again to clear the handler to stop the audio frame data callback.
///
/// @param handler audio data callback handler for media player
- (void)setAudioHandler:(id<ZegoFlutterMediaPlayerAudioHandler>)handler;

-(id<ZegoFlutterMediaPlayerAudioHandler>_Nullable)getMediaPlayerAudioHandler;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoMediaPlayerAudioManager_h */
