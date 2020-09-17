//
//  ZegoCustomVideoCaptureManager.h
//  Pods
//
//  Created by lizhanpeng@ZEGO on 2020/9/11.
//

#ifndef ZegoCustomVideoCaptureManager_h
#define ZegoCustomVideoCaptureManager_h

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

@interface ZegoCustomVideoCaptureClient : NSObject

- (void)setVideoMirrorMode:(int)mode;

- (void)sendCVPixelBuffer:(CVPixelBufferRef)buffer timestamp:(CMTime)timestamp;

+ (instancetype)new NS_UNAVAILABLE;

- (instancetype)init NS_UNAVAILABLE;

@end

@protocol ZegoCustomVideoCaptureDelegate <NSObject>

@required

- (void)onStart:(ZegoCustomVideoCaptureClient*)client;

- (void)onStop;

@end

@interface ZegoCustomVideoCaptureManager : NSObject

+ (instancetype)sharedInstance;

- (void)setCustomVideoCaptureDelegate:(id<ZegoCustomVideoCaptureDelegate>)delegate channel:(int)channel;

@end

#endif /* ZegoCustomVideoCaptureManager_h */
