#include "./RWLock.h"

CRWLock::CRWLock  (void) {}
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
    m_Mutex.UnLock();

    m_Writers.Wake();
}

void CRWLock::WriteLock()
{
    m_Mutex.Lock();
        while (m_ActiveReaders > 0)
            m_Writers.Sleep(m_Mutex.m_CriticalSection);

        m_ActiveWriters++;
    m_Mutex.UnLock();
}

void CRWLock::WriteUnLock()
{
    m_Mutex.Lock();
        m_ActiveWriters--;
    m_Mutex.UnLock();

    if(m_ActiveWriters > 0) m_Writers.Wake();
    else m_Readers.WakeAll();
}