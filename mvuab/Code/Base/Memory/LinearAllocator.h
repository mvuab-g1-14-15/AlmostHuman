#pragma once
#include "Allocator.h"

class CLinearAllocator : public CAllocator
{
    private:
        void *m_CurrentAddress;

    public:
        CLinearAllocator(unsigned int l_Size, void *l_MemAddress);
        ~CLinearAllocator();

        void *Allocate(unsigned int l_Size, unsigned int l_Alignment);
        void  Deallocate(void *l_MemAddress);
        void  Reset(void);
};
