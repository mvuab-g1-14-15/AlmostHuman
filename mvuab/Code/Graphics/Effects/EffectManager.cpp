#include "EffectManager.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils\Defines.h"
#include "EngineConfig.h"

CEffectManager::CEffectManager()
    : m_WorldMatrix( Math::m44fIDENTITY ),
      m_ProjectionMatrix( Math::m44fIDENTITY ),
      m_ViewMatrix( Math::m44fIDENTITY ),
      m_ViewProjectionMatrix( Math::m44fIDENTITY ),
      m_LightViewMatrix( Math::m44fIDENTITY ),
      m_ShadowProjectionMatrix( Math::m44fIDENTITY ),
      m_CameraEye( Math::v3fZERO ),
      m_Filename( "" ),
      CManager()
{
}

CEffectManager::CEffectManager( CXMLTreeNode &atts)
    : CManager(atts)
{
    /*  TODO RAUL
        PONER LECTURA DE XML
    */
}
CEffectManager::~CEffectManager()
{
    CleanUp();
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

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault( unsigned short VertexType )
{
    return m_DefaultTechniqueEffectMap[VertexType];
}

size_t CEffectManager::GetMaxLights() const
{
    return MAX_LIGHTS_BY_SHADER;
}

CEffect* CEffectManager::GetEffect( const std::string& Name )
{
    return m_Effects.GetResource( Name );
}

void CEffectManager::CleanUp()
{
    m_DefaultTechniqueEffectMap.clear();
    m_Effects.Destroy();
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
    // Obtain the filename
    //m_Filename = EngineConfigInstance->GetEffectsPath();
    // Check if the file exist
    CXMLTreeNode newFile;

    if ( !newFile.LoadFile( mConfigPath.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "CEffectManager::Load No se puede abrir \"%s\"!",
                               mConfigPath.c_str() );
        return;
    }

    // Parse the file and search for the key's
    CXMLTreeNode l_Node = newFile["effects"];

    if ( !l_Node.Exists() )
    {
        LOG_ERROR_APPLICATION( "CEffectManager::Load Tag \"%s\" no existe",
                               "effects" );
        return;
    }

    for ( int i = 0; i < l_Node.GetNumChildren(); i++ )
    {
        CXMLTreeNode& l_CurrentNode = l_Node( i );
        const std::string& l_TagName = l_CurrentNode.GetName();

        if ( l_TagName == "comment" )
        { continue; }

        if ( l_TagName == "technique" )
        {
            const std::string& l_TechniquetName = l_CurrentNode.GetPszProperty( "name" );
            int l_VertexType = l_CurrentNode.GetIntProperty( "vertex_type" );
            std::string l_EffectName;
            CXMLTreeNode l_HandlesNode;

            for ( int j = 0; j < l_CurrentNode.GetNumChildren(); j++ )
            {
                CXMLTreeNode& l_CurrentSubNode = l_CurrentNode( j );
                const std::string& l_TagName = l_CurrentSubNode.GetName();

                if ( l_TagName == "effect" )
                {
                    l_EffectName = l_CurrentSubNode.GetPszProperty( "name" );
                    CEffect* l_pEffect = new CEffect( l_EffectName );

                    if ( !l_pEffect->Load( l_CurrentSubNode ) )
                    {
                        std::string msg_error = "EffectManager::Load->Error al intentar cargar el efecto: " + l_EffectName;
                        LOG_ERROR_APPLICATION( msg_error.c_str() );
                        CHECKED_DELETE( l_pEffect );
                    }
                    else if(!m_Effects.AddResource(l_EffectName, l_pEffect))
                    {
                        CHECKED_DELETE( l_pEffect );
                    }
                }
                else if ( l_TagName == "handles" )
                { l_HandlesNode = l_CurrentSubNode; }
            }

            CEffectTechnique* l_NewTechnique = new CEffectTechnique( l_TechniquetName, l_EffectName, l_HandlesNode );

            if ( !AddResource( l_TechniquetName, l_NewTechnique ) )
            {
                LOG_ERROR_APPLICATION( "CEffectManager::Error adding the new effect technique %s with effect %s!",
                                       l_TechniquetName.c_str(), l_EffectName.c_str() );
                CHECKED_DELETE( l_NewTechnique );
            }

            if ( m_DefaultTechniqueEffectMap.find( l_VertexType ) == m_DefaultTechniqueEffectMap.end() )
            { m_DefaultTechniqueEffectMap[l_VertexType] = l_TechniquetName; }
        }
    }
}

void CEffectManager::Reload()
{
    CleanUp();
    Destroy();
    Init();
}

CEffectTechnique* CEffectManager::GetEffectTechnique( const std::string & aName ) const
{
    CEffectTechnique* lTech = GetConstResource(aName);

    if( !lTech )
    { LOG_ERROR_APPLICATION("The technique %s does not exist!!", aName.c_str() ); }

    return lTech;
}