#include "SceneRenderComands\ScatteredLightRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "RenderableObject\Scene.h"
#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

CScatteredLightSceneRendererCommand::CScatteredLightSceneRendererCommand(CXMLTreeNode& atts) : CStagedTexturedRendererCommand(atts)
{
    m_Decay = atts.GetAttribute<float>("Decay", 0.96815f);
    m_Exposure = atts.GetAttribute<float>("Exposure", 0.2f);

    m_Weight = atts.GetAttribute<float>("Weight", 0.98767f);
    m_Density = atts.GetAttribute<float>("Density", 0.926f);

    m_Samples = atts.GetAttribute<int>("Samples", 20);
}

CScatteredLightSceneRendererCommand::~CScatteredLightSceneRendererCommand()
{
}

void CScatteredLightSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    //Math::Vect3f l_LightPos(93.8f, -8.7f, -24.5f);
    Math::Vect3f l_LightPos(125.8f, 2.7f, -46.5f);
    Math::Vect2f l_ScrPos = GM.ToScreenCoordinates(l_LightPos);

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    RECT l_Rect = { 0, 0, l_Width - 1, l_Height - 1 };
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("scattering_light_pool_renderable_object_technique")->Apply();

    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatteredLight(m_Decay, m_Exposure, m_Weight, m_Density, m_Samples, Math::Vect2f(l_ScrPos.x, l_ScrPos.y), Math::Vect2i(l_Width, l_Height));

    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
