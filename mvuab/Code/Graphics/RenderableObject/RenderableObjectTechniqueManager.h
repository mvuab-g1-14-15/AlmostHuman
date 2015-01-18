#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHINQUE_MANAGER_H

#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Utils\MapManager.h"
#include "Utils\Name.h"

class CRenderableObjectTechniqueManager : public CMapManager<CRenderableObjectTechnique>
{

public:
    CRenderableObjectTechniqueManager();
    virtual ~CRenderableObjectTechniqueManager();
    void Destroy();
    void Load(const std::string &FileName);
    std::string GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType);
    CMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques(){ return m_PoolRenderableObjectTechniques; }
private:
    CMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
private:
    void InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName, uint32 VertexType);
};

#endif