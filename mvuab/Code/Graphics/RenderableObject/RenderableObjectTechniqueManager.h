#pragma once
#ifndef RENDERABLE_OBJECT_TECHINQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHINQUE_MANAGER_H

#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableObject\PoolRenderableObjectTechnique.h"
#include "Utils\MapManager.h"
#include "Utils\Name.h"
#include "Utils\SingletonPattern.h"

class CRenderableObjectTechniqueManager : public CMapManager<CRenderableObjectTechnique>,
  public CSingleton<CRenderableObjectTechniqueManager >
{

public:
  CRenderableObjectTechniqueManager();
  virtual ~CRenderableObjectTechniqueManager();
  void Destroy();
  void Load( const std::string& FileName );
  void ReLoad();
  std::string GetRenderableObjectTechniqueNameByVertexType( uint32 VertexType );
  CMapManager<CPoolRenderableObjectTechnique>& GetPoolRenderableObjectTechniques()
  {
    return m_PoolRenderableObjectTechniques;
  }
private:
  CMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
  std::string m_FileName;
private:
  void InsertRenderableObjectTechnique( const std::string& ROTName,
                                        const std::string& TechniqueName );
};

#endif