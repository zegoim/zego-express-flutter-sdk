# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "zego_express_engine")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "zego_express_engine_plugin")

# Download native SDK
include(${CMAKE_CURRENT_LIST_DIR}/download.cmake)
download_native_sdk("${CMAKE_CURRENT_LIST_DIR}")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/zego_express_engine_plugin.cpp
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/zego_express_engine_plugin.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoCustomVideoCaptureManager.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoCustomVideoDefine.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoCustomVideoProcessManager.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoCustomVideoRenderManager.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoMediaPlayerAudioManager.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoMediaPlayerVideoManager.h
  ${CMAKE_CURRENT_LIST_DIR}/include/zego_express_engine/ZegoMediaPlayerBlockDataManager.h
  ${CMAKE_CURRENT_LIST_DIR}/ZegoCustomVideoCaptureManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoCustomVideoProcessManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoCustomVideoRenderManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoMediaPlayerAudioManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoMediaPlayerVideoManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoMediaPlayerBlockDataManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ZegoLog.h
  ${CMAKE_CURRENT_LIST_DIR}/ZegoLog.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/DataToImageTools.hpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineEventHandler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineEventHandler.h
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineMethodHandler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoExpressEngineMethodHandler.h
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoTextureRenderer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoTextureRenderer.h
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoTextureRendererController.cpp
  ${CMAKE_CURRENT_LIST_DIR}/internal/ZegoTextureRendererController.h
)

# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
add_library(${PLUGIN_NAME} SHARED
  ${PLUGIN_SOURCES}
)

# Apply a standard set of build settings that are configured in the
# application-level CMakeLists.txt. This can be removed for plugins that want
# full control over build settings.
apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# /WX- Don't mistake warnings for errors
# /bigobj Used to fix this problem (Error: C1128 sections exceed object file format limit)
target_compile_options(${PLUGIN_NAME} PRIVATE /W4 /WX- /wd4100 /wd4267 /wd4189 /wd4244 /wd4996 /bigobj /utf-8)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include"
  "${CMAKE_CURRENT_LIST_DIR}/libs/x64/include"
  "${CMAKE_CURRENT_LIST_DIR}/libs/x64/include/internal"
)
target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter
  flutter_wrapper_plugin
  ${CMAKE_CURRENT_LIST_DIR}/libs/x64/ZegoExpressEngine.lib
)

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
#set(zego_express_engine_bundled_libraries
#  ${CMAKE_CURRENT_LIST_DIR}/libs/x64/ZegoExpressEngine.dll
#  PARENT_SCOPE
#)

file(GLOB zego_express_engine_bundled_libraries
     "${CMAKE_CURRENT_LIST_DIR}/libs/x64/*.dll"
)

set(zego_express_engine_bundled_libraries ${zego_express_engine_bundled_libraries} PARENT_SCOPE)
