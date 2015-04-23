#include "Logger.h"
#include "Utils/BaseUtils.h"
#include "Utils/StringUtils.h"

#include <iostream>
#include <windows.h> // WinApi header


CLogger::CLogger()
    : m_eLogLevel( eLogInfo )
    , m_sPathFile( "Logs/" )
{
}

void CLogger::AddNewLog( ELogLevel ll, const char* class_str, const char* file, long line, const char* format, ... )
{
    if ( ll >= m_eLogLevel )
    {
        va_list args;
        char* buffer;
        va_start( args, format );
        int len = _vscprintf( format, args ) + 1;
        buffer = ( char* )malloc( len * sizeof( char ) );
        vsprintf_s( buffer, len, format, args );

        SLog newLog;
        newLog.m_bLineFeed    = false;
        newLog.m_sLogText     = buffer;
        newLog.m_eLogLevel    = ll;
        newLog.m_class        = class_str;
        newLog.m_File         = file;
        newLog.m_Line         = line;

        HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        switch ( ll )
        {
            case eLogInfo:
                {
                    SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
                }
                break;

            case eLogWarning:
                {
                    SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );
                }
                break;

            case eLogError:
                {
                    SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );
                }
                break;
        }

        std::string lStr("]["), lCopyClassStr(newLog.m_class);
        lCopyClassStr.insert(5, lStr);
        std::cout <<
                  "----------------------------------------------------------------------------------------------------------------------------------\n";
        std::cout << "[" << lCopyClassStr << "]" << std::endl << "[File][" << file << "]" << std::endl << "[Line][" << line <<
                  "]" << std::endl << "[Message]" << newLog.m_sLogText << std::endl;
        std::cout <<
                  "----------------------------------------------------------------------------------------------------------------------------------\n";

        // Add new log to the vector
        m_vLogs.push_back( newLog );

        delete buffer;
    }
}

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

            report += number;

            report += "\t";

            switch ( new_log.m_eLogLevel )
            {
                case eLogInfo:
                    {
                        report += "[INFORMATION]";
                    }
                    break;

                case eLogWarning:
                    {
                        report += "[WARNING]";
                    }
                    break;

                case eLogError:
                    {
                        report += "[ERROR]";
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

    return isOk;
}
