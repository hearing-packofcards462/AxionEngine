#pragma once

#include "Axion/Graphics/RHI/CommandList.h"

AXION_NAMESPCE_BEGIN

namespace Graphics::RHI {

DEFINE_HANDLE_FOR_TYPE( DX12CommandList, DX12CommandList )

class DX12CommandList : public ICommandList
{
public:
    ~DX12CommandList() override {};

    void                   begin() override;
    void                   end() override;
    void                   setCurrentFrame( uint index ) override;
    const CommandListDesc& getDescription() const override;

    void resourceBarrier( const TextureHandle& texture, ResourceState newState ) override;
    void clearTexture( const TextureHandle& texture, const ClearValue& clearValue ) override;

    NativeObject       getNativeObject( ObjectType objectType ) override;
    void               setDebugName( const std::string& name ) override;
    const std::string& getDebugName() const override;
    std::string        toString() const override;

    DX12CommandList( const ComPtr<ID3D12Device2>& device, const CommandListDesc& desc );

private:
    ComPtr<ID3D12GraphicsCommandList>           _cmdList;
    std::vector<ComPtr<ID3D12CommandAllocator>> _cmdAllocators;

    uint            _currentFrame = 0;
    CommandListDesc _desc;

    std::string _name;
};

} // namespace Graphics::RHI

AXION_NAMESPCE_END