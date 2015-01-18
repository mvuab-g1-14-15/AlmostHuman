#include "RenderableObject\PoolRenderableObjectTechnique.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
  
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
    Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
    for(uint32 i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i)
    {
        CHECKED_DELETE(m_RenderableObjectTechniqueElements[i]);
    }
    m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, 
    CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
    CXMLTreeNode nodo = CXMLTreeNode::CXMLTreeNode();
    nodo.WritePszProperty("name", TechniqueName.c_str());
    CPoolRenderableObjectTechniqueElement* PoolRenderableObjectTechniqueElement = new CPoolRenderableObjectTechniqueElement(Name, new CEffectTechnique(nodo),ROTOnRenderableObjectTechniqueManager); 
    m_RenderableObjectTechniqueElements.push_back(PoolRenderableObjectTechniqueElement);
}

void CPoolRenderableObjectTechnique::Apply()
{
    //No me queda claro como va exactamente
}