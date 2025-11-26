#include "ShaderCompiler.h"
#include <filesystem>

AXION_NAMESPACE_BEGIN

namespace Graphics {

void ShaderCompiler::begin() {
    // 1. Create Only One Slang Session for performance
    // CAUTION ! MONO-THREAD
    if ( !_globalSession )
    {
        SlangGlobalSessionDesc desc = {};
        desc.enableGLSL             = true;
        createGlobalSession( &desc, _globalSession.writeRef() );
    }
}

bool ShaderCompiler::compileFile( const ShaderDesc& desc, std::vector<uchar>& outCode ) {

    AXION_LOG_ASSERT( _globalSession, Logger::Module::GFX, "No Slang Compile Session Active" );

    SessionDesc sessionDesc {};

    TargetDesc targetDesc;
    switch ( desc.format )
    {
        case Shader::NativeFormat::DXIL:
            targetDesc.format  = SLANG_DXIL;
            targetDesc.profile = _globalSession->findProfile( "sm_6_0" );
            break;
        case Shader::NativeFormat::SPIR_V:
            targetDesc.format  = SLANG_SPIRV;
            targetDesc.profile = _globalSession->findProfile( "glsl_450" );
            break;
        case Shader::NativeFormat::GLSL:
            targetDesc.format  = SLANG_GLSL;
            targetDesc.profile = _globalSession->findProfile( "glsl_450" );
            break;
        default:
            break;
    }
    // --- Fill Slang import/include paths ---
    std::filesystem::path shaderFilePath( desc.path );
    if ( !std::filesystem::exists( shaderFilePath ) )
    {
        AXION_LOG_ERROR( Logger::Module::Shader, "Shader file not found: {}", desc.path );
        return false;
    }

    std::string shaderDir  = shaderFilePath.parent_path().string();
    std::string moduleName = shaderFilePath.stem().string();

    std::vector<const char*> includePtrs;
    includePtrs.reserve( desc.includePaths.size() + 1 ); // +1 para el directorio del shader

    // Añadimos el directorio donde está el propio shader para que Slang lo encuentre
    includePtrs.push_back( shaderDir.c_str() );

    // Añadimos los includes extra del usuario
    for ( auto& p : desc.includePaths )
        includePtrs.push_back( p.c_str() );

    sessionDesc.searchPaths     = includePtrs.data();
    sessionDesc.searchPathCount = (SlangInt)includePtrs.size();
    sessionDesc.targets         = &targetDesc;
    sessionDesc.targetCount     = 1;

    // PreprocessorMacroDesc fancyFlag    = { "ENABLE_FANCY_FEATURE", "1" };
    // sessionDesc.preprocessorMacros     = &fancyFlag;
    // sessionDesc.preprocessorMacroCount = 1;

    Slang::ComPtr<ISession> session;
    _globalSession->createSession( sessionDesc, session.writeRef() );

    Slang::ComPtr<IBlob>   diagnostics;
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

    // --------------------------------------------------------
    // FETCH ENTRY POINT
    // --------------------------------------------------------
    Slang::ComPtr<slang::IEntryPoint> entryPoint;

    if ( !desc.entryPoint.empty() )
    {
        module->findEntryPointByName( desc.entryPoint.c_str(), entryPoint.writeRef() );
    } else
    {
        AXION_LOG_WARN( Logger::Module::Shader, "Entry Point not Defined, automatically looking for an entry point" );
        if ( module->getDefinedEntryPointCount() > 0 )
        {
            module->getDefinedEntryPoint( 0, entryPoint.writeRef() );
        }
    }

    if ( !entryPoint )
    {
        AXION_LOG_ERROR( Logger::Module::Shader,
                         "Failed to find entry point '{}' in {}",
                         desc.entryPoint.empty() ? "(auto)" : desc.entryPoint,
                         desc.path );
        return false;
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
}
void ShaderCompiler::end() {
    _globalSession = nullptr;
}
} // namespace Graphics
// namespace Graphics
AXION_NAMESPACE_END
