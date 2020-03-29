#import "ZegoExpressEnginePlugin.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>
#import "ZegoUtils.h"
#import "ZegoLog.h"

#import "ZegoPlatformViewRenderController.h"
#import "ZegoTextureRenderController.h"

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
    [ZegoExpressEngine destroyEngine:nil];
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    
    ZegoExpressEnginePlugin *instance = [[ZegoExpressEnginePlugin alloc] initWithRegistrar:registrar];
    
    FlutterMethodChannel *methodChannel = [FlutterMethodChannel
      methodChannelWithName:@"plugins.zego.im/zego_express_engine"
            binaryMessenger:[registrar messenger]];
    [registrar addMethodCallDelegate:instance channel:methodChannel];
    
    FlutterEventChannel *eventChannel = [FlutterEventChannel eventChannelWithName:@"plugins.zego.im/zego_express_event_handler" binaryMessenger:[registrar messenger]];
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

#pragma mark - Engine Method Call

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
    NSDictionary *args = call.arguments;
    if ([@"getVersion" isEqualToString:call.method]) {
        
        result([ZegoExpressEngine getVersion]);
        
    } else if ([@"enablePlatformView" isEqualToString:call.method]) {
      
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        _isEnablePlatformView = enable;
        result(nil);
        
#pragma mark Main
        
    } else if ([@"createEngine" isEqualToString:call.method]) {
        
        unsigned int appID = [ZegoUtils unsignedIntValue:args[@"appID"]];
        NSString *appSign = args[@"appID"];
        BOOL isTestEnv = [ZegoUtils boolValue:args[@"isTestEnv"]];
        int scenario = [ZegoUtils intValue:args[@"scenario"]];
        
        ZegoEngineConfig *config = [[ZegoEngineConfig alloc] init];
        if(!self.isEnablePlatformView)
        {
            ZegoCustomVideoRenderConfig *renderConfig = [[ZegoCustomVideoRenderConfig alloc] init];
            renderConfig.frameFormatSeries = ZegoVideoFrameFormatSeriesRGB;
            renderConfig.bufferType = ZegoVideoBufferTypeCVPixelBuffer;
            [config setCustomVideoRenderConfig:renderConfig];
        }
        
        [ZegoExpressEngine setEngineConfig:config];
        
        if ([ZegoExpressEngine createEngineWithAppID:appID appSign:appSign isTestEnv:isTestEnv scenario:(ZegoScenario)scenario eventHandler:self])
        {
            NSLog(@"create engine success");
            
            if(self.isEnablePlatformView)
            {
                [self.registrar registerViewFactory:[ZegoPlatformViewRenderController sharedInstance] withId:@"plugins.zego.im/zego_express_view"];
            }
            else
            {
                [[ZegoTextureRenderController sharedInstance] initController];
            }
            
            result(@(0));
        } else {
            [self throwCreateEngineError:result ofMethodName:call.method];
        }
      
    } else if ([@"destroyEngine" isEqualToString:call.method]) {
        
        [ZegoExpressEngine destroyEngine:nil];
        if(!self.isEnablePlatformView)
        {
            [[ZegoTextureRenderController sharedInstance] uninitController];
        }
        
        result(nil);
      
    } else if ([@"uploadLog" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] uploadLog];
        
        result(nil);
      
    } else if ([@"setDebugVerbose" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        int language = [ZegoUtils intValue:args[@"language"]];
        
        [[ZegoExpressEngine sharedEngine] setDebugVerbose:enable language:(ZegoLanguage)language];
        
        result(nil);
      
#pragma mark Room
        
    } else if ([@"loginRoom" isEqualToString:call.method]) {
      
        NSString *roomID = args[@"roomID"];
        NSDictionary *userMap = args[@"user"];
        NSDictionary *configMap = args[@"config"];
        
        ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:userMap[@"userID"] userName:userMap[@"userName"]];
        
        if (configMap) {
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
      
    } else if ([@"logoutRoom" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        
        [[ZegoExpressEngine sharedEngine] logoutRoom:roomID];
        
        result(nil);
        
#pragma mark Publisher
      
    } else if ([@"startPublishingStream" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] startPublishingStream:streamID channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"stopPublishingStream" isEqualToString:call.method]) {
      
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] stopPublishingStream:(ZegoPublishChannel)channel];
        
        result(nil);
        
    } else if ([@"setStreamExtraInfo" isEqualToString:call.method]) {
      
        NSString *extraInfo = args[@"extraInfo"];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        // TODO: 参数顺序
        [[ZegoExpressEngine sharedEngine] setStreamExtraInfo:extraInfo callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        } channel:(ZegoPublishChannel)channel];
        
    } else if([@"createTextureRenderer" isEqualToString:call.method]) {
        
        int viewWidth = [ZegoUtils intValue:args[@"width"]];
        int viewHeight = [ZegoUtils intValue:args[@"height"]];
        
        int64_t textureID = [[ZegoTextureRenderController sharedInstance] createRenderer:[self.registrar textures] viewWidth:viewWidth viewHeight:viewHeight];
        
        result(@(textureID));
        
    } else if([@"destroyTextureRenderer" isEqualToString:call.method]) {
        
        int64_t textureID = [ZegoUtils longLongValue:args[@"textureID"]];
        [[ZegoTextureRenderController sharedInstance] releaseRenderer: textureID];
        
        result(nil);
        
    } else if([@"destroyPlatfornView" isEqualToString:call.method]) {
        
        int viewID = [ZegoUtils intValue:args[@"viewID"]];
        [[ZegoPlatformViewRenderController sharedInstance] removeView:@(viewID)];
        
        result(nil);
        
    } else if ([@"startPreview" isEqualToString:call.method]) {
        // TODO: 预览
        NSDictionary *canvas = args[@"canvas"];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        // 使用platform view时为viewID，使用Texture时为textureID,对外传入都叫viewID
        int64_t viewID = [ZegoUtils longLongValue:canvas[@"viewID"]];
        int viewMode = [ZegoUtils intValue:canvas[@"viewMode"]];
        
        
        if(self.isEnablePlatformView) {
            // 使用Platform View 预览
            ZegoPlatformViewRenderer *renderer = [[ZegoPlatformViewRenderController sharedInstance] getRenderer:@(viewID)];
            if(renderer) {
                ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:[renderer getUIView]];
                canvas.viewMode = (ZegoViewMode)viewMode;
                [[ZegoExpressEngine sharedEngine] startPreview:canvas channel:(ZegoPublishChannel)channel];
            } else {
                //TODO: 直接抛出Flutter异常
            }

        } else {
            // 使用Texture渲染
            if([[ZegoTextureRenderController sharedInstance] addCaptureRenderer:viewID ofKey:@(channel)]) {
                
                [[ZegoTextureRenderController sharedInstance] startRendering];
            } else {
                //兼容纯音频预览
            }
            
            [[ZegoExpressEngine sharedEngine] startPreview:nil channel:(ZegoPublishChannel)channel];
        }
        
        result(nil);
        
    } else if ([@"stopPreview" isEqualToString:call.method]) {
        // TODO: 预览
        int channel = [ZegoUtils intValue:args[@"channel"]];
        [[ZegoExpressEngine sharedEngine] stopPreview:(ZegoPublishChannel) channel];
        
        if(!self.isEnablePlatformView) {
            
            [[ZegoTextureRenderController sharedInstance] removeCaptureRenderer:@(channel)];
            [[ZegoTextureRenderController sharedInstance] stopRendering];
        }
        
        result(nil);
        
    } else if ([@"setVideoConfig" isEqualToString:call.method]) {
      
        NSDictionary *configMap = args[@"config"];
        int capWidth = [ZegoUtils intValue:configMap[@"captureWidth"]];
        int capHeight = [ZegoUtils intValue:configMap[@"captureHeight"]];
        int encWidth = [ZegoUtils intValue:configMap[@"encodeWidth"]];
        int encHeight = [ZegoUtils intValue:configMap[@"encodeHeight"]];
        int bitrate = [ZegoUtils intValue:configMap[@"bitrate"]];
        int fps = [ZegoUtils intValue:configMap[@"fps"]];
        
        ZegoVideoConfig *configObject = [[ZegoVideoConfig alloc] init];
        configObject.captureResolution = CGSizeMake(capWidth, capHeight);
        configObject.encodeResolution = CGSizeMake(encWidth, encHeight);
        configObject.bitrate = bitrate;
        configObject.fps = fps;
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] setVideoConfig:configObject channel:(ZegoPublishChannel)channel];
        
        result(nil);
        
    } else if ([@"setVideoMirrorMode" isEqualToString:call.method]) {
        
        int mode = [ZegoUtils intValue:args[@"mode"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] setVideoMirrorMode:(ZegoVideoMirrorMode)mode channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"setAppOrientation" isEqualToString:call.method]) {
        
        int orientation = [ZegoUtils intValue:args[@"orientation"]];
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
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] setAppOrientation:uiOrientation channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"setAudioConfig" isEqualToString:call.method]) {
        
        NSDictionary *configMap = args[@"config"];
        int bitrate = [ZegoUtils intValue:configMap[@"bitrate"]];
        int channel = [ZegoUtils intValue:configMap[@"channel"]];
        int codecID = [ZegoUtils intValue:configMap[@"codecID"]];
        
        ZegoAudioConfig *configObject = [[ZegoAudioConfig alloc] init];
        configObject.bitrate = bitrate;
        configObject.channel = channel;
        configObject.codecID = codecID;
        
        [[ZegoExpressEngine sharedEngine] setAudioConfig:configObject];
        
        result(nil);
      
    } else if ([@"mutePublishStreamAudio" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] mutePublishStreamAudio:mute channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"mutePublishStreamVideo" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] mutePublishStreamVideo:mute channel:(ZegoPublishChannel)channel];
        
        result(nil);
        
    } else if ([@"enableTrafficControl" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        int property = [ZegoUtils intValue:args[@"property"]];
        
        [[ZegoExpressEngine sharedEngine] enableTrafficControl:enable property:(ZegoTrafficControlProperty)property];
        
        result(nil);
        
    } else if ([@"setMinVideoBitrateForTrafficControl" isEqualToString:call.method]) {
        
        int bitrate = [ZegoUtils intValue:args[@"bitrate"]];
        int mode = [ZegoUtils intValue:args[@"mode"]];
        
        [[ZegoExpressEngine sharedEngine] setMinVideoBitrateForTrafficControl:bitrate mode:(ZegoTrafficControlMinVideoBitrateMode)mode];
        
        result(nil);
        
    } else if ([@"setCaptureVolume" isEqualToString:call.method]) {
      
        int volume = [ZegoUtils intValue:args[@"volume"]];
        
        [[ZegoExpressEngine sharedEngine] setCaptureVolume:volume];
        
        result(nil);
        
    } else if ([@"addPublishCDNURL" isEqualToString:call.method]) {
        
        NSString *targetURL = args[@"targetURL"];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] addPublishCDNURL:targetURL streamID:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
      
    } else if ([@"removePublishCDNURL" isEqualToString:call.method]) {
        
        NSString *targetURL = args[@"targetURL"];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] removePublishCDNURL:targetURL streamID:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
        
    } else if ([@"enablePublishDirectToCDN" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        ZegoCDNConfig *cdnConfig = nil;
        NSDictionary *config = args[@"config"];
        if (config) {
            NSString *URL = config[@"URL"];
            NSString *authParam = config[@"authParam"];
            
            cdnConfig = [[ZegoCDNConfig alloc] init];
            cdnConfig.URL = URL;
            cdnConfig.authParam = authParam;
        }
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] enablePublishDirectToCDN:enable config:cdnConfig channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"setPublishWatermark" isEqualToString:call.method]) {
        
        NSDictionary *watermarkMap = args[@"watermark"];
        ZegoWatermark *watermarkObject = nil;
        if (watermarkMap) {
            NSString *imageURL = watermarkMap[@"imageURL"];
            int left = [ZegoUtils intValue:watermarkMap[@"left"]];
            int top = [ZegoUtils intValue:watermarkMap[@"top"]];
            int bottom = [ZegoUtils intValue:watermarkMap[@"bottom"]];
            int right = [ZegoUtils intValue:watermarkMap[@"right"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            watermarkObject = [[ZegoWatermark alloc] initWithImageURL:imageURL layout:rect];
        }
        
        BOOL isPreviewVisible = [ZegoUtils boolValue:args[@"isPreviewVisible"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] setPublishWatermark:watermarkObject isPreviewVisible:isPreviewVisible channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"sendSEI" isEqualToString:call.method]) {
        
        FlutterStandardTypedData *data = args[@"byteData"];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] sendSEI:data.data channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"enableHardwareEncoder" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableHardwareEncoder:enable];
        
        result(nil);
      
    } else if ([@"setCapturePipelineScaleMode" isEqualToString:call.method]) {
        
        int mode = [ZegoUtils intValue:args[@"mode"]];
        
        [[ZegoExpressEngine sharedEngine] setCapturePipelineScaleMode:(ZegoCapturePipelineScaleMode)mode];
        
        result(nil);
        
#pragma mark Player
      
    } else if ([@"startPlayingStream" isEqualToString:call.method]) {
        // TODO: 拉流
        NSString *streamID = args[@"streamID"];
        NSDictionary *canvas = args[@"canvas"];
        int64_t viewID = [ZegoUtils longLongValue:canvas[@"viewID"]];
        int mode = [ZegoUtils intValue:canvas[@"viewMode"]];
        
        NSDictionary *playerConfig = args[@"config"];
        
        if(self.isEnablePlatformView) {
            // 使用PlatformView渲染
            ZegoPlatformViewRenderer *renderer = [[ZegoPlatformViewRenderController sharedInstance] getRenderer:@(viewID)];
            
            ZegoCanvas *canvas = [[ZegoCanvas alloc] initWithView:[renderer getUIView]];
            canvas.viewMode = mode;
            
            if(playerConfig) {
                ZegoPlayerConfig *objPlayerConfig = [[ZegoPlayerConfig alloc] init];
                objPlayerConfig.videoLayer = (ZegoPlayerVideoLayer)[ZegoUtils intValue:playerConfig[@"videoLayer"]];
                
                NSDictionary * cdnConfig = playerConfig[@"cdnConfig"];
                if(cdnConfig) {
                    ZegoCDNConfig *objCdnConfig = [[ZegoCDNConfig alloc] init];
                    objCdnConfig.URL = cdnConfig[@"URL"];
                    objCdnConfig.authParam = cdnConfig[@"authParam"];
                    objPlayerConfig.CDNConfig = objCdnConfig;
                }
                
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:canvas config:objPlayerConfig];
            } else {
                
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas:canvas];
            }
            
        } else {
            // 使用Texture渲染
            ZegoPlayerConfig *objPlayerConfig = nil;
            if(playerConfig) {
                objPlayerConfig = [[ZegoPlayerConfig alloc] init];
                objPlayerConfig.videoLayer = (ZegoPlayerVideoLayer)[ZegoUtils intValue:playerConfig[@"videoLayer"]];
                
                NSDictionary * cdnConfig = playerConfig[@"cdnConfig"];
                if(cdnConfig) {
                    ZegoCDNConfig *objCdnConfig = [[ZegoCDNConfig alloc] init];
                    objCdnConfig.URL = cdnConfig[@"URL"];
                    objCdnConfig.authParam = cdnConfig[@"authParam"];
                    objPlayerConfig.CDNConfig = objCdnConfig;
                }
            }
            
            if([[ZegoTextureRenderController sharedInstance] addRemoteRenderer:viewID ofKey:streamID]) {
                [[ZegoTextureRenderController sharedInstance] startRendering];
                
            } else {
                // 兼容纯音频拉流
                
            }
            
            if(objPlayerConfig) {
                
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas: nil config:objPlayerConfig];
            } else {
                
                [[ZegoExpressEngine sharedEngine] startPlayingStream:streamID canvas: nil];
            }
        }
        
        result(nil);
      
    } else if ([@"stopPlayingStream" isEqualToString:call.method]) {
        // TODO: 拉流
        NSString *streamID = args[@"streamID"];
        [[ZegoExpressEngine sharedEngine] stopPlayingStream:streamID];
        
        if(!self.isEnablePlatformView) {
            [[ZegoTextureRenderController sharedInstance] removeRemoteRenderer:streamID];
            [[ZegoTextureRenderController sharedInstance] stopRendering];
        }
        
        
        
      
    } else if ([@"setPlayVolume" isEqualToString:call.method]) {
        
        int volume = [ZegoUtils intValue:args[@"volume"]];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] setPlayVolume:volume streamID:streamID];
        
        result(nil);
      
    } else if ([@"mutePlayStreamAudio" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] mutePlayStreamAudio:mute streamID:streamID];
        
        result(nil);
      
    } else if ([@"mutePlayStreamVideo" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] mutePlayStreamVideo:mute streamID:streamID];
        
        result(nil);
      
    } else if ([@"enableHardwareDecoder" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableHardwareDecoder:enable];
        
        result(nil);
      
    } else if ([@"enableCheckPoc" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableCheckPoc:enable];
        
        result(nil);
        
#pragma mark Mixer
        
    } else if ([@"startMixerTask" isEqualToString:call.method]) {
        
        NSString *taskID = args[@"taskID"];
        ZegoMixerTask *taskObject = [[ZegoMixerTask alloc] initWithTaskID:taskID];
        
        // MixerInput
        NSMutableArray<ZegoMixerInput *> *inputListObject = nil;
        NSArray<NSDictionary *> *inputListMap = args[@"inputList"];
        if (inputListMap) {
            inputListObject = [[NSMutableArray alloc] init];
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
        }
        
        if (inputListObject) {
            [taskObject setInputList:inputListObject];
        }
        
        // MixerOutput
        NSMutableArray<ZegoMixerOutput *> *outputListObject = nil;
        NSArray<NSDictionary *> *outputListMap = args[@"outputList"];
        if (outputListMap) {
            outputListObject = [[NSMutableArray alloc] init];
            for (NSDictionary *outputMap in outputListMap) {
                NSString *target = outputMap[@"target"];
                
                // MixerOutput AudioConfig
                NSDictionary *audioConfigMap = outputMap[@"audioConfig"];
                ZegoMixerAudioConfig *audioConfigObject = nil;
                if (audioConfigMap) {
                    int bitrate = [ZegoUtils intValue:audioConfigMap[@"bitrate"]];
                    int channel = [ZegoUtils intValue:audioConfigMap[@"channel"]];
                    int codecID = [ZegoUtils intValue:audioConfigMap[@"codecID"]];
                    audioConfigObject = [[ZegoMixerAudioConfig alloc] init];
                    audioConfigObject.bitrate = bitrate;
                    audioConfigObject.channel = (ZegoAudioChannel)channel;
                    audioConfigObject.codecID = (ZegoAudioCodecID)codecID;
                }
                
                // MixerOutput VideoConfig
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                ZegoMixerVideoConfig *videoConfigObject = nil;
                if (videoConfigMap) {
                    int width = [ZegoUtils intValue:videoConfigMap[@"width"]];
                    int height = [ZegoUtils intValue:videoConfigMap[@"height"]];
                    int fps = [ZegoUtils intValue:videoConfigMap[@"fps"]];
                    int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                    videoConfigObject = [[ZegoMixerVideoConfig alloc] init];
                    videoConfigObject.resolution = CGSizeMake((CGFloat)width, (CGFloat)height);
                    videoConfigObject.bitrate = bitrate;
                    videoConfigObject.fps = fps;
                }
                
                ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
                outputObject.audioConfig = audioConfigObject;
                outputObject.videoConfig = videoConfigObject;
                [outputListObject addObject:outputObject];
            }
        }
        
        if (outputListObject) {
            [taskObject setOutputList:outputListObject];
        }
                                                
        // Watermark
        ZegoWatermark *watermarkObject = nil;
        NSDictionary *watermarkMap = args[@"watermark"];
        if (watermarkMap) {
            NSString *imageURL = watermarkMap[@"imageURL"];
            int left = [ZegoUtils intValue:watermarkMap[@"left"]];
            int top = [ZegoUtils intValue:watermarkMap[@"top"]];
            int bottom = [ZegoUtils intValue:watermarkMap[@"bottom"]];
            int right = [ZegoUtils intValue:watermarkMap[@"right"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            watermarkObject = [[ZegoWatermark alloc] initWithImageURL:imageURL layout:rect];
        }
        
        if (watermarkObject) {
            [taskObject setWatermark:watermarkObject];
        }
        
        // Background Image
        NSString *backgroundImageURL = args[@"backgroundImageURL"];
        
        if (backgroundImageURL.length > 0) {
            [taskObject setBackgroundImageURL:backgroundImageURL];
        }
        
        // Enable SoundLevel
        BOOL enableSoundLevel = [ZegoUtils boolValue:args[@"enableSoundLevel"]];
        
        [taskObject enableSoundLevel:enableSoundLevel];
        
        [[ZegoExpressEngine sharedEngine] startMixerTask:taskObject callback:^(int errorCode, NSDictionary * _Nullable extendedData) {
            result(@{
                @"errorCode": @(errorCode),
                @"extendedData": extendedData ? : @{}
            });
        }];
        
    } else if ([@"stopMixerTask" isEqualToString:call.method]) {
        
        NSString *taskID = args[@"taskID"];
        ZegoMixerTask *taskObject = [[ZegoMixerTask alloc] initWithTaskID:taskID];
        
        // MixerInput
        NSMutableArray<ZegoMixerInput *> *inputListObject = nil;
        NSArray<NSDictionary *> *inputListMap = args[@"inputList"];
        if (inputListMap) {
            inputListObject = [[NSMutableArray alloc] init];
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
        }
        
        if (inputListObject) {
            [taskObject setInputList:inputListObject];
        }
        
        // MixerOutput
        NSMutableArray<ZegoMixerOutput *> *outputListObject = nil;
        NSArray<NSDictionary *> *outputListMap = args[@"outputList"];
        if (outputListMap) {
            outputListObject = [[NSMutableArray alloc] init];
            for (NSDictionary *outputMap in outputListMap) {
                NSString *target = outputMap[@"target"];
                
                // MixerOutput AudioConfig
                NSDictionary *audioConfigMap = outputMap[@"audioConfig"];
                ZegoMixerAudioConfig *audioConfigObject = nil;
                if (audioConfigMap) {
                    int bitrate = [ZegoUtils intValue:audioConfigMap[@"bitrate"]];
                    int channel = [ZegoUtils intValue:audioConfigMap[@"channel"]];
                    int codecID = [ZegoUtils intValue:audioConfigMap[@"codecID"]];
                    audioConfigObject = [[ZegoMixerAudioConfig alloc] init];
                    audioConfigObject.bitrate = bitrate;
                    audioConfigObject.channel = (ZegoAudioChannel)channel;
                    audioConfigObject.codecID = (ZegoAudioCodecID)codecID;
                }
                
                // MixerOutput VideoConfig
                NSDictionary *videoConfigMap = outputMap[@"videoConfig"];
                ZegoMixerVideoConfig *videoConfigObject = nil;
                if (videoConfigMap) {
                    int width = [ZegoUtils intValue:videoConfigMap[@"width"]];
                    int height = [ZegoUtils intValue:videoConfigMap[@"height"]];
                    int fps = [ZegoUtils intValue:videoConfigMap[@"fps"]];
                    int bitrate = [ZegoUtils intValue:videoConfigMap[@"bitrate"]];
                    videoConfigObject = [[ZegoMixerVideoConfig alloc] init];
                    videoConfigObject.resolution = CGSizeMake((CGFloat)width, (CGFloat)height);
                    videoConfigObject.bitrate = bitrate;
                    videoConfigObject.fps = fps;
                }
                
                ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
                outputObject.audioConfig = audioConfigObject;
                outputObject.videoConfig = videoConfigObject;
                [outputListObject addObject:outputObject];
            }
        }
        
        if (outputListObject) {
            [taskObject setOutputList:outputListObject];
        }
        
        // no need to set watermark
        
        // no need to set background image
        
        // no need to set enable sound level
        
        [[ZegoExpressEngine sharedEngine] stopMixerTask:taskObject callback:^(int errorCode) {
            result(@(errorCode));
        }];
        
#pragma mark Device
        
    } else if ([@"muteMicrophone" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        
        [[ZegoExpressEngine sharedEngine] muteMicrophone:mute];
        
        result(nil);
      
    } else if ([@"muteAudioOutput" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        
        [[ZegoExpressEngine sharedEngine] muteAudioOutput:mute];
        
        result(nil);
      
    } else if ([@"enableAudioCaptureDevice" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableAudioCaptureDevice:enable];
        
        result(nil);
    
    } else if ([@"setBuiltInSpeakerOn" isEqualToString:call.method]) {
    
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] setBuiltInSpeakerOn:enable];
        
        result(nil);
      
    } else if ([@"enableCamera" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] enableCamera:enable channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"useFrontCamera" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] useFrontCamera:enable channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"startSoundLevelMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] startSoundLevelMonitor];
        
        result(nil);
      
    } else if ([@"stopSoundLevelMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] stopSoundLevelMonitor];
        
        result(nil);
      
    } else if ([@"startAudioSpectrumMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] startAudioSpectrumMonitor];
        
        result(nil);
      
    } else if ([@"stopAudioSpectrumMonitor" isEqualToString:call.method]) {
        
        [[ZegoExpressEngine sharedEngine] stopSoundLevelMonitor];
        
        result(nil);
        
#pragma mark Preprocess
      
    } else if ([@"enableAEC" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableAEC:enable];
        
        result(nil);
      
    } else if ([@"setAECMode" isEqualToString:call.method]) {
        
        int mode = [ZegoUtils intValue:args[@"mode"]];
        
        [[ZegoExpressEngine sharedEngine] setAECMode:(ZegoAECMode)mode];
        
        result(nil);
      
    } else if ([@"enableAGC" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableAGC:enable];
        
        result(nil);
      
    } else if ([@"enableANS" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        [[ZegoExpressEngine sharedEngine] enableANS:enable];
        
        result(nil);
      
    } else if ([@"enableBeautify" isEqualToString:call.method]) {
        
        int feature = [ZegoUtils intValue:args[@"feature"]];
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] enableBeautify:(ZegoBeautifyFeature)feature channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"setBeautifyOption" isEqualToString:call.method]) {
      
        NSDictionary *option = args[@"option"];
        ZegoBeautifyOption *optionObject = [[ZegoBeautifyOption alloc] init];
        optionObject.polishStep = [ZegoUtils doubleValue:option[@"polishStep"]];
        optionObject.whitenFactor = [ZegoUtils doubleValue:option[@"whitenFactor"]];
        optionObject.sharpenFactor = [ZegoUtils doubleValue:option[@"sharpenFactor"]];
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] setBeautifyOption: optionObject channel:(ZegoPublishChannel)channel];
        
        result(nil);
        
#pragma mark IM
        
    } else if ([@"sendBroadcastMessage" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        NSString *message = args[@"message"];
        
        [[ZegoExpressEngine sharedEngine] sendBroadcastMessage:message roomID:roomID callback:^(int errorCode, unsigned long long messageID) {
            result(@{
                @"errorCode": @(errorCode),
                @"messageID": @(messageID)
            });
        }];
        
    } else if ([@"sendBarrageMessage" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        NSString *message = args[@"message"];
        
        [[ZegoExpressEngine sharedEngine] sendBarrageMessage:message roomID:roomID callback:^(int errorCode, NSString * _Nonnull messageID) {
            result(@{
                @"errorCode": @(errorCode),
                @"messageID": messageID
            });
        }];
        
    } else if ([@"sendCustomCommand" isEqualToString:call.method]) {
        
        NSString *roomID = args[@"roomID"];
        NSString *command = args[@"command"];
        NSArray<NSDictionary *> *userListMap = args[@"toUserList"];
        
        NSMutableArray<ZegoUser *> *userListObject = nil;
        if (userListMap) {
            userListObject = [[NSMutableArray alloc] init];
            for(NSDictionary *userMap in userListMap) {
                ZegoUser *userObject = [[ZegoUser alloc] initWithUserID:userMap[@"userID"] userName:userMap[@"userName"]];
                [userListObject addObject:userObject];
            }
        }
        
        [[ZegoExpressEngine sharedEngine] sendCustomCommand:command toUserList:userListObject roomID:roomID callback:^(int errorCode) {
            result(@(errorCode));
        }];
        
    } else {
      result(FlutterMethodNotImplemented);
    }
}

#pragma mark - ZegoEventHandler

- (void)onDebugError:(int)errorCode funcName:(NSString *)funcName info:(NSString *)info {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, errorCode: %d, funcName: %@, info: %@", sink, errorCode, funcName, info);
    
    if (sink) {
        sink(@{
            @"method": @"onDebugError",
            @"errorCode": @(errorCode),
            @"funcName": funcName,
            @"info": info
        });
    }
}

#pragma mark Room Callback

- (void)onRoomStateUpdate:(ZegoRoomState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, errorCode: %d, roomID: %@", sink, errorCode, roomID);
    
    if (sink) {
        sink(@{
            @"method": @"onRoomStateUpdate",
            @"errorCode": @(errorCode),
            @"extendedData": extendedData,
            @"roomID": roomID
        });
    }
}

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType userList:(NSArray<ZegoUser *> *)userList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, updateType: %@, usersCount: %d, roomID: %@", sink, updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)userList.count, roomID);
    
    if (sink) {
        NSMutableArray *userListArray = [[NSMutableArray alloc] init];
        for (ZegoUser *user in userList) {
            [userListArray addObject:@{
                @"userID": user.userID,
                @"userName": user.userName
            }];
        }
        
        sink(@{
            @"method": @"onRoomUserUpdate",
            @"updateType": @(updateType),
            @"userList": userListArray,
            @"roomID": roomID
        });
    }
}

