
#pragma once
#include "Axion/Graphics/RHI/Resource.h"
#include "Axion/Graphics/Handle.h"

AXION_NAMESPACE_BEGIN

namespace Graphics {



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
