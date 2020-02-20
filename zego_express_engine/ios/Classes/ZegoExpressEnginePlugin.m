#import "ZegoExpressEnginePlugin.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>
#import "ZegoFlutterUtils.h"

@interface ZegoExpressEnginePlugin()<FlutterStreamHandler, ZegoEventHandler>

@property (nonatomic, assign) BOOL isEnablePlatformView;
@property (nonatomic, strong) NSObject<FlutterPluginRegistrar> *registrar;

@end

@implementation ZegoExpressEnginePlugin
{
    volatile FlutterEventSink _eventSink;
}

- (instancetype)initWithRegistrar:(NSObject<FlutterPluginRegistrar> *)registrar {
    if (self = [super init]) {
        
        _registrar = registrar;
        _isEnablePlatformView = NO;
    }
    
    return self;
}

- (void)dealloc {
    if([ZegoExpressEngine sharedEngine]) {
        [ZegoExpressEngine destroyEngine];
    }
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    
    ZegoExpressEnginePlugin* instance = [[ZegoExpressEnginePlugin alloc] initWithRegistrar:registrar];
    
    FlutterMethodChannel* methodChannel = [FlutterMethodChannel
      methodChannelWithName:@"plugins.zego.im/zego_express_engine"
            binaryMessenger:[registrar messenger]];
    [registrar addMethodCallDelegate:instance channel:methodChannel];
    
    FlutterEventChannel* eventChannel = [FlutterEventChannel eventChannelWithName:@"plugins.zego.im/zego_express_event_handler" binaryMessenger:[registrar messenger]];
    [eventChannel setStreamHandler:instance];
}

/**
 * Sets up an event stream and begin emitting events.
 *
 * Invoked when the first listener is registered with the Stream associated to
 * this channel on the Flutter side.
 *
 * @param arguments Arguments for the stream.
 * @param events A callback to asynchronously emit events. Invoke the
 *     callback with a `FlutterError` to emit an error event. Invoke the
 *     callback with `FlutterEndOfEventStream` to indicate that no more
 *     events will be emitted. Any other value, including `nil` are emitted as
 *     successful events.
 * @return A FlutterError instance, if setup fails.
 */
- (FlutterError* _Nullable)onListenWithArguments:(id _Nullable)arguments
                                       eventSink:(FlutterEventSink)events {
    _eventSink = events;
    return nil;
}

/**
 * Tears down an event stream.
 *
 * Invoked when the last listener is deregistered from the Stream associated to
 * this channel on the Flutter side.
 *
 * The channel implementation may call this method with `nil` arguments
 * to separate a pair of two consecutive set up requests. Such request pairs
 * may occur during Flutter hot restart.
 *
 * @param arguments Arguments for the stream.
 * @return A FlutterError instance, if teardown fails.
 */
- (FlutterError* _Nullable)onCancelWithArguments:(id _Nullable)arguments {
    _eventSink = nil;
    return nil;
}

