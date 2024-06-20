# Define a custom function to copy window platform universal code
function(copy_universal_code WORKSPACE)
  message(NOTICE "[ZEGO][PLUGIN] Copy the windows universal code to the Linux platform.")

  set(WINDOWS_IMPL_INTERNAL_DIR "${WORKSPACE}/../windows/internal/universal")
  set(LINUX_IMPL_INTERNAL_DIR "${WORKSPACE}/internal")

  if(EXISTS "${LINUX_IMPL_INTERNAL_DIR}")
    message(NOTICE "[ZEGO][PLUGIN] Remove ${LINUX_IMPL_INTERNAL_DIR}/universal")
    file(REMOVE_RECURSE "${LINUX_IMPL_INTERNAL_DIR}/universal")
  endif()

  message(NOTICE "[ZEGO][PLUGIN] Copy ${WINDOWS_IMPL_INTERNAL_DIR} to ${LINUX_IMPL_INTERNAL_DIR}")
  file(COPY "${WINDOWS_IMPL_INTERNAL_DIR}" DESTINATION "${LINUX_IMPL_INTERNAL_DIR}")

  endfunction()
