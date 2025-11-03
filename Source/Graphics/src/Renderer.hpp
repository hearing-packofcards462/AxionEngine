#pragma once
#include "Axion/Graphics/Renderer.h"
#include "GPUFrame.hpp"

AXION_NAMESPACE_BEGIN

namespace Graphics {

class Renderer : public IRenderer
{

public:
    virtual ~Renderer();
    virtual void render() override;
    virtual void render( const GPUSceneView& gpuScene ) override;
    virtual void destroy() override;
    virtual bool isHeadless() override;

    virtual const WindowHandle&      getWindow() override;
    virtual void                     setWindow( const WindowHandle& wnd ) override;
    virtual const Settings&          getSettings() const override;
    virtual const RHI::DeviceHandle& getDevice() const override;

    virtual std::string toString() const override;

    Renderer( const WindowHandle& wnd, const RendererSettings& settings );

private:
    void windowCallback( const Extent2D& newSize );

    RendererSettings _setts;

    RHI::DeviceHandle       _device      = nullptr;
    RHI::CommandListHandle  _commandList = nullptr;
    std::vector<RHI::Fence> _frameFences;

    // Window Related
    WindowHandle                                                                    _wnd            = nullptr;
    std::unique_ptr<Event::EventDispatcher<Event::WindowResizeEvent>::Subscription> _resizeCbHandle = nullptr;
    bool                                                                            _pendingResize  = false;
    RHI::SwapchainHandle                                                            _swapchain      = nullptr;

    std::vector<GPUFrame> _frames;
    uint                  _currentFrame = 0;
    const uint            _FRAMES_IN_FLIGHT;
};

} // namespace Graphics

AXION_NAMESPACE_END