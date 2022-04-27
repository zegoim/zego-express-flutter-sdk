//
//  ZegoExpressEngineMethodHandler.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/5/8.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoExpressEngineMethodHandler.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

#import "ZegoExpressEngineEventHandler.h"

#import "ZegoPlatformViewFactory.h"
#import "ZegoTextureRendererController.h"
#import "ZegoCustomVideoCaptureManager.h"

#import "ZegoUtils.h"
#import "ZegoLog.h"
#import <objc/message.h>

@interface ZegoExpressEngineMethodHandler ()

@property (nonatomic, strong) id<FlutterPluginRegistrar> registrar;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoMediaPlayer *> *mediaPlayerMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoAudioEffectPlayer *> *audioEffectPlayerMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoRealTimeSequentialDataManager *> *realTimeSequentialDataManagerMap;

@property (nonatomic, strong) ZegoRangeAudio *rangeAudioInstance;

@end

@implementation ZegoExpressEngineMethodHandler

+ (instancetype)sharedInstance {
    static ZegoExpressEngineMethodHandler *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoExpressEngineMethodHandler alloc] init];
    });
    return instance;
}


#pragma mark - Main

- (void)getVersion:(FlutterMethodCall *)call result:(FlutterResult)result {

    result([ZegoExpressEngine getVersion]);
}

- (void)createEngineWithProfile:(FlutterMethodCall *)call result:(FlutterResult)result {

    // Report framework info
    [self reportPluginInfo];

    NSDictionary *profileMap = call.arguments[@"profile"];
    unsigned int appID = [ZegoUtils unsignedIntValue:profileMap[@"appID"]];
    NSString *appSign = profileMap[@"appSign"];
    // BOOL isTestEnv = [ZegoUtils boolValue:call.arguments[@"isTestEnv"]];
    int scenario = [ZegoUtils intValue:profileMap[@"scenario"]];

    _enablePlatformView = [ZegoUtils boolValue:profileMap[@"enablePlatformView"]];
    _registrar = call.arguments[@"registrar"];

    // Set eventSink for ZegoExpressEngineEventHandler
    FlutterEventSink sink = call.arguments[@"eventSink"];
    if (!sink) {
        ZGError(@"[createEngineWithProfile] FlutterEventSink is nil");
    }
    [ZegoExpressEngineEventHandler sharedInstance].eventSink = sink;

    // Create engine
    ZegoEngineProfile *profile = [ZegoEngineProfile new];
    profile.appID = appID;
    if (![ZegoUtils isNullObject:appSign]) {
        profile.appSign = appSign;
    }
    profile.scenario = (ZegoScenario)scenario;
    [ZegoExpressEngine createEngineWithProfile:profile eventHandler:[ZegoExpressEngineEventHandler sharedInstance]];

    // Set platform language to dart
    SEL selector = NSSelectorFromString(@"setPlatformLanguage:");
    if ([ZegoExpressEngine respondsToSelector:selector]) {
        ((void (*)(id, SEL, int))objc_msgSend)(ZegoExpressEngine.class, selector, 4);
    }

    [ZegoExpressEngine setApiCalledCallback:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setDataRecordEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setAudioDataHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setCustomAudioProcessHandler:[ZegoExpressEngineEventHandler sharedInstance]];

    // Init texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] initController];
    }

    ZGLog(@"[createEngineWithProfile] platform:iOS, enablePlatformView:%@, sink: %p, appID:%u, appSign:%@, scenario:%d", _enablePlatformView ? @"true" : @"false", sink, appID, appSign, scenario);

    result(nil);
}

- (void)createEngine:(FlutterMethodCall *)call result:(FlutterResult)result {

    // Report framework info
    [self reportPluginInfo];

    unsigned int appID = [ZegoUtils unsignedIntValue:call.arguments[@"appID"]];
    NSString *appSign = call.arguments[@"appSign"];
    BOOL isTestEnv = [ZegoUtils boolValue:call.arguments[@"isTestEnv"]];
    int scenario = [ZegoUtils intValue:call.arguments[@"scenario"]];

    _enablePlatformView = [ZegoUtils boolValue:call.arguments[@"enablePlatformView"]];
    _registrar = call.arguments[@"registrar"];

    // Set eventSink for ZegoExpressEngineEventHandler
    FlutterEventSink sink = call.arguments[@"eventSink"];
    if (!sink) {
        ZGError(@"[createEngine] FlutterEventSink is nil");
    }
    [ZegoExpressEngineEventHandler sharedInstance].eventSink = sink;

    // Create engine
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [ZegoExpressEngine createEngineWithAppID:appID appSign:appSign isTestEnv:isTestEnv scenario:(ZegoScenario)scenario eventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
#pragma clang diagnostic pop

    // Set platform language to dart
    SEL selector = NSSelectorFromString(@"setPlatformLanguage:");
    if ([ZegoExpressEngine respondsToSelector:selector]) {
        ((void (*)(id, SEL, int))objc_msgSend)(ZegoExpressEngine.class, selector, 4);
    }

    [ZegoExpressEngine setApiCalledCallback:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setDataRecordEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setAudioDataHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setCustomAudioProcessHandler:[ZegoExpressEngineEventHandler sharedInstance]];

    // Init texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] initController];
    }

    ZGLog(@"[createEngine] platform:iOS, enablePlatformView:%@, sink: %p, appID:%u, appSign:%@, isTestEnv:%@, scenario:%d", _enablePlatformView ? @"true" : @"false", sink, appID, appSign, isTestEnv ? @"true" : @"false", scenario);

    result(nil);
}

- (void)destroyEngine:(FlutterMethodCall *)call result:(FlutterResult)result {

    [ZegoExpressEngine destroyEngine:nil];

    // Uninit texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] uninitController];
    }

    result(nil);
}

- (void)setEngineConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];
    ZegoEngineConfig *configObject = nil;

    if (configMap && configMap.count > 0) {

        configObject = [[ZegoEngineConfig alloc] init];
        configObject.advancedConfig = configMap[@"advancedConfig"];

        NSDictionary *logConfigMap = configMap[@"logConfig"];
        ZegoLogConfig *logConfigObject = nil;
        if (logConfigMap && logConfigMap.count > 0) {
            logConfigObject = [[ZegoLogConfig alloc] init];
            logConfigObject.logPath = logConfigMap[@"logPath"];
            logConfigObject.logSize = [ZegoUtils unsignedLongLongValue:logConfigMap[@"logSize"]];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            configObject.logConfig = logConfigObject;
#pragma clang diagnostic pop
        }

        [ZegoExpressEngine setEngineConfig:configObject];

        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"setEngineConfig_null_config" uppercaseString] message:@"Invoke `setEngineConfig` with null config" details:nil]);
    }
}

- (void)setLogConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];
    ZegoLogConfig *logConfigObject = nil;

    if (configMap && configMap.count > 0) {
        logConfigObject = [[ZegoLogConfig alloc] init];

        logConfigObject.logPath = configMap[@"logPath"];
        logConfigObject.logSize = [ZegoUtils unsignedLongLongValue:configMap[@"logSize"]];

        [ZegoExpressEngine setLogConfig:logConfigObject];

        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"setLogConfig_null_config" uppercaseString] message:@"Invoke `setLogConfig` with null config" details:nil]);
    }
}

- (void)setRoomMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    
    [ZegoExpressEngine setRoomMode:(ZegoRoomMode)mode];

    result(nil);
}


- (void)uploadLog:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] uploadLog];

    result(nil);
}

- (void)setDebugVerbose:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int language = [ZegoUtils intValue:call.arguments[@"language"]];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] setDebugVerbose:enable language:(ZegoLanguage)language];
#pragma clang diagnostic pop

    result(nil);
}

- (void)enableDebugAssistant:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableDebugAssistant:enable];

    result(nil);
}

- (void)callExperimentalAPI:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *params = call.arguments[@"params"];

    NSString *callResult = [[ZegoExpressEngine sharedEngine] callExperimentalAPI:params];

    result(callResult);
}

- (void)setDummyCaptureImagePath:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *filePath = call.arguments[@"filePath"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setDummyCaptureImagePath:filePath channel:(ZegoPublishChannel)channel];

    result(nil);
}


#pragma mark - Room

- (void)loginRoom:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSDictionary *userMap = call.arguments[@"user"];
    NSDictionary *configMap = call.arguments[@"config"];

    ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:userMap[@"userID"] userName:userMap[@"userName"]];

    if (configMap && configMap.count > 0) {
        unsigned int maxMemberCount = [ZegoUtils unsignedIntValue:configMap[@"maxMemberCount"]];
        BOOL isUserStatusNotify = [ZegoUtils boolValue:configMap[@"isUserStatusNotify"]];
        NSString *token = configMap[@"token"];

        ZegoRoomConfig *configObject = [[ZegoRoomConfig alloc] init];
        configObject.maxMemberCount = maxMemberCount;
        configObject.isUserStatusNotify = isUserStatusNotify;
        configObject.token = token;

        [[ZegoExpressEngine sharedEngine] loginRoom:roomID user:userObject config:configObject];
    } else {
        [[ZegoExpressEngine sharedEngine] loginRoom:roomID user:userObject];
    }

    result(nil);
}

- (void)loginMultiRoom:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSString *roomID = call.arguments[@"roomID"];
    NSDictionary *configMap = call.arguments[@"config"];
    ZegoRoomConfig *configObject = nil;

    if (configMap && configMap.count > 0) {
        unsigned int maxMemberCount = [ZegoUtils unsignedIntValue:configMap[@"maxMemberCount"]];
        BOOL isUserStatusNotify = [ZegoUtils boolValue:configMap[@"isUserStatusNotify"]];
        NSString *token = configMap[@"token"];

        configObject = [[ZegoRoomConfig alloc] init];
        configObject.maxMemberCount = maxMemberCount;
        configObject.isUserStatusNotify = isUserStatusNotify;
        configObject.token = token;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] loginMultiRoom:roomID config:configObject];
#pragma clang diagnostic pop

    result(nil);
}

- (void)logoutRoom:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    if ([ZegoUtils isNullObject:roomID]) {
        [[ZegoExpressEngine sharedEngine] logoutRoom];
    } else {
        [[ZegoExpressEngine sharedEngine] logoutRoom:roomID];
    }
    
    result(nil);
}

- (void)switchRoom:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *fromRoomID = call.arguments[@"fromRoomID"];
    NSString *toRoomID = call.arguments[@"toRoomID"];
    NSDictionary *configMap = call.arguments[@"config"];

    if (configMap && configMap.count > 0) {
        unsigned int maxMemberCount = [ZegoUtils unsignedIntValue:configMap[@"maxMemberCount"]];
        BOOL isUserStatusNotify = [ZegoUtils boolValue:configMap[@"isUserStatusNotify"]];
        NSString *token = configMap[@"token"];

        ZegoRoomConfig *configObject = [[ZegoRoomConfig alloc] init];
        configObject.maxMemberCount = maxMemberCount;
        configObject.isUserStatusNotify = isUserStatusNotify;
        configObject.token = token;

        [[ZegoExpressEngine sharedEngine] switchRoom:fromRoomID toRoomID:toRoomID config:configObject];
    } else {
        [[ZegoExpressEngine sharedEngine] switchRoom:fromRoomID toRoomID:toRoomID];
    }

    result(nil);
}

