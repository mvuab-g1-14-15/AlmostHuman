#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "Core.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(
  CXMLTreeNode& atts )
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

	CTexture* l_FinalTexture = m_StageTextures[1].m_Texture;
	CTexture* l_TempTexture = m_StageTextures[2].m_Texture;
	
	uint32 width, height;
	GM.GetWidthAndHeight( width, height );
	RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

	for(size_t i = 0; i < m_nIteration; ++i)
	{
		CTexture *l_SourceTexture=i==0 ? m_StageTextures[0].m_Texture : ((i%2)==0 ? l_FinalTexture : l_TempTexture);
        CTexture *l_RenderTargetTexture=(i%2)==0 ? l_TempTexture : l_FinalTexture;
		
		CEffectManager* l_EM = CEffectManager::GetSingletonPtr();
		
		CEffectTechnique *l_Technique=i==0 ? l_EM->GetResource("DrawQuadSampler0Technique") : l_EM->GetResource(m_NameTechnique);//TODO TECHNIQUE DRAWQUAD
        
        l_RenderTargetTexture->SetAsRenderTarget(0);
		GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_Technique, l_Rect, Math::CColor::CColor(),l_SourceTexture, 0.0f, 0.0f, 1.0f, 1.0f );
		
		l_RenderTargetTexture->UnsetAsRenderTarget(0);
	}

  
}