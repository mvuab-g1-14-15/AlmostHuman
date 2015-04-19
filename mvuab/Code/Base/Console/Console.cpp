#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <wincon.h>
#include "console.h"
#include "Utils\Defines.h"
#include "ActionManager.h"
#include "EngineManagers.h"

#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS
    using namespace std;
#endif

BOOL CConsole::sm_bConsole = FALSE;

CConsole::CConsole( const CXMLTreeNode & atts )
    : CManager(atts)
    , m_bRedirected(FALSE)
    , m_sNumColumns( 0 )
    , m_sNumLines( 0 )
    , m_sMaxLines( 0 )
    , m_wAttrib( 0 )
    , mX( 0 )
    , mY( 0 )
    , mWidth( 1200 )
    , mHeight( 600 )
    , mConsoleNode( atts )
{
}

void CConsole::Init()
{
    const std::string & lNodeName = mConsoleNode.GetName();
    ASSERT( lNodeName == "console" , "Incorrect node for creating the console");

    // Redirect all the stuff to the console
    m_bRedirected = TRUE;

    // Create the console
    if( CreateConsole() == FALSE )
    {
        ASSERT(false, "Error creating the console");
    }

    SetMaxLinesInWindow( mConsoleNode.GetIntProperty("max_lines_in_window", 10 ) );
    SetNumberOfLines(mConsoleNode.GetIntProperty("lines", 10 ) );
    SetNumberOfColumns(mConsoleNode.GetIntProperty("columns", 10 ) );

    // Redirect all to the console
    RedirectToConsole( 0 );

    // Set the user properties to the console
    if( mConsoleNode.GetBoolProperty("full_size", false ) )
    {
        SetFullSize();
    }
    else
    {
        const Math::Vect2i& lSize = mConsoleNode.GetVect2iProperty("size", Math::Vect2i(0, 0));
        ASSERT(lSize.x != 0, "The console width is 0" );
        ASSERT(lSize.y != 0, "The console height is 0" );
        SetSize( lSize.x, lSize.y );
    }
}

void CConsole::Update()
{
    if ( ActionManagerInstance->DoAction( "ClearConsole" ) )
    {
        Clear();
    }
}

void CConsole::Render()
{

}

CConsole::~CConsole()
{
    DestroyConsole ();
}

BOOL CConsole::CreateConsole ()
{
    if (sm_bConsole == TRUE) // we have already created a console
    {
        return FALSE;
    }
    // I am not allocating the console if there is already one.

    if (!AllocConsole ()) //Just call the Console allocation API
    {
        sm_bConsole = FALSE;
        m_dwError = GetLastError (); //Lets get the error and store it away.
        return FALSE;
    }
    else
    {
        sm_bConsole = TRUE; //To make sure we wont allocate again
        //Lets keep all the stuff around
        m_sNumLines = (short)GetSettings (SC_LINES);
        m_sNumColumns = (short)GetSettings (SC_COLUMNS);
        m_sMaxLines  = (short) GetSettings (SC_MAXLINES);
        m_wAttrib = GetSettings (SC_ATTRIB);
        m_dwError = 0; // Lets keep this zero for the time being.
        Recalculate();
        return TRUE;
    }
}

void CConsole::SetSize( int w, int h )
{
    mWidth = w;
    mHeight = h;
    Recalculate();
}

void CConsole::MoveConsole( int x, int y )
{
    mX = x;
    mY = y;
    Recalculate();
}

void CConsole::SetFullSize()
{
    const HWND hDesktop = GetDesktopWindow();
    RECT desktop;
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    mX = desktop.left;
    mY = desktop.top;
    mWidth = desktop.right - desktop.left;
    mHeight = desktop.bottom - desktop.top;
    Recalculate();
}

void CConsole::Recalculate()
{
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, mX, mY, mWidth, mHeight, TRUE);
}

BOOL CConsole::DestroyConsole ()
{
    if (sm_bConsole == FALSE) //There is no console to destroy
    { return TRUE; } //as Well return asif we deleted
    if (!FreeConsole ())
    {
        sm_bConsole = TRUE;
        m_dwError = GetLastError ();//Lets keep the error here if someone wants
        return FALSE;
    }
    else
    {
        sm_bConsole = FALSE;
        return TRUE;
    }
}
short CConsole::GetNumberOfLines()
{
    return (short) GetSettings (SC_LINES);
}

short CConsole::SetNumberOfLines (short sLines)
{
    short sRes = m_sNumLines;
    m_sNumLines = sLines;
    SetupConsole (SC_LINES);
    return sRes;
}

short CConsole::SetNumberOfColumns (short sColumns)
{
    short sOld = m_sNumColumns;
    m_sNumColumns = sColumns;
    SetupConsole (SC_COLUMNS);
    return sOld;
}

short CConsole::GetNumberOfColumns ()
{
    return (short)GetSettings (SC_COLUMNS);
}

WORD CConsole::GetAttributes ()
{
    return (short) GetSettings (SC_ATTRIB);
}
WORD CConsole::SetAttributes (WORD wAttrib, short NumChars)
{
    WORD wOld = m_wAttrib;
    m_wAttrib = wAttrib;
    SetupConsole (SC_ATTRIB);
    ApplyAttrib(NumChars);
    return wOld;
}
short CConsole::SetMaxLinesInWindow (short maxLines)
{
    short sOld = m_sMaxLines;
    m_sMaxLines = maxLines;
    SetupConsole (SC_MAXLINES);
    return sOld;
}
short CConsole::GetMaxLinesInWindow ()
{
    return (short) GetSettings (SC_MAXLINES);
}

//Now here we have the basic beginning traits of a CConsole.
//But it has to do more than Allocing and Freeing consoles.
//So here it is..


void CConsole::RedirectToConsole (WORD wFlags)
{
    int                        hConHandle;
    long                       lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE                       *fp;
    //First lets see if we already have allocated a console
    if (!sm_bConsole)
    { CreateConsole(); }

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo((HANDLE)lStdHandle, &coninfo);
    SetupConsole (SC_COLUMNS | SC_LINES | SC_ATTRIB | SC_MAXLINES);


    // redirect unbuffered STDOUT to the console

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // redirect unbuffered STDIN to the console

    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console

    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );
    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    ios::sync_with_stdio();
    m_bRedirected = TRUE; //Whether the Console is redirected
}

/*
    This will be function which will actually set up the console to the user
    settings.
*/
BOOL CConsole::SetupConsole(WORD wFlags)
{

    long                       lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    if (!sm_bConsole)
    { return FALSE; } // There aint no console to set up, Duh

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo((HANDLE)lStdHandle, &coninfo);


    if (wFlags & SC_COLUMNS || wFlags & SC_LINES) //Set up only the columns
    {
        //Number of Columns to be set
        if (m_sNumColumns)
        { coninfo.dwSize.X = m_sNumColumns; }
        // number of lines to be set
        if (m_sNumLines)
        { coninfo.dwSize.Y = m_sNumLines; }

        //Set the screen buffer size
        SetConsoleScreenBufferSize((HANDLE)lStdHandle, coninfo.dwSize);

    }
    if (wFlags & SC_ATTRIB)
    {
        //Attributes as specified
        if (m_wAttrib)
        { coninfo.wAttributes = m_wAttrib; }
        //Set the Text attributes
        SetConsoleTextAttribute ((HANDLE)lStdHandle, coninfo.wAttributes);
    }

    if (wFlags & SC_MAXLINES)
    {
        SMALL_RECT rect;

        //Maximum Size of the window
        if (m_sMaxLines)
        { rect.Bottom = m_sMaxLines; }
        else
        { rect.Bottom = coninfo.dwMaximumWindowSize.Y; }

        if (m_sNumColumns)
        { rect.Right = m_sNumColumns; }
        else
        { rect.Right = coninfo.dwMaximumWindowSize.X; }
        rect.Top = rect.Left = 0;

        //Set the console window maximum size
        SetConsoleWindowInfo ((HANDLE)lStdHandle, TRUE,
                              &rect);
    }
    return TRUE;

}

HANDLE CConsole::GetHandle (DWORD dwFlag)
{
    if (!sm_bConsole)
    { return (HANDLE) NULL; }
    return GetStdHandle (dwFlag);
}

BOOL CConsole::Clear ()
{
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                       cursor */
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in
                                       the current buffer */

    if (!sm_bConsole)
    { return FALSE; }
    HANDLE hConsole = GetHandle (STD_OUTPUT_HANDLE);

    /* get the number of character cells in the current buffer */


    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if (!bSuccess) { return bSuccess; }

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
                                           dwConSize, coordScreen, &cCharsWritten );
    if (!bSuccess) { return bSuccess; }

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    if (!bSuccess) { return bSuccess; }

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
                                           dwConSize, coordScreen, &cCharsWritten );
    if (!bSuccess) { return bSuccess; }

    /* put the cursor at (0, 0) */

    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
    if (!bSuccess) { return bSuccess; }
    return TRUE;
}

BOOL CConsole::ApplyAttrib (short NumChars)
{
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                       cursor */
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in
                                       the current buffer */
    if (!sm_bConsole)
    { return FALSE; }
    HANDLE hConsole = GetHandle (STD_OUTPUT_HANDLE);
    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );

    if (!bSuccess) { return bSuccess; }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
                                           NumChars ? NumChars : dwConSize, csbi.dwCursorPosition, &cCharsWritten );

    return  bSuccess;
}

WORD CConsole::GetSettings (WORD wFlags)
{
    long                       lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    if (!sm_bConsole)
    { return FALSE; } // There aint no console to set up, Duh

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo((HANDLE)lStdHandle, &coninfo);

    switch (wFlags)
    {
        case SC_ATTRIB:
            return coninfo.wAttributes;
            break;
        case SC_LINES:
            return coninfo.dwSize.Y;
            break;
        case SC_MAXLINES:
            return coninfo.dwMaximumWindowSize.Y;
            break;
        case SC_COLUMNS:
            return coninfo.dwSize.X;
            break;
    }
    return 0;
}

