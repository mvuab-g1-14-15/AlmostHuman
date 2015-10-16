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
}

CScatteredLightSceneRendererCommand::~CScatteredLightSceneRendererCommand()
{
}

void CScatteredLightSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    //Math::Vect3f l_LightPos(93.8f, -8.7f, -24.5f);
    //Math::Vect3f l_LightPos(125.8f, 2.7f, -46.5f);
    //Math::Vect2f l_ScrPos = GM.ToScreenCoordinates(l_LightPos);

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    RECT l_Rect = { 0, 0, l_Width - 1, l_Height - 1 };
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("scattering_light_pool_renderable_object_technique")->Apply();

    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource(l_TechniqueName);
    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
}
