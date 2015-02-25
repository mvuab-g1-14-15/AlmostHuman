#include "Logger.h"
#include "Utils/BaseUtils.h"
#include "Utils/StringUtils.h"

#include <iostream>
#include <windows.h> // WinApi header


CLogger::CLogger()
  : m_uLinesCount( 1 )
  , m_uCapacity( CAPACITY )
  , m_eLogLevel( ELL_INFORMATION )
  , m_bErrors( false )
  , m_bWarnings( false )
  , m_sPathFile( "Logs/" )
{
  m_vLogs.reserve( m_uCapacity );
}

void CLogger::AddNewLog( ELOG_LEVEL ll, const char* format, ... )
{
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
  va_list args;
  char* buffer;
  va_start( args, format );
  int len = _vscprintf( format, args ) + 1;
  buffer = ( char* )malloc( len * sizeof( char ) );
  vsprintf_s( buffer, len, format, args );

  if ( ll >= m_eLogLevel )
  {
    SLog newLog;
    newLog.m_bLineFeed    = false;
    newLog.m_sLogText        = buffer;
    newLog.m_eLogLevel    = ll;
    newLog.m_uLogLine = m_uLinesCount;
    m_uLinesCount++;

    switch ( ll )
    {
    case ELL_INFORMATION:
    {
      SetConsoleTextAttribute( hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
    }
    break;

    case ELL_WARNING:
    {
      SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );
      m_bWarnings = true;
    }
    break;

    case ELL_ERROR:
    {
      SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );
      m_bErrors = true;
    }
    break;
    }

    if ( m_vLogs.size() == m_uCapacity )
    {
      //Tenemos que sacar el log más antiguo:
      std::vector<SLog>::iterator it = m_vLogs.begin();
      m_vLogs.erase( it );
    }

    std::cout << newLog.m_sLogText << std::endl << std::endl;
    m_vLogs.push_back( newLog );
  }

  delete buffer;
}

//TODO: Grabar la informacion de dia y hora
bool CLogger::SaveLogsInFile()
{
  bool isOk = false;
  FILE* file = NULL;


  std::string l_sPathFile    = "";
  uint32 day, month, year, hour, minute, second;
  baseUtils::GetDate( day, month, year, hour, minute, second );
  StringUtils::Format( l_sPathFile, "%s_(%d-%d-%d)_(%dh-%dm-%ds).txt", m_sPathFile.c_str(), day, month, year, hour,
                       minute, second );


  fopen_s( &file, l_sPathFile.c_str(), "w" );
  std::string report = l_sPathFile;
  report += "\n";

  if ( file != NULL )
  {
    std::vector<SLog>::const_iterator it        = m_vLogs.begin();
    std::vector<SLog>::const_iterator itEnd = m_vLogs.end();

    for ( ; it != itEnd; ++it )
    {
      //Recorremos todos los logs y vamos rellenando el
      SLog new_log = *it;
      std::string number;
      StringUtils::Format( l_sPathFile, "%d", new_log.m_uLogLine );


      report += number;

      report += "\t";

      switch ( new_log.m_eLogLevel )
      {
      case ELL_INFORMATION:
      {
        report += "[INFORMATION]";
      }
      break;

      case ELL_WARNING:
      {
        report += "[  WARNING  ]";
      }
      break;

      case ELL_ERROR:
      {
        report += "[___ERROR___]";
      }
      }

      report += "\t";
      report += new_log.m_sLogText;
      report += "\n";
    }

    fprintf( file, "%s", report.c_str() );

    fclose( file );
    isOk = true;
  }

  if ( isOk )
    AddNewLog( ELL_INFORMATION, "El log ha sido guardado correctamente en un fichero fisico" );

  return isOk;
}
