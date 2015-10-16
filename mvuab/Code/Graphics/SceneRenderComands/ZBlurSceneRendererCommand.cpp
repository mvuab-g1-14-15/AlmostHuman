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
}

CZBlurSceneRendererCommand::~CZBlurSceneRendererCommand()
{
}

void CZBlurSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    ActivateTextures();
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("ZBlur_pool_renderable_object_technique")->Apply();

    const std::string & l_EffectTechName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource( l_EffectTechName );
    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    RECT l_Rect = { 0, 0, l_Width - 1, l_Height - 1 };
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
