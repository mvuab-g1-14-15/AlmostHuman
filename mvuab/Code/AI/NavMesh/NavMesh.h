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

	void AddMesh( Math::Vect3f, const CStaticMesh* aStaticMesh );

	void Calculate();

	float m_sizeX;
	float m_sizeY;
	float m_sizeZ;

	Math::Vect3f m_Pos;

private:
	std::vector<CRenderableVertexs*> m_RVs;

	std::vector<float> m_VB;
	std::vector<int> m_IB;

	Math::Vect3f m_BBMin;
	Math::Vect3f m_BBMax;

	int m_MaxIndex;

	void RecalculateBB( Math::Vect3f );
};

#endif // MAV_MESH_H