- (void)renewToken:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSString *token = call.arguments[@"token"];

    [[ZegoExpressEngine sharedEngine] renewToken:token roomID:roomID];

    result(nil);
}

- (void)setRoomExtraInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSString *key = call.arguments[@"key"];
    NSString *value = call.arguments[@"value"];

    [[ZegoExpressEngine sharedEngine] setRoomExtraInfo:value forKey:key roomID:roomID callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}


#pragma mark - Publisher

- (void)startPublishingStream:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    
    ZegoPublisherConfig *config = nil;
    NSDictionary *configMap = call.arguments[@"config"];

    if (configMap && configMap.count > 0) {
        config = [[ZegoPublisherConfig alloc] init];
        config.roomID = configMap[@"roomID"];
    }

    if (config) {
        [[ZegoExpressEngine sharedEngine] startPublishingStream:streamID config:config channel:(ZegoPublishChannel)channel];
    } else {
        [[ZegoExpressEngine sharedEngine] startPublishingStream:streamID channel:(ZegoPublishChannel)channel];
    }

    result(nil);
}

- (void)stopPublishingStream:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] stopPublishingStream:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setStreamExtraInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *extraInfo = call.arguments[@"extraInfo"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setStreamExtraInfo:extraInfo channel:(ZegoPublishChannel)channel callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}

- (void)startPreview:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    // Handle ZegoCanvas

    NSDictionary *canvasMap = call.arguments[@"canvas"];

    if (canvasMap && canvasMap.count > 0) {
        // Preview video

        // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
        int64_t viewID = [ZegoUtils longLongValue:canvasMap[@"view"]];
        int viewMode = [ZegoUtils intValue:canvasMap[@"viewMode"]];
        int backgroundColor = [ZegoUtils intValue:canvasMap[@"backgroundColor"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];

            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:[platformView getUIView]];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;

                [[ZegoExpressEngine sharedEngine] startPreview:canvas channel:(ZegoPublishChannel)channel];
            } else {
                // Preview video without creating the PlatformView in advance
                // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                NSString *errorMessage = [NSString stringWithFormat:@"The PlatformView for viewID:%ld cannot be found, developer should call `createPlatformView` first and get the viewID", (long)viewID];
                ZGError(@"[startPreview] %@", errorMessage);
                result([FlutterError errorWithCode:[@"startPreview_No_PlatformView" uppercaseString] message:errorMessage details:nil]);
                return;
            }

        } else {
            // Render with Texture
            if ([[ZegoTextureRendererController sharedInstance] addCapturedRenderer:viewID key:@(channel) viewMode:(ZegoViewMode)viewMode]) {
                [[ZegoTextureRendererController sharedInstance] startRendering];
            } else {
                // Preview video without creating TextureRenderer in advance
                // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createTextureRenderer` first and get the textureID", (long)viewID];
                ZGError(@"[startPreview] %@", errorMessage);
                result([FlutterError errorWithCode:[@"startPreview_No_TextureRenderer" uppercaseString] message:errorMessage details:nil]);
                return;
            }

            // Using Custom Video Renderer
            [[ZegoExpressEngine sharedEngine] startPreview:nil channel:(ZegoPublishChannel)channel];
        }

    } else { /* if (canvas && canvas.count > 0) */

        // Preview audio only
        [[ZegoExpressEngine sharedEngine] startPreview:nil channel:(ZegoPublishChannel)channel];
    }

    result(nil);
}

- (void)stopPreview:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    [[ZegoExpressEngine sharedEngine] stopPreview:(ZegoPublishChannel)channel];

    if (!self.enablePlatformView) {
        // Stop Texture Renderer
        [[ZegoTextureRendererController sharedInstance] removeCapturedRenderer:@(channel)];
        [[ZegoTextureRendererController sharedInstance] stopRendering];
    }

    result(nil);
}

- (void)setVideoConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];
    int capWidth = [ZegoUtils intValue:configMap[@"captureWidth"]];
    int capHeight = [ZegoUtils intValue:configMap[@"captureHeight"]];
    int encWidth = [ZegoUtils intValue:configMap[@"encodeWidth"]];
    int encHeight = [ZegoUtils intValue:configMap[@"encodeHeight"]];
    int bitrate = [ZegoUtils intValue:configMap[@"bitrate"]];
    int fps = [ZegoUtils intValue:configMap[@"fps"]];
    int codecID = [ZegoUtils intValue:configMap[@"codecID"]];

    ZegoVideoConfig *configObject = [[ZegoVideoConfig alloc] init];
    configObject.captureResolution = CGSizeMake(capWidth, capHeight);
    configObject.encodeResolution = CGSizeMake(encWidth, encHeight);
    configObject.bitrate = bitrate;
    configObject.fps = fps;
    configObject.codecID = (ZegoVideoCodecID)codecID;
    
    if (configMap[@"keyFrameInterval"]) {
        configObject.keyFrameInterval = [ZegoUtils intValue:configMap[@"keyFrameInterval"]];
    }

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setVideoConfig:configObject channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)getVideoConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    ZegoVideoConfig *configObject = [[ZegoExpressEngine sharedEngine] getVideoConfig:(ZegoPublishChannel)channel];

    result(@{
        @"captureWidth": @((int)configObject.captureResolution.width),
        @"captureHeight": @((int)configObject.captureResolution.height),
        @"encodeWidth": @((int)configObject.encodeResolution.width),
        @"encodeHeight": @((int)configObject.encodeResolution.height),
        @"bitrate": @(configObject.bitrate),
        @"fps": @(configObject.fps),
        @"codecID": @(configObject.codecID)
    });
}

- (void)setVideoMirrorMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mirrorMode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:(ZegoVideoMirrorMode)mode channel:(ZegoPublishChannel)channel];

    result(nil);

}

- (void)setAppOrientation:(FlutterMethodCall *)call result:(FlutterResult)result {

    int orientation = [ZegoUtils intValue:call.arguments[@"orientation"]];
    UIInterfaceOrientation  uiOrientation = UIInterfaceOrientationUnknown;
    switch (orientation) {
        case 0:
            uiOrientation = UIInterfaceOrientationPortrait;
            break;
        case 1:
            uiOrientation = UIInterfaceOrientationLandscapeRight;
            break;
        case 2:
            uiOrientation = UIInterfaceOrientationPortraitUpsideDown;
            break;
        case 3:
            uiOrientation = UIInterfaceOrientationLandscapeLeft;
        default:
            break;
    }

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setAppOrientation:uiOrientation channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setAudioConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    NSDictionary *configMap = call.arguments[@"config"];
    int bitrate = [ZegoUtils intValue:configMap[@"bitrate"]];
    int audioChannel = [ZegoUtils intValue:configMap[@"channel"]];
    int codecID = [ZegoUtils intValue:configMap[@"codecID"]];
    

    ZegoAudioConfig *configObject = [[ZegoAudioConfig alloc] init];
    configObject.bitrate = bitrate;
    configObject.channel = (ZegoAudioChannel)audioChannel;
    configObject.codecID = (ZegoAudioCodecID)codecID;

    [[ZegoExpressEngine sharedEngine] setAudioConfig:configObject channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)getAudioConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    ZegoAudioConfig *configObject = [[ZegoExpressEngine sharedEngine] getAudioConfig:channel];

    result(@{
        @"bitrate": @(configObject.bitrate),
        @"channel": @(configObject.channel),
        @"codecID": @(configObject.codecID)
    });
}

- (void)setPublishStreamEncryptionKey:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *key = call.arguments[@"key"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setPublishStreamEncryptionKey:key channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)takePublishStreamSnapshot:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] takePublishStreamSnapshot:^(int errorCode, UIImage * _Nullable image) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSData *imageData = nil;
            if (image) {
                imageData = UIImageJPEGRepresentation(image, 1);
            }

            dispatch_async(dispatch_get_main_queue(), ^{
                result(@{
                    @"errorCode": @(errorCode),
                    @"image": imageData ?: [[NSNull alloc] init]
                });
            });
        });
    } channel:(ZegoPublishChannel)channel];
}

- (void)mutePublishStreamAudio:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] mutePublishStreamAudio:mute channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)mutePublishStreamVideo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] mutePublishStreamVideo:mute channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setStreamAlignmentProperty:(FlutterMethodCall *)call result:(FlutterResult)result {

    int alignment = [ZegoUtils intValue:call.arguments[@"alignment"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setStreamAlignmentProperty:alignment channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)enableTrafficControl:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int property = [ZegoUtils intValue:call.arguments[@"property"]];

    [[ZegoExpressEngine sharedEngine] enableTrafficControl:enable property:(ZegoTrafficControlProperty)property];

    result(nil);
}

- (void)setMinVideoBitrateForTrafficControl:(FlutterMethodCall *)call result:(FlutterResult)result {

    int bitrate = [ZegoUtils intValue:call.arguments[@"bitrate"]];
    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setMinVideoBitrateForTrafficControl:bitrate mode:(ZegoTrafficControlMinVideoBitrateMode)mode channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setMinVideoFpsForTrafficControl:(FlutterMethodCall *)call result:(FlutterResult)result {

    int fps = [ZegoUtils intValue:call.arguments[@"fps"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setMinVideoFpsForTrafficControl:fps channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setMinVideoResolutionForTrafficControl:(FlutterMethodCall *)call result:(FlutterResult)result {

    int width = [ZegoUtils intValue:call.arguments[@"width"]];
    int height = [ZegoUtils intValue:call.arguments[@"height"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setMinVideoResolutionForTrafficControl:width height:height channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setTrafficControlFocusOn:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setTrafficControlFocusOn:(ZegoTrafficControlFocusOnMode)mode];

    result(nil);
}

- (void)setCaptureVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [[ZegoExpressEngine sharedEngine] setCaptureVolume:volume];

    result(nil);
}

- (void)setAudioCaptureStereoMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setAudioCaptureStereoMode:(ZegoAudioCaptureStereoMode)mode];

    result(nil);
}

- (void)addPublishCdnUrl:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *targetURL = call.arguments[@"targetURL"];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] addPublishCdnUrl:targetURL streamID:streamID callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}

- (void)removePublishCdnUrl:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *targetURL = call.arguments[@"targetURL"];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] removePublishCdnUrl:targetURL streamID:streamID callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}

