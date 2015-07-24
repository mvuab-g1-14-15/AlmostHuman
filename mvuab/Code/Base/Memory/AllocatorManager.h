#pragma once

class CLinearAllocator;
class CFreeListAllocator;

class CAllocatorManager
{
    private:
        void *m_pLinearAllocatorMem;
        void *m_pFreeListAllocatorMem;

    public:
        CAllocatorManager();
        ~CAllocatorManager();

        CLinearAllocator    *m_pLinearAllocator;
        CFreeListAllocator  *m_pFreeListAllocator;
};
