#include "ZegoDataTypeConvert.h"


// FlValueMap::

FlValue* zego_value_new(int value) {
    return fl_value_new_int(value);
}

FlValue* zego_value_new(bool value) {
    return fl_value_new_bool(value);
}

FlValue* zego_value_new(double value) {
    return fl_value_new_float(value);
}

FlValue* zego_value_new(const char* value) {
    return fl_value_new_string(value);
}

FlValue* zego_value_list_new() {
    return fl_value_new_list();
}

FlValue* zego_value_map_new() {
    return fl_value_new_map();
}

bool zego_value_is_null(FlValue* value) { 
    return fl_value_get_type(value) == FL_VALUE_TYPE_NULL;
}

int zego_value_get_int(FlValue* value) {
    return fl_value_get_int(value);
}

int zego_value_get_long(FlValue* value) {
    return fl_value_get_int(value);
}

bool zego_value_get_bool(FlValue* value) {
    return fl_value_get_bool(value);
}

double zego_value_get_double(FlValue* value) {
    return fl_value_get_float(value);
}

std::string zego_value_get_string(FlValue* value) {
    return std::string(fl_value_get_string(value));
}

std::vector<float> zego_value_get_vector_float(FlValue* value) {
     std::vector<float> vec(fl_value_get_float32_list(value), fl_value_get_float32_list(value) + fl_value_get_length(value));
     return vec;
}

std::vector<uint8_t> zego_value_get_vector_uint8(FlValue* value) {
    std::vector<uint8_t> vec(fl_value_get_uint8_list(value), fl_value_get_uint8_list(value) + fl_value_get_length(value));
    return vec;
}

FTMap zego_value_get_map(FlValue* value) {
    return FlValueMap(value);
}

FTArray zego_value_get_list(FlValue* value) {
    return FlValueList(value);
}