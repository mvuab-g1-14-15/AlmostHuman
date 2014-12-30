#include "EffectTechnique.h"
#include "Core.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Lights\LightManager.h"
#include "Logger\Logger.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode & node)
    : m_TechniqueName(node.GetPszProperty("name")),
    m_EffectName(node.GetPszProperty("effect")),
    m_UseCameraPosition(node.GetBoolProperty("use_camera_position")),
    m_UseInverseProjMatrix(node.GetBoolProperty("use_inverse_projection_matrix")),
    m_UseInverseViewMatrix(node.GetBoolProperty("use_inverse_view_matrix")),
    m_UseInverseWorldMatrix(node.GetBoolProperty("use_inverse_world_matrix")),
    m_UseLights(node.GetBoolProperty("use_lights")),
    m_UseLightAmbientColor(node.GetBoolProperty("use_light_ambient_color")),
    m_UseProjMatrix(node.GetBoolProperty("use_projection_matrix")),
    m_UseViewMatrix(node.GetBoolProperty("use_view_matrix")),
    m_UseWorldMatrix(node.GetBoolProperty("use_world_matrix")),
    m_UseWorldViewMatrix(node.GetBoolProperty("use_world_view_matrix")),
    m_UseWorldViewProjectionMatrix(node.GetBoolProperty("use_world_view_projection_matrix")),
    m_UseViewProjectionMatrix(node.GetBoolProperty("use_view_projection_matrix")),
    m_UseViewToLightProjectionMatrix(node.GetBoolProperty("use_view_to_light_projection_matrix")),
    m_UseTime(node.GetBoolProperty("use_time")),
    m_NumOfLights(node.GetIntProperty("num_of_lights")),
    m_D3DTechnique(0)
{
    if(!Refresh())
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CEffectTechnique::CEffectTechnique()-> Error al hacer refresh");
}

CEffectTechnique::~CEffectTechnique()
{
}

bool CEffectTechnique::BeginRender()
{
    LPD3DXEFFECT l_Effect = m_Effect->GetEffect();
    CEffectManager* l_pEffectManager = CEffectManager::GetSingletonPtr();
    D3DXHANDLE l_Handle;
    if(m_UseCameraPosition)
    {
        l_Handle = m_Effect->GetCameraPositionParameter();
        l_Effect->SetFloatArray(l_Handle, &l_pEffectManager->GetCameraEye().x, 3);
    }

    if(m_UseInverseProjMatrix)
    {
        l_Handle = m_Effect->GetInverseProjectionMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetProjectionMatrix().GetInverted().GetD3DXMatrix());
    }
    if(m_UseInverseViewMatrix)
    {
        l_Handle = m_Effect->GetInverseViewMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetViewMatrix().GetInverted().GetD3DXMatrix());
    }
    if(m_UseInverseWorldMatrix)
    {
        l_Handle = m_Effect->GetInverseWorldMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetWorldMatrix().GetInverted().GetD3DXMatrix());
    }
    if(m_UseLights)
    {
        if(!m_Effect->SetLights(m_NumOfLights))
        {
            CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CEffectTechnique::BeginRender()->Error al hacer set lights de con NumOfLights=%d", m_NumOfLights);
            return false;
        }

        l_Handle = m_Effect->GetLightEnabledParameter();
        l_Effect->SetBoolArray(l_Handle, &m_Effect->GetLightsEnabled()[0], MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsTypeParameter();
        l_Effect->SetIntArray(l_Handle, m_Effect->GetLightsType(), MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsAngleParameter();
        l_Effect->SetFloatArray(l_Handle, m_Effect->GetLightsAngle(), MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsFallOffParameter();
        l_Effect->SetFloatArray(l_Handle, m_Effect->GetLightsFallOff(), MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsStartRangeAttenuationParameter();
        l_Effect->SetFloatArray(l_Handle, &m_Effect->GetLightsStartRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsEndRangeAttenuationParameter();
        l_Effect->SetFloatArray(l_Handle, &m_Effect->GetLightsEndRangeAttenuation()[0], MAX_LIGHTS_BY_SHADER);
        l_Handle = m_Effect->GetLightsPositionParameter();
        l_Effect->SetFloatArray(l_Handle, &m_Effect->GetLightsPosition()[0].x, MAX_LIGHTS_BY_SHADER*3);
        l_Handle = m_Effect->GetLightsDirectionParameter();
        l_Effect->SetFloatArray(l_Handle, &m_Effect->GetLightsDirection()[0].x, MAX_LIGHTS_BY_SHADER*3);
        l_Handle = m_Effect->GetLightsColorParameter();
        l_Effect->SetFloatArray(l_Handle, &m_Effect->GetLightsColor()[0].x, MAX_LIGHTS_BY_SHADER*3);
    }

    if(m_UseLightAmbientColor)
    {
        
    }
    else
    {

    }

    if(m_UseProjMatrix)
    {
        l_Handle = m_Effect->GetProjectionMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetProjectionMatrix().GetD3DXMatrix());
    }
    if(m_UseViewMatrix)
    {
        l_Handle = m_Effect->GetViewMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetViewMatrix().GetD3DXMatrix());
    }
    if(m_UseWorldMatrix)
    {
        l_Handle = m_Effect->GetWorldMatrixParameter();
        l_Effect->SetMatrix(l_Handle, &l_pEffectManager->GetWorldMatrix().GetD3DXMatrix());
    }
    if(m_UseWorldViewMatrix)
    {
        l_Handle = m_Effect->GetWorldViewMatrixParameter();
        Math::Mat44f world = l_pEffectManager->GetWorldMatrix();
        Math::Mat44f view = l_pEffectManager->GetViewMatrix();
        l_Effect->SetMatrix(l_Handle, &(view*world).GetD3DXMatrix());
    }
    if(m_UseWorldViewProjectionMatrix)
    {
        l_Handle = m_Effect->GetWorldViewProjectionMatrixParameter();
        Math::Mat44f world = l_pEffectManager->GetWorldMatrix();
        Math::Mat44f view = l_pEffectManager->GetViewMatrix();
        Math::Mat44f projection = l_pEffectManager->GetProjectionMatrix();
        l_Effect->SetMatrix(l_Handle, &(projection*view*world).GetD3DXMatrix());
    }
    if(m_UseViewProjectionMatrix)
    {
        l_Handle = m_Effect->GetViewProjectionMatrixParameter();
        Math::Mat44f view = l_pEffectManager->GetViewMatrix();
        Math::Mat44f projection = l_pEffectManager->GetProjectionMatrix();
        l_Effect->SetMatrix(l_Handle, &(projection*view).GetD3DXMatrix());
    }
    if(m_UseViewToLightProjectionMatrix)
    {
        D3DXMATRIX l_ViewToProjectionMatrix=l_pEffectManager->GetViewMatrix().GetD3DXMatrix();
        l_Effect->SetMatrix(m_Effect->GetViewToLightProjectionMatrixParameter(), &(l_pEffectManager->GetShadowProjectionMatrix()*l_pEffectManager->GetLightViewMatrix()).GetD3DXMatrix());
    }
    if(m_UseTime)
    {
        l_Handle = m_Effect->GetTimeParameter();
        l_Effect->SetFloat(l_Handle, (float)timeGetTime());
    }

    return true;
}

bool CEffectTechnique::Refresh()
{
    CEffectManager* l_pEffectManager = CEffectManager::GetSingletonPtr();
    m_Effect = l_pEffectManager->GetEffect(m_EffectName);
    m_D3DTechnique = m_Effect->GetTechniqueByName(m_TechniqueName);
    return (m_Effect!=0 && m_D3DTechnique!=0);
}