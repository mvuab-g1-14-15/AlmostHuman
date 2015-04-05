#ifndef GIZMO_H
#define GIZMO_H

#include "Object3D.h"
#include "Utils/Name.h"

class CGizmoElement;

class CGizmo : public CObject3D, public CName
{
public:
  CGizmo( std::string name, Math::Vect3f position, float yaw, float pitch );
  virtual ~CGizmo();

  void Init();
  void Update();
  void Render();

  void AddElement( CGizmoElement* element );

private:
  std::vector<CGizmoElement*> m_Elements;
};

#endif // GIZMO_H
