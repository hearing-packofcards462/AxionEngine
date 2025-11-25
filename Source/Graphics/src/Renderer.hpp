#pragma once
#include "Axion/Graphics/Renderer.h"
#include "GPUFrame.hpp"
#include "GPUResourcePool.hpp"

AXION_NAMESPACE_BEGIN

namespace Graphics {

class Renderer : public IRenderer
{

public:
    virtual ~Renderer();
    virtual void render() override;
    // virtual void render( const GPUSceneView& gpuScene ) override;
    virtual void destroy() override;
    virtual bool isHeadless() override;

    virtual const WindowPtr& getWindow() override;
    virtual void             setWindow( const WindowPtr& wnd ) override;
    virtual const Settings&  getSettings() const override;

    virtual IGPUResourcePool& getResourcePool() override;

    virtual const RHI::DevicePtr& getDevice() const override;

    virtual std::string toString() const override;

    Renderer( const WindowPtr& wnd, const RendererSettings& settings );

private:
    void windowCallback( const Extent2D& newSize );

    RendererSettings _setts;
    // RHI -- GPU
    RHI::DevicePtr          _device      = nullptr;
    RHI::CommandListPtr     _commandList = nullptr;
    std::vector<RHI::Fence> _frameFences;
    // GPU Resources
    GPUResourcePoolPtr    _resourcePool   = nullptr;
    std::vector<GPUFrame> _frames;
    //Pipelines & shaders
    // ShaderRegistryPtr     _shaderRegistry = nullptr;
    // PipelineRegistryPtr   _pipRegistry    = nullptr;
    // Window Related
    WindowPtr                                                                       _wnd            = nullptr;
    std::unique_ptr<Event::EventDispatcher<Event::WindowResizeEvent>::Subscription> _resizeCbHandle = nullptr;
    bool                                                                            _pendingResize  = false;
    RHI::SwapchainPtr                                                               _swapchain      = nullptr;
    // Query
    uint       _currentFrame = 0;
    const uint _FRAMES_IN_FLIGHT;
};

} // namespace Graphics

AXION_NAMESPACE_END