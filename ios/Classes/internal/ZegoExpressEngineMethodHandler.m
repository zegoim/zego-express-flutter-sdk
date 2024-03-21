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
#import "ZegoCustomVideoRenderManager.h"
#import "ZegoCustomVideoProcessManager.h"
#import "ZegoMediaPlayerVideoManager.h"
#import "ZegoMediaPlayerAudioManager.h"
#import "ZegoMediaPlayerBlockDataManager.h"

#import "ZegoUtils.h"
#import "ZegoLog.h"
#import <objc/message.h>

@interface ZegoExpressEngineMethodHandler ()

@property (nonatomic, strong) id<FlutterPluginRegistrar> registrar;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoMediaPlayer *> *mediaPlayerMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *,  ZegoScreenCaptureSource *> *screenCaptureSouceMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoAudioEffectPlayer *> *audioEffectPlayerMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoMediaDataPublisher *> *mediaDataPublisherMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoRealTimeSequentialDataManager *> *realTimeSequentialDataManagerMap;

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, ZegoAIVoiceChanger *> *aiVoiceChangerMap;

@property (nonatomic, strong) ZegoRangeAudio *rangeAudioInstance;

@property (nonatomic, strong) ZegoCopyrightedMusic *copyrightedMusicInstance;

@property (nonatomic, assign) BOOL pluginReported;

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

- (void)setRegistrar:(id<FlutterPluginRegistrar>)registrar eventSink:(FlutterEventSink)sink {
    ZGLog(@"[setRegistrar] registrar:%p, sink:%p", registrar, sink);
    _registrar = registrar;
    // Set eventSink for ZegoExpressEngineEventHandler
    [ZegoExpressEngineEventHandler sharedInstance].eventSink = sink;
}

- (void)unInit {
    [ZegoExpressEngineEventHandler sharedInstance].eventSink = nil;
    [ZegoExpressEngine destroyEngine:nil];

    // Uninit texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] uninitController];
    }
}

- (void)initApiCalledCallback {
    [ZegoExpressEngine setApiCalledCallback:[ZegoExpressEngineEventHandler sharedInstance]];
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    ZGLog(@"[DartCall] [%@]", call.method);
    SEL selector = NSSelectorFromString([NSString stringWithFormat:@"%@:result:", call.method]);

    // Handle unrecognized method
    if (![self respondsToSelector:selector]) {
        ZGLog(@"[handleMethodCall] Unrecognized selector: %@", call.method);
        result(FlutterMethodNotImplemented);
        return;
    }

    NSMethodSignature *signature = [self methodSignatureForSelector:selector];
    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];

    invocation.target = self;
    invocation.selector = selector;
    [invocation setArgument:&call atIndex:2];
    [invocation setArgument:&result atIndex:3];
    [invocation invoke];
}


#pragma mark - Main

- (void)getVersion:(FlutterMethodCall *)call result:(FlutterResult)result {

    result([ZegoExpressEngine getVersion]);
}

- (void)isFeatureSupported:(FlutterMethodCall *)call result:(FlutterResult)result {

    int featureType = [ZegoUtils intValue:call.arguments[@"featureType"]];
    BOOL ret = [ZegoExpressEngine isFeatureSupported:(ZegoFeatureType)featureType];
    result(@(ret));
}

- (void)setRoomScenario:(FlutterMethodCall *)call result:(FlutterResult)result {

    int scenario = [ZegoUtils intValue:call.arguments[@"scenario"]];
    [[ZegoExpressEngine sharedEngine] setRoomScenario:(ZegoScenario)scenario];
    result(nil);
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

    [[ZegoExpressEngine sharedEngine] setDataRecordEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setAudioDataHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setCustomAudioProcessHandler:[ZegoExpressEngineEventHandler sharedInstance]];

    // Init texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] initControllerWithMessenger:self.registrar.messenger];
    }

    ZGLog(@"[createEngineWithProfile] platform:iOS, enablePlatformView:%@, appID:%u, scenario:%d", _enablePlatformView ? @"true" : @"false", appID, scenario);

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

    [[ZegoExpressEngine sharedEngine] setDataRecordEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setAudioDataHandler:[ZegoExpressEngineEventHandler sharedInstance]];
    [[ZegoExpressEngine sharedEngine] setCustomAudioProcessHandler:[ZegoExpressEngineEventHandler sharedInstance]];

    // Init texture renderer
    if (!self.enablePlatformView) {
        [[ZegoTextureRendererController sharedInstance] initControllerWithMessenger:self.registrar.messenger];
    }

    ZGLog(@"[createEngine] platform:iOS, enablePlatformView:%@, appID:%u, isTestEnv:%@, scenario:%d", _enablePlatformView ? @"true" : @"false", appID, isTestEnv ? @"true" : @"false", scenario);

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

    // Report framework info
    [self reportPluginInfo];

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
            logConfigObject.logCount = [ZegoUtils unsignedIntValue:logConfigMap[@"logCount"]];

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
        logConfigObject.logCount = [ZegoUtils unsignedIntValue:configMap[@"logCount"]];

        [ZegoExpressEngine setLogConfig:logConfigObject];

        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"setLogConfig_null_config" uppercaseString] message:@"Invoke `setLogConfig` with null config" details:nil]);
    }
}

- (void)setLocalProxyConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSArray<NSDictionary *> *proxyListMap = call.arguments[@"proxyList"];

    NSMutableArray<ZegoProxyInfo *> *proxyList = [[NSMutableArray alloc] init];
    for(NSDictionary *proxyMap in proxyListMap) {
        ZegoProxyInfo *proxyObject = [[ZegoProxyInfo alloc] init];
        proxyObject.ip = proxyMap[@"ip"];
        proxyObject.port = [ZegoUtils intValue:proxyMap[@"port"]];
        proxyObject.hostName = proxyMap[@"hostName"];
        proxyObject.userName = proxyMap[@"userName"];
        proxyObject.password = proxyMap[@"password"];
        
        [proxyList addObject:proxyObject];
    }
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    
    [ZegoExpressEngine setLocalProxyConfig:proxyList enable:enable];
    
    result(nil);
}

- (void)setCloudProxyConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSArray<NSDictionary *> *proxyListMap = call.arguments[@"proxyList"];

    NSMutableArray<ZegoProxyInfo *> *proxyList = [[NSMutableArray alloc] init];
    for(NSDictionary *proxyMap in proxyListMap) {
        ZegoProxyInfo *proxyObject = [[ZegoProxyInfo alloc] init];
        proxyObject.ip = proxyMap[@"ip"];
        proxyObject.port = [ZegoUtils intValue:proxyMap[@"port"]];
        proxyObject.hostName = proxyMap[@"hostName"];
        proxyObject.userName = proxyMap[@"userName"];
        proxyObject.password = proxyMap[@"password"];
        
        [proxyList addObject:proxyObject];
    }
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    
    NSString *token = call.arguments[@"token"];
    
    [ZegoExpressEngine setCloudProxyConfig:proxyList token:token enable:enable];
    
    result(nil);
}

- (void)setRoomMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    
    [ZegoExpressEngine setRoomMode:(ZegoRoomMode)mode];

    result(nil);
}

- (void)setLicense:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *license = call.arguments[@"license"];
    
    [ZegoExpressEngine setLicense:license];

    result(nil);
}

- (void)setGeoFence:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];
    
    NSArray<NSNumber *> *areaList = call.arguments[@"areaList"];
    
    [ZegoExpressEngine setGeoFence:(ZegoGeoFenceType)type area:areaList];

    result(nil);
}

- (void)uploadLog:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] uploadLog];

    result(nil);
}

