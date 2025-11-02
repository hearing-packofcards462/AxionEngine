#pragma once
#include "DX12Debug.hpp"
#include "DX12DescriptorHeap.h"

RAIKO_NAMESPCE_BEGIN

namespace  Graphics::RHI {
void DX12DescriptorHeap::init( const ComPtr<ID3D12Device>& device, Type type, uint numDescriptors ) {

    _capacity = numDescriptors;
    _type     = type;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors             = numDescriptors;

    switch ( type )
    {
        case Type::CBV_SRV_UAV:
            desc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            break;
        case Type::RTV:
            desc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            break;
        case Type::DSV:
            desc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            break;
    }

    DX_CHECK( device->CreateDescriptorHeap( &desc, IID_PPV_ARGS( &_heap ) ) );
    _descriptorSize = device->GetDescriptorHandleIncrementSize( desc.Type );
    _baseCPU        = _heap->GetCPUDescriptorHandleForHeapStart();
    if ( type == Type::CBV_SRV_UAV )
    {
        _baseGPU = _heap->GetGPUDescriptorHandleForHeapStart();
    }
}

void DX12DescriptorHeap::reset() {
    _allocated = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::allocateCPU() {
    RAIKO_LOG_ASSERT( _allocated < _capacity, Logger::Module::RHI, "FATAL | Out of descriptor heap space" );

    D3D12_CPU_DESCRIPTOR_HANDLE handle = {
        _baseCPU.ptr + _allocated * _descriptorSize };
    ++_allocated;
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::getGPU( D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle ) const {
    RAIKO_LOG_ASSERT( _type == Type::CBV_SRV_UAV, Logger::Module::RHI, "FATAL | This can only be used with CBV_SRV_UAV Type Heaps" );
    uint                        offset = static_cast<uint>( ( cpuHandle.ptr - _baseCPU.ptr ) / _descriptorSize );
    D3D12_GPU_DESCRIPTOR_HANDLE gpu    = { _baseGPU.ptr + offset * _descriptorSize };
    return gpu;
}

} // namespace RHI

RAIKO_NAMESPCE_END