
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Math\Color.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "RenderableVertex\VertexTypes.h"
#include "EngineManagers.h"

#include <string>


CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(
    CXMLTreeNode& atts )
    : CStagedTexturedRendererCommand( atts )
{
    CRenderableObjectTechniqueManager* lROT = ROTMInstance;
    std::string l_TechniqueName =
        lROT->GetRenderableObjectTechniqueNameByVertexType(
            SCREEN_COLOR_VERTEX::GetVertexType() );
    m_RenderableObjectTechnique =
        lROT->GetResource( l_TechniqueName );
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
}

void CDeferredShadingSceneRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CDeferredShadingSceneRendererCommand::Execute", Profiler::Color::Orchid )
  ActivateTextures();
  SetLightsData( GM );
}


void CDeferredShadingSceneRendererCommand::SetLightsData( CGraphicsManager& GM )
{
    CLightManager* lLM = LightMInstance;
    uint32 l_Width, l_Height;
    GM.GetWidthAndHeight( l_Width, l_Height );
    RECT l_Rect = { 0, 0, ( long )l_Width, ( long )l_Height};

    if ( m_RenderableObjectTechnique != NULL )
    {
      CEffectTechnique* l_ET = m_RenderableObjectTechnique->GetEffectTechnique();
      uint32 n_lights = lLM->GetLightCount();
      for ( uint32 i = 0, lCount = lLM->GetLightCount(); i < lCount; ++i )
      {
        CLight* lLight = lLM->GetLight( i );
        if( lLight && lLight->IsVisible() )
        {
          //LOG_INFO_APPLICATION("Rendering light %s of room %s", lLight->GetName().c_str(), lLight->GetRoomName().c_str());
          l_ET->GetEffect()->SetLight( lLight );
          GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_ET, l_Rect, Math::colWHITE, NULL, 0, 0, 1, 1 );
        }
      }
    }
}