#include <Axion/Graphics/ShaderProgram/ShaderCompiler.h>
#include <filesystem>

AXION_NAMESPACE_BEGIN

namespace Graphics {

void SlangShaderCompiler::begin() {
    // 1. Create Only One Slang Session for performance
    // CAUTION ! MONO-THREAD
    if ( !_globalSession )
    {
        SlangGlobalSessionDesc desc = {};
        desc.enableGLSL             = true;
        createGlobalSession( &desc, _globalSession.writeRef() );
    }
}

bool SlangShaderCompiler::compileFile( const CompileDescription& desc, std::vector<uchar>& outCode ) {

    AXION_LOG_ASSERT( _globalSession, Logger::Module::GFX, "No Slang Compile Session Active" );

    SessionDesc sessionDesc {};

    TargetDesc targetDesc;
    switch ( desc.format )
    {
        case NativeFormat::DXIL:
            targetDesc.format = SLANG_DXIL;
            break;
        case NativeFormat::SPIR_V:
            targetDesc.format = SLANG_SPIRV;
            break;
        case NativeFormat::GLSL:
            targetDesc.format = SLANG_GLSL;
            break;
        default:
            break;
    }
    // --- Fill Slang import/include paths ---
    std::vector<const char*> includePtrs;
    includePtrs.reserve( desc.includePaths.size() );

    for ( auto& p : desc.includePaths )
        includePtrs.push_back( p.c_str() );

    sessionDesc.searchPaths     = includePtrs.data();
    sessionDesc.searchPathCount = (SlangInt)includePtrs.size();

    // PreprocessorMacroDesc fancyFlag    = { "ENABLE_FANCY_FEATURE", "1" };
    // sessionDesc.preprocessorMacros     = &fancyFlag;
    // sessionDesc.preprocessorMacroCount = 1;

    Slang::ComPtr<ISession> session;
    _globalSession->createSession( sessionDesc, session.writeRef() );

    Slang::ComPtr<IBlob> diagnostics;
    std::string          moduleName =
        std::filesystem::path( desc.path ).stem().string();
    Slang::ComPtr<IModule> module( session->loadModule( moduleName.c_str(), diagnostics.writeRef() ) );

    if ( diagnostics )
    {
        const char* diagText = (const char*)diagnostics->getBufferPointer();
        AXION_LOG_ERROR( Logger::Module::Shader, "{}", diagText );
    }
    if ( !module )
    {
        AXION_LOG_ERROR( Logger::Module::Shader, "Slang failed to load module {}", desc.path.c_str() );
        return false;
    }

    Slang::ComPtr<slang::IEntryPoint> entryPoint;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        module->findEntryPointByName( "computeMain", entryPoint.writeRef() );
        if ( !entryPoint )
        {
            AXION_LOG_ERROR( Logger::Module::Shader, "Slang failed to get entry point", desc.path.c_str() );
            return false;
        }
    }

    IComponentType*                      components[] = { module, entryPoint };
    Slang::ComPtr<slang::IComponentType> composedProgram;
    {
        Slang::ComPtr<slang::IBlob> diagnostics;
        SlangResult                 result = session->createCompositeComponentType(
            components,
            2,
            composedProgram.writeRef(),
            diagnostics.writeRef() );

        if ( diagnostics )
        {
            const char* diagText = (const char*)diagnostics->getBufferPointer();
            AXION_LOG_ERROR( Logger::Module::Shader, "{}", diagText );
        }
        SLANG_RETURN_ON_FAIL( result );
    }

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    {
        Slang::ComPtr<slang::IBlob> diagnostics;
        SlangResult                 result = composedProgram->link(
            linkedProgram.writeRef(),
            diagnostics.writeRef() );
        if ( diagnostics )
        {
            const char* diagText = (const char*)diagnostics->getBufferPointer();
            AXION_LOG_ERROR( Logger::Module::Shader, "{}", diagText );
        }
        SLANG_RETURN_ON_FAIL( result );
    }

    // 7. Get Target Kernel Code
    Slang::ComPtr<slang::IBlob> nativeCode;
    {
        Slang::ComPtr<slang::IBlob> diagnostics;
        SlangResult                 result = linkedProgram->getEntryPointCode(
            0,
            0,
            nativeCode.writeRef(),
            diagnostics.writeRef() );
        if ( diagnostics )
        {
            const char* diagText = (const char*)diagnostics->getBufferPointer();
            AXION_LOG_ERROR( Logger::Module::Shader, "{}", diagText );
        }
        SLANG_RETURN_ON_FAIL( result );
    }

    outCode.resize( nativeCode->getBufferSize() );
    memcpy( outCode.data(), nativeCode->getBufferPointer(), outCode.size() );

    AXION_LOG_INFO( Logger::Module::Shader, "Compiled {} bytes of native shader code", nativeCode->getBufferSize() );

    return true;

    // slang::ProgramLayout* layout = module->getLayout();
}
void SlangShaderCompiler::end() {
    _globalSession = nullptr;
}
} // namespace Graphics
// namespace Graphics
AXION_NAMESPACE_END
