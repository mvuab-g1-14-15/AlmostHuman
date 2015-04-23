#ifndef GIZMOS_MANAGER_H
#define GIZMOS_MANAGER_H

#include "Utils\Manager.h"
#include "Utils\Defines.h"

#include "Utils/TemplatedVectorMapManager.h"
#include "Gizmo.h"
#include "GizmoElement.h"

class CGizmosManager : public CManager, public CTemplatedVectorMapManager<CGizmo>
{
public:
  CGizmosManager();
  CGizmosManager(CXMLTreeNode& atts);
  virtual ~CGizmosManager();

  void Init();
  void Update();
  void Render();

  CGizmoElement* CreateGizmoElement( CGizmoElement::EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color = Math::colWHITE );
  CGizmo* CreateGizmo( std::string name, Math::Vect3f position, float yaw, float pitch );
};

#endif // GIZMOS_MANAGER_H