- (void)submitLog:(FlutterMethodCall *)call result:(FlutterResult)result {

    [ZegoExpressEngine submitLog];

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
    NSString *flutterAssetPrefix = @"flutter-asset://";
    if ([filePath hasPrefix:flutterAssetPrefix]) {
        NSString *assetName = [filePath substringFromIndex:flutterAssetPrefix.length];
#if TARGET_OS_IPHONE
        NSString *assetKey = [_registrar lookupKeyForAsset:assetName];
        NSString *assetRealPath = [[NSBundle mainBundle] pathForResource:assetKey ofType:nil];
#elif TARGET_OS_OSX
        NSString *assetDir = @"/Contents/Frameworks/App.framework/Resources/flutter_assets/";
        NSString *assetRealPath = [NSString stringWithFormat:@"%@%@%@", [[NSBundle mainBundle] bundlePath], assetDir, assetName];
#endif
        NSString *processedURL = [NSString stringWithFormat:@"file:%@", assetRealPath];
        ZGLog(@"[setDummyCaptureImagePath] Flutter asset prefix detected, origin URL: '%@', processed URL: '%@'", filePath, processedURL);
        if (!assetRealPath) {
            ZGLog(@"[setDummyCaptureImagePath] Can not get real path for flutter asset: '%@', please check if the asset is correctly declared in flutter project's pubspec.yaml", assetName);
        } else {
            filePath = processedURL;
        }
    }
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

    ZegoRoomConfig *configObject = [[ZegoRoomConfig alloc] init];
    if (configMap && configMap.count > 0) {
        unsigned int maxMemberCount = [ZegoUtils unsignedIntValue:configMap[@"maxMemberCount"]];
        BOOL isUserStatusNotify = [ZegoUtils boolValue:configMap[@"isUserStatusNotify"]];
        NSString *token = configMap[@"token"];

        configObject.maxMemberCount = maxMemberCount;
        configObject.isUserStatusNotify = isUserStatusNotify;
        configObject.token = token;
    }

    [[ZegoExpressEngine sharedEngine] loginRoom:roomID user:userObject config:configObject callback: ^(int errorCode, NSDictionary * _Nullable extendedData) {
            NSString *extendedDataJsonString = @"{}";
            if (extendedData) {
                NSError *error;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
                if (!jsonData) {
                    ZGLog(@"[loginRoomCallback] extendedData error: %@", error);
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

- (void)logoutRoom:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    if ([ZegoUtils isNullObject:roomID]) {
        [[ZegoExpressEngine sharedEngine] logoutRoomWithCallback: ^(int errorCode, NSDictionary * _Nullable extendedData) {
            NSString *extendedDataJsonString = @"{}";
            if (extendedData) {
                NSError *error;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
                if (!jsonData) {
                    ZGLog(@"[logoutRoomCallback] extendedData error: %@", error);
                }else{
                    extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
                }
            }

            result(@{
                @"errorCode": @(errorCode),
                @"extendedData": extendedDataJsonString
            });
        }];
    } else {
        [[ZegoExpressEngine sharedEngine] logoutRoomWithCallback:roomID callback: ^(int errorCode, NSDictionary * _Nullable extendedData) {
            NSString *extendedDataJsonString = @"{}";
            if (extendedData) {
                NSError *error;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
                if (!jsonData) {
                    ZGLog(@"[logoutRoomCallback] extendedData error: %@", error);
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

- (void)getRoomStreamList:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    int type = [ZegoUtils intValue:call.arguments[@"streamListType"]];
    ZegoRoomStreamListType list_type = (ZegoRoomStreamListType)type;

    ZegoRoomStreamList * stream_list = [[ZegoExpressEngine sharedEngine] getRoomStreamList:roomID streamListType:list_type];
    
    NSMutableArray *streamListPublishArray = [[NSMutableArray alloc] init];
    for (ZegoStream *stream in stream_list.publishStreamList)
    {
        NSString *userID = @"";
        NSString *userName = @"";
        if (stream.user != nil && stream.user.userID != nil) {
            userID = stream.user.userID;
        }
        
        if (stream.user != nil && stream.user.userName != nil) {
            userName = stream.user.userName;
        }
        
        [streamListPublishArray addObject:@{
            @"user": @{
                @"userID": userID,
                @"userName": userName
            },
            @"streamID": stream.streamID,
            @"extraInfo": stream.extraInfo
        }];
    }
    
    NSMutableArray *streamListPlayArray = [[NSMutableArray alloc] init];
    for (ZegoStream *stream in stream_list.playStreamList)
    {
        NSString *userID = @"";
        NSString *userName = @"";
        if (stream.user != nil && stream.user.userID != nil) {
            userID = stream.user.userID;
        }
        
        if (stream.user != nil && stream.user.userName != nil) {
            userName = stream.user.userName;
        }
        
        [streamListPlayArray addObject:@{
            @"user": @{
                @"userID": userID,
                @"userName": userName
            },
            @"streamID": stream.streamID,
            @"extraInfo": stream.extraInfo
        }];
    }
    
    result(@{
        @"publishStreamList": streamListPublishArray,
        @"playStreamList": streamListPlayArray
    });

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
        config.forceSynchronousNetworkTime = [ZegoUtils intValue:configMap[@"forceSynchronousNetworkTime"]];
        config.streamCensorshipMode = (ZegoStreamCensorshipMode)[ZegoUtils intValue:configMap[@"streamCensorshipMode"]];
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
        BOOL alphaBlend = [ZegoUtils boolValue:canvasMap[@"alphaBlend"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];
#if TARGET_OS_IPHONE
            if (alphaBlend) {
                [platformView.view setBackgroundColor:UIColor.clearColor];
            }
#endif

            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:platformView.view];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;

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
            [[ZegoTextureRendererController sharedInstance] enableTextureAlpha:alphaBlend withTexture:viewID];
            // Render with Texture
            if (![[ZegoTextureRendererController sharedInstance] bindCapturedChannel:@(channel) withTexture:viewID]) {
                // Preview video without creating TextureRenderer in advance
                // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createCanvasView` first and get the textureID", (long)viewID];
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
        [[ZegoTextureRendererController sharedInstance] unbindCapturedChannel:@(channel)];
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
    if (codecID > 4) {
        codecID = 100;
    }
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

- (void)setPublishDualStreamConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSArray<NSDictionary *> *configListMap = call.arguments[@"configList"];
    NSMutableArray<ZegoPublishDualStreamConfig *> *configList = [[NSMutableArray alloc] init];
    
    for (NSDictionary *configMap in configListMap) {
        ZegoPublishDualStreamConfig *config = [[ZegoPublishDualStreamConfig alloc] init];
        int streamType = [ZegoUtils intValue:configMap[@"streamType"]];
        int width = [ZegoUtils intValue:configMap[@"encodeWidth"]];
        int height = [ZegoUtils intValue:configMap[@"encodeHeight"]];
        
        config.streamType = (ZegoVideoStreamType) streamType;
        config.fps = [ZegoUtils intValue:configMap[@"fps"]];
        config.bitrate = [ZegoUtils intValue:configMap[@"bitrate"]];
        config.encodeResolution = CGSizeMake(width, height);
        
        [configList addObject:config];
    }
    
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setPublishDualStreamConfig:configList.copy channel:(ZegoPublishChannel)channel];

    result(nil);

}

- (void)setAppOrientation:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
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
#endif
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

    [[ZegoExpressEngine sharedEngine] takePublishStreamSnapshot:^(int errorCode, ZGImage * _Nullable image) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSData *imageData = nil;
            if (image) {
#if TARGET_OS_IPHONE
                imageData = UIImageJPEGRepresentation(image, 1);
#elif TARGET_OS_OSX
                CGImageRef cgImage = [image CGImageForProposedRect:nil context:nil hints:nil];
                NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
                imageData = [bitmapRep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}];
#endif
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
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableTrafficControl:enable property:(ZegoTrafficControlProperty)property channel:(ZegoPublishChannel)channel];

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
    int timeout = [ZegoUtils intValue:call.arguments[@"timeout"]];

    [[ZegoExpressEngine sharedEngine] addPublishCdnUrl:targetURL streamID:streamID timeout:timeout callback:^(int errorCode) {
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
        cdnConfig.protocol = config[@"protocol"];
        cdnConfig.quicVersion = config[@"quicVersion"];
        cdnConfig.quicConnectMode = [ZegoUtils intValue:config[@"quicConnectMode"]];
        
        int httpdnsIndex = [ZegoUtils intValue:config[@"httpdns"]];
        cdnConfig.httpdns = (ZegoHttpDNSType)httpdnsIndex;
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
#if TARGET_OS_IPHONE
            NSString *assetKey = [_registrar lookupKeyForAsset:assetName];
            NSString *assetRealPath = [[NSBundle mainBundle] pathForResource:assetKey ofType:nil];
#elif TARGET_OS_OSX
            NSString *assetDir = @"/Contents/Frameworks/App.framework/Resources/flutter_assets/";
            NSString *assetRealPath = [NSString stringWithFormat:@"%@%@%@", [[NSBundle mainBundle] bundlePath], assetDir, assetName];
#endif
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

    NSDictionary *configMap = call.arguments[@"config"];

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

- (void)sendAudioSideInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    double timeStampMs = [ZegoUtils doubleValue:call.arguments[@"timeStampMs"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] sendAudioSideInfo:data.data timeStampMs: timeStampMs channel:(ZegoPublishChannel)channel];

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
    if (codecID > 4) {
        codecID = 100;
    }

    int supportRet = 0;
    if ([ZegoUtils isNullObject:call.arguments[@"codecBackend"]]) {
        supportRet = [[ZegoExpressEngine sharedEngine] isVideoEncoderSupported:(ZegoVideoCodecID)codecID] ? 1: 0;
    } else {
        int codecBackend = [ZegoUtils intValue:call.arguments[@"codecBackend"]];
        supportRet = [[ZegoExpressEngine sharedEngine] isVideoEncoderSupported:(ZegoVideoCodecID)codecID codecBackend:(ZegoVideoCodecBackend)codecBackend];
    }

    result(@(supportRet));
}

- (void)setAppOrientationMode:(FlutterMethodCall *)call result:(FlutterResult)result {

#if TARGET_OS_IPHONE
    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setAppOrientationMode:(ZegoOrientationMode)mode];
#endif

    result(nil);
}

- (void)setLowlightEnhancement:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setLowlightEnhancement:(ZegoLowlightEnhancementMode)mode channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setVideoSource:(FlutterMethodCall *)call result:(FlutterResult)result {

    int source = [ZegoUtils intValue:call.arguments[@"source"]];
    BOOL hasInstanceID = NO;
    int instanceID = -1;
    if (![ZegoUtils isNullObject:call.arguments[@"instanceID"]]) {
        hasInstanceID = YES;
        instanceID = [ZegoUtils intValue:call.arguments[@"instanceID"]];
    }

    BOOL hasChannel = NO;
    int channel = 0;
    if (![ZegoUtils isNullObject:call.arguments[@"channel"]]) {
        hasChannel = YES;
        channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    }
    
    /// 标识推流通道的视频源
    [ZegoTextureRendererController.sharedInstance setVideoSourceChannel:@(channel) withSource:source];

    int ret = 0;
    if (!hasChannel && !hasInstanceID) {
        ret = [[ZegoExpressEngine sharedEngine] setVideoSource:(ZegoVideoSourceType)source];
    } else if (hasChannel && !hasInstanceID) {
        ret = [[ZegoExpressEngine sharedEngine] setVideoSource:(ZegoVideoSourceType)source channel:(ZegoPublishChannel)channel];
    } else if (!hasChannel && hasInstanceID) {
        ret = [[ZegoExpressEngine sharedEngine] setVideoSource:(ZegoVideoSourceType)source instanceID:instanceID];
    } else {
        ret = [[ZegoExpressEngine sharedEngine] setVideoSource:(ZegoVideoSourceType)source instanceID:instanceID channel:(ZegoPublishChannel)channel];
    }

    result(@(ret));
}

- (void)setAudioSource:(FlutterMethodCall *)call result:(FlutterResult)result {

    int source = [ZegoUtils intValue:call.arguments[@"source"]];
    BOOL hasConfig = NO;
    ZegoAudioSourceMixConfig *config = [[ZegoAudioSourceMixConfig alloc] init];
    if (![ZegoUtils isNullObject:call.arguments[@"config"]]) {
        hasConfig = YES;
        NSDictionary *configMap = call.arguments[@"config"];

        NSArray<NSNumber *> *audioEffectPlayerIndexList = configMap[@"audioEffectPlayerIndexList"];
        config.audioEffectPlayerCount = (int)audioEffectPlayerIndexList.count;
        int *audioEffectPlayers = NULL;
        if (config.audioEffectPlayerCount > 0) {
            audioEffectPlayers = malloc(sizeof(int)*config.audioEffectPlayerCount);
        }
        config.audioEffectPlayerIndexList = audioEffectPlayers;
        for(NSNumber* index in audioEffectPlayerIndexList) {
            *audioEffectPlayers = [index intValue];
            audioEffectPlayers += 1;
        }

        NSArray<NSNumber *> *mediaPlayerIndexList = configMap[@"mediaPlayerIndexList"];
        config.mediaPlayerCount = (int)mediaPlayerIndexList.count;
        int *mediaPlayers = NULL;
        if (config.mediaPlayerCount > 0) {
            mediaPlayers  = malloc(sizeof(int)*config.mediaPlayerCount);
        }
        config.mediaPlayerIndexList = mediaPlayers;
        for(NSNumber * index in mediaPlayerIndexList) {
            *mediaPlayers = [index intValue];
            mediaPlayers += 1;
        }
    }

    BOOL hasChannel = NO;
    int channel = -1;
    if (![ZegoUtils isNullObject: call.arguments[@"channel"] ] ) {
        hasChannel = YES;
        channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    }

    int ret = -1;
    if (!hasChannel && !hasConfig) {
        ret = [[ZegoExpressEngine sharedEngine] setAudioSource:(ZegoAudioSourceType)source];
    } else if (hasChannel && !hasConfig) {
        ret = [[ZegoExpressEngine sharedEngine] setAudioSource:(ZegoAudioSourceType)source channel:(ZegoPublishChannel)channel];
    } else if (!hasChannel && hasConfig) {
        ret = [[ZegoExpressEngine sharedEngine] setAudioSource:(ZegoAudioSourceType)source config:config];
        free(config.mediaPlayerIndexList);
        free(config.audioEffectPlayerIndexList);
    } else {
        if (channel == 0) {
            ret = [[ZegoExpressEngine sharedEngine] setAudioSource:(ZegoAudioSourceType)source config:config];
        }
        free(config.mediaPlayerIndexList);
        free(config.audioEffectPlayerIndexList);
    }

    result(@(ret));
}

- (void)enableVideoObjectSegmentation:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue: call.arguments[@"enable"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    
    ZegoObjectSegmentationConfig *config = [[ZegoObjectSegmentationConfig alloc] init];
    NSDictionary *configMap = call.arguments[@"config"];
    if (configMap) {
        config.objectSegmentationType = (ZegoObjectSegmentationType)[ZegoUtils intValue:configMap[@"objectSegmentationType"]];
        
        NSDictionary *backgroundConfigMap = configMap[@"backgroundConfig"];
        if (backgroundConfigMap) {
            ZegoBackgroundConfig *backgroundConfig = [[ZegoBackgroundConfig alloc] init];
            backgroundConfig.processType = (ZegoBackgroundProcessType)[ZegoUtils intValue:backgroundConfigMap[@"processType"]];
            backgroundConfig.blurLevel = (ZegoBackgroundBlurLevel)[ZegoUtils intValue:backgroundConfigMap[@"blurLevel"]];
            backgroundConfig.color = [ZegoUtils intValue:backgroundConfigMap[@"color"]];
            backgroundConfig.imageURL = backgroundConfigMap[@"imageURL"];
            backgroundConfig.videoURL = backgroundConfigMap[@"videoURL"];
            config.backgroundConfig = backgroundConfig;
        }
    }

    [[ZegoExpressEngine sharedEngine] enableVideoObjectSegmentation:enable config:config channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)enableAlphaChannelVideoEncoder:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue: call.arguments[@"enable"]];
    int alphaLayout = [ZegoUtils intValue:call.arguments[@"alphaLayout"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableAlphaChannelVideoEncoder:enable alphaLayout:(ZegoAlphaLayoutType)alphaLayout channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)setCameraStabilizationMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraStabilizationMode:mode channel:(ZegoPublishChannel)channel];

    result(nil);
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
        playerConfig.sourceResourceType = (ZegoResourceType)[ZegoUtils intValue:playerConfigMap[@"sourceResourceType"]];
        playerConfig.codecTemplateID = [ZegoUtils intValue:playerConfigMap[@"codecTemplateID"]];
        playerConfig.resourceSwitchMode = [ZegoUtils intValue:playerConfigMap[@"resourceSwitchMode"]];
        playerConfig.resourceWhenStopPublish = [ZegoUtils intValue:playerConfigMap[@"resourceWhenStopPublish"]];
        int videoCodecIDIndex = [ZegoUtils intValue:playerConfigMap[@"videoCodecID"]];
        playerConfig.videoCodecID = (ZegoVideoCodecID)videoCodecIDIndex;
        if (videoCodecIDIndex > 4) {
            playerConfig.videoCodecID = ZegoVideoCodecIDUnknown;
        }
        playerConfig.roomID = playerConfigMap[@"roomID"];
        NSDictionary * cdnConfigMap = playerConfigMap[@"cdnConfig"];

        if (cdnConfigMap && cdnConfigMap.count > 0) {
            ZegoCDNConfig *cdnConfig = [[ZegoCDNConfig alloc] init];
            cdnConfig.url = cdnConfigMap[@"url"];
            cdnConfig.authParam = cdnConfigMap[@"authParam"];
            cdnConfig.protocol = cdnConfigMap[@"protocol"];
            cdnConfig.quicVersion = cdnConfigMap[@"quicVersion"];
            cdnConfig.quicConnectMode = [ZegoUtils intValue:cdnConfigMap[@"quicConnectMode"]];
            
            int httpdnsIndex = [ZegoUtils intValue:cdnConfigMap[@"httpdns"]];
            cdnConfig.httpdns = (ZegoHttpDNSType)httpdnsIndex;
            
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
        BOOL alphaBlend = [ZegoUtils boolValue:canvasMap[@"alphaBlend"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];
#if TARGET_OS_IPHONE
            if (alphaBlend) {
                [platformView.view setBackgroundColor:UIColor.clearColor];
            }
#endif

            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:platformView.view];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;

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
            [[ZegoTextureRendererController sharedInstance] enableTextureAlpha:alphaBlend withTexture:viewID];
            // Render with Texture
            if (![[ZegoTextureRendererController sharedInstance] bindRemoteStreamId:streamID withTexture:viewID]) {
                // Play video without creating TextureRenderer in advance
                // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createCanvasView` first and get the textureID", (long)viewID];
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
        [[ZegoTextureRendererController sharedInstance] unbindRemoteStreamId:streamID];
    }

    result(nil);
}

- (void)setPlayStreamDecryptionKey:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *key = call.arguments[@"key"];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamDecryptionKey:key streamID:streamID];

    result(nil);
}

- (void)setPlayStreamCrossAppInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];
    NSDictionary *infoMap = call.arguments[@"info"];
    ZegoCrossAppInfo *info = [[ZegoCrossAppInfo alloc] init];
    info.appID = [ZegoUtils unsignedIntValue:infoMap[@"appID"]];
    info.token = infoMap[@"token"];
    
    [[ZegoExpressEngine sharedEngine] setPlayStreamCrossAppInfo:info streamID:streamID];

    result(nil);
}

- (void)takePlayStreamSnapshot:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] takePlayStreamSnapshot:streamID callback:^(int errorCode, ZGImage * _Nullable image) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSData *imageData = nil;
            if (image) {
#if TARGET_OS_IPHONE
                imageData = UIImageJPEGRepresentation(image, 1);
#elif TARGET_OS_OSX
                CGImageRef cgImage = [image CGImageForProposedRect:nil context:nil hints:nil];
                NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
                imageData = [bitmapRep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}];
#endif
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

- (void)setPlayStreamVideoType:(FlutterMethodCall *)call result:(FlutterResult)result {

    int streamType = [ZegoUtils intValue:call.arguments[@"streamType"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamVideoType:(ZegoVideoStreamType)streamType streamID:streamID];

    result(nil);
}

- (void)setPlayStreamBufferIntervalRange:(FlutterMethodCall *)call result:(FlutterResult)result {

    unsigned int minBufferInterval = [ZegoUtils unsignedIntValue:call.arguments[@"minBufferInterval"]];
    unsigned int maxBufferInterval = [ZegoUtils unsignedIntValue:call.arguments[@"maxBufferInterval"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] setPlayStreamBufferIntervalRange:streamID min:minBufferInterval max:maxBufferInterval];

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

- (void)muteAllPlayAudioStreams:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteAllPlayAudioStreams:mute];

    result(nil);
}

- (void)mutePlayStreamVideo:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] mutePlayStreamVideo:mute streamID:streamID];

    result(nil);
}

- (void)muteAllPlayVideoStreams:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];

    [[ZegoExpressEngine sharedEngine] muteAllPlayVideoStreams:mute];

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
    if (codecID > 4) {
        codecID = 100;
    }

    int supportRet = 0;
    if ([ZegoUtils isNullObject:call.arguments[@"codecBackend"]]) {
        supportRet = [[ZegoExpressEngine sharedEngine] isVideoDecoderSupported:(ZegoVideoCodecID)codecID] ? 1: 0;
    } else {
        int codecBackend = [ZegoUtils intValue:call.arguments[@"codecBackend"]];
        supportRet = [[ZegoExpressEngine sharedEngine] isVideoDecoderSupported:(ZegoVideoCodecID)codecID codecBackend:(ZegoVideoCodecBackend)codecBackend];
    }

    result(@(supportRet));
}

