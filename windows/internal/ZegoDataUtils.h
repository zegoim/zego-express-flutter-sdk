#pragma once

#include <flutter/encodable_value.h>
#include <flutter/event_channel.h>
#include <memory>

#define ZFValue(varName) flutter::EncodableValue(varName)
#define ZFMap flutter::EncodableMap
#define ZFArray flutter::EncodableList

#define ZFArgument flutter::EncodableMap &
#define ZFResult std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
#define ZFEventSink std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>
#define ZFMoveResult(result) std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result))

#define ZFPluginRegistrar flutter::PluginRegistrarWindows
#define ZFBinaryMessenger flutter::BinaryMessenger
#define ZFTextureRegistrar flutter::TextureRegistrar

bool zego_value_is_null(flutter::EncodableValue value);

int32_t zego_value_get_int(flutter::EncodableValue value);
int64_t zego_value_get_long(flutter::EncodableValue value);
bool zego_value_get_bool(flutter::EncodableValue value);
double zego_value_get_double(flutter::EncodableValue value);
std::string zego_value_get_string(flutter::EncodableValue value);
std::vector<float> zego_value_get_vector_float(flutter::EncodableValue value);
std::vector<uint8_t> zego_value_get_vector_uint8(flutter::EncodableValue value);
ZFMap zego_value_get_map(flutter::EncodableValue value);
ZFArray zego_value_get_list(flutter::EncodableValue value);