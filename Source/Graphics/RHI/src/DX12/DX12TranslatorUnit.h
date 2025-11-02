#pragma once
#include "Axion/Graphics/RHI/Common.h"
#include <array>

AXION_NAMESPCE_BEGIN
namespace Graphics::RHI {
// Translates engine flags to DirectX12 types
namespace DX12Translator {

constexpr DXGI_FORMAT get( Format formatType ) {
    switch ( formatType )
    {
        case Format::UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;

        // 8-bit
        case Format::R8_UINT:
            return DXGI_FORMAT_R8_UINT;
        case Format::R8_SINT:
            return DXGI_FORMAT_R8_SINT;
        case Format::R8_UNORM:
            return DXGI_FORMAT_R8_UNORM;
        case Format::R8_SNORM:
            return DXGI_FORMAT_R8_SNORM;

        // RG8
        case Format::RG8_UINT:
            return DXGI_FORMAT_R8G8_UINT;
        case Format::RG8_SINT:
            return DXGI_FORMAT_R8G8_SINT;
        case Format::RG8_UNORM:
            return DXGI_FORMAT_R8G8_UNORM;
        case Format::RG8_SNORM:
            return DXGI_FORMAT_R8G8_SNORM;

        // 16-bit
        case Format::R16_UINT:
            return DXGI_FORMAT_R16_UINT;
        case Format::R16_SINT:
            return DXGI_FORMAT_R16_SINT;
        case Format::R16_UNORM:
            return DXGI_FORMAT_R16_UNORM;
        case Format::R16_SNORM:
            return DXGI_FORMAT_R16_SNORM;
        case Format::R16_FLOAT:
            return DXGI_FORMAT_R16_FLOAT;

        // 16-bit packed
        case Format::BGRA4_UNORM:
            return DXGI_FORMAT_B4G4R4A4_UNORM;
        case Format::B5G6R5_UNORM:
            return DXGI_FORMAT_B5G6R5_UNORM;
        case Format::B5G5R5A1_UNORM:
            return DXGI_FORMAT_B5G5R5A1_UNORM;

        // 8-bit RGBA
        case Format::RGBA8_UINT:
            return DXGI_FORMAT_R8G8B8A8_UINT;
        case Format::RGBA8_SINT:
            return DXGI_FORMAT_R8G8B8A8_SINT;
        case Format::RGBA8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case Format::RGBA8_SNORM:
            return DXGI_FORMAT_R8G8B8A8_SNORM;
        case Format::BGRA8_UNORM:
            return DXGI_FORMAT_B8G8R8A8_UNORM;
        case Format::BGRX8_UNORM:
            return DXGI_FORMAT_B8G8R8X8_UNORM;
        case Format::SRGBA8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case Format::SBGRA8_UNORM:
            return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case Format::SBGRX8_UNORM:
            return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

        // HDR-ish
        case Format::R10G10B10A2_UNORM:
            return DXGI_FORMAT_R10G10B10A2_UNORM;
        case Format::R11G11B10_FLOAT:
            return DXGI_FORMAT_R11G11B10_FLOAT;

        // RG16
        case Format::RG16_UINT:
            return DXGI_FORMAT_R16G16_UINT;
        case Format::RG16_SINT:
            return DXGI_FORMAT_R16G16_SINT;
        case Format::RG16_UNORM:
            return DXGI_FORMAT_R16G16_UNORM;
        case Format::RG16_SNORM:
            return DXGI_FORMAT_R16G16_SNORM;
        case Format::RG16_FLOAT:
            return DXGI_FORMAT_R16G16_FLOAT;

        // 32-bit single
        case Format::R32_UINT:
            return DXGI_FORMAT_R32_UINT;
        case Format::R32_SINT:
            return DXGI_FORMAT_R32_SINT;
        case Format::R32_FLOAT:
            return DXGI_FORMAT_R32_FLOAT;

        // RGBA16
        case Format::RGBA16_UINT:
            return DXGI_FORMAT_R16G16B16A16_UINT;
        case Format::RGBA16_SINT:
            return DXGI_FORMAT_R16G16B16A16_SINT;
        case Format::RGBA16_FLOAT:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case Format::RGBA16_UNORM:
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        case Format::RGBA16_SNORM:
            return DXGI_FORMAT_R16G16B16A16_SNORM;

        // RG32
        case Format::RG32_UINT:
            return DXGI_FORMAT_R32G32_UINT;
        case Format::RG32_SINT:
            return DXGI_FORMAT_R32G32_SINT;
        case Format::RG32_FLOAT:
            return DXGI_FORMAT_R32G32_FLOAT;

        // RGB32
        case Format::RGB32_UINT:
            return DXGI_FORMAT_R32G32B32_UINT;
        case Format::RGB32_SINT:
            return DXGI_FORMAT_R32G32B32_SINT;
        case Format::RGB32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;

        // RGBA32
        case Format::RGBA32_UINT:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case Format::RGBA32_SINT:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case Format::RGBA32_FLOAT:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        // Depth / Stencil
        case Format::D16:
            return DXGI_FORMAT_D16_UNORM;
        case Format::D24S8:
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case Format::X24G8_UINT:
            return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        case Format::D32:
            return DXGI_FORMAT_D32_FLOAT;
        case Format::D32S8:
            return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case Format::X32G8_UINT:
            return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

        // BC compressed
        case Format::BC1_UNORM:
            return DXGI_FORMAT_BC1_UNORM;
        case Format::BC1_UNORM_SRGB:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
        case Format::BC2_UNORM:
            return DXGI_FORMAT_BC2_UNORM;
        case Format::BC2_UNORM_SRGB:
            return DXGI_FORMAT_BC2_UNORM_SRGB;
        case Format::BC3_UNORM:
            return DXGI_FORMAT_BC3_UNORM;
        case Format::BC3_UNORM_SRGB:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
        case Format::BC4_UNORM:
            return DXGI_FORMAT_BC4_UNORM;
        case Format::BC4_SNORM:
            return DXGI_FORMAT_BC4_SNORM;
        case Format::BC5_UNORM:
            return DXGI_FORMAT_BC5_UNORM;
        case Format::BC5_SNORM:
            return DXGI_FORMAT_BC5_SNORM;
        case Format::BC6H_UFLOAT:
            return DXGI_FORMAT_BC6H_UF16;
        case Format::BC6H_SFLOAT:
            return DXGI_FORMAT_BC6H_SF16;
        case Format::BC7_UNORM:
            return DXGI_FORMAT_BC7_UNORM;
        case Format::BC7_UNORM_SRGB:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

        default:
            return DXGI_FORMAT_UNKNOWN;
    }
}

constexpr D3D12_COMMAND_LIST_TYPE get( QueueType type ) {
    switch ( type )
    {
        case QueueType::Graphics:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case QueueType::Compute:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case QueueType::Transfer:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        default:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
}

constexpr D3D12_FENCE_FLAGS get( FenceType type ) {
    switch ( type )
    {
        case FenceType::Shared:
            return D3D12_FENCE_FLAG_SHARED;
        // case FenceType::SharedCrossAdapter:
        //     return D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER;
        // case FenceType::NonMonitored:
        //     return D3D12_FENCE_FLAG_NON_MONITORED;
        default:
            return D3D12_FENCE_FLAG_NONE;
    }
}

constexpr D3D12_SRV_DIMENSION getSRVDimension( TextureDimension dim ) {
    switch ( dim )
    {
        case TextureDimension::Texture1D:
            return D3D12_SRV_DIMENSION_TEXTURE1D;
        case TextureDimension::Texture2D:
            return D3D12_SRV_DIMENSION_TEXTURE2D;
        case TextureDimension::Texture3D:
            return D3D12_SRV_DIMENSION_TEXTURE3D;
    }
    return D3D12_SRV_DIMENSION_TEXTURE2D;
}

constexpr D3D12_RTV_DIMENSION getRTVDimension( TextureDimension dim ) {
    switch ( dim )
    {
        case TextureDimension::Texture1D:
            return D3D12_RTV_DIMENSION_TEXTURE1D;
        case TextureDimension::Texture2D:
            return D3D12_RTV_DIMENSION_TEXTURE2D;
        case TextureDimension::Texture3D:
            return D3D12_RTV_DIMENSION_TEXTURE3D;
    }
    return D3D12_RTV_DIMENSION_TEXTURE2D;
}

constexpr D3D12_DSV_DIMENSION getDSVDimension( TextureDimension dim ) {
    switch ( dim )
    {
        case TextureDimension::Texture1D:
            return D3D12_DSV_DIMENSION_TEXTURE1D;
        case TextureDimension::Texture2D:
            return D3D12_DSV_DIMENSION_TEXTURE2D;
        case TextureDimension::Texture3D:
            return D3D12_DSV_DIMENSION_UNKNOWN; // DSV cannot be 3D
    }
    return D3D12_DSV_DIMENSION_TEXTURE2D;
}

constexpr D3D12_UAV_DIMENSION getUAVDimension( TextureDimension dim ) {
    switch ( dim )
    {
        case TextureDimension::Texture1D:
            return D3D12_UAV_DIMENSION_TEXTURE1D;
        case TextureDimension::Texture2D:
            return D3D12_UAV_DIMENSION_TEXTURE2D;
        case TextureDimension::Texture3D:
            return D3D12_UAV_DIMENSION_TEXTURE3D;
    }
    return D3D12_UAV_DIMENSION_TEXTURE2D;
}

constexpr D3D12_RESOURCE_STATES get( ResourceState s ) {
    D3D12_RESOURCE_STATES dx = D3D12_RESOURCE_STATE_COMMON;

    if ( s == ResourceState::Undefined )
        return D3D12_RESOURCE_STATE_COMMON;

    // Common / read
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::GeneralRead ) )
        dx |= D3D12_RESOURCE_STATE_GENERIC_READ;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::Common ) )
        dx |= D3D12_RESOURCE_STATE_COMMON;

    // Buffers
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VertexBuffer ) )
        dx |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::ConstantBuffer ) )
        dx |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::IndexBuffer ) )
        dx |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::IndirectArgument ) )
        dx |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;

    // Shader resources
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::PixelShaderResource ) )
        dx |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::NonPixelShaderResource ) )
        dx |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::ShaderResource ) )
        dx |= ( D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE );

    // UAV
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::UnorderedAccess ) )
        dx |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

    // RT / Depth
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::RenderTarget ) )
        dx |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::DepthWrite ) )
        dx |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::DepthRead ) )
        dx |= D3D12_RESOURCE_STATE_DEPTH_READ;

    // Copy
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::CopySource ) )
        dx |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::CopyDest ) )
        dx |= D3D12_RESOURCE_STATE_COPY_DEST;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::ResolveSource ) )
        dx |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::ResolveDest ) )
        dx |= D3D12_RESOURCE_STATE_RESOLVE_DEST;

    // Present
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::Present ) )
        dx |= D3D12_RESOURCE_STATE_PRESENT;

    // Ray tracing
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::RaytracingAS ) )
        dx |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;

    // Shading rate
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::ShadingRateSource ) )
        dx |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;

    // Video
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoDecodeRead ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_DECODE_READ;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoDecodeWrite ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoProcessRead ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoProcessWrite ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoEncodeRead ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ;
    if ( static_cast<uint32_t>( s ) & static_cast<uint32_t>( ResourceState::VideoEncodeWrite ) )
        dx |= D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE;

    return dx;
}

} // namespace DX12Translator
} // namespace Graphics::RHI
AXION_NAMESPCE_END