#include "MessageHandler\MessageHandler.h"
#include "Utils/BaseUtils.h"

#include <iostream>
#include <windows.h> // WinApi header

void CMessageHandler::Assert( const char* file, long line, const char* aAssertMsg, ... )
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
        wsprintf( s_text, "%s \nFile '%s' Line %d", buffer, file, line );

        switch ( ::MessageBox( NULL, s_text, "ASSERTION ERROR", MB_ICONEXCLAMATION | MB_OKCANCEL ) )
        {
            case IDOK:
                {
                    callIt = 0;
                    _asm { int 3 }
                    break;
                }
        }
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
}