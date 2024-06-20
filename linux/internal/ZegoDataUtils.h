#pragma once

#ifdef _WIN32

#define FTValue(varName) flutter::EncodableValue(varName)
#define FTMap flutter::EncodableMap
#define FTArray flutter::EncodableList

#define FTArgument flutter::EncodableMap
#define FTResult std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
#define FTEventSink std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>
#define FTMoveResult(result) std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result))

#define FTPluginRegistrar flutter::PluginRegistrarWindows
#define FTBinaryMessenger flutter::BinaryMessenger
#define FTTextureRegistrar flutter::TextureRegistrar

#elif defined(__linux__)
#include <flutter_linux/flutter_linux.h>

#define FTValue(varName) zego_value_new(varName)
#define FTMap FlValueMap
#define FTArray FlValueList

#define FTArgument FlValueMap &
#define FTResult std::unique_ptr<FlResult>
#define FTEventSink std::unique_ptr<FlEventSink>
#define FTMoveResult(result) std::shared_ptr<FlResult>(std::move(result))

#define FTPluginRegistrar FlPluginRegistrar
#define FTBinaryMessenger FlBinaryMessenger
#define FTTextureRegistrar FlTextureRegistrar

#endif
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#ifdef __linux__
class FlResult {
  private:
    FlMethodCall *method_call;

  public:
    FlResult(FlMethodCall *method);
    ~FlResult();
    void Success();
    void Success(FlValue *value);
    void NotImplemented();
    void Error(const std::string &code, const std::string &message, FlValue *value);
    void Error(const std::string &code, const std::string &message);
};

class FlEventSink {
  private:
    FlEventChannel *event_channel;

  public:
    FlEventSink(FlEventChannel *event);
    void Success();
    void Success(FlValue *value);
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

        MapProxy &operator=(FlValue *val) {
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

    MapProxy operator[](FlValue *key) { return MapProxy(*this, key); }

    size_t size() const { return fl_value_get_length(map); }

    operator FlValue *() { return map; }
};

class FlValueList {
  private:
    FlValue *list;

  public:
    FlValueList() : list(fl_value_new_list()) {}
    FlValueList(FlValue *value) : list(value) {}

    FlValue *operator[](size_t index) { return fl_value_get_list_value(list, index); }

    void push_back(FlValue *value) { fl_value_append_take(list, value); }

    void emplace_back(FlValue *value) { fl_value_append_take(list, value); }

    size_t size() const { return fl_value_get_length(list); }

    operator FlValue *() { return list; }

    FlValue *at(size_t index) { return fl_value_get_list_value(list, index); }

    class Iterator {
      private:
        FlValueList &owner_;
        size_t index;

      public:
        Iterator(FlValueList &owner, size_t idx) : owner_(owner), index(idx) {}

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
#endif

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
