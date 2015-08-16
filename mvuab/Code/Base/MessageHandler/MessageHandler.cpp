#include "MessageHandler\MessageHandler.h"
#include "Utils/BaseUtils.h"

#include <iostream>
#include <windows.h> // WinApi header
#include <assert.h>

static bool sEnabledAsserts = true;

void CMessageHandler::Assert( const char* file, long line, const char* aAssertMsg, ... )
{
    if( sEnabledAsserts )
    {
        va_list args;
        char* buffer;
        va_start( args, aAssertMsg );
        int len = _vscprintf( aAssertMsg, args ) + 1;
        buffer = ( char* )malloc( len * sizeof( char ) );
        vsprintf_s( buffer, len, aAssertMsg, args );
        static char s_text[199] = "";
        static int callIt = 1;
        va_end(args);

        if ( callIt )
        {
            wsprintf( s_text, "%s \nFile:\t'%s'\nLine:\t%d \n\n Abort:\tLaunch Breakpoint\nRetry:\tContinues Execution\nIgnore:\tIgnores all asserts", buffer, file,
                      line );
            switch ( ::MessageBox( NULL, s_text, "ASSERTION ERROR", MB_ICONHAND | MB_ABORTRETRYIGNORE ) )
            {
            case IDABORT:
            {
                callIt = 0;
                __debugbreak();//_asm { int 3 }
                break;
            }
            case IDIGNORE:
            {
                sEnabledAsserts = false;
                break;
            }
            }
        }

        free(buffer);
    }
}

void CMessageHandler::FatalError( const char* file, long line, const char* aAssertMsg, ... )
{
    va_list args;
    char* buffer;
    va_start( args, aAssertMsg );
    int len = _vscprintf( aAssertMsg, args ) + 1;
    buffer = ( char* )malloc( len * sizeof( char ) );
    vsprintf_s( buffer, len, aAssertMsg, args );
    static char s_text[199] = "";
    static int callIt = 1;
    va_end(args);

    if ( callIt )
    {
        wsprintf( s_text, "%s \nFile '%s' Line %d\nOk:Close application", buffer, file, line );

        switch ( ::MessageBox( NULL, s_text, "FATAL ERROR", MB_ICONERROR | MB_OK ) )
        {
        case IDOK:
        {
            ::exit( EXIT_FAILURE );
            break;
        }
        }
    }

    free(buffer);
}