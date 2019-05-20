# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-mqtt/third_party
# ------------------------------------------------------------------------------

include(ExternalProject)

option(WITH_DEPS "Build with deps" ON)
if(WITH_DEPS)
  ExternalProject_Add(external-deps
    SOURCE_DIR ${PROJECT_BINARY_DIR}/third_party/deps
    GIT_REPOSITORY https://github.com/aggresss/deps.git
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
    set(DEPS_PREFIX ${PROJECT_BINARY_DIR}/third_party/deps PARENT_SCOPE)
endif(WITH_DEPS)

option(WITH_MBEDTLS "Build with mbedtls" ON)
if(WITH_MBEDTLS)
  ExternalProject_Add(external-mbedtls
    URL https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.16.1.tar.gz
    UPDATE_COMMAND ""
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/mbedtls
      -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE:STRING=Debug
      -DENABLE_TESTING:BOOL=OFF
      -DENABLE_PROGRAMS:BOOL=ON
    TEST_COMMAND ""
    )
    set(MBEDTLS_PREFIX ${PROJECT_BINARY_DIR}/third_party/mbedtls PARENT_SCOPE)
endif(WITH_MBEDTLS)

option(WITH_WOLFSSL "Build with wolfssl" OFF)
if(WITH_WOLFSSL)
  ExternalProject_Add(external-wolfSSL
    BUILD_IN_SOURCE TRUE
    URL https://github.com/wolfSSL/wolfssl/archive/v4.0.0-stable.tar.gz
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./autogen.sh &&
      ./configure
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        --enable-static=${THIRDPARTY_WITH_STATIC}
        --enable-shared=${THIRDPARTY_WITH_SHARED}
        --disable-debug
        --host=${C_COMPILER_MACHINE}
        --prefix=${PROJECT_BINARY_DIR}/third_party/wolfssl
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    TEST_COMMAND ""
    )
    set(WOLFSSL_PREFIX ${PROJECT_BINARY_DIR}/third_party/wolfssl PARENT_SCOPE)
endif(WITH_WOLFSSL)



