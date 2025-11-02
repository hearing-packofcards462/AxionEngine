#pragma once
#include "Raiko/Graphics/RHI/Device.h"

RAIKO_NAMESPCE_BEGIN

namespace Graphics::RHI {

DEFINE_HANDLE_FOR_TYPE( IDX12Device, DX12Device )

class IDX12Device : public IDevice
{

public:
    enum class FeatureLevel
    {
        _1_0_Generic = 0x100,
        _1_0_Core    = 0x1000,
        _9_1         = 0x9100,
        _9_2         = 0x9200,
        _9_3         = 0x9300,
        _10_0        = 0xa000,
        _10_1        = 0xa100,
        _11_0        = 0xb000,
        _11_1        = 0xb100,
        _12_0        = 0xc000,
        _12_1        = 0xc100,
        _12_2        = 0xc200
    };

    struct Description {
        FeatureLevel featureLevel               = FeatureLevel::_12_1;
        bool         enableDebugLayer           = true;
        bool         useWarp                    = false;
        std::string  debugName                  = "DX12 Device";
        uint         renderTargetViewHeapSize   = 1024;
        uint         depthStencilViewHeapSize   = 1024;
        uint         shaderResourceViewHeapSize = 16384;
        uint         samplerHeapSize            = 1024;
        bool         enableHeapDirectlyIndexed  = false;
    };

protected:
    // DX12 Specific Methods
    virtual ComPtr<IDXGIAdapter4> getGPUAdapter()                                         = 0;
    virtual ComPtr<ID3D12Device2> createDevice( const ComPtr<IDXGIAdapter4>& gpuAdapter ) = 0;
    virtual void                  enableDebugLayer()                                      = 0;
};

typedef IDX12Device::Description  DX12DeviceDesc;
typedef IDX12Device::FeatureLevel DX12DeviceFeatureLevel;
DX12DeviceHandle                  createDX12Device( const DX12DeviceDesc& desc );

} // namespace Graphics::RHI

RAIKO_NAMESPCE_END