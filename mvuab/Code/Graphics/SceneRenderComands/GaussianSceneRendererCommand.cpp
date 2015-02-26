#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "Core.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
  , m_nIteration(atts.GetIntProperty("nIteration", 0))
  , m_NameTechnique(atts.GetPszProperty("technique","no_tech"))
{
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
	ActivateTextures();

	uint32 width = 0, height = 0;
	GM.GetWidthAndHeight(width, height);

	RECT l_Rect = { 0, 0, width, height };
    unsigned int l_Iter = (m_nIteration > 0)? m_nIteration + 1 : m_nIteration;

	for(size_t i = 0; i < l_Iter; ++i)
	{
		CEffectManager* l_EM = CEffectManager::GetSingletonPtr();
		CEffectTechnique *l_Technique = (i == 0)? l_EM->GetResource("DrawQuadSampler0Technique") : l_EM->GetResource(m_NameTechnique);

        m_StageTextures[0].m_Texture->SetAsRenderTarget(0);
		GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
		m_StageTextures[0].m_Texture->UnsetAsRenderTarget(0);
	}

    //GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CEffectManager::GetSingletonPtr()->GetResource("DrawQuadSampler0Technique"), l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
