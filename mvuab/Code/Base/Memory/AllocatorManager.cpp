#include "AllocatorManager.h"

#include "Allocator.h"
#include "LinearAllocator.h"
#include "FreeListAllocator.h"

#define B   1
#define KB  1024 * B
#define MB  1024 * KB
#define GB  1024 * MB

#define LINEAR_SIZE 16 * MB
#define FREE_LIST_SIZE 512 * MB

CAllocatorManager::CAllocatorManager()
{
    m_pLinearAllocatorMem = (void *) malloc (LINEAR_SIZE);
    m_pFreeListAllocatorMem = (void *) malloc (FREE_LIST_SIZE);

    m_pLinearAllocator = new CLinearAllocator(LINEAR_SIZE, m_pLinearAllocatorMem);
    m_pFreeListAllocator = new CFreeListAllocator(LINEAR_SIZE, m_pLinearAllocatorMem);
}

CAllocatorManager::~CAllocatorManager()
{
    delete(m_pLinearAllocator);
    delete(m_pFreeListAllocator);

    free(m_pLinearAllocatorMem);
    free(m_pFreeListAllocatorMem);
}