- (void)enablePublishDirectToCDN:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    ZegoCDNConfig *cdnConfig = nil;
    NSDictionary *config = call.arguments[@"config"];
    if (config && config.count > 0) {
        NSString *url = config[@"url"];
        NSString *authParam = config[@"authParam"];

        cdnConfig = [[ZegoCDNConfig alloc] init];
        cdnConfig.url = url;
        cdnConfig.authParam = authParam;
    }

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enablePublishDirectToCDN:enable config:cdnConfig channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setPublishWatermark:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *watermarkMap = call.arguments[@"watermark"];
    ZegoWatermark *watermarkObject = nil;
    if (watermarkMap && watermarkMap.count > 0) {
        NSString *imageURL = watermarkMap[@"imageURL"];
        int left = [ZegoUtils intValue:watermarkMap[@"left"]];
        int top = [ZegoUtils intValue:watermarkMap[@"top"]];
        int bottom = [ZegoUtils intValue:watermarkMap[@"bottom"]];
        int right = [ZegoUtils intValue:watermarkMap[@"right"]];
        CGRect rect = CGRectMake(left, top, right - left, bottom - top);

        NSString *flutterAssetPrefix = @"flutter-asset://";
        if ([imageURL hasPrefix:flutterAssetPrefix]) {
            NSString *assetName = [imageURL substringFromIndex:flutterAssetPrefix.length];
            NSString *assetKey = [_registrar lookupKeyForAsset:assetName];
            NSString *assetRealPath = [[NSBundle mainBundle] pathForResource:assetKey ofType:nil];
            NSString *processedURL = [NSString stringWithFormat:@"file://%@", assetRealPath];
            ZGLog(@"[setPublishWatermark] Flutter asset prefix detected, origin URL: '%@', processed URL: '%@'", imageURL, processedURL);
            if (!assetRealPath) {
                ZGLog(@"[setPublishWatermark] Can not get real path for flutter asset: '%@', please check if the asset is correctly declared in flutter project's pubspec.yaml", assetName);
            } else {
                imageURL = processedURL;
            }
        }

        watermarkObject = [[ZegoWatermark alloc] initWithImageURL:imageURL layout:rect];
    }

    BOOL isPreviewVisible = [ZegoUtils boolValue:call.arguments[@"isPreviewVisible"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setPublishWatermark:watermarkObject isPreviewVisible:isPreviewVisible channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setSEIConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"watermark"];

    ZegoSEIConfig *config = [ZegoSEIConfig defaultConfig]; // Use default config
    if (configMap && configMap.count > 0) {
        config.type = (ZegoSEIType)[ZegoUtils intValue:configMap[@"type"]];
    }

    [[ZegoExpressEngine sharedEngine] setSEIConfig:config];

    result(nil);
}

- (void)sendSEI:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] sendSEI:data.data channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)enableHardwareEncoder:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableHardwareEncoder:enable];

    result(nil);
}

- (void)setCapturePipelineScaleMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setCapturePipelineScaleMode:(ZegoCapturePipelineScaleMode)mode];

    result(nil);
}

- (void)enableH265EncodeFallback:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableH265EncodeFallback:enable];

    result(nil);
}

- (void)isVideoEncoderSupported:(FlutterMethodCall *)call result:(FlutterResult)result {

    int codecID = [ZegoUtils intValue:call.arguments[@"codecID"]];

    BOOL isSupport = [[ZegoExpressEngine sharedEngine] isVideoEncoderSupported:(ZegoVideoCodecID)codecID];

    result(@(isSupport));
}


#pragma mark - Player

- (void)startPlayingStream:(FlutterMethodCall *)call result:(FlutterResult)result {
    // TODO: Deprecated since 1.19.0

    NSString *streamID = call.arguments[@"streamID"];

    // Handle ZegoPlayerConfig

    ZegoPlayerConfig *playerConfig = nil;

    NSDictionary *playerConfigMap = call.arguments[@"config"];

    if (playerConfigMap && playerConfigMap.count > 0) {

        playerConfig = [[ZegoPlayerConfig alloc] init];
        playerConfig.resourceMode = (ZegoStreamResourceMode)[ZegoUtils intValue:playerConfigMap[@"resourceMode"]];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        playerConfig.videoLayer = (ZegoPlayerVideoLayer)[ZegoUtils intValue:playerConfigMap[@"videoLayer"]];
#pragma clang diagnostic pop
        playerConfig.roomID = playerConfigMap[@"roomID"];
        NSDictionary * cdnConfigMap = playerConfigMap[@"cdnConfig"];

        if (cdnConfigMap && cdnConfigMap.count > 0) {
            ZegoCDNConfig *cdnConfig = [[ZegoCDNConfig alloc] init];
            cdnConfig.url = cdnConfigMap[@"url"];
            cdnConfig.authParam = cdnConfigMap[@"authParam"];
            playerConfig.cdnConfig = cdnConfig;
        }
    }

    // Handle ZegoCanvas

    NSDictionary *canvasMap = call.arguments[@"canvas"];

    if (canvasMap && canvasMap.count > 0) {
        // Play video

        // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
        int64_t viewID = [ZegoUtils longLongValue:canvasMap[@"view"]];
        int viewMode = [ZegoUtils intValue:canvasMap[@"viewMode"]];
        int backgroundColor = [ZegoUtils intValue:canvasMap[@"backgroundColor"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];

            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:[platformView getUIView]];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;

                if (playerConfig) {
                    [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:canvas config:playerConfig];
                } else {
                    [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:canvas];
                }
            } else {
                // Play video without creating the PlatformView in advance
                // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                NSString *errorMessage = [NSString stringWithFormat:@"The PlatformView for viewID:%ld cannot be found, developer should call `createPlatformView` first and get the viewID", (long)viewID];
                ZGError(@"[startPlayingStream] %@", errorMessage);
                result([FlutterError errorWithCode:[@"startPlayingStream_No_PlatformView" uppercaseString] message:errorMessage details:nil]);
                return;
            }

        } else {
            // Render with Texture
            if ([[ZegoTextureRendererController sharedInstance] addRemoteRenderer:viewID key:streamID viewMode:(ZegoViewMode)viewMode]) {
                [[ZegoTextureRendererController sharedInstance] startRendering];
            } else {
                // Play video without creating TextureRenderer in advance
                // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createTextureRenderer` first and get the textureID", (long)viewID];
                ZGError(@"[startPlayingStream] %@", errorMessage);
                result([FlutterError errorWithCode:[@"startPlayingStream_No_TextureRenderer" uppercaseString] message:errorMessage details:nil]);
                return;
            }

            // Using Custom Video Renderer
            if (playerConfig) {
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:nil config:playerConfig];
            } else {
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:nil];
            }
        }

    } else { /* if (canvas && canvas.count > 0) */

        // Play audio only
        if (playerConfig) {
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:nil config:playerConfig];
        } else {
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:nil];
        }
    }

    result(nil);
}

- (void)stopPlayingStream:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];
    [[ZegoExpressEngine sharedEngine] stopPlayingStream:streamID];

    if (!self.enablePlatformView) {
        // Stop Texture render
        [[ZegoTextureRendererController sharedInstance] removeRemoteRenderer:streamID];
        [[ZegoTextureRendererController sharedInstance] stopRendering];
    }

    result(nil);
}

- (void)setPlayStreamDecryptionKey:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *key = call.arguments[@"key"];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamDecryptionKey:key streamID:streamID];

    result(nil);
}

- (void)takePlayStreamSnapshot:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] takePlayStreamSnapshot:streamID callback:^(int errorCode, UIImage * _Nullable image) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSData *imageData = nil;
            if (image) {
                imageData = UIImageJPEGRepresentation(image, 1);
            }

            dispatch_async(dispatch_get_main_queue(), ^{
                result(@{
                    @"errorCode": @(errorCode),
                    @"image": imageData ?: [[NSNull alloc] init]
                });
            });
        });
    }];
}

- (void)setPlayVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayVolume:volume streamID:streamID];

    result(nil);
}

- (void)setAllPlayStreamVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [[ZegoExpressEngine sharedEngine] setAllPlayStreamVolume:volume];

    result(nil);
}

- (void)setPlayStreamVideoLayer:(FlutterMethodCall *)call result:(FlutterResult)result {

    int videoLayer = [ZegoUtils intValue:call.arguments[@"videoLayer"]];
    NSString *streamID = call.arguments[@"streamID"];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] setPlayStreamVideoLayer:(ZegoPlayerVideoLayer)videoLayer streamID:streamID];
#pragma clang diagnostic pop

    result(nil);
}

- (void)setPlayStreamVideoType:(FlutterMethodCall *)call result:(FlutterResult)result {

    int streamType = [ZegoUtils intValue:call.arguments[@"streamType"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamVideoType:(ZegoVideoStreamType)streamType streamID:streamID];

    result(nil);
}

- (void)setPlayStreamBufferIntervalRange:(FlutterMethodCall *)call result:(FlutterResult)result {

    int minBufferInterval = [ZegoUtils intValue:call.arguments[@"minBufferInterval"]];
    int maxBufferInterval = [ZegoUtils intValue:call.arguments[@"maxBufferInterval"]];
    NSString *streamID = call.arguments[@"streamID"];
    NSRange range = NSMakeRange(minBufferInterval, maxBufferInterval - minBufferInterval);

    [[ZegoExpressEngine sharedEngine] setPlayStreamBufferIntervalRange:range streamID:streamID];

    result(nil);
}

- (void)setPlayStreamFocusOn:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamFocusOn:streamID];

    result(nil);
}

- (void)mutePlayStreamAudio:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] mutePlayStreamAudio:mute streamID:streamID];

    result(nil);
}

- (void)muteAllPlayStreamAudio:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteAllPlayStreamAudio:mute];

    result(nil);
}

- (void)mutePlayStreamVideo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] mutePlayStreamVideo:mute streamID:streamID];

    result(nil);
}

- (void)muteAllPlayStreamVideo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteAllPlayStreamVideo:mute];

    result(nil);
}

- (void)enableHardwareDecoder:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableHardwareDecoder:enable];

    result(nil);
}

- (void)enableCheckPoc:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableCheckPoc:enable];

    result(nil);
}

- (void)isVideoDecoderSupported:(FlutterMethodCall *)call result:(FlutterResult)result {

    int codecID = [ZegoUtils intValue:call.arguments[@"codecID"]];

    BOOL isSupport = [[ZegoExpressEngine sharedEngine] isVideoDecoderSupported:(ZegoVideoCodecID)codecID];

    result(@(isSupport));
}

- (void)setPlayStreamsAlignmentProperty:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setPlayStreamsAlignmentProperty:(ZegoStreamAlignmentMode)mode];

    result(nil);
}

#pragma mark - Mixer

