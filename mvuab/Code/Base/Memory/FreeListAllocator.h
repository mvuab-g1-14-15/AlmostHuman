#pragma once
#include "Allocator.h"

class CFreeListAllocator : public CAllocator
{
    private:
        struct AllocHeader
        {
            unsigned int m_Size;
            unsigned int m_Alignment;
        };

        struct FreeBlock
        {
            unsigned int m_Size;
            FreeBlock *m_Next;
        };

        struct FreeBlock *m_FreeBlocks;

    public:
        CFreeListAllocator(unsigned int l_Size, void *l_MemAddress);
        ~CFreeListAllocator();

        void *Allocate(unsigned int l_Size, unsigned int l_Alignment);
        void  Deallocate(void *l_MemAddress);
};
