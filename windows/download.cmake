# Define a custom function to download and extract the native SDK
function(download_native_sdk WORKSPACE)
  message(NOTICE "[ZEGO][PLUGIN] Download native dependency")

  if(EXISTS "${WORKSPACE}/../.debugging")
    message(NOTICE "[ZEGO][PLUGIN] Found the .debugging file in the project root, no need to download dependency, exit!")
    return()
  endif()

  if($ENV{DEPS} MATCHES "^http.+")
    set(DEPSURL $ENV{DEPS})
    message(NOTICE "[ZEGO][PLUGIN][DEV] 'DEPS' env was found: ${DEPSURL}")
  else()
    file(STRINGS "${WORKSPACE}/../DEPS.yaml" DEPSYAML)
    string(REGEX MATCH "windows: .+$" DEPSURL ${DEPSYAML})
    string(REGEX REPLACE "windows: " "" DEPSURL ${DEPSURL})
  endif()

  string(REGEX MATCH "version=[0-9\.]+$" DEPSVER ${DEPSURL})
  string(REGEX REPLACE "version=" "" DEPSVER ${DEPSVER})
  message(NOTICE "[ZEGO][PLUGIN] Native version: ${DEPSVER}")

  set(LIBSDIR "${WORKSPACE}/libs")
  file(MAKE_DIRECTORY "${LIBSDIR}")

  # Check if the specific version SDK exists
  if(EXISTS "${LIBSDIR}/VERSION.txt" AND EXISTS "${LIBSDIR}/x64/ZegoExpressEngine.dll")
    file(STRINGS "${LIBSDIR}/VERSION.txt" LINES)
    list(POP_FRONT LINES VERSION)
    string(STRIP ${VERSION} VERSION)

    if(VERSION STREQUAL DEPSVER)
      message(NOTICE "[ZEGO][PLUGIN] The specified version SDK (${VERSION}) already exists!")
      return()
    else()
      message(NOTICE "[ZEGO][PLUGIN] SDK was found in cache, but the version (${VERSION}) does not match the version specified in DEPS (${DEPSVER}), overwrite!")
    endif()
  endif()

  # Download and extract the SDK
  file(DOWNLOAD "${DEPSURL}" "${LIBSDIR}/sdk.zip" TIMEOUT 60)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${LIBSDIR}/sdk.zip" WORKING_DIRECTORY "${LIBSDIR}")
  if (IS_DIRECTORY "${LIBSDIR}/release")
      set(LIBSDIR_PATH "${LIBSDIR}/release")
  else ()
      set(LIBSDIR_PATH "${LIBSDIR}/debug")
  endif()
  file(COPY "${LIBSDIR_PATH}/Library/x64" DESTINATION "${LIBSDIR}")
  file(COPY "${LIBSDIR_PATH}/VERSION.txt" DESTINATION "${LIBSDIR}")
  file(REMOVE_RECURSE "${LIBSDIR_PATH}")
  file(REMOVE "${LIBSDIR}/sdk.zip")

  message(NOTICE "[ZEGO][PLUGIN] Done!")
endfunction()
