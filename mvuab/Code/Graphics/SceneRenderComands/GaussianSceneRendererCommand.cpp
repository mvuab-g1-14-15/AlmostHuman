#include "SceneRenderComands\GaussianSceneRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "GraphicsManager.h"
#include "Core.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(CXMLTreeNode& atts ) : CStagedTexturedRendererCommand( atts )
    , m_Width(atts.GetIntProperty("width", 0))
    , m_Height(atts.GetIntProperty("height", 0))
    , m_nIteration(atts.GetIntProperty("nIteration", 0))
    , m_NameTechnique(atts.GetPszProperty("technique","no_tech"))
{
    CEffectTechnique *l_Technique = CEffectManager::GetSingletonPtr()->GetResource(m_NameTechnique);
    l_Technique->SetUseTextureSize(true);

    if(atts.GetBoolProperty("texture_width_as_frame_buffer", true))
    {
        m_Width = CCore::GetSingletonPtr()->GetScreenWidth();
        m_Height = CCore::GetSingletonPtr()->GetScreenHeight();
    }

    m_pAuxTexture[0] = new CTexture();
    m_pAuxTexture[0]->Create("t1", m_Width, m_Height, 0, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);

    m_pAuxTexture[1] = new CTexture();
    m_pAuxTexture[1]->Create("t2", m_Width, m_Height, 0, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
    CHECKED_DELETE(m_pAuxTexture[0]);
    CHECKED_DELETE(m_pAuxTexture[1]);
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
    RECT l_Rect = { 0, 0, m_Width, m_Height };
    CEffectTechnique *l_Technique = CEffectManager::GetSingletonPtr()->GetResource("DrawQuadSampler0Technique");

    m_pAuxTexture[0]->SetAsRenderTarget(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
    m_pAuxTexture[0]->UnsetAsRenderTarget(0);

    l_Technique = CEffectManager::GetSingletonPtr()->GetResource(m_NameTechnique);
    l_Technique->SetTextureSize(m_Width, m_Height);

	for(size_t i = 0; i < m_nIteration; ++i)
	{
         m_pAuxTexture[(i + 1) % 2]->SetAsRenderTarget(0);
         GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_Technique, l_Rect, Math::CColor::CColor(), m_pAuxTexture[i % 2], 0.0f, 0.0f, 1.0f, 1.0f );
	     m_pAuxTexture[(i + 1) % 2]->UnsetAsRenderTarget(0);
    }

    unsigned int w = 0, h = 0;
    GM.GetWidthAndHeight(w, h);

    l_Rect.right = w;
    l_Rect.bottom = h;

    m_StageTextures[0].m_Texture->SetAsRenderTarget(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CEffectManager::GetSingletonPtr()->GetResource("DrawQuadSampler0Technique"), l_Rect, Math::CColor::CColor(), m_pAuxTexture[m_nIteration % 2], 0.0f, 0.0f, 1.0f, 1.0f );
    m_StageTextures[0].m_Texture->UnsetAsRenderTarget(0);
}
