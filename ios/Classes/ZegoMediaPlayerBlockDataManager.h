//
//  ZegoMediaPlayerBlockDataManager.h
//  zego_express_engine
//
//  Created by zego on 2023/6/15.
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

/// Set the media resource block data callback handler of the media player.
///
/// Available since: 3.4.0
/// Description: This callback can be set to listen to and decrypt the media resource block data for playing the user's own ciphertext media resources.
/// When to call: After the [ZegoMediaPlayer] instance created, before playing media resources.
/// Restrictions: None.
/// Caution: When it is no longer necessary to listen to the callback for data decryption, please call this function again to clear the handler.
///
/// @param handler The media resource block data callback handler of the media player.
- (void)setBlockDataHandler:(id<ZegoFlutterMediaPlayerBlockDataHandler>)handler;

- (id<ZegoFlutterMediaPlayerBlockDataHandler>_Nullable)getMediaPlayerBlockDataHandler;

@end

NS_ASSUME_NONNULL_END

#endif /* ZegoMediaPlayerVideoManager_h */
