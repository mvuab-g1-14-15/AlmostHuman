
#include "InstanceMesh.h"
#include "Core.h"
#include "StaticMeshManager.h"


CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName): 
    m_pStaticMesh(CStaticMeshManager::GetSingletonPtr()->GetResource(CoreName)), m_NumFaces(0), m_NumVertexs(0), m_NumDraws(0)
{
    if(!m_pStaticMesh)
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CInstanceMesh::CInstanceMesh No se puede instanciar m_pStaticMesh");

    SetName(Name);
}
    
CInstanceMesh::~CInstanceMesh()
{
}

void CInstanceMesh::Render()
{
  CGraphicsManager *GM = CGraphicsManager::GetSingletonPtr();

  Math::Mat44f t, rot, pos;

  t.SetIdentity();
  rot.SetIdentity();
  pos.SetIdentity();

  rot.RotByAnglesYXZ(m_fYaw, m_fPitch, m_fRoll);
  pos.Translate(m_Position);

  t = pos * rot;

  GM->SetTransform(t);
  if(0 != m_pStaticMesh)
  {
    m_pStaticMesh->Render(GM);

    m_NumVertexs = m_pStaticMesh->GetNumVertex();
    m_NumFaces     = m_pStaticMesh->GetNumFaces();
    m_NumDraws     = m_pStaticMesh->GetNumDraws();
  }

  t.SetIdentity();
  GM->SetTransform(t);
}