- (void)onRoomStreamUpdate:(ZegoUpdateType)updateType streamList:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, updateType: %@, streamsCount: %d, roomID: %@", sink, updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)streamList.count, roomID);
    
    if (sink) {
        NSMutableArray *streamListArray = [[NSMutableArray alloc] init];
        for (ZegoStream *stream in streamList) {
            [streamListArray addObject:@{
                @"user": @{
                    @"userID": stream.user.userID,
                    @"userName": stream.user.userName
                },
                @"streamID": stream.streamID,
                @"extraInfo": stream.extraInfo
            }];
        }
        
        sink(@{
            @"method": @"onRoomStreamUpdate",
            @"updateType": @(updateType),
            @"streamList": streamListArray,
            @"roomID": roomID
        });
    }
}

- (void)onRoomStreamExtraInfoUpdate:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, streamsCount: %d, roomID: %@", sink, (int)streamList.count, roomID);
    
    if (sink) {
        NSMutableArray *streamListArray = [[NSMutableArray alloc] init];
        for (ZegoStream *stream in streamList) {
            [streamListArray addObject:@{
                @"user": @{
                    @"userID": stream.user.userID,
                    @"userName": stream.user.userName
                },
                @"streamID": stream.streamID,
                @"extraInfo": stream.extraInfo
            }];
        }
        
        sink(@{
            @"method": @"onRoomStreamExtraInfoUpdate",
            @"streamList": streamListArray,
            @"roomID": roomID
        });
    }
}

