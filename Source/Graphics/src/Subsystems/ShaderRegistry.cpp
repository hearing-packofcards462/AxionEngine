#include "ShaderRegistry.hpp"

AXION_NAMESPACE_BEGIN

namespace Graphics {

static const std::vector<uchar> INVALID_BYTECODE = {};

ShaderRegistry::ShaderRegistry() {
    AXION_LOG_INFO( Logger::Module::GFX, "Renderer's Shader Registry Created Succesfully" );
    _compiler.begin();
}

ShaderRegistry::~ShaderRegistry() {
    AXION_LOG_INFO( Logger::Module::GFX, "Destroying Renderer's Shader Registry" );
    _compiler.end();
}

ShaderHandle ShaderRegistry::registerShader( const ShaderDesc& desc, const std::string& name ) {
    // std::scoped_lock lock( _mutex );

    uint id = UINT32_MAX;
    for ( uint i = 0; i < _shaders.size(); ++i )
    {
        if ( !_shaders[i].alive )
        {
            id          = i;
            _shaders[i] = { {}, desc, /*std::move( layout ),*/ name, ShaderState::Uncompiled, true };
            break;
        }
    }
    if ( id == UINT32_MAX )
    {
        id = (uint)_shaders.size();
        _shaders.push_back( { {}, desc, /*std::move( layout ),*/ name, ShaderState::Uncompiled, true } );
    }
    // Map name
    if ( !name.empty() )
        _nameToHandle[name] = { id };

    AXION_LOG_INFO( Logger::Module::GFX, "Registered Shader: {} with path: {}", name, desc.path );
    return ShaderHandle { id };
}

const std::vector<uchar>& ShaderRegistry::getBytecode( ShaderHandle handle ) const {
    // std::scoped_lock lock( _mutex );
    if ( handle.id >= _shaders.size() )
    {
        AXION_LOG_ERROR( Logger::Module::GFX, "Accessing invalid ShaderHandle ID: {}", handle.id );
        return INVALID_BYTECODE;
    }

    auto& record = _shaders[handle.id];
    if ( !record.alive )
    {
        AXION_LOG_ERROR( Logger::Module::GFX, "Accessing dead ShaderHandle: {}", record.name );
        return INVALID_BYTECODE;
    }

    if ( record.bytecode.empty() )
    {
        AXION_LOG_WARN( Logger::Module::GFX, "Shader bytecode is empty (not compiled yet?): {}", record.name );
        return INVALID_BYTECODE;
    }

    return record.bytecode;
}

std::optional<ShaderHandle> ShaderRegistry::findShader( const std::string& name ) const {
    // std::scoped_lock lock( _mutex );

    auto it = _nameToHandle.find( name );
    if ( it == _nameToHandle.end() )
        return std::nullopt;
    return it->second;
}

const std::vector<uchar>& ShaderRegistry::compileShader( ShaderHandle handle ) {
    // std::scoped_lock lock( _mutex );

    if ( handle.id >= _shaders.size() )
    {
        AXION_LOG_ERROR( Logger::Module::GFX, "Accessing invalid ShaderHandle ID: {}", handle.id );
        return INVALID_BYTECODE;
    }

    auto& record = _shaders[handle.id];
    if ( record.state == ShaderState::Ready )
    {
        return record.bytecode;
    }

    AXION_LOG_INFO( Logger::Module::GFX, "Compiling Shader: {} with path: {}", record.name, record.desc.path );

    if ( _compiler.compileFile( record.desc, record.bytecode ) )
    {
        record.state = ShaderState::Ready;
    } else
    {
        record.state = ShaderState::Failed;
        AXION_LOG_ERROR( Logger::Module::GFX, "Failed to compile shader: {}", record.name );
        // Aquí podrías cargar un bytecode de "Error Shader" (rosa chillón) por defecto
    }

    return record.bytecode;
}

const std::vector<uchar>& ShaderRegistry::compileShader( const std::string& name ) {
    auto handleOpt = findShader( name );

    if ( !handleOpt.has_value() )
    {
        AXION_LOG_ERROR( Logger::Module::GFX, "Cannot compile shader, name not found: {}", name );
        return INVALID_BYTECODE;
    }
    return compileShader( *handleOpt );
}

void ShaderRegistry::compileAllShaders( bool async ) {
    if ( async )
    {

    } else
    {
        AXION_LOG_INFO( Logger::Module::GFX, "Compiling ALL Shaders | Num Threads: {} ", 1 );
        for ( size_t i = 0; i < _shaders.size(); ++i )
        {
            if ( _shaders[i].alive && _shaders[i].state == ShaderState::Uncompiled )
            {
                compileShader( (ShaderHandle)i );
            }
        }
    }
}

} // namespace Graphics
AXION_NAMESPACE_END