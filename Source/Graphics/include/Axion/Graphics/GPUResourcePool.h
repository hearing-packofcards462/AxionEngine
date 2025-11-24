
#pragma once
#include "Axion/Graphics/RHI/Resource.h"
#include "Axion/Graphics/Handle.h"

AXION_NAMESPACE_BEGIN

namespace Graphics {

DEFINE_UNIQUE_PTR_FOR_TYPE( IGPUResourcePool, GPUResourcePool )

/// Public interface of the GPU Resource Pool.
/// Internal implementation is private to the renderer.
class IGPUResourcePool
{
public:
    virtual ~IGPUResourcePool() = default;

    virtual BufferHandle                registerBuffer( const RHI::BufferDesc& desc, const void* initialData = nullptr, const std::string& name = "" ) = 0;
    virtual RHI::BufferPtr&             getBuffer( BufferHandle handle )                                                                          = 0;
    virtual std::optional<BufferHandle> findBuffer( const std::string& name ) const                                                                    = 0;
    virtual void                        destroyBuffer( BufferHandle handle )                                                                           = 0;

    virtual void clear() = 0;
};

} // namespace Graphics

AXION_NAMESPACE_END

// // gfx/resource_pool.h
// #pragma once
// #include "gpu_handles.h"

// namespace gfx
// {
// class ResourcePool
// {
// public:
//     template<typename ResourceType, typename... Args>
//     auto createGpuResource(Args&&... args)
//     {
//         // This specialization will be implemented per resource
//         return ResourceType::Create(*this, std::forward<Args>(args)...);
//     }

//     GpuMeshHandle allocateMesh(); // calls down into RHI/Vulkan/DX12
// };
// }
// namespace gfx
// {
// struct GpuMeshHandle
// {
//     uint32_t id = 0; // or pointer to an interface in your RHI
// };
// }
// / gfx/gpu_mesh.h
// #pragma once
// #include "gpu_handles.h"
// #include "resource_pool.h"
// #include <vector>
// #include <glm/glm.hpp>

// namespace gfx
// {
// class GpuMesh
// {
// public:
//     static GpuMeshHandle Create(ResourcePool& pool,
//                                 const std::vector<glm::vec3>& vertices,
//                                 const std::vector<uint32_t>& indices)
//     {
//         // here: allocate buffers via RHI
//         auto handle = pool.allocateMesh();

//         // upload vertices + indices to GPU via RHI
//         // pool.device->createBuffer(...)

//         return handle;
//     }
// };
// // }