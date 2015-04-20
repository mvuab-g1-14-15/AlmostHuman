#ifndef MAV_MESH_MANAGER_H
#define MAV_MESH_MANAGER_H

#include "XML\XMLTreeNode.h"

#include "Utils\Manager.h"

class CNavMesh;
class CNavMeshManager: public CManager
{
public:
	CNavMeshManager(const CXMLTreeNode& atts);
	virtual ~CNavMeshManager();

	virtual void Init();
	virtual void Update();
	virtual void Render();

private:
	CNavMesh* m_NavMesh;

};

#endif // MAV_MESH_MANAGER_H
