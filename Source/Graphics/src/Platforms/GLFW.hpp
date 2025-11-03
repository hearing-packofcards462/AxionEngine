#pragma once
#include "Axion/Graphics/Platforms/GLFW.h"


AXION_NAMESPACE_BEGIN

namespace Graphics {

class GLFWWindow final : public IGLFWWindow
{
public:
    GLFWWindow( const Settings& settings = {} );
    ~GLFWWindow() override;
    GLFWWindow( const GLFWWindow& )            = delete;
    GLFWWindow& operator=( const GLFWWindow& ) = delete;

    bool processMessages() override;

    void setFullscreen( bool fullscreen ) override;
    bool minimized() const override;

    const Settings& getSettings() const override { return _setts; }
    void            setSettings( const Settings& settings ) override { _setts = settings; }

    bool              shouldClose() const override { return _shouldClose; }
    PlatformType      getPlatformType() const override { return PlatformType::GLFW; }
    RHI::NativeObject getNativeObject() override;

    Event::EventDispatcher<Event::WindowResizeEvent>& onResize() override { return _onResize; }
    Event::EventDispatcher<Event::WindowCloseEvent>&  onClose() override { return _onClose; }
    Event::EventDispatcher<Event::KeyEvent>&          onKey() override { return _onKey; }
    Event::EventDispatcher<Event::MouseButtonEvent>&  onMouseButton() override { return _onMouseButton; }
    Event::EventDispatcher<Event::MouseMoveEvent>&    onMouseMove() override { return _onMouseMove; }
    Event::EventDispatcher<Event::MouseScrollEvent>&  onMouseScroll() override { return _onMouseScroll; }

private:
    void setCallbacksFunctions();

    GLFWwindow* _hWnd = nullptr; // Native type handle

    Settings _setts       = {};
    bool     _initialized = false;
    bool     _shouldClose = false;
    bool     _minimized   = false;

    Extent2D _windowedSizeCache;

    // Event Managing
    Event::EventDispatcher<Event::WindowResizeEvent> _onResize;
    Event::EventDispatcher<Event::WindowCloseEvent>  _onClose;
    Event::EventDispatcher<Event::KeyEvent>          _onKey;
    Event::EventDispatcher<Event::MouseButtonEvent>  _onMouseButton;
    Event::EventDispatcher<Event::MouseMoveEvent>    _onMouseMove;
    Event::EventDispatcher<Event::MouseScrollEvent>  _onMouseScroll;
    // Event::EventDispatcher<Event::WindowFocusEvent>  _onFocus;
};

} // namespace Graphics
AXION_NAMESPACE_END
