#pragma once
#ifndef AH_MUTEX_
#define AH_MUTEX_

#include <Windows.h>

class CMutex
{
    public:
        CMutex  (void);
        ~CMutex (void);

        void Lock   (void);
        void UnLock (void);

        CRITICAL_SECTION    m_CriticalSection;
};

#endif 