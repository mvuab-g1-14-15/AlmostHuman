#include "SceneRenderComands\LensOfFlareRendererCommand.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "Math\Vector2.h"
#include "Math\Color.h"

CLensOfFlareRendererCommand::CLensOfFlareRendererCommand( CXMLTreeNode& atts ) :
  CStagedTexturedRendererCommand( atts )
{

}


void CLensOfFlareRendererCommand::Execute( CGraphicsManager& GM )
{
  CLight* Sun = CLightManager::GetSingletonPtr()->GetLight( 0 );

  if ( !Sun )
    return;

  // Todo, the sun is on the frustum?
  Math::Vect2i l_SunInScreen = GM.ToScreenCoordinates( Sun->GetPosition() );

  std::string l_EffectTechName =
    CRenderableObjectTechniqueManager::GetSingletonPtr()->GetRenderableObjectTechniqueNameByVertexType(
      SCREEN_COLOR_VERTEX::GetVertexType() );
  CRenderableObjectTechnique* l_ROT =
    CRenderableObjectTechniqueManager::GetSingletonPtr()->GetResource( l_EffectTechName );

  CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

  RECT l_Rect = { l_SunInScreen.x, l_SunInScreen.y, ( long )10, ( long )10};

  Math::CColor color = Math::colGREEN;

  if ( l_EffectTech )
  {
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_EffectTech, l_Rect, color,
        m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
  }
  else
  {
    GM.DrawColoredQuad2DTexturedInPixels( l_Rect, color,
                                          m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
  }
}