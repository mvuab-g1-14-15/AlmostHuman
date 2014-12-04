
#include "InstanceMesh.h"
#include "Core.h"
#include "StaticMeshManager.h"


CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName):
	m_pStaticMesh(0)
{
	m_pStaticMesh = CCore::GetSingletonPtr()->GetpStaticMeshManager()->GetResource(CoreName);
	SetName(Name);
}
	
CInstanceMesh::~CInstanceMesh()
{
	CHECKED_DELETE(m_pStaticMesh);
}

void CInstanceMesh::Render(CGraphicsManager *GM)
{
	m_pStaticMesh->Render(GM);
}