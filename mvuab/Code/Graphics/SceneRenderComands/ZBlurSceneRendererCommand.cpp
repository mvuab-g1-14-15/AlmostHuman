#include "SceneRenderComands\ZBlurSceneRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"
#include "Timer/Timer.h"

CZBlurSceneRendererCommand::CZBlurSceneRendererCommand(CXMLTreeNode& atts) : CStagedTexturedRendererCommand(atts)
{
    m_FocalStart1 = m_FocalStart2 = atts.GetAttribute<float>("FocalStart", 2.0f);
    m_FocalEnd1 = m_FocalEnd2 = atts.GetAttribute<float>("FocalEnd", 4.0f);

    m_BlurConstant1 = m_BlurConstant2 = atts.GetAttribute<float>("BlurConstant", 0.1f);
    m_BlurEnd1 = m_BlurEnd2 = atts.GetAttribute<float>("BlurEnd", 10.0f);

    m_Time = 1.0f;
    m_AccTime = 0.0f;
}

CZBlurSceneRendererCommand::~CZBlurSceneRendererCommand()
{
}

void CZBlurSceneRendererCommand::Execute(CGraphicsManager &GM)
{
    BROFILER_CATEGORY( "CZBlurSceneRendererCommand::Execute", Profiler::Color::Orchid)
    m_AccTime += CEngine::GetSingletonPtr()->GetTimer()->GetElapsedTime();

    float l_FocalStart = LerpAndClamp(m_FocalStart1, m_FocalStart2, m_AccTime / m_Time);
    float l_FocalEnd = LerpAndClamp(m_FocalEnd1, m_FocalEnd2, m_AccTime / m_Time);

    float l_BlurConstant = LerpAndClamp(m_BlurConstant1, m_BlurConstant2, m_AccTime / m_Time);
    float l_BlurEnd = LerpAndClamp(m_BlurEnd1, m_BlurEnd2, m_AccTime / m_Time);

    ActivateTextures();
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("ZBlur_pool_renderable_object_technique")->Apply();

    const std::string & l_EffectTechName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource(l_EffectTechName);

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetBlur(l_FocalStart, l_FocalEnd, l_BlurConstant, l_BlurEnd);

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    RECT l_Rect = { 0, 0, l_Width - 1, l_Height - 1 };
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}

float CZBlurSceneRendererCommand::LerpAndClamp(float l_Start, float l_End, float t)
{
    float l = (1.0f - t) * l_Start + t * l_End;

    if(l_Start > l_End) l = (l < l_End) ? l_End : l;
    else l = (l > l_End) ? l_End : l;

    return l;

}

void CZBlurSceneRendererCommand::AdjustLens(float l_FocalStart, float l_FocalEnd, float l_BlurConstant, float l_BlurEnd, float l_Time)
{
    m_FocalStart1 = m_FocalStart2;
    m_FocalEnd1 = m_FocalEnd2;

    m_BlurConstant1 = m_BlurConstant2;
    m_BlurEnd1 = m_BlurEnd2;

    m_FocalStart2 = l_FocalStart;
    m_FocalEnd2 = l_FocalEnd;

    m_BlurConstant2 = l_BlurConstant;
    m_BlurEnd2 = l_BlurEnd;

    m_Time = (l_Time > 0.001) ? l_Time : 0.01f;
    m_AccTime = 0.0f;
}
