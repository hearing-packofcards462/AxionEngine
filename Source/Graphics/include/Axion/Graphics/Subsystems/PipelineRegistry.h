#pragma once
#include "Axion/Graphics/RHI/Pipeline.h"
#include "Axion/Graphics/Subsystems/ShaderRegistry.h"
#include <variant>

AXION_NAMESPACE_BEGIN

namespace Graphics {

class IPipelineRegistry
{
public:
    virtual ~IPipelineRegistry() = default;

    IPipelineRegistry( const IPipelineRegistry& )            = delete;
    IPipelineRegistry& operator=( const IPipelineRegistry& ) = delete;
    IPipelineRegistry( IPipelineRegistry&& )                 = delete;
    IPipelineRegistry& operator=( IPipelineRegistry&& )      = delete;

    class GraphicBuilder;
    class ComputeBuilder;

    GraphicBuilder graphic( const std::string& name ) { return GraphicBuilder( *this, name ); }
    ComputeBuilder compute( const std::string& name ) { return ComputeBuilder( *this, name ); }

    virtual const RHI::GraphicPipelinePtr& getGraphicPipeline( PipelineHandle handle )   = 0;
    virtual const RHI::ComputePipelinePtr& getComputePipeline( PipelineHandle handle )   = 0;
    virtual std::optional<PipelineHandle>  findPipeline( const std::string& name ) const = 0;

    // Recargar pipelines (Hot-Reloading)
    virtual void reloadAll() = 0;

protected:
    IPipelineRegistry() = default;

    virtual PipelineHandle createGraphic( const std::string& name, const RHI::GraphicPipelineDesc& desc ) = 0;
    virtual PipelineHandle createCompute( const std::string& name, const RHI::ComputePipelineDesc& desc ) = 0;

    friend class GraphicBuilder;
    friend class ComputeBuilder;
};

class IPipelineRegistry::GraphicBuilder
{
public:
    GraphicBuilder( IPipelineRegistry& reg, std::string name )
        : _registry( reg )
        , _name( std::move( name ) ) {
        _desc.rasterizerState   = { RHI::FillMode::Solid,
                                    RHI::CullMode::Back,
                                    /*...*/ };
        _desc.depthStencilState = { true, true, RHI::CompareOp::Less };
        _desc.topology          = RHI::PrimitiveTopology::TriangleList;
    }

    GraphicBuilder& vs( const std::string& shaderName ) {
        _vsName = shaderName;
        return *this;
    }
    GraphicBuilder& ps( const std::string& shaderName ) {
        _psName = shaderName;
        return *this;
    }
    GraphicBuilder& gs( const std::string& shaderName ) {
        _gsName = shaderName;
        return *this;
    }
    GraphicBuilder& hs( const std::string& shaderName ) {
        _hsName = shaderName;
        return *this;
    }
    GraphicBuilder& ds( const std::string& shaderName ) {
        _dsName = shaderName;
        return *this;
    }

    GraphicBuilder& addRenderTarget( Format fmt, RHI::BlendAttachment blend = {} ) {
        _desc.renderTargetFormats.push_back( fmt );
        _desc.blendState.attachments.push_back( blend );
        return *this;
    }
    GraphicBuilder& setDepthFormat( Format fmt ) {
        _desc.depthStencilFormat = fmt;
        return *this;
    }
    GraphicBuilder& wireframe() {
        _desc.rasterizerState.fillMode = RHI::FillMode::Wireframe;
        return *this;
    }
    GraphicBuilder& cullNone() {
        _desc.rasterizerState.cullMode = RHI::CullMode::None;
        return *this;
    }
    GraphicBuilder& disableDepth() {
        _desc.depthStencilState.depthEnable = false;
        return *this;
    }
    GraphicBuilder& setRasterizer( const RHI::RasterizerState& state ) {
        _desc.rasterizerState = state;
        return *this;
    }

    // Si no se llama, el Registry intentará crearlo via Reflexión de Slang
    GraphicBuilder& setLayout( RHI::IPipelineLayout* layout ) {
        _desc.layout = layout;
        return *this;
    }

    PipelineHandle create() {
        return _registry.createGraphic( _name, _desc );
    }

private:
    IPipelineRegistry&       _registry;
    std::string              _name;
    RHI::GraphicPipelineDesc _desc;

    std::string _vsName, _psName, _gsName, _hsName, _dsName;
};

class IPipelineRegistry::ComputeBuilder
{
public:
    ComputeBuilder( IPipelineRegistry& reg, std::string name )
        : _registry( reg )
        , _name( std::move( name ) ) {}

    /// @brief Define el shader de cómputo a utilizar (por nombre en ShaderRegistry).
    ComputeBuilder& shader( const std::string& shaderName ) {
        _shaderName = shaderName;
        return *this;
    }

    /// @brief Define manual del Layout. Si no se llama, se autogenera por reflexión.
    ComputeBuilder& setLayout( RHI::IPipelineLayout* layout ) {
        _desc.layout = layout;
        return *this;
    }

    /// @brief Construye el pipeline.
    PipelineHandle create() {
        return _registry.createCompute( _name, _desc, _shaderName );
    }

private:
    IPipelineRegistry&       _registry;
    std::string              _name;
    RHI::ComputePipelineDesc _desc;

    std::string _shaderName;
};

} // namespace Graphics
AXION_NAMESPACE_END