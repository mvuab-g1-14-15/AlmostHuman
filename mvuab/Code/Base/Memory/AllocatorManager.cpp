#include "AllocatorManager.h"

#include "Allocator.h"
#include "LinearAllocator.h"
#include "FreeListAllocator.h"

#define B   1
#define KB  1024 * B
#define MB  1024 * KB
#define GB  1024 * MB

#define LINEAR_SIZE 16 * MB
#define FREE_LIST_SIZE 2 * GB

CAllocatorManager::CAllocatorManager()
{
    m_pLinearAllocatorMem = (void *) malloc ((unsigned int) LINEAR_SIZE);
    m_pFreeListAllocatorMem = (void *) malloc ((unsigned int) FREE_LIST_SIZE);

    //memset(m_pLinearAllocatorMem, 0, (unsigned int) LINEAR_SIZE);
    //memset(m_pFreeListAllocatorMem, 0, (unsigned int) FREE_LIST_SIZE);

    m_pLinearAllocator = new CLinearAllocator((unsigned int) LINEAR_SIZE, m_pLinearAllocatorMem);
    m_pFreeListAllocator = new CFreeListAllocator((unsigned int) FREE_LIST_SIZE, m_pLinearAllocatorMem);
}

CAllocatorManager::~CAllocatorManager()
{
    delete(m_pLinearAllocator);
    delete(m_pFreeListAllocator);

    free(m_pLinearAllocatorMem);
    free(m_pFreeListAllocatorMem);
}