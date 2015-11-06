#include "Effects\EffectManager.h"
#include "GraphicsManager.h"
#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include <windef.h>
#include "RenderableVertex\VertexTypes.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"

#include "EngineManagers.h"
#include "Utils/Defines.h"
#include "GUIManager.h"
#include "Window/Windows.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand( CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
  , mSetPlayerLife( atts.GetAttribute<bool>( "set_player_life", false ) )
  ,   m_Color( Math::colWHITE )
{
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand()
{
}

void CDrawQuadRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CDrawQuadRendererCommand::Execute", Profiler::Color::Orchid )
    ActivateTextures();

    CRenderableObjectTechniqueManager* lROTM = ROTMInstance;

    const std::string& l_EffectTechName = lROTM->GetRenderableObjectTechniqueNameByVertexType(
            SCREEN_COLOR_VERTEX::GetVertexType() );

    CRenderableObjectTechnique* l_ROT = lROTM->GetResource( l_EffectTechName );

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

    l_EffectTech->GetEffect()->SetLight( 0 );

    if( mSetPlayerLife )
    {
      CEffect* lEffect = l_EffectTech->GetEffect();

      float lLife = GUIInstance->GetWindow("HUD.xml")->GetElement("Life")->GetProgress();
      //LOG_INFO_APPLICATION("Setting life %5.2f", lLife);
      lLife = Math::Utils::Clamp(lLife, 0.05f, 100.0f);
      lEffect->SetLife(lLife);
    }

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