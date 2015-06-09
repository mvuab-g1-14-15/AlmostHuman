
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
    ActivateTextures();
    SetLightsData( GM );
}


void CDeferredShadingSceneRendererCommand::SetLightsData( CGraphicsManager& GM )
{
    CLightManager* l_LightManager = LightMInstance;
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