- (void)setPlayStreamsAlignmentProperty:(FlutterMethodCall *)call result:(FlutterResult)result {

    int mode = [ZegoUtils intValue:call.arguments[@"mode"]];

    [[ZegoExpressEngine sharedEngine] setPlayStreamsAlignmentProperty:(ZegoStreamAlignmentMode)mode];

    result(nil);
}

- (void)updatePlayingCanvas:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *streamID = call.arguments[@"streamID"];
    
    // Handle ZegoCanvas

    NSDictionary *canvasMap = call.arguments[@"canvas"];

    int errorCode = 0;
    if (canvasMap && canvasMap.count > 0) {
        // Play video

        // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
        int64_t viewID = [ZegoUtils longLongValue:canvasMap[@"view"]];
        int viewMode = [ZegoUtils intValue:canvasMap[@"viewMode"]];
        int backgroundColor = [ZegoUtils intValue:canvasMap[@"backgroundColor"]];
        BOOL alphaBlend = [ZegoUtils boolValue:canvasMap[@"alphaBlend"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];
#if TARGET_OS_IPHONE
            if (alphaBlend) {
                [platformView.view setBackgroundColor:UIColor.clearColor];
            }
#endif

            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:platformView.view];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;

                errorCode = [[ZegoExpressEngine sharedEngine] updatePlayingCanvas:streamID canvas:canvas];
            } else {
                // Play video without creating the PlatformView in advance
                // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                NSString *errorMessage = [NSString stringWithFormat:@"The PlatformView for viewID:%ld cannot be found, developer should call `createPlatformView` first and get the viewID", (long)viewID];
                ZGError(@"[updatePlayingCanvas] %@", errorMessage);
                result([FlutterError errorWithCode:[@"updatePlayingCanvas_No_PlatformView" uppercaseString] message:errorMessage details:nil]);
                return;
            }

        } else {
            [[ZegoTextureRendererController sharedInstance] enableTextureAlpha:alphaBlend withTexture:viewID];
            // Render with Texture
            if (![[ZegoTextureRendererController sharedInstance] bindRemoteStreamId:streamID withTexture:viewID]) {
                // Play video without creating TextureRenderer in advance
                // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createCanvasView` first and get the textureID", (long)viewID];
                ZGError(@"[updatePlayingCanvas] %@", errorMessage);
                result([FlutterError errorWithCode:[@"updatePlayingCanvas_No_TextureRenderer" uppercaseString] message:errorMessage details:nil]);
                return;
            }
        }

    }

    result(@(errorCode));
}

#if TARGET_OS_IPHONE
- (void)enableVideoSuperResolution:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSString *streamID = call.arguments[@"streamID"];

    [[ZegoExpressEngine sharedEngine] enableVideoSuperResolution:streamID enable:enable];

    result(nil);
}

- (void)initVideoSuperResolution:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] initVideoSuperResolution];

    result(nil);
}

- (void)uninitVideoSuperResolution:(FlutterMethodCall *)call result:(FlutterResult)result {

    [[ZegoExpressEngine sharedEngine] uninitVideoSuperResolution];

    result(nil);
}

#endif

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

            inputObject.volume = [ZegoUtils intValue:inputMap[@"volume"]];
            inputObject.isAudioFocus = [ZegoUtils boolValue:inputMap[@"isAudioFocus"]];
            inputObject.audioDirection = [ZegoUtils intValue:inputMap[@"audioDirection"]];
            
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
                fontStyle.border = [ZegoUtils boolValue:fontMap[@"border"]];
                fontStyle.borderColor = [ZegoUtils intValue:fontMap[@"borderColor"]];
            
                labelInfo.font = fontStyle;
                inputObject.label = labelInfo;
            }
            
            NSNumber *renderMode = inputMap[@"renderMode"];
            if (renderMode && ![ZegoUtils isNullObject:renderMode]) {
                inputObject.renderMode = (ZegoMixRenderMode)[ZegoUtils intValue:renderMode];
            }

            NSDictionary *imageInfoMap = inputMap[@"imageInfo"];
            if (imageInfoMap && ![ZegoUtils isNullObject:imageInfoMap]) {
                NSString *url = imageInfoMap[@"url"];
                int displayMode = [ZegoUtils intValue:imageInfoMap[@"displayMode"]];
                inputObject.imageInfo = [[ZegoMixerImageInfo alloc] initWithURLAndDisplayMode:url displayMode:displayMode];
            }

            NSNumber *cornerRadius = inputMap[@"cornerRadius"];
            if (cornerRadius && ![ZegoUtils isNullObject:cornerRadius]) {
                inputObject.cornerRadius = [ZegoUtils intValue:cornerRadius];
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
                if (codecID > 4) {
                    codecID = 100;
                }
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                bool enableLowBitrateHD = [ZegoUtils boolValue:videoConfigMap[@"enableLowBitrateHD"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency enableLowBitrateHD:enableLowBitrateHD];
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
        int mixMode = [ZegoUtils intValue:audioConfigMap[@"mixMode"]];
        ZegoMixerAudioConfig *audioConfigObject = [[ZegoMixerAudioConfig alloc] init];
        audioConfigObject.bitrate = bitrate;
        audioConfigObject.channel = (ZegoAudioChannel)channel;
        audioConfigObject.codecID = (ZegoAudioCodecID)codecID;
        audioConfigObject.mixMode  = (ZegoAudioMixMode)mixMode;

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
        videoConfigObject.quality = [ZegoUtils intValue:videoConfigMap[@"quality"]];
        videoConfigObject.rateControlMode = (ZegoVideoRateControlMode)[ZegoUtils intValue:videoConfigMap[@"rateControlMode"]];

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
    
    // whiteboard
    NSDictionary *whiteboardMap = call.arguments[@"whiteboard"];
    if (whiteboardMap && whiteboardMap.count > 0) {
        unsigned long long whiteboardID = [ZegoUtils unsignedLongLongValue:whiteboardMap[@"whiteboardID"]];
        if (whiteboardID != 0) {
            ZegoMixerWhiteboard *whiteboard = [[ZegoMixerWhiteboard alloc] init];
            whiteboard.whiteboardID = whiteboardID;
            whiteboard.horizontalRatio = [ZegoUtils intValue:whiteboardMap[@"horizontalRatio"]];
            whiteboard.verticalRatio = [ZegoUtils intValue:whiteboardMap[@"verticalRatio"]];
            whiteboard.isPPTAnimation = [ZegoUtils boolValue:whiteboardMap[@"isPPTAnimation"]];
            whiteboard.zOrder = [ZegoUtils intValue:whiteboardMap[@"zOrder"]];
            
            NSDictionary *layoutMap = whiteboardMap[@"layout"];
            if (layoutMap && layoutMap.count > 0) {
                int top = [ZegoUtils intValue:layoutMap[@"top"]];
                int left = [ZegoUtils intValue:layoutMap[@"left"]];
                int right = [ZegoUtils intValue:layoutMap[@"right"]];
                int bottom = [ZegoUtils intValue:layoutMap[@"bottom"]];
                whiteboard.layout = CGRectMake(left, top, right - left, bottom - top);
            }
            
            [taskObject setWhiteboard:whiteboard];
        }
    }
    
    // Background Color
    int backgroundColor = [ZegoUtils intValue:call.arguments[@"backgroundColor"]];
    [taskObject setBackgroundColor: backgroundColor];

    // Background Image
    NSString *backgroundImageURL = call.arguments[@"backgroundImageURL"];
    if (backgroundImageURL.length > 0) {
        [taskObject setBackgroundImageURL:backgroundImageURL];
    }

    // Enable SoundLevel
    BOOL enableSoundLevel = [ZegoUtils boolValue:call.arguments[@"enableSoundLevel"]];
    [taskObject enableSoundLevel:enableSoundLevel];
    
    // Stream AlignmentMode
    int streamAlignmentMode = [ZegoUtils intValue:call.arguments[@"streamAlignmentMode"]];
    [taskObject setStreamAlignmentMode:(ZegoStreamAlignmentMode)streamAlignmentMode];
    
    // User Data
    FlutterStandardTypedData *userData = call.arguments[@"userData"];
    [taskObject setUserData:userData.data length:(int)userData.data.length];

    // minPlayStreamBufferLength
    int minPlayStreamBufferLength = [ZegoUtils intValue:call.arguments[@"minPlayStreamBufferLength"]];
    [taskObject setMinPlayStreamBufferLength:minPlayStreamBufferLength];

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
                if (codecID > 4) {
                    codecID = 100;
                }
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                bool enableLowBitrateHD = [ZegoUtils boolValue:videoConfigMap[@"enableLowBitrateHD"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency enableLowBitrateHD: enableLowBitrateHD];
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
                if (codecID > 4) {
                    codecID = 100;
                }
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                bool enableLowBitrateHD = [ZegoUtils boolValue:videoConfigMap[@"enableLowBitrateHD"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency enableLowBitrateHD: enableLowBitrateHD];
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
    
    // Stream AlignmentMode
    int streamAlignmentMode = [ZegoUtils intValue:call.arguments[@"streamAlignmentMode"]];
    taskObject.streamAlignmentMode = (ZegoStreamAlignmentMode)streamAlignmentMode;

    // minPlayStreamBufferLength
    taskObject.minPlayStreamBufferLength = [ZegoUtils intValue:call.arguments[@"minPlayStreamBufferLength"]];

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
                if (codecID > 4) {
                    codecID = 100;
                }
                int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                int encodeLatency = [ZegoUtils intValue:videoConfigMap[@"encodeLatency"]];
                int encodeProfile = [ZegoUtils intValue:videoConfigMap[@"encodeProfile"]];
                bool enableLowBitrateHD = [ZegoUtils boolValue:videoConfigMap[@"enableLowBitrateHD"]];
                ZegoMixerOutputVideoConfig *videoConfig = [[ZegoMixerOutputVideoConfig alloc] init];
                [videoConfig configWithCodecID: (ZegoVideoCodecID)codecID bitrate: bitrate encodeProfile: (ZegoEncodeProfile)encodeProfile encodeLatency: encodeLatency enableLowBitrateHD: enableLowBitrateHD];
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

#if TARGET_OS_OSX

- (void)getAudioDeviceList:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];
    NSArray<ZegoDeviceInfo *> * audioDeviceList = [[ZegoExpressEngine sharedEngine] getAudioDeviceList: (ZegoAudioDeviceType)type];
    NSMutableArray *audioDeviceListResult = [[NSMutableArray alloc] init];
    for (ZegoDeviceInfo *info in audioDeviceList) {
        [audioDeviceListResult addObject:@{
            @"deviceID": info.deviceID,
            @"deviceName": info.deviceName
        }];
    }
    
    result(audioDeviceListResult);
}

- (void)getDefaultAudioDeviceID:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];
    NSString *deviceID = [[ZegoExpressEngine sharedEngine] getDefaultAudioDeviceID: (ZegoAudioDeviceType)type];

    result(deviceID);
}

