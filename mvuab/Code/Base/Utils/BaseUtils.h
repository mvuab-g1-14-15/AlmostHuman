//-----------------------------------------------------------------------
// Base Utils
// Utilidades para la libreria BASE
//-----------------------------------------------------------------------
#ifndef __BASE_UTILS_H__
#define __BASE_UTILS_H__


#include <sstream>
#include <stdio.h>
#include <vector>
#include "Windows.h"
#include "Utils\Types.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace baseUtils 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace baseUtils 
{
    inline void    FormatSrting    ( std::string& output, const char* format, ... )
    {
        va_list args;
        char* buffer;
        va_start(args,format);
        int len = _vscprintf(format, args) + 1;
        buffer = (char*)malloc(len*sizeof(char));
        vsprintf_s( buffer, len, format, args );
        output = buffer;
        delete buffer;
    }

    inline void GetDate (uint32& day, uint32& month, uint32& year, uint32& hour, uint32& minute, uint32& second)
    {
        SYSTEMTIME st;
        GetSystemTime(&st);
        day            =    st.wDay;
        month        = st.wMonth;
        year        = st.wYear;
        hour        = st.wHour;
        minute    = st.wMinute;
        second    = st.wSecond;
    }

	inline void GetFilesFromPath(const std::string &Path, const std::string &Extension, std::vector<std::string> &_OutFiles)
	{
		std::string FilesToLookUp = Path + "*." + Extension;
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind= FindFirstFile(FilesToLookUp.c_str(), &FindFileData);
		while (hFind != INVALID_HANDLE_VALUE) 
		{
			_OutFiles.push_back(FindFileData.cFileName);
			if (!FindNextFile(hFind, &FindFileData))
			{
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}
		}
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace baseUtils 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BASE_UTILS_H__
