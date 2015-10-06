#include "SceneRenderComands\ZBlurSceneRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "GraphicsManager.h"

#include "EngineManagers.h"

CZBlurSceneRendererCommand::CZBlurSceneRendererCommand(CXMLTreeNode& atts) : CStagedTexturedRendererCommand(atts)
{
    m_FocalStart = atts.GetAttribute<float>("FocalStart", 2.0f);
    m_FocalEnd = atts.GetAttribute<float>("FocalEnd", 5.0f);

    m_BlurConstant = atts.GetAttribute<float>("Constant", 0.1f);
    m_BlurEnd = atts.GetAttribute<float>("BlurEnd", 50.0f);
}

CZBlurSceneRendererCommand::~CZBlurSceneRendererCommand()
{
}

void CZBlurSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    ActivateTextures();

    CRenderableObjectTechniqueManager* lROTM = ROTMInstance;
    unsigned int l_Width = 0, l_Height = 0;

    const std::string & l_EffectTechName = lROTM->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = lROTM->GetResource( l_EffectTechName );
    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

    GM.GetWidthAndHeight(l_Width, l_Height);
    RECT l_Rect = { 0, 0, ( long ) l_Width - 1, ( long ) l_Height - 1 };

    l_EffectTech->GetEffect()->SetZBlur(m_FocalStart, m_FocalEnd, m_BlurConstant, m_BlurEnd);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
