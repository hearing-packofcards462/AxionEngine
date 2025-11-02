#pragma once
#include "Axion/Graphics/Platforms/Window.h"
#include <assert.h>

AXION_NAMESPCE_BEGIN

namespace Graphics {

/**
 * @brief Handle definition for Win32-specific window implementation.
 */
DEFINE_HANDLE_FOR_TYPE( IWin32Window, Win32Window )

/**
 * @brief Interface for a Win32 window implementation.
 *
 */
class IWin32Window : public IWindow
{
protected:
};

/**
 * @brief Factory function to create a Win32-compatible window.
 *
 * This function instantiates a concrete @ref Win32Window based on
 * the provided application instance handle and optional settings.
 *
 * @param hInstance The Win32 application instance handle.
 * @param settings  Window configuration (name, size, fullscreen, etc.).
 *
 * @return A handle to the created window object.
 *
 * @see IWindow
 * @see Win32Window
 */
WindowHandle createWindowForWin32( HINSTANCE hInstance, const WindowSettings& settings = {} );

} // namespace Renderer

AXION_NAMESPCE_END
