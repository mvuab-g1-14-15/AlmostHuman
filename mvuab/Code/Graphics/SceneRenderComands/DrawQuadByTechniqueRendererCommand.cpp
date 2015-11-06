#include "Effects\EffectManager.h"
#include "GraphicsManager.h"
#include "SceneRenderComands\DrawQuadByTechniqueRendererCommand.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Utils\Defines.h"

#include <windef.h>
#include "RenderableVertex\VertexTypes.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"

#include "EngineManagers.h"

#ifdef _DEBUG
    #include <sstream>
#endif

CDrawQuadByTechniqueRendererCommand::CDrawQuadByTechniqueRendererCommand( CXMLTreeNode& atts )
    :   CStagedTexturedRendererCommand( atts )
    ,   m_Technique (EffectManagerInstance->GetEffectTechnique(atts.GetAttribute<std::string>("technique", "null_tech")))
    ,   m_Color( Math::colWHITE )
{
    ASSERT(m_Technique, "DrawQuadByTechniqueRendererCommand empty technique");
}

CDrawQuadByTechniqueRendererCommand::~CDrawQuadByTechniqueRendererCommand()
{
}

void CDrawQuadByTechniqueRendererCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CDrawQuadByTechniqueRendererCommand::Execute", Profiler::Color::Orchid )
    ActivateTextures();

    #ifdef _DEBUG

    if ( false ) // DEBUG
    {
        DebugTextures();
    }

    #endif

    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

    if ( m_Technique )
    {
        m_Technique->GetEffect()->SetLight( 0 );
        GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( m_Technique, l_Rect, m_Color,
                m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    }
    else
    {
        GM.DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color,
                                              m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    }
}