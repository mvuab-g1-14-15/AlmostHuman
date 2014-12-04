#pragma once
#ifndef INC_INSTANCE_MESH_H_
#define INC_INSTANCE_MESH_H_

#include "GraphicsManager.h"
#include "RenderableObject\RenderableObject.h"
#include "StaticMesh.h"
#include "Utils\Defines.h"

#include <string>

class CInstanceMesh : public CRenderableObject
{
private:
	CStaticMesh* m_pStaticMesh;
public:
	CInstanceMesh(const std::string &Name, const std::string &CoreName);
	~CInstanceMesh();
	void Render(CGraphicsManager *GM);
	GET_SET(CStaticMesh*, pStaticMesh);
};

#endif //INC_INSTANCE_MESH_H_
