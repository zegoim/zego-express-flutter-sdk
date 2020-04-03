package im.zego.zego_express_engine;

import android.app.Application;

import androidx.annotation.NonNull;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;
import io.flutter.view.TextureRegistry;

/** ZegoExpressEnginePlugin */
public class ZegoExpressEnginePlugin implements FlutterPlugin, MethodCallHandler, EventChannel.StreamHandler {

    private EventChannel.EventSink sink;
    private Class<?> manager;
    private final Application application;
    private final TextureRegistry textureRegistry;

    private HashMap<String, Method> methodHashMap = new HashMap<>();

    private ZegoExpressEnginePlugin(Application application, TextureRegistry textureRegistry) {
        this.application = application;
        this.textureRegistry = textureRegistry;

        try {
            this.manager = Class.forName("im.zego.zego_express_engine.ZegoExpressEngineMethodHandler");
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {

        ZegoExpressEnginePlugin plugin = new ZegoExpressEnginePlugin((Application) flutterPluginBinding.getApplicationContext(), flutterPluginBinding.getTextureRegistry());

        final MethodChannel methodChannel = new MethodChannel(flutterPluginBinding.getFlutterEngine().getDartExecutor(), "plugins.zego.im/zego_express_engine");
        methodChannel.setMethodCallHandler(plugin);

        final EventChannel eventChannel = new EventChannel(flutterPluginBinding.getFlutterEngine().getDartExecutor(), "plugins.zego.im/zego_express_event_handler");
        eventChannel.setStreamHandler(plugin);

    }

    // This static function is optional and equivalent to onAttachedToEngine. It supports the old
    // pre-Flutter-1.12 Android projects. You are encouraged to continue supporting
    // plugin registration via this function while apps migrate to use the new Android APIs
    // post-flutter-1.12 via https://flutter.dev/go/android-project-migration.
    //
    // It is encouraged to share logic between onAttachedToEngine and registerWith to keep
    // them functionally equivalent. Only one of onAttachedToEngine or registerWith will be called
    // depending on the user's project. onAttachedToEngine or registerWith must both be defined
    // in the same class.
    public static void registerWith(Registrar registrar) {

        ZegoExpressEnginePlugin plugin = new ZegoExpressEnginePlugin((Application) registrar.context(), registrar.textures());

        final MethodChannel methodChannel = new MethodChannel(registrar.messenger(), "plugins.zego.im/zego_express_engine");
        methodChannel.setMethodCallHandler(plugin);

        final EventChannel eventChannel = new EventChannel(registrar.messenger(), "plugins.zego.im/zego_express_event_handler");
        eventChannel.setStreamHandler(plugin);

    }

    @Override
    public void onListen(Object arguments, EventChannel.EventSink events) {
        this.sink = events;
    }

    @Override
    public void onCancel(Object arguments) {
        this.sink = null;
    }

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
        try {
            Method method = methodHashMap.get(call.method);
            if (method == null) {
                if (call.method.equals("createEngine")) {
                    method = this.manager.getMethod(call.method, MethodCall.class, Result.class, Application.class, EventChannel.EventSink.class, TextureRegistry.class);
                } else {
                    method = this.manager.getMethod(call.method, MethodCall.class, Result.class);
                }
                methodHashMap.put(call.method, method);
            }

            if (call.method.equals("createEngine")) {
                method.invoke(null, call, result, this.application, this.sink, this.textureRegistry);
            } else {
                method.invoke(null, call, result);
            }

        } catch (NoSuchMethodException e) {
            result.notImplemented();
        } catch (IllegalAccessException e) {
            result.notImplemented();
        } catch (InvocationTargetException e) {
            result.notImplemented();
        }
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    }
}
