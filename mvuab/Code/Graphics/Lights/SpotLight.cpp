#include "SpotLight.h"

CSpotLight::CSpotLight()
{
}

void CSpotLight::SetFallOff(const float FallOff)
{
  m_FallOff = FallOff;
}

void CSpotLight::SetAngle(const float Angle)
{
  m_Angle = Angle;
}

float CSpotLight::GetAngle() const
{
  return m_Angle;
}

float CSpotLight::GetFallOff() const
{
  return m_FallOff;
}