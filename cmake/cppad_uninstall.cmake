set(MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")

if(EXISTS ${MANIFEST})
  file(STRINGS ${MANIFEST} files)
  foreach(file ${files})
    if(IS_SYMLINK ${file} OR EXISTS ${file})
      message(STATUS "Removing file: '${file}'")

      execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove ${file}
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
        )

      if(NOT "${rm_retval}" STREQUAL 0)
        message(FATAL_ERROR "Failed to remove file: '${file}'.")
      endif()
    else()
      message(STATUS "File '${file}' does not exist.")
    endif()
  endforeach(file)
else()
  message(FATAL_ERROR "Cannot find install manifest: ${MANIFEST}")
endif()