- (void)startMixerTask:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *taskID = call.arguments[@"taskID"];
    ZegoMixerTask *taskObject = [[ZegoMixerTask alloc] initWithTaskID:taskID];

    // MixerInput
    NSArray<NSDictionary *> *inputListMap = call.arguments[@"inputList"];
    if (inputListMap && inputListMap.count > 0) {
        NSMutableArray<ZegoMixerInput *> *inputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *inputMap in inputListMap) {
            NSString *streamID = inputMap[@"streamID"];
            int contentType = [ZegoUtils intValue:inputMap[@"contentType"]];
            int left = [ZegoUtils intValue:inputMap[@"left"]];
            int top = [ZegoUtils intValue:inputMap[@"top"]];
            int right = [ZegoUtils intValue:inputMap[@"right"]];
            int bottom = [ZegoUtils intValue:inputMap[@"bottom"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            unsigned int soundLevelID = [ZegoUtils unsignedIntValue:inputMap[@"soundLevelID"]];
            ZegoMixerInput *inputObject = [[ZegoMixerInput alloc] initWithStreamID:streamID contentType:(ZegoMixerInputContentType)contentType layout:rect soundLevelID:soundLevelID];
            [inputListObject addObject:inputObject];
            
            NSDictionary *labelMap = inputMap[@"label"];
            if (labelMap && ![ZegoUtils isNullObject:labelMap]) {
                NSString *text = labelMap[@"text"];
            
                ZegoLabelInfo *labelInfo = [[ZegoLabelInfo alloc] initWithText:text];
                labelInfo.left = [ZegoUtils intValue:labelMap[@"left"]];
                labelInfo.top = [ZegoUtils intValue:labelMap[@"top"]];
            
                NSDictionary *fontMap = labelMap[@"font"];
                ZegoFontStyle *fontStyle = [[ZegoFontStyle alloc] init];
                fontStyle.type = (ZegoFontType)[ZegoUtils intValue:fontMap[@"type"]];
                fontStyle.size = [ZegoUtils intValue:fontMap[@"size"]];
                fontStyle.color = [ZegoUtils intValue:fontMap[@"color"]];
                fontStyle.transparency = [ZegoUtils intValue:fontMap[@"transparency"]];
            
                labelInfo.font = fontStyle;
                inputObject.label = labelInfo;
            }
            
            NSNumber *renderMode = inputMap[@"renderMode"];
            if (renderMode && ![ZegoUtils isNullObject:renderMode]) {
                inputObject.renderMode = (ZegoMixRenderMode)[ZegoUtils intValue:renderMode];
            }
            
        }
        [taskObject setInputList:inputListObject];
    }

    // MixerOutput
    NSArray<NSDictionary *> *outputListMap = call.arguments[@"outputList"];
    if (outputListMap && outputListMap.count > 0) {
        NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *outputMap in outputListMap) {
            NSString *target = outputMap[@"target"];
            ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
            
            if ([[outputMap allKeys] containsObject: @"videoConfig"]) {
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                int codecID = [ZegoUtils intValue:videoConfigMap[@"videoCodecID"]];
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency];
                [outputObject setVideoConfig: videoConfig];
            }
            [outputListObject addObject:outputObject];
        }
        [taskObject setOutputList:outputListObject];
    }

    // AudioConfig
    NSDictionary *audioConfigMap = call.arguments[@"audioConfig"];
    if (audioConfigMap && audioConfigMap.count > 0) {
        int bitrate = [ZegoUtils intValue:audioConfigMap[@"bitrate"]];
        int channel = [ZegoUtils intValue:audioConfigMap[@"channel"]];
        int codecID = [ZegoUtils intValue:audioConfigMap[@"codecID"]];
        ZegoMixerAudioConfig *audioConfigObject = [[ZegoMixerAudioConfig alloc] init];
        audioConfigObject.bitrate = bitrate;
        audioConfigObject.channel = (ZegoAudioChannel)channel;
        audioConfigObject.codecID = (ZegoAudioCodecID)codecID;

        [taskObject setAudioConfig:audioConfigObject];
    }

    // VideoConfig
    NSDictionary *videoConfigMap = call.arguments[@"videoConfig"];
    if (videoConfigMap && videoConfigMap.count > 0) {
        int width = [ZegoUtils intValue:videoConfigMap[@"width"]];
        int height = [ZegoUtils intValue:videoConfigMap[@"height"]];
        int fps = [ZegoUtils intValue:videoConfigMap[@"fps"]];
        int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
        ZegoMixerVideoConfig *videoConfigObject = [[ZegoMixerVideoConfig alloc] init];
        videoConfigObject.resolution = CGSizeMake((CGFloat)width, (CGFloat)height);
        videoConfigObject.bitrate = bitrate;
        videoConfigObject.fps = fps;

        [taskObject setVideoConfig:videoConfigObject];
    }

    // Watermark
    NSDictionary *watermarkMap = call.arguments[@"watermark"];
    if (watermarkMap && watermarkMap.count > 0) {
        NSString *imageURL = watermarkMap[@"imageURL"];
        if (imageURL && [imageURL length] > 0) {
            int left = [ZegoUtils intValue:watermarkMap[@"left"]];
            int top = [ZegoUtils intValue:watermarkMap[@"top"]];
            int bottom = [ZegoUtils intValue:watermarkMap[@"bottom"]];
            int right = [ZegoUtils intValue:watermarkMap[@"right"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            ZegoWatermark *watermarkObject = [[ZegoWatermark alloc] initWithImageURL:imageURL layout:rect];

            [taskObject setWatermark:watermarkObject];
        }
    }

    // Background Image
    NSString *backgroundImageURL = call.arguments[@"backgroundImageURL"];
    if (backgroundImageURL.length > 0) {
        [taskObject setBackgroundImageURL:backgroundImageURL];
    }

    // Enable SoundLevel
    BOOL enableSoundLevel = [ZegoUtils boolValue:call.arguments[@"enableSoundLevel"]];
    [taskObject enableSoundLevel:enableSoundLevel];

    // Set AdvancedConfig
    NSDictionary<NSString *, NSString *> *advancedConfig = call.arguments[@"advancedConfig"];
    [taskObject setAdvancedConfig:advancedConfig];

    [[ZegoExpressEngine sharedEngine] startMixerTask:taskObject callback:^(int errorCode, NSDictionary * _Nullable extendedData) {

        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[startMixerTaskCallback] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        result(@{
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString
        });
    }];
}

- (void)stopMixerTask:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *taskID = call.arguments[@"taskID"];
    ZegoMixerTask *taskObject = [[ZegoMixerTask alloc] initWithTaskID:taskID];

    // MixerInput
    NSArray<NSDictionary *> *inputListMap = call.arguments[@"inputList"];
    if (inputListMap && inputListMap.count > 0) {
        NSMutableArray<ZegoMixerInput *> *inputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *inputMap in inputListMap) {
            NSString *streamID = inputMap[@"streamID"];
            int contentType = [ZegoUtils intValue:inputMap[@"contentType"]];
            int left = [ZegoUtils intValue:inputMap[@"left"]];
            int top = [ZegoUtils intValue:inputMap[@"top"]];
            int right = [ZegoUtils intValue:inputMap[@"right"]];
            int bottom = [ZegoUtils intValue:inputMap[@"bottom"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            unsigned int soundLevelID = [ZegoUtils unsignedIntValue:inputMap[@"soundLevelID"]];
            ZegoMixerInput *inputObject = [[ZegoMixerInput alloc] initWithStreamID:streamID contentType:(ZegoMixerInputContentType)contentType layout:rect soundLevelID:soundLevelID];
            [inputListObject addObject:inputObject];
        }
        [taskObject setInputList:inputListObject];
    }

    // MixerOutput
    NSArray<NSDictionary *> *outputListMap = call.arguments[@"outputList"];
    if (outputListMap && outputListMap.count > 0) {
        NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *outputMap in outputListMap) {
            NSString *target = outputMap[@"target"];
            ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
            
            if ([[outputMap allKeys] containsObject: @"videoConfig"]) {
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                int codecID = [ZegoUtils intValue:videoConfigMap[@"videoCodecID"]];
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency];
                [outputObject setVideoConfig: videoConfig];
            }
            [outputListObject addObject:outputObject];
        }
        [taskObject setOutputList:outputListObject];
    }

    // no need to set audio config

    // no need to set video config

    // no need to set watermark

    // no need to set background image

    // no need to set enable sound level

    [[ZegoExpressEngine sharedEngine] stopMixerTask:taskObject callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}

- (void)startAutoMixerTask:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSString *taskID = call.arguments[@"taskID"];
    NSString *roomID = call.arguments[@"roomID"];

    ZegoAutoMixerTask *taskObject = [[ZegoAutoMixerTask alloc] init];
    taskObject.taskID = taskID;
    taskObject.roomID = roomID;

    // MixerOutput
    NSArray<NSDictionary *> *outputListMap = call.arguments[@"outputList"];
    if (outputListMap && outputListMap.count > 0) {
        NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *outputMap in outputListMap) {
            NSString *target = outputMap[@"target"];
            ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
            
            if ([[outputMap allKeys] containsObject: @"videoConfig"]) {
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                int codecID = [ZegoUtils intValue:videoConfigMap[@"videoCodecID"]];
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency];
                [outputObject setVideoConfig: videoConfig];
            }
            [outputListObject addObject:outputObject];
        }
        taskObject.outputList = outputListObject;
    }

    // AudioConfig
    NSDictionary *audioConfigMap = call.arguments[@"audioConfig"];
    if (audioConfigMap && audioConfigMap.count > 0) {
        int bitrate = [ZegoUtils intValue:audioConfigMap[@"bitrate"]];
        int channel = [ZegoUtils intValue:audioConfigMap[@"channel"]];
        int codecID = [ZegoUtils intValue:audioConfigMap[@"codecID"]];
        ZegoMixerAudioConfig *audioConfigObject = [[ZegoMixerAudioConfig alloc] init];
        audioConfigObject.bitrate = bitrate;
        audioConfigObject.channel = (ZegoAudioChannel)channel;
        audioConfigObject.codecID = (ZegoAudioCodecID)codecID;

        taskObject.audioConfig = audioConfigObject;
    }

    // Enable SoundLevel
    BOOL enableSoundLevel = [ZegoUtils boolValue:call.arguments[@"enableSoundLevel"]];
    taskObject.enableSoundLevel = enableSoundLevel;

    [[ZegoExpressEngine sharedEngine] startAutoMixerTask:taskObject callback:^(int errorCode, NSDictionary * _Nullable extendedData) {

        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[startMixerTaskCallback] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        result(@{
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString
        });
    }];
}

- (void)stopAutoMixerTask:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *taskID = call.arguments[@"taskID"];
    NSString *roomID = call.arguments[@"roomID"];

    ZegoAutoMixerTask *taskObject = [[ZegoAutoMixerTask alloc] init];
    taskObject.taskID = taskID;
    taskObject.roomID = roomID;

    // MixerOutput
    NSArray<NSDictionary *> *outputListMap = call.arguments[@"outputList"];
    if (outputListMap && outputListMap.count > 0) {
        NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
        for (NSDictionary *outputMap in outputListMap) {
            NSString *target = outputMap[@"target"];
            ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
            
            if ([[outputMap allKeys] containsObject: @"videoConfig"]) {
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                int codecID = [ZegoUtils intValue:videoConfigMap[@"videoCodecID"]];
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency];
                [outputObject setVideoConfig: videoConfig];
            }
            [outputListObject addObject:outputObject];
        }
        [taskObject setOutputList:outputListObject];
    }

    // no need to set audio config

    // no need to set video config

    // no need to set watermark

    // no need to set background image

    // no need to set enable sound level

    [[ZegoExpressEngine sharedEngine] stopAutoMixerTask:taskObject callback:^(int errorCode) {
        result(@{@"errorCode": @(errorCode)});
    }];
}


#pragma mark - Device

- (void)muteMicrophone:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteMicrophone:mute];

    result(nil);
}

