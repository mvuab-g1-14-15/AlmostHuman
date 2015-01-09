#include "RenderableObjectTechniqueManager.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{

}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
    Destroy();
}

void CRenderableObjectTechniqueManager::Destroy()
{
    m_PoolRenderableObjectTechniques.Destroy();
}

void CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{

}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType)
{
    //CMapManager<CPoolRenderableObjectTechnique> 
    return NULL;
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{

}