#pragma once
#include <Axion/Graphics/RHI/Common.h>
#include <Axion/Graphics/Subsystems/ShaderRegistry.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>

AXION_NAMESPACE_BEGIN

namespace Graphics {

using namespace slang;

class ShaderCompiler
{
public:
    void begin();
    bool compileFile( const ShaderDesc& desc, std::vector<uchar>& outCode );
    void end();

private:
    Slang::ComPtr<IGlobalSession> _globalSession = nullptr;

}; // namespace ShaderCompiler

} // namespace Graphics
AXION_NAMESPACE_END