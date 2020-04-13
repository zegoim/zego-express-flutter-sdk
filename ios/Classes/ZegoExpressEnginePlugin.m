#import "ZegoExpressEnginePlugin.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

#import "ZegoExpressEngineEventHandler.h"

#import "ZegoPlatformViewFactory.h"
#import "ZegoTextureRendererController.h"

#import "ZegoUtils.h"
#import "ZegoLog.h"

@interface ZegoExpressEnginePlugin() <FlutterStreamHandler>

@property (nonatomic, strong) NSObject<FlutterPluginRegistrar> *registrar;
@property (nonatomic, strong) FlutterMethodChannel *methodChannel;
@property (nonatomic, strong) FlutterEventChannel *eventChannel;
@property (nonatomic, strong) FlutterEventSink eventSink;
@property (nonatomic, strong) ZegoExpressEngineEventHandler *eventHandler;
@property (nonatomic, assign) BOOL enablePlatformView;

@end

@implementation ZegoExpressEnginePlugin

- (instancetype)initWithRegistrar:(NSObject<FlutterPluginRegistrar> *)registrar {
    self = [super init];
    if (self) {
        _registrar = registrar;
        _enablePlatformView = NO;
    }
    return self;
}

- (void)dealloc {
    [ZegoExpressEngine destroyEngine:nil];
    [self.methodChannel setMethodCallHandler:nil];
    [self.eventChannel setStreamHandler:nil];
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    
    ZegoExpressEnginePlugin *instance = [[ZegoExpressEnginePlugin alloc] initWithRegistrar:registrar];
    
    FlutterMethodChannel *methodChannel = [FlutterMethodChannel
      methodChannelWithName:@"plugins.zego.im/zego_express_engine"
            binaryMessenger:[registrar messenger]];
    [registrar addMethodCallDelegate:instance channel:methodChannel];
    instance.methodChannel = methodChannel;
    
    FlutterEventChannel *eventChannel = [FlutterEventChannel eventChannelWithName:@"plugins.zego.im/zego_express_event_handler" binaryMessenger:[registrar messenger]];
    [eventChannel setStreamHandler:instance];
    instance.eventChannel = eventChannel;
    
    // Register platform view factory
    [registrar registerViewFactory:[ZegoPlatformViewFactory sharedInstance] withId:@"plugins.zego.im/zego_express_view"];
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
    self.eventSink = events;
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
    self.eventSink = nil;
    return nil;
}

