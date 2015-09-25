#include "./RWLock.h"
#include <Windows.h>
#include <stdio.h>

CRWLock::CRWLock  (void) : m_ActiveReaders(0), m_ActiveWriters(0), m_WritersWaiting(0) {}
CRWLock::~CRWLock (void) {}

void CRWLock::ReadLock()
{
    m_Mutex.Lock();
        while(m_ActiveWriters > 0)
            m_Readers.Sleep(m_Mutex.m_CriticalSection);

        m_ActiveReaders++;
    m_Mutex.UnLock();
}

void CRWLock::ReadUnLock()
{
    m_Mutex.Lock();
        m_ActiveReaders--;

        if(m_WritersWaiting > 0 && m_ActiveReaders == 0) m_Writers.Wake();
    m_Mutex.UnLock();
}

void CRWLock::WriteLock()
{
    m_Mutex.Lock();
        m_WritersWaiting++;

        while (m_ActiveReaders > 0 || m_ActiveWriters > 0)
            m_Writers.Sleep(m_Mutex.m_CriticalSection);

        m_ActiveWriters++;
        m_WritersWaiting--;
    m_Mutex.UnLock();
}

void CRWLock::WriteUnLock()
{
    m_Mutex.Lock();
        m_ActiveWriters--;

        if(m_WritersWaiting > 0) m_Writers.Wake();
        else m_Readers.WakeAll();
    m_Mutex.UnLock();
}