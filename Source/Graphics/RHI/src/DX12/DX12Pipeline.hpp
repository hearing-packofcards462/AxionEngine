#pragma once
#include "Axion/Graphics/RHI/Pipeline.h"
#include "DX12Device.hpp"

AXION_NAMESPACE_BEGIN

namespace Graphics::RHI {

DEFINE_COM_HANDLE_FOR_TYPE( DX12PipelineLayout, DX12PipelineLayout )

class DX12PipelineLayout final : public RefCounter<IPipelineLayout>
{
public:
    DX12PipelineLayout( const ComPtr<ID3D12Device2>& device, const PipelineLayoutDesc& desc );
    ~DX12PipelineLayout() override;

    const Description& getDescription() const override { return _desc; }
    void               setDebugName( const std::string& name ) override;
    const std::string& getDebugName() const override { return _desc.debugName; }
    NativeObject       getNativeObject( ObjectType objectType ) override;
    std::string        toString() const override;

private:
    void buildRootSignature( const ComPtr<ID3D12Device2>& device );

    PipelineLayoutDesc          _desc;
    ComPtr<ID3D12RootSignature> _rootSignature;
};

DEFINE_COM_HANDLE_FOR_TYPE( DX12GraphicPipeline, DX12GraphicPipeline )

class DX12GraphicPipeline : public RefCounter<IGraphicPipeline>
{
public:
    DX12GraphicPipeline( const ComPtr<ID3D12Device2>& device, const Description& desc );
    ~DX12GraphicPipeline() override;

    const Description& getDescription() const override { return _desc; }
    void               setDebugName( const std::string& name ) override;
    const std::string& getDebugName() const override { return _desc.debugName; }
    NativeObject       getNativeObject( ObjectType objectType ) override;
    std::string        toString() const override;

    ID3D12PipelineState* getPipelineState() const { return _pso.Get(); }

private:
    void                           createPipelineState( const ComPtr<ID3D12Device2>& device );
    static D3D12_INPUT_LAYOUT_DESC makeInputLayout( const IGraphicPipeline::Description& desc, std::vector<D3D12_INPUT_ELEMENT_DESC>& out );

    Description _desc;
    ComPtr<ID3D12PipelineState> _pso;
};

} // namespace Graphics::RHI

AXION_NAMESPACE_END