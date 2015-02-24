//-----------------------------------------------------------------------
// Base Utils
// Utilidades para la libreria BASE
//-----------------------------------------------------------------------
#ifndef __BASE_UTILS_H__
#define __BASE_UTILS_H__


#include <sstream>
#include <stdio.h>
#include <vector>
#include <Windows.h>
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include <sstream>
#include <string>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace baseUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace baseUtils
{
inline void    FormatSrting( std::string& output, const char* format, ... )
{
  va_list args;
  char* buffer;
  va_start( args, format );
  int len = _vscprintf( format, args ) + 1;
  buffer = ( char* )malloc( len * sizeof( char ) );
  vsprintf_s( buffer, len, format, args );
  output = buffer;
  delete buffer;
}

inline void  Trace( const char* format, ... )
{
  va_list args;
  char* buffer;
  va_start( args, format );
  int len = _vscprintf( format, args ) + 1;
  buffer = ( char* )malloc( len * sizeof( char ) );
  vsprintf_s( buffer, len, format, args );
  OutputDebugStringA( buffer );
  delete buffer;
}

inline void TraceVect3f( const std::string& output, Math::Vect3f vector )
{
  std::string  outputStr = output + "=>%f-%f-%f\n";
  Trace( outputStr.c_str(), vector.x, vector.y, vector.z );
}

inline void GetDate( uint32& day, uint32& month, uint32& year, uint32& hour, uint32& minute,
                     uint32& second )
{
  SYSTEMTIME st;
  GetSystemTime( &st );
  day            =    st.wDay;
  month        = st.wMonth;
  year        = st.wYear;
  hour        = st.wHour;
  minute    = st.wMinute;
  second    = st.wSecond;
}

inline void GetFilesFromPath( const std::string& Path, const std::string& Extension,
                              std::vector<std::string>& _OutFiles )
{
  std::string FilesToLookUp = Path + "*." + Extension;
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = FindFirstFile( FilesToLookUp.c_str(), &FindFileData );

  while ( hFind != INVALID_HANDLE_VALUE )
  {
    _OutFiles.push_back( FindFileData.cFileName );

    if ( !FindNextFile( hFind, &FindFileData ) )
    {
      FindClose( hFind );
      hFind = INVALID_HANDLE_VALUE;
    }
  }
}

inline std::vector<std::string>& split( const std::string& s, char delim,
                                        std::vector<std::string>& elems )
{
  std::stringstream ss( s );
  std::string item;

  while ( std::getline( ss, item, delim ) )
    elems.push_back( item );

  return elems;
}

inline std::vector<std::string> split( const std::string& s, char delim )
{
  std::vector<std::string> elems;
  split( s, delim, elems );
  return elems;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace baseUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BASE_UTILS_H__
