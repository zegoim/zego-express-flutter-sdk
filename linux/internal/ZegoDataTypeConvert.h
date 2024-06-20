#pragma once

#include <flutter_linux/flutter_linux.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define FTValue(varName) zego_value_new(varName)
#define FTMap FlValueMap
#define FTArray FlValueList

#define FTArgument FlValueMap &
#define FTResult std::unique_ptr<FlResult>
#define FTMoveResult(result) std::shared_ptr<FlResult>(std::move(result))
#define FTEventSink std::unique_ptr<FlEventSink>

#define FTPluginRegistrar FlPluginRegistrar
#define FTBinaryMessenger FlBinaryMessenger
#define FTTextureRegistrar FlTextureRegistrar

class FlResult {
  private:
    FlMethodCall *method_call;

  public:
    FlResult(FlMethodCall *method) {
        method_call = static_cast<FlMethodCall *>(g_object_ref(method));
    };

    ~FlResult() {
        g_object_unref(method_call);
    }

    void Success() { Success(nullptr); }
    void Success(FlValue *value) { fl_method_call_respond_success(method_call, value, nullptr); }

    void NotImplemented() { fl_method_call_respond_not_implemented(method_call, nullptr); }

    void Error(const std::string &code, const std::string &message, FlValue *value) {
        fl_method_call_respond_error(method_call, code.c_str(), message.c_str(), value, nullptr);
    }

    void Error(const std::string &code, const std::string &message = "") {
        Error(code, message, nullptr);
    }
};

class FlEventSink {
  private:
    FlEventChannel *event_channel;

  public:
    FlEventSink(FlEventChannel *event) : event_channel(event){};

    void Success() { Success(nullptr); }
    void Success(FlValue *value) {
        if (g_main_context_get_thread_default() != g_main_context_default()) {
          IdleValue* idle_value = new IdleValue{event_channel, value};
          g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, func, idle_value, nullptr);
        } else {
          fl_event_channel_send(event_channel, value, nullptr, nullptr);
        }
    }
  private:
    typedef struct {
        FlEventChannel* channel;
        FlValue* value;
    } IdleValue;
    static gboolean func(gpointer user_data) {
      IdleValue* data = static_cast<IdleValue*>(user_data);
      fl_event_channel_send(data->channel, data->value, nullptr, nullptr);
      delete data;
      return FALSE;
    }
};

class FlValueMap {
  private:
    FlValue *map;

  public:
    FlValueMap() : map(fl_value_new_map()) {}

    FlValueMap(FlValue *value) : map(value) {}

    class Iterator {
      private:
        FlValueMap &owner_;
        size_t index;

      public:
        Iterator(FlValueMap &owner, size_t idx) : owner_(owner), index(idx) {}

        // 解引用运算符返回键值对
        std::pair<FlValue *, FlValue *> operator*() {
            FlValue *key = fl_value_get_map_key(owner_.map, index);
            FlValue *value = fl_value_get_map_value(owner_.map, index);
            ;
            return {key, value};
        }

        Iterator &operator++() {
            ++index;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return index != other.index || owner_ != other.owner_;
        }
    };

    Iterator begin() { return Iterator(*this, 0); }

    Iterator end() { return Iterator(*this, fl_value_get_length(map)); }

    class MapProxy {
      public:
        MapProxy(FlValueMap &owner, FlValue *key) : owner_(owner), key_(key), exists_(false) {
            // 检查键是否存在
            FlValue *temp = fl_value_lookup(owner.map, key_);
            exists_ = (temp != nullptr);
            if (exists_) {
                value_ = fl_value_ref(temp);
            } else {
                value_ = fl_value_new_null(); // 或者你想要的默认类型
            }
        }

        ~MapProxy() { fl_value_unref(value_); }

        // 赋值操作
        MapProxy &operator=(FlValue *val) {
            // 更新 map
            value_ = val;
            fl_value_set(owner_.map, key_, value_);

            return *this;
        }

        operator FlValue *() { return value_; }

      private:
        FlValueMap &owner_;
        FlValue *key_;
        FlValue *value_;
        bool exists_;
    };

    // 提供 operator[] 以创建 MapProxy 实例
    MapProxy operator[](FlValue *key) { return MapProxy(*this, key); }

    // 获取列表长度
    size_t size() const { return fl_value_get_length(map); }

    operator FlValue *() { return map; }
};

class FlValueList {
  private:
    FlValue *list;

  public:
    FlValueList() : list(fl_value_new_list()) {}

    FlValueList(FlValue *value) : list(value) {}

    ~FlValueList() {}

    // 提供 operator[] 以创建 ListProxy 实例
    FlValue *operator[](size_t index) { return fl_value_get_list_value(list, index); }

    // 添加元素到列表尾部
    void push_back(FlValue *value) { fl_value_append_take(list, value); }

    void emplace_back(FlValue *value) { fl_value_append_take(list, value); }

    // 获取列表长度
    size_t size() const { return fl_value_get_length(list); }

    operator FlValue *() { return list; }

    // 获取指定索引处的元素
    FlValue *at(size_t index) { return fl_value_get_list_value(list, index); }

    class Iterator {
      private:
        FlValueList &owner_;
        size_t index;

      public:
        Iterator(FlValueList &owner, size_t idx) : owner_(owner), index(idx) {}

        // 解引用运算符返回键值对
        FlValue *operator*() { return fl_value_get_list_value(owner_.list, index); }

        Iterator &operator++() {
            ++index;
            return *this;
        }

        bool operator!=(const Iterator &other) const { return index != other.index; }
    };

    Iterator begin() { return Iterator(*this, 0); }

    Iterator end() { return Iterator(*this, fl_value_get_length(list)); }
};

FlValue *zego_value_new(int value);
FlValue *zego_value_new(int64_t value);
FlValue *zego_value_new(long long value);
FlValue *zego_value_new(bool value);
FlValue *zego_value_new(double value);
FlValue *zego_value_new(const char *value);
FlValue *zego_value_new(std::string value);
FlValue *zego_value_new(std::vector<uint8_t> value);
FlValue *zego_value_new(std::vector<float> value);
bool zego_value_is_null(FlValue *value);
int64_t zego_value_get_int(FlValue *value);
int64_t zego_value_get_long(FlValue *value);
bool zego_value_get_bool(FlValue *value);
double zego_value_get_double(FlValue *value);
std::string zego_value_get_string(FlValue *value);
std::vector<float> zego_value_get_vector_float(FlValue *value);
std::vector<uint8_t> zego_value_get_vector_uint8(FlValue *value);
FTMap zego_value_get_map(FlValue *value);
FTArray zego_value_get_list(FlValue *value);
