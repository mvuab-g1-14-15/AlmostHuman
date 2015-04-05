#include "Gizmos/GizmosManager.h"
#include "Gizmos/Gizmo.h"
#include "Gizmos/GizmoElement.h"

CGizmosManager::CGizmosManager()
{

}

CGizmosManager::~CGizmosManager()
{

}

void CGizmosManager::Init()
{

}

void CGizmosManager::Update()
{
  std::vector<CGizmo*>::iterator it = m_ResourcesVector.begin(),
    it_end = m_ResourcesVector.end();

  for ( ; it != it_end; ++it )
    ( *it )->Update();
}

void CGizmosManager::Render()
{
  std::vector<CGizmo*>::iterator it = m_ResourcesVector.begin(),
                                 it_end = m_ResourcesVector.end();

  for ( ; it != it_end; ++it )
    ( *it )->Render();
}

CGizmoElement* CGizmosManager::CreateGizmoElement( CGizmoElement::EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color)
{
  return new CGizmoElement( type, size, position, yaw, pitch, color );
}

CGizmo* CGizmosManager::CreateGizmo( std::string name, Math::Vect3f position, float yaw, float pitch )
{
  return new CGizmo( name, position, yaw, pitch );
}
