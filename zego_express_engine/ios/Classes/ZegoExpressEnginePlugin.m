#import "ZegoExpressEnginePlugin.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface ZegoExpressEnginePlugin()<FlutterStreamHandler, ZegoEventHandler>

@end

@implementation ZegoExpressEnginePlugin
{
    volatile FlutterEventSink _eventSink;
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    
    ZegoExpressEnginePlugin* instance = [[ZegoExpressEnginePlugin alloc] init];
    
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

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
  if ([@"getSDKVersion" isEqualToString:call.method]) {
      
      result([ZegoExpressEngine getVersion]);
      
  } else {
      result(FlutterMethodNotImplemented);
  }
}



@end
