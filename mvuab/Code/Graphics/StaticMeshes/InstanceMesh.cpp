
#include "InstanceMesh.h"
#include "Core.h"
#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"

#include "Core.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Cameras/CameraManager.h"
#include "Math\AABB.h"

CInstanceMesh::CInstanceMesh( const std::string& Name,
                              const std::string& CoreName ): m_pStaticMesh( CStaticMeshManager::GetSingletonPtr()->GetResource(
                                      CoreName ) ), CRenderableObject()
{
  if ( !m_pStaticMesh )
    LOG_ERROR_APPLICATION(
                                           "CInstanceMesh::CInstanceMesh No se puede instanciar m_pStaticMesh" );

  SetName( Name );
}
CInstanceMesh::CInstanceMesh( CXMLTreeNode& atts )
  : CRenderableObject( atts ), m_pStaticMesh( CStaticMeshManager::GetSingletonPtr()->GetResource(
        atts.GetPszProperty( "core", "no_staticMesh" ) ) )
{
}


CInstanceMesh::~CInstanceMesh()
{
}

void CInstanceMesh::Render()
{
  if ( m_pStaticMesh )
  {
    Math::Mat44f lTransform = GetTransform();
    CGraphicsManager::GetSingletonPtr()->SetTransform( lTransform );
    Math::AABB3f laabb = m_pStaticMesh->GetAABB();
    CFrustum lCameraFrustum = CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera()->GetFrustum();
    Math::Vect3f lPositionTransformed = lTransform * laabb.GetCenter();
    // TODO: Fix the frustum culling
    //if(lCameraFrustum.SphereVisible( D3DXVECTOR3(lPositionTransformed.x, lPositionTransformed.y, lPositionTransformed.z), laabb.GetRadius()) )
      m_pStaticMesh->Render( CGraphicsManager::GetSingletonPtr() );
    Math::Mat44f t;
    CGraphicsManager::GetSingletonPtr()->SetTransform( t );
  }
}