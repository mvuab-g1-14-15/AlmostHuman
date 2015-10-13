#include "SceneRenderComands\GaussianSceneRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "GraphicsManager.h"

#include "EngineManagers.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(CXMLTreeNode& atts )
    : CStagedTexturedRendererCommand( atts )
    , m_Width( 0 )
    , m_Height( 0 )
    , mIterations( atts.GetAttribute<int32>("iterations", 0) )
{
    // Check the correct status of the command
    ASSERT( !m_StageTextures[0].mIsDynamic, "The first texture of the command must be a static texture" );
    ASSERT(mIterations % 2 != 0 || mIterations == 0, "The number of iterations must be even" );

    // Obtain the height of the output texture
    m_Width  = m_StageTextures[1].m_Texture->GetWidth();
    m_Height = m_StageTextures[1].m_Texture->GetHeight();
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    CRenderableObjectTechniqueManager* lROTM = ROTMInstance;

    const std::string & l_EffectTechName = lROTM->GetRenderableObjectTechniqueNameByVertexType(
            SCREEN_COLOR_VERTEX::GetVertexType() );

    CRenderableObjectTechnique* l_ROT = lROTM->GetResource( l_EffectTechName );

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

    RECT l_Rect = { 0, 0, ( long )m_Width - 1, ( long )m_Height - 1 };

    // Set the depht texture
    m_StageTextures[1].m_Texture->Activate( m_StageTextures[1].m_StageId );

    m_StageTextures[1].m_Texture->SetAsRenderTarget(0);
    l_EffectTech->GetEffect()->SetFBSize( Math::Vect2u(m_Width, m_Height) );
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    m_StageTextures[1].m_Texture->UnsetAsRenderTarget(0);

    if( mIterations > 1 )
    {
        std::swap( m_StageTextures[0], m_StageTextures[1] );
        for(uint32 i = 1; i < mIterations; ++i)
        {
            m_StageTextures[1].m_Texture->SetAsRenderTarget(0);
            GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
            m_StageTextures[1].m_Texture->UnsetAsRenderTarget(0);
            std::swap( m_StageTextures[0], m_StageTextures[1] );
        }
    }
}
