# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-mqtt/third_party
# ------------------------------------------------------------------------------

include(ExternalProject)

ExternalProject_Add(external-mbedtls
  SOURCE_DIR ${THIRD_PARTY_DIRS}/mbedtls
  UPDATE_COMMAND ""
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/mbedtls
    -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
    -DCMAKE_BUILD_TYPE:STRING=Debug
    -DENABLE_TESTING:BOOL=OFF
    -DENABLE_PROGRAMS:BOOL=OFF
  TEST_COMMAND ""
  )
set(MBEDTLS_PREFIX ${PROJECT_BINARY_DIR}/third_party/mbedtls PARENT_SCOPE)

ExternalProject_Add(external-paho
  DEPENDS external-mbedtls
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/paho.mqtt.c
  URL ""
  UPDATE_COMMAND ""
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/paho
    -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
    -DCMAKE_BUILD_TYPE:STRING=Debug
    -DPAHO_WITH_SSL:BOOL=FALSE
    -DPAHO_BUILD_DOCUMENTATION:BOOL=FALSE
    -DPAHO_BUILD_SAMPLES:BOOL=FALSE
    -DPAHO_ENABLE_TESTING:BOOL=FALSE
    -DPAHO_BUILD_STATIC:BOOL=TRUE
    -DPAHO_WITH_MBEDTLS:BOOL=TRUE
    -DMBEDTLS_SEARCH_PATH:PATH=${PROJECT_BINARY_DIR}/third_party/mbedtls
  TEST_COMMAND ""
  )
set(PAHO_PREFIX ${PROJECT_BINARY_DIR}/third_party/paho PARENT_SCOPE)