- (void)useAudioDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    int type = [ZegoUtils intValue:call.arguments[@"type"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    [[ZegoExpressEngine sharedEngine] useAudioDevice:deviceID deviceType: (ZegoAudioDeviceType)type];

    result(nil);
}

- (void)useVideoDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    [[ZegoExpressEngine sharedEngine] useVideoDevice:deviceID channel: (ZegoPublishChannel) channel];

    result(nil);
}

- (void)getVideoDeviceList:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSArray<ZegoDeviceInfo *> * videoDeviceList = [[ZegoExpressEngine sharedEngine] getVideoDeviceList];
    NSMutableArray *videoDeviceListResult = [[NSMutableArray alloc] init];
    for (ZegoDeviceInfo *info in videoDeviceList) {
        [videoDeviceListResult addObject:@{
            @"deviceID": info.deviceID,
            @"deviceName": info.deviceName
        }];
    }
    
    result(videoDeviceListResult);
}

- (void)getDefaultVideoDeviceID:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *deviceID = [[ZegoExpressEngine sharedEngine] getDefaultVideoDeviceID];

    result(deviceID);
}

- (void)getAudioDeviceVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    int volume = [[ZegoExpressEngine sharedEngine] getAudioDeviceVolume:deviceID  deviceType: (ZegoAudioDeviceType)deviceType];

    result(@(volume));
}

- (void)setAudioDeviceVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
    [[ZegoExpressEngine sharedEngine] setAudioDeviceVolume:deviceID deviceType: (ZegoAudioDeviceType)deviceType volume: volume];

    result(nil);
}

- (void)startAudioDeviceVolumeMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    [[ZegoExpressEngine sharedEngine] startAudioDeviceVolumeMonitor:deviceID deviceType: (ZegoAudioDeviceType)deviceType];

    result(nil);
}

- (void)stopAudioDeviceVolumeMonitor:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    [[ZegoExpressEngine sharedEngine] stopAudioDeviceVolumeMonitor:deviceID deviceType: (ZegoAudioDeviceType)deviceType];

    result(nil);
}

- (void)muteAudioDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    BOOL mute = [ZegoUtils boolValue:call.arguments[@"mute"]];
    [[ZegoExpressEngine sharedEngine] muteAudioDevice:deviceID deviceType: (ZegoAudioDeviceType)deviceType mute: mute];

    result(nil);
}

- (void)isAudioDeviceMuted:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    NSString *deviceID = call.arguments[@"deviceID"];
    BOOL mute = [[ZegoExpressEngine sharedEngine] isAudioDeviceMuted:deviceID deviceType: (ZegoAudioDeviceType)deviceType];

    result(@(mute));
}

- (void)enableMixSystemPlayout:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    [[ZegoExpressEngine sharedEngine] enableMixSystemPlayout:enable];

    result(nil);
}

- (void)setMixSystemPlayoutVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
    [[ZegoExpressEngine sharedEngine] setMixSystemPlayoutVolume:volume];

    result(nil);
}

- (void)enableMixEnginePlayout:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    [[ZegoExpressEngine sharedEngine] enableMixEnginePlayout:enable];

    result(nil);
}

- (void)getCurrentAudioDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceType = [ZegoUtils intValue:call.arguments[@"deviceType"]];
    ZegoDeviceInfo *info = [[ZegoExpressEngine sharedEngine] getCurrentAudioDevice:(ZegoAudioDeviceType)deviceType];

    result(@{
        @"deviceID": info.deviceID,
        @"deviceName": info.deviceName
    });
}

#endif

#if TARGET_OS_IPHONE
- (void)setAudioDeviceMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    int deviceMode = [ZegoUtils intValue:call.arguments[@"deviceMode"]];
    [[ZegoExpressEngine sharedEngine] setAudioDeviceMode:(ZegoAudioDeviceMode)deviceMode];

    result(nil);
}

#endif

- (void)enableAudioCaptureDevice:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[ZegoExpressEngine sharedEngine] enableAudioCaptureDevice:enable];

        result(nil);
    });
}

- (void)setAudioRouteToSpeaker:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    BOOL defaultToSpeaker = [ZegoUtils boolValue:call.arguments[@"defaultToSpeaker"]];

    [[ZegoExpressEngine sharedEngine] setAudioRouteToSpeaker:defaultToSpeaker];
#endif
    result(nil);
}

- (void)getAudioRouteType:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    ZegoAudioRoute type = [[ZegoExpressEngine sharedEngine] getAudioRouteType];

    result(@(type));
#elif TARGET_OS_OSX
    result(@(ZegoAudioRouteSpeaker)); // TODO: "Unknown"?
#endif
}

- (void)enableCamera:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableCamera:enable channel:(ZegoPublishChannel)channel];

    result(nil);
}

- (void)useFrontCamera:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] useFrontCamera:enable channel:(ZegoPublishChannel)channel];
#endif
    result(nil);
}

- (void)isCameraFocusSupported:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    BOOL supported = [[ZegoExpressEngine sharedEngine] isCameraFocusSupported:(ZegoPublishChannel)channel];
    result(@(supported));
#elif TARGET_OS_OSX
    result(@(NO));
#endif
}

- (void)setCameraFocusMode:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    int model = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraFocusMode:(ZegoCameraFocusMode)model channel:(ZegoPublishChannel)channel];
#endif
    result(nil);
}

- (void)setCameraFocusPointInPreview:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    float x = [ZegoUtils floatValue:call.arguments[@"x"]];
    float y = [ZegoUtils floatValue:call.arguments[@"y"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraFocusPointInPreviewX:x y: y channel:(ZegoPublishChannel)channel];
#endif
    result(nil);
}

- (void)setCameraExposureMode:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    int model = [ZegoUtils intValue:call.arguments[@"mode"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposureMode:(ZegoCameraExposureMode)model channel:(ZegoPublishChannel)channel];
#endif
    result(nil);
}

- (void)setCameraExposurePointInPreview:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    float x = [ZegoUtils floatValue:call.arguments[@"x"]];
    float y = [ZegoUtils floatValue:call.arguments[@"y"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposurePointInPreviewX:x y: y channel:(ZegoPublishChannel)channel];
#endif
    result(nil);
}

- (void)setCameraExposureCompensation:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    float value = [ZegoUtils floatValue:call.arguments[@"value"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraExposureCompensation:value channel:channel];
#endif
    result(nil);
}

- (void)setCameraZoomFactor:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    float factor = [ZegoUtils floatValue:call.arguments[@"factor"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] setCameraZoomFactor:factor channel:channel];
#endif
    result(nil);
}

- (void)getCameraMaxZoomFactor:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    float factor = [[ZegoExpressEngine sharedEngine] getCameraMaxZoomFactor:channel];

    result(@(factor));
#elif TARGET_OS_OSX
    result(@(1.0)); // TODO: Unknown?
#endif
}

- (void)enableCameraAdaptiveFPS:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int minFPS = [ZegoUtils intValue:call.arguments[@"minFPS"]];
    int maxFPS = [ZegoUtils intValue:call.arguments[@"maxFPS"]];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] enableCameraAdaptiveFPS:enable minFPS:minFPS maxFPS:maxFPS channel:(ZegoPublishChannel)channel];

    result(nil);
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

- (void)enableSpeechEnhance:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    int level = [ZegoUtils intValue:call.arguments[@"level"]];

    [[ZegoExpressEngine sharedEngine] enableSpeechEnhance:enable level:level];

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] enableBeautify:(ZegoBeautifyFeature)feature channel:(ZegoPublishChannel)channel];
#pragma clang diagnostic pop
    result(nil);
}

- (void)setBeautifyOption:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSDictionary *option = call.arguments[@"option"];
    ZegoBeautifyOption *optionObject = [[ZegoBeautifyOption alloc] init];
    optionObject.polishStep = [ZegoUtils doubleValue:option[@"polishStep"]];
    optionObject.whitenFactor = [ZegoUtils doubleValue:option[@"whitenFactor"]];
    optionObject.sharpenFactor = [ZegoUtils doubleValue:option[@"sharpenFactor"]];

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[ZegoExpressEngine sharedEngine] setBeautifyOption: optionObject channel:(ZegoPublishChannel)channel];
#pragma clang diagnostic pop
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

- (void)enableAlignedAudioAuxData:(FlutterMethodCall *)call result:(FlutterResult)result {
    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *paramMap = call.arguments[@"param"];
    
    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];
    
    [[ZegoExpressEngine sharedEngine] enableAlignedAudioAuxData:enable param:param];
    
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
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
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
    int samples = [ZegoUtils intValue:call.arguments[@"samples"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    CMTime timestamp = CMTimeMakeWithSeconds(referenceTimeMillisecond, 1000);
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    [[ZegoExpressEngine sharedEngine] sendCustomAudioCaptureAACData:(unsigned char*)[data.data bytes] dataLength:dataLength configLength:configLength timestamp:timestamp samples:samples param:param channel:channel];

    result(nil);
}

- (void)sendCustomAudioCapturePCMData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
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
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
    param.channel = [ZegoUtils intValue:paramMap[@"channel"]];

    [[ZegoExpressEngine sharedEngine] fetchCustomAudioRenderPCMData:(unsigned char*)[data.data bytes] dataLength:dataLength param:param];

    result(nil);
}

- (void)sendReferenceAudioPCMData:(FlutterMethodCall *)call result:(FlutterResult)result {

    FlutterStandardTypedData *data = call.arguments[@"data"];
    int dataLength = [ZegoUtils intValue:call.arguments[@"dataLength"]];
    NSDictionary *paramMap = call.arguments[@"param"];

    ZegoAudioFrameParam *param = [[ZegoAudioFrameParam alloc] init];
    param.sampleRate = [ZegoUtils intValue:paramMap[@"sampleRate"]];
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

- (void)sendTransparentMessage:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSString *roomID = call.arguments[@"roomID"];
    int senMode = [ZegoUtils intValue:call.arguments[@"sendMode"]];
    int senType = [ZegoUtils intValue:call.arguments[@"sendType"]];
    int timeOut = [ZegoUtils intValue:call.arguments[@"timeOut"]];

    FlutterStandardTypedData *content = call.arguments[@"content"];

    NSArray<NSDictionary *> *userListMap = call.arguments[@"recvUserList"];

    NSMutableArray<ZegoUser *> *userListObject = nil;
    if (userListMap && userListMap.count > 0) {
        userListObject = [[NSMutableArray alloc] init];
        for(NSDictionary *userMap in userListMap) {
            NSString *uid =  userMap[@"userID"];
            if(uid.length == 0)
                continue;
            ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:userMap[@"userID"] userName:userMap[@"userName"]];
            [userListObject addObject:userObject];
        }
    }

    ZegoRoomSendTransparentMessage *message = [[ZegoRoomSendTransparentMessage alloc] init];
    message.timeOut = timeOut;
    message.sendMode = (ZegoRoomTransparentMessageMode)senMode;
    message.sendType = (ZegoRoomTransparentMessageType)senType;
    message.recvUserList = userListObject;
    message.content = content.data;


    [[ZegoExpressEngine sharedEngine] sendTransparentMessage:message roomID:roomID callback:^(int errorCode) {
        result(@{
            @"errorCode": @(errorCode)
        });
    }];
}