- (void)isMicrophoneMuted:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL muted = [[ZegoExpressEngine sharedEngine] isMicrophoneMuted];

    result(@(muted));
}

- (void)muteSpeaker:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteSpeaker:mute];

    result(nil);
}

- (void)isSpeakerMuted:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL muted = [[ZegoExpressEngine sharedEngine] isSpeakerMuted];

    result(@(muted));
}

- (void)enableAudioCaptureDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableAudioCaptureDevice:enable];

    result(nil);
}

- (void)setBuiltInSpeakerOn:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] setBuiltInSpeakerOn:enable];
#pragma clang diagnostic pop

    result(nil);
}

- (void)setAudioRouteToSpeaker:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL defaultToSpeaker = [ZegoUtils boolValue:call.arguments[@"defaultToSpeaker"]];

    [[ZegoExpressEngine sharedEngine] setAudioRouteToSpeaker:defaultToSpeaker];

    result(nil);
}

- (void)getAudioRouteType:(FlutterMethodCall *)call result:(FlutterResult)result {

    ZegoAudioRoute type = [[ZegoExpressEngine sharedEngine] getAudioRouteType];

    result(@(type));
}

- (void)enableCamera:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableCamera:enable channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)useFrontCamera:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] useFrontCamera:enable channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)isCameraFocusSupported:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    BOOL supported = [[ZegoExpressEngine sharedEngine] isCameraFocusSupported:(ZegoPublishChannel)channel];

    result(@(supported));
}

- (void)setCameraFocusMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int model = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraFocusMode:(ZegoCameraFocusMode)model channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setCameraFocusPointInPreview:(FlutterMethodCall *)call result:(FlutterResult)result {

    float x = [ZegoUtils floatValue:call.arguments[@"x"]];
    float y = [ZegoUtils floatValue:call.arguments[@"y"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraFocusPointInPreviewX:x y: y channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setCameraExposureMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int model = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposureMode:(ZegoCameraExposureMode)model channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setCameraExposurePointInPreview:(FlutterMethodCall *)call result:(FlutterResult)result {

    float x = [ZegoUtils floatValue:call.arguments[@"x"]];
    float y = [ZegoUtils floatValue:call.arguments[@"y"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposurePointInPreviewX:x y: y channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setCameraExposureCompensation:(FlutterMethodCall *)call result:(FlutterResult)result {

    float value = [ZegoUtils floatValue:call.arguments[@"value"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposureCompensation:value channel:channel];

    result(nil);
}

- (void)setCameraZoomFactor:(FlutterMethodCall *)call result:(FlutterResult)result {

    float factor = [ZegoUtils floatValue:call.arguments[@"factor"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraZoomFactor:factor channel:channel];

    result(nil);
}

- (void)getCameraMaxZoomFactor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    float factor = [[ZegoExpressEngine sharedEngine] getCameraMaxZoomFactor:channel];

    result(@(factor));
}

- (void)startSoundLevelMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    ZegoSoundLevelConfig *config = [[ZegoSoundLevelConfig alloc] init];

    NSDictionary *configMap = call.arguments[@"config"];
    config.millisecond = [ZegoUtils intValue:configMap[@"millisecond"]];
    config.enableVAD = [ZegoUtils boolValue:configMap[@"enableVAD"]];
    [[ZegoExpressEngine sharedEngine] startSoundLevelMonitorWithConfig:config];

    result(nil);
}

- (void)stopSoundLevelMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopSoundLevelMonitor];

    result(nil);
}

- (void)startAudioSpectrumMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int millisecond = [ZegoUtils intValue:call.arguments[@"millisecond"]];
    [[ZegoExpressEngine sharedEngine] startAudioSpectrumMonitor:millisecond];

    result(nil);
}

- (void)stopAudioSpectrumMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopAudioSpectrumMonitor];

    result(nil);
}

- (void)enableHeadphoneMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableHeadphoneMonitor:enable];

    result(nil);
}

- (void)setHeadphoneMonitorVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [[ZegoExpressEngine sharedEngine] setHeadphoneMonitorVolume:volume];

    result(nil);
}

- (void)startAudioVADStableStateMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];

    if ([ZegoUtils isNullObject:call.arguments[@"millisecond"]]) {
        [[ZegoExpressEngine sharedEngine] startAudioVADStableStateMonitor:(ZegoAudioVADStableStateMonitorType)type];
    } else {
        int millisecond = [ZegoUtils intValue:call.arguments[@"millisecond"]];
        [[ZegoExpressEngine sharedEngine] startAudioVADStableStateMonitor:(ZegoAudioVADStableStateMonitorType)type millisecond:millisecond];
    }

    result(nil);
}

- (void)stopAudioVADStableStateMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];

    [[ZegoExpressEngine sharedEngine] stopAudioVADStableStateMonitor:(ZegoAudioVADStableStateMonitorType)type];

    result(nil);
}


#pragma mark - Preprocess

- (void)enableAEC:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableAEC:enable];

    result(nil);
}

- (void)enableHeadphoneAEC:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableHeadphoneAEC:enable];

    result(nil);
}

- (void)setAECMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setAECMode:(ZegoAECMode)mode];

    result(nil);
}

- (void)enableAGC:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableAGC:enable];

    result(nil);
}

- (void)enableANS:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableANS:enable];

    result(nil);
}

- (void)enableTransientANS:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableTransientANS:enable];

    result(nil);
}

- (void)setANSMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setANSMode:(ZegoANSMode)mode];

    result(nil);
}

- (void)startEffectsEnv:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] startEffectsEnv];

    result(nil);
}

- (void)stopEffectsEnv:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopEffectsEnv];

    result(nil);
}

- (void)enableEffectsBeauty:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [[ZegoExpressEngine sharedEngine] enableEffectsBeauty: enable];
    
    result(nil);
}

- (void)setEffectsBeautyParam:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *param = call.arguments[@"param"];
    ZegoEffectsBeautyParam *paramObject = [[ZegoEffectsBeautyParam alloc] init];
    paramObject.rosyIntensity = [ZegoUtils intValue:param[@"rosyIntensity"]];
    paramObject.sharpenIntensity = [ZegoUtils intValue:param[@"sharpenIntensity"]];
    paramObject.smoothIntensity = [ZegoUtils intValue:param[@"smoothIntensity"]];
    paramObject.whitenIntensity = [ZegoUtils intValue:param[@"whitenIntensity"]];

    [[ZegoExpressEngine sharedEngine] setEffectsBeautyParam: paramObject];

    result(nil);
}

- (void)enableBeautify:(FlutterMethodCall *)call result:(FlutterResult)result {

    int feature = [ZegoUtils intValue:call.arguments[@"featureBitmask"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableBeautify:(ZegoBeautifyFeature)feature channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setBeautifyOption:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *option = call.arguments[@"option"];
    ZegoBeautifyOption *optionObject = [[ZegoBeautifyOption alloc] init];
    optionObject.polishStep = [ZegoUtils doubleValue:option[@"polishStep"]];
    optionObject.whitenFactor = [ZegoUtils doubleValue:option[@"whitenFactor"]];
    optionObject.sharpenFactor = [ZegoUtils doubleValue:option[@"sharpenFactor"]];

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setBeautifyOption: optionObject channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setAudioEqualizerGain:(FlutterMethodCall *)call result:(FlutterResult)result {

    int bandIndex = [ZegoUtils intValue:call.arguments[@"bandIndex"]];
    float bandGain = [ZegoUtils floatValue:call.arguments[@"bandGain"]];

    [[ZegoExpressEngine sharedEngine] setAudioEqualizerGain:bandIndex bandGain:bandGain];

    result(nil);
}

- (void)setVoiceChangerPreset:(FlutterMethodCall *)call result:(FlutterResult)result {

    int preset = [ZegoUtils intValue:call.arguments[@"preset"]];

    [[ZegoExpressEngine sharedEngine] setVoiceChangerPreset:(ZegoVoiceChangerPreset)preset];

    result(nil);
}

- (void)setVoiceChangerParam:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *paramMap = call.arguments[@"param"];
    ZegoVoiceChangerParam *param = [[ZegoVoiceChangerParam alloc] init];
    param.pitch = [ZegoUtils floatValue:paramMap[@"pitch"]];

    [[ZegoExpressEngine sharedEngine] setVoiceChangerParam:param];

    result(nil);
}

- (void)setReverbPreset:(FlutterMethodCall *)call result:(FlutterResult)result {

    int preset = [ZegoUtils intValue:call.arguments[@"preset"]];

    [[ZegoExpressEngine sharedEngine] setReverbPreset:(ZegoReverbPreset)preset];

    result(nil);
}

- (void)setReverbAdvancedParam:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *paramMap = call.arguments[@"param"];
    ZegoReverbAdvancedParam *param = [[ZegoReverbAdvancedParam alloc] init];
    param.roomSize = [ZegoUtils floatValue:paramMap[@"roomSize"]];
    param.reverberance = [ZegoUtils floatValue:paramMap[@"reverberance"]];
    param.damping = [ZegoUtils floatValue:paramMap[@"damping"]];
    param.wetOnly = [ZegoUtils boolValue:paramMap[@"wetOnly"]];
    param.wetGain = [ZegoUtils floatValue:paramMap[@"wetGain"]];
    param.dryGain = [ZegoUtils floatValue:paramMap[@"dryGain"]];
    param.toneLow = [ZegoUtils floatValue:paramMap[@"toneLow"]];
    param.toneHigh = [ZegoUtils floatValue:paramMap[@"toneHigh"]];
    param.preDelay = [ZegoUtils floatValue:paramMap[@"preDelay"]];
    param.stereoWidth = [ZegoUtils floatValue:paramMap[@"stereoWidth"]];

    [[ZegoExpressEngine sharedEngine] setReverbAdvancedParam:param];

    result(nil);
}

- (void)setReverbEchoParam:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoReverbEchoParam *param = [[ZegoReverbEchoParam alloc] init];
    param.inGain = [ZegoUtils floatValue:paramMap[@"inGain"]];
    param.outGain = [ZegoUtils floatValue:paramMap[@"outGain"]];
    param.numDelays = [ZegoUtils intValue:paramMap[@"numDelays"]];
    param.delay = paramMap[@"delay"];
    param.decay = paramMap[@"decay"];

    [[ZegoExpressEngine sharedEngine] setReverbEchoParam:param];

    result(nil);
}

- (void)enableVirtualStereo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int angle = [ZegoUtils intValue:call.arguments[@"angle"]];

    [[ZegoExpressEngine sharedEngine] enableVirtualStereo:enable angle:angle];

    result(nil);
}

- (void)enablePlayStreamVirtualStereo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int angle = [ZegoUtils intValue:call.arguments[@"angle"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] enablePlayStreamVirtualStereo:enable angle:angle streamID:streamID];

    result(nil);
}

- (void)setElectronicEffects:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    int tonal = [ZegoUtils intValue:call.arguments[@"tonal"]];

    [[ZegoExpressEngine sharedEngine] setElectronicEffects:enable mode:(ZegoElectronicEffectsMode)mode tonal:tonal];

    result(nil);
}