#pragma mark Publisher Callback

- (void)onPublisherStateUpdate:(ZegoPublisherState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, state: %d, errorCode: %d, streamID: %@", sink, (int)state, errorCode, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPublisherStateUpdate",
            @"state": @(state),
            @"errorCode": @(errorCode),
            @"extendedData": extendedData,
            @"streamID": streamID
        });
    }
}

- (void)onPublisherQualityUpdate:(ZegoPublishStreamQuality *)quality streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onPublisherQualityUpdate",
            @"quality": @{
                @"videoCaptureFPS": @(quality.videoCaptureFPS),
                @"videoEncodeFPS": @(quality.videoEncodeFPS),
                @"videoSendFPS": @(quality.videoSendFPS),
                @"videoKBPS": @(quality.videoKBPS),
                @"audioCaptureFPS": @(quality.audioCaptureFPS),
                @"audioSendFPS": @(quality.audioSendFPS),
                @"audioKBPS": @(quality.audioKBPS),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"level": @(quality.level),
                @"isHardwareEncode": @(quality.isHardwareEncode)
            },
            @"streamID": streamID
        });
    }
}

- (void)onPublisherCapturedAudioFirstFrame {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p", sink);
    
    if (sink) {
        sink(@{
            @"method": @"onPublisherCapturedAudioFirstFrame",
        });
    }
}

