
#pragma once
#include "Axion/Common/Defines.h"
#include "Axion/Graphics/Handle.h"
#include <optional>
#include <vector>

AXION_NAMESPACE_BEGIN

namespace Graphics {

namespace Shader {

enum NativeFormat : uchar
{
    DXIL,
    SPIR_V,
    GLSL
};
struct Description {
    std::string              path;         // source file
    std::vector<std::string> includePaths; // for Slang `import`
    NativeFormat             format        = DXIL;
    bool                     useReflection = false;
};

} // namespace Shader

typedef Shader::Description ShaderDesc;

DEFINE_UNIQUE_PTR_FOR_TYPE( IShaderRegistry, ShaderRegistry )

class IShaderRegistry
{
public:
    virtual ~IShaderRegistry() = default;

    virtual ShaderHandle              registerShader( const ShaderDesc& desc, const std::string& name ) = 0;
    virtual const std::vector<uchar>& getBytecode( ShaderHandle handle ) const                          = 0;
    // virtual const ShaderLayoutDesc& getLayout( ShaderHandle handle ) const;
    virtual std::optional<ShaderHandle> findShader( const std::string& name ) const = 0;
    virtual const std::vector<uchar>&   compileShader( ShaderHandle handle )        = 0;
    virtual void                        compileAllShaders( bool async = false )     = 0;
};

} // namespace Graphics
AXION_NAMESPACE_END