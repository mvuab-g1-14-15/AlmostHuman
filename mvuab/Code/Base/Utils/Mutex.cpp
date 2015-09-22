#include "./Mutex.h"

CMutex::CMutex(void)
{
    InitializeCriticalSection(&m_CriticalSection);
}

CMutex::~CMutex(void)
{
    DeleteCriticalSection(&m_CriticalSection);
}

void CMutex::Lock(void)
{
    EnterCriticalSection(&m_CriticalSection);   
}

void CMutex::UnLock(void)
{
    LeaveCriticalSection(&m_CriticalSection);
}