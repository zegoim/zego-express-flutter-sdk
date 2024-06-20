# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

# Project-level configuration.
set(PROJECT_NAME "zego_express_engine")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed.
set(PLUGIN_NAME "zego_express_engine_plugin")

# Download native SDK
include(${CMAKE_CURRENT_LIST_DIR}/download.cmake)
download_native_sdk("${CMAKE_CURRENT_LIST_DIR}")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "zego_express_engine_plugin.cc"
  "ZegoDataTypeConvert.cpp"
  "ZegoDataTypeConvert.h"
  "ZegoLog.h"
  "ZegoLog.cpp"
  # "internal/DataToImageTools.hpp"
  "internal/ZegoExpressEngineInterface.h"
  "internal/ZegoExpressEngineMethodHandler.cpp"
  "internal/ZegoExpressEngineMethodHandler.h"
  "internal/ZegoExpressEngineEventHandler.h"
  "internal/ZegoExpressEngineEventHandler.cpp"
  "internal/ZegoTextureRendererController.h"
  "internal/ZegoTextureRendererController.cpp"
  "internal/ZegoTextureRenderer.h"
  "internal/ZegoTextureRenderer.cpp"
  "internal/ZegoTexture.h"
  "internal/ZegoTexture.cc"
  "internal/ZegoUtils.h"
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

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include"
  "${CMAKE_CURRENT_LIST_DIR}/libs/x86_64/include"
  "${CMAKE_CURRENT_LIST_DIR}/libs/x86_64/include/internal"
)

target_link_libraries(${PLUGIN_NAME} PRIVATE 
  flutter
  PkgConfig::GTK
  ${CMAKE_CURRENT_LIST_DIR}/libs/x86_64/libZegoExpressEngine.so
)

# target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK ZegoExpressEngine)

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(zego_express_engine_bundled_libraries
  PARENT_SCOPE
)