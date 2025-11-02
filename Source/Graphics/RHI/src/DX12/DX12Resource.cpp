#pragma once
#include "DX12Resource.hpp"
#include "DX12Debug.hpp"
#include "DX12TranslatorUnit.h"

RAIKO_NAMESPCE_BEGIN

namespace Graphics::RHI {

#pragma region Texture
DX12Texture::DX12Texture( const TextureDesc& desc, DX12Device::Context& ctx, const void* initialData )
    : _desc( desc )
    , _stateTracker( desc.mipLevels, desc.arraySize ) {

    D3D12_RESOURCE_DESC dx12Desc = {};
    dx12Desc.Dimension           = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    dx12Desc.Width               = desc.size.width;
    dx12Desc.Height              = desc.size.height;
    dx12Desc.MipLevels           = static_cast<UINT16>( desc.mipLevels );
    dx12Desc.DepthOrArraySize    = ( desc.dimension == TextureDimension::Texture3D ) ? static_cast<UINT16>( desc.size.depth ) : static_cast<UINT16>( desc.arraySize );
    dx12Desc.Format              = DX12Translator::get( desc.format );
    dx12Desc.SampleDesc.Count    = desc.sampleCount;
    dx12Desc.Flags               = D3D12_RESOURCE_FLAG_NONE;

    ResourceState initialState = ResourceState::Common;
    if ( initialData )
        initialState = ResourceState::CopyDest;
    else
    {
        if ( desc.viewFlags & TextureViewRenderTarget )
        {
            dx12Desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            initialState = ResourceState::RenderTarget;
        }
        if ( desc.viewFlags & TextureViewDepthStencil )
        {
            dx12Desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            initialState = ResourceState::DepthWrite;
        }
        if ( desc.viewFlags & TextureViewUnorderedAccess )
        {
            dx12Desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            initialState = ResourceState::UnorderedAccess;
        }
        if ( _desc.viewFlags & TextureViewShaderResource )
            initialState = ResourceState::GeneralRead;
    }

    switch ( desc.dimension )
    {
        case TextureDimension::Texture1D:
            dx12Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
            break;
        case TextureDimension::Texture2D:
            dx12Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            break;
        case TextureDimension::Texture3D:
            dx12Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
            break;
    }

    CD3DX12_HEAP_PROPERTIES heapProps( D3D12_HEAP_TYPE_DEFAULT );

    DX_CHECK( ctx.device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &dx12Desc,
        DX12Translator::get( initialState ),
        nullptr,
        IID_PPV_ARGS( &_resource ) ) );

    _stateTracker.setState( initialState );

    createViews( ctx.device, ctx.resources, true );

    setDebugName( _desc.debugName );
}

DX12Texture::DX12Texture( const ComPtr<ID3D12Device2>& device, const ComPtr<ID3D12Resource>& resource, const TextureDesc& desc, bool useDecriptionParams, DX12Device::Resources& resources )
    : _desc( desc )
    , _resource( resource )
    , _stateTracker( desc.mipLevels, desc.arraySize ) {
    createViews( device, resources, useDecriptionParams );
    setDebugName( _desc.debugName );
}

DX12Texture::~DX12Texture() {
}

const TextureDesc& DX12Texture::getDescription() const {
    return _desc;
}

void DX12Texture::createViews( const ComPtr<ID3D12Device2>& device, DX12Device::Resources& resources, bool useDescriptionParams ) {
    if ( _desc.viewFlags & TextureViewShaderResource )
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format                          = DX12Translator::get( _desc.format );
        srvDesc.ViewDimension                   = DX12Translator::getSRVDimension( _desc.dimension ); // helper
        srvDesc.Texture2D.MipLevels             = _desc.mipLevels;

        _srvHandle = resources.heapSRV.allocateCPU();
        device->CreateShaderResourceView( _resource.Get(), useDescriptionParams ? &srvDesc : nullptr, _srvHandle );
    }

    if ( _desc.viewFlags & TextureViewRenderTarget )
    {
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format                        = DX12Translator::get( _desc.format );
        rtvDesc.ViewDimension                 = DX12Translator::getRTVDimension( _desc.dimension );
        _rtvHandle                            = resources.heapRTV.allocateCPU();
        device->CreateRenderTargetView( _resource.Get(), useDescriptionParams ? &rtvDesc : nullptr, _rtvHandle );
    }

    if ( _desc.viewFlags & TextureViewDepthStencil )
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format                        = DX12Translator::get( _desc.format );
        dsvDesc.ViewDimension                 = DX12Translator::getDSVDimension( _desc.dimension );
        _dsvHandle                            = resources.heapDSV.allocateCPU();
        device->CreateDepthStencilView( _resource.Get(), useDescriptionParams ? &dsvDesc : nullptr, _dsvHandle );
    }
}

void DX12Texture::uploadInitialData( DX12Device::Context& ctx, const void* initialData ) {
}

void DX12Texture::setDebugName( const std::string& name ) {
    _desc.debugName = name;
    _resource->SetName( std::wstring( name.begin(), name.end() ).c_str() );
}

const std::string& DX12Texture::getDebugName() const {
    return _desc.debugName;
}

NativeObject DX12Texture::getNativeObject( ObjectType objectType ) {
    switch ( objectType )
    {
        case ObjectTypes::DX12_Resource:
            return NativeObject( objectType, _resource.Get() );
        default:
            RAIKO_LOG_ERROR( Logger::Module::RHI, "DX12 Texture | Wrong Object Type" );
            return nullptr;
    }
}

ResourceStateTracker& DX12Texture::stateTracker() {
    return _stateTracker;
}

std::string DX12Texture::toString() const {
    return std::string();
}
#pragma endregion
#pragma region Buffer

DX12Buffer::DX12Buffer( const BufferDesc&    desc,
                        DX12Device::Context& ctx,
                        const void*          initialData )
    : _desc( desc ) {

    // Choose proper heap
    D3D12_HEAP_PROPERTIES heapProps {};
    switch ( desc.memory )
    {
        case MemoryUsage::CPUVisible:
            heapProps = CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD );
            break;
        case MemoryUsage::Readback:
            heapProps = CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_READBACK );
            break;
        default:
            heapProps = CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT );
            break;
    }

    // Resource flags (for UAV)
    D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
    if ( ( desc.viewFlags & BufferViewUnorderedAccess ) != BufferViewNone )
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer( desc.size, flags );

    // --- Infer initial state ---
    ResourceState initialState = ResourceState::Common;
    if ( _desc.memory == MemoryUsage::CPUVisible )
        initialState = ResourceState::GeneralRead;
    else if ( ( _desc.viewFlags & BufferViewConstantBuffer ) != BufferViewNone )
        initialState = ResourceState::ConstantBuffer;
    else if ( ( _desc.viewFlags & BufferViewShaderResource ) != BufferViewNone )
        initialState = ResourceState::ShaderResource;

    D3D12_RESOURCE_STATES dxInitState = D3D12_RESOURCE_STATE_COMMON;
    dxInitState                       = DX12Translator::get( initialState );

    DX_CHECK( ctx.device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        dxInitState,
        nullptr,
        IID_PPV_ARGS( &_resource ) ) );

    setDebugName( desc.debugName );

    _stateTracker.setState( initialState );

    // if ( initialData )
    //     uploadInitialData( initialData );

    createViews( ctx );
}

void* DX12Buffer::map() {
    RAIKO_LOG_ASSERT(
        _desc.memory == MemoryUsage::CPUVisible || _desc.memory == MemoryUsage::Readback,
        Logger::Module::RHI,
        "Map called on non-CPU buffer" );

    void*         ptr = nullptr;
    CD3DX12_RANGE range( 0, 0 );
    _resource->Map( 0, &range, &ptr );
    return ptr;
}

void DX12Buffer::unmap() {
    _resource->Unmap( 0, nullptr );
}

void DX12Buffer::setDebugName( const std::string& name ) {
    _desc.debugName = name;
    _resource->SetName( std::wstring( name.begin(), name.end() ).c_str() );
}

NativeObject DX12Buffer::getNativeObject( ObjectType objectType ) {
    switch ( objectType )
    {
        case ObjectTypes::DX12_Resource:
            return NativeObject( objectType, _resource.Get() );
        default:
            RAIKO_LOG_ERROR( Logger::Module::RHI, "DX12 Buffer | Wrong Object Type" );
            return nullptr;
    }
}

std::string DX12Buffer::toString() const {
    return fmt::format( "" );
}
void DX12Buffer::createViews( DX12Device::Context& ctx ) {
    // Constant Buffer
    if ( ( _desc.viewFlags & BufferViewConstantBuffer ) != BufferViewNone )
    {

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbv {};
        cbv.BufferLocation = _resource->GetGPUVirtualAddress();
        cbv.SizeInBytes    = (UINT)Math::AlignUp( _desc.size, (size_t)256 );

        _cbvHandle = ctx.resources.heapSRV.allocateCPU();
        ctx.device->CreateConstantBufferView( &cbv, _srvHandle );

        _stateTracker.setState( ResourceState::ConstantBuffer );
    }

    // SRV
    if ( ( _desc.viewFlags & BufferViewShaderResource ) != BufferViewNone )
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC desc {};
        desc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;
        desc.Format                     = DXGI_FORMAT_UNKNOWN;
        desc.Buffer.NumElements         = (UINT)( _desc.size / _desc.stride );
        desc.Buffer.StructureByteStride = _desc.stride;
        desc.Shader4ComponentMapping    = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

        _srvHandle = ctx.resources.heapSRV.allocateCPU();
        ctx.device->CreateShaderResourceView( _resource.Get(), &desc, _srvHandle );

        _stateTracker.setState( ResourceState::ShaderResource );
    }

    // UAV
    if ( ( _desc.viewFlags & BufferViewUnorderedAccess ) != BufferViewNone )
    {

        D3D12_UNORDERED_ACCESS_VIEW_DESC desc {};
        desc.ViewDimension              = D3D12_UAV_DIMENSION_BUFFER;
        desc.Format                     = DXGI_FORMAT_UNKNOWN;
        desc.Buffer.NumElements         = (UINT)( _desc.size / _desc.stride );
        desc.Buffer.StructureByteStride = _desc.stride;

        _uavHandle = ctx.resources.heapSRV.allocateCPU();
        ctx.device->CreateUnorderedAccessView( _resource.Get(), nullptr, &desc, _srvHandle );

        _stateTracker.setState( ResourceState::UnorderedAccess );
    }
}

void DX12Buffer::uploadInitialData( DX12Device::Context& ctx, const void* initialData ) {
    // if ( !initialData )
    //     return;

    // // CPU-visible: just map and copy
    // if ( _desc.memory == MemoryUsage::CPUVisible )
    // {
    //     void* dst = map();
    //     std::memcpy( dst, initialData, _desc.size );
    //     unmap();

    //     _stateTracker.setState( ResourceState::ConstantBuffer ); // Or appropriate state for CPU-visible
    //     return;
    // }

    // // GPU-only: create a temporary upload buffer
    // ComPtr<ID3D12Resource> uploadBuffer;
    // {
    //     CD3DX12_HEAP_PROPERTIES heapProps( D3D12_HEAP_TYPE_UPLOAD );
    //     auto                    bufDesc = CD3DX12_RESOURCE_DESC::Buffer( _desc.size );
    //     DX_CHECK( ctx.device->CreateCommittedResource(
    //         &heapProps,
    //         D3D12_HEAP_FLAG_NONE,
    //         &bufDesc,
    //         D3D12_RESOURCE_STATE_GENERIC_READ,
    //         nullptr,
    //         IID_PPV_ARGS( &uploadBuffer ) ) );
    // }

    // // Map + copy data
    // {
    //     void*         mapped = nullptr;
    //     CD3DX12_RANGE readRange( 0, 0 );
    //     uploadBuffer->Map( 0, &readRange, &mapped );
    //     std::memcpy( mapped, initialData, _desc.size );
    //     uploadBuffer->Unmap( 0, nullptr );
    // }

    // auto cmdList = ctx.resources.uploadContext.commandList->get(); // ID3D12GraphicsCommandList*

    // // Reset the command list
    // ctx.resources.uploadContext.commandList->reset();

    // // Copy from upload buffer to GPU buffer
    // cmdList->CopyBufferRegion( _resource.Get(), 0, uploadBuffer.Get(), 0, _desc.size );

    // // Transition GPU buffer to its intended usage
    // D3D12_RESOURCE_STATES targetState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    // if ( ( _desc.viewFlags & BufferViewUnorderedAccess ) != BufferViewNone )
    //     targetState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

    // CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
    //     _resource.Get(),
    //     D3D12_RESOURCE_STATE_COMMON, // initial
    //     targetState                  // final
    // );
    // cmdList->ResourceBarrier( 1, &barrier );

    // DX_CHECK( cmdList->Close() );

    // // Execute the upload
    // ID3D12CommandList* lists[] = { cmdList };
    // auto               queue   = ctx.primaryQueue->queue.Get();
    // queue->ExecuteCommandLists( 1, lists );

    // // Signal & wait
    // ctx.resources.uploadContext.fenceValue++;
    // DX_CHECK( queue->Signal( ctx.resources.uploadContext.fence.Get(), ctx.resources.uploadContext.fenceValue ) );

    // if ( ctx.resources.uploadContext.fence->GetCompletedValue() < ctx.resources.uploadContext.fenceValue )
    // {
    //     DX_CHECK( ctx.resources.uploadContext.fence->SetEventOnCompletion(
    //         ctx.resources.uploadContext.fenceValue,
    //         ctx.resources.uploadContext.fenceEvent ) );
    //     WaitForSingleObject( ctx.resources.uploadContext.fenceEvent, INFINITE );
    // }

    // // Update state tracker
    // ResourceState finalState = ResourceState::VertexBuffer;
    // if ( ( _desc.viewFlags & BufferViewConstantBuffer ) != BufferViewNone )
    //     finalState = ResourceState::ConstantBuffer;
    // else if ( ( _desc.viewFlags & BufferViewUnorderedAccess ) != BufferViewNone )
    //     finalState = ResourceState::UnorderedAccess;

    // _stateTracker.setState( finalState );
}

DX12Buffer::~DX12Buffer() {
}
#pragma endregion

} // namespace Graphics::RHI

RAIKO_NAMESPCE_END