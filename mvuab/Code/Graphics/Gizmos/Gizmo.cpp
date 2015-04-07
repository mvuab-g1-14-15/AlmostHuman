#include "Gizmos/Gizmo.h"

#include "Core.h"
#include "GraphicsManager.h"

#include "Math/Matrix44.h"

#include "GizmoElement.h"

CGizmo::CGizmo( std::string name, Math::Vect3f position, float yaw, float pitch )
  : CObject3D( position, yaw, pitch )
  , CName( name )
{

}

CGizmo::~CGizmo()
{
  std::vector<CGizmoElement*>::iterator it = m_Elements.begin(),
    it_end = m_Elements.end();

  for ( ; it != it_end; ++it )
    CHECKED_DELETE(*it);

  m_Elements.clear();
}

void CGizmo::Init()
{

}

void CGizmo::Update()
{
  std::vector<CGizmoElement*>::iterator it = m_Elements.begin(),
    it_end = m_Elements.end();

  for ( ; it != it_end; ++it )
    ( *it )->Update();

  MakeTransform();
}

void CGizmo::Render()
{
  CGraphicsManager* l_GM = GraphicsInstance;

  Math::Mat44f t = GetTransform();

  std::vector<CGizmoElement*>::iterator it = m_Elements.begin(),
                                        it_end = m_Elements.end();

  for ( ; it != it_end; ++it )
    ( *it )->Render( t );

  t.SetIdentity();
  l_GM->SetTransform( t );
}

void CGizmo::AddElement(CGizmoElement* element)
{
  m_Elements.push_back(element);
}
