#pragma once
#ifndef RENDERABLE_OBJECT_LAYERS_MANAGER_H
#define RENDERABLE_OBJECT_LAYERS_MANAGER_H

#include "RenderableObject\RenderableObjectsManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\SingletonPattern.h"

class CGraphicsManager;

class CRenderableObjectsLayersManager : public
  CTemplatedVectorMapManager<CRenderableObjectsManager>,
  public CSingleton< CRenderableObjectsLayersManager >
{
private:
  std::string m_FileName;
  CRenderableObjectsManager* m_DefaultRenderableObjectManager;
  CRenderableObjectsManager* GetRenderableObjectManager( CXMLTreeNode& Node );
public:
  CRenderableObjectsLayersManager();
  ~CRenderableObjectsLayersManager();
  void Destroy();
  void Load( const std::string& FileName );
  void Reload();
  void Update();
  void Render();
  void Render( const std::string& LayerName );
};
#endif