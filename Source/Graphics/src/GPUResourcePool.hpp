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

    // BUFFER
    BufferHandle                registerBuffer( const RHI::BufferDesc& desc, const void* initialData = nullptr, const std::string& name = "" ) override;
    RHI::BufferPtr&             getBuffer( BufferHandle handle )  override;
    std::optional<BufferHandle> findBuffer( const std::string& name ) const override;
    void                        destroyBuffer( BufferHandle handle ) override;
    
    // TEXTURE

    virtual void clear() override;

private:
    RHI::IDevice*      _device = nullptr;
    mutable std::mutex _mutex;

    template <typename T>
    struct ResourceRecord {
        T           ptr = nullptr;
        std::string name;
        bool        alive = false;
    };

    //Buffers
    std::vector<ResourceRecord<RHI::BufferPtr>>   _buffers;
    std::unordered_map<std::string, BufferHandle> _nameToHandle;
    //Textures
    // std::vector<ResourceRecord<RHI::TexturePtr>>   _textures;
    // std::unordered_map<std::string, TextureHandle> _nameToHandle;
};

} // namespace Graphics
AXION_NAMESPACE_END
