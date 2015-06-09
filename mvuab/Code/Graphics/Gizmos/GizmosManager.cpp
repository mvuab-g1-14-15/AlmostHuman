#include "Gizmos/GizmosManager.h"
#include "Gizmos/Gizmo.h"
#include "Gizmos/GizmoElement.h"

CGizmosManager::CGizmosManager()
  :CManager()
{

}

CGizmosManager::CGizmosManager(CXMLTreeNode& atts)
  :CManager(atts)
{
	/*TODO RAUL
	PONER LECTURA XML
	*/
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
	CGizmoElement* l_GizmoElement = new CGizmoElement( type, size, position, yaw, pitch, color );
	l_GizmoElement->Init();
	return l_GizmoElement;
}

CGizmo* CGizmosManager::CreateGizmo( std::string name, Math::Vect3f position, float yaw, float pitch )
{
	CGizmo* l_Gizmo = new CGizmo( name, position, yaw, pitch );
	l_Gizmo->Init();
	return l_Gizmo;
}
