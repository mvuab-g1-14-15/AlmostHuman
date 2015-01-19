#include "SpotLight.h"

CSpotLight::CSpotLight(CXMLTreeNode node) : CDirectionalLight(node)
{
	m_Angle = node.GetFloatProperty("angle", 0);
    m_FallOff = node.GetFloatProperty("falloff", 0);
	SetType(CLight::SPOT);
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