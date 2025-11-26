#pragma once
#include "Axion/Graphics/Subsystems/PipelineRegistry.h"
#include <variant>

AXION_NAMESPACE_BEGIN

namespace Graphics {

class PipelineRegistry final : public IPipelineRegistry
{
public:
    explicit PipelineRegistry( RHI::IDevice* device, IShaderRegistry& shaderReg );
    ~PipelineRegistry() override;

    RHI::IGraphicPipeline*        getGraphicPipeline( PipelineHandle handle ) override;
    RHI::IComputePipeline*        getComputePipeline( PipelineHandle handle ) override;
    std::optional<PipelineHandle> findPipeline( const std::string& name ) const override;

    // Recargar pipelines (Hot-Reloading)
    void reloadAll() override;

    PipelineHandle createGraphic( const std::string& name, const RHI::GraphicPipelineDesc& desc ) override;
    PipelineHandle createCompute( const std::string& name, const RHI::ComputePipelineDesc& desc ) override;

private:
    RHI::IDevice*    _device = nullptr;
    IShaderRegistry& _shaderReg;


};

AXION_NAMESPACE_END
} // namespace Graphics