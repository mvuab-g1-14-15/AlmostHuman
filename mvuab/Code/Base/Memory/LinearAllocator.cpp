#include "LinearAllocator.h"

CLinearAllocator::CLinearAllocator(unsigned int l_Size, void *l_MemAddress) : CAllocator(l_Size, l_MemAddress)
{
    m_CurrentAddress = l_MemAddress;
}

CLinearAllocator::~CLinearAllocator()
{
    m_CurrentAddress = 0;
}

void *CLinearAllocator::Allocate(unsigned int l_Size, unsigned int l_Alignment)
{
    unsigned int l_Offset = alignOffset(m_CurrentAddress, l_Alignment);
    unsigned int l_TotalSize = l_Offset + l_Size;

    if(m_UsedMemory + l_TotalSize > m_Size) return 0;
    unsigned int l_NewMemAddress = (unsigned int) m_CurrentAddress + l_Offset;

    m_CurrentAddress = (void *) (l_NewMemAddress + l_Size);
    m_UsedMemory += l_TotalSize;
    m_NumAllocations++;

    return (void *) l_NewMemAddress;
}

void CLinearAllocator::Deallocate(void *l_MemAddress)
{
}

void CLinearAllocator::Reset(void)
{
    m_CurrentAddress = m_MemAddress;
    m_NumAllocations = 0;
    m_UsedMemory = 0;
}