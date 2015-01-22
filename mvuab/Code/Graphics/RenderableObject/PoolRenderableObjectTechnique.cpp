#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Effects\EffectManager.h"
CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
  
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
		m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->
			SetEffectTechnique(m_RenderableObjectTechniqueElements[i]->
				m_RenderableObjectTechnique.GetEffectTechnique());

		m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->
			SetName(m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetName());
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