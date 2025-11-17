#pragma once
#include <Axion/Graphics/RHI/Common.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>

AXION_NAMESPACE_BEGIN

namespace Graphics {

using namespace slang;

class SlangShaderCompiler
{
public:
    enum NativeFormat : uchar
    {
        DXIL,
        SPIR_V,
        GLSL
    };
    struct CompileDescription {
        std::string              path;         // source file
        std::vector<std::string> includePaths; // for Slang `import`
        NativeFormat             format = DXIL;
    };

    void begin();
    bool compileFile( const CompileDescription& desc, std::vector<uchar>& outCode );
    void end();

private:
    Slang::ComPtr<IGlobalSession> _globalSession = nullptr;

}; // namespace ShaderCompiler

typedef SlangShaderCompiler::CompileDescription SlangCompileDesc;

} // namespace Graphics
AXION_NAMESPACE_END