- (void)onPublisherCapturedVideoFirstFrame:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, channel: %d", sink, (int)channel);
    
    if (sink) {
        sink(@{
            @"method": @"onPublisherCapturedVideoFirstFrame",
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherVideoSizeChanged:(CGSize)size channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, width: %d, height: %d, channel: %d", sink, (int)size.width, (int)size.height, (int)channel);
    
    if (sink) {
        sink(@{
            @"method": @"onPublisherVideoSizeChanged",
            @"width": @((int)size.width),
            @"height": @((int)size.height),
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherRelayCDNStateUpdate:(NSArray<ZegoStreamRelayCDNInfo *> *)streamInfoList streamID:(NSString *)streamID {
    //TODO: gaiming
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, infosCount: %d, streamID: %@", sink, (int)streamInfoList.count, streamID);
    
    if (sink) {
        NSMutableArray *streamInfoListArray = [[NSMutableArray alloc] init];
        for (ZegoStreamRelayCDNInfo *info in streamInfoList) {
            [streamInfoListArray addObject:@{
                @"URL": info.URL,
                @"state": @(info.state),
                @"updateReason": @(info.updateReason),
                @"stateTime": @(info.stateTime)
            }];
        }
        
        sink(@{
            @"method": @"onPublisherRelayCDNStateUpdate",
            @"streamInfoList": streamInfoListArray,
            @"streamID": streamID
        });
    }
}

#pragma mark Player Callback

- (void)onPlayerStateUpdate:(ZegoPlayerState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, state: %d, errorCode: %d, streamID: %@", sink, (int)state, errorCode, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerStateUpdate",
            @"state": @(state),
            @"errorCode": @(errorCode),
            @"extendedData": extendedData,
            @"streamID": streamID
        });
    }
}

- (void)onPlayerQualityUpdate:(ZegoPlayStreamQuality *)quality streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerQualityUpdate",
            @"quality": @{
                @"videoRecvFPS": @(quality.videoRecvFPS),
                @"videoDecodeFPS": @(quality.videoDecodeFPS),
                @"videoRenderFPS": @(quality.videoRenderFPS),
                @"videoKBPS": @(quality.videoKBPS),
                @"audioRecvFPS": @(quality.audioRecvFPS),
                @"audioDecodeFPS": @(quality.audioDecodeFPS),
                @"audioRenderFPS": @(quality.audioRenderFPS),
                @"audioKBPS": @(quality.audioKBPS),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"level": @(quality.level),
                @"delay": @(quality.delay),
                @"isHardwareDecode": @(quality.isHardwareDecode)
            },
            @"streamID": streamID
        });
    }
}

