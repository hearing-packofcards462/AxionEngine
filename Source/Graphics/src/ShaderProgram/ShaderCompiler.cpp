#include <Axion/Graphics/ShaderProgram/ShaderCompiler.h>

AXION_NAMESPACE_BEGIN

namespace Graphics {

bool ShaderCompiler::testCompile() {

    // 1. Create Slang session
    Slang::ComPtr<slang::IGlobalSession> globalSession;
    if ( SLANG_FAILED( slang_createGlobalSession( 1 ,globalSession.writeRef() ) ) )
    {
        std::cerr << "[Slang] Failed to create global session.\n";
        return false;
    }

   
}
}
// namespace Graphics
AXION_NAMESPACE_END