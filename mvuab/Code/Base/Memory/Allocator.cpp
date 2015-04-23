#include "Allocator.h"

CAllocator::CAllocator(unsigned int l_Size, void *l_MemAddress)
{
    m_Size = l_Size;
    m_MemAddress = l_MemAddress;

    m_UsedMemory = 0;
    m_NumAllocations = 0;
}

CAllocator::~CAllocator()
{
    m_Size = 0;
    m_MemAddress = nullptr;

    m_UsedMemory = 0;
    m_NumAllocations = 0;
}

unsigned int CAllocator::GetSize(void)
{
    return m_Size;
}

unsigned int CAllocator::GetUsedMemory(void)
{
    return m_UsedMemory;
}

unsigned int CAllocator::GetNumAllocations(void)
{
    return m_NumAllocations;
}