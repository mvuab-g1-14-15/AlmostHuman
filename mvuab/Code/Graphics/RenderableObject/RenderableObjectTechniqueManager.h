#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHINQUE_MANAGER_H

#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Utils\MapManager.h"
#include "Utils\Name.h"

class CRenderableObjectTechniqueManager : public CMapManager<CRenderableObjectTechnique>
{
private:
    CMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
    void InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName);
public:
    CRenderableObjectTechniqueManager();
    virtual ~CRenderableObjectTechniqueManager();
    void Destroy();
    void Load(const std::string &FileName);
    std::string GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType);
    CMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques(){ return m_PoolRenderableObjectTechniques; }
};

#endif