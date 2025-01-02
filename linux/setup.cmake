# Define a custom function to copy window platform common code
function(copy_common_code WORKSPACE)
  message(NOTICE "[ZEGO][PLUGIN] Copy the windows common code to the Linux platform.")

  set(WINDOWS_IMPL_INTERNAL_LIST
    ${WORKSPACE}/../windows/internal/ZegoExpressEngineEventHandler.h
    ${WORKSPACE}/../windows/internal/ZegoExpressEngineEventHandler.cpp
    ${WORKSPACE}/../windows/internal/ZegoExpressEngineInterface.h
    ${WORKSPACE}/../windows/internal/ZegoExpressEngineMethodHandler.h
    ${WORKSPACE}/../windows/internal/ZegoExpressEngineMethodHandler.cpp
    ${WORKSPACE}/../windows/internal/ZegoTextureRendererController.h
    ${WORKSPACE}/../windows/internal/ZegoTextureRendererController.cpp
    ${WORKSPACE}/../windows/internal/ZegoUtils.h
    ${WORKSPACE}/../windows/internal/ZegoLog.h
    ${WORKSPACE}/../windows/internal/ZegoLog.cpp
  )
  set(LINUX_IMPL_INTERNAL_DIR "${WORKSPACE}/internal")
  file(COPY ${WINDOWS_IMPL_INTERNAL_LIST} DESTINATION ${LINUX_IMPL_INTERNAL_DIR})

  endfunction()
