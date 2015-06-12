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

#ifdef _DEBUG
    #include <sstream>
#endif

CDrawQuadRendererCommand::CDrawQuadRendererCommand( CXMLTreeNode& atts ):
    CStagedTexturedRendererCommand( atts )
{
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand()
{
}

void CDrawQuadRendererCommand::Execute( CGraphicsManager& GM )
{
    ActivateTextures();

    CRenderableObjectTechniqueManager* lROTM = ROTMInstance;

    const std::string & l_EffectTechName = lROTM->GetRenderableObjectTechniqueNameByVertexType(
            SCREEN_COLOR_VERTEX::GetVertexType() );

    CRenderableObjectTechnique* l_ROT =
        lROTM->GetResource( l_EffectTechName );

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

    l_EffectTech->GetEffect()->SetLight( 0 );

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