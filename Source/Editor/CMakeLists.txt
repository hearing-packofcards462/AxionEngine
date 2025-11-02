   
file(GLOB APP_SOURCES
    "src/*.cpp"
)

add_executable(RaikoEditor ${APP_SOURCES})

target_link_libraries(RaikoEditor
   RaikoCommon
   RaikoGFX
   RaikoRHI
)


set_target_properties(RaikoEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin"
)