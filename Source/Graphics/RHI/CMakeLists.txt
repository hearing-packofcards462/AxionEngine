# /////////////////////////////////////////////////////////////////////////////
# Raikō Engine Graphics::RHI Lower module
# /////////////////////////////////////////////////////////////////////////////

file(GLOB RHI_SOURCES
    "src/DX12/*.cpp"
    "src/DX12/*.hpp"
    "src/DX12/*.h"
    "src/Vulkan/*.cpp"
    "src/Vulkan/*.hpp"
    "src/Vulkan/*.h"
    "src/*.h"
    "src/*.cpp"
)

add_library(RaikoRHI STATIC ${RHI_SOURCES})


target_include_directories(RaikoRHI
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include   
${CMAKE_SOURCE_DIR}/ThirdParty/Directx/include    
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src  
)

# Link system libraries
target_link_libraries(RaikoRHI
PUBLIC
# Direct X12
dxgi.lib
d3d12.lib
d3dcompiler.lib
user32.lib
dxguid.lib
# Vulkan
#Internal Modules
RaikoCommon
)

# Si quieres flags específicos por backend
if(BUILD_GLFW)
    target_link_libraries(RaikoRHI PUBLIC glfw )
    target_compile_definitions(RaikoRHI PRIVATE BUILD_GLFW)
endif()
