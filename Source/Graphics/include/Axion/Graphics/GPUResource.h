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