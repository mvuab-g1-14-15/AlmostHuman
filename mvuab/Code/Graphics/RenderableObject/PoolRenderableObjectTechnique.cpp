#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"
CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
    SetName(TreeNode.GetPszProperty("name", ""));
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
    Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
    for(size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i)
    {
        CHECKED_DELETE(m_RenderableObjectTechniqueElements[i]);
    }
    m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, 
    CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
    CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement = 
		new CPoolRenderableObjectTechniqueElement(Name, CEffectManager::GetSingletonPtr()->GetResource(TechniqueName),
			ROTOnRenderableObjectTechniqueManager); 

    m_RenderableObjectTechniqueElements.push_back(PoolRenderableObjectTechniqueElement);
}

void CPoolRenderableObjectTechnique::Apply()
{
	size_t l_Count = m_RenderableObjectTechniqueElements.size();
	for(size_t i = 0; i<l_Count; ++i)
	{
		CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement *l_PoolRenderableObjectTechniqueElement=m_RenderableObjectTechniqueElements[i];
		l_PoolRenderableObjectTechniqueElement->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(l_PoolRenderableObjectTechniqueElement->m_RenderableObjectTechnique.GetEffectTechnique());

	}
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement
	(const std::string &Name, CEffectTechnique *EffectTechnique, 
	CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
{
	m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
	m_RenderableObjectTechnique.SetEffectTechnique(EffectTechnique);
	m_RenderableObjectTechnique.SetName(Name);
}