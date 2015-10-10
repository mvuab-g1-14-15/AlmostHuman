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
    m_RenderTexture = new CTexture();

    m_RenderTexture->Create("SL_PASS01",512, 512, 1, CTexture::eUsageDynamic, CTexture::eDefaultPool, CTexture::eRGBA8);
}

CScatteredLightSceneRendererCommand::~CScatteredLightSceneRendererCommand()
{
}

void CScatteredLightSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    Math::Vect3f l_LightPos(66.7f, -9.68f, -70.95f);
    Math::Vect2f l_ScrPos = GM.ToScreenCoordinates(l_LightPos);

    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());
    CRenderableObjectTechnique* l_ROT = ROTMInstance->GetResource( l_TechniqueName );
    CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();

    //CCamera l_LastCamera;
    //CCamera *l_CurrentCamera = CameraMInstance->GetCurrentCamera();

    //memcpy(&l_LastCamera, l_CurrentCamera, sizeof(CCamera));

    //l_CurrentCamera->SetPosition(l_LightPos);
    //l_CurrentCamera->SetDirection(l_LastCamera.GetDirection());

    //GM.SetupMatrices();

    unsigned int l_Width = 0, l_Height = 0;
    GM.GetWidthAndHeight(l_Width, l_Height);

    GM.Clear(true, true, true, 0);
    RECT l_Rect = { 0, 0, l_Width - 1, l_Height - 1 };

    m_RenderTexture->SetAsRenderTarget();
        D3DLIGHT9 l_Light;
        ZeroMemory(&l_Light, sizeof(D3DLIGHT9));

        l_Light.Type = D3DLIGHT_POINT;

        l_Light.Position.x =  120.7f;
        l_Light.Position.y =   -5.0f;
        l_Light.Position.x = -54.65f;

        l_Light.Diffuse.r = 1.0f;
        l_Light.Diffuse.g = 1.0f;
        l_Light.Diffuse.b = 1.0f;
        l_Light.Diffuse.a = 1.0f;

        l_Light.Range = 200.0f;
        l_Light.Attenuation0 = 0.1f;

        GM.GetDevice()->SetLight(0, &l_Light);
        GM.GetDevice()->LightEnable(0, TRUE);
        GM.GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

        GM.SetUseTechniques(false);
        CEngineManagers::GetSingletonPtr()->GetSceneManager()->RenderLayer("solid");
        GM.SetUseTechniques(true);

        GM.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_RenderTexture->UnsetAsRenderTarget(0);
    
    m_RenderTexture->Activate(0);
    GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(l_EffectTech, l_Rect, Math::CColor::CColor(), m_RenderTexture, 0.0f, 0.0f, 1.0f, 1.0f );
}
