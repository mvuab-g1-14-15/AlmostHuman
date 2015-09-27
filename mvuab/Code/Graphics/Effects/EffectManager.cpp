#include "EffectManager.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils\Defines.h"
#include "EngineConfig.h"

CEffectManager::CEffectManager()
    : CManager()
    , m_WorldMatrix( Math::m44fIDENTITY )
    , m_ProjectionMatrix( Math::m44fIDENTITY )
    , m_ViewMatrix( Math::m44fIDENTITY )
    , m_ViewProjectionMatrix( Math::m44fIDENTITY )
    , m_LightViewMatrix( Math::m44fIDENTITY )
    , m_ShadowProjectionMatrix( Math::m44fIDENTITY )
    , m_CameraEye( Math::v3fZERO )
{
}

CEffectManager::CEffectManager( CXMLTreeNode &atts)
    : CManager(atts)
{
}
CEffectManager::~CEffectManager()
{
    Destroy();
}

const Math::Mat44f& CEffectManager::GetWorldMatrix() const
{
    return m_WorldMatrix;
}

const Math::Mat44f& CEffectManager::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

const Math::Mat44f& CEffectManager::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const Math::Mat44f& CEffectManager::GetViewProjectionMatrix()
{
    return m_ViewProjectionMatrix;
}
const Math::Vect3f& CEffectManager::GetCameraEye()
{
    return m_CameraEye;
}
const Math::Mat44f& CEffectManager::GetLightViewMatrix() const
{
    return m_LightViewMatrix;
}

const Math::Mat44f& CEffectManager::GetShadowProjectionMatrix()
{
    return m_ShadowProjectionMatrix;
}

void CEffectManager::SetWorldMatrix( const Math::Mat44f& Matrix )
{
    m_WorldMatrix = Matrix;
}

void CEffectManager::SetProjectionMatrix( const Math::Mat44f& Matrix )
{
    m_ProjectionMatrix = Matrix;
}

void CEffectManager::SetViewMatrix( const Math::Mat44f& Matrix )
{
    m_ViewMatrix = Matrix;
}

void CEffectManager::SetViewProjectionMatrix( const Math::Mat44f& ViewProjectionMatrix )
{
    m_ViewProjectionMatrix = ViewProjectionMatrix;
}

void CEffectManager::SetLightViewMatrix( const Math::Mat44f& Matrix )
{
    m_LightViewMatrix = Matrix;
}

void CEffectManager::SetShadowProjectionMatrix( const Math::Mat44f& Matrix )
{
    m_ShadowProjectionMatrix = Matrix;
}

void CEffectManager::SetCameraEye( const Math::Vect3f& CameraEye )
{
    m_CameraEye = CameraEye;
}

void CEffectManager::ActivateCamera( const Math::Mat44f& ViewMatrix,
                                     const Math::Mat44f& ProjectionMatrix, const Math::Vect3f& CameraEye )
{
    SetViewMatrix( ViewMatrix );
    SetProjectionMatrix( ProjectionMatrix );
    SetCameraEye( CameraEye );
}

void CEffectManager::Init()
{
  Load(mConfigPath);
}

void CEffectManager::Load( const std::string& lFile )
{
    CXMLTreeNode l_Node, lEffectsXml;
    if ( lEffectsXml.LoadAndFindNode(lFile.c_str(), "effects", l_Node ) )
    {
      for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount ; ++i )
      {
        const CXMLTreeNode& l_CurrentNode = l_Node( i );
        const std::string& l_TagName = l_CurrentNode.GetName();
        if ( l_TagName == "technique" )
        {
            CEffectTechnique* l_NewTechnique = new CEffectTechnique( l_CurrentNode  );
            if ( !AddResource( l_NewTechnique->GetName(), l_NewTechnique ) )
            {
              LOG_ERROR_APPLICATION( "Effect technique %s is duplicated", l_NewTechnique->GetName() );
              CHECKED_DELETE( l_NewTechnique );
            }
        }
        else if ( l_TagName == "effect" )
        {
            Load(l_CurrentNode.GetAttribute<std::string>("file", "no_file"));
        }
      }
    }
}

void CEffectManager::ReloadEffects()
{
  for( TMapResource::iterator lItb = m_Resources.begin(), lIte = m_Resources.end(); lItb != lIte; ++lItb)
  {
    (lItb)->second->Refresh();
  }
}

CEffectTechnique* CEffectManager::GetEffectTechnique( const std::string & aName ) const
{
    CEffectTechnique* lTech = GetConstResource(aName);

    if( !lTech )
    {
        LOG_ERROR_APPLICATION("The technique %s does not exist!!", aName.c_str() );
    }

    return lTech;
}


void CEffectManager::BeginRender()
{
}

