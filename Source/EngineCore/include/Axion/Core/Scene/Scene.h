// // core/scene.h
// #pragma once
// #include <entt/entt.hpp> 
// #include "components.h"

// namespace Core
// {
// class Scene
// {
// public:
//     entt::entity createEntity() { return registry.create(); }

//     template<typename T, typename... Args>
//     T& addComponent(entt::entity e, Args&&... args)
//     {
//         return registry.emplace<T>(e, std::forward<Args>(args)...);
//     }

//     entt::registry& getRegistry() { return registry; }

// private:
//     entt::registry registry;
// };
// }

// core::Scene scene;

// auto e = scene.createEntity();
// scene.addComponent<core::TransformComponent>(e);
// scene.addComponent<core::MeshComponent>(e,
//     std::vector<glm::vec3>{...},   // vertices
//     std::vector<uint32_t>{...}     // indices
// );

// gfx::ResourcePool gpuPool;
// core::SceneGpuBuilder builder(gpuPool);

// auto gpuScene = builder.build(scene);
// // gpuScene.gpuMeshes[e] gives you the mesh handle