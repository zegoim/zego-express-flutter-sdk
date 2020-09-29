#import "ZegoExpressEnginePlugin.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

#import "ZegoExpressEngineMethodHandler.h"
#import "ZegoExpressEngineEventHandler.h"

#import "ZegoPlatformViewFactory.h"
#import "ZegoTextureRendererController.h"

#import "ZegoLog.h"

@interface ZegoExpressEnginePlugin() <FlutterStreamHandler>

@property (nonatomic, strong) id<FlutterPluginRegistrar> registrar;

@property (nonatomic, strong) FlutterMethodChannel *methodChannel;
@property (nonatomic, strong) FlutterEventChannel *eventChannel;

@property (nonatomic, strong) FlutterEventSink eventSink;

@end

@implementation ZegoExpressEnginePlugin

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {

    ZegoExpressEnginePlugin *instance = [[ZegoExpressEnginePlugin alloc] init];

    instance.registrar = registrar;

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

#pragma mark - Handle Event Sink

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
    ZGLog(@"[FlutterEventSink] [onListen] set eventSink: %p", _eventSink);
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
    ZGLog(@"[FlutterEventSink] [onCancel] set eventSink: %p to nil", _eventSink);
    self.eventSink = nil;
    return nil;
}

#pragma mark - Handle Method Call

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    ZGLog(@"[DartCall] [%@]", call.method);

    // When createEngine, re-create a FlutterMethodCall object to add additional parameters
    if ([@"createEngine" isEqualToString:call.method]) {

        NSMutableDictionary *argumentsMap = [NSMutableDictionary dictionaryWithDictionary:call.arguments];

        argumentsMap[@"eventSink"] = _eventSink;
        argumentsMap[@"registrar"] = _registrar;

        call = [FlutterMethodCall methodCallWithMethodName:@"createEngine" arguments:argumentsMap];
    }

    SEL selector = NSSelectorFromString([NSString stringWithFormat:@"%@:result:", call.method]);

    // Handle unrecognized method
    if (![[ZegoExpressEngineMethodHandler sharedInstance] respondsToSelector:selector]) {
        ZGLog(@"[handleMethodCall] Unrecognized selector: %@", call.method);
        result(FlutterMethodNotImplemented);
        return;
    }

    NSMethodSignature *signature = [[ZegoExpressEngineMethodHandler sharedInstance] methodSignatureForSelector:selector];

    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];

    invocation.target = [ZegoExpressEngineMethodHandler sharedInstance];
    invocation.selector = selector;

    [invocation setArgument:&call atIndex:2];
    [invocation setArgument:&result atIndex:3];

    [invocation invoke];

}

@end
