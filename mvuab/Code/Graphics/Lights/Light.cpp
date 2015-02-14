#include "Light.h"
#include "Math\Color.h"
#include "GraphicsManager.h"
#include "Effects\Effect.h"
#include "Core.h"
#include "Effects\EffectManager.h"
#include "Texture\Texture.h"

#include <string>

CLight::CLight( const CXMLTreeNode& node )
  : CObject3D()
  , m_StartRangeAttenuation( node.GetFloatProperty( "att_start_range", 0 ) )
  , m_EndRangeAttenuation( node.GetFloatProperty( "att_end_range", 0 ) )
  , m_Color( node.GetVect3fProperty( "color", Math::Vect3f( 1, 1, 1 ) ) )
  , m_Intensity( node.GetFloatProperty( "intensity", 0 ) )
{
  m_Position = node.GetVect3fProperty( "pos", Math::Vect3f( 0, 0, 0 ) );
  SetName( node.GetPszProperty( "name", "" ) );
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

void CLight::SetGenerateDynamicShadowMap( bool GenerateDynamicShadowMap )
{
  m_GenerateDynamicShadowMap = GenerateDynamicShadowMap;
}

bool CLight::GetGenerateDynamicShadowMap() const
{
  return m_GenerateDynamicShadowMap;
}

void CLight::SetGenerateStaticShadowMap( bool GenerateStaticShadowMap )
{
  m_GenerateStaticShadowMap = GenerateStaticShadowMap;
}

bool CLight::GetGenerateStaticShadowMap() const
{
  return m_GenerateStaticShadowMap;
}

void CLight::SetMustUpdateStaticShadowMap( bool MustUpdateStaticShadowMap )
{
  m_MustUpdateStaticShadowMap = MustUpdateStaticShadowMap;
}

bool CLight::GetMustUpdateStaticShadowMap() const
{
  return m_MustUpdateStaticShadowMap;
}

CTexture* CLight::GetStaticShadowMap() const
{
  return m_StaticShadowMap;
}

CTexture* CLight::GetDynamicShadowMap() const
{
  return m_DynamicShadowMap;
}

CTexture* CLight::GetShadowMaskTexture() const
{
  return m_ShadowMaskTexture;
}

std::vector<CRenderableObjectsManager*>&
CLight::GetStaticShadowMapRenderableObjectsManagers()
{
  return m_StaticShadowMapRenderableObjectsManagers;
}

std::vector<CRenderableObjectsManager*>&
CLight::GetDynamicShadowMapRenderableObjectsManagers()
{
  return m_DynamicShadowMapRenderableObjectsManagers;
}

void CLight::GenerateShadowMap( CGraphicsManager* GM )
{
  //TODO ALEX. I DON'T HAVE FUCKING IDEA
}

const Mat44f& CLight::GetViewShadowMap() const
{
  return m_ViewShadowMap;
}

const Mat44f& CLight::GetProjectionShadowMap() const
{
  return m_ProjectionShadowMap;
}

void CLight::BeginRenderEffectManagerShadowMap( CEffect* Effect )
{
  if ( m_GenerateDynamicShadowMap )
  {
    CEffectManager* l_EM = CCore::GetSingletonPtr()->GetEffectManager();
    l_EM->SetLightViewMatrix( m_ViewShadowMap );
    l_EM->SetShadowProjectionMatrix( m_ProjectionShadowMap );

    if ( m_ShadowMaskTexture != NULL )
      m_ShadowMaskTexture->Activate( SHADOW_MAP_MASK_STAGE );

    if ( m_GenerateStaticShadowMap )
      m_StaticShadowMap->Activate( STATIC_SHADOW_MAP_STAGE );

    m_DynamicShadowMap->Activate( DYNAMIC_SHADOW_MAP_STAGE );
    Effect->SetShadowMapParameters( m_ShadowMaskTexture != NULL,
                                    m_GenerateStaticShadowMap, m_GenerateDynamicShadowMap &&
                                    m_DynamicShadowMapRenderableObjectsManagers.size() != 0 );
  }
}