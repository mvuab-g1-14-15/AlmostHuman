#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CDirectionalLight : public CLight
{
public:
  CDirectionalLight( const CXMLTreeNode& node );
  void SetDirection( const Math::Vect3f& Direction );
  Math::Vect3f GetDirection() const;
  virtual void Render();

protected:
  Math::Vect3f m_Direction;
};

#endif // DIRECTIONAL_LIGHT_H
