#include "Light.h"
#include "Core.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "GraphicsManager.h"
#include "Math\Color.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"
#include <string>

CLight::CLight( const CXMLTreeNode& node )
  : CObject3D(), CName( node.GetPszProperty( "name", "" ) )
  , m_StartRangeAttenuation( node.GetFloatProperty( "att_start_range", 0 ) )
  , m_EndRangeAttenuation( node.GetFloatProperty( "att_end_range", 0 ) )
  , m_Color( node.GetVect3fProperty( "color", Math::Vect3f( 1, 1, 1 ) ) )
  , m_Intensity( node.GetFloatProperty( "intensity", 0 ) )
  , m_GenerateDynamicShadowMap( node.GetBoolProperty( "generate_shadow_map", false ) )
  , m_GenerateStaticShadowMap( node.GetBoolProperty( "generate_static_shadow_map", false ) )
  , m_RenderShadows( node.GetBoolProperty( "render_shadows", false ) )
  , m_ViewShadowMap( Math::m44fIDENTITY )
  , m_ProjectionShadowMap( Math:: m44fIDENTITY )
  , m_DynamicShadowMap( 0 )
  , m_StaticShadowMap( 0 )
  , m_ShadowMaskTexture( 0 )
{
  m_Position = node.GetVect3fProperty( "pos", Math::Vect3f( 0, 0, 0 ) );

  if ( m_GenerateDynamicShadowMap )
  {
    m_DynamicShadowMap = new CTexture();
    CTexture::TFormatType l_FormatType;
    l_FormatType = m_DynamicShadowMap->GetFormatTypeFromString(
                     node.GetPszProperty( "shadow_map_format_type", "" ) );
    size_t l_Width = node.GetIntProperty( "shadow_map_width", 0 );
    size_t l_Height = node.GetIntProperty( "shadow_map_height", 0 );
    m_DynamicShadowMap->Create( "Light_Dynamic", l_Width, l_Height, 1,
                                CTexture::RENDERTARGET, CTexture::DEFAULT, l_FormatType );
  }

  if ( m_GenerateStaticShadowMap )
  {
    m_StaticShadowMap = new CTexture();
    CTexture::TFormatType l_FormatType;
    l_FormatType = m_StaticShadowMap->GetFormatTypeFromString(
                     node.GetPszProperty( "static_shadow_map_format_type", "" ) );
    size_t l_Width = node.GetIntProperty( "static_shadow_map_width", 0 );
    size_t l_Height = node.GetIntProperty( "static_shadow_map_height", 0 );
    m_StaticShadowMap->Create( "Light_Static", l_Width, l_Height, 1,
                               CTexture::RENDERTARGET, CTexture::DEFAULT, l_FormatType );
  }

  const std::string& l_ShadowMaskTextureFile = node.GetPszProperty( "shadow_texture_mask", "" );

  if ( l_ShadowMaskTextureFile != "" )
  {
    m_ShadowMaskTexture = CTextureManager::GetSingletonPtr()->GetTexture( l_ShadowMaskTextureFile );
    assert( m_ShadowMaskTexture );
  }

  for ( int i = 0; i < node.GetNumChildren() ; ++i )
  {
    const std::string& l_Layer = node( i ).GetPszProperty( "renderable_objects_manager", "" );
    CRenderableObjectsManager* l_ROM =
      CoreInstance->GetRenderableObjectsLayersManager()->GetResource( l_Layer );

    if ( !l_ROM )
      continue;

    const std::string& TagName = node( i ).GetName() ;

    if ( TagName == "static" )
      m_StaticShadowMapRenderableObjectsManagers.push_back( l_ROM );

    if ( TagName == "dynamic" )
      m_DynamicShadowMapRenderableObjectsManagers.push_back( l_ROM );
  }
}

CLight::~CLight()
{
  CHECKED_DELETE( m_DynamicShadowMap );
  CHECKED_DELETE( m_StaticShadowMap );
}

void CLight::SetIntensity( const float intensity )
{
  m_Intensity = intensity;
}

float CLight::GetIntensity() const
{
  return m_Intensity;
}

void CLight::SetType( const ELightType Type )
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

CLight::ELightType CLight::GetType() const
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
  if ( !m_RenderShadows )
    return;

  SetShadowMap( GM );

  if ( m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap )
  {
    // To write into the texture of the static shadow map
    m_StaticShadowMap->SetAsRenderTarget();
    GM->BeginRender();
    GM->Clear( true, true, false, 0x000000ff );

    for ( size_t i = 0, lROMSize = m_StaticShadowMapRenderableObjectsManagers.size(); i < lROMSize; ++i )
      m_StaticShadowMapRenderableObjectsManagers[i]->Render();

    GM->EndRender();
    m_StaticShadowMap->UnsetAsRenderTarget();
    m_MustUpdateStaticShadowMap = false;
    m_StaticShadowMap->Save( "static_shadow_map" );
  }

  if ( !m_DynamicShadowMapRenderableObjectsManagers.empty() )
  {
    m_DynamicShadowMap->SetAsRenderTarget();
    GM->BeginRender();
    GM->Clear( true, true, true, 0xffffffff );

    for ( size_t i = 0; i < m_DynamicShadowMapRenderableObjectsManagers.size(); ++i )
      m_DynamicShadowMapRenderableObjectsManagers[i]->Render();

    GM->EndRender();
    m_DynamicShadowMap->UnsetAsRenderTarget();
  }
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
    CEffectManager* l_EM = EffectManagerInstance;
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