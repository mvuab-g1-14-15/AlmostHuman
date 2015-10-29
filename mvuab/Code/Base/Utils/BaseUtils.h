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
#include "Math\Color.h"
#include <sstream>
#include <string>
#include "Defines.h"
#include <sys\stat.h>

#ifdef _DEBUG
    #define TIMER_START() \
        LARGE_INTEGER g_timeFreq = { 0 }, g_lastTime = { 0 }, g_actualTime = { 0 }; \
        QueryPerformanceCounter(&g_lastTime); \
        QueryPerformanceFrequency(&g_timeFreq);

    #define TIMER_STOP( msg ) \
        QueryPerformanceCounter(&g_actualTime); \
        double t = (double) (g_actualTime.QuadPart - g_lastTime.QuadPart) / g_timeFreq.QuadPart;\
        STATIC_LOG_INFO_APPLICATION("%s %f milliseconds\n", msg, (float) t * 1000.0f);
#else
# define TIMER_START()
# define TIMER_STOP(msg)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace baseUtils
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace baseUtils
{
  inline long GetFileSize(std::string filename)
  {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
  }

  inline bool fileExists(const std::string& file)
  {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
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
        free(buffer);
        va_end(args);
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

    inline float Random()
    {
        return rand() / (float)RAND_MAX;
    }

    inline unsigned int RandRange( unsigned int aMin, unsigned int aMax )
    {
        if ( aMin > aMax )
        {
            std::swap( aMin, aMax );
        }

        return (unsigned int)Math::Utils::Floor( Random() * ( ( aMax - aMin )  + aMin ) );
    }

    inline float RandRange( float fMin, float fMax )
    {
        if ( fMin > fMax )
        {
            std::swap( fMin, fMax );
        }
        return ( Random() * ( fMax - fMin ) ) + fMin;
    }

    inline bool RandomBool()
    {
      return ( ( rand() % 2 ) == 1 );
    }

    inline Math::Vect3f RandRange( Math::Vect3f aMin, Math::Vect3f aMax )
    {
        float x = RandRange( aMin.x, aMax.x );
        float y = RandRange( aMin.y, aMax.y );
        float z = RandRange( aMin.z, aMax.z );

        return Math::Vect3f( x, y, z);
    }

    inline Math::CColor RandRange( Math::CColor aMin, Math::CColor aMax )
    {
        float r = RandRange( aMin.r, aMax.r );
        float g = RandRange( aMin.g, aMax.g );
        float b = RandRange( aMin.b, aMax.b );
        float a = RandRange( aMin.a, aMax.a );

        return Math::CColor( r, g, b, 1.0f);
    }

    inline Math::Vect2f RandRange( Math::Vect2f aMin, Math::Vect2f aMax )
    {
        float x = RandRange( aMin.x, aMax.x );
        float y = RandRange( aMin.y, aMax.y );

        return Math::Vect2f( x, y );
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
