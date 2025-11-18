#pragma once
#include <Axion/Graphics/GPUResourcePool.h>
#include <Axion/Graphics/RHI/Device.h>

AXION_NAMESPACE_BEGIN
namespace Graphics {

class GPUResourcePool final : public IGPUResourcePool
{
public:
    explicit GPUResourcePool( RHI::IDevice* device );
    ~GPUResourcePool() override;

    ~IGPUResourcePool() = default;

    BufferHandle                registerBuffer( const RHI::BufferDesc& desc, const void* initialData = nullptr, const std::string& name = "" ) override;
    RHI::BufferPtr& const*      getBuffer( BufferHandle handle ) const override;
    std::optional<BufferHandle> findBuffer( const std::string& name ) const override;
    void                        destroyBuffer( BufferHandle handle ) override;

    virtual void clear() = 0;

private:
    RHI::IDevice*      m_device = nullptr;
    mutable std::mutex _mutex;

    template <typename T>
    struct ResourceRecord {
        T           buffer = nullptr;
        std::string name;
        bool        alive = false;
    };
    std::vector<ResourceRecord<RHI::BufferPtr>> m_buffers;
    std::unordered_map<std::string, BufferHandle> _nameToHandle;
};

} // namespace Graphics
AXION_NAMESPACE_END
