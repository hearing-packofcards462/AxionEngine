
add_executable(GraphisModuleApi  main.cpp)
target_link_libraries(GraphisModuleApi PRIVATE AxionGFX)
add_test(NAME GraphisModuleApitTest COMMAND GraphisModuleApi)


