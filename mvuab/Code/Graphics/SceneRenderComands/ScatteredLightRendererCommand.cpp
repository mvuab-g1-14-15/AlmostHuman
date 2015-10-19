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
    m_RenderTarget1.Create("OcclusionMap",  640, 360, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
    m_RenderTarget2.Create("OcclusionMap",  640, 360, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
    m_RenderTarget3.Create("OcclusionMap", 1280, 720, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
}

CScatteredLightSceneRendererCommand::~CScatteredLightSceneRendererCommand()
{
}

void CScatteredLightSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    // 92.0f, -9.0f, -44.0f
    // 94.0f, -9.0f, -25.3f 
    //
    //
    //
    std::vector<BOOL> l_ActiveLights; ;
    std::vector<Math::Vect3f> l_PosLights;

    CEngineManagers::GetSingletonPtr();

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(92.0f, -9.0f, -44.0f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(94.0f, -9.0f, -25.3f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(0.0f, 0.0f, 0.0f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(0.0f, 0.0f, 0.0f));

    CFrustum l_CameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
    for(unsigned int i = 0; i < l_PosLights.size(); i++)
    {
        l_ActiveLights[i] = l_CameraFrustum.SphereIsVisible(l_PosLights[i], 0.1f);
    }

    if((l_ActiveLights[0] | l_ActiveLights[1] | l_ActiveLights[2] | l_ActiveLights[3]) == 0)
    {
        return;
    }

    // Occlusion Map
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("occlusion_map_pool_renderable_object_technique")->Apply();
    m_RenderTarget1.SetAsRenderTarget(0);

    GM.DisableZTest(); GM.DisableZWrite();
    GM.GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(50, 50, 50), 1, 0);

    CEngineManagers::GetSingletonPtr()->GetSceneManager()->RenderLayer("solid");

    m_RenderTarget1.UnsetAsRenderTarget(0);
    GM.EnableZTest(); GM.EnableZWrite();

    // Generate Rays Of God over occlusion Map
    RECT l_Rect1 = { 0, 0, 640, 360 };
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("scattering_light_pool_renderable_object_technique")->Apply();

    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);

    m_RenderTarget2.SetAsRenderTarget(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect1, Math::CColor::CColor(), &m_RenderTarget1, 0.0f, 0.0f, 1.0f, 1.0f);
    m_RenderTarget2.UnsetAsRenderTarget(0);

    // Set texture to original size
    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);
    RECT l_Rect2 = { 0, 0, l_Width, l_Height };

    /*ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("draw_quad_pool_renderable_object_technique")->Apply();
    m_RenderTarget3.SetAsRenderTarget(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect2, Math::CColor::CColor(), &m_RenderTarget2, 0.0f, 0.0f, 1.0f, 1.0f);
    m_RenderTarget3.UnsetAsRenderTarget(0);*/

    // Merge Rays Of God and normal scene texture
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("merge_pool_renderable_object_technique")->Apply();
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);
    l_EffectTech =  l_ROT->GetEffectTechnique();

    m_RenderTarget2.Activate(0);
    m_StageTextures[0].m_Texture->Activate(1);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect2, Math::CColor::CColor(), NULL, 0.0f, 0.0f, 1.0f, 1.0f);
}
