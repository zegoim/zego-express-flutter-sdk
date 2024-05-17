#pragma once

#include <flutter_linux/flutter_linux.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>

#define FTValue(varName) zego_value_new(varName)
#define FTMap FlValueMap
#define FTArray FlValueList

#define FTArgument FlValueMap&
#define FTResult std::unique_ptr<FlResult>
#define FTMoveResult(result) std::shared_ptr<FlResult>(std::move(result))

class FlResult {
private:
    FlMethodCall* method_call;
    
public:
    FlResult(FlMethodCall* method) : method_call(method) {};

    void Success() {
        Success(nullptr);
    }
    void Success(FlValue *value) {
        fl_method_call_respond_success(method_call, value, nullptr);
    }

    void Error(const std::string &code, const std::string &message, FlValue *value) {
        fl_method_call_respond_error(method_call, code.c_str(), message.c_str(), value, nullptr);
    }

    void Error(const std::string &code, const std::string &message = "") {
        Error(code, message, nullptr);
    }
};

class FlValueMap {
private:
    FlValue* map;

public:
    FlValueMap() : map(fl_value_new_map()) {}

    FlValueMap(FlValue *value) : map(value) {}

    class MapProxy {
    public:
        MapProxy(FlValueMap& owner, FlValue* key)
            : owner_(owner), key_(key), exists_(false) {
            // 检查键是否存在
            FlValue* temp = fl_value_lookup(owner.map, key_);
            exists_ = (temp != nullptr);
            if (exists_) {
                value_ = fl_value_ref(temp);
            } else {
                value_ = fl_value_new_null(); // 或者你想要的默认类型
            }
        }

        ~MapProxy() {
            fl_value_unref(value_);
        }

        // 赋值操作
        MapProxy& operator=(FlValue* val) {
            // 更新 map
            value_ = val;
            fl_value_set(owner_.map, key_, value_);

            return *this;
        }

        operator FlValue*() {
            return value_;
        }

    private:
        FlValueMap& owner_;
        FlValue* key_;
        FlValue* value_;
        bool exists_;
    };

    // 提供 operator[] 以创建 MapProxy 实例
    MapProxy operator[](FlValue *key) {
        return MapProxy(*this, key);
    }

    operator FlValue*() {
        return map;
    }
};


class FlValueList {
private:
    FlValue* list;

public:
    FlValueList() : list(fl_value_new_list()) {}

    FlValueList(FlValue *value) : list(value) {} 

    ~FlValueList() {}

    // 提供 operator[] 以创建 ListProxy 实例
    FlValue* operator[](size_t index) {
        return fl_value_get_list_value(list, index);;
    }

    // 添加元素到列表尾部
    void push_back(FlValue* value) {
        fl_value_append_take(list, value);
    }

    void emplace_back(FlValue *value) {
        fl_value_append_take(list, value);
    }

    // 获取列表长度
    size_t size() const {
        return fl_value_get_length(list);
    }

    operator FlValue*() {
        return list;
    }
};


FlValue* zego_value_new(int value);

FlValue* zego_value_new(bool value);

FlValue* zego_value_new(double value);

FlValue* zego_value_new(const char* value);

bool zego_value_is_null(FlValue* value);

int zego_value_get_int(FlValue* value);

int zego_value_get_long(FlValue* value);

bool zego_value_get_bool(FlValue* value);

double zego_value_get_double(FlValue* value);

std::string zego_value_get_string(FlValue* value);

std::vector<float> zego_value_get_vector_float(FlValue* value);

std::vector<uint8_t> zego_value_get_vector_uint8(FlValue* value);

FTMap zego_value_get_map(FlValue* value);

FTArray zego_value_get_list(FlValue* value);


