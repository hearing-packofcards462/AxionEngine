#pragma once
#include "Raiko/Common/Defines.h"
#include "Raiko/Graphics/Platforms/Win32.h"
#include "Raiko/Graphics/Platforms/GLFW.h"
#include "Raiko/Graphics/Renderer.h"

USING_RAIKO_NAMESPACE

int main( /*int argc, char* argv[]*/ ) {

    try
    {
#ifdef RAIKO_DEBUG
        Raiko::Logger::init( Logger::Level::Info, "Engine.log" );
#endif

        auto wnd = Raiko::Graphics::createWindowForWin32( GetModuleHandle( nullptr ), { .name = "Test Window" } );
        // auto wnd = Raiko::Graphics::createWindowForGLFW(  { .name = "Test Window" } );
        auto rnd = Raiko::Graphics::createRenderer( wnd,
                                                    { .gfxApi        = Graphics::API::DirectX12,
                                                      .bufferingType = Graphics::BufferingType::Double,
                                                      .presentMode   = Graphics::PresentMode::Vsync } );

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

#ifdef RAIKO_DEBUG
        Raiko::Logger::shutdown();
#endif

    } catch ( const std::exception& e )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}