#pragma mark - Custom Audio IO

- (ZegoAudioSampleRate)convertAudioSampleRate:(int)sampleRateIndex {
    switch (sampleRateIndex) {
        case 0:
            return ZegoAudioSampleRateUnknown;
        case 1:
            return ZegoAudioSampleRate8K;
        case 2:
            return ZegoAudioSampleRate16K;
        case 3:
            return ZegoAudioSampleRate22K;
        case 4:
            return ZegoAudioSampleRate24K;
        case 5:
            return ZegoAudioSampleRate32K;
        case 6:
            return ZegoAudioSampleRate44K;
        case 7:
            return ZegoAudioSampleRate48K;
    }
    return ZegoAudioSampleRateUnknown;
}

- (void)enableCustomAudioCaptureProcessing:(FlutterMethodCall *)call result:(FlutterResult)result {
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoCustomAudioProcessConfig *config = [[ZegoCustomAudioProcessConfig alloc] init];
    config.sampleRate = [ZegoUtils intValue:configMap[@"sampleRate"]];
    config.samples = [ZegoUtils intValue:configMap[@"samples"]];
    config.channel = [ZegoUtils intValue:configMap[@"channel"]];
    
    [[ZegoExpressEngine sharedEngine] enableCustomAudioCaptureProcessing:enable config:config];
    
    result(nil);
}

- (void)enableCustomAudioCaptureProcessingAfterHeadphoneMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoCustomAudioProcessConfig *config = [[ZegoCustomAudioProcessConfig alloc] init];
    config.sampleRate = [ZegoUtils intValue:configMap[@"sampleRate"]];
    config.samples = [ZegoUtils intValue:configMap[@"samples"]];
    config.channel = [ZegoUtils intValue:configMap[@"channel"]];
    
    [[ZegoExpressEngine sharedEngine] enableCustomAudioCaptureProcessingAfterHeadphoneMonitor:enable config:config];
    
    result(nil);
}

- (void)enableCustomAudioRemoteProcessing:(FlutterMethodCall *)call result:(FlutterResult)result {
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoCustomAudioProcessConfig *config = [[ZegoCustomAudioProcessConfig alloc] init];
    config.sampleRate = [ZegoUtils intValue:configMap[@"sampleRate"]];
    config.samples = [ZegoUtils intValue:configMap[@"samples"]];
    config.channel = [ZegoUtils intValue:configMap[@"channel"]];
    
    [[ZegoExpressEngine sharedEngine] enableCustomAudioRemoteProcessing:enable config:config];
    
    result(nil);
}

- (void)enableCustomAudioPlaybackProcessing:(FlutterMethodCall *)call result:(FlutterResult)result {
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoCustomAudioProcessConfig *config = [[ZegoCustomAudioProcessConfig alloc] init];
    config.sampleRate = [ZegoUtils intValue:configMap[@"sampleRate"]];
    config.samples = [ZegoUtils intValue:configMap[@"samples"]];
    config.channel = [ZegoUtils intValue:configMap[@"channel"]];
    
    [[ZegoExpressEngine sharedEngine] enableCustomAudioPlaybackProcessing:enable config:config];
    
    result(nil);
}

- (void)startAudioDataObserver:(FlutterMethodCall *)call result:(FlutterResult)result {
    int bitmask = [ZegoUtils intValue:call.arguments[@"observerBitMask"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [self convertAudioSampleRate:[ZegoUtils intValue:paramMap[@"sampleRate"]]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    [[ZegoExpressEngine sharedEngine] startAudioDataObserver:bitmask param:param];

    result(nil);
}

- (void)stopAudioDataObserver:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopAudioDataObserver];

    result(nil);
}

- (void)enableCustomAudioIO:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];

    ZegoCustomAudioConfig *config = [[ZegoCustomAudioConfig alloc] init];
    config.sourceType = [ZegoUtils intValue:configMap[@"sourceType"]];
    
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableCustomAudioIO:enable config:config channel:channel];

    result(nil);
}

- (void)sendCustomAudioCaptureAACData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    int configLength = [ZegoUtils intValue:call.arguments[@"configLength"]];
    int referenceTimeMillisecond = [ZegoUtils intValue:call.arguments[@"referenceTimeMillisecond"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [self convertAudioSampleRate:[ZegoUtils intValue:paramMap[@"sampleRate"]]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    CMTime timestamp = CMTimeMakeWithSeconds(referenceTimeMillisecond, 1000);
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] sendCustomAudioCaptureAACData:(unsigned char*)[data.data bytes] dataLength:dataLength configLength:configLength timestamp:timestamp param:param channel:channel];

    result(nil);
}

- (void)sendCustomAudioCapturePCMData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [self convertAudioSampleRate:[ZegoUtils intValue:paramMap[@"sampleRate"]]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];


    [[ZegoExpressEngine sharedEngine] sendCustomAudioCapturePCMData:(unsigned char*)[data.data bytes] dataLength:dataLength param:param channel:channel];

    result(nil);
}

- (void)fetchCustomAudioRenderPCMData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [self convertAudioSampleRate:[ZegoUtils intValue:paramMap[@"sampleRate"]]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    [[ZegoExpressEngine sharedEngine] fetchCustomAudioRenderPCMData:(unsigned char*)[data.data bytes] dataLength:dataLength param:param];

    result(nil);
}

- (void)sendReferenceAudioPCMData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [self convertAudioSampleRate:[ZegoUtils intValue:paramMap[@"sampleRate"]]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    [[ZegoExpressEngine sharedEngine] sendReferenceAudioPCMData:(unsigned char*)[data.data bytes] dataLength:dataLength param:param];

    result(nil);
}


#pragma mark - IM

- (void)sendBroadcastMessage:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSString *message = call.arguments[@"message"];

    [[ZegoExpressEngine sharedEngine] sendBroadcastMessage:message roomID:roomID callback:^(int errorCode, unsigned long long messageID) {
        result(@{
            @"errorCode": @(errorCode),
            @"messageID": @(messageID)
        });
    }];
}

- (void)sendBarrageMessage:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSString *message = call.arguments[@"message"];

    [[ZegoExpressEngine sharedEngine] sendBarrageMessage:message roomID:roomID callback:^(int errorCode, NSString * _Nonnull messageID) {
        result(@{
            @"errorCode": @(errorCode),
            @"messageID": messageID
        });
    }];
}

- (void)sendCustomCommand:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    NSString *command = call.arguments[@"command"];
    NSArray<NSDictionary *> *userListMap = call.arguments[@"toUserList"];

    NSMutableArray<ZegoUser *> *userListObject = nil;
    if (userListMap && userListMap.count > 0) {
        userListObject = [[NSMutableArray alloc] init];
        for(NSDictionary *userMap in userListMap) {
            ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:userMap[@"userID"] userName:userMap[@"userName"]];
            [userListObject addObject:userObject];
        }
    }

    [[ZegoExpressEngine sharedEngine] sendCustomCommand:command toUserList:userListObject roomID:roomID callback:^(int errorCode) {
        result(@{
            @"errorCode": @(errorCode)
        });
    }];
}


#pragma mark - CustomVideoCapture

- (void)enableCustomVideoCapture:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    ZegoCustomVideoCaptureConfig *config = [[ZegoCustomVideoCaptureConfig alloc] init];

    if (configMap && configMap.count > 0) {
        ZegoVideoBufferType bufferType = (ZegoVideoBufferType)[ZegoUtils intValue:configMap[@"bufferType"]];
        config.bufferType = bufferType;
    } else {
        // If `config` is empty, set the default configuration (pixel buffer for iOS)
        config.bufferType = ZegoVideoBufferTypeCVPixelBuffer;
    }

    [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureHandler:(id<ZegoCustomVideoCaptureHandler>)[ZegoCustomVideoCaptureManager sharedInstance]];
    [[ZegoExpressEngine sharedEngine] enableCustomVideoCapture:enable config:config channel:(ZegoPublishChannel)channel];

    // When using custom video capture, turn off preview mirroring
    if (enable) {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:ZegoVideoMirrorModeNoMirror channel:(ZegoPublishChannel)channel];
    } else {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:ZegoVideoMirrorModeOnlyPreviewMirror channel:(ZegoPublishChannel)channel];
    }
}


#pragma mark - MediaPlayer

- (void)createMediaPlayer:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.mediaPlayerMap) {
        self.mediaPlayerMap = [NSMutableDictionary dictionary];
    }

    ZegoMediaPlayer *mediaPlayer = [[ZegoExpressEngine sharedEngine] createMediaPlayer];

    if (mediaPlayer) {
        NSNumber *index = mediaPlayer.index;

        [mediaPlayer setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.mediaPlayerMap[index] = mediaPlayer;

        result(index);
    } else {
        result(@(-1));
    }
}

- (void)destroyMediaPlayer:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        [[ZegoExpressEngine sharedEngine] destroyMediaPlayer:mediaPlayer];
    }

    [self.mediaPlayerMap removeObjectForKey:index];

    result(nil);
}

- (void)mediaPlayerLoadResource:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        NSString *path = call.arguments[@"path"];

        [mediaPlayer loadResource:path callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    }
}

- (void)mediaPlayerLoadResourceFromMediaData:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        FlutterStandardTypedData *mediaData = call.arguments[@"mediaData"];
        int startPosition = [ZegoUtils intValue:call.arguments[@"startPosition"]];

        [mediaPlayer loadResourceFromMediaData:mediaData.data startPosition:startPosition callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    }
}

- (void)mediaPlayerLoadResourceWithPosition:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        NSString *path = call.arguments[@"path"];
        int startPosition = [ZegoUtils intValue:call.arguments[@"startPosition"]];

        [mediaPlayer loadResourceWithPosition:path startPosition:startPosition callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    }
}

- (void)mediaPlayerStart:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    [mediaPlayer start];

    result(nil);
}

- (void)mediaPlayerStop:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    [mediaPlayer stop];

    result(nil);
}

- (void)mediaPlayerPause:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    [mediaPlayer pause];

    result(nil);
}

- (void)mediaPlayerResume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    [mediaPlayer resume];

    result(nil);
}

- (void)mediaPlayerSeekTo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        unsigned long long millisecond = [ZegoUtils unsignedLongLongValue:call.arguments[@"millisecond"]];

        [mediaPlayer seekTo:millisecond callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    }
}

- (void)mediaPlayerEnableRepeat:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [mediaPlayer enableRepeat:enable];

    result(nil);
}

- (void)mediaPlayerEnableAux:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    [mediaPlayer enableAux:enable];

    result(nil);
}

- (void)mediaPlayerMuteLocal:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [mediaPlayer muteLocal:mute];

    result(nil);
}

