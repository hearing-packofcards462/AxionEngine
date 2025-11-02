#pragma once
#include "Axion/Graphics/RHI/Common.h"

namespace Graphics {
constexpr const char* gfxApiToString( API api ) {
    switch ( api )
    {
        case API::DirectX12:
            return "DirectX12";
        case API::Vulkan:
            return "Vulkan";
        case API::OpenGL:
            return "OpenGL";
        default:
            return "Unknown";
    }
}

constexpr const char* bufferingTypeToString( BufferingType type ) {
    switch ( type )
    {
        case BufferingType::Single:
            return "Single";
        case BufferingType::Double:
            return "Double";
        case BufferingType::Triple:
            return "Triple";
        default:
            return "Unknown";
    }
}

constexpr const char* presentModeToString( PresentMode mode ) {
    switch ( mode )
    {
        case PresentMode::Vsync:
            return "Vsync";
        case PresentMode::Immediate:
            return "Immediate";
        case PresentMode::Mailbox:
            return "Mailbox";
        default:
            return "Unknown";
    }
}
} // namespace Graphics
RAIKO_NAMESPCE_END
