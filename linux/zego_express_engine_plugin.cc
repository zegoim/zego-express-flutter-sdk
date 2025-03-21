#include "include/zego_express_engine/zego_express_engine_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include "internal/ZegoLog.h"
#include "internal/ZegoExpressEngineEventHandler.h"
#include "internal/ZegoExpressEngineMethodHandler.h"
#include "internal/ZegoExpressEngineInterface.h"

#define ZEGO_EXPRESS_ENGINE_PLUGIN(obj)                                                            \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), zego_express_engine_plugin_get_type(),                      \
                                ZegoExpressEnginePlugin))

struct _ZegoExpressEnginePlugin {
    GObject parent_instance;
};

G_DEFINE_TYPE(ZegoExpressEnginePlugin, zego_express_engine_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void zego_express_engine_plugin_handle_method_call(ZegoExpressEnginePlugin *self,
                                                          FlMethodCall *method_call) {

    std::unique_ptr<ZFlResult> result(new ZFlResult(method_call));

    const gchar *method = fl_method_call_get_name(method_call);

    ZF::logInfo("[DartCall][%s]", method);

    FlValue *args = fl_method_call_get_args(method_call);
    ZFMap argument(args);

    auto pair = G_MethodMap.find(std::string(method));
    if (pair != G_MethodMap.end()) {
        try {
            if (!pair->second.second &&
                !ZegoExpressEngineMethodHandler::getInstance().isEngineCreated()) {
                fl_method_call_respond_error(method_call, "Engine_not_created",
                                             "Please call createEngineWithProfile first", nullptr,
                                             nullptr);
            } else {
                pair->second.first(argument, std::move(result));
            }
        } catch (std::exception &e) {
            fl_method_call_respond_error(method_call, "method_call_error", e.what(), nullptr,
                                         nullptr);
        }
    } else {
        fl_method_call_respond_not_implemented(method_call, nullptr);
    }
}

FlMethodResponse *get_platform_version() {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
}

static void zego_express_engine_plugin_dispose(GObject *object) {
    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    ZegoExpressEngineMethodHandler::getInstance().clearPluginRegistrar();
    G_OBJECT_CLASS(zego_express_engine_plugin_parent_class)->dispose(object);
}

static void zego_express_engine_plugin_class_init(ZegoExpressEnginePluginClass *klass) {
    G_OBJECT_CLASS(klass)->dispose = zego_express_engine_plugin_dispose;
}

static void zego_express_engine_plugin_init(ZegoExpressEnginePlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data) {
    ZegoExpressEnginePlugin *plugin = ZEGO_EXPRESS_ENGINE_PLUGIN(user_data);
    zego_express_engine_plugin_handle_method_call(plugin, method_call);
}

static FlMethodErrorResponse *listen_cb(FlEventChannel *channel, FlValue *args,
                                        gpointer user_data) {
    std::unique_ptr<ZFlEventSink> events(new ZFlEventSink(channel));
    ZegoExpressEngineEventHandler::getInstance()->setEventSink(std::move(events));
    return NULL;
}

static FlMethodErrorResponse *cancel_cb(FlEventChannel *channel, FlValue *args,
                                        gpointer user_data) {
    ZegoExpressEngineEventHandler::getInstance()->clearEventSink();
    return NULL;
}

void zego_express_engine_plugin_register_with_registrar(FlPluginRegistrar *registrar) {
    ZegoExpressEnginePlugin *plugin =
        ZEGO_EXPRESS_ENGINE_PLUGIN(g_object_new(zego_express_engine_plugin_get_type(), nullptr));

    g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
    g_autoptr(FlBinaryMessenger) messenger = fl_plugin_registrar_get_messenger(registrar);
    g_autoptr(FlTextureRegistrar) textureRegistrar = fl_plugin_registrar_get_texture_registrar(registrar);
    g_autoptr(FlMethodChannel) method_channel = fl_method_channel_new(
        messenger, "plugins.zego.im/zego_express_engine", FL_METHOD_CODEC(codec));
    fl_method_channel_set_method_call_handler(method_channel, method_call_cb, g_object_ref(plugin),
                                              g_object_unref);
    FlEventChannel *event_channel = fl_event_channel_new(
        messenger, "plugins.zego.im/zego_express_event_handler", FL_METHOD_CODEC(codec));
    fl_event_channel_set_stream_handlers(event_channel, listen_cb, cancel_cb, NULL, NULL);

    ZegoExpressEngineMethodHandler::getInstance().setPluginBinaryMessenger(messenger);
    ZegoExpressEngineMethodHandler::getInstance().setPluginTexture(textureRegistrar);

    g_object_unref(plugin);
}