- (void)mediaPlayerSetPlayerCanvas:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    NSDictionary *canvasMap = call.arguments[@"canvas"];

    if (canvasMap && canvasMap.count > 0) {
        // media video

        // This parameter is actually viewID when using PlatformView
        int64_t viewID = [ZegoUtils longLongValue:canvasMap[@"view"]];
        int viewMode = [ZegoUtils intValue:canvasMap[@"viewMode"]];
        int backgroundColor = [ZegoUtils intValue:canvasMap[@"backgroundColor"]];

        // Render with PlatformView
        ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];

        if (platformView) {
            ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:[platformView getUIView]];
            canvas.viewMode = (ZegoViewMode)viewMode;
            canvas.backgroundColor = backgroundColor;

            [mediaPlayer setPlayerCanvas:canvas];
        } else {
            // media video without creating the PlatformView in advance
            // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
            NSString *errorMessage = [NSString stringWithFormat:@"The PlatformView for viewID:%ld cannot be found, developer should call `createPlatformView` first and get the viewID", (long)viewID];
            ZGError(@"[mediaPlayerSetPlayerCanvas] %@", errorMessage);
            result([FlutterError errorWithCode:[@"setPlayerCanvas_No_PlatformView" uppercaseString] message:errorMessage details:nil]);
            return;
        }

    }

    result(nil);
}

- (void)mediaPlayerSetVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [mediaPlayer setVolume:volume];

    result(nil);
}

- (void)mediaPlayerSetPlayVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [mediaPlayer setPlayVolume:volume];

    result(nil);
}

- (void)mediaPlayerSetPublishVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

    [mediaPlayer setPublishVolume:volume];

    result(nil);
}

- (void)mediaPlayerSetProgressInterval:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    unsigned long long millisecond = [ZegoUtils unsignedLongLongValue:call.arguments[@"millisecond"]];

    [mediaPlayer setProgressInterval:millisecond];

    result(nil);
}

- (void)mediaPlayerGetPlayVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        int volume = mediaPlayer.playVolume;
        result(@(volume));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerGetPublishVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        int volume = mediaPlayer.publishVolume;
        result(@(volume));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerGetTotalDuration:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        unsigned long long totalDuration = mediaPlayer.totalDuration;
        result(@(totalDuration));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerGetCurrentProgress:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        unsigned long long currentProgress = mediaPlayer.currentProgress;
        result(@(currentProgress));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerGetAudioTrackCount:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        unsigned int audioTrackCount = mediaPlayer.audioTrackCount;
        result(@(audioTrackCount));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerSetAudioTrackIndex:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    unsigned int trackIndex = [ZegoUtils unsignedIntValue:call.arguments[@"trackIndex"]];

    [mediaPlayer setAudioTrackIndex:trackIndex];

    result(nil);
}

- (void)mediaPlayerSetVoiceChangerParam:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    NSDictionary *paramMap = call.arguments[@"param"];
    ZegoVoiceChangerParam *param = [[ZegoVoiceChangerParam alloc] init];
    param.pitch = [ZegoUtils floatValue:paramMap[@"pitch"]];

    int audioChannel = [ZegoUtils intValue:call.arguments[@"audioChannel"]];

    [mediaPlayer setVoiceChangerParam:param audioChannel:audioChannel];

    result(nil);
}

- (void)mediaPlayerGetCurrentState:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        ZegoMediaPlayerState currentState = mediaPlayer.currentState;
        result(@(currentState));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerEnableAccurateSeek:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    ZegoAccurateSeekConfig *config = [[ZegoAccurateSeekConfig alloc] init];
    config.timeout = [ZegoUtils unsignedLongLongValue:configMap[@"timeout"]];

    [mediaPlayer enableAccurateSeek:enable config:config];

    result(nil);
}

- (void)mediaPlayerSetNetWorkBufferThreshold:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    int threshold = [ZegoUtils intValue:call.arguments[@"threshold"]];

    [mediaPlayer setNetWorkBufferThreshold:threshold];

    result(nil);
}

- (void)mediaPlayerSetNetWorkResourceMaxCache:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    int time = [ZegoUtils intValue:call.arguments[@"time"]];
    int size = [ZegoUtils intValue:call.arguments[@"size"]];

    [mediaPlayer setNetWorkResourceMaxCache:time size:size];

    result(nil);
}

- (void)mediaPlayerSetPlaySpeed:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    float speed = [ZegoUtils floatValue:call.arguments[@"speed"]];

    [mediaPlayer setPlaySpeed:speed];

    result(nil);
}

- (void)mediaPlayerGetNetWorkResourceCache:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        ZegoNetWorkResourceCache *networkResourceCache = mediaPlayer.getNetWorkResourceCache;
        result(@{
            @"time": @(networkResourceCache.time),
            @"size": @(networkResourceCache.size)
        });
    } else {
        result(@{});
    }
}

- (void)mediaPlayerTakeSnapshot:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        [mediaPlayer takeSnapshot:^(int errorCode, ZGImage * _Nullable image) {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                NSData *imageData = nil;
                if (image) {
                    imageData = UIImageJPEGRepresentation(image, 1);
                }

                dispatch_async(dispatch_get_main_queue(), ^{
                    result(@{
                        @"errorCode": @(errorCode),
                        @"image": imageData ?: [[NSNull alloc] init]
                    });
                });
            });
        }];
    }
}

- (void)mediaPlayerEnableFrequencySpectrumMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        int millisecond = [ZegoUtils intValue:call.arguments[@"millisecond"]];
        [mediaPlayer enableFrequencySpectrumMonitor: enable millisecond: millisecond];
    }

    result(nil);
}

- (void)mediaPlayerEnableSoundLevelMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        int millisecond = [ZegoUtils intValue:call.arguments[@"millisecond"]];
        [mediaPlayer enableSoundLevelMonitor: enable millisecond: millisecond];
    }

    result(nil);
}

#pragma mark - AudioEffectPlayer

- (void)createAudioEffectPlayer:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.audioEffectPlayerMap) {
        self.audioEffectPlayerMap = [NSMutableDictionary dictionary];
    }

    ZegoAudioEffectPlayer *audioEffectPlayer = [[ZegoExpressEngine sharedEngine] createAudioEffectPlayer];

    if (audioEffectPlayer) {
        NSNumber *index = [audioEffectPlayer getIndex];

        [audioEffectPlayer setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.audioEffectPlayerMap[index] = audioEffectPlayer;

        result(index);

    } else {
        result(@(-1));
    }
}

