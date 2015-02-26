#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "Core.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(CXMLTreeNode& atts ) : CStagedTexturedRendererCommand( atts )
  , m_NameTechnique(atts.GetPszProperty("technique","no_tech"))
  , m_TextureAsFB(atts.GetBoolProperty("texture_width_as_frame_buffer", true))

  , m_Width(atts.GetIntProperty("width", 0))
  , m_Height(atts.GetIntProperty("height", 0))
  , m_nIteration(atts.GetIntProperty("nIteration", 0))
{
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
	if(m_TextureAsFB)
    {
        GM.GetWidthAndHeight(m_Width, m_Height);
        m_TextureAsFB = false;
    }

	RECT l_Rect = { 0, 0, m_Width, m_Height };

    ActivateTextures();
    CEffectTechnique *l_Technique = CEffectManager::GetSingletonPtr()->GetResource("DrawQuadSampler0Technique");

    m_StageTextures[0].m_Texture->SetAsRenderTarget(0);

    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    l_Technique = CEffectManager::GetSingletonPtr()->GetResource(m_NameTechnique);
    l_Technique->SetTextureSize(m_Width, m_Height);

	for(size_t i = 0; i < m_nIteration; ++i)
	{
        GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
	}

    m_StageTextures[0].m_Texture->UnsetAsRenderTarget(0);

    unsigned int w = 0, h = 0;
    GM.GetWidthAndHeight(w, h);

    l_Rect.right = w;
    l_Rect.bottom = h;

    l_Technique = CEffectManager::GetSingletonPtr()->GetResource("DrawQuadSampler0Technique");
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
