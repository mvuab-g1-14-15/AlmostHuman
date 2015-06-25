#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "EngineManagers.h"
#include <sstream>

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode& TreeNode) : CName(
        TreeNode.GetAttribute<std::string>("name", ""))
{
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
    Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
    for (size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i )
    {
        CHECKED_DELETE( m_RenderableObjectTechniqueElements[i] );
    }
    if (m_RenderableObjectTechniqueElements.size() != 0)
    {
        m_RenderableObjectTechniqueElements.clear();
    }
}

void CPoolRenderableObjectTechnique::AddElement(const std::string& Name, const std::string& TechniqueName,
        CRenderableObjectTechnique* ROTOnRenderableObjectTechniqueManager)
{
    CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement =
        new CPoolRenderableObjectTechniqueElement( Name,
                EffectManagerInstance->GetResource( TechniqueName ),
                ROTOnRenderableObjectTechniqueManager );
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

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(
    const std::string& Name, CEffectTechnique* EffectTechnique,
    CRenderableObjectTechnique* OnRenderableObjectTechniqueManager)
{
    m_RenderableObjectTechnique = CRenderableObjectTechnique(Name, EffectTechnique);
    m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
}