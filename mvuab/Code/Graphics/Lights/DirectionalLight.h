#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CBillboardInstance;

class CDirectionalLight : public CLight
{
public:
  CDirectionalLight( const CXMLTreeNode& node );
  void SetDirection( const Math::Vect3f& Direction );
  Math::Vect3f GetDirection() const;
  virtual void Render();
  virtual void SetShadowMap( CGraphicsManager* GM );

protected:
  CBillboardInstance* mLookAtBillboard;
  Math::Vect3f m_Direction;
  Math::Vect3f mLookAt;
  Math::Vect2f m_OrthoShadowMapSize;
};

#endif // DIRECTIONAL_LIGHT_H
