#include "ZegoDataTypeConvert.h"

bool zego_value_is_null(flutter::EncodableValue value) {
    return value.IsNull();
}

int zego_value_get_int(flutter::EncodableValue value) {
    return std::get<int32_t>(value);
}

int zego_value_get_long(flutter::EncodableValue value) {
    return value.LongValue();
}

bool zego_value_get_bool(flutter::EncodableValue value) {
    return std::get<bool>(value);
}

double zego_value_get_double(flutter::EncodableValue value) {
    return std::get<double>(value);
}

std::string zego_value_get_string(flutter::EncodableValue value) {
    return std::get<std::string>(value);
}

std::vector<float> zego_value_get_vector_float(flutter::EncodableValue value) {
    return std::get<std::vector<float>>(value);
}

std::vector<uint8_t> zego_value_get_vector_uint8(flutter::EncodableValue value) {
    return std::get<std::vector<uint8_t>>(value);
}

FTMap zego_value_get_map(flutter::EncodableValue value) {
    return std::get<FTMap>(value);
}

FTArray zego_value_get_list(flutter::EncodableValue value) {
    return std::get<FTArray>(value);
}