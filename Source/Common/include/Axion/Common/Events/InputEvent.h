#pragma once
#include "Axion/Common/Defines.h"
#include "Axion/Common/Events/Event.h"

AXION_NAMESPCE_BEGIN

namespace Event {

/**
 * @brief Base input event type.
 */
struct InputEvent : public Event {
    explicit InputEvent( void* h )
        : handle( h ) {}
    void* handle; ///< Native window handle (HWND on Win32)
};

/**
 * @brief Keyboard event base.
 */
struct KeyEvent : public InputEvent {
    KeyEvent( void* h, uint k, bool pressed )
        : InputEvent( h )
        , keyCode( k )
        , pressed( pressed ) {}
    uint keyCode;
    bool pressed;
};

/**
 * @brief Mouse button press/release event.
 */
struct MouseButtonEvent : public InputEvent {
    MouseButtonEvent( void* h, uint button, bool pressed )
        : InputEvent( h )
        , button( button )
        , pressed( pressed ) {}
    uint button;
    bool pressed;
};

/**
 * @brief Mouse move event.
 */
struct MouseMoveEvent : public InputEvent {
    MouseMoveEvent( void* h, int32_t x, int32_t y )
        : InputEvent( h )
        , x( x )
        , y( y ) {}
    int32_t x;
    int32_t y;
};

/**
 * @brief Mouse wheel scroll event.
 */
struct MouseScrollEvent : public InputEvent {
    MouseScrollEvent( void* h, float delta )
        : InputEvent( h )
        , delta( delta ) {}
    float delta;
};

} // namespace Event

AXION_NAMESPCE_END
