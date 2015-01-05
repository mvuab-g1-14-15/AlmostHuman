
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
    CGraphicsManager::GetSingletonPtr()->SetTransform(GetTransform());
    if(m_pStaticMesh)
    {
        m_pStaticMesh->Render(CGraphicsManager::GetSingletonPtr());
        
        m_NumVertexs   = m_pStaticMesh->GetNumVertex();
        m_NumFaces     = m_pStaticMesh->GetNumFaces();
        m_NumDraws     = m_pStaticMesh->GetNumDraws();
    }
    Math::Mat44f t;
    CGraphicsManager::GetSingletonPtr()->SetTransform(t);
}