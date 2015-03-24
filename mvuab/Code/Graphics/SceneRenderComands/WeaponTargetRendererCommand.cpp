#include "SceneRenderComands\WeaponTargetRendererCommand.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "Math\Vector2.h"
#include "Math\Color.h"
#include "Effects\Effect.h"
#include "Cameras\CameraManager.h"
#include "Cameras\Camera.h"

CWeaponTargetRendererCommand::CWeaponTargetRendererCommand( CXMLTreeNode& atts ) 
	: CDrawQuadRendererCommand( atts )
	, mGizmoSize(atts.GetIntProperty("size", 20 ))
{
  std::string l_TechniqueName =
    CRenderableObjectTechniqueManager::GetSingletonPtr()->GetRenderableObjectTechniqueNameByVertexType(
      SCREEN_COLOR_VERTEX::GetVertexType() );
  m_RenderableObjectTechnique =
    CRenderableObjectTechniqueManager::GetSingletonPtr()->GetResource( l_TechniqueName );
}

void CWeaponTargetRendererCommand::Execute( CGraphicsManager& GM )
{
  // Activate the textures
  ActivateTextures();

  uint32 width, height;
  GM.GetWidthAndHeight( width, height );
  Math::Vect2f l_ScreenMiddlePoint( float32( width / 2 ), float32( height / 2 ) );
  
  long left = l_ScreenMiddlePoint.x - mGizmoSize;
  long top = l_ScreenMiddlePoint.y - mGizmoSize;
  long bottom = l_ScreenMiddlePoint.y  + mGizmoSize;
  long right = l_ScreenMiddlePoint.x + mGizmoSize;
  RECT l_Rect = { left, top, right, bottom };

  CEffectTechnique* l_EffectTech =  m_RenderableObjectTechnique->GetEffectTechnique();
  // Modify the color of the flares with the light color
  m_Color = Math::colBLACK;

	if ( l_EffectTech )
	{
		GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_EffectTech, l_Rect, m_Color,
			m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
	}
	else
	{
		GM.DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color,
											m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
	}
}