
#pragma once
#include "Axion/Graphics/Subsystems/ShaderRegistry.h"
#include "ShaderCompiler.h"

AXION_NAMESPACE_BEGIN

namespace Graphics {

DEFINE_UNIQUE_PTR_FOR_TYPE( ShaderRegistry, ShaderRegistry )

class ShaderRegistry final : public IShaderRegistry
{
public:
    explicit ShaderRegistry();
    ~ShaderRegistry() override;

    const std::vector<uchar>& getBytecode( ShaderHandle handle ) const override;
    // const ShaderLayoutDesc& getLayout( ShaderHandle handle ) const;
    std::optional<ShaderHandle> findShader( const std::string& name ) const override;
    const std::vector<uchar>&   compileShader( ShaderHandle handle ) override;
    const std::vector<uchar>&   compileShader( const std::string& name ) override;
    void                        compileAllShaders( bool async = false ) override;

private:
    ShaderHandle registerShader( const ShaderDesc& desc, const std::string& name ) override;

    enum class ShaderState : uint8_t
    {
        Uncompiled,
        Compiling,
        Ready,
        Failed
    };
    struct ShaderRecord {
        std::vector<uchar> bytecode;
        ShaderDesc         desc;
        // ShaderLayoutDesc   layout;
        std::string name;
        ShaderState state = ShaderState::Uncompiled;
        bool        alive = false;
    };

    ShaderCompiler _compiler;
    std::mutex     _mutex;

    std::vector<ShaderRecord>                     _shaders;
    std::unordered_map<std::string, ShaderHandle> _nameToHandle;
};

} // namespace Graphics
AXION_NAMESPACE_END