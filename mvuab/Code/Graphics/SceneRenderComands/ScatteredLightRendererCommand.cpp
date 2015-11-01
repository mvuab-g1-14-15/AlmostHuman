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
    uint32 w = 0, h = 0;
    GraphicsInstance->GetWidthAndHeight(w, h);

    m_RenderTarget1.Create("OcclusionMap", w / 2, h / 2, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
    m_RenderTarget2.Create("ScatteredLight", w / 2, h / 2, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
    m_RenderTarget3.Create("MergeOcclusionScattering", w, h, 1, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, CTexture::eRGBA8);
}

CScatteredLightSceneRendererCommand::~CScatteredLightSceneRendererCommand()
{
}

void CScatteredLightSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    BROFILER_CATEGORY( "CScatteredLightSceneRendererCommand::Execute", Profiler::Color::Orchid )
    //  92.31f, -9.304f, -44.21f sala1 vent1
    //  94.02f, -9.206f, -23.10f sala1 vent2
    //   10.0f,   80.0f,   10.0f pasillo
    //-2000.0f,  291.0f,  76.0f space

    if(!GetVisible())
    {
        return;
    }

    std::vector<BOOL> l_ActiveLights; ;
    std::vector<Math::Vect3f> l_PosLights;

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(92.31f, -9.304f, -44.21f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(94.02f, -9.206f, -23.10f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(10.0f, 80.0f, 10.0f));

    l_ActiveLights.push_back(FALSE);
    l_PosLights.push_back(Math::Vect3f(-2000.0f, 351.0f, 106.0f));

    CFrustum l_CameraFrustum = CameraMInstance->GetCurrentCamera()->GetFrustum();
    Math::Vect3f l_CameraPosition = CameraMInstance->GetCurrentCamera()->GetPosition();
    Math::Vect3f l_CameraDirection = CameraMInstance->GetCurrentCamera()->GetDirection();

    std::string l_RoomName = CEngineManagers::GetSingletonPtr()->GetSceneManager()->GetCurrentRoomName();

    if(l_RoomName == "sala1")
    {
        l_ActiveLights[0] = l_CameraFrustum.SphereIsVisible(l_PosLights[0], 2.0f) ? TRUE : FALSE;
        l_ActiveLights[1] = l_CameraFrustum.SphereIsVisible(l_PosLights[1], 2.0f) ? TRUE : FALSE;
    }

    if(l_RoomName == "pasillo")
    {
        l_ActiveLights[2] = TRUE;
    }

    if(l_RoomName == "space")
    {
        l_ActiveLights[3] = TRUE;
    }

    if((l_ActiveLights[0] | l_ActiveLights[1] | l_ActiveLights[2] | l_ActiveLights[3]) == 0)
    {
        return;
    }

    // Occlusion Map
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("occlusion_map_pool_renderable_object_technique")->Apply();

    // Vertex 19
    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(19);
    CRenderableObjectTechnique *l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    CEffectTechnique *l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    // Vertex 31
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(31);
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    // Vertex 51
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(51);
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    // Vertex 63
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(63);
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    // Vertex 67
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(67);
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    // Vertex 415
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(415);
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech = l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);
    l_EffectTech->GetEffect()->SetCameraPosition(CameraMInstance->GetCurrentCamera()->GetPosition());

    m_RenderTarget1.SetAsRenderTarget(0);

    GM.DisableZTest(); GM.DisableZWrite();
    GM.GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1, 0);

    CEngineManagers::GetSingletonPtr()->GetSceneManager()->RenderLayer("solid");
    CEngineManagers::GetSingletonPtr()->GetSceneManager()->RenderLayer("characters");

    GM.EnableZTest(); GM.EnableZWrite();
    m_RenderTarget1.UnsetAsRenderTarget(0);

    // Generate Rays Of God over occlusion Map
    RECT l_Rect1 = { 0, 0, l_Width / 2, l_Height / 2 };
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("scattering_light_pool_renderable_object_technique")->Apply();

    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);

    l_EffectTech =  l_ROT->GetEffectTechnique();
    l_EffectTech->GetEffect()->SetScatterLights(l_ActiveLights, l_PosLights);

    m_RenderTarget2.SetAsRenderTarget(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect1, Math::CColor::CColor(), &m_RenderTarget1, 0.0f, 0.0f, 1.0f, 1.0f);
    m_RenderTarget2.UnsetAsRenderTarget(0);

    // Set texture to original size
    RECT l_Rect2 = { 0, 0, l_Width, l_Height };

    //ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("draw_quad_pool_renderable_object_technique")->Apply();
    //m_RenderTarget3.SetAsRenderTarget(0);
    //GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect2, Math::CColor::CColor(), &m_RenderTarget2, 0.0f, 0.0f, 1.0f, 1.0f);
    //m_RenderTarget3.UnsetAsRenderTarget(0);

    // Merge Rays Of God and normal scene texture
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("merge_pool_renderable_object_technique")->Apply();
    l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    l_ROT = ROTMInstance->GetResource(l_TechniqueName);
    l_EffectTech =  l_ROT->GetEffectTechnique();

    m_RenderTarget2.Activate(0);
    m_StageTextures[0].m_Texture->Activate(1);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect2, Math::CColor::CColor(), NULL, 0.0f, 0.0f, 1.0f, 1.0f);
}
