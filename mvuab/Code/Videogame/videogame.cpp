#include <Windows.h>
#include "Math/Vector3.h"
#include "XML/XMLTreeNode.h"
#include "Enemies/EnemyManager.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"
#include "Engine.h"
#include "VideogameProcess.h"
#include "TestProcess.h"
#include "Utils\Defines.h"
#include <string>
#include "Exceptions\Exception.h"
#include "Core.h"
#include "Utils\GPUStatics.h"
#include "Console\Console.h"

#include <iostream>

#if _DEBUG
#include "Memory\MemLeaks.h"
#endif


#define APPLICATION_NAME    "VIDEOGAME"

void ShowErrorMessage( const std::string& message )
{
  bool logSaved = CLogger::GetSingletonPtr()->SaveLogsInFile();
  std::string end_message = "";

  if ( logSaved )
    end_message += "Sorry, Application failed. Logs saved\n";
  else
    end_message += "Sorry, Application failed. Logs could not be saved\n";

  end_message += message;
  MessageBox( 0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR );
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
    return 0;
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
    WNDCLASSEX wc = {    sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

    RegisterClassEx( &wc );

    // Optain the engine instance
	CGPUStatics *gpu = new CGPUStatics();
    CLogger* pLogger = new CLogger();
    CEngine* pEngine = new CEngine();
    // Create the application's window
    HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL,
                              wc.hInstance, NULL );

#ifdef _DEBUG

    /*if ( AllocConsole() )
    {
      freopen( "CONIN$", "r", stdin );
      freopen( "CONOUT$", "w", stdout );
      freopen( "CONOUT$", "w", stderr );
      //HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
      system( "mode 120, 30" );
      SetConsoleTitle( "LOG WINDOW" );
      _COORD coord;
      coord.X = 800;
      coord.Y = 800;

      _SMALL_RECT Rect;
      Rect.Top = 0;
      Rect.Left = 0;
      Rect.Bottom = 800 - 1;
      Rect.Right = 800 - 1;

      HANDLE Handle = GetStdHandle( STD_OUTPUT_HANDLE );    // Get Handle
      SetConsoleScreenBufferSize( Handle, coord );          // Set Buffer Size
      SetConsoleWindowInfo( Handle, TRUE, &Rect );          // Set Window Size
    }
	*/
#endif

	CConsole con(TRUE);
    con.RedirectToConsole(0);

	// test stdio
    con.SetNumberOfLines (200);
    con.SetNumberOfColumns (132);
    con.SetMaxLinesInWindow (25);

	WORD x = con.SetAttributes (FOREGROUND_BLUE|BACKGROUND_GREEN,2);
	con.SetAttributes (x);

	
    //pEngine->Init( new CVideogameProcess(), "./Data/config.xml", hWnd );
    pEngine->Init( new CTestProcess(), "./Data/config.xml", hWnd );

    SetWindowPos(hWnd, 0, CCore::GetSingletonPtr()->GetWindowXPos(), CCore::GetSingletonPtr()->GetWindowYPos(), CCore::GetSingletonPtr()->GetScreenWidth(), CCore::GetSingletonPtr()->GetScreenHeight(), SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

    // Añadir aquí el Init de la applicacioón

    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );

    //CEnemyManager l_EnemyManager;
    //l_EnemyManager.Init("./Data/enemies.xml");
    // Añadir en el while la condición de salida del programa de la aplicación

    while ( msg.message != WM_QUIT )
    {
      if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
      else
      {
        pEngine->Update();
        pEngine->Render();
      }
    }

    UnregisterClass( APPLICATION_NAME, wc.hInstance );

    // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
    CHECKED_DELETE( pEngine );
    CHECKED_DELETE( pLogger );
	CHECKED_DELETE( gpu );

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