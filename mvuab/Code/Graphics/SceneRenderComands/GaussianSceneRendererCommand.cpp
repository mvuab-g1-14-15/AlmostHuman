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
    , m_Width( atts.GetIntProperty("width", 0) )
    , m_Height( atts.GetIntProperty("height", 0) )
    , mIterations( atts.GetIntProperty("iterations", 0) )
	, m_Technique( EffectManagerInstance->GetResource(atts.GetPszProperty("technique", "no_tech")) )
{
    if(atts.GetBoolProperty("texture_width_as_frame_buffer", true))
        GraphicsInstance->GetWidthAndHeight(m_Width, m_Height);

    m_Technique->SetTextureSize(m_Width, m_Height);
    m_Technique->SetUseTextureSize(true);

	ASSERT( m_StageTextures.size() == 2, "The number of textures for the gaussian" );
    
	m_pAuxTexture[0] = m_StageTextures[0].m_Texture;
    m_pAuxTexture[1] = m_StageTextures[1].m_Texture;

	ASSERT( m_StageTextures[0].mIsDynamic && !m_StageTextures[1].mIsDynamic, "The first texture must be dynamic and the second static" );
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
	ASSERT( m_Technique && m_pAuxTexture[0] && m_pAuxTexture[1], "Null data to execute the gaussian cmd" );

	RECT l_Rect = { 0, 0, m_Width, m_Height };
	m_Technique->SetTextureSize(m_Width, m_Height);
	m_Technique->SetUseTextureSize(true);

	for(uint32 i = 0; i < mIterations; ++i)
	{
		m_pAuxTexture[0]->SetAsRenderTarget(0);
		GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(m_Technique, l_Rect, Math::CColor::CColor(), m_pAuxTexture[1], 0.0f, 0.0f, 1.0f, 1.0f );
		m_pAuxTexture[0]->UnsetAsRenderTarget(0);
		std::swap( m_pAuxTexture[0], m_pAuxTexture[1] );
	}
}
