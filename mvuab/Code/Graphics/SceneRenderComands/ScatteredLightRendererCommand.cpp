#include "SceneRenderComands\ScatteredLightRendererCommand.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

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
    Math::Vect3f l_LightPos(66.7f, -9.68f, -70.95f);
    Math::Vect2f l_ScrPos = GM.ToScreenCoordinates(l_LightPos);
    std::string &l_TechniqueName = ROTMInstance->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType());

    CCamera l_LastCamera;
    CCamera *l_CurrentCamera = CameraMInstance->GetCurrentCamera();

    memcpy(&l_LastCamera, l_CurrentCamera, sizeof(CCamera));

    l_CurrentCamera->SetPosition(l_LightPos);
    l_CurrentCamera->SetDirection(l_LastCamera.GetDirection());

    GM.SetupMatrices();

    
}