#pragma mark - CustomVideoCapture

- (void)enableCustomVideoRender:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];

    ZegoCustomVideoRenderConfig *config = [[ZegoCustomVideoRenderConfig alloc] init];

    ZegoVideoBufferType bufferType = (ZegoVideoBufferType)[ZegoUtils intValue:configMap[@"bufferType"]];
    config.bufferType = bufferType;
    config.frameFormatSeries = (ZegoVideoFrameFormatSeries)[ZegoUtils intValue:configMap[@"frameFormatSeries"]];
    config.enableEngineRender = [ZegoUtils boolValue:configMap[@"enableEngineRender"]];
    
    if (config.enableEngineRender && !self.enablePlatformView) {
        if (enable) {
            [[ZegoTextureRendererController sharedInstance] setCustomVideoRenderHandler:[[ZegoCustomVideoRenderManager sharedInstance] getCustomVideoRenderHandler]];
        } else {
            [[ZegoTextureRendererController sharedInstance] setCustomVideoRenderHandler:nil];
        }
    } else {
        if (enable) {
            [[ZegoExpressEngine sharedEngine] setCustomVideoRenderHandler:(id<ZegoCustomVideoRenderHandler>)[ZegoCustomVideoRenderManager sharedInstance]];
        } else {
            [[ZegoExpressEngine sharedEngine] setCustomVideoRenderHandler:nil];
        }
        
        [[ZegoExpressEngine sharedEngine] enableCustomVideoRender:enable config: config];
    }

    result(nil);
}

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
    
    if (enable) {
        [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureHandler:(id<ZegoCustomVideoCaptureHandler>)[ZegoCustomVideoCaptureManager sharedInstance]];
    } else {
        [[ZegoExpressEngine sharedEngine] setCustomVideoCaptureHandler:nil];
    }

    
    [[ZegoExpressEngine sharedEngine] enableCustomVideoCapture:enable config:config channel:(ZegoPublishChannel)channel];

    // When using custom video capture, turn off preview mirroring
    if (enable) {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:ZegoVideoMirrorModeNoMirror channel:(ZegoPublishChannel)channel];
    } else {
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:ZegoVideoMirrorModeOnlyPreviewMirror channel:(ZegoPublishChannel)channel];
    }

    result(nil);
}

- (void)enableCustomVideoProcessing:(FlutterMethodCall *)call result:(FlutterResult)result {

    BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
    NSDictionary *configMap = call.arguments[@"config"];
    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];

    ZegoCustomVideoProcessConfig *config = [[ZegoCustomVideoProcessConfig alloc] init];

    ZegoVideoBufferType bufferType = (ZegoVideoBufferType)[ZegoUtils intValue:configMap[@"bufferType"]];
    config.bufferType = bufferType;

    if (enable) {
        [[ZegoExpressEngine sharedEngine] setCustomVideoProcessHandler:(id<ZegoCustomVideoProcessHandler>)[ZegoCustomVideoProcessManager sharedInstance]];
    } else {
        [[ZegoExpressEngine sharedEngine] setCustomVideoProcessHandler:nil];
    }
    
    [[ZegoExpressEngine sharedEngine] enableCustomVideoProcessing:(BOOL)enable config:config channel:(ZegoPublishChannel)channel];

    result(nil);
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
        if (!self.enablePlatformView) {
            [mediaPlayer setVideoHandler:[ZegoTextureRendererController sharedInstance] format:ZegoVideoFrameFormatBGRA32 type:ZegoVideoBufferTypeCVPixelBuffer];
        }
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
        [[ZegoTextureRendererController sharedInstance] unbindMediaPlayerIndex:index];
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
        BOOL alphaBlend = [ZegoUtils boolValue:canvasMap[@"alphaBlend"]];

        if (self.enablePlatformView) {
            // Render with PlatformView
            ZegoPlatformView *platformView = [[ZegoPlatformViewFactory sharedInstance] getPlatformView:@(viewID)];
#if TARGET_OS_IPHONE
            if (alphaBlend) {
                [platformView.view setBackgroundColor:UIColor.clearColor];
            }
#endif
            
            if (platformView) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:platformView.view];
                canvas.viewMode = (ZegoViewMode)viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;
                
                [mediaPlayer setPlayerCanvas:canvas];
            } else {
                // Media video without creating the PlatformView in advance
                // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                NSString *errorMessage = [NSString stringWithFormat:@"The PlatformView for viewID:%ld cannot be found, developer should call `createPlatformView` first and get the viewID", (long)viewID];
                ZGError(@"[mediaPlayerSetPlayerCanvas] %@", errorMessage);
                result([FlutterError errorWithCode:[@"mediaPlayerSetPlayerCanvas_No_PlatformView" uppercaseString] message:errorMessage details:nil]);
                return;
            }
        } else {
            // Render with Texture
            if (![[ZegoTextureRendererController sharedInstance] bindMediaPlayerIndex:index withTexture:viewID]) {
                // Media video without creating TextureRenderer in advance
                // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                NSString *errorMessage = [NSString stringWithFormat:@"The TextureRenderer for textureID:%ld cannot be found, developer should call `createCanvasView` first and get the textureID", (long)viewID];
                ZGError(@"[mediaPlayerSetPlayerCanvas] %@", errorMessage);
                result([FlutterError errorWithCode:[@"mediaPlayerSetPlayerCanvas_No_TextureRenderer" uppercaseString] message:errorMessage details:nil]);
                return;
            }
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
#if TARGET_OS_IPHONE
                    imageData = UIImageJPEGRepresentation(image, 1);
#elif TARGET_OS_OSX
                CGImageRef cgImage = [image CGImageForProposedRect:nil context:nil hints:nil];
                NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
                imageData = [bitmapRep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}];
#endif
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

- (void)mediaPlayerLoadCopyrightedMusicResourceWithPosition:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        NSString *resourceID = call.arguments[@"resourceID"];
        int startPosition = [ZegoUtils intValue:call.arguments[@"startPosition"]];

        [mediaPlayer loadCopyrightedMusicResourceWithPosition:resourceID startPosition:startPosition callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    }
}

- (void)mediaPlayerClearView:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        [mediaPlayer clearView];
    }

    result(nil);
}

- (void)mediaPlayerSetActiveAudioChannel:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        int audioChannel = [ZegoUtils intValue:call.arguments[@"audioChannel"]];
        [mediaPlayer setActiveAudioChannel: audioChannel];
    }

    result(nil);
}

- (void)mediaPlayerEnableAudioData:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        if (enable) {
            [mediaPlayer setAudioHandler:(id<ZegoMediaPlayerAudioHandler>)[ZegoMediaPlayerAudioManager sharedInstance]];
        } else {
            [mediaPlayer setAudioHandler:nil];
        }
    }

   result(nil);
}

- (void)mediaPlayerEnableVideoData:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        if (enable) {
            if (!self.enablePlatformView) {
                [[ZegoTextureRendererController sharedInstance] setMediaPlayerVideoHandle:[[ZegoMediaPlayerVideoManager sharedInstance] getMediaPlayerVideoHandler]];
            } else {
                int format = [ZegoUtils intValue:call.arguments[@"format"]];
                [mediaPlayer setVideoHandler:(id<ZegoMediaPlayerVideoHandler>)[ZegoMediaPlayerVideoManager sharedInstance] format:(ZegoVideoFrameFormat)format type:ZegoVideoBufferTypeCVPixelBuffer];
            }
        } else {
            if (!self.enablePlatformView) {
                [[ZegoTextureRendererController sharedInstance] setMediaPlayerVideoHandle: nil];
            } else {
                int format = [ZegoUtils intValue:call.arguments[@"format"]];
                [mediaPlayer setVideoHandler:nil format:(ZegoVideoFrameFormat)format type:ZegoVideoBufferTypeCVPixelBuffer];
            }
        }
    }

    result(nil);
}

- (void)mediaPlayerEnableBlockData:(FlutterMethodCall *)call result:(FlutterResult)result {

     NSNumber *index = call.arguments[@"index"];
     ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

     if (mediaPlayer) {
         BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
         unsigned int blockSize = [ZegoUtils unsignedIntValue:call.arguments[@"blockSize"]];
         if (enable) {
             [mediaPlayer setBlockDataHandler:(id<ZegoMediaPlayerBlockDataHandler>)[ZegoMediaPlayerBlockDataManager sharedInstance] blockSize:blockSize];
         } else {
             [mediaPlayer setBlockDataHandler:nil blockSize:blockSize];
         }
     }

    result(nil);
}

#if TARGET_OS_IPHONE
- (void)mediaPlayerSetAudioTrackMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        int mode = [ZegoUtils intValue:call.arguments[@"mode"]];
        [mediaPlayer setAudioTrackMode: mode];
    }

    result(nil);
}

- (void)mediaPlayerSetAudioTrackPublishIndex:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        int index_ = [ZegoUtils intValue:call.arguments[@"index_"]];
        [mediaPlayer setAudioTrackPublishIndex: index_];
    }

    result(nil);
}
#endif

- (void)mediaPlayerLoadResourceWithConfig:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {

        NSDictionary *resourceMap = call.arguments[@"resource"];
        ZegoMediaPlayerResource *resource = [[ZegoMediaPlayerResource alloc] init];
        resource.resourceID =  resourceMap[@"resourceID"];
        resource.startPosition = [ZegoUtils longLongValue:resourceMap[@"startPosition"]];
        resource.loadType = (ZegoMultimediaLoadType)[ZegoUtils intValue:resourceMap[@"loadType"]];
        resource.alphaLayout = (ZegoAlphaLayoutType)[ZegoUtils intValue:resourceMap[@"alphaLayout"]];
        resource.filePath =  resourceMap[@"filePath"];
        
        FlutterStandardTypedData *memory = resourceMap[@"memory"];
        resource.memory = memory.data;
        resource.memoryLength = (int)memory.data.length;

        [mediaPlayer loadResourceWithConfig:resource callback:^(int errorCode) {
            result(@{
                @"errorCode": @(errorCode)
            });
        }];
    } else {
        result([FlutterError errorWithCode:[@"loadResourceWithConfig_Can_not_find_player" uppercaseString] message:@"Invoke `loadResourceWithConfig` but can't find specific player" details:nil]);
    }
}

- (void)mediaPlayerUpdatePosition:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        FlutterStandardTypedData *positionArray = call.arguments[@"position"];
        float position[3];
        convertFloatArray(position, positionArray);
        [mediaPlayer updatePosition:position];
    }

    result(nil);
}

- (void)mediaPlayerGetMediaInfo:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        ZegoMediaPlayerMediaInfo *info = mediaPlayer.getMediaInfo;
        result(@{
            @"width": @(info.width),
            @"height": @(info.height),
            @"frameRate": @(info.frameRate),
        });
    } else {
        result(@{});
    }
}

- (void)mediaPlayerSetHttpHeader:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        NSDictionary *headersMap = call.arguments[@"headers"];
        [mediaPlayer setHttpHeader:headersMap];
    }

    result(nil);
}

- (void)mediaPlayerGetCurrentRenderingProgress:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        unsigned long long progress = mediaPlayer.currentRenderingProgress;
        result(@(progress));
    } else {
        result(@(0));
    }
}

- (void)mediaPlayerEnableLiveAudioEffect:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        ZegoLiveAudioEffectMode mode = (ZegoLiveAudioEffectMode) [ZegoUtils unsignedIntValue:call.arguments[@"mode"]];
        [mediaPlayer enableLiveAudioEffect:enable mode:mode];
    }
    
    result(nil);
}

- (void)mediaPlayerSetPlayMediaStreamType:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        ZegoMediaStreamType streamType = (ZegoMediaStreamType) [ZegoUtils unsignedIntValue:call.arguments[@"streamType"]];
        [mediaPlayer setPlayMediaStreamType:streamType];
    }
    
    result(nil);
}

- (void)mediaPlayerEnableLocalCache:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        BOOL enable = [ZegoUtils boolValue:call.arguments[@"enable"]];
        NSString *cacheDir = call.arguments[@"cacheDir"];
        [mediaPlayer enableLocalCache:enable cacheDir:cacheDir];
    }
    
    result(nil);
}