- (void)onPlayerMediaEvent:(ZegoPlayerMediaEvent)event streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, event: %d, streamID: %@", sink, (int)event, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerMediaEvent",
            @"event": @(event),
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvAudioFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, streamID: %@", sink, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvAudioFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvVideoFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, streamID: %@", sink, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvVideoFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRenderVideoFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, streamID: %@", sink, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerRenderVideoFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerVideoSizeChanged:(CGSize)size streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, width: %d, height: %d, streamID: %@", sink, (int)size.width, (int)size.height, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerVideoSizeChanged",
            @"width": @((int)size.width),
            @"height": @((int)size.height),
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvSEI:(NSData *)data streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, streamID: %@", sink, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvSEI",
            @"data": data,
            @"streamID": streamID
        });
    }
}

#pragma mark Mixer Callback

- (void)onMixerRelayCDNStateUpdate:(NSArray<ZegoStreamRelayCDNInfo *> *)infoList taskID:(NSString *)taskID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, infosCount: %d, taskID: %@", sink, (int)infoList.count, taskID);
    
    if (sink) {
        NSMutableArray *infoListArray = [[NSMutableArray alloc] init];
        for (ZegoStreamRelayCDNInfo *info in infoList) {
            [infoListArray addObject:@{
                @"URL": info.URL,
                @"state": @(info.state),
                @"updateReason": @(info.updateReason),
                @"stateTime": @(info.stateTime)
            }];
        }
        
        sink(@{
            @"method": @"onMixerRelayCDNStateUpdate",
            @"infoList": infoListArray,
            @"taskID": taskID
        });
    }
}

