#ifndef GIZMO_ELEMENT_H
#define GIZMO_ELEMENT_H

#include "Object3D.h"
#include "Math\Color.h"

class CGizmoElement : public CObject3D
{
public:
  typedef enum { eCube = 0, eSphere } EGizmoElementType;

  CGizmoElement( EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color = Math::colWHITE );
  virtual ~CGizmoElement();

  void Init();
  void Update();
  void Render( Math::Mat44f transform );

private:
  EGizmoElementType m_Type;

  float m_Size;
  Math::CColor m_Color;
};

#endif // GIZMO_ELEMENT_H
