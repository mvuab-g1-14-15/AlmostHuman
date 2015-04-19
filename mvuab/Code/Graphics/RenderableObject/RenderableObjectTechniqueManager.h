#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHINQUE_MANAGER_H

#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Utils\MapManager.h"
#include "Utils\Name.h"
#include "Utils\Manager.h"

class CRenderableObjectTechniqueManager : public CMapManager<CRenderableObjectTechnique>,
  public CManager
{
public:
  CRenderableObjectTechniqueManager();
  CRenderableObjectTechniqueManager( CXMLTreeNode& atts);
  virtual ~CRenderableObjectTechniqueManager();
  void Destroy();
  void Init();
  void Update(){}
  void Render(){}
  void ReLoad();
  std::string GetRenderableObjectTechniqueNameByVertexType( uint32 VertexType );
  CMapManager<CPoolRenderableObjectTechnique>& GetPoolRenderableObjectTechniques()
  {
    return m_PoolRenderableObjectTechniques;
  }
private:
  CMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
private:
  void InsertRenderableObjectTechnique( const std::string& ROTName,
                                        const std::string& TechniqueName );
};

#endif