- (void)destroyAudioEffectPlayer:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        [[ZegoExpressEngine sharedEngine] destroyAudioEffectPlayer:audioEffectPlayer];
        [self.audioEffectPlayerMap removeObjectForKey:index];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"destroyAudioEffectPlayer_Can_not_find_player" uppercaseString] message:@"Invoke `destroyAudioEffectPlayer` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerStart:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        NSString *path = call.arguments[@"path"];

        NSDictionary *configMap = call.arguments[@"config"];
        ZegoAudioEffectPlayConfig *configObject = nil;

        if (configMap && configMap.count > 0) {
            configObject = [[ZegoAudioEffectPlayConfig alloc] init];
            configObject.playCount = [ZegoUtils unsignedIntValue:configMap[@"playCount"]];
            configObject.isPublishOut = [ZegoUtils boolValue:configMap[@"isPublishOut"]];;
        }

        [audioEffectPlayer start:audioEffectID path:path config:configObject];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerStart_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerStart` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerStop:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        [audioEffectPlayer stop:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerStop_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerStop` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerPause:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        [audioEffectPlayer pause:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerPause_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerPause` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerResume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        [audioEffectPlayer resume:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerResume_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerResume` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerStopAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        [audioEffectPlayer stopAll];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerStopAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerStopAll` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerPauseAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        [audioEffectPlayer pauseAll];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerPauseAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerPauseAll` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerResumeAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        [audioEffectPlayer resumeAll];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerResumeAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerResumeAll` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSeekTo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        unsigned long long millisecond = [ZegoUtils unsignedLongLongValue:call.arguments[@"millisecond"]];
        [audioEffectPlayer seekTo:millisecond audioEffectID:audioEffectID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSeekTo_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSeekTo` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSetVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setVolume:volume audioEffectID:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetVolume_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetVolume` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSetVolumeAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setVolumeAll:volume];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetVolumeAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerGetTotalDuration:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        unsigned long long totalDuration = [audioEffectPlayer getTotalDuration:audioEffectID];
        result(@(totalDuration));

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerGetTotalDuration_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerGetCurrentProgress:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        unsigned long long currentProgress = [audioEffectPlayer getCurrentProgress:audioEffectID];
        result(@(currentProgress));

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerGetCurrentProgress_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerLoadResource:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        NSString *path = call.arguments[@"path"];

        [audioEffectPlayer loadResource:path audioEffectID:audioEffectID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerLoadResource_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerLoadResource` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerUnloadResource:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        [audioEffectPlayer unloadResource:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerUnloadResource_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerUnloadResource` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSetPlaySpeed:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        float speed = [ZegoUtils floatValue:call.arguments[@"speed"]];
        [audioEffectPlayer setPlaySpeed: speed audioEffectID: audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetPlaySpeed_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player" details:nil]);
    }
}


#pragma mark - Record

- (void)startRecordingCapturedData:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];

    ZegoDataRecordConfig *config = [[ZegoDataRecordConfig alloc] init];
    config.filePath = configMap[@"filePath"];
    config.recordType = (ZegoDataRecordType)[ZegoUtils intValue:configMap[@"recordType"]];

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] startRecordingCapturedData:config channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)stopRecordingCapturedData:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] stopRecordingCapturedData:(ZegoPublishChannel)channel];

    result(nil);
}

#pragma mark - Range Audio
- (void)createRangeAudio:(FlutterMethodCall *)call result:(FlutterResult)result {

    ZegoRangeAudio *rangeAudio = [[ZegoExpressEngine sharedEngine] createRangeAudio];

    if (rangeAudio) {
        self.rangeAudioInstance = rangeAudio;
        [self.rangeAudioInstance setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        result(@(0));
    } else {
        result(@(-1));
    }
}

- (void)destroyRangeAudio:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        [[ZegoExpressEngine sharedEngine] destroyRangeAudio:self.rangeAudioInstance];
        [self.rangeAudioInstance setEventHandler:nil];
        self.rangeAudioInstance = nil;
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `destroyRangeAudio` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioEnableSpatializer:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        [self.rangeAudioInstance enableSpatializer:enable];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioEnableSpatializer` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioEnableMicrophone:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        [self.rangeAudioInstance enableMicrophone:enable];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioEnableMicrophone` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioEnableSpeaker:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        [self.rangeAudioInstance enableSpeaker:enable];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioEnableSpeaker` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetAudioReceiveRange:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        float range = [ZegoUtils floatValue:call.arguments[@"range"]];
        [self.rangeAudioInstance setAudioReceiveRange:range];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
        [self.rangeAudioInstance setRangeAudioMode:(ZegoRangeAudioMode)mode];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetMode` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetTeamID:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSString *teamID = call.arguments[@"teamID"];
        [self.rangeAudioInstance setTeamID:teamID];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetTeamID` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioUpdateAudioSource:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSString *userID = call.arguments[@"userID"];
        NSArray<NSNumber *> *positionArray = call.arguments[@"position"];
        float position[3];
        convertFloatArray(position, positionArray);
        [self.rangeAudioInstance updateAudioSource:userID position:position];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetTeamID` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioUpdateSelfPosition:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSArray<NSNumber *> *positionArray = call.arguments[@"position"];
        NSArray<NSNumber *> *axisForwardArray = call.arguments[@"axisForward"];
        NSArray<NSNumber *> *axisRightArray = call.arguments[@"axisRight"];
        NSArray<NSNumber *> *axisUpArray = call.arguments[@"axisUp"];
        float position[3];
        float axisForward[3];
        float axisRight[3];
        float axisUp[3];
        convertFloatArray(position, positionArray);
        convertFloatArray(axisForward, axisForwardArray);
        convertFloatArray(axisRight, axisRightArray);
        convertFloatArray(axisUp, axisUpArray);
        [self.rangeAudioInstance updateSelfPosition:position axisForward:axisForward axisRight:axisRight axisUp:axisUp];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioUpdateSelfPosition` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioMuteUser:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSString *userID = call.arguments[@"userID"];
        BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

        [self.rangeAudioInstance muteUser:userID mute:mute];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioMuteUser` but can't find specific instance" details:nil]);
    }
}

#pragma mark - real time sequential data manager

- (void)createRealTimeSequentialDataManager:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.realTimeSequentialDataManagerMap) {
        self.realTimeSequentialDataManagerMap = [NSMutableDictionary dictionary];
    }

    NSString *roomID = call.arguments[@"roomID"];

    ZegoRealTimeSequentialDataManager *manager = [[ZegoExpressEngine sharedEngine] createRealTimeSequentialDataManager:roomID];

    if (manager) {
        NSNumber *index = [manager getIndex];

        [manager setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.realTimeSequentialDataManagerMap[index] = manager;

        result(index);
    } else {
        result(@(-1));
    }
    
}

- (void)destroyRealTimeSequentialDataManager:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        [[ZegoExpressEngine sharedEngine] destroyRealTimeSequentialDataManager:manager];       
    }

    [self.realTimeSequentialDataManagerMap removeObjectForKey:index];
    result(nil);
}

- (void)dataManagerSendRealTimeSequentialData:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        NSString *streamID = call.arguments[@"streamID"];
        FlutterStandardTypedData *data = call.arguments[@"data"];

        [manager sendRealTimeSequentialData:data.data streamID: streamID callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];     

    } else {
        result([FlutterError errorWithCode:[@"dataManagerSendRealTimeSequentialData_Can_not_find_manager" uppercaseString] message:@"Invoke `dataManagerSendRealTimeSequentialData` but can't find data manager" details:nil]);
    }
    
}

- (void)dataManagerStartBroadcasting:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        NSString *streamID = call.arguments[@"streamID"];
        [manager startBroadcasting:streamID];
        result(nil);    

    } else {
        result([FlutterError errorWithCode:[@"dataManagerStartBroadcasting_Can_not_find_manager" uppercaseString] message:@"Invoke `dataManagerStartBroadcasting` but can't find data manager" details:nil]);
    }
}

- (void)dataManagerStartSubscribing:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        NSString *streamID = call.arguments[@"streamID"];
        [manager startSubscribing:streamID];
        result(nil);    

    } else {
        result([FlutterError errorWithCode:[@"dataManagerStartSubscribing_Can_not_find_manager" uppercaseString] message:@"Invoke `dataManagerStartSubscribing` but can't find data manager" details:nil]);
    }
}

- (void)dataManagerStopBroadcasting:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        NSString *streamID = call.arguments[@"streamID"];
        [manager stopBroadcasting:streamID];
        result(nil);    

    } else {
        result([FlutterError errorWithCode:[@"dataManagerStopBroadcasting_Can_not_find_manager" uppercaseString] message:@"Invoke `dataManagerStopBroadcasting` but can't find data manager" details:nil]);
    }
}

- (void)dataManagerStopSubscribing:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];

    ZegoRealTimeSequentialDataManager *manager = self.realTimeSequentialDataManagerMap[index];

    if (manager) {
        NSString *streamID = call.arguments[@"streamID"];
        [manager stopSubscribing:streamID];
        result(nil);    

    } else {
        result([FlutterError errorWithCode:[@"dataManagerStopSubscribing_Can_not_find_manager" uppercaseString] message:@"Invoke `dataManagerStopSubscribing` but can't find data manager" details:nil]);
    }
}

#pragma mark - Utilities

- (void)startPerformanceMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int millisecond = [ZegoUtils intValue:call.arguments[@"millisecond"]];

    [[ZegoExpressEngine sharedEngine] startPerformanceMonitor:millisecond];

    result(nil);
}

- (void)stopPerformanceMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopPerformanceMonitor];

    result(nil);
}

- (void)startNetworkProbe:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];

    ZegoNetworkProbeConfig *config = [[ZegoNetworkProbeConfig alloc] init];
    config.enableTraceroute = [ZegoUtils boolValue:configMap[@"enableTraceroute"]];

    [[ZegoExpressEngine sharedEngine] startNetworkProbe:config callback:^(int errorCode, ZegoNetworkProbeResult * _Nonnull probeResult) {

        NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];

        if (probeResult.httpProbeResult) {
            NSDictionary *httpProbeResultMap = @{
                @"errorCode": @(probeResult.httpProbeResult.errorCode),
                @"requestCostTime": @(probeResult.httpProbeResult.requestCostTime)
            };
            resultMap[@"httpProbeResult"] = httpProbeResultMap;
        }
        
        if (probeResult.tcpProbeResult) {
            NSDictionary *tcpProbeResultMap = @{
                @"errorCode": @(probeResult.tcpProbeResult.errorCode),
                @"rtt": @(probeResult.tcpProbeResult.rtt),
                @"connectCostTime": @(probeResult.tcpProbeResult.connectCostTime),
            };
            resultMap[@"tcpProbeResult"] = tcpProbeResultMap;
        }

        if (probeResult.udpProbeResult) {
            NSDictionary *udpProbeResultMap = @{
                @"errorCode": @(probeResult.udpProbeResult.errorCode),
                @"rtt": @(probeResult.udpProbeResult.rtt)
            };
            resultMap[@"udpProbeResult"] = udpProbeResultMap;
        }

        if (probeResult.tracerouteResult) {
            NSDictionary *tracerouteResultMap = @{
                @"errorCode": @(probeResult.tracerouteResult.errorCode),
                @"tracerouteCostTime": @(probeResult.tracerouteResult.tracerouteCostTime)
            };
            resultMap[@"tracerouteResult"] = tracerouteResultMap;
        }
        
        result(resultMap);
        
    }];
}

- (void)stopNetworkProbe:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopNetworkProbe];

    result(nil);
}

- (void)startNetworkSpeedTest:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoNetworkSpeedTestConfig *config = [[ZegoNetworkSpeedTestConfig alloc] init];
    config.testUplink = [ZegoUtils boolValue:configMap[@"testUplink"]];
    config.expectedUplinkBitrate = [ZegoUtils intValue:configMap[@"expectedUplinkBitrate"]];
    config.testDownlink = [ZegoUtils boolValue:configMap[@"testDownlink"]];
    config.expectedDownlinkBitrate = [ZegoUtils intValue:configMap[@"expectedDownlinkBitrate"]];

    [[ZegoExpressEngine sharedEngine] startNetworkSpeedTest:config];

    result(nil);
}

- (void)stopNetworkSpeedTest:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] stopNetworkSpeedTest];

    result(nil);
}

- (void)getNetworkTimeInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    ZegoNetworkTimeInfo *info = [[ZegoExpressEngine sharedEngine] getNetworkTimeInfo];
    
    result(@{
        @"timestamp": @(info.timestamp),
        @"maxDeviation": @(info.maxDeviation)
    });
}


#pragma mark - PlatformView Utils

- (void)destroyPlatformView:(FlutterMethodCall *)call result:(FlutterResult)result {

    int viewID = [ZegoUtils intValue:call.arguments[@"viewID"]];
    BOOL state = [[ZegoPlatformViewFactory sharedInstance] destroyPlatformView:@(viewID)];

    ZGLog(@"[destroyPlatformView] viewID: %d, success: %@", viewID, state ? @"true" : @"false");

    result(@(state));
}


#pragma mark - TextureRenderer Utils

- (void)createTextureRenderer:(FlutterMethodCall *)call result:(FlutterResult)result {

    int viewWidth = [ZegoUtils intValue:call.arguments[@"width"]];
    int viewHeight = [ZegoUtils intValue:call.arguments[@"height"]];

    int64_t textureID = [[ZegoTextureRendererController sharedInstance] createTextureRenderer:[_registrar textures] viewWidth:viewWidth viewHeight:viewHeight];

    ZGLog(@"[createTextureRenderer][Result] w: %d, h: %d, textureID: %ld", viewWidth, viewHeight, (long)textureID);

    result(@(textureID));
}

- (void)updateTextureRendererSize:(FlutterMethodCall *)call result:(FlutterResult)result {

    int64_t textureID = [ZegoUtils longLongValue:call.arguments[@"textureID"]];
    int viewWidth = [ZegoUtils intValue:call.arguments[@"width"]];
    int viewHeight = [ZegoUtils intValue:call.arguments[@"height"]];
    BOOL state = [[ZegoTextureRendererController sharedInstance] updateTextureRenderer:textureID viewWidth:viewWidth viewHeight:viewHeight];

    ZGLog(@"[updateTextureRendererSize][Result] w: %d, h: %d, textureID: %ld, success: %@", viewWidth, viewHeight, (long)textureID, state ? @"true" : @"false");

    result(@(state));
}

- (void)destroyTextureRenderer:(FlutterMethodCall *)call result:(FlutterResult)result {

    int64_t textureID = [ZegoUtils longLongValue:call.arguments[@"textureID"]];
    BOOL state = [[ZegoTextureRendererController sharedInstance] destroyTextureRenderer:textureID];

    ZGLog(@"[destroyTextureRenderer][Result] textureID: %ld, success: %@", (long)textureID, state ? @"true" : @"false");

    result(@(state));
}


#pragma mark - Assets Utils

- (void)getAssetAbsolutePath:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *assetPath = call.arguments[@"assetPath"];
    NSString *assetKey = [_registrar lookupKeyForAsset:assetPath];
    NSString *realPath = [[NSBundle mainBundle] pathForResource:assetKey ofType:nil];

    ZGLog(@"[getAssetAbsolutePath] assetPath: %@, realPath: %@", assetPath, realPath);

    result(realPath);
}

#pragma mark - Private

- (void)setPluginVersion:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSString *version = call.arguments[@"version"];

    ZGLog(@"*** Plugin Version: %@", version);
    
    result(nil);
}

void convertFloatArray(float *position, NSArray<NSNumber *> *list) {
    for (int i = 0; i < list.count; i++) {
        position[i] = list[i].floatValue;
    }
}

- (void)reportPluginInfo {
    NSDictionary *advancedConfigMap = @{@"thirdparty_framework_info": @"flutter"};

    ZegoEngineConfig *configObject = [[ZegoEngineConfig alloc] init];
    configObject.advancedConfig = advancedConfigMap;

    [ZegoExpressEngine setEngineConfig:configObject];
}

@end
