#pragma once
#include "Raiko/Common/Defines.h"
#include "Raiko/Common/Events/Event.h"

RAIKO_NAMESPCE_BEGIN

namespace Event {

/**
 * @brief Base window event class
 */
struct WindowEvent : public Event {
    WindowEvent( void* handle )
        : handle( handle ) {}
    void* handle; ///< Native window handle
};

/**
 * @brief Emitted when the window is resized.
 */
struct WindowResizeEvent : public WindowEvent {
    WindowResizeEvent( void* h, uint w, uint hgt )
        : WindowEvent( h )
        , width( w )
        , height( hgt ) {}
    uint width;
    uint height;
};

/**
 * @brief Emitted when the window is requested to close.
 */
struct WindowCloseEvent : public WindowEvent {
    using WindowEvent::WindowEvent;
};

/**
 * @brief Emitted when the window gains or loses focus.
 */
struct WindowFocusEvent : public WindowEvent {
    WindowFocusEvent( void* h, bool focused )
        : WindowEvent( h )
        , focused( focused ) {}
    bool focused;
};

} // namespace Event

RAIKO_NAMESPCE_END
