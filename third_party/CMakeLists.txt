# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-mqtt/third_party
# ------------------------------------------------------------------------------

include(ExternalProject)

ExternalProject_Add(external-mbedtls
  URL http://repo.router7.com/libs/mbedtls-2.16.1.tar.gz
  URL https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.16.1.tar.gz
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
    -DPAHO_MBEDTLS_PATH:STRING=${PROJECT_BINARY_DIR}/third_party/mbedtls
  TEST_COMMAND ""
  )
set(PAHO_PREFIX ${PROJECT_BINARY_DIR}/third_party/paho PARENT_SCOPE)


