#include "Core.h"
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Math\Color.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "RenderableVertex\VertexTypes.h"

#include <string>
/*
<set_render_target name="deferred_multiple_render_target">
  <dynamic_texture stage_id="0" name="DiffuseMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
  <dynamic_texture stage_id="1" name="LightMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
  <dynamic_texture stage_id="2" name="NormalMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
  <dynamic_texture stage_id="3" name="DepthMapTexture" texture_width_as_frame_buffer="true" format_type="R32F"/>
</set_render_target>
*/

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(
  CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
{
  std::string l_TechniqueName =
    ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(
      SCREEN_COLOR_VERTEX::GetVertexType() );
  m_RenderableObjectTechnique =
    ROTMInstance->GetResource( l_TechniqueName );
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
}

void CDeferredShadingSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  ActivateTextures();
#ifdef _DEBUG

  if ( false ) // DEBUG
    DebugTextures();

#endif
  SetLightsData( GM );
}


void CDeferredShadingSceneRendererCommand::SetLightsData( CGraphicsManager& GM )
{
  CLightManager* l_LightManager = CCore::GetSingletonPtr()->GetLightManager();
  uint32 l_Width, l_Height;
  GM.GetWidthAndHeight( l_Width, l_Height );
  RECT l_Rect = { 0, 0, ( long )l_Width, ( long )l_Height};

  if ( m_RenderableObjectTechnique != NULL )
  {
    CEffectTechnique* l_ET = m_RenderableObjectTechnique->GetEffectTechnique();
    size_t n_lights = l_LightManager->GetResourcesVector().size();

    for ( size_t i = 0; i < n_lights; ++i )
    {
      l_ET->GetEffect()->SetLight( i );
      GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_ET, l_Rect, Math::colWHITE, NULL, 0, 0, 1,
          1 );
    }
  }
}