#ifndef MAV_MESH_H
#define MAV_MESH_H

#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"

#include <vector>
#include "Utils\Types.h"

class CStaticMesh;
class CRenderableVertexs;

class CNavMesh
{
public:
	CNavMesh();
	~CNavMesh();

	void AddMesh( const CStaticMesh* aStaticMesh );

private:
	std::vector<CRenderableVertexs*> m_RVs;

	std::vector<float> m_VB;
	std::vector<int> m_IB;
};

#endif // MAV_MESH_H
