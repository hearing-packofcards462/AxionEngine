#pragma once
#include "Raiko/Graphics/RHI/Common.h"

RAIKO_NAMESPCE_BEGIN

namespace Graphics::RHI {

DEFINE_HANDLE_FOR_TYPE( ITexture, Texture )

class ITexture : public IResource
{
public:
    struct Description {
        Extent3D         size        = { 1, 1, 1 };
        Format           format      = Format::UNKNOWN;
        TextureDimension dimension   = TextureDimension::Texture2D;
        uint             mipLevels   = 1;
        uint             sampleCount = 1;
        uint             arraySize   = 1;
        std::string      debugName   = "";
        TextureViewFlags viewFlags = TextureViewShaderResource;
    };
    virtual ~ITexture()                                         = default;
    virtual const ITexture::Description& getDescription() const = 0;
};

typedef ITexture::Description TextureDesc;

DEFINE_HANDLE_FOR_TYPE( IBuffer, Buffer )

class IBuffer : public IResource
{
public:
    struct Description {
        size_t      size       = 0;
        uint        stride     = 0; // for structured buffers
        MemoryUsage memory     = MemoryUsage::GPUOnly;
        BufferUsage usageFlags = BufferUsage::None;
        BufferViewFlags  viewFlags  = BufferViewNone;
        std::string debugName  = "";
    };

    virtual ~IBuffer()                                = default;
    virtual const Description& getDescription() const = 0;

    //
    // CPU access
    //
    virtual void* map()   = 0;
    virtual void  unmap() = 0;
};

using BufferDesc = IBuffer::Description;

DEFINE_HANDLE_FOR_TYPE( IAccel, Accel )

class IAccel : public IResource
{
public:
    virtual ~IAccel() = default;
};

} // namespace Graphics::RHI

RAIKO_NAMESPCE_END