#pragma mark - Engine Method Call

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
    NSDictionary *args = call.arguments;
    
    if ([@"getVersion" isEqualToString:call.method]) {
        
        result([ZegoExpressEngine getVersion]);
        
#pragma mark Main
        
    } else if ([@"createEngine" isEqualToString:call.method]) {
        
        unsigned int appID = [ZegoUtils unsignedIntValue:args[@"appID"]];
        NSString *appSign = args[@"appSign"];
        BOOL isTestEnv = [ZegoUtils boolValue:args[@"isTestEnv"]];
        int scenario = [ZegoUtils intValue:args[@"scenario"]];
        BOOL enablePlatformView = [ZegoUtils boolValue:args[@"enablePlatformView"]];
        
        _enablePlatformView = enablePlatformView;
        
        ZegoEngineConfig *config = [[ZegoEngineConfig alloc] init];
        if (!self.enablePlatformView) {
            ZegoCustomVideoRenderConfig *renderConfig = [[ZegoCustomVideoRenderConfig alloc] init];
            renderConfig.frameFormatSeries = ZegoVideoFrameFormatSeriesRGB;
            renderConfig.bufferType = ZegoVideoBufferTypeCVPixelBuffer;
            [config setCustomVideoRenderConfig:renderConfig];
        }
        
        [ZegoExpressEngine setEngineConfig:config];
        
        self.eventHandler = [[ZegoExpressEngineEventHandler alloc] initWithSink:self.eventSink];
        
        [ZegoExpressEngine createEngineWithAppID:appID appSign:appSign isTestEnv:isTestEnv scenario:(ZegoScenario)scenario eventHandler:self.eventHandler];
            
        if (!self.enablePlatformView) {
            [[ZegoTextureRendererController sharedInstance] initController];
        }
        
        result(nil);
      
    } else if ([@"destroyEngine" isEqualToString:call.method]) {
        
        [ZegoExpressEngine destroyEngine:nil];
        
        if (!self.enablePlatformView) {
            // uninit texture render
            [[ZegoTextureRendererController sharedInstance] uninitController];
        }
        
        self.eventHandler = nil;
        
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
        
        [[ZegoExpressEngine sharedEngine] setStreamExtraInfo:extraInfo channel:(ZegoPublishChannel)channel callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
        
    } else if([@"createTextureRenderer" isEqualToString:call.method]) {
        
        int viewWidth = [ZegoUtils intValue:args[@"width"]];
        int viewHeight = [ZegoUtils intValue:args[@"height"]];
        
        int64_t textureID = [[ZegoTextureRendererController sharedInstance] createTextureRenderer:[self.registrar textures] viewWidth:viewWidth viewHeight:viewHeight];
        
        result(@(textureID));
        
    } else if ([@"updateTextureRendererSize" isEqualToString:call.method]) {
        
        int64_t textureID = [ZegoUtils longLongValue:args[@"textureID"]];
        int viewWidth = [ZegoUtils intValue:args[@"width"]];
        int viewHeight = [ZegoUtils intValue:args[@"height"]];
        [[ZegoTextureRendererController sharedInstance] updateTextureRenderer:textureID viewWidth:viewWidth viewHeight:viewHeight];
        
        result(nil);
        
    } else if([@"destroyTextureRenderer" isEqualToString:call.method]) {
        
        int64_t textureID = [ZegoUtils longLongValue:args[@"textureID"]];
        [[ZegoTextureRendererController sharedInstance] destroyTextureRenderer:textureID];
        
        result(nil);
        
    } else if([@"destroyPlatformView" isEqualToString:call.method]) {
        
        int viewID = [ZegoUtils intValue:args[@"viewID"]];
        [[ZegoPlatformViewFactory sharedInstance] destroyPlatformView:@(viewID)];
        
        result(nil);
        
    } else if ([@"startPreview" isEqualToString:call.method]) {
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        // Handle ZegoCanvas
        
        NSDictionary *canvasMap = args[@"canvas"];
        
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
                    // Preview video without creating the PlatfromView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatfromView and get viewID (PlatformViewID)
                    // TODO: Throw Flutter Exception
                }

            } else {
                // Render with Texture
                if ([[ZegoTextureRendererController sharedInstance] addCapturedRenderer:viewID key:@(channel)]) {
                    [[ZegoTextureRendererController sharedInstance] startRendering];
                } else {
                    // Preview video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    // TODO: Throw Flutter Exception
                }
                
                // Using Custom Video Renderer
                [[ZegoExpressEngine sharedEngine] startPreview:nil channel:(ZegoPublishChannel)channel];
            }
            
        } else { /* if (canvas && canvas.count > 0) */
            
            // Preview audio only
            [[ZegoExpressEngine sharedEngine] startPreview:nil channel:(ZegoPublishChannel)channel];
        }
        
        result(nil);
        
    } else if ([@"stopPreview" isEqualToString:call.method]) {
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        [[ZegoExpressEngine sharedEngine] stopPreview:(ZegoPublishChannel)channel];
        
        if (!self.enablePlatformView) {
            // Stop Texture Renderer
            [[ZegoTextureRendererController sharedInstance] removeCapturedRenderer:@(channel)];
            [[ZegoTextureRendererController sharedInstance] stopRendering];
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
        
        int mode = [ZegoUtils intValue:args[@"mirrorMode"]];
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
        
    } else if ([@"addPublishCdnUrl" isEqualToString:call.method]) {
        
        NSString *targetURL = args[@"targetURL"];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] addPublishCdnUrl:targetURL streamID:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
      
    } else if ([@"removePublishCdnUrl" isEqualToString:call.method]) {
        
        NSString *targetURL = args[@"targetURL"];
        NSString *streamID = args[@"streamID"];
        
        [[ZegoExpressEngine sharedEngine] removePublishCdnUrl:targetURL streamID:streamID callback:^(int errorCode) {
            result(@{@"errorCode": @(errorCode)});
        }];
        
    } else if ([@"enablePublishDirectToCDN" isEqualToString:call.method]) {
        
        BOOL enable = [ZegoUtils boolValue:args[@"enable"]];
        
        ZegoCDNConfig *cdnConfig = nil;
        NSDictionary *config = args[@"config"];
        if (config) {
            NSString *url = config[@"url"];
            NSString *authParam = config[@"authParam"];
            
            cdnConfig = [[ZegoCDNConfig alloc] init];
            cdnConfig.url = url;
            cdnConfig.authParam = authParam;
        }
        
        int channel = [ZegoUtils intValue:args[@"channel"]];
        
        [[ZegoExpressEngine sharedEngine] enablePublishDirectToCDN:enable config:cdnConfig channel:(ZegoPublishChannel)channel];
        
        result(nil);
      
    } else if ([@"setPublishWatermark" isEqualToString:call.method]) {
        
        NSDictionary *watermarkMap = args[@"watermark"];
        ZegoWatermark *watermarkObject = nil;
        if (watermarkMap && watermarkMap.count > 0) {
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
        
        NSString *streamID = args[@"streamID"];
        
        // Handle ZegoPlayerConfig
        
        ZegoPlayerConfig *playerConfig = nil;
        
        NSDictionary *playerConfigMap = args[@"config"];
        
        if (playerConfigMap && playerConfigMap.count > 0) {
            
            playerConfig = [[ZegoPlayerConfig alloc] init];
            playerConfig.videoLayer = (ZegoPlayerVideoLayer)[ZegoUtils intValue:playerConfigMap[@"videoLayer"]];
            NSDictionary * cdnConfigMap = playerConfigMap[@"cdnConfig"];
            
            if (cdnConfigMap && cdnConfigMap.count > 0) {
                ZegoCDNConfig *cdnConfig = [[ZegoCDNConfig alloc] init];
                cdnConfig.url = cdnConfigMap[@"url"];
                cdnConfig.authParam = cdnConfigMap[@"authParam"];
                playerConfig.cdnConfig = cdnConfig;
            }
        }
        
        // Handle ZegoCanvas
        
        NSDictionary *canvasMap = args[@"canvas"];
        
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
                    // Play video without creating the PlatfromView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatfromView and get viewID (PlatformViewID)
                    // TODO: Throw Flutter Exception
                }
                
            } else {
                // Render with Texture
                if ([[ZegoTextureRendererController sharedInstance] addRemoteRenderer:viewID key:streamID]) {
                    [[ZegoTextureRendererController sharedInstance] startRendering];
                } else {
                    // Play video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    // TODO: Throw Flutter Exception
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
      
    } else if ([@"stopPlayingStream" isEqualToString:call.method]) {
        
        NSString *streamID = args[@"streamID"];
        [[ZegoExpressEngine sharedEngine] stopPlayingStream:streamID];
        
        if (!self.enablePlatformView) {
            // Stop Texture render
            [[ZegoTextureRendererController sharedInstance] removeRemoteRenderer:streamID];
            [[ZegoTextureRendererController sharedInstance] stopRendering];
        }
        
        result(nil);
        
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
        NSArray<NSDictionary *> *inputListMap = args[@"inputList"];
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
        NSArray<NSDictionary *> *outputListMap = args[@"outputList"];
        if (outputListMap && outputListMap.count > 0) {
            NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
            for (NSDictionary *outputMap in outputListMap) {
                NSString *target = outputMap[@"target"];
                ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
                [outputListObject addObject:outputObject];
            }
            [taskObject setOutputList:outputListObject];
        }
        
        // AudioConfig
        NSDictionary *audioConfigMap = args[@"audioConfig"];
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
        NSDictionary *videoConfigMap = args[@"videoConfig"];
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
        NSDictionary *watermarkMap = args[@"watermark"];
        if (watermarkMap && watermarkMap.count > 0) {
            NSString *imageURL = watermarkMap[@"imageURL"];
            int left = [ZegoUtils intValue:watermarkMap[@"left"]];
            int top = [ZegoUtils intValue:watermarkMap[@"top"]];
            int bottom = [ZegoUtils intValue:watermarkMap[@"bottom"]];
            int right = [ZegoUtils intValue:watermarkMap[@"right"]];
            CGRect rect = CGRectMake(left, top, right - left, bottom - top);
            ZegoWatermark *watermarkObject = [[ZegoWatermark alloc] initWithImageURL:imageURL layout:rect];
            
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
            
            NSString *extendedDataJsonString = @"{}";
            if (extendedData) {
                NSError *error;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
                if (!jsonData) {
                    ZGLog(@"extendedData error: %@", error);
                }else{
                    extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
                }
            }
            
            result(@{
                @"errorCode": @(errorCode),
                @"extendedData": extendedDataJsonString
            });
        }];
        
    } else if ([@"stopMixerTask" isEqualToString:call.method]) {
        
        NSString *taskID = args[@"taskID"];
        ZegoMixerTask *taskObject = [[ZegoMixerTask alloc] initWithTaskID:taskID];
        
        // MixerInput
        NSArray<NSDictionary *> *inputListMap = args[@"inputList"];
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
        NSArray<NSDictionary *> *outputListMap = args[@"outputList"];
        if (outputListMap && outputListMap.count > 0) {
            NSMutableArray<ZegoMixerOutput *> *outputListObject = [[NSMutableArray alloc] init];
            for (NSDictionary *outputMap in outputListMap) {
                NSString *target = outputMap[@"target"];
                ZegoMixerOutput *outputObject = [[ZegoMixerOutput alloc] initWithTarget:target];
                [outputListObject addObject:outputObject];
            }
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
      
    } else if ([@"muteSpeaker" isEqualToString:call.method]) {
        
        BOOL mute = [ZegoUtils boolValue:args[@"mute"]];
        
        [[ZegoExpressEngine sharedEngine] muteSpeaker:mute];
        
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
        if (userListMap && userListMap.count > 0) {
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


@end
