#pragma once
#include "Raiko/Graphics/GPUSceneView.h"
#include "Raiko/Graphics/Platforms/Window.h"
#include "Raiko/Graphics/RHI/Device.h"

RAIKO_NAMESPCE_BEGIN

namespace Graphics {

DEFINE_HANDLE_FOR_TYPE( IRenderer, Renderer )

class IRenderer
{
public:
    struct Settings {
        API           gfxApi        = API::DirectX12;
        BufferingType bufferingType = BufferingType::Double;
        bool          debugMode     = true;
        PresentMode   presentMode   = PresentMode::Vsync;  // Only if windowed
        Format        outputFormat  = Format::RGBA8_UNORM; // Only if windowed
    };

    virtual ~IRenderer()                                = default;
    virtual void render()                               = 0;
    virtual void render( const GPUSceneView& gpuScene ) = 0;
    virtual void destroy()                              = 0;
    virtual bool isHeadless()                           = 0;

    virtual const WindowHandle&      getWindow()                          = 0;
    virtual void                     setWindow( const WindowHandle& wnd ) = 0;
    virtual const Settings&          getSettings() const                  = 0;
    virtual const RHI::DeviceHandle& getDevice() const                    = 0;

    virtual std::string toString() const = 0;
};

typedef IRenderer::Settings RendererSettings;

RendererHandle createRenderer( const WindowHandle& wnd, const RendererSettings& settings = {} );
RendererHandle createHeadlessRenderer( const RendererSettings& settings = {} );

} // namespace Graphics

RAIKO_NAMESPCE_END
