#pragma once

#include <flutter/encodable_value.h>
#include <memory>

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

bool zego_value_is_null(flutter::EncodableValue value);

int64_t zego_value_get_int(flutter::EncodableValue value);

int64_t zego_value_get_long(flutter::EncodableValue value);

bool zego_value_get_bool(flutter::EncodableValue value);

double zego_value_get_double(flutter::EncodableValue value);

std::string zego_value_get_string(flutter::EncodableValue value);

std::vector<float> zego_value_get_vector_float(flutter::EncodableValue value);

std::vector<uint8_t> zego_value_get_vector_uint8(flutter::EncodableValue value);

FTMap zego_value_get_map(flutter::EncodableValue value);

FTArray zego_value_get_list(flutter::EncodableValue value);


