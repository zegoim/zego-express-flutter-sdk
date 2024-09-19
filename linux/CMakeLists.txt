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
include(${CMAKE_CURRENT_SOURCE_DIR}/download.cmake)
download_native_sdk("${CMAKE_CURRENT_SOURCE_DIR}")

# Copy common code
include(${CMAKE_CURRENT_SOURCE_DIR}/setup.cmake)
copy_common_code("${CMAKE_CURRENT_SOURCE_DIR}")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "zego_express_engine_plugin.cc"
  "internal/ZegoLog.h"
  "internal/ZegoLog.cpp"
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
  "internal/ZegoDataUtils.h"
  "internal/ZegoDataUtils.cpp"
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
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/${FLUTTER_TARGET_PLATFORM}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/${FLUTTER_TARGET_PLATFORM}/include/internal"
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter
  PkgConfig::GTK 
)

file(GLOB LIBRARY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/${FLUTTER_TARGET_PLATFORM}/*")

foreach(LIB_FILE ${LIBRARY_FILES})
  if(LIB_FILE MATCHES ".*\\.so.*")
    # 链接库文件到目标
    target_link_libraries(${PLUGIN_NAME} PRIVATE ${LIB_FILE})

    # 添加库文件路径到变量
    list(APPEND zego_express_engine_bundled_libraries ${LIB_FILE})
  endif()
endforeach()


# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(zego_express_engine_bundled_libraries
  ${zego_express_engine_bundled_libraries}
  PARENT_SCOPE
)

# Add $ORIGIN to RPATH so that lib/libzego_express_engine_plugin.so can find lib/libZegoExpressEngine.so at runtime
set_property(
    TARGET ${PLUGIN_NAME}
    PROPERTY BUILD_RPATH
    "\$ORIGIN"
)