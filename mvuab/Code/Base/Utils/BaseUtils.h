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

namespace TIMER_VAR
{
  static LARGE_INTEGER g_timeFreq = { 0 }, g_lastTime = { 0 }, g_actualTime = { 0 };
}

inline void TIMER_START()
{
  QueryPerformanceCounter(&TIMER_VAR::g_lastTime); QueryPerformanceFrequency(&TIMER_VAR::g_timeFreq);
}

inline void TIMER_STOP()
{
  QueryPerformanceCounter(&TIMER_VAR::g_actualTime); QueryPerformanceFrequency(&TIMER_VAR::g_timeFreq);
  double t = (double) (TIMER_VAR::g_actualTime.QuadPart - TIMER_VAR::g_lastTime.QuadPart) / (double) TIMER_VAR::g_timeFreq.QuadPart;

  printf("%time: %f\n", (float) t * 1000.0f);
}

inline float Random()
{
    return rand() / (float)RAND_MAX;
}

inline float RandRange( float fMin, float fMax )
{
    if ( fMin > fMax ) std::swap( fMin, fMax );
    return ( Random() * ( fMax - fMin ) ) + fMin;
}

inline Math::Vect3f RandRange( Math::Vect3f aMin, Math::Vect3f aMax )
{
	float x = RandRange( aMin.x, aMax.x );
	float y = RandRange( aMin.y, aMax.y );
	float z = RandRange( aMin.z, aMax.z );

	return Math::Vect3f( x, y, z);
}

inline Math::Vect3f RandUnitVec()
{
    float x = ( Random() * 2.0f ) - 1.0f;
    float y = ( Random() * 2.0f ) - 1.0f;
    float z = ( Random() * 2.0f ) - 1.0f;

    return Math::Vect3f(x, y, z).Normalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace baseUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BASE_UTILS_H__