- (void)onMixerSoundLevelUpdate:(NSDictionary<NSNumber *,NSNumber *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onMixerSoundLevelUpdate",
            @"soundLevels": soundLevels
        });
    }
}

#pragma mark Device Callback

- (void)onCapturedSoundLevelUpdate:(NSNumber *)soundLevel {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onCapturedSoundLevelUpdate",
            @"soundLevel": soundLevel
        });
    }
}

- (void)onRemoteSoundLevelUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onRemoteSoundLevelUpdate",
            @"soundLevels": soundLevels
        });
    }
}

- (void)onCapturedAudioSpectrumUpdate:(NSArray<NSNumber *> *)audioSpectrum {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onCapturedAudioSpectrumUpdate",
            @"audioSpectrum": audioSpectrum
        });
    }
}

- (void)onRemoteAudioSpectrumUpdate:(NSDictionary<NSString *,NSArray<NSNumber *> *> *)audioSpectrums {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    if (sink) {
        sink(@{
            @"method": @"onRemoteAudioSpectrumUpdate",
            @"audioSpectrums": audioSpectrums
        });
    }
}

- (void)onDeviceError:(int)errorCode deviceName:(NSString *)deviceName {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, errorCode: %d, deviceName: %@", sink, errorCode, deviceName);
    
    if (sink) {
        sink(@{
            @"method": @"onDeviceError",
            @"errorCode": @(errorCode),
            @"deviceName": deviceName
        });
    }
}

