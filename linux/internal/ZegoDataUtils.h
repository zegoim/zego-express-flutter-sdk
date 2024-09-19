#pragma once

#include <flutter_linux/flutter_linux.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Linux specific definitions
#define ZFValue(varName) zego_value_new(varName)
#define ZFMap ZFlValueMap
#define ZFArray ZFlValueList
#define ZFArgument ZFlValueMap &
#define ZFResult std::unique_ptr<ZFlResult>
#define ZFEventSink std::unique_ptr<ZFlEventSink>
#define ZFMoveResult(result) std::shared_ptr<ZFlResult>(std::move(result))
#define ZFPluginRegistrar FlPluginRegistrar
#define ZFBinaryMessenger FlBinaryMessenger
#define ZFTextureRegistrar FlTextureRegistrar

class ZFlResult;
class ZFlEventSink;
class ZFlValueMap;
class ZFlValueList;

// Function declarations...
FlValue *zego_value_new(int value);
FlValue *zego_value_new(int64_t value);
FlValue *zego_value_new(long long value);
FlValue *zego_value_new(bool value);
FlValue *zego_value_new(double value);
FlValue *zego_value_new(const char *value);
FlValue *zego_value_new(std::string value);
FlValue *zego_value_new(std::vector<uint8_t> value);
FlValue *zego_value_new(std::vector<float> value);

FlValue *zego_value_new(FlValue *value);
bool zego_value_is_null(FlValue *value);
int64_t zego_value_get_int(FlValue *value);
int64_t zego_value_get_long(FlValue *value);
bool zego_value_get_bool(FlValue *value);
double zego_value_get_double(FlValue *value);
std::string zego_value_get_string(FlValue *value);
std::vector<float> zego_value_get_vector_float(FlValue *value);
std::vector<uint8_t> zego_value_get_vector_uint8(FlValue *value);
ZFMap zego_value_get_map(FlValue *value);
ZFArray zego_value_get_list(FlValue *value);

// Class declarations...
class ZFlResult {
private:
    FlMethodCall *method_call;
public:
    ZFlResult(FlMethodCall *method_call);
    ~ZFlResult();

    void Success(FlValue *value = nullptr);
    void NotImplemented();
    void Error(const std::string &code, const std::string &message, FlValue *value = nullptr);
};

class ZFlEventSink {
private:
    FlEventChannel *event_channel;
public:
    ZFlEventSink(FlEventChannel *event_channel);
    ~ZFlEventSink();

    void Success(FlValue *value = nullptr);
};

class ZFlValueMap {
private:
    FlValue *map;
public:
    ZFlValueMap();
    explicit ZFlValueMap(FlValue *value);
    ~ZFlValueMap();

    class Iterator {
    public:
        Iterator(ZFlValueMap &owner, size_t index);
        std::pair<FlValue *, FlValue *> operator*();
        Iterator &operator++();
        bool operator!=(const Iterator &other) const;
    private:
        ZFlValueMap &owner_;
        size_t index;
    };

    Iterator begin();
    Iterator end();

    class MapProxy {
    public:
        MapProxy(ZFlValueMap &owner, FlValue *key);
        ~MapProxy();
        MapProxy &operator=(FlValue *val);
        operator FlValue *();
    private:
        ZFlValueMap &owner_;
        FlValue *key_;
        FlValue *value_;
        bool exists_;
    };

    MapProxy operator[](FlValue *key);
    size_t size() const;
    operator FlValue *();
};

class ZFlValueList {
private:
    FlValue *list;
public:
    ZFlValueList();
    explicit ZFlValueList(FlValue *value);
    ~ZFlValueList();

    FlValue *operator[](size_t index);
    FlValue *at(size_t index);
    void push_back(FlValue *value);
    void emplace_back(FlValue *value);
    size_t size() const;
    operator FlValue *();

    class Iterator {
    public:
        Iterator(ZFlValueList &owner, size_t index);
        FlValue *operator*();
        Iterator &operator++();
        bool operator!=(const Iterator &other) const;
    private:
        ZFlValueList &owner_;
        size_t index;
    };

    Iterator begin();
    Iterator end();
};