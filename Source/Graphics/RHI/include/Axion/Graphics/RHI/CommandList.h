#pragma once
#include "Axion/Common/Math.h"
#include "Axion/Graphics/RHI/Common.h"
#include "Axion/Graphics/RHI/Resource.h"

AXION_NAMESPACE_BEGIN

namespace Graphics::RHI {

DEFINE_HANDLE_FOR_TYPE( ICommandList, CommandList )

class ICommandList : public IResource
{
public:
    struct Description {
        QueueType queueType;
        uint      numFrames = 1;
    };

    virtual ~ICommandList() = default;

    virtual void               begin()                       = 0;
    virtual void               end()                         = 0;
    virtual void               setCurrentFrame( uint index ) = 0;
    virtual const Description& getDescription() const        = 0;

    virtual void barrier( const TextureHandle& texture, ResourceState newState )                                                          = 0;
    virtual void barrier( const BufferHandle& buffer, ResourceState newState )                                                            = 0;
    virtual void clearTexture( const TextureHandle& texture, const ClearValue& clearValue )                                               = 0;
    virtual void copyBuffer( const BufferHandle& dst, const BufferHandle& src, ulong numBytes, ulong dstOffset = 0, ulong srcOffset = 0 ) = 0;

    // virtual void beginRenderPass( /* ... */ ) = 0;
    // virtual void endRenderPass()              = 0;
    // virtual void bindPipeline( /* ... */ ) = 0;
    // virtual void draw( uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0 ) = 0;
    // virtual void dispatch( uint32_t x, uint32_t y, uint32_t z )                                                                 = 0;
};

typedef ICommandList::Description CommandListDesc;

} // namespace Graphics::RHI

AXION_NAMESPACE_END