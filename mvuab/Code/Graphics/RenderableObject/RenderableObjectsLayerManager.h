#pragma once
#ifndef RENDERABLE_OBJECT_LAYER_MANAGER_H
#define RENDERABLE_OBJECT_LAYER_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"


class CGraphicsManager;
class CXMLTreeNode;
class CRenderableObject;
class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
private:
	std::string m_FileName;
	CRenderableObjectsManager *m_DefaultRenderableObjectManager;
	CRenderableObjectsManager * GetRenderableObjectManager(const CXMLTreeNode &Node);
public:
	CRenderableObjectsLayersManager();
	~CRenderableObjectsLayersManager();
	void Load(const std::string &FileName);
	void Unload();
	void Reload();
	void Update(float ElapsedTime);
	void Render(CGraphicsManager *GM);
	void Render(CGraphicsManager *GM, const std::string &LayerName);
	CRenderableObjectsManager *get_DefaultRenderableObjectManager() { return m_DefaultRenderableObjectManager; };

};
#endif