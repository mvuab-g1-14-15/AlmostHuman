#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "LensFlare.h"

#include "EngineConfig.h"
#include "EngineManagers.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

CLightManager::CLightManager() : CManager()
{
}

CLightManager::CLightManager(CXMLTreeNode& atts): CManager(atts), mAmbientLightColor(Math::Vect3f(0.0f, 0.0f, 0.0f))
{

}

CLightManager::~CLightManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(std::vector<CLight *>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); it++)
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(*it);

    m_ResourcesMap.clear();
    m_ResourcesVector.clear();
}

void CLightManager::Init()
{
    Load(mConfigPath);
}

bool CLightManager::Load(const std::string& FileName)
{
    CXMLTreeNode lXmlFile, lLightsNode;
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    if(!lXmlFile.LoadAndFindNode(mConfigPath.c_str(), "lights", lLightsNode))
    {
        LOG_ERROR_APPLICATION("Error loading Lights %s.", FileName.c_str());
        return false;
    }

    for(uint32 i = 0, lCount = lLightsNode.GetNumChildren() ; i < lCount; ++i)
    {
        const CXMLTreeNode& lCurrentNode = lLightsNode(i);
        const std::string& lCurrentNodeName = lCurrentNode.GetName();

        CLight *lNewLight = 0;

        if(lCurrentNodeName == "omni_light")
        {
            lNewLight = (COmniLight *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(COmniLight), __alignof(COmniLight));
            new (lNewLight) COmniLight(lCurrentNode);
        }
        else if(lCurrentNodeName == "directional_light")
        {
            lNewLight = (CDirectionalLight *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(CDirectionalLight), __alignof(CDirectionalLight));
            new (lNewLight) CDirectionalLight(lCurrentNode);
        }
        else if(lCurrentNodeName == "spot_light")
        {
            lNewLight = (CSpotLight *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(CSpotLight), __alignof(CSpotLight));
            new (lNewLight) CSpotLight(lCurrentNode);
        }
        else if(lCurrentNodeName == "ambient_light")
        {
            mAmbientLightColor = lCurrentNode.GetAttribute<Math::Vect3f>("color", Math::Vect3f(0.0f, 0.0f, 0.0f));
        }
        else if(lCurrentNodeName == "lens_flare")
        {
            CLensFlare *lLensFlare = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CLensFlare>();

            if(lLensFlare->Init(lCurrentNode))
            {
                mLensFlares.AddResource(lLensFlare->GetName(), lLensFlare);
            }
            else
            {
                l_AllocatorManger->m_pFreeListAllocator->MakeDelete(lLensFlare);
                LOG_ERROR_APPLICATION("Error creating lens flare %s", lLensFlare->GetName().c_str());
            }
        }

        if(lNewLight && !AddResource(lNewLight->GetName(), lNewLight))
        {
            l_AllocatorManger->m_pFreeListAllocator->MakeDelete(lNewLight);
        }
    }

    return true;
}

Math::Vect3f& CLightManager::GetAmbientLight()
{
    return mAmbientLightColor;
}

void CLightManager::Render()
{
#ifdef _DEBUG
    TVectorResources::iterator itb = m_ResourcesVector.begin(), ite = m_ResourcesVector.end();

    for(; itb != ite; ++itb)
    {
        (*itb)->Render();
    }

#endif
}

CLight *CLightManager::GetLight(size_t at)
{
    return GetResourceById(at);
}

size_t  CLightManager::GetLightCount()
{
    return m_ResourcesVector.size();
}

void CLightManager::GenerateShadowMap(CGraphicsManager *GM)
{
    TVectorResources::iterator itb = m_ResourcesVector.begin(), ite = m_ResourcesVector.end();

    for(; itb != ite; ++itb)
    {
        (*itb)->GenerateShadowMap(GM);
    }
}

bool CLightManager::ReLoad()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(std::vector<CLight *>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); it++)
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(*it);

    m_ResourcesMap.clear();
    m_ResourcesVector.clear();

    return Load(mConfigPath);
}