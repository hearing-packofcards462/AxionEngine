#pragma once
#include "Axion/Graphics/RHI/Common.h"

AXION_NAMESPCE_BEGIN

namespace Graphics::RHI {

class ResourceStateTracker
{
public:
    explicit ResourceStateTracker( uint mipLevels = 1, uint arrayLayers = 1 );

    // --- Main API ---
    bool          needsTransition( ResourceState newState ) const;
    void          setState( ResourceState newState );
    void          reset(); // resets to Unknown/uninitialized
    ResourceState getCurrentState() const { return _globalState; }
    bool          isInitialized() const { return _initialized; }

    // --- Optional per-subresource (Textures) ---
    void          setState( ResourceState newState, uint mip, uint layer = 0 );
    ResourceState getState( uint mip, uint layer = 0 ) const;

private:
    ResourceState _globalState = ResourceState::Undefined;
    bool          _initialized = false;

    std::vector<ResourceState> _subresourceStates;
    uint                       _miplevels   = 1;
    uint                       _arrayLayers = 1;
};

} // namespace RHI
AXION_NAMESPCE_END
