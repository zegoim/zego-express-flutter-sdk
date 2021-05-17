cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "zego_express_engine")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "zego_express_engine_plugin")

# Download native windows sdk with a script(python)
# If developers need to copy sdk locally, this command can be commented
execute_process(COMMAND python ${CMAKE_CURRENT_LIST_DIR}/download_native_sdk.py)

# Setup deps: native SDK header and link search path
include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/include)

# It seems that flutter doesn't support windows on x86
link_directories(${CMAKE_CURRENT_LIST_DIR}/deps/lib/x64)

add_library(${PLUGIN_NAME} SHARED
  ${CMAKE_CURRENT_LIST_DIR}/zego_express_engine_plugin.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineMethodHandler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineMethodHandler.h
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineEventHandler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineEventHandler.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin ZegoExpressEngine.lib)

# List of absolute paths to libraries that should be bundled with the plugin
set(zego_express_engine_bundled_libraries
  ${CMAKE_CURRENT_LIST_DIR}/deps/lib/x64/ZegoExpressEngine.dll
  PARENT_SCOPE
)
