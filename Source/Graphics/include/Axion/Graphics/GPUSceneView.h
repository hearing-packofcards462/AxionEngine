#pragma once
#include "Axion/Graphics/RHI/Device.h"

AXION_NAMESPACE_BEGIN

namespace Graphics {

class GPUSceneView { };

} // namespace Graphics

// // core/scene_gpu_builder.h
// #pragma once
// #include "scene.h"
// #include "../gfx/resource_pool.h"
// #include "../gfx/gpu_mesh.h"

// namespace core
// {
// struct SceneGpuResources
// {
//     // For now only meshes — later textures, materials, etc.
//     std::unordered_map<entt::entity, gfx::GpuMeshHandle> gpuMeshes;
// };

// class SceneGpuBuilder
// {
// public:
//     SceneGpuBuilder(gfx::ResourcePool& pool) : pool(pool) {}

//     SceneGpuResources build(const Scene& scene)
//     {
//         SceneGpuResources out;

//         auto& reg = scene.getRegistry();
//         auto meshView = reg.view<MeshComponent>();

//         for (auto e : meshView)
//         {
//             const auto& mesh = meshView.get<MeshComponent>(e);

//             auto handle = pool.createGpuResource<gfx::GpuMesh>(mesh.vertices, mesh.indices);
//             out.gpuMeshes[e] = handle;
//         }
//         return out;
//     }

// private:
//     gfx::ResourcePool& pool;
// };
// }

AXION_NAMESPACE_END