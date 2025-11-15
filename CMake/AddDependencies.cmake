# =============================================================================
# Helper for Axion Engine to handle third-party libraries
# =============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")


# -------------------------
# GLM (header-only)
# -------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glm-1.0.2)
# -------------------------
# GLFW (optional, compiled)
# -------------------------
option(BUILD_GLFW "Build GLFW as part of this project" ON)
if(BUILD_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glfw)
endif()
# -------------------------
# Fmt (header only)
# -------------------------
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)       
set(FMT_TEST OFF CACHE BOOL "" FORCE)          
set(FMT_DOC OFF CACHE BOOL "" FORCE)           
set(FMT_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/fmt)
# -------------------------
# Slang (prebuilt)
# -------------------------
set(CMAKE_PREFIX_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/ExternalBuild/install"
    ${CMAKE_PREFIX_PATH}
)

find_package(Slang REQUIRED)