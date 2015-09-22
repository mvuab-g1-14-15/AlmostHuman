#include "./ConditionVarible.h"

CConditionVarible::CConditionVarible()
{
    InitializeConditionVariable(&m_ConditionVariable);
}

CConditionVarible::~CConditionVarible()
{
}

void CConditionVarible::Sleep(CRITICAL_SECTION &l_CriticalSenction)
{
    SleepConditionVariableCS(&m_ConditionVariable, &l_CriticalSenction, INFINITE);
}

void CConditionVarible::Wake(void)
{
    WakeConditionVariable(&m_ConditionVariable);
}

void CConditionVarible::WakeAll(void)
{
    WakeAllConditionVariable(&m_ConditionVariable);
}