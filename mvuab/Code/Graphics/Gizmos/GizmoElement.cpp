#include "Gizmos/GizmoElement.h"

#include "Core.h"
#include "GraphicsManager.h"

#include "Math/Vector3.h"
#include "Math/Matrix44.h"

CGizmoElement::CGizmoElement( EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color )
  : CObject3D( position, yaw, pitch )
  , m_Type( type )
  , m_Size( size )
  , m_Color( color )
{
  MakeTransform();
}

CGizmoElement::~CGizmoElement()
{

}

void CGizmoElement::Init()
{

}

void CGizmoElement::Update()
{
  MakeTransform();
}

void CGizmoElement::Render( Math::Mat44f transform )
{
  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();

  Math::Mat44f t = GetTransform();
  l_GM->SetTransform( t * transform );

  if ( m_Type == eCube )
    l_GM->DrawCube( m_Size, m_Color );

  if ( m_Type == eSphere )
    l_GM->DrawSphere( m_Size, m_Color );

  t.SetIdentity();
  l_GM->SetTransform( t );
}
