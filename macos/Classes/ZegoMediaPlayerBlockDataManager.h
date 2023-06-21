//
//  ZegoMediaPlayerBlockDataManager.h
//  Pods
//
//  Created by 27 on 2023/2/2.
//

#ifndef ZegoMediaPlayerBlockDataManager_h
#define ZegoMediaPlayerBlockDataManager_h

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZegoFlutterMediaPlayerBlockDataHandler <NSObject>

@optional

/// The callback triggered when the media player is about to throw the block data of the media resource.
///
/// Available since: 3.4.0
/// Description: The callback triggered when the media player is about to throw the block data of the media resource.
/// Trigger: The callback is generated when the media player starts playing.
/// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param mediaPlayerIndex Callback player index.
/// @param path The path of the media resource.
- (void)mediaPlayer:(int)mediaPlayerIndex blockBegin:(NSString *)path;

/// The callback triggered when the media player throws the block data of the media resource.
///
/// Available since: 3.4.0
/// Description: The callback triggered when the media player throws the block data of the media resource.
/// Trigger: This callback will be generated after receiving the [onBlockBegin] callback.
/// Caution: The callback does not actually take effect until call [setBlockDataHandler] to set. The buffer size before and after decryption should be consistent.
/// Restrictions: When playing copyrighted music, this callback will be disabled by default. If necessary, please contact ZEGO technical support.
///
/// @param mediaPlayerIndex Callback player index.
/// @param buffer The block data of the media resource.
/// @param bufferSize Length of media resource block data.
/// @return The size of the buffer, -1 is returned for failure.
- (int)mediaPlayer:(int)mediaPlayerIndex
         blockData:(unsigned char *const)buffer
        bufferSize:(unsigned int)bufferSize;
@end

@interface ZegoMediaPlayerBlockDataManager : NSObject

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
- (void)setBlockDataHandler:(id<ZegoFlutterMediaPlayerBlockDataHandler>)handler;

- (id<ZegoFlutterMediaPlayerBlockDataHandler>_Nullable)getMediaPlayerBlockDataHandler;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoMediaPlayerBlockDataManager_h */
