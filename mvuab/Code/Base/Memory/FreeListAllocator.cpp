#include "FreeListAllocator.h"
#include <stdio.h>

CFreeListAllocator::CFreeListAllocator(unsigned int l_Size, void *l_MemAddress) : CAllocator(l_Size, l_MemAddress)
{
    m_FreeBlocks = (struct FreeBlock *) l_MemAddress;

    m_FreeBlocks->m_Size = l_Size;
    m_FreeBlocks->m_Next = 0;
}

CFreeListAllocator::~CFreeListAllocator()
{
    m_FreeBlocks = 0;
}

void *CFreeListAllocator::Allocate(unsigned int l_Size, unsigned int l_Alignment)
{
    unsigned int l_Offset = 0;
    unsigned int l_TotalSize = 0;
    struct FreeBlock *l_PrevBlock = 0;
    struct FreeBlock *l_FreeBlock = m_FreeBlocks;

    while(l_FreeBlock != 0)
    {
        l_Offset = alignOffsetWithHeader(l_FreeBlock, l_Alignment, sizeof(struct AllocHeader));
        l_TotalSize = l_Size + l_Offset;

        if(l_FreeBlock->m_Size > l_TotalSize) break;

        l_PrevBlock = l_FreeBlock;
        l_FreeBlock = l_FreeBlock->m_Next;
    }

    if(l_FreeBlock == 0) return 0;

    if(l_FreeBlock->m_Size - l_TotalSize <= sizeof(struct FreeBlock))
    {
        l_TotalSize = l_FreeBlock->m_Size;

        if(l_PrevBlock != 0) l_PrevBlock->m_Next = l_FreeBlock->m_Next;
        else m_FreeBlocks = l_FreeBlock->m_Next;
    }
    else
    {
        struct FreeBlock *l_NextFreeBlock = (struct FreeBlock *) ((unsigned int) l_FreeBlock + l_TotalSize);
        l_NextFreeBlock->m_Size = l_FreeBlock->m_Size - l_TotalSize;
        l_NextFreeBlock->m_Next = l_FreeBlock->m_Next;

        if(l_PrevBlock != 0) l_PrevBlock->m_Next = l_NextFreeBlock;
        else m_FreeBlocks = l_NextFreeBlock;
    }

    unsigned int l_NewMemAddress = (unsigned int) l_FreeBlock + l_Offset;
    struct AllocHeader *l_AllocHeader = (struct AllocHeader *) (l_NewMemAddress - sizeof(struct AllocHeader));

    l_AllocHeader->m_Size = l_Size + l_Offset;
    l_AllocHeader->m_Alignment = l_Offset;

    m_UsedMemory += l_Size + l_Offset;
    m_NumAllocations++;

    return (void *) (l_NewMemAddress);
}

void CFreeListAllocator::Deallocate(void *l_MemAddress)
{
    if(l_MemAddress == 0) return;

    struct AllocHeader *l_AllocHeader = (struct AllocHeader *) ((unsigned int) l_MemAddress - sizeof(struct AllocHeader));
    unsigned int l_BlockStart = (unsigned int) l_MemAddress - l_AllocHeader->m_Alignment;
    unsigned int l_BlockEnd = l_BlockStart + l_AllocHeader->m_Size;

    struct FreeBlock *l_PrevBlock = 0;
    struct FreeBlock *l_FreeBlock = m_FreeBlocks;

    while(l_FreeBlock != 0 && (unsigned int) l_FreeBlock < l_BlockEnd)
    {
        l_PrevBlock = l_FreeBlock;
        l_FreeBlock = l_FreeBlock->m_Next;
    }

    if(l_PrevBlock == nullptr)
    {
        l_PrevBlock = (FreeBlock*) l_BlockStart;
        l_PrevBlock->m_Size = l_AllocHeader->m_Size;
        l_PrevBlock->m_Next = m_FreeBlocks;

        m_FreeBlocks = l_PrevBlock;
    }
    else if((unsigned int) l_PrevBlock + l_PrevBlock->m_Size == l_BlockStart)
    {
        l_PrevBlock->m_Size += l_AllocHeader->m_Size;
    }
    else
    {
        FreeBlock* temp = (FreeBlock*) l_BlockStart;
        temp->m_Size = l_AllocHeader->m_Size;
        temp->m_Next = l_PrevBlock->m_Next;
        l_PrevBlock->m_Next = temp;

        l_PrevBlock = temp;
    }

    if( l_PrevBlock != 0 && (unsigned int) l_FreeBlock == l_BlockEnd)
    {
        l_PrevBlock->m_Size += l_FreeBlock->m_Size;
        l_PrevBlock->m_Next = l_FreeBlock->m_Next;
    }

    m_UsedMemory -= l_AllocHeader->m_Size;
    m_NumAllocations--;
}