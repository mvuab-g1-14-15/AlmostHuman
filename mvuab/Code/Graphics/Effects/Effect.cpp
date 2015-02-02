#include "Effect.h"
#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Logger\Logger.h"
#include "GraphicsManager.h"
#include "Lights/LightManager.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"

CEffect::CEffect()
  : m_FileName(""),
    m_Effect(0),
    m_WorldMatrixParameter(0),
    m_ViewMatrixParameter(0),
    m_ProjectionMatrixParameter(0),
    m_WorldViewMatrixParameter(0),
    m_ViewProjectionMatrixParameter(0),
    m_WorldViewProjectionMatrixParameter(0),
    m_ViewToLightProjectionMatrixParameter(0),
    m_LightEnabledParameter(0),
    m_LightsTypeParameter(0),
    m_LightsPositionParameter(0),
    m_LightsDirectionParameter(0),
    m_LightsAngleParameter(0),
    m_LightsColorParameter(0),
    m_LightsFallOffParameter(0),
    m_LightsStartRangeAttenuationParameter(0),
    m_LightsEndRangeAttenuationParameter(0),
    m_CameraPositionParameter(0),
    m_BonesParameter(0),
    m_TimeParameter(0)
{
  memset(m_LightsEnabled,                 0, sizeof(BOOL) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsType,                    0, sizeof(int32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsAngle,                   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsFallOff,                 0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsStartRangeAttenuation,   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsEndRangeAttenuation,     0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsPosition,                0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsDirection,               0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsColor,                   0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
}

CEffect::CEffect( const std::string & EffectName)
  :
  m_FileName(""),
  m_Effect(0),
  m_WorldMatrixParameter(0),
  m_ViewMatrixParameter(0),
  m_ProjectionMatrixParameter(0),
  m_WorldViewMatrixParameter(0),
  m_ViewProjectionMatrixParameter(0),
  m_WorldViewProjectionMatrixParameter(0),
  m_ViewToLightProjectionMatrixParameter(0),
  m_LightEnabledParameter(0),
  m_LightsTypeParameter(0),
  m_LightsPositionParameter(0),
  m_LightsDirectionParameter(0),
  m_LightsAngleParameter(0),
  m_LightsColorParameter(0),
  m_LightsFallOffParameter(0),
  m_LightsStartRangeAttenuationParameter(0),
  m_LightsEndRangeAttenuationParameter(0),
  m_CameraPositionParameter(0),
  m_BonesParameter(0),
  m_TimeParameter (0)
{
  SetName(EffectName);
  memset(m_LightsEnabled,                 0, sizeof(BOOL) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsType,                    0, sizeof(int32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsAngle,                   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsFallOff,                 0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsStartRangeAttenuation,   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsEndRangeAttenuation,     0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsPosition,                0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsDirection,               0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsColor,                   0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
}

CEffect::~CEffect()
{
  CHECKED_RELEASE(m_Effect);
}

void CEffect::SetNullParameters()
{
  // Reset the parameters
  m_WorldMatrixParameter = 0;
  m_ViewMatrixParameter = 0;
  m_ProjectionMatrixParameter = 0;
  m_WorldViewMatrixParameter = 0;
  m_ViewProjectionMatrixParameter = 0;
  m_WorldViewProjectionMatrixParameter = 0;
  m_ViewToLightProjectionMatrixParameter = 0;
  m_LightEnabledParameter = 0;
  m_LightsTypeParameter = 0;
  m_LightsPositionParameter = 0;
  m_LightsDirectionParameter = 0;
  m_LightsAngleParameter = 0;
  m_LightsColorParameter = 0;
  m_LightsFallOffParameter = 0;
  m_LightsStartRangeAttenuationParameter = 0;
  m_LightsEndRangeAttenuationParameter = 0;
  m_CameraPositionParameter = 0;
  m_BonesParameter = 0;
  m_TimeParameter = 0;

  memset(m_LightsEnabled,                 0, sizeof(BOOL) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsType,                    0, sizeof(int32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsAngle,                   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsFallOff,                 0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsStartRangeAttenuation,   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsEndRangeAttenuation,     0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsPosition,                0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsDirection,               0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsColor,                   0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &a_Handle)
{
  a_Handle = m_Effect->GetParameterBySemantic(0,SemanticName.c_str());
  if(!a_Handle)
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName.c_str(), m_FileName.c_str());
}

void CEffect::GetParameterBySemantic(const char* SemanticName, D3DXHANDLE &a_Handle)
{
  a_Handle = m_Effect->GetParameterBySemantic(0,SemanticName);
  if(!a_Handle)
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName, m_FileName.c_str());
}

bool CEffect::LoadEffect()
{
  // Obtain the device from the graphics manager and load the effect
  LPDIRECT3DDEVICE9 l_Device = CGraphicsManager::GetSingletonPtr()->GetDevice();

  DWORD dwShaderFlags = 0;

  //dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
  //dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
  //dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
  dwShaderFlags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;

  LPD3DXBUFFER l_ErrorBuffer = 0;

  D3DXMACRO pDefines [2];
  pDefines[0].Name = "_NORMAL_MAP";
  pDefines[0].Definition = "TRUE";
  pDefines[1].Name = "_SELF_ILLUM";
  pDefines[1].Definition = "TRUE";

  HRESULT l_HR = D3DXCreateEffectFromFile(
                   l_Device,
                   m_FileName.c_str(),
                   0, // [CONST D3DXMACRO* pDefines,] pDefines,
                   0, // LPD3DXINCLUDE pInclude,
                   dwShaderFlags,
                   0, // LPD3DXEFFECTPOOL pPool,
                   &m_Effect,
                   &l_ErrorBuffer );

  //assert(l_HR == S_OK );
  if( l_ErrorBuffer )
  {
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CEffect::Error creating effect '%s':\n%s", m_FileName.c_str(), l_ErrorBuffer->GetBufferPointer());
    CHECKED_RELEASE(l_ErrorBuffer);
    return false;
  }


  assert(m_Effect);

  // Get the references to the handlers inside the effect
  GetParameterBySemantic(WorldMatrixParameterStr, m_WorldMatrixParameter);
  GetParameterBySemantic(ViewMatrixParameterStr, m_ViewMatrixParameter);
  GetParameterBySemantic(ProjectionMatrixParameterStr, m_ProjectionMatrixParameter);
  GetParameterBySemantic(InverseWorldMatrixParameterStr, m_InverseWorldMatrixParameter);
  GetParameterBySemantic(InverseViewMatrixParameterStr, m_InverseViewMatrixParameter);
  GetParameterBySemantic(InverseProjectionMatrixParameterStr, m_InverseProjectionMatrixParameter);
  GetParameterBySemantic(WorldViewMatrixParameterStr, m_WorldViewMatrixParameter);
  GetParameterBySemantic(ViewProjectionMatrixParameterStr, m_ViewProjectionMatrixParameter);
  GetParameterBySemantic(WorldViewProjectionMatrixParameterStr, m_WorldViewProjectionMatrixParameter);
  GetParameterBySemantic(ViewToLightProjectionMatrixParameterStr, m_ViewToLightProjectionMatrixParameter);
  GetParameterBySemantic(LightEnabledParameterStr, m_LightEnabledParameter);
  GetParameterBySemantic(LightsTypeParameterStr, m_LightsTypeParameter);
  GetParameterBySemantic(LightsPositionParameterStr, m_LightsPositionParameter);
  GetParameterBySemantic(LightsDirectionParameterStr, m_LightsDirectionParameter);
  GetParameterBySemantic(LightsAngleParameterStr, m_LightsAngleParameter);
  GetParameterBySemantic(LightsColorParameterStr, m_LightsColorParameter);
  GetParameterBySemantic(LightsFallOffParameterStr, m_LightsFallOffParameter);
  GetParameterBySemantic(LightsStartRangeAttenuationParameterStr, m_LightsStartRangeAttenuationParameter);
  GetParameterBySemantic(LightsEndRangeAttenuationParameterStr, m_LightsEndRangeAttenuationParameter);
  GetParameterBySemantic(CameraPositionParameterStr, m_CameraPositionParameter);
  GetParameterBySemantic(BonesParameterStr, m_BonesParameter);
  GetParameterBySemantic(TimeParameterStr, m_TimeParameter);

  return true;
}

void CEffect::Unload()
{
  SetNullParameters();
  CHECKED_RELEASE(m_Effect);
}

bool CEffect::Load(const std::string &FileName)
{
  m_FileName = FileName;
  return LoadEffect();
}

bool CEffect::Reload()
{
  Unload();
  return Load(m_FileName);
}

D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
  return (m_Effect) ? m_Effect->GetTechniqueByName(TechniqueName.c_str()) : 0;
}

bool CEffect::SetLights(size_t NumOfLights)
{
  if( NumOfLights > MAX_LIGHTS_BY_SHADER )
    return false;

  // Obtain the manager of lights
  CLightManager * l_pLightManager = CLightManager::GetSingletonPtr();

  //Reset all the lights of the effect
  memset(m_LightsEnabled,                 0, sizeof(BOOL) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsType,                    0, sizeof(int32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsAngle,                   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsFallOff,                 0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsStartRangeAttenuation,   0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsEndRangeAttenuation,     0, sizeof(float32) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsPosition,                0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsDirection,               0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);
  memset(m_LightsColor,                   0, sizeof(Math::Vect3f) * MAX_LIGHTS_BY_SHADER);

  for( size_t i = 0; i < NumOfLights; ++i )
  {
    CLight* l_pCurrentLight = l_pLightManager->GetLight(i);

    m_LightsEnabled[i] = (BOOL)1;
    CLight::TLightType l_LightType = l_pCurrentLight->GetType();
    m_LightsType[i] = static_cast<int>(l_LightType);
    m_LightsStartRangeAttenuation[i] = l_pCurrentLight->GetStartRangeAttenuation();
    m_LightsEndRangeAttenuation[i] = l_pCurrentLight->GetEndRangeAttenuation();
    m_LightsPosition[i] = l_pCurrentLight->GetPosition();

    Math::CColor l_Color = l_pCurrentLight->GetColor();
    m_LightsColor[i] = Math::Vect3f(l_Color.GetRed()/255.0f, l_Color.GetGreen()/255.0f, l_Color.GetBlue()/255.0f);

    if( l_LightType == CLight::DIRECTIONAL )
    {
      CDirectionalLight* l_pDirectionalLight = static_cast<CDirectionalLight*>(l_pCurrentLight);
      m_LightsDirection[i] = l_pDirectionalLight->GetDirection();
    }
    else if( l_LightType == CLight::SPOT )
    {
      CSpotLight* l_SpotLight = static_cast<CSpotLight*>(l_pCurrentLight);
      m_LightsDirection[i] = l_SpotLight->GetDirection();
      m_LightsAngle[i] = l_SpotLight->GetAngle();
      m_LightsFallOff[i] = l_SpotLight->GetFallOff();
    }
  }

  return true;
}

bool CEffect::SetCameraPosition(Math::Vect3f CameraPosition)
{
  float32 l_Camera[3];
  l_Camera[0] = CameraPosition.x;
  l_Camera[1] = CameraPosition.y;
  l_Camera[2] = CameraPosition.z;
  return ( m_Effect->SetFloatArray(m_CameraPositionParameter, l_Camera, 3) == S_OK );
}

bool CEffect::SetWorldMatrix(const Math::Mat44f &Matrix, bool Inverse)
{
  if( Inverse )
    return (m_Effect->SetMatrix(m_InverseWorldMatrixParameter, &Matrix.GetInverted().GetD3DXMatrix()) == S_OK );
  else
    return (m_Effect->SetMatrix(m_WorldMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetViewMatrix( const Math::Mat44f &Matrix, bool Inverse)
{
  if( Inverse )
    return (m_Effect->SetMatrix(m_InverseViewMatrixParameter, &Matrix.GetInverted().GetD3DXMatrix()) == S_OK );
  else
    return (m_Effect->SetMatrix(m_ViewMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetProjectionMatrix( const Math::Mat44f &Matrix, bool Inverse)
{
  if( Inverse )
    return (m_Effect->SetMatrix(m_InverseProjectionMatrixParameter, &Matrix.GetInverted().GetD3DXMatrix()) == S_OK );
  else
    return (m_Effect->SetMatrix(m_ProjectionMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetWorldViewMatrix( const Math::Mat44f &Matrix )
{
  return (m_Effect->SetMatrix(m_WorldViewMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetWorldViewProjectionMatrix( const Math::Mat44f &Matrix )
{
  return (m_Effect->SetMatrix(m_WorldViewProjectionMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetViewProjectionMatrix( const Math::Mat44f &Matrix )
{
  return (m_Effect->SetMatrix(m_ViewProjectionMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}

bool CEffect::SetViewToLightMatrix( const Math::Mat44f &Matrix )
{
  return (m_Effect->SetMatrix(m_ViewToLightProjectionMatrixParameter, &Matrix.GetD3DXMatrix()) == S_OK );
}
