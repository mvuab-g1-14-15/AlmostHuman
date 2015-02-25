#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <sstream>
#include <stdio.h>
#include <vector>
#include <Windows.h>
#include "Utils\Types.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace StringUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace StringUtils
{
inline void Format( std::string& output, const char* format, ... )
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

inline std::vector<std::string>& Split( const std::string& s, char delim,
                                        std::vector<std::string>& elems )
{
  std::stringstream ss( s );
  std::string item;

  while ( std::getline( ss, item, delim ) )
    elems.push_back( item );

  return elems;
}

inline std::vector<std::string> Split( const std::string& s, char delim )
{
  std::vector<std::string> elems;
  Split( s, delim, elems );
  return elems;
}

inline char* ToCharPtr( const std::string& s )
{
  char* cstr = new char[s.length() + 1];
  strcpy( cstr, s.c_str() );
  return cstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace StringUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__String_UTILS_H__