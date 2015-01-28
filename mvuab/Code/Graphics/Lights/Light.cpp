#include "Light.h"
#include "Math\Color.h"
#include "GraphicsManager.h"

#include <string>

CLight::CLight( const CXMLTreeNode& node )
  : CObject3D()
  , m_StartRangeAttenuation(node.GetFloatProperty("att_start_range", 0))
  , m_EndRangeAttenuation(node.GetFloatProperty("att_end_range", 0))
  , m_Color(node.GetVect3fProperty("color", Math::Vect3f(1, 1, 1)))
  , m_Intensity(node.GetFloatProperty("intensity", 0))
{
  m_Position = node.GetVect3fProperty("pos", Math::Vect3f(0, 0, 0));
  SetName(node.GetPszProperty("name",""));
}

CLight::~CLight()
{
}

void CLight::SetIntensity( const float intensity )
{
  m_Intensity = intensity;
}

float CLight::GetIntensity() const
{
  return m_Intensity;
}

void CLight::SetType( const TLightType Type )
{
  m_Type = Type;
}

void CLight::SetColor( const Math::CColor& Color )
{
  m_Color = Color;
}

const Math::CColor& CLight::GetColor() const
{
  return m_Color;
}

void CLight::SetStartRangeAttenuation( const float StartRangeAttenuation )
{
  m_StartRangeAttenuation = StartRangeAttenuation;
}

float CLight::GetStartRangeAttenuation() const
{
  return m_StartRangeAttenuation;
}

void CLight::SetEndRangeAttenuation( const float EndRangeAttenuation )
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