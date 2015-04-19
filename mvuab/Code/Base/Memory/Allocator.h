#pragma once
#include <new>
#include <cmath>

class CAllocator
{
    protected:
        unsigned int m_UsedMemory;
        unsigned int m_NumAllocations;

        unsigned int m_Size;
        void         *m_MemAddress;

    private:

    public:
        CAllocator(unsigned int l_Size, void *l_MemAddress);
        virtual ~CAllocator();

        virtual void *Allocate(unsigned int l_Size, unsigned int Aligment) = 0;
        virtual void  Deallocate(void *l_MemAddress) = 0;

        unsigned int GetSize(void);
        unsigned int GetUsedMemory(void);
        unsigned int GetNumAllocations(void);

        template <class T> T    *MakeNew(void) { return new (Allocate(sizeof(T), __alignof(T))) T(); }
        template <class T> void  MakeDelete(T *l_ClassPtr) { if(l_ClassPtr){l_ClassPtr->~T(); Deallocate(l_ClassPtr);} }
};

inline unsigned int alignOffset(void *l_Address, unsigned int l_Alignment)
{
    unsigned int l_Offset = l_Alignment - ((unsigned int) l_Address & (l_Alignment - 1));
    return (l_Offset == l_Alignment) ? 0 : l_Offset;
}

inline unsigned int alignOffsetWithHeader(void *l_Address, unsigned int l_Alignment, unsigned int l_HeaderSize)
{
    unsigned int l_Offset = alignOffset(l_Address, l_Alignment);
    if(l_HeaderSize > l_Offset) l_Offset += l_Alignment * (unsigned int) ceil((float) (l_HeaderSize - l_Offset) / (float)l_Alignment);

    return l_Offset;
}
