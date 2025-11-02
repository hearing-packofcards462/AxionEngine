# /////////////////////////////////////////////////////////////////////////////
# Raikō Engine Common module
# /////////////////////////////////////////////////////////////////////////////

file(GLOB COMMON_SOURCES
    "src/*.cpp"
)


add_library(RaikoCommon STATIC ${COMMON_SOURCES})


target_include_directories(RaikoCommon
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include   
)

target_link_libraries(RaikoCommon PUBLIC glm fmt::fmt)


