#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:
  float m_Angle;
  float m_FallOff;
public:
  CSpotLight( CXMLTreeNode node );
  void SetAngle( const float Angle );
  float GetAngle() const;
  void SetFallOff( const float FallOff );
  float GetFallOff() const;
  virtual void Render();
  virtual void SetShadowMap( CGraphicsManager* GM );
};

#endif // SPOT_LIGHT_H