- (void)throwCreateEngineError:(FlutterResult)result ofMethodName:(NSString*)methodName {
    NSString *errorMessage = [NSString stringWithFormat:@"[ERROR]: %@ %@", methodName, @"Create engine error! Please check the parameters for errors."];
    //[ZegoLog logNotice:[NSString stringWithFormat:@"[Flutter-Native] %@", errorMessage]];
    result([FlutterError errorWithCode:[[NSString stringWithFormat:@"%@_ERROR", methodName] uppercaseString] message:errorMessage details:nil]);
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
    NSDictionary *args = call.arguments;
    if([@"getSDKVersion" isEqualToString:call.method]) {
        
        result([ZegoExpressEngine getVersion]);
        
    } else if ([@"enablePlatformView" isEqualToString:call.method]) {
      
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        _isEnablePlatformView = enable;
        result(nil);
        
    } else if([@"createEngine" isEqualToString:call.method]) {
        
        unsigned int appID = [ZegoFlutterUtils numberToUintValue:args[@"appID"]];
        NSString *appSign = args[@"appID"];
        BOOL isTestEnv = [ZegoFlutterUtils numberToBoolValue:args[@"isTestEnv"]];
        int scenario = [ZegoFlutterUtils numberToIntValue:args[@"scenario"]];
        
        if([ZegoExpressEngine createEngineWithAppID:appID appSign:appSign isTestEnv:isTestEnv scenario:scenario eventHandler:self]) {
            NSLog(@"create engine success");
            result(@(0));
        } else {
            [self throwCreateEngineError:result ofMethodName:call.method];
        }
      
    } else if([@"destroyEngine" isEqualToString:call.method]) {
        
        if([ZegoExpressEngine sharedEngine]) {
            [ZegoExpressEngine destroyEngine];
        }
        
        result(nil);
      
    } else if([@"uploadLog" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] uploadLog];
        result(nil);
      
    } else if([@"setDebugVerbose" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        int language = [ZegoFlutterUtils numberToIntValue:args[@"language"]];
        [[ZegoExpressEngine sharedEngine] setDebugVerbose:enable language:language];
        result(nil);
      
    } else if([@"loginRoom" isEqualToString:call.method]) {
      
        NSString *roomID = args[@"roomID"];
        NSDictionary *user = args[@"user"];
        NSDictionary *config = args[@"config"];
        NSString *token = args[@"token"];
        
        ZegoUser* userObject = [[ZegoUser alloc] initWithUserID:user[@"userID"] userName:user[@"userName"]];
        ZegoRoomConfig* configObject = nil;
        if(config) {
            int maxMemberCount = [ZegoFlutterUtils numberToIntValue:config[@"maxMemberCount"]];
            BOOL isUserStatusNotify = [ZegoFlutterUtils numberToBoolValue:config[@"isUserStatusNotify"]];
            configObject = [[ZegoRoomConfig alloc] initWithMaxMemberCount:maxMemberCount isUserStatusNotify:isUserStatusNotify];
        }
        
        if(token) {
            [[ZegoExpressEngine sharedEngine] loginRoom:roomID user:userObject config:configObject token: token];
        } else {
            [[ZegoExpressEngine sharedEngine] loginRoom:roomID user:userObject config:configObject];
        }
        
        result(nil);
      
    } else if([@"logoutRoom" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        [[ZegoExpressEngine sharedEngine] logoutRoom:roomID];
        result(nil);
      
    } else if([@"startPublishing" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        [[ZegoExpressEngine sharedEngine] startPublishing:streamID];
        result(nil);
      
    } else if([@"stopPublishing" isEqualToString:call.method]) {
      
        [[ZegoExpressEngine sharedEngine] stopPublishing];
        result(nil);
        
    } else if([@"setStreamExtraInfo" isEqualToString:call.method]) {
      
        NSString *extraInfo = args[@"extraInfo"];
        [[ZegoExpressEngine sharedEngine] setStreamExtraInfo:extraInfo callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
        
    } else if([@"startPreview" isEqualToString:call.method]) {
      
    } else if([@"stopPreview" isEqualToString:call.method]) {
      
    } else if([@"setVideoConfig" isEqualToString:call.method]) {
      
        NSDictionary *config = args[@"config"];
        if(!config) {
            result(nil);
        }
        
        int capWidth = [ZegoFlutterUtils numberToIntValue:config[@"captureWidth"]];
        int capHeight = [ZegoFlutterUtils numberToIntValue:config[@"captureHeight"]];
        int encWidth = [ZegoFlutterUtils numberToIntValue:config[@"encodeWidth"]];
        int encHeight = [ZegoFlutterUtils numberToIntValue:config[@"encodeHeight"]];
        int bitrate = [ZegoFlutterUtils numberToIntValue:config[@"bitrate"]];
        int fps = [ZegoFlutterUtils numberToIntValue:config[@"fps"]];
        ZegoVideoConfig *configObject = [[ZegoVideoConfig alloc] initWithResolution:ZegoResolution360x640];
        configObject.captureResolution = CGSizeMake(capWidth, capHeight);
        configObject.encodeResolution = CGSizeMake(encWidth, encHeight);
        configObject.bitrate = bitrate;
        configObject.fps = fps;
        
        [[ZegoExpressEngine sharedEngine] setVideoConfig:configObject];
        result(nil);
        
    } else if([@"setVideoMirrorMode" isEqualToString:call.method]) {
        
        int mode = [ZegoFlutterUtils numberToIntValue:args[@"mode"]];
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:(ZegoVideoMirrorMode)mode];
        result(nil);
      
    } else if([@"setAppOrientation" isEqualToString:call.method]) {
        
        int orientation = [ZegoFlutterUtils numberToIntValue:args[@"orientation"]];
        UIInterfaceOrientation  uiOrientation = UIInterfaceOrientationUnknown;
        switch (orientation) {
            case 0:
                uiOrientation = UIInterfaceOrientationPortrait;
                break;
            case 1:
                uiOrientation = UIInterfaceOrientationLandscapeLeft;
                break;
            case 2:
                uiOrientation = UIInterfaceOrientationPortraitUpsideDown;
                break;
            case 3:
                uiOrientation = UIInterfaceOrientationLandscapeRight;
            default:
                break;
        }
        
        [[ZegoExpressEngine sharedEngine] setAppOrientation:uiOrientation];
        result(nil);
      
    } else if([@"setAudioConfig" isEqualToString:call.method]) {
        
        NSDictionary *config = args[@"config"];
        if(!config) {
            result(nil);
        }
        
        int bitrate = [ZegoFlutterUtils numberToIntValue:config[@"bitrate"]];
        int channels = [ZegoFlutterUtils numberToIntValue:config[@"channels"]];
        int codecID = [ZegoFlutterUtils numberToIntValue:config[@"codecID"]];
        ZegoAudioConfig *configObject = [[ZegoAudioConfig alloc] initWithPreset:ZegoAudioConfigPresetNormalLatencyStandardQuality];
        configObject.bitrate = bitrate;
        configObject.channels = channels;
        configObject.codecID = codecID;
        
        [[ZegoExpressEngine sharedEngine] setAudioConfig:configObject];
        result(nil);
      
    } else if([@"mutePublishStreamAudio" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoFlutterUtils numberToBoolValue:args[@"mute"]];
        [[ZegoExpressEngine sharedEngine] mutePublishStreamAudio:mute];
        result(nil);
      
    } else if([@"mutePublishStreamVideo" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoFlutterUtils numberToBoolValue:args[@"mute"]];
        [[ZegoExpressEngine sharedEngine] mutePublishStreamVideo:mute];
        result(nil);
      
    } else if([@"setCaptureVolume" isEqualToString:call.method]) {
      
        int volume = [ZegoFlutterUtils numberToIntValue:args[@"volume"]];
        [[ZegoExpressEngine sharedEngine] setCaptureVolume:volume];
        result(nil);
        
    } else if([@"addPublishCDNURL" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        NSString *targetURL = args[@"targetURL"];
        [[ZegoExpressEngine sharedEngine] addPublishCDNURL:targetURL stream:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
      
    } else if([@"removePublishCDNUrl" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        NSString *targetURL = args[@"targetURL"];
        [[ZegoExpressEngine sharedEngine] removePublishCDNURL:targetURL stream:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
      
    } else if([@"setPublishWatermark" isEqualToString:call.method]) {
        
        NSDictionary *watermark = args[@"watermark"];
        BOOL isPreviewVisible = [ZegoFlutterUtils numberToBoolValue:args[@"isPreviewVisible"]];
        ZegoWatermark *wkObject = nil;
        if(watermark) {
            int left = [ZegoFlutterUtils numberToIntValue:watermark[@"left"]];
            int top = [ZegoFlutterUtils numberToIntValue:watermark[@"top"]];
            int bottom = [ZegoFlutterUtils numberToIntValue:watermark[@"bottom"]];
            int right = [ZegoFlutterUtils numberToIntValue:watermark[@"right"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            wkObject = [[ZegoWatermark alloc] initWithImageURL:watermark[@"imageURL"] layout:rect];
        }
        
        [[ZegoExpressEngine sharedEngine] setPublishWatermark:wkObject isPreviewVisible:isPreviewVisible];
        result(nil);
      
    } else if([@"sendSEI" isEqualToString:call.method]) {
        
        FlutterStandardTypedData *data = args[@"byteData"];
        [[ZegoExpressEngine sharedEngine] sendSEI:data.data];
        result(nil);
      
    } else if([@"enableHardwareEncoder" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableHardwareEncoder:enable];
        result(nil);
      
    } else if([@"setCapturePipelineScaleMode" isEqualToString:call.method]) {
        
        int mode = [ZegoFlutterUtils numberToIntValue:args[@"mode"]];
        [[ZegoExpressEngine sharedEngine] setCapturePipelineScaleMode:(ZegoCapturePipelineScaleMode)mode];
        result(nil);
      
    } else if([@"startPlayingStream" isEqualToString:call.method]) {
      
    } else if([@"stopPlayingStream" isEqualToString:call.method]) {
      
      
    } else if([@"setPlayVolume" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        int volume = [ZegoFlutterUtils numberToIntValue:args[@"volume"]];
        [[ZegoExpressEngine sharedEngine] setPlayVolume:volume stream:streamID];
        result(nil);
      
    } else if([@"mutePlayStreamAudio" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] mutePlayStreamAudio:enable stream:streamID];
        result(nil);
      
    } else if([@"mutePlayStreamVideo" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] mutePlayStreamVideo:enable stream:streamID];
        result(nil);
      
    } else if([@"enableHardwareDecoder" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableHardwareDecoder:enable];
        result(nil);
      
    } else if([@"enableCheckPoc" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableCheckPoc:enable];
        result(nil);
      
    } else if([@"enableAEC" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableAEC:enable];
        result(nil);
      
    } else if([@"setAECMode" isEqualToString:call.method]) {
        
        int mode = [ZegoFlutterUtils numberToIntValue:args[@"mode"]];
        [[ZegoExpressEngine sharedEngine] setAECMode:(ZegoAECMode)mode];
        result(nil);
      
    } else if([@"enableAGC" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableAGC:enable];
        result(nil);
      
    } else if([@"enableANS" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableANS:enable];
        result(nil);
      
    } else if([@"enableBeautify" isEqualToString:call.method]) {
        
        int feature = [ZegoFlutterUtils numberToIntValue:args[@"feature"]];
        [[ZegoExpressEngine sharedEngine] enableBeautify:feature];
        result(nil);
      
    } else if([@"setBeautifyOption" isEqualToString:call.method]) {
      
        NSDictionary *option = args[@"option"];
        ZegoBeautifyOption *optionObject = [[ZegoBeautifyOption alloc] init];
        optionObject.polishStep = [ZegoFlutterUtils numberToDoubleValue:option[@"polishStep"]];
        optionObject.whitenFactor = [ZegoFlutterUtils numberToDoubleValue:option[@"whitenFactor"]];
        optionObject.sharpenFactor = [ZegoFlutterUtils numberToDoubleValue:option[@"sharpenFactor"]];
        
        [[ZegoExpressEngine sharedEngine] setBeautifyOption: optionObject];
        result(nil);
        
    } else if([@"muteMicrophone" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoFlutterUtils numberToBoolValue:args[@"mute"]];
        [[ZegoExpressEngine sharedEngine] muteMicrophone:mute];
        result(nil);
      
    } else if([@"muteAudioOutput" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoFlutterUtils numberToBoolValue:args[@"mute"]];
        [[ZegoExpressEngine sharedEngine] muteAudioOutput:mute];
        result(nil);
      
    } else if([@"enableAudioCaptureDevice" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableAudioCaptureDevice:enable];
        result(nil);
      
    } else if([@"enableCamera" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] enableCamera:enable];
        result(nil);
      
    } else if([@"useFrontCamera" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoFlutterUtils numberToBoolValue:args[@"enable"]];
        [[ZegoExpressEngine sharedEngine] useFrontCamera:enable];
        result(nil);
      
    } else if([@"startSoundLevelMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] startSoundLevelMonitor];
        result(nil);
      
    } else if([@"stopSoundLevelMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] stopSoundLevelMonitor];
        result(nil);
      
    } else if([@"startAudioSpectrumMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] startAudioSpectrumMonitor];
        result(nil);
      
    } else if([@"stopAudioSpectrumMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] stopSoundLevelMonitor];
      
    } else if([@"sendBroadcastMessage" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        NSString *message = args[@"message"];
        [[ZegoExpressEngine sharedEngine] sendBroadcastMessage:message roomID:roomID callback:^(int errorCode) {
            result(@(errorCode));
        }];
      
    } else if([@"sendCustomCommand" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        NSString *command = args[@"command"];
        NSArray<NSDictionary *> *userList = args[@"toUserList"];
        
        NSMutableArray<ZegoUser *> *userObjectList = nil;
        if(userList) {
            userObjectList = [NSMutableArray array];
            for(NSDictionary* user in userList) {
                ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:user[@"userID"] userName:user[@"userName"]];
                [userObjectList addObject:userObject];
            }
        }
        
        [[ZegoExpressEngine sharedEngine] sendCustomCommand:command toUserList:userObjectList roomID:roomID callback:^(int errorCode) {
            result(@(errorCode));
        }];
        
    } else {
      result(FlutterMethodNotImplemented);
    }
}

@end
