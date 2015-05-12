#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "InstanceMesh.h"

#include "Math\AABB.h"
#include "GraphicsManager.h"

#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"

#include "EngineManagers.h"

CInstanceMesh::CInstanceMesh( const std::string& Name, const std::string& CoreName ) : m_pStaticMesh( SMeshMInstance->GetResource(CoreName ) ), CRenderableObject()
{
    if ( !m_pStaticMesh )
    { LOG_ERROR_APPLICATION( "CInstanceMesh::CInstanceMesh No se puede instanciar m_pStaticMesh" ); }

    if(m_pStaticMesh != 0)
    {
        std::vector<Math::Vect3f> m_VB = m_pStaticMesh->GetVB();
        Math::Mat44f l_Transform = GetTransform();
        int l_VecElements = m_VB.size();

        #pragma omp parallel for
        for(int i = 0; i < l_VecElements; ++i)
        {
            m_VB[i] = l_Transform * m_VB[i];
        }

        m_pStaticMesh->SetVB(m_VB);
    }

    SetName( Name );
}

CInstanceMesh::CInstanceMesh( CXMLTreeNode& atts ) : CRenderableObject( atts ), m_pStaticMesh( SMeshMInstance->GetResource(atts.GetPszProperty( "core", "no_staticMesh" ) ) )
{
    if(m_pStaticMesh != 0)
    {
        std::vector<Math::Vect3f> m_VB = m_pStaticMesh->GetVB();
        Math::Mat44f l_Transform = GetTransform();
        int l_VecElements = m_VB.size();

        #pragma omp parallel for
        for(int i = 0; i < l_VecElements; ++i)
        {
            m_VB[i] = l_Transform * m_VB[i];
        }

        m_pStaticMesh->SetVB(m_VB);
    }
}


CInstanceMesh::~CInstanceMesh()
{
}

void CInstanceMesh::Render()
{
    if (!m_pStaticMesh) { return; }

    Math::Mat44f lTransform = GetTransform();
    GraphicsInstance->SetTransform( lTransform );
    Math::AABB3f laabb = m_pStaticMesh->GetAABB();

    CFrustum lCameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
    Math::Vect3f lPositionTransformed = lTransform * laabb.GetCenter();

    // TODO: Fix the frustum culling
    if(lCameraFrustum.SphereVisible( D3DXVECTOR3(lPositionTransformed.x, lPositionTransformed.y, lPositionTransformed.z), laabb.GetRadius()) )
    { m_pStaticMesh->Render( GraphicsInstance ); }

    Math::Mat44f t;
    GraphicsInstance->SetTransform( t );
}