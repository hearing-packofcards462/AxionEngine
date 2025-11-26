#pragma once
#include "Axion/Common/Defines.h"
#include "Axion/Graphics/Handle.h"
#include <optional>
#include <vector>

AXION_NAMESPACE_BEGIN

namespace Graphics {

namespace Shader {

/// @brief Target binary format for shader compilation.
enum NativeFormat : uchar
{
    DXIL,   ///< DirectX Intermediate Language (DirectX 12).
    SPIR_V, ///< Standard Portable Intermediate Representation (Vulkan).
    GLSL    ///< OpenGL Shading Language.
};

/// @brief Configuration descriptor for a shader source.
struct Description {
    std::string              path;                  ///< Path to the .slang source file.
    std::vector<std::string> includePaths;          ///< Additional directories for import resolution.
    NativeFormat             format = DXIL;         ///< Target binary format.
    std::string              entryPoint;            ///< Name of the entry point function (e.g., "vsMain").
    bool                     useReflection = false; ///< Whether to generate reflection data.
};

} // namespace Shader

typedef Shader::Description ShaderDesc;

/// @brief Interface for managing shader compilation, storage, and retrieval.
/// Handles lifecycle, async compilation, and name-to-handle mapping.
class IShaderRegistry
{
public:
    virtual ~IShaderRegistry() = default;

    // Disable copy/move to ensure unique registry ownership.
    IShaderRegistry( const IShaderRegistry& )            = delete;
    IShaderRegistry& operator=( const IShaderRegistry& ) = delete;
    IShaderRegistry( IShaderRegistry&& )                 = delete;
    IShaderRegistry& operator=( IShaderRegistry&& )      = delete;

    /// @brief Fluent builder helper for configuring and registering shaders.
    class Builder
    {
    public:
        Builder( IShaderRegistry& reg, std::string name )
            : _registry( reg )
            , _name( std::move( name ) ) {}

        /// @brief Sets the source file path.
        Builder& path( const std::string& p ) {
            _desc.path = p;
            return *this;
        }

        /// @brief Adds an include directory for imports.
        Builder& include( const std::string& inc ) {
            _desc.includePaths.push_back( inc );
            return *this;
        }

        /// @brief Sets target format to DXIL (DirectX 12).
        Builder& asDXIL() {
            _desc.format = Shader::DXIL;
            return *this;
        }

        /// @brief Sets target format to SPIR-V (Vulkan).
        Builder& asSPIRV() {
            _desc.format = Shader::SPIR_V;
            return *this;
        }

        /// @brief Sets the entry point function name.
        Builder& entry( const std::string& name ) {
            _desc.entryPoint = name;
            return *this;
        }

        /// @brief Finalizes configuration and registers the shader.
        /// @return The handle to the registered shader.
        ShaderHandle load() {
            return _registry.registerShader( _desc, _name );
        }

    private:
        IShaderRegistry& _registry;
        std::string      _name;
        ShaderDesc       _desc;
    };

    /// @brief Starts the fluent registration process for a new shader. To register it, call load()
    /// @param name Logical name for the shader (used for lookups).
    Builder shader( const std::string& name ) {
        return Builder( *this, name );
    }

    /// @brief Retrieves the compiled bytecode for a given handle.
    /// Returns a static empty vector if handle is invalid or compilation failed.
    virtual const std::vector<uchar>& getBytecode( ShaderHandle handle ) const = 0;

    /// @brief Looks up a shader handle by its logical name.
    virtual std::optional<ShaderHandle> findShader( const std::string& name ) const = 0;

    /// @brief Triggers compilation for a specific shader if not already ready.
    virtual const std::vector<uchar>& compileShader( ShaderHandle handle ) = 0;

    /// @brief Triggers compilation by name (Convenience method).
    virtual const std::vector<uchar>& compileShader( const std::string& name ) = 0;

    /// @brief Compiles all registered shaders that are not yet ready.
    /// @param async If true, compilation happens on worker threads (not implemented yet).
    virtual void compileAllShaders( bool async = false ) = 0;

protected:
    IShaderRegistry() = default;

    /// @brief Internal method to register shader metadata without compiling.
    virtual ShaderHandle registerShader( const ShaderDesc& desc, const std::string& name ) = 0;

    friend class Builder;
};
} // namespace Graphics
AXION_NAMESPACE_END