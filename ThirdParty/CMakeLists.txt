# GLM Mathematics library
add_subdirectory(glm-1.0.2)
# GLFW (Optional)
if(BUILD_GLFW)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
endif()
# FORMATTING
add_subdirectory(fmt)
# IMAGE LOADING
# IMGUI

# 3D OBJECTS
