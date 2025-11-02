# /////////////////////////////////////////////////////////////////////////////
# Axion Engine Common module
# /////////////////////////////////////////////////////////////////////////////

file(GLOB COMMON_SOURCES
    "src/*.cpp"
)


add_library(AxionCommon STATIC ${COMMON_SOURCES})


target_include_directories(AxionCommon
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include   
)

target_link_libraries(AxionCommon PUBLIC glm fmt::fmt)


