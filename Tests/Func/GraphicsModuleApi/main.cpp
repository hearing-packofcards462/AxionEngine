#pragma once
#include "Axion/Common/Defines.h"
#include "Axion/Graphics/Platforms/GLFW.h"
#include "Axion/Graphics/Platforms/Win32.h"
#include "Axion/Graphics/Renderer.h"
#include "Axion/Graphics/ShaderProgram/ShaderCompiler.h"

USING_AXION_NAMESPACE

int main( /*int argc, char* argv[]*/ ) {

    try
    {
#ifdef AXION_DEBUG
        Axion::Logger::init( Logger::Level::Info, "Engine.log" );
#endif

        auto wnd = Axion::Graphics::createWindowForWin32( GetModuleHandle( nullptr ), { .name = "GFX API TEST" } );
        // auto wnd = Axion::Graphics::createWindowForGLFW(  { .name = "Test Window" } );
        auto rnd = Axion::Graphics::createRenderer( wnd,
                                                    { .gfxApi        = Graphics::API::DirectX12,
                                                      .bufferingType = Graphics::BufferingType::Double,
                                                      .presentMode   = Graphics::PresentMode::Vsync } );

        // Declare Reources
        auto& res        = rnd->getResourcePool();
        auto  bufferTest = res.registerBuffer( { .size = 16, .debugName = "TestBuffer" }, nullptr, "TestBuffer" );
        auto  bufferPtr  = res.getBuffer( bufferTest );
        // AXION_LOG_INFO( Logger::Module::Editor, "Buffer Count: {}", bufferPtr->getRefCount() );
        res.destroyBuffer( bufferTest );

        // Shader

        Axion::Graphics::SlangShaderCompiler compiler;
        compiler.begin();
        Axion::Graphics::SlangCompileDesc compileDesc {};
        compileDesc.path = AXION_SHADER_DIR "/Slang/TestShader.slang";
        std::vector<uchar> outDXILCode;
        compiler.compileFile( compileDesc, outDXILCode );
        compiler.end();

        while ( !wnd->shouldClose() )
        {
            static uint64_t                           frameCounter   = 0;
            static double                             elapsedSeconds = 0.0;
            static std::chrono::high_resolution_clock clock;
            static auto                               t0 = clock.now();

            frameCounter++;
            auto t1        = clock.now();
            auto deltaTime = t1 - t0;
            t0             = t1;

            elapsedSeconds += deltaTime.count() * 1e-9;
            if ( elapsedSeconds > 1.0 )
            {
                // wchar_t buffer[100];
                // double  fps = frameCounter / elapsedSeconds;
                // swprintf_s( buffer, 100, L"FPS: %.2f\n", fps ); // formatea con 2 decimales
                // OutputDebugStringW( buffer );                   // Unicode, no necesitas la versión ANSI

                frameCounter   = 0;
                elapsedSeconds = 0.0;
            }

            wnd->processMessages();
            rnd->render();
        };

    } catch ( const std::exception& e )
    {
        return EXIT_FAILURE;
    }
#ifdef AXION_DEBUG
    Axion::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}