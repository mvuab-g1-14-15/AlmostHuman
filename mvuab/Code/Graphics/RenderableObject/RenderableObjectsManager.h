#pragma once

#include "Utils\MapManager.h"
#include "RenderableObject\RenderableObject.h"
#include "GraphicsManager.h"

#include <vector>


class CRenderableObjectsManager : CMapManager<CRenderableObject>
{
	private:
		std::vector<CRenderableObject *> m_RenderableObjects;
	public:
		CRenderableObjectsManager();
		~CRenderableObjectsManager();
		void Update(float ElapsedTime);
		void Render(CGraphicsManager *GM);
		CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const
		std::string &InstanceName, const Vect3f &Position);
		CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName,
		const std::string &InstanceModelName, const Vect3f &Position);
		void AddResource(const std::string &Name, CRenderableObject *RenderableObject);
		void CleanUp();
		void Load(const std::string &FileName);
		CRenderableObject * GetInstance(const std::string &Name) const;
};