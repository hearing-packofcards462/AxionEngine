#include "GPUResourcePool.hpp"

AXION_NAMESPACE_BEGIN
namespace Graphics {

GPUResourcePool::GPUResourcePool( RHI::IDevice* device )
    : _device( device ) {
    AXION_LOG_INFO( Logger::Module::GFX, "Renderer's Resource Pool Created Succesfully" );
}

GPUResourcePool::~GPUResourcePool() {
    clear();
    AXION_LOG_INFO( Logger::Module::GFX, "Destroying Renderer's Resource Pool" );
}

BufferHandle GPUResourcePool::registerBuffer( const RHI::BufferDesc& desc, const void* initialData, const std::string& name ) {
    std::scoped_lock lock( _mutex );

    auto buffer = _device->createBuffer( desc, initialData );

    // Find free slot
    uint id = UINT32_MAX;
    for ( uint i = 0; i < _buffers.size(); ++i )
    {
        if ( !_buffers[i].alive )
        {
            id          = i;
            _buffers[i] = { buffer, name, true };
            break;
        }
    }
    // Or append new one
    if ( id == UINT32_MAX )
    {
        id = (uint)_buffers.size();
        _buffers.push_back( { buffer, name, true } );
    }
    // Map name
    if ( !name.empty() )
        _nameToHandle[name] = { id };


    return BufferHandle { id };
}

RHI::BufferPtr& GPUResourcePool::getBuffer( BufferHandle handle ) {
    std::scoped_lock lock( _mutex );

    ResourceRecord<RHI::BufferPtr>& rec = _buffers[handle.id];
    return rec.ptr; // Return a dummy/empty BufferPtr or throw an exception
}

std::optional<BufferHandle> GPUResourcePool::findBuffer( const std::string& name ) const {
    std::scoped_lock lock( _mutex );

    auto it = _nameToHandle.find( name );
    if ( it == _nameToHandle.end() )
        return std::nullopt;
    return it->second;
}

void GPUResourcePool::destroyBuffer( BufferHandle handle ) {
    std::scoped_lock lock( _mutex );

    if ( !handle.isValid() || handle.id >= _buffers.size() )
        return;

    auto& rec = _buffers[handle.id];
    if ( rec.alive )
    {
        // rec.ptr   = nullptr;
        // delete rec.ptr;
        // rec.alive = false;

        if ( !rec.name.empty() )
            _nameToHandle.erase( rec.name );
    }
}

void GPUResourcePool::clear() {
    // std::scoped_lock lock( _mutex );

    // for ( auto& rec : _buffers )
    // {
    //     if ( rec.alive )
    //     {
    //         delete rec.ptr;
    //         rec.ptr   = nullptr;
    //         rec.alive = false;
    //     }
    // }

    // _nameToHandle.clear();
}

} // namespace Graphics
AXION_NAMESPACE_END
