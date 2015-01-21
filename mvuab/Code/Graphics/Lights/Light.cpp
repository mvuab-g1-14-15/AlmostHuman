#include "Light.h"
#include "Math\Color.h"
#include "GraphicsManager.h"

#include <string>

#define D3DFVF_CUSTOMVERTEXLIGHT (D3DFVF_XYZ|D3DFVF_DIFFUSE)
typedef struct CUSTOMVERTEXLIGHT
{
  float x, y, z;
  DWORD color;

  static unsigned int getFlags()
  {
    return ( D3DFVF_CUSTOMVERTEXLIGHT );
  }
} CUSTOMVERTEXLIGHT;

CLight::CLight( const CXMLTreeNode& node )
  : CObject3D()
  , m_StartRangeAttenuation( node.GetFloatProperty( "att_start_range", 0 ) )
  , m_EndRangeAttenuation( node.GetFloatProperty( "att_end_range", 0 ) )
  , m_Color( Math::colWHITE )
  , m_Intensity( node.GetFloatProperty( "intensity", 0 ) )
{
  m_Position = node.GetVect3fProperty( "pos", Math::Vect3f( 0, 0, 0 ) );
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