#include "EffectManager.h"
#include "Effect.h"
#include "EffectPool.h"
#include "EffectTechnique.h"
#include "Utils\Defines.h"
#include "EngineConfig.h"
#include "SharedEffect.h"

static bool sSharedEffectInited = false;

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
    , mEffectPool( new CEffectPool() )
{
}
CEffectManager::~CEffectManager()
{
    CleanUp();
    Destroy();
    CHECKED_DELETE( mEffectPool );
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
    if( mEffectPool->Init() )
        Load(mConfigPath);
}

void CEffectManager::Load( const std::string& lFile )
{
    CXMLTreeNode l_Node, lEffectsXml;
    if ( !lEffectsXml.LoadAndFindNode(lFile.c_str(), "effects", l_Node ) )
    {
        return;
    }

    for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount ; ++i )
    {
        CXMLTreeNode& l_CurrentNode = l_Node( i );
        const std::string& l_TagName = l_CurrentNode.GetName();

        if ( l_TagName == "technique" )
        {
            const std::string& l_TechniquetName = l_CurrentNode.GetAttribute<std::string>( "name", "null_tech" );
            int l_VertexType = l_CurrentNode.GetAttribute<int32>( "vertex_type", 0 );
            std::string l_EffectName;
            CXMLTreeNode l_HandlesNode;

            for ( uint32 j = 0, lCurrentNodeCount = l_CurrentNode.GetNumChildren(); j < lCurrentNodeCount; j++ )
            {
                CXMLTreeNode& l_CurrentSubNode = l_CurrentNode( j );
                const std::string& l_TagName = l_CurrentSubNode.GetName();

                if ( l_TagName == "effect" )
                {
                    l_EffectName = l_CurrentSubNode.GetAttribute<std::string>( "name", "no_effect" );
                    CEffect* l_pEffect = 0;//mEffectPool->CreateEffect(l_CurrentNode);
                    l_pEffect = new CEffect( l_EffectName );

                    if ( !l_pEffect->Load( l_CurrentSubNode, mEffectPool ) )
                    {
                        std::string msg_error = "EffectManager::Load->Error al intentar cargar el efecto: " + l_EffectName;
                        LOG_ERROR_APPLICATION( msg_error.c_str() );
                        CHECKED_DELETE( l_pEffect );
                    }
                    else if(!m_Effects.AddResource(l_EffectName, l_pEffect))
                    {
                        CHECKED_DELETE( l_pEffect );
                    }

                    if( l_pEffect && !sSharedEffectInited )
                    {
                        CSharedEffect* lSharedEffect = mEffectPool->GetSharedEffect();
                        ASSERT( lSharedEffect, "Null shared effect" );

                        lSharedEffect->SetEffect( l_pEffect->GetEffect() );
                        lSharedEffect->LinkSemantics();

                        sSharedEffectInited = true;
                    }
                }
                else if ( l_TagName == "handles" )
                {
                    l_HandlesNode = l_CurrentSubNode;
                }
            }

            CEffectTechnique* l_NewTechnique = new CEffectTechnique( l_TechniquetName, l_EffectName, l_HandlesNode );

            if ( !AddResource( l_TechniquetName, l_NewTechnique ) )
            {
                LOG_ERROR_APPLICATION( "CEffectManager::Error adding the new effect technique %s with effect %s!",
                                       l_TechniquetName.c_str(), l_EffectName.c_str() );
                CHECKED_DELETE( l_NewTechnique );
            }

            if ( m_DefaultTechniqueEffectMap.find( l_VertexType ) == m_DefaultTechniqueEffectMap.end() )
            {
                m_DefaultTechniqueEffectMap[l_VertexType] = l_TechniquetName;
            }
        }
        else if ( l_TagName == "effect" )
        {
            Load(l_CurrentNode.GetAttribute<std::string>("file", "no_file"));
        }
    }
}

void CEffectManager::ReloadEffects()
{
    std::map<std::string, CEffect*>& lEffectsVector = m_Effects.GetResourcesMap();
    std::map<std::string, CEffect*>::iterator lItb = lEffectsVector.begin(), lIte = lEffectsVector.end();
    for( ; lItb != lIte; ++lItb )
    {
        lItb->second->Reload(mEffectPool);
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
    mEffectPool->Bind();
}

