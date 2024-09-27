#include "ZegoDataUtils.h"

bool zego_value_is_null(flutter::EncodableValue value) { return value.IsNull(); }

int32_t zego_value_get_int(flutter::EncodableValue value) {
    // dart 没有 int32_t int64_t 区分，这里处理了 int32 最高位为 1（负数）的 case
    return (int32_t)zego_value_get_long(value);
}

int64_t zego_value_get_long(flutter::EncodableValue value) { return value.LongValue(); }

bool zego_value_get_bool(flutter::EncodableValue value) { return std::get<bool>(value); }

double zego_value_get_double(flutter::EncodableValue value) { return std::get<double>(value); }

std::string zego_value_get_string(flutter::EncodableValue value) {
    return std::get<std::string>(value);
}

std::vector<float> zego_value_get_vector_float(flutter::EncodableValue value) {
    return std::get<std::vector<float>>(value);
}

std::vector<uint8_t> zego_value_get_vector_uint8(flutter::EncodableValue value) {
    return std::get<std::vector<uint8_t>>(value);
}

ZFMap zego_value_get_map(flutter::EncodableValue value) { return std::get<ZFMap>(value); }

ZFArray zego_value_get_list(flutter::EncodableValue value) { return std::get<ZFArray>(value); }