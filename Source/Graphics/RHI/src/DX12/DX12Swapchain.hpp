#pragma once

#include "DX12Device.hpp"
#include "DX12Resource.hpp"
#include "DX12TranslatorUnit.h"
#include "Axion/Graphics/RHI/Common.h"
#include "Axion/Graphics/RHI/Swapchain.h"

AXION_NAMESPACE_BEGIN

namespace Graphics::RHI {

DEFINE_COM_HANDLE_FOR_TYPE( DX12Swapchain, DX12Swapchain )

class DX12Swapchain final : public RefCounter<ISwapchain>
{
public:
    ~DX12Swapchain() override;

    void                              updateImages() override;
    void                              present() override;
    uint                              acquireNextImage() override;
    uint                              getCurrentImageIndex() override;
    const Description&                getDescription() override;
    const std::vector<TextureHandle>& getSwapImages() override;
    void                              update( const Description& newDesc ) override;

    NativeObject       getNativeObject( ObjectType objectType ) override;
    void               setDebugName( const std::string& name ) override;
    const std::string& getDebugName() const override;
    std::string        toString() const override;

    DX12Swapchain( const HWND hwnd, DX12Device::Context& ctx, const ISwapchain::Description& desc );

private:
    bool checkTearingSupport();

    ISwapchain::Description _desc;

    ComPtr<ID3D12Device2>   _device;
    ComPtr<IDXGISwapChain4> _swapchain;
    uint                    _currentImage;

    DX12DescriptorHeap         _heapRTV;
    std::vector<TextureHandle> _swapImages;

    std::string _name;

    bool _initialized = false;
};

} // namespace Graphics::RHI

AXION_NAMESPACE_END