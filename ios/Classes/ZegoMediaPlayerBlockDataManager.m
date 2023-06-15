//
//  ZegoMediaPlayerBlockDataManager.m
//  zego_express_engine
//
//  Created by zego on 2023/6/15.
//

#import "ZegoMediaPlayerBlockDataManager.h"
#import "ZegoLog.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoMediaPlayerBlockDataManager() <ZegoMediaPlayerBlockDataHandler>

@property (nonatomic, weak) id<ZegoFlutterMediaPlayerBlockDataHandler> handler;

@end

@implementation ZegoMediaPlayerBlockDataManager

+ (instancetype)sharedInstance {
    static ZegoMediaPlayerBlockDataManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoMediaPlayerBlockDataManager alloc] init];
    });
    return instance;
}

- (void)setBlockDataHandler:(id<ZegoFlutterMediaPlayerBlockDataHandler>)handler {
    self.handler = handler;
}

- (id<ZegoFlutterMediaPlayerBlockDataHandler>)getMediaPlayerBlockDataHandler {
    return self.handler;
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer blockBegin:(NSString *)path {
    ZGLog(@"[MediaPlayer][mediaPlayer:blockBegin:] path:%@", path);
    if ([self.handler respondsToSelector:@selector(mediaPlayer:blockBegin:)]) {
        [self.handler mediaPlayer:mediaPlayer.index.intValue blockBegin:path];
    }
}

- (int)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer blockData:(unsigned char *const)buffer bufferSize:(unsigned int)bufferSize {
    if ([self.handler respondsToSelector:@selector(mediaPlayer:blockData:bufferSize:)]) {
        return [self.handler mediaPlayer:mediaPlayer.index.intValue blockData:buffer bufferSize:bufferSize];
    }
    return -1;
}

@end
