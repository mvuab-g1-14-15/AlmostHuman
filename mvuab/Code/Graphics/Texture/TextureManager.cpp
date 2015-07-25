#include "TextureManager.h"

#include "EngineManagers.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"


CTextureManager::CTextureManager()
    : CManager()
{
}

CTextureManager::CTextureManager(CXMLTreeNode& atts)
    : CManager(atts)
{
}

CTextureManager::~CTextureManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(it->second);

    m_Resources.clear();
    l_AllocatorManger->m_pFreeListAllocator->MakeDelete(m_DummyTexture);
}

void CTextureManager::Init()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();
    m_DummyTexture = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CTexture>();

    if(!m_DummyTexture->Load("Data/textures/Dummy.png"))
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(m_DummyTexture);
        LOG_ERROR_APPLICATION("The Dummy texture could not be loaded");
        m_DummyTexture = 0;
    }
}

void CTextureManager::Reload()
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();

    for(; itb != ite; ++itb)
    {
        itb->second->Reload();
    }
}

CTexture* CTextureManager::GetTexture(const std::string& fileName)
{
    if(fileName == "")
    {
        return m_DummyTexture;
    }
    else if(m_Resources.find(fileName) == m_Resources.end())
    {
        CTexture* t = 0;
        CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

        if(fileName.find("Cube") != std::string::npos)
        {
            t = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CCubedTexture>();
        }
        else if(fileName.find("GUI") != std::string::npos)
        {
            t = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CGUITexture>();
        }
        else
        {
            t = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CTexture>();
        }

        if(!t->Load(fileName))
        {
            l_AllocatorManger->m_pFreeListAllocator->MakeDelete(t);
            t = 0;

            LOG_ERROR_APPLICATION("The texture %s could not be loaded", fileName.c_str());
            return m_DummyTexture;
        }

        m_Resources[fileName] = t;
        return t;
    }
    else
    {
        return m_Resources[fileName];
    }
}
