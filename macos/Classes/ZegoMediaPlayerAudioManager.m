//
//  ZegoMediaPlayerAudioManager.m
//  zego_express_engine
//
//  Created by zego on 2023/12/12.
//

#import "ZegoMediaPlayerAudioManager.h"
#import "ZegoTextureRendererController.h"
#import "ZegoExpressEngineMethodHandler.h"
#import "ZegoLog.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoMediaPlayerAudioManager() <ZegoMediaPlayerAudioHandler>

@property (nonatomic, weak) id<ZegoFlutterMediaPlayerAudioHandler> handler;

@end

@implementation ZegoMediaPlayerAudioManager

+ (instancetype)sharedInstance {
    static ZegoMediaPlayerAudioManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoMediaPlayerAudioManager alloc] init];
    });
    return instance;
}

-(void)setAudioHandler:(id<ZegoFlutterMediaPlayerAudioHandler>)handler {
    
    self.handler = handler;
}

-(id<ZegoFlutterMediaPlayerAudioHandler>_Nullable)getMediaPlayerAudioHandler {
    return self.handler;
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer audioFrameData:(unsigned char *)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param {
    if([self.handler respondsToSelector:@selector(mediaPlayer:audioFrameData:dataLength:param:)]) {
        ZGFlutterAudioFrameParam *audioFrameParam = [[ZGFlutterAudioFrameParam alloc] init];
        audioFrameParam.sampleRate = (ZGFlutterAudioSampleRate)param.sampleRate;
        audioFrameParam.channel = (ZGFlutterAudioChannel)param.channel;
        
        [self.handler mediaPlayer:[mediaPlayer.index intValue] audioFrameData:data dataLength:dataLength param:audioFrameParam];
    }
}

@end
