#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#include "EngineConfig.h"
#include "EngineManagers.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "EngineManagers.h"
#include <sstream>

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode& TreeNode) : CName( TreeNode.GetAttribute<std::string>("name", ""))
{
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
    Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for (size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i )
    {
         l_AllocatorManger->m_pFreeListAllocator->MakeDelete( m_RenderableObjectTechniqueElements[i] );
    }
    
    m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string& Name, const std::string& TechniqueName, CRenderableObjectTechnique* ROTOnRenderableObjectTechniqueManager)
{
    CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement = (CPoolRenderableObjectTechniqueElement *) l_AllocatorManager->m_pFreeListAllocator->Allocate(sizeof(CPoolRenderableObjectTechniqueElement), __alignof(CPoolRenderableObjectTechniqueElement));
    new (PoolRenderableObjectTechniqueElement) CPoolRenderableObjectTechniqueElement(Name, EffectManagerInstance->GetResource(TechniqueName), ROTOnRenderableObjectTechniqueManager);

    m_RenderableObjectTechniqueElements.push_back( PoolRenderableObjectTechniqueElement );
}

void CPoolRenderableObjectTechnique::Apply()
{
    CRenderableObjectTechniqueManager* l_ROTM = ROTMInstance;

    for ( size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i )
    {
        CPoolRenderableObjectTechniqueElement* l_CurrentElement = m_RenderableObjectTechniqueElements[i];
        CRenderableObjectTechnique* l_ROT = l_CurrentElement->m_OnRenderableObjectTechniqueManager;
        l_ROT->SetEffectTechnique( l_CurrentElement->m_RenderableObjectTechnique.GetEffectTechnique() );
    }
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string& Name, CEffectTechnique* EffectTechnique, CRenderableObjectTechnique* OnRenderableObjectTechniqueManager)
{
    m_RenderableObjectTechnique = CRenderableObjectTechnique(Name, EffectTechnique);
    m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
}