- (void)mediaPlayerGetPlaybackStatistics:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaPlayer *mediaPlayer = self.mediaPlayerMap[index];

    if (mediaPlayer) {
        ZegoMediaPlayerStatisticsInfo *info = [mediaPlayer getPlaybackStatistics];
        result(@{
            @"videoSourceFps" : @(info.videoSourceFps),
            @"videoDecodeFps" : @(info.videoDecodeFps),
            @"videoRenderFps" : @(info.videoRenderFps),
            @"audioSourceFps" : @(info.audioSourceFps),
            @"audioDecodeFps" : @(info.audioDecodeFps),
            @"audioRenderFps" : @(info.audioRenderFps),
        });
    } else {
        result(nil);
    }
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
            configObject.isPublishOut = [ZegoUtils boolValue:configMap[@"isPublishOut"]];
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

- (void)audioEffectPlayerSetPlayVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setPlayVolume:volume audioEffectID:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetPlayVolume_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetPlayVolume` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSetPublishVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setPublishVolume:volume audioEffectID:audioEffectID];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetPublishVolume_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetPublishVolume` but can't find specific player" details:nil]);
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

- (void)audioEffectPlayerSetPlayVolumeAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setPlayVolumeAll:volume];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetPlayVolumeAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetPlayVolumeAll` but can't find specific player" details:nil]);
    }
}

- (void)audioEffectPlayerSetPublishVolumeAll:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];
        [audioEffectPlayer setPublishVolumeAll:volume];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerSetPublishVolumeAll_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerSetPublishVolumeAll` but can't find specific player" details:nil]);
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

- (void)audioEffectPlayerUpdatePosition:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAudioEffectPlayer *audioEffectPlayer = self.audioEffectPlayerMap[index];

    if (audioEffectPlayer) {
        unsigned int audioEffectID = [ZegoUtils unsignedIntValue:call.arguments[@"audioEffectID"]];
        FlutterStandardTypedData *positionArray = call.arguments[@"position"];
        float position[3];
        convertFloatArray(position, positionArray);
        [audioEffectPlayer updatePosition:audioEffectID position:position];

        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"audioEffectPlayerUpdatePosition_Can_not_find_player" uppercaseString] message:@"Invoke `audioEffectPlayerUpdatePosition` but can't find specific player" details:nil]);
    }
}


#pragma mark - MediaDataPublisher

- (void)createMediaDataPublisher:(FlutterMethodCall *)call result:(FlutterResult)result {
    if (!self.mediaDataPublisherMap) {
        self.mediaDataPublisherMap = [NSMutableDictionary dictionary];
    }
    
    NSDictionary *configMap = call.arguments[@"config"];
    ZegoMediaDataPublisherConfig *configObject = nil;

    if (configMap && configMap.count > 0) {
        configObject = [[ZegoMediaDataPublisherConfig alloc] init];
        configObject.channel = [ZegoUtils intValue:configMap[@"channel"]];
        configObject.mode = [ZegoUtils intValue:configMap[@"mode"]];
    }
    
    ZegoMediaDataPublisher *instance = [[ZegoExpressEngine sharedEngine] createMediaDataPublisher:configObject];
    
    if (instance) {
        NSNumber *index = [instance getIndex];
        [instance setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.mediaDataPublisherMap[index] = instance;
        result(index);
    } else {
        result(@(-1));
    }
}

- (void)destroyMediaDataPublisher:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        [[ZegoExpressEngine sharedEngine] destroyMediaDataPublisher:publisher];
        [self.mediaDataPublisherMap removeObjectForKey:index];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"destroyMediaDataPublisher_Can_not_find_publisher" uppercaseString] message:@"Invoke `destroyMediaDataPublisher` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherAddMediaFilePath:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        NSString *path = call.arguments[@"path"];
        BOOL isClear = [ZegoUtils boolValue:call.arguments[@"isClear"]];
        [publisher addMediaFilePath:path isClear:isClear];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherAddMediaFilePath_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherAddMediaFilePath` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherGetCurrentDuration:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        unsigned long long duration = [publisher getCurrentDuration];
        result(@(duration));
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherGetCurrentDuration_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherGetCurrentDuration` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherGetTotalDuration:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        unsigned long long duration = [publisher getTotalDuration];
        result(@(duration));
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherGetTotalDuration_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherGetTotalDuration` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherReset:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        [publisher reset];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherReset_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherReset` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherSeekTo:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        unsigned long long ms = [ZegoUtils unsignedLongLongValue:call.arguments[@"millisecond"]];
        [publisher seekTo:ms];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherSeekTo_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherSeekTo` but can't find specific publisher" details:nil]);
    }
}

- (void)mediaDataPublisherSetVideoSendDelayTime:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoMediaDataPublisher *publisher = self.mediaDataPublisherMap[index];

    if (publisher) {
        int delayTime = [ZegoUtils intValue:call.arguments[@"delayTime"]];
        [publisher setVideoSendDelayTime:delayTime];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"mediaDataPublisherSetVideoSendDelayTime_Can_not_find_publisher" uppercaseString] message:@"Invoke `mediaDataPublisherSetVideoSendDelayTime` but can't find specific publisher" details:nil]);
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
        ZegoReceiveRangeParam *rangeParam = [[ZegoReceiveRangeParam alloc] init];
        NSDictionary *paramMap = call.arguments[@"param"];
        rangeParam.min = [ZegoUtils floatValue:paramMap[@"min"]];
        rangeParam.max = [ZegoUtils floatValue:paramMap[@"max"]];
        int ret = [self.rangeAudioInstance setAudioReceiveRangeWithParam:rangeParam];
        result(@(ret));

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
        FlutterStandardTypedData *positionArray = call.arguments[@"position"];
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
        FlutterStandardTypedData *positionArray = call.arguments[@"position"];
        FlutterStandardTypedData *axisForwardArray = call.arguments[@"axisForward"];
        FlutterStandardTypedData *axisRightArray = call.arguments[@"axisRight"];
        FlutterStandardTypedData *axisUpArray = call.arguments[@"axisUp"];
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

- (void)rangeAudioSetPositionUpdateFrequency:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        int frequency = [ZegoUtils intValue:call.arguments[@"frequency"]];

        [self.rangeAudioInstance setPositionUpdateFrequency:frequency];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetRangeAudioVolume:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        int volume = [ZegoUtils intValue:call.arguments[@"volume"]];

        [self.rangeAudioInstance setRangeAudioVolume:volume];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetRangeAudioVolume` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetStreamVocalRange:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSString* streamID = call.arguments[@"streamID"];
        ZegoVocalRangeParam *rangeParam = [[ZegoVocalRangeParam alloc] init];
        NSDictionary *paramMap = call.arguments[@"param"];
        rangeParam.min = [ZegoUtils floatValue:paramMap[@"min"]];
        rangeParam.max = [ZegoUtils floatValue:paramMap[@"max"]];

        int ret = [self.rangeAudioInstance setStreamVocalRangeWithParam:streamID param:rangeParam];
        result(@(ret));

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioUpdateStreamPosition:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        NSString* streamID = call.arguments[@"streamID"];
        FlutterStandardTypedData *positionArray = call.arguments[@"position"];
        float position[3];
        convertFloatArray(position, positionArray);
        [self.rangeAudioInstance updateStreamPosition:streamID position: position];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioUpdateStreamPosition` but can't find specific instance" details:nil]);
    }
}

- (void)rangeAudioSetRangeAudioCustomMode:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.rangeAudioInstance) {
        int speakMode = [ZegoUtils intValue:call.arguments[@"speakMode"]];
        int listenMode = [ZegoUtils intValue:call.arguments[@"listenMode"]];
        [self.rangeAudioInstance setRangeAudioCustomMode:(ZegoRangeAudioSpeakMode)speakMode listenMode:(ZegoRangeAudioListenMode)listenMode];
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"rangeAudio_Can_not_find_Instance" uppercaseString] message:@"Invoke `rangeAudioSetRangeAudioCustomMode` but can't find specific instance" details:nil]);
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

    unsigned int interval = [ZegoUtils unsignedIntValue:call.arguments[@"interval"]];
    [[ZegoExpressEngine sharedEngine] startNetworkSpeedTest:config interval: interval];

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

- (void)startDumpData:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoDumpDataConfig *config = [[ZegoDumpDataConfig alloc] init];
    config.dataType = (ZegoDumpDataType) [ZegoUtils unsignedIntValue:configMap[@"dataType"]];
    [[ZegoExpressEngine sharedEngine] startDumpData:config];
    
    result(nil);
}

- (void)stopDumpData:(FlutterMethodCall *)call result:(FlutterResult)result {
    [[ZegoExpressEngine sharedEngine] stopDumpData];

    result(nil);
}

- (void)uploadDumpData:(FlutterMethodCall *)call result:(FlutterResult)result {
    [[ZegoExpressEngine sharedEngine] uploadDumpData];

    result(nil);
}

- (void)removeDumpData:(FlutterMethodCall *)call result:(FlutterResult)result {
    [[ZegoExpressEngine sharedEngine] removeDumpData];

    result(nil);
}

#pragma mark - Copyrighted Music
- (void)createCopyrightedMusic:(FlutterMethodCall *)call result:(FlutterResult)result {

    ZegoCopyrightedMusic *copyrightedMusic = [[ZegoExpressEngine sharedEngine] createCopyrightedMusic];

    if (copyrightedMusic) {
        self.copyrightedMusicInstance = copyrightedMusic;
        [self.copyrightedMusicInstance setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        result(@(0));
    } else {
        result(@(-1));
    }
}

- (void)destroyCopyrightedMusic:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        [[ZegoExpressEngine sharedEngine] destroyCopyrightedMusic:self.copyrightedMusicInstance];
        [self.copyrightedMusicInstance setEventHandler:nil];
        self.copyrightedMusicInstance = nil;
        result(nil);

    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `destroyCopyrightedMusic` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicClearCache:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        [self.copyrightedMusicInstance clearCache];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicClearCache` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicDownload:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        [self.copyrightedMusicInstance download:resourceID callback:^(int errorCode) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicDownload` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicCancelDownload:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        [self.copyrightedMusicInstance cancelDownload:resourceID];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicCancelDownload` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetAverageScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int averageScore = [self.copyrightedMusicInstance getAverageScore: resourceID];
        result(@(averageScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetAverageScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetCacheSize:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        unsigned long long cacheSize = [self.copyrightedMusicInstance getCacheSize];
        result(@(cacheSize));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetCacheSize` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetCurrentPitch:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int currentPitch = [self.copyrightedMusicInstance getCurrentPitch: resourceID];
        result(@(currentPitch));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetDuration:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        unsigned long long duration = [self.copyrightedMusicInstance getDuration: resourceID];
        result(@(duration));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetDuration` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetKrcLyricByToken:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *krcToken = call.arguments[@"krcToken"];
        [self.copyrightedMusicInstance getKrcLyricByToken:krcToken callback:^(int errorCode, NSString *_Nonnull lyrics) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"lyrics"] = lyrics;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetKrcLyricByToken` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetLrcLyric:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *songID = call.arguments[@"songID"];
        if ([ZegoUtils isNullObject:call.arguments[@"vendorID"]]) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [self.copyrightedMusicInstance getLrcLyric:songID callback:^(int errorCode, NSString *_Nonnull lyrics) {
#pragma clang diagnostic pop
                NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
                resultMap[@"errorCode"] = @(errorCode);
                resultMap[@"lyrics"] = lyrics;
                result(resultMap);
            }];
        } else {
            int vendorID = [ZegoUtils intValue:call.arguments[@"vendorID"]];
            [self.copyrightedMusicInstance getLrcLyric:songID vendorID:(ZegoCopyrightedMusicVendorID)vendorID callback:^(int errorCode, NSString *_Nonnull lyrics) {
                NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
                resultMap[@"errorCode"] = @(errorCode);
                resultMap[@"lyrics"] = lyrics;
                result(resultMap);
            }];
        }
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetLrcLyric` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetLrcLyricWithConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicGetLyricConfig *config = [[ZegoCopyrightedMusicGetLyricConfig alloc] init];
        
        config.songID = configMap[@"songID"];
        config.vendorID = [ZegoUtils unsignedIntValue:configMap[@"vendorID"]];

        [self.copyrightedMusicInstance getLrcLyricWithConfig:config callback:^(int errorCode, NSString *_Nonnull lyrics) {
                NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
                resultMap[@"errorCode"] = @(errorCode);
                resultMap[@"lyrics"] = lyrics;
                result(resultMap);
            }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetLrcLyricWithConfig` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetMusicByToken:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *shareToken = call.arguments[@"shareToken"];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        [self.copyrightedMusicInstance getMusicByToken:shareToken callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
#pragma clang diagnostic pop
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetMusicByToken` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetPreviousScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int previousScore = [self.copyrightedMusicInstance getPreviousScore: resourceID];
        result(@(previousScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetStandardPitch:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        [self.copyrightedMusicInstance getStandardPitch:resourceID callback:^(int errorCode, NSString *_Nonnull pitch) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"pitch"] = pitch;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetStandardPitch` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetTotalScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int totalScore = [self.copyrightedMusicInstance getTotalScore: resourceID];
        result(@(totalScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetTotalScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicInitCopyrightedMusic:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicConfig *config = [[ZegoCopyrightedMusicConfig alloc] init];
        
        NSDictionary *userMap = configMap[@"user"];
        ZegoUser *user = [[ZegoUser alloc] init];
        user.userID = userMap[@"userID"];
        user.userName = userMap[@"userName"];
        
        config.user = user;
        
        [self.copyrightedMusicInstance initCopyrightedMusic:config callback:^(int errorCode) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicInitCopyrightedMusic` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicPauseScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int pauseScore = [self.copyrightedMusicInstance pauseScore: resourceID];
        result(@(pauseScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicPauseScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicQueryCache:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *songID = call.arguments[@"songID"];
        int type = [ZegoUtils intValue:call.arguments[@"type"]];
        BOOL isQueryCache = false;
        if ([ZegoUtils isNullObject:call.arguments[@"vendorID"]]) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            isQueryCache = [self.copyrightedMusicInstance queryCache: songID type:(ZegoCopyrightedMusicType)type];
#pragma clang diagnostic pop
        } else {
            int vendorID = [ZegoUtils intValue:call.arguments[@"vendorID"]];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            isQueryCache = [self.copyrightedMusicInstance queryCache:songID type:(ZegoCopyrightedMusicType)type vendorID:(ZegoCopyrightedMusicVendorID)vendorID];
#pragma clang diagnostic pop
        }
        result(@(isQueryCache));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicQueryCache` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicQueryCacheWithConfig:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicQueryCacheConfig *config = [[ZegoCopyrightedMusicQueryCacheConfig alloc] init];
        
        config.songID = configMap[@"songID"];
        config.vendorID = [ZegoUtils unsignedIntValue:configMap[@"vendorID"]];
        config.resourceType = [ZegoUtils unsignedIntValue:configMap[@"resourceType"]];
        config.resourceQualityType = [ZegoUtils unsignedIntValue:configMap[@"resourceQualityType"]];
        
        BOOL isQueryCache = [self.copyrightedMusicInstance queryCache:config];
        result(@(isQueryCache));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicQueryCacheWithConfig` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicQueryCacheWithConfigV2:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicQueryCacheConfigV2 *config = [[ZegoCopyrightedMusicQueryCacheConfigV2 alloc] init];
        
        config.songID = configMap[@"songID"];
        config.vendorID = [ZegoUtils unsignedIntValue:configMap[@"vendorID"]];
        config.resourceType = [ZegoUtils unsignedIntValue:configMap[@"resourceType"]];
        config.resourceQualityType = [ZegoUtils unsignedIntValue:configMap[@"resourceQualityType"]];
        
        BOOL isQueryCache = [self.copyrightedMusicInstance queryCacheV2:config];
        result(@(isQueryCache));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicQueryCacheWithConfigV2` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicRequestAccompaniment:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicRequestConfig *config = [[ZegoCopyrightedMusicRequestConfig alloc] init];
        config.songID = configMap[@"songID"];
        config.mode = (ZegoCopyrightedMusicBillingMode)[ZegoUtils intValue:configMap[@"mode"]];
        config.vendorID = (ZegoCopyrightedMusicVendorID)[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.masterID = configMap[@"masterID"];
        config.sceneID = [ZegoUtils intValue:configMap[@"sceneID"]];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        [self.copyrightedMusicInstance requestAccompaniment:config callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
#pragma clang diagnostic pop
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicRequestAccompaniment` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicRequestAccompanimentClip:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicRequestConfig *config = [[ZegoCopyrightedMusicRequestConfig alloc] init];
        config.songID = configMap[@"songID"];
        config.mode = (ZegoCopyrightedMusicBillingMode)[ZegoUtils intValue:configMap[@"mode"]];
        config.vendorID = (ZegoCopyrightedMusicVendorID)[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.masterID = configMap[@"masterID"];
        config.sceneID = [ZegoUtils intValue:configMap[@"sceneID"]];
        
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        [self.copyrightedMusicInstance requestAccompanimentClip:config callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
#pragma clang diagnostic pop
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicRequestAccompanimentClip` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicRequestSong:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicRequestConfig *config = [[ZegoCopyrightedMusicRequestConfig alloc] init];
        config.songID = configMap[@"songID"];
        config.mode = (ZegoCopyrightedMusicBillingMode)[ZegoUtils intValue:configMap[@"mode"]];
        config.vendorID = (ZegoCopyrightedMusicVendorID)[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.masterID = configMap[@"masterID"];
        config.sceneID = [ZegoUtils intValue:configMap[@"sceneID"]];
    
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        [self.copyrightedMusicInstance requestSong:config callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
#pragma clang diagnostic pop
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicRequestSong` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicResetScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int resetScore = [self.copyrightedMusicInstance resetScore: resourceID];
        result(@(resetScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicResetScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicResumeScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int resumeScore = [self.copyrightedMusicInstance resumeScore: resourceID];
        result(@(resumeScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicResumeScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicSendExtendedRequest:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *command = call.arguments[@"command"];
        NSString *params = call.arguments[@"params"];
        
        [self.copyrightedMusicInstance sendExtendedRequest:command params:params callback:^(int errorCode, NSString *_Nonnull command, NSString *_Nonnull result_) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"command"] = command;
            resultMap[@"result"] = result_;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicSendExtendedRequest` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicSetScoringLevel:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        int level = [ZegoUtils intValue:call.arguments[@"level"]];
        [self.copyrightedMusicInstance setScoringLevel: level];
        result(nil);
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicSetScoringLevel` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicStartScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int pitchValueInterval = [ZegoUtils intValue:call.arguments[@"pitchValueInterval"]];
        int startScore = [self.copyrightedMusicInstance startScore: resourceID pitchValueInterval:pitchValueInterval];
        result(@(startScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicStartScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicStopScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int stopScore = [self.copyrightedMusicInstance stopScore: resourceID];
        result(@(stopScore));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicStopScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetFullScore:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSString *resourceID = call.arguments[@"resourceID"];
        int score = [self.copyrightedMusicInstance getFullScore: resourceID];
        result(@(score));
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetFullScore` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetSharedResource:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicGetSharedConfig *config = [[ZegoCopyrightedMusicGetSharedConfig alloc] init];
        config.songID = configMap[@"songID"];
        config.vendorID = (ZegoCopyrightedMusicVendorID)[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        
        ZegoCopyrightedMusicResourceType type = (ZegoCopyrightedMusicResourceType)[ZegoUtils intValue:call.arguments[@"type"]];
        [self.copyrightedMusicInstance getSharedResource: config type: type callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetSharedResource` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicGetSharedResourceV2:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicGetSharedConfigV2 *config = [[ZegoCopyrightedMusicGetSharedConfigV2 alloc] init];
        config.songID = configMap[@"songID"];
        config.vendorID = [ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.resourceType = [ZegoUtils intValue:configMap[@"resourceType"]];
        
        [self.copyrightedMusicInstance getSharedResource: config callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicGetSharedResourceV2` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicRequestResource:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicRequestConfig *config = [[ZegoCopyrightedMusicRequestConfig alloc] init];
        config.songID = configMap[@"songID"];
        config.mode = (ZegoCopyrightedMusicBillingMode)[ZegoUtils intValue:configMap[@"mode"]];
        config.vendorID = (ZegoCopyrightedMusicVendorID)[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.masterID = configMap[@"masterID"];
        config.sceneID = [ZegoUtils intValue:configMap[@"sceneID"]];
        
        ZegoCopyrightedMusicResourceType type = (ZegoCopyrightedMusicResourceType)[ZegoUtils intValue:call.arguments[@"type"]];
        [self.copyrightedMusicInstance requestResource: config type: type callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicRequestResource` but can't find specific instance" details:nil]);
    }
}

- (void)copyrightedMusicRequestResourceV2:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (self.copyrightedMusicInstance) {
        NSDictionary *configMap = call.arguments[@"config"];
        
        ZegoCopyrightedMusicRequestConfigV2 *config = [[ZegoCopyrightedMusicRequestConfigV2 alloc] init];
        config.songID = configMap[@"songID"];
        config.mode = [ZegoUtils intValue:configMap[@"mode"]];
        config.vendorID =[ZegoUtils intValue:configMap[@"vendorID"]];
        config.roomID = configMap[@"roomID"];
        config.masterID = configMap[@"masterID"];
        config.sceneID = [ZegoUtils intValue:configMap[@"sceneID"]];
        config.resourceType = [ZegoUtils intValue:configMap[@"resourceType"]];
        
        [self.copyrightedMusicInstance requestResource: config callback:^(int errorCode, NSString *_Nonnull resource) {
            NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
            resultMap[@"errorCode"] = @(errorCode);
            resultMap[@"resource"] = resource;
            result(resultMap);
        }];
    } else {
        result([FlutterError errorWithCode:[@"copyrightedMusic_Can_not_find_Instance" uppercaseString] message:@"Invoke `copyrightedMusicRequestResourceV2` but can't find specific instance" details:nil]);
    }
}

#pragma mark - ScreenCapture
#if TARGET_OS_OSX
- (void)getScreenCaptureSources:(FlutterMethodCall *)call result:(FlutterResult)result {

    int thumbnailWidth = [ZegoUtils intValue: call.arguments[@"thumbnailWidth"]];
    int thumbnailHeight = [ZegoUtils intValue: call.arguments[@"thumbnailHeight"]];
    int iconWidth = [ZegoUtils intValue: call.arguments[@"iconWidth"]];
    int iconHeight = [ZegoUtils intValue: call.arguments[@"iconHeight"]];
    
    CGSize thumbnailSize = CGSizeMake((CGFloat)thumbnailWidth, (CGFloat)thumbnailHeight);
    CGSize iconSize = CGSizeMake((CGFloat)iconWidth, (CGFloat)iconHeight);
    
    NSArray<ZegoScreenCaptureSourceInfo *> *infos = [[ZegoExpressEngine sharedEngine] getScreenCaptureSourcesWithThumbnailSize:thumbnailSize iconSize: iconSize];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSMutableArray *resultMap = [[NSMutableArray alloc] init];
        for (ZegoScreenCaptureSourceInfo *info in infos) {
            
            NSData *thumbnailImage = nil;
            if (info.thumbnailImage) {
                CGImageRef cgImage = [info.thumbnailImage CGImageForProposedRect:nil context:nil hints:nil];
                if (cgImage) {
                    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
                    thumbnailImage = [bitmapRep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}];
                }
            }
            
            NSData *iconImage = nil;
            if (info.iconImage) {
                CGImageRef cgImage = [info.iconImage CGImageForProposedRect:nil context:nil hints:nil];
                if (cgImage) {
                    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
                    iconImage = [bitmapRep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}];
                }
            }
            
            [resultMap addObject: @{
                @"sourceType": @((int)info.sourceType),
                @"sourceID": @((int)info.sourceID),
                @"sourceName": info.sourceName,
                @"thumbnailImage": thumbnailImage ?: [[NSNull alloc] init],
                @"iconImage": iconImage ?: [[NSNull alloc] init],
            }];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            result(resultMap);
        });
    });
}

- (void)getCaptureSourceRectScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        CGRect rect = [screenCaptureSource getCaptureSourceRect];
        
        NSMutableDictionary *resultMap = [[NSMutableDictionary alloc] init];
        resultMap[@"x"] = @(rect.origin.x);
        resultMap[@"y"] = @(rect.origin.y);
        resultMap[@"width"] = @(rect.size.width);
        resultMap[@"height"] = @(rect.size.height);
        result(resultMap);
    } else {
        result(nil);
    }
}

- (void)createScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.screenCaptureSouceMap) {
        self.screenCaptureSouceMap = [NSMutableDictionary dictionary];
    }
    
    unsigned int sourceId = [ZegoUtils unsignedIntValue: call.arguments[@"sourceId"]];
    int sourceType = [ZegoUtils intValue: call.arguments[@"sourceType"]];

    ZegoScreenCaptureSource *screenCaptureSource = [[ZegoExpressEngine sharedEngine] createScreenCaptureSource: sourceId sourceType: (ZegoScreenCaptureSourceType)sourceType];

    if (screenCaptureSource) {
        NSNumber *index = [screenCaptureSource getIndex];

        [screenCaptureSource setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.screenCaptureSouceMap[index] = screenCaptureSource;

        result(index);
    } else {
        result(@(-1));
    }
}

- (void)destroyScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.screenCaptureSouceMap) {
        self.screenCaptureSouceMap = [NSMutableDictionary dictionary];
    }
    
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        [[ZegoExpressEngine sharedEngine] destroyScreenCaptureSource:screenCaptureSource];
    }

    [self.screenCaptureSouceMap removeObjectForKey:index];

    result(nil);
}

- (void)enableCursorVisibleScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        BOOL visible = [ZegoUtils boolValue: call.arguments[@"visible"]];
        [screenCaptureSource enableCursorVisible:visible];
    }

    result(nil);
}

- (void)enableWindowActivateScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        BOOL active = [ZegoUtils boolValue: call.arguments[@"active"]];
        [screenCaptureSource enableWindowActivate:active];
    }

    result(nil);
}

- (void)setExcludeWindowListScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        
        NSArray<NSNumber *> *list = call.arguments[@"list"];
        
        [screenCaptureSource setExcludeWindowList:list];
    }

    result(nil);
}

- (void)updateCaptureRegionScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        NSDictionary *rectMap = call.arguments[@"rect"];
        
        int x = [ZegoUtils intValue:rectMap[@"x"]];
        int y = [ZegoUtils intValue:rectMap[@"y"]];
        int width = [ZegoUtils intValue:rectMap[@"width"]];
        int height = [ZegoUtils intValue:rectMap[@"height"]];
        
        CGRect rect = CGRectMake(x, y, width, height);
        
        [screenCaptureSource updateCaptureRegion:rect];
    }

    result(nil);
}

- (void)updatePublishRegionScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        NSDictionary *rectMap = call.arguments[@"rect"];
        
        int x = [ZegoUtils intValue:rectMap[@"x"]];
        int y = [ZegoUtils intValue:rectMap[@"y"]];
        int width = [ZegoUtils intValue:rectMap[@"width"]];
        int height = [ZegoUtils intValue:rectMap[@"height"]];
        
        CGRect rect = CGRectMake(x, y, width, height);
        
        [screenCaptureSource updatePublishRegion:rect];
    }

    result(nil);
}

- (void)updateCaptureSourceScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        unsigned int sourceId = [ZegoUtils unsignedIntValue:call.arguments[@"sourceId"]];
        int sourceType = [ZegoUtils intValue:call.arguments[@"sourceType"]];
        
        [screenCaptureSource updateCaptureSource:sourceId sourceType:(ZegoScreenCaptureSourceType) sourceType];
    }

    result(nil);
}
#elif TARGET_OS_IPHONE
- (void)updateScreenCaptureConfigScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSDictionary *configMap = call.arguments[@"config"];
    
    ZegoScreenCaptureConfig *config = [[ZegoScreenCaptureConfig alloc] init];
        
    config.captureVideo = [ZegoUtils boolValue:configMap[@"captureVideo"]];
    config.captureAudio = [ZegoUtils boolValue:configMap[@"captureAudio"]];
    config.applicationVolume = [ZegoUtils intValue:configMap[@"applicationVolume"]];
    config.microphoneVolume = [ZegoUtils intValue:configMap[@"microphoneVolume"]];

    if (@available(iOS 12.0, *)) {
        [[ZegoExpressEngine sharedEngine] updateScreenCaptureConfig:config];
        result(nil);
    } else {
        // Fallback on earlier versions
        result([FlutterError errorWithCode:[@"System_Version_Is_Too_Low" uppercaseString] message:@"Only available on iOS 12.0 or newer" details:nil]);
    }
}

- (void)setAppGroupIDScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    if (@available(iOS 12.0, *)) {
        NSString *groupID = call.arguments[@"groupID"];
        [[ZegoExpressEngine sharedEngine] setAppGroupID:groupID];
        result(nil);
    } else {
        // Fallback on earlier versions
        result([FlutterError errorWithCode:[@"System_Version_Is_Too_Low" uppercaseString] message:@"Only available on iOS 12.0 or newer" details:nil]);
    }
}
#endif

- (void)startCaptureScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    if (@available(iOS 12.0, *)) {
        BOOL inApp = NO;
        if (![ZegoUtils isNullObject:call.arguments[@"inApp"]]) {
            inApp = [ZegoUtils boolValue:call.arguments[@"inApp"]];
        }
        
        ZegoScreenCaptureConfig *config = [[ZegoScreenCaptureConfig alloc] init];
        if (![ZegoUtils isNullObject:call.arguments[@"config"]]) {
            NSDictionary *configMap = call.arguments[@"config"];
            config.captureAudio = [ZegoUtils boolValue:configMap[@"captureAudio"]];
            config.captureVideo = [ZegoUtils boolValue:configMap[@"captureVideo"]];
            config.applicationVolume = [ZegoUtils intValue:configMap[@"applicationVolume"]];
            config.microphoneVolume = [ZegoUtils intValue:configMap[@"microphoneVolume"]];
        }
        
        if (inApp) {
            [[ZegoExpressEngine sharedEngine] startScreenCaptureInApp:config];
        } else {
            [[ZegoExpressEngine sharedEngine] startScreenCapture:config];
            
            RPSystemBroadcastPickerView *broadcastPickerView = [[RPSystemBroadcastPickerView alloc] initWithFrame:CGRectMake(0, 0, 44, 44)];
            NSArray *bundlePathArray = [[NSBundle mainBundle] pathsForResourcesOfType:@"appex" inDirectory:@"PlugIns"];
            if (bundlePathArray.firstObject) {
                NSBundle *bundle = [NSBundle bundleWithPath:bundlePathArray.firstObject];
                if (bundle) {
                    broadcastPickerView.preferredExtension = bundle.bundleIdentifier;
                    for (UIView *subView in broadcastPickerView.subviews) {
                        if ([subView isMemberOfClass:[UIButton class]]) {
                            UIButton *button = (UIButton *)subView;
                            [button sendActionsForControlEvents:UIControlEventAllEvents];
                        }
                    }
                }
            }
        }
    } else {
        // Fallback on earlier versions
        result([FlutterError errorWithCode:[@"System_Version_Is_Too_Low" uppercaseString] message:@"Only available on iOS 12.0 or newer" details:nil]);
        return;
    }
    
#elif TARGET_OS_OSX
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        [screenCaptureSource startCapture];
    }
#endif
    result(nil);
}

- (void)stopCaptureScreenCaptureSource:(FlutterMethodCall *)call result:(FlutterResult)result {
#if TARGET_OS_IPHONE
    if (@available(iOS 12.0, *)) {
        [[ZegoExpressEngine sharedEngine] stopScreenCapture];
    } else {
        // Fallback on earlier versions
        result([FlutterError errorWithCode:[@"System_Version_Is_Too_Low" uppercaseString] message:@"Only available on iOS 12.0 or newer" details:nil]);
        return;
    }
    
#elif TARGET_OS_OSX
    NSNumber *index = call.arguments[@"index"];
    ZegoScreenCaptureSource *screenCaptureSource = self.screenCaptureSouceMap[index];

    if (screenCaptureSource) {
        [screenCaptureSource stopCapture];
    }
#endif
    result(nil);
}

#pragma mark - AIVoiceChanger

- (void)createAIVoiceChanger:(FlutterMethodCall *)call result:(FlutterResult)result {

    if (!self.aiVoiceChangerMap) {
        self.aiVoiceChangerMap = [NSMutableDictionary dictionary];
    }

    ZegoAIVoiceChanger *aiVoiceChanger = [[ZegoExpressEngine sharedEngine] createAIVoiceChanger];

    if (aiVoiceChanger) {
        int index = aiVoiceChanger.getIndex;

        [aiVoiceChanger setEventHandler:[ZegoExpressEngineEventHandler sharedInstance]];
        self.aiVoiceChangerMap[@(index)] = aiVoiceChanger;

        result(@(index));
    } else {
        result(@(-1));
    }
}

- (void)destroyAIVoiceChanger:(FlutterMethodCall *)call result:(FlutterResult)result {

    NSNumber *index = call.arguments[@"index"];
    ZegoAIVoiceChanger *aiVoiceChanger = self.aiVoiceChangerMap[index];

    if (aiVoiceChanger) {
        [aiVoiceChanger setEventHandler:nil];
        [[ZegoExpressEngine sharedEngine] destroyAIVoiceChanger:aiVoiceChanger];
    }

    [self.aiVoiceChangerMap removeObjectForKey:index];

    result(nil);
}

- (void)aiVoiceChangerGetSpeakerList:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoAIVoiceChanger *aiVoiceChanger = self.aiVoiceChangerMap[index];

    if (aiVoiceChanger) {
        [aiVoiceChanger getSpeakerList];
    }

    result(nil);
}

- (void)aiVoiceChangerInitEngine:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoAIVoiceChanger *aiVoiceChanger = self.aiVoiceChangerMap[index];

    if (aiVoiceChanger) {
        [aiVoiceChanger initEngine];
    }

    result(nil);
}

- (void)aiVoiceChangerUpdate:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoAIVoiceChanger *aiVoiceChanger = self.aiVoiceChangerMap[index];

    if (aiVoiceChanger) {
        [aiVoiceChanger update];
    }

    result(nil);
}

- (void)aiVoiceChangerSetSpeaker:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    NSNumber *index = call.arguments[@"index"];
    ZegoAIVoiceChanger *aiVoiceChanger = self.aiVoiceChangerMap[index];

    if (aiVoiceChanger) {
        int speakerID = [ZegoUtils intValue:call.arguments[@"speakerID"]];
        [aiVoiceChanger setSpeaker:speakerID];
    }

    result(nil);
}
- (void)enableColorEnhancement:(FlutterMethodCall *)call result:(FlutterResult)result {
    
    BOOL enable = [ZegoUtils boolValue: call.arguments[@"enable"]];

    NSDictionary *paramsMap = call.arguments[@"params"];
    
    ZegoColorEnhancementParams *p = [[ZegoColorEnhancementParams alloc] init];
    p.intensity = [ZegoUtils floatValue:paramsMap[@"intensity"]];
    p.skinToneProtectionLevel = [ZegoUtils floatValue:paramsMap[@"skinToneProtectionLevel"]];
    p.lipColorProtectionLevel = [ZegoUtils floatValue:paramsMap[@"lipColorProtectionLevel"]];

    int channel = [ZegoUtils intValue:call.arguments[@"channel"]];
        
    [[ZegoExpressEngine sharedEngine] enableColorEnhancement:enable params:p channel:(ZegoPublishChannel)channel];
    
    result(nil);
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

- (void)destroyTextureRenderer:(FlutterMethodCall *)call result:(FlutterResult)result {

    int64_t textureID = [ZegoUtils longLongValue:call.arguments[@"textureID"]];
    BOOL state = [[ZegoTextureRendererController sharedInstance] destroyTextureRenderer:textureID];

    ZGLog(@"[destroyTextureRenderer][Result] textureID: %ld, success: %@", (long)textureID, state ? @"true" : @"false");

    result(@(state));
}

#pragma mark - Assets Utils

- (void)getAssetAbsolutePath:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSString *assetPath = call.arguments[@"assetPath"];
#if TARGET_OS_IPHONE
    NSString *assetKey = [_registrar lookupKeyForAsset:assetPath];
    NSString *realPath = [[NSBundle mainBundle] pathForResource:assetKey ofType:nil];
#elif TARGET_OS_OSX
    NSString *assetDir = @"/Contents/Frameworks/App.framework/Resources/flutter_assets/";
    NSString *realPath = [NSString stringWithFormat:@"%@%@%@", [[NSBundle mainBundle] bundlePath], assetDir, assetPath];
#endif
    ZGLog(@"[getAssetAbsolutePath] assetPath: %@, realPath: %@", assetPath, realPath);
    result(realPath);
}

#pragma mark - Private

- (void)setPluginVersion:(FlutterMethodCall *)call result:(FlutterResult)result {
    NSString *version = call.arguments[@"version"];

    ZGLog(@"*** Plugin Version: %@", version);
    
    result(nil);
}

void convertFloatArray(float *position, FlutterStandardTypedData *list) {
    const float* tmp = list.data.bytes;
    for (int i = 0; i < list.elementCount; i++) {
        position[i] = *(tmp + i);
    }
}

- (void)reportPluginInfo {

    if (self.pluginReported) { return; }

    self.pluginReported = true;

    NSDictionary *advancedConfigMap = @{@"thirdparty_framework_info": @"flutter"};

    ZegoEngineConfig *configObject = [[ZegoEngineConfig alloc] init];
    configObject.advancedConfig = advancedConfigMap;

    [ZegoExpressEngine setEngineConfig:configObject];
}

@end
