#include <Windows.h>
#include "Math/Vector3.h"
#include "XML/XMLTreeNode.h"
//#include "Enemies/EnemyManager.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"
#include "Engine.h"
#include "EngineConfig.h"
#include "VideogameProcess.h"
#include "TestProcess.h"
#include "Utils\Defines.h"
#include <string>
#include "Exceptions\Exception.h"

#include "ScriptManager.h"
#include "Utils\GPUStatics.h"
#include "Console\Console.h"
#include "TestProcess\PhysicProcess.h"
#include "TestProcess\PlayerPhysicProcess.h"
#include "TestProcess\AStarProcess.h"
#include "TestProcess\PreProcess.h"

#include "EngineConfig.h"
#include "EngineManagers.h"
#include "GraphicsManager.h"

#include <iostream>
#include <ctime>

#include "VideoPlayer.h"

#if _DEBUG
    #include "Memory\MemLeaks.h"
#endif


#define APPLICATION_NAME    "ALMOST HUMAN"
static bool g_Exit = false;

void ShowErrorMessage( const std::string& message )
{
    bool logSaved = CLogger::GetSingletonPtr()->SaveLogsInFile();
    std::string end_message = "";

    if ( logSaved )
    {
        end_message += "Sorry, Application failed. Logs saved\n";
    }
    else
    {
        end_message += "Sorry, Application failed. Logs could not be saved\n";
    }

    end_message += message;
    MessageBox( 0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR );
}

void updateThread(CEngine* pEngine)
{
    while(!g_Exit)
    {
        //pEngine->ProcessInputs();

        if(!CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->isDeviceLost()) 
            pEngine->Render();
    }
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch ( msg )
    {
        case WM_DESTROY:
            {
                PostQuitMessage( 0 );
                exit(1);
            }
            break;

        case WM_KEYDOWN:
            {
                switch ( wParam )
                {
                    case VK_ESCAPE:
                        //Cleanup();
                        PostQuitMessage( 0 );
                        return 0;
                        break;
                }
            }
            break;
    }//end switch( msg )

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow )
{
    try
    {
        #if _DEBUG
        MemLeaks::MemoryBegin();
        #endif
        // Register the window class
        WNDCLASSEX wc =
        {
            sizeof( WNDCLASSEX ),
            CS_CLASSDC, MsgProc,
            0L,
            0L,
            GetModuleHandle( NULL ),
            NULL,
            NULL,
            NULL,
            NULL,
            APPLICATION_NAME,
            NULL
        };
        RegisterClassEx( &wc );

        // http://xmlsoft.org/threads.html
        xmlInitParser();

        // For random number generation
        srand( ( unsigned int )time( 0 ) );

        // Read the configuration of the engine
        CEngineConfig* lEngineConfig = new CEngineConfig();
        lEngineConfig->Load( "./Data/config.xml" );
        CEngine* pEngine = new CEngine();
        CGPUStatics* gpu = new CGPUStatics();
        CLogger* pLogger = new CLogger();

        DWORD style;

        if ( lEngineConfig->GetFullScreenMode() || lEngineConfig->GetFitDesktop() )
        {
            uint32 lTaskBarHeight = 0;

            if ( lEngineConfig->GetFitDesktop() )
            {
                RECT rect;
                HWND taskBar = FindWindow( "Shell_traywnd", NULL );

                if ( taskBar && GetWindowRect( taskBar, &rect ) )
                {
                    lTaskBarHeight = rect.bottom - rect.top;
                }
            }

            lEngineConfig->SetScreenPosition( Math::Vect2i( 0, 0 ) );
            lEngineConfig->SetScreenSize( Math::Vect2i( GetSystemMetrics( SM_CXSCREEN ),  GetSystemMetrics( SM_CYSCREEN ) - lTaskBarHeight ) );
            lEngineConfig->SetScreenResolution( Math::Vect2i( GetSystemMetrics( SM_CXSCREEN ),  GetSystemMetrics( SM_CYSCREEN ) - lTaskBarHeight ) );
            style = WS_POPUP | WS_VISIBLE;
        }
        else
        {
            style = WS_OVERLAPPED | WS_SYSMENU;
        }

        Math::Vect2i lScreenPosition = lEngineConfig->GetScreenPosition();
        Math::Vect2i lScreenSize     = lEngineConfig->GetScreenSize();
        HWND hWnd = CreateWindow
                    (
                        APPLICATION_NAME,
                        APPLICATION_NAME,
                        WS_OVERLAPPEDWINDOW,
                        lScreenPosition.x,
                        lScreenPosition.y,
                        lScreenSize.x,
                        lScreenSize.y,
                        NULL,
                        NULL,
                        wc.hInstance,
                        NULL
                    );

        lEngineConfig->SetWindowId( hWnd );
        pEngine->SetRunnigProcess( new CPlayerPhysicProcess() );
        pEngine->Init( lEngineConfig );
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );
        MSG msg;
        ZeroMemory( &msg, sizeof( msg ) );

        /*CVideoPlayer l_VideoPlayer;
        if(!l_VideoPlayer.Play("test.avi"))
        {
            char s[] = "Error playing video\n";
            OutputDebugStringA(s);
        }*/


        HANDLE h = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) updateThread, pEngine, NULL, NULL);
        while ( msg.message != WM_QUIT )
        {
            if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
            {
                pEngine->ProcessInputs();
                pEngine->Update();

                //if(!CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->isDeviceLost()) 
                //    pEngine->Render();
            }
        }

        g_Exit = true;

        WaitForSingleObject(h, INFINITE);
        CloseHandle(h);
        
        UnregisterClass( APPLICATION_NAME, wc.hInstance );

        // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
        CHECKED_DELETE( pEngine );
        CHECKED_DELETE( pLogger );
        CHECKED_DELETE( gpu );
        CHECKED_DELETE( lEngineConfig );
        #if _DEBUG
        MemLeaks::MemoryEnd();
        #endif
    }
    catch ( CException& e )
    {
        ShowErrorMessage( e.GetDescription() );
    }
    catch ( ... )
    {
        ShowErrorMessage( "Exception Occured" );
    }

    return 0;
}