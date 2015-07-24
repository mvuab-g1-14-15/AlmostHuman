#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Utils\MapManager.h"

#include "Effects\EffectManager.h"
#include "EngineManagers.h"
#include "EngineConfig.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#include <map>
#include <sstream>

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager() : CManager()
{
}

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager(CXMLTreeNode& atts) : CManager(atts)
{
}
CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
    Destroy();
}

void CRenderableObjectTechniqueManager::Destroy()
{
    std::map<std::string, CPoolRenderableObjectTechnique *> l_Resources = m_PoolRenderableObjectTechniques.GetResourcesMap();
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    std::map<std::string, CPoolRenderableObjectTechnique *>::iterator itb1 = l_Resources.begin(), ite1 = l_Resources.end();
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();

    for (; itb1 != ite1; ++itb1) l_AllocatorManger->m_pFreeListAllocator->MakeDelete(itb1->second);
    for (; itb != ite; ++itb) l_AllocatorManger->m_pFreeListAllocator->MakeDelete(itb->second);

    m_Resources.clear();
    l_Resources.clear();
}

void CRenderableObjectTechniqueManager::Init()
{
    Load(mConfigPath.c_str());
}

void CRenderableObjectTechniqueManager::Load(const std::string& aFile)
{
    CXMLTreeNode TreeNode, lPoolXml;
    if(lPoolXml.LoadAndFindNode(aFile.c_str(), "renderable_object_techniques", TreeNode))
    {
        CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

        for(uint32 i = 0, lCount = TreeNode.GetNumChildren(); i < lCount; ++i)
        {
            CXMLTreeNode& l_PoolNode = TreeNode(i);
            const std::string& TagName = l_PoolNode.GetName();

            if(TagName == "pool_renderable_object_technique")
            {
                CPoolRenderableObjectTechnique* PoolRenderableObjectTechnique = (CPoolRenderableObjectTechnique *) l_AllocatorManager->m_pFreeListAllocator->Allocate(sizeof(CPoolRenderableObjectTechnique), __alignof(CPoolRenderableObjectTechnique));
                new (PoolRenderableObjectTechnique) CPoolRenderableObjectTechnique(l_PoolNode);
 
                if(!m_PoolRenderableObjectTechniques.AddResource(PoolRenderableObjectTechnique->GetName(), PoolRenderableObjectTechnique))
                {
                    l_AllocatorManager->m_pFreeListAllocator->MakeDelete(PoolRenderableObjectTechnique);
                    continue;
                }

                for(int j = 0, lPoolCount = l_PoolNode.GetNumChildren(); j < lPoolCount; ++j)
                {
                    const std::string& SubTagName = l_PoolNode(j).GetName();
                    if(SubTagName == "default_technique")
                    {
                        const  std::string&  l_VertexTypeStr = GetRenderableObjectTechniqueNameByVertexType(l_PoolNode(j).GetAttribute<int32>("vertex_type", 0));
                        const std::string& l_TechniqueName = l_PoolNode(j).GetAttribute<std::string>("technique", "");

                        InsertRenderableObjectTechnique(l_VertexTypeStr , l_TechniqueName);
                        PoolRenderableObjectTechnique->AddElement(l_VertexTypeStr, l_TechniqueName, GetResource(l_VertexTypeStr));
                    }
                }


            }
            else if(TagName == "pool")
            {
                Load(l_PoolNode.GetAttribute<std::string>("file", "no_file"));
            }
        }
    }
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType)
{
    std::stringstream l_VertexType;
    l_VertexType << "DefaultROTTechnique_" << VertexType;

    return l_VertexType.str();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string& aROTName, const std::string& aTechniqueName)
{
    // Only store the default renderable objects techniques, therefore check if they exist before inserting them
    if(!Exist(aROTName))
    {
        CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

        CRenderableObjectTechnique* lRenderableObjectTechnique = (CRenderableObjectTechnique *) l_AllocatorManager->m_pFreeListAllocator->Allocate(sizeof(CRenderableObjectTechnique), __alignof(CRenderableObjectTechnique));
        new (lRenderableObjectTechnique)  CRenderableObjectTechnique(aROTName, EffectManagerInstance->GetResource(aTechniqueName));

        if(!AddResource(aROTName, lRenderableObjectTechnique))
        {
            l_AllocatorManager->m_pFreeListAllocator->MakeDelete(lRenderableObjectTechnique);
        }
    }
}

void CRenderableObjectTechniqueManager::ReLoad()
{
    Destroy();
    Init();
}