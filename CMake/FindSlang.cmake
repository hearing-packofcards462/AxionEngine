# ---------------------------------------------------------------------------------------
# FindSlang.cmake
#
# Helper for AxionEngine to import the Slang SDK built by our superbuild.
# Expected install path:
#   ${PROJECT_SOURCE_DIR}/ExternalBuild/build/install/
#
# Creates:
#   Slang::Slang  - Imported library target
#   SLANG_FOUND   - True if Slang was successfully detected
# ---------------------------------------------------------------------------------------

set(_SLANG_ROOT "${PROJECT_SOURCE_DIR}/ExternalBuild/build/install")

# Include directory
set(_SLANG_INCLUDE_DIR "${_SLANG_ROOT}/include")

# Possible library names based on platform
if (WIN32)
    set(_SLANG_LIB_NAMES "slang" "slang.lib")
elseif (APPLE)
    set(_SLANG_LIB_NAMES "libslang.dylib" "slang")
else()
    set(_SLANG_LIB_NAMES "libslang.so" "slang")
endif()

# Try to locate the actual library file
set(_SLANG_LIB_DIR "${_SLANG_ROOT}/lib")
find_library(SLANG_LIBRARY
    NAMES ${_SLANG_LIB_NAMES}
    PATHS "${_SLANG_LIB_DIR}"
)

# Final detection
if (SLANG_LIBRARY AND EXISTS "${_SLANG_INCLUDE_DIR}")
    set(SLANG_FOUND TRUE)
else()
    set(SLANG_FOUND FALSE)
endif()

# Expose as imported target
if (SLANG_FOUND AND NOT TARGET Slang::Slang)
    add_library(Slang::Slang UNKNOWN IMPORTED)

    set_target_properties(Slang::Slang PROPERTIES
        IMPORTED_LOCATION "${SLANG_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${_SLANG_INCLUDE_DIR}"
    )
endif()
