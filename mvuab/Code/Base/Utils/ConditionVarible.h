#pragma once
#ifndef AH_CONDVAR_
#define AH_CONDVAR_

#include <Windows.h>

class CConditionVarible
{
    private:
        CONDITION_VARIABLE m_ConditionVariable;

    public:
        CConditionVarible  (void);
        ~CConditionVarible (void);

        void Sleep   (CRITICAL_SECTION &l_CriticalSenction);

        void Wake    (void);
        void WakeAll (void);
};

#endif 