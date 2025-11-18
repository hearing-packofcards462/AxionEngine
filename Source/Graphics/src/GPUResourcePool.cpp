// #include "ResourcePool.h"

// AXION_NAMESPACE_BEGIN
// namespace Graphics {

// ResourcePool::ResourcePool(IDevice* device)
//     : m_device(device)
// {}

// ResourcePool::~ResourcePool()
// {
//     clear();
// }

// BufferHandle ResourcePool::registerBuffer(const BufferDesc& desc,
//                                           const void* initialData,
//                                           const std::string& name)
// {
//     std::scoped_lock lock(m_mutex);

//     IBuffer* buffer = m_device->createBuffer(desc, initialData);

//     // Find free slot
//     uint32_t id = UINT32_MAX;
//     for (uint32_t i = 0; i < m_buffers.size(); ++i) {
//         if (!m_buffers[i].alive) {
//             id = i;
//             m_buffers[i] = { buffer, name, true };
//             break;
//         }
//     }

//     // Or append new one
//     if (id == UINT32_MAX) {
//         id = (uint32_t)m_buffers.size();
//         m_buffers.push_back({ buffer, name, true });
//     }

//     // Map name
//     if (!name.empty())
//         m_nameToHandle[name] = { id };

//     return BufferHandle{ id };
// }

// IBuffer* ResourcePool::getBuffer(BufferHandle handle) const
// {
//     std::scoped_lock lock(m_mutex);

//     if (!handle.isValid() || handle.id >= m_buffers.size())
//         return nullptr;

//     const BufferRecord& rec = m_buffers[handle.id];
//     return rec.alive ? rec.buffer : nullptr;
// }

// std::optional<BufferHandle> ResourcePool::findBuffer(const std::string& name) const
// {
//     std::scoped_lock lock(m_mutex);

//     auto it = m_nameToHandle.find(name);
//     if (it == m_nameToHandle.end()) return std::nullopt;
//     return it->second;
// }

// void ResourcePool::destroyBuffer(BufferHandle handle)
// {
//     std::scoped_lock lock(m_mutex);

//     if (!handle.isValid() || handle.id >= m_buffers.size()) return;

//     auto& rec = m_buffers[handle.id];
//     if (rec.alive) {
//         delete rec.buffer;
//         rec.buffer = nullptr;
//         rec.alive = false;

//         if (!rec.name.empty())
//             m_nameToHandle.erase(rec.name);
//     }
// }

// void ResourcePool::clear()
// {
//     std::scoped_lock lock(m_mutex);

//     for (auto& rec : m_buffers) {
//         // if (rec.alive) {
//             delete rec.buffer;
//             rec.buffer = nullptr;
//             rec.alive = false;
//         }
//     }

//     m_nameToHandle.clear();
// }

// } // namespace Graphics
// AXION_NAMESPACE_END
