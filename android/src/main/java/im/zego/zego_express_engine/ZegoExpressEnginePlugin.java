package im.zego.zego_express_engine;

import androidx.annotation.NonNull;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;

import im.zego.zego_express_engine.internal.ZegoLog;
import im.zego.zego_express_engine.internal.ZegoPlatformViewFactory;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

import static im.zego.zego_express_engine.internal.ZegoUtils.getStackTrace;

/** ZegoExpressEnginePlugin */
public class ZegoExpressEnginePlugin implements FlutterPlugin, MethodCallHandler, EventChannel.StreamHandler {

    private Registrar registrar;
    private FlutterPluginBinding pluginBinding;

    private MethodChannel methodChannel;
    private EventChannel eventChannel;

    private EventChannel.EventSink sink;

    private final Class<?> manager;
    private final HashMap<String, Method> methodHashMap = new HashMap<>();

    public ZegoExpressEnginePlugin() {
        try {
            this.manager = Class.forName("im.zego.zego_express_engine.internal.ZegoExpressEngineMethodHandler");
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }


    /* FlutterPlugin Interface */

    // V2 embedding

    @SuppressWarnings("deprecation")
    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {

        MethodChannel methodChannel = new MethodChannel(flutterPluginBinding.getFlutterEngine().getDartExecutor(), "plugins.zego.im/zego_express_engine");
        EventChannel eventChannel = new EventChannel(flutterPluginBinding.getFlutterEngine().getDartExecutor(), "plugins.zego.im/zego_express_event_handler");

        // Register platform view factory
        flutterPluginBinding.getPlatformViewRegistry().registerViewFactory("plugins.zego.im/zego_express_view", ZegoPlatformViewFactory.getInstance());

        this.setupPlugin(null, flutterPluginBinding, methodChannel, eventChannel);
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {

        this.methodChannel.setMethodCallHandler(null);
        this.methodChannel = null;

        this.eventChannel.setStreamHandler(null);
        this.eventChannel = null;

        this.pluginBinding = null;
    }


    /* Adapt to Flutter versions before 1.12 */

    // V1 embedding

    // This static function is optional and equivalent to onAttachedToEngine. It supports the old
    // pre-Flutter-1.12 Android projects. You are encouraged to continue supporting
    // plugin registration via this function while apps migrate to use the new Android APIs
    // post-flutter-1.12 via https://flutter.dev/go/android-project-migration.
    //
    // It is encouraged to share logic between onAttachedToEngine and registerWith to keep
    // them functionally equivalent. Only one of onAttachedToEngine or registerWith will be called
    // depending on the user's project. onAttachedToEngine or registerWith must both be defined
    // in the same class.
    @SuppressWarnings("unused")
    public static void registerWith(Registrar registrar) {

        MethodChannel methodChannel = new MethodChannel(registrar.messenger(), "plugins.zego.im/zego_express_engine");
        EventChannel eventChannel = new EventChannel(registrar.messenger(), "plugins.zego.im/zego_express_event_handler");

        // Register platform view factory
        registrar.platformViewRegistry().registerViewFactory("plugins.zego.im/zego_express_view", ZegoPlatformViewFactory.getInstance());

        ZegoExpressEnginePlugin plugin = new ZegoExpressEnginePlugin();
        plugin.setupPlugin(registrar, null, methodChannel, eventChannel);
    }


    /* Setup ZegoExpressEngine Plugin */

    private void setupPlugin(Registrar registrar, FlutterPluginBinding pluginBinding, MethodChannel methodChannel, EventChannel eventChannel) {

        this.registrar = registrar;
        this.pluginBinding = pluginBinding;

        this.methodChannel = methodChannel;
        this.methodChannel.setMethodCallHandler(this);

        this.eventChannel = eventChannel;
        this.eventChannel.setStreamHandler(this);

        try {
            Method initApiCalledCallbackMethod = this.manager.getMethod("initApiCalledCallback");
            initApiCalledCallbackMethod.invoke(null);
        } catch (NoSuchMethodException e) {
            ZegoLog.log("[DartCall] [NoSuchMethodException] [%s] %s | %s", "initApiCalledCallback", e.getMessage(), getStackTrace(e));
        } catch (IllegalAccessException e) {
            ZegoLog.log("[DartCall] [IllegalAccessException] [%s] %s | %s", "initApiCalledCallback", e.getMessage(), getStackTrace(e));
        } catch (InvocationTargetException e) {
            Throwable t = e.getTargetException();
            ZegoLog.log("[DartCall] [InvocationTargetException] [%s] %s | %s | %s", "initApiCalledCallback", t.getCause(), t.getMessage(), getStackTrace(t));
        }
    }


    /* EventChannel.StreamHandler Interface */

    @Override
    public void onListen(Object arguments, EventChannel.EventSink events) {
        this.sink = events;
        ZegoLog.log("[FlutterEventSink] [onListen] set eventSink: %d", this.sink.hashCode());
    }

    @Override
    public void onCancel(Object arguments) {
        ZegoLog.log("[FlutterEventSink] [onCancel] set eventSink: %d to null", this.sink.hashCode());
        this.sink = null;
    }



    /* MethodCallHandler Interface */

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
        ZegoLog.log("[DartCall] [%s]", call.method);
        try {
            Method method = methodHashMap.get(call.method);
            if (method == null) {
                if (call.method.equals("createEngine")) {
                    method = this.manager.getMethod(call.method, MethodCall.class, Result.class, Registrar.class, FlutterPluginBinding.class, EventChannel.EventSink.class);
                } else if (call.method.equals("createEngineWithProfile")) {
                    method = this.manager.getMethod(call.method, MethodCall.class, Result.class, Registrar.class, FlutterPluginBinding.class, EventChannel.EventSink.class);
                } else {
                    method = this.manager.getMethod(call.method, MethodCall.class, Result.class);
                }
                methodHashMap.put(call.method, method);
            }

            if (call.method.equals("createEngine")) {
                method.invoke(null, call, result, this.registrar, this.pluginBinding, this.sink);
            } else if (call.method.equals("createEngineWithProfile")) {
                method.invoke(null, call, result, this.registrar, this.pluginBinding, this.sink);
            } else {
                method.invoke(null, call, result);
            }

        } catch (NoSuchMethodException e) {
            ZegoLog.log("[DartCall] [NoSuchMethodException] [%s] %s | %s", call.method, e.getMessage(), getStackTrace(e));
            result.notImplemented();
        } catch (IllegalAccessException e) {
            ZegoLog.log("[DartCall] [IllegalAccessException] [%s] %s | %s", call.method, e.getMessage(), getStackTrace(e));
            result.error("IllegalAccessException", String.format("[%s] %s", call.method, e.getMessage()), null);
        } catch (InvocationTargetException e) {
            Throwable t = e.getTargetException();
            ZegoLog.log("[DartCall] [InvocationTargetException] [%s] %s | %s | %s", call.method, t.getCause(), t.getMessage(), getStackTrace(t));
            result.error("InvocationTargetException", String.format("[%s] %s", call.method, t.getMessage()), null);
        }
    }

}
