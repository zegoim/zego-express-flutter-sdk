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

bool zego_value_is_null(FlValue *value) { return fl_value_get_type(value) == FL_VALUE_TYPE_NULL; }
int64_t zego_value_get_int(FlValue *value) { return fl_value_get_int(value); }
int64_t zego_value_get_long(FlValue *value) { return fl_value_get_int(value); }
bool zego_value_get_bool(FlValue *value) { return fl_value_get_bool(value); }
double zego_value_get_double(FlValue *value) { return fl_value_get_float(value); }
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

FlValue *zego_value_list_new() { return fl_value_new_list(); }
FlValue *zego_value_map_new() { return fl_value_new_map(); }

ZFMap zego_value_get_map(FlValue *value) { return ZFlValueMap(value); }
ZFArray zego_value_get_list(FlValue *value) { return ZFlValueList(value); }

FlValue *zego_value_new(FlValue *value) { return value; }

// Class ZFlResult implementation
ZFlResult::ZFlResult(FlMethodCall *method) {
    method_call = static_cast<FlMethodCall *>(g_object_ref(method));
}
ZFlResult::~ZFlResult() { g_object_unref(method_call); }
void ZFlResult::Success(FlValue *value) {
    fl_method_call_respond_success(method_call, value, nullptr);
}
void ZFlResult::NotImplemented() { fl_method_call_respond_not_implemented(method_call, nullptr); }
void ZFlResult::Error(const std::string &code, const std::string &message, FlValue *value) {
    fl_method_call_respond_error(method_call, code.c_str(), message.c_str(), value, nullptr);
}

// Class ZFlEventSink implementation
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
ZFlEventSink::ZFlEventSink(FlEventChannel *event_channel) : event_channel(event_channel) {}
ZFlEventSink::~ZFlEventSink() {}
void ZFlEventSink::Success(FlValue *value) {
    if (g_main_context_get_thread_default() != g_main_context_default()) {
        IdleValue *idle_value = new IdleValue{event_channel, value};
        g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, func, idle_value, nullptr);
    } else {
        fl_event_channel_send(event_channel, value, nullptr, nullptr);
    }
}

// Class ZFlValueMap implementation
ZFlValueMap::ZFlValueMap() : map(fl_value_new_map()) {}
ZFlValueMap::ZFlValueMap(FlValue *value) : map(value) {}
ZFlValueMap::~ZFlValueMap() {}
ZFlValueMap::Iterator ZFlValueMap::begin() { return Iterator(*this, 0); }
ZFlValueMap::Iterator ZFlValueMap::end() { return Iterator(*this, fl_value_get_length(map)); }
ZFlValueMap::MapProxy ZFlValueMap::operator[](FlValue *key) { return MapProxy(*this, key); }
size_t ZFlValueMap::size() const { return fl_value_get_length(map); }

ZFlValueMap::Iterator::Iterator(ZFlValueMap &owner, size_t idx) : owner_(owner), index(idx) {}
std::pair<FlValue *, FlValue *> ZFlValueMap::Iterator::operator*() {
    FlValue *key = fl_value_get_map_key(owner_.map, index);
    FlValue *value = fl_value_get_map_value(owner_.map, index);
    return {key, value};
}
ZFlValueMap::Iterator &ZFlValueMap::Iterator::operator++() {
    ++index;
    return *this;
}
bool ZFlValueMap::Iterator::operator!=(const Iterator &other) const {
    return index != other.index || owner_ != other.owner_;
}

ZFlValueMap::MapProxy::MapProxy(ZFlValueMap &owner, FlValue *key)
    : owner_(owner), key_(key), exists_(false) {
    FlValue *temp = fl_value_lookup(owner.map, key_);
    exists_ = (temp != nullptr);
    if (exists_) {
        value_ = fl_value_ref(temp);
    } else {
        value_ = fl_value_new_null();
    }
}
ZFlValueMap::MapProxy::~MapProxy() { fl_value_unref(value_); }
ZFlValueMap::MapProxy &ZFlValueMap::MapProxy::operator=(FlValue *val) {
    value_ = val;
    fl_value_set(owner_.map, key_, value_);
    return *this;
}
ZFlValueMap::MapProxy::operator FlValue *() { return value_; }
ZFlValueMap::operator FlValue *() { return map; }

// Class ZFlValueList implementation
ZFlValueList::ZFlValueList() : list(fl_value_new_list()) {}
ZFlValueList::ZFlValueList(FlValue *value) : list(value) {}
ZFlValueList::~ZFlValueList() {}
FlValue *ZFlValueList::operator[](size_t index) { return fl_value_get_list_value(list, index); }
FlValue *ZFlValueList::at(size_t index) { return fl_value_get_list_value(list, index); }
void ZFlValueList::push_back(FlValue *value) { fl_value_append_take(list, value); }
void ZFlValueList::emplace_back(FlValue *value) { fl_value_append_take(list, value); }
size_t ZFlValueList::size() const { return fl_value_get_length(list); }

ZFlValueList::operator FlValue *() { return list; }

ZFlValueList::Iterator ZFlValueList::begin() { return Iterator(*this, 0); }
ZFlValueList::Iterator ZFlValueList::end() { return Iterator(*this, fl_value_get_length(list)); }

ZFlValueList::Iterator::Iterator(ZFlValueList &owner, size_t idx) : owner_(owner), index(idx) {}
FlValue *ZFlValueList::Iterator::operator*() { return fl_value_get_list_value(owner_.list, index); }
ZFlValueList::Iterator &ZFlValueList::Iterator::operator++() {
    ++index;
    return *this;
}
bool ZFlValueList::Iterator::operator!=(const Iterator &other) const {
    return index != other.index;
}