#pragma once
#include "Axion/Graphics/RHI/Common.h"

AXION_NAMESPACE_BEGIN

namespace  Graphics::RHI {

class DX12DescriptorHeap
{
public:
    enum class Type
    {
        CBV_SRV_UAV,
        RTV,
        DSV
    };

    void init( const ComPtr<ID3D12Device>& device, Type type, uint numDescriptors );
    void reset();

    D3D12_CPU_DESCRIPTOR_HANDLE allocateCPU();
    D3D12_GPU_DESCRIPTOR_HANDLE getGPU( D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle ) const;

    ID3D12DescriptorHeap* getHeap() const { return _heap.Get(); }
    Type                  getType() const { return _type; }

private:
    ComPtr<ID3D12DescriptorHeap> _heap           = nullptr;
    uint                         _descriptorSize = 0;
    uint                         _allocated      = 0;
    uint                         _capacity       = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE  _baseCPU {};
    D3D12_GPU_DESCRIPTOR_HANDLE  _baseGPU {};
    Type                         _type;
};
} // namespace RHI

AXION_NAMESPACE_END