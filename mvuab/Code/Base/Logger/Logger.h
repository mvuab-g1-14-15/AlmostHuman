//----------------------------------------------------------------------------------
// CLogger class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de almacenar mensajes hasta una capacidad limitada.
// Otra clase obtendrá la información almacenada en esta para printarla por pantalla, consola o enviarla por red
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "Utils\SingletonPattern.h"
#include <vector>
#include <string>

//-----Definicion de nuevos tipos-----------------------
#define CAPACITY 500

enum ELOG_LEVEL
{
  //! Nothing is printed out to the log
  ELL_NONE = 0,

  //! High log level, warnings, errors and important information
  //! texts are printed out.
  ELL_INFORMATION,

  //! Default log level, warnings and errors are printed out
  ELL_WARNING,

  //! Low log level, only errors are printed into the log
  ELL_ERROR
};

struct SLog
{
  std::string     m_sLogText;
  ELOG_LEVEL      m_eLogLevel;
  unsigned int    m_uLogLine;
  bool            m_bLineFeed;
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

  void SetCapacity( unsigned int capacity )
  {
    m_uCapacity = capacity;
  }
  unsigned int GetCapacity()
  {
    return m_uCapacity;
  }

  ELOG_LEVEL GetLogLevel() const
  {
    return m_eLogLevel;
  }
  void SetLogLevel( ELOG_LEVEL ll )
  {
    m_eLogLevel = ll;
  }

  void AddNewLog( ELOG_LEVEL ll, const char* format, ... );
  const std::vector<SLog>& GetLogs() const
  {
    return m_vLogs;
  }
  bool Errors() const
  {
    return m_bErrors;
  }
  bool Warnings() const
  {
    return m_bWarnings;
  }
  bool SaveLogsInFile();
  void  SetPathFile( const std::string& path )
  {
    m_sPathFile = path;
  }

private:

  std::vector<SLog> m_vLogs;
  unsigned int m_uLinesCount;
  unsigned int m_uCapacity;
  ELOG_LEVEL m_eLogLevel;
  bool m_bErrors;
  bool m_bWarnings;
  std::string     m_sPathFile;

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