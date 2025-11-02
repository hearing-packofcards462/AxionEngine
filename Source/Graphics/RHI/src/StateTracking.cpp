#include "StateTracking.h"
#include <algorithm>

RAIKO_NAMESPCE_BEGIN

namespace Graphics::RHI {

ResourceStateTracker::ResourceStateTracker( uint mipLevels, uint arrayLayers )
    : _miplevels( mipLevels )
    , _arrayLayers( arrayLayers ) {
}
bool ResourceStateTracker::needsTransition( ResourceState newState ) const {
    if ( !_initialized )
        return false;
    return _globalState != newState;
}

void ResourceStateTracker::setState( ResourceState newState ) {
    _globalState = newState;
    _initialized = true;
}

void ResourceStateTracker::reset() {
    _globalState = ResourceState::Undefined;
    _initialized = false;
    _subresourceStates.clear();
}

void ResourceStateTracker::setState( ResourceState newState, uint mip, uint layer ) {
    if ( _subresourceStates.empty() )
        _subresourceStates.resize( _miplevels * _arrayLayers, ResourceState::Undefined );

    uint32_t idx            = layer * _miplevels + mip;
    _subresourceStates[idx] = newState;
}

ResourceState ResourceStateTracker::getState( uint mip, uint layer ) const {
    if ( _subresourceStates.empty() )
        return _globalState;

    uint32_t idx = layer * _miplevels + mip;
    return _subresourceStates[idx];
}

} // namespace RHI
RAIKO_NAMESPCE_END