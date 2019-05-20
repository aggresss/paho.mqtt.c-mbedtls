# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/paho.mqtt.c-mbedtls/examples
# ------------------------------------------------------------------------------

SUBDIRLIST(SUB_DIRS ${CMAKE_CURRENT_LIST_DIR})
foreach(subdir ${SUB_DIRS})
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt)
    add_subdirectory(${subdir})
  endif()
endforeach()