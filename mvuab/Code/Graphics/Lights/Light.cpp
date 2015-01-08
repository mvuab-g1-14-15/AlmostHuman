#include "Light.h"
#include "Math\Color.h"

#include <string>

CLight::CLight()
{
}

CLight::~CLight()
{
}

void CLight::SetIntensity(const float intensity)
{
	m_Intensity = intensity;
}

float CLight::GetIntensity() const
{
	return m_Intensity;
}

void CLight::SetType(const TLightType Type)
{
	m_Type = Type;
}

void CLight::SetName(const std::string &Name)
{
	m_Name = Name;
}

const std::string &CLight::GetName()
{
	return m_Name;
}

void CLight::SetColor(const Math::CColor &Color)
{
	m_Color = Color;
}

const Math::CColor& CLight::GetColor() const
{
	return m_Color;
}

void CLight::SetStartRangeAttenuation(const float StartRangeAttenuation)
{
	m_StartRangeAttenuation = StartRangeAttenuation;
}

float CLight::GetStartRangeAttenuation() const
{
	return m_StartRangeAttenuation;
}

void CLight::SetEndRangeAttenuation(const float EndRangeAttenuation)
{
	m_EndRangeAttenuation = EndRangeAttenuation;
}

float CLight::GetEndRangeAttenuation() const
{
	return m_EndRangeAttenuation;
}

bool CLight::RenderShadows() const
{
	return false;
}

CLight::TLightType CLight::GetType() const
{
	return m_Type;
}

void CLight::Render()
{
}