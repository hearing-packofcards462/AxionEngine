cmake_minimum_required(VERSION 3.16)
project(AxionEngineSuperbuild LANGUAGES C CXX)

include(ExternalProject)

# Where externals will be installed
set(EXTERNAL_INSTALL_DIR ${CMAKE_BINARY_DIR}/install CACHE PATH "Install dir for heavy externals libs")

# Compiler flags
if(MSVC)
    set(DEFAULT_CXX_FLAGS "/W4 /Zi /std:c++20")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(DEFAULT_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -g")
endif()

# Slang superbuild
ExternalProject_Add(
    extern_slang
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/../slang
    BINARY_DIR ${CMAKE_BINARY_DIR}/slang-build
    INSTALL_DIR ${EXTERNAL_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        -DSLANG_ENABLE_TESTS=OFF
        -DSLANG_ENABLE_EXAMPLES=OFF
        -DSLANG_ENABLE_SLANGRT=OFF
        -DSLANG_ENABLE_SLANGD=OFF
        -DSLANG_ENABLE_PRELUDE=OFF
        -DSLANG_ENABLE_CUDA=OFF
        -DSLANG_ENABLE_CORE_SHARED=OFF
        -DSLANG_ENABLE_SLANG_RHI=OFF
        -DSLANG_ENABLE_SLANG_GFX=OFF
        -DSLANG_ENABLE_RHI_D3D11=OFF
        -DSLANG_ENABLE_RHI_D3D12=OFF
        -DSLANG_ENABLE_RHI_VULKAN=OFF
        -DSLANG_ENABLE_RHI_WEBGPU=OFF
        -DSLANG_ENABLE_LIBRARY=ON
        -DUSE_LLVM_OBJCOPY_DEBUG=OFF
        -DUSE_LLVM=OFF
        -DENABLE_WARNINGS_AS_ERRORS=OFF
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
        <BINARY_DIR>/Release/bin
        <INSTALL_DIR>/bin
)

# Expose install dir for top-level projects
set(SLANG_INSTALL_DIR ${EXTERNAL_INSTALL_DIR} CACHE PATH "Slang install dir")
