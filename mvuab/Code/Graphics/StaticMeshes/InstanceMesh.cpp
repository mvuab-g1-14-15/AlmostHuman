
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
}

void CInstanceMesh::Render(CGraphicsManager *GM)
{
	Mat44f t, rot, pos;
	t.SetIdentity();
	rot.SetIdentity();
	pos.SetIdentity();

	rot.RotByAnglesYXZ(m_fYaw, m_fPitch, m_fRoll);
	pos.Translate(m_Position);

	t = pos*rot;

	GM->SetTransform(t);
	if(m_pStaticMesh)
		m_pStaticMesh->Render(GM);

	t.SetIdentity();
	GM->SetTransform(t);
}