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