- (void)onRemoteCameraStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, state: %d, streamID: %@", sink, (int)state, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onRemoteCameraStateUpdate",
            @"state": @(state),
            @"streamID": streamID
        });
    }
}

- (void)onRemoteMicStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, state: %d, streamID: %@", sink, (int)state, streamID);
    
    if (sink) {
        sink(@{
            @"method": @"onRemoteMicStateUpdate",
            @"state": @(state),
            @"streamID": streamID
        });
    }
}

#pragma mark IM Callback

- (void)onIMRecvBroadcastMessage:(NSArray<ZegoBroadcastMessageInfo *> *)messageList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, messageListCount: %d, roomID: %@", sink, (int)messageList.count, roomID);
    
    if (sink) {
        NSMutableArray *messageListArray = [[NSMutableArray alloc] init];
        for (ZegoBroadcastMessageInfo *info in messageList) {
            [messageListArray addObject:@{
                @"message": info.message,
                @"messageID": @(info.messageID),
                @"sendTime": @(info.sendTime),
                @"fromUser": @{
                    @"userID": info.fromUser.userID,
                    @"userName": info.fromUser.userName
                }
            }];
        }
        
        sink(@{
            @"method": @"onIMRecvBroadcastMessage",
            @"messageList": messageListArray,
            @"roomID": roomID
        });
    }
}

- (void)onIMRecvBarrageMessage:(NSArray<ZegoBarrageMessageInfo *> *)messageList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, messageListCount: %d, roomID: %@", sink, (int)messageList.count, roomID);
    
    if (sink) {
        NSMutableArray *messageListArray = [[NSMutableArray alloc] init];
        for (ZegoBarrageMessageInfo *info in messageList) {
            [messageListArray addObject:@{
                @"message": info.message,
                @"messageID": info.messageID,
                @"sendTime": @(info.sendTime),
                @"fromUser": @{
                    @"userID": info.fromUser.userID,
                    @"userName": info.fromUser.userName
                }
            }];
        }
        
        sink(@{
            @"method": @"onIMRecvBarrageMessage",
            @"messageList": messageListArray,
            @"roomID": roomID
        });
    }
}

- (void)onIMRecvCustomCommand:(NSString *)command fromUser:(ZegoUser *)fromUser roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"sink: %p, command: %@, fromUserID: %@, fromUserName: %@, roomID: %@", sink, command, fromUser.userID, fromUser.userName, roomID);
    
    if (sink) {
        sink(@{
            @"method": @"onIMRecvCustomCommand",
            @"fromUser": @{
                @"userID": fromUser.userID,
                @"userName": fromUser.userName
            },
            @"roomID": roomID
        });
    }
}

@end
