import Cocoa
import FlutterMacOS

public class ZegoExpressEnginePlugin: NSObject, FlutterPlugin, FlutterStreamHandler {

    var registrar: FlutterPluginRegistrar?
    var methodChannel: FlutterMethodChannel?
    var eventChannel: FlutterEventChannel?
    var eventSink: FlutterEventSink?

    public static func register(with registrar: FlutterPluginRegistrar) {
        let instance = ZegoExpressEnginePlugin()
        instance.registrar = registrar

        let methodChannel = FlutterMethodChannel.init(name: "plugins.zego.im/zego_express_engine", binaryMessenger: registrar.messenger)
        registrar.addMethodCallDelegate(instance, channel: methodChannel)
        instance.methodChannel = methodChannel

        let eventChannel = FlutterEventChannel.init(name: "plugins.zego.im/zego_express_event_handler", binaryMessenger: registrar.messenger)
        eventChannel.setStreamHandler(instance)
        instance.eventChannel = eventChannel

        // Register platform view factory
        registrar.register(ZegoPlatformViewFactory.sharedInstance(), withId: "plugins.zego.im/zego_express_view")

        // init api called callback
        ZegoExpressEngineMethodHandler.sharedInstance().initApiCalledCallback()
    }


    // MARK: Handle Event Sink

    public func onListen(withArguments arguments: Any?, eventSink events: @escaping FlutterEventSink) -> FlutterError? {
        self.eventSink = events
        return nil
    }

    public func onCancel(withArguments arguments: Any?) -> FlutterError? {
        self.eventSink = nil
        return nil
    }

    // MARK: Handle Method Call

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        if call.method.hasPrefix("createEngine") {
            ZegoExpressEngineMethodHandler.sharedInstance().setRegistrar(self.registrar!, eventSink: self.eventSink!)
        }
        ZegoExpressEngineMethodHandler.sharedInstance().handle(call, result: result)
    }
}
