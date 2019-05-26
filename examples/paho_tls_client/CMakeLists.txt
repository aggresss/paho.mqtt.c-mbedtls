# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-mqtt/example subdirectory
# ------------------------------------------------------------------------------


string(REGEX MATCH "[^/]+$" SUBMODULE ${CMAKE_CURRENT_LIST_DIR})
option(${SUBMODULE} "Build ${SUBMODULE}" OFF)

include_directories(${TOP_DIRS})
include_directories(${CONFIG_DIRS})

add_compile_options(-std=gnu99)

if(${SUBMODULE} OR BUILD_ALL)
  # Build

  include_directories(${PAHO_PREFIX}/include)
  link_directories(${PAHO_PREFIX}/lib)
  include_directories(${MBEDTLS_PREFIX}/include)
  link_directories(${MBEDTLS_PREFIX}/lib)

  set(TEST_TARGET_1 "paho_tls_async")
  add_executable(${TEST_TARGET_1} ${TEST_TARGET_1}.c)
  add_dependencies(${TEST_TARGET_1} external-paho)
  target_link_libraries(${TEST_TARGET_1} paho-mqtt3as-static mbedtls mbedx509 mbedcrypto pthread)

endif()

