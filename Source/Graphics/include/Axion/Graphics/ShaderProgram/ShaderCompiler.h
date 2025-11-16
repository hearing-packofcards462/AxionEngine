#pragma once
#include <Axion/Graphics/RHI/Common.h>
#include <iostream>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>
#include <string>
#include <vector>

AXION_NAMESPACE_BEGIN

namespace Graphics {

class ShaderCompiler
{
public:
    static bool testCompile();
};


} // namespace Graphics
AXION_NAMESPACE_END