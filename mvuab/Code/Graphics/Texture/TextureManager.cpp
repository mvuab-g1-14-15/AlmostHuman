#include "TextureManager.h"
#include "EngineManagers.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

CTextureManager::CTextureManager() :CManager()
{
}

CTextureManager::CTextureManager(CXMLTreeNode& atts) :CManager(atts)
{
}

CTextureManager::~CTextureManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(it->second);
        it->second = 0;
    }

    m_Resources.clear();
}

void CTextureManager::Reload()
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
    for ( ; itb != ite; ++itb ) itb->second->Reload();
}

CTexture *CTextureManager::GetTexture(const std::string &fileName)
{
    TMapResource::iterator it = m_Resources.find(fileName);
    if(it != m_Resources.end()) return it->second;

    CTexture *t = 0;
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    if(fileName.find("Cube") != std::string::npos) t = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CCubedTexture>();
    else t = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CTexture>();

    if(!t->Load(fileName))
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(t);
        LOG_ERROR_APPLICATION( "The texture %s could not be loaded", fileName.c_str() );
        return 0;
    }

    m_Resources[fileName] = t;
    return t;
}