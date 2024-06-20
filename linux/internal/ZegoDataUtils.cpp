#include "ZegoDataUtils.h"

FlValue *zego_value_new(int value) { return fl_value_new_int(value); }
FlValue *zego_value_new(int64_t value) { return fl_value_new_int(value); }
FlValue *zego_value_new(long long value) { return fl_value_new_int(value); }
FlValue *zego_value_new(bool value) { return fl_value_new_bool(value); }
FlValue *zego_value_new(double value) { return fl_value_new_float(value); }
FlValue *zego_value_new(const char *value) { return fl_value_new_string(value); }
FlValue *zego_value_new(std::string value) { return fl_value_new_string(value.c_str()); }

FlValue *zego_value_new(std::vector<uint8_t> value) {
    return fl_value_new_uint8_list(value.data(), value.size());
}
FlValue *zego_value_new(std::vector<float> value) {
    return fl_value_new_float32_list(value.data(), value.size());
}

FlValue *zego_value_list_new() { return fl_value_new_list(); }
FlValue *zego_value_map_new() { return fl_value_new_map(); }

bool zego_value_is_null(FlValue *value) { return fl_value_get_type(value) == FL_VALUE_TYPE_NULL; }
int64_t zego_value_get_int(FlValue *value) { return fl_value_get_int(value); }
int64_t zego_value_get_long(FlValue *value) { return fl_value_get_int(value); }
bool zego_value_get_bool(FlValue *value) { return fl_value_get_bool(value); }
double zego_value_get_double(FlValue *value) { return fl_value_get_float(value); }
FTMap zego_value_get_map(FlValue *value) { return FlValueMap(value); }
FTArray zego_value_get_list(FlValue *value) { return FlValueList(value); }

std::string zego_value_get_string(FlValue *value) {
    return std::string(fl_value_get_string(value));
}
std::vector<float> zego_value_get_vector_float(FlValue *value) {
    std::vector<float> vec(fl_value_get_float32_list(value),
                           fl_value_get_float32_list(value) + fl_value_get_length(value));
    return vec;
}
std::vector<uint8_t> zego_value_get_vector_uint8(FlValue *value) {
    std::vector<uint8_t> vec(fl_value_get_uint8_list(value),
                             fl_value_get_uint8_list(value) + fl_value_get_length(value));
    return vec;
}


FlResult::FlResult(FlMethodCall *method) {
    method_call = static_cast<FlMethodCall *>(g_object_ref(method));
};

FlResult::~FlResult() { g_object_unref(method_call); }

void FlResult::Success() { Success(nullptr); }
void FlResult::Success(FlValue *value) {
    fl_method_call_respond_success(method_call, value, nullptr);
}

void FlResult::NotImplemented() { fl_method_call_respond_not_implemented(method_call, nullptr); }

void FlResult::Error(const std::string &code, const std::string &message, FlValue *value) {
    fl_method_call_respond_error(method_call, code.c_str(), message.c_str(), value, nullptr);
}

void FlResult::Error(const std::string &code, const std::string &message = "") {
    Error(code, message, nullptr);
}

typedef struct {
    FlEventChannel *channel;
    FlValue *value;
} IdleValue;
static gboolean func(gpointer user_data) {
    IdleValue *data = static_cast<IdleValue *>(user_data);
    fl_event_channel_send(data->channel, data->value, nullptr, nullptr);
    delete data;
    return FALSE;
}
FlEventSink::FlEventSink(FlEventChannel *event) : event_channel(event){};

void FlEventSink::Success() { Success(nullptr); }
void FlEventSink::Success(FlValue *value) {
    if (g_main_context_get_thread_default() != g_main_context_default()) {
        IdleValue *idle_value = new IdleValue{event_channel, value};
        g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, func, idle_value, nullptr);
    } else {
        fl_event_channel_send(event_channel, value, nullptr, nullptr);
    }
}

