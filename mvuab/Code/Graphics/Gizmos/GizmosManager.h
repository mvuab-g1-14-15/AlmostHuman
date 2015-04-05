#ifndef GIZMOS_MANAGER_H
#define GIZMOS_MANAGER_H

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"

#include "Utils/TemplatedVectorMapManager.h"
#include "Gizmo.h"
#include "GizmoElement.h"

class CGizmosManager : public CSingleton<CGizmosManager>, public CTemplatedVectorMapManager<CGizmo>
{
public:
  CGizmosManager();
  virtual ~CGizmosManager();

  void Init();
  void Update();
  void Render();

  CGizmoElement* CreateGizmoElement( CGizmoElement::EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch );
  CGizmo* CreateGizmo( std::string name, Math::Vect3f position, float yaw, float pitch );
};

#endif // GIZMOS_MANAGER_H
