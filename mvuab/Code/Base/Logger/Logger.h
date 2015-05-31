#pragma once
#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "Utils\SingletonPattern.h"
#include "Utils\Types.h"
#include <vector>
#include <string>

#define CAPACITY 500

enum ELogLevel
{
    eLogInfo = 0,
    eLogWarning,
    eLogError
};

struct SLog
{
    ELogLevel       m_eLogLevel;
    bool            m_bLineFeed;
    std::string     m_class;
    std::string     m_File;
    uint32          m_Line;
    std::string     m_sLogText;
};

//------------------------------------------------------------

class CLogger: public CSingleton<CLogger>
{
    public:
        CLogger();
        virtual ~CLogger()
        {
            /*Nothing to do*/;
        }
        ELogLevel GetLogLevel() const
        {
            return m_eLogLevel;
        }
        void SetLogLevel( ELogLevel ll )
        {
            m_eLogLevel = ll;
        }

        void AddNewLog( ELogLevel ll, const char* class_str, const char* file, long line, const char* format, ... );

        const std::vector<SLog>& GetLogs() const
        {
            return m_vLogs;
        }
        bool SaveLogsInFile();
        void  SetPathFile( const std::string& path )
        {
            m_sPathFile = path;
        }

    private:

        std::vector<SLog> m_vLogs;
        ELogLevel m_eLogLevel;
        std::string     m_sPathFile;
        bool            mEnabledConsole;

};

enum CodeColors
{
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};

class Modifier
{
        CodeColors code;
    public:
        Modifier( CodeColors pCode ) : code( pCode ) {}
        void SetColor( CodeColors Color )
        {
            code = Color;
        }
        friend std::ostream&
        operator<<( std::ostream& os, const Modifier& mod )
        {
            return os << "\033[" << mod.code << "m";
        }
};

#endif // INC_LOGGER_H_