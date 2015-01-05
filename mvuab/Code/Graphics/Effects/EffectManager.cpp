#include "EffectManager.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils\Defines.h"

CEffectManager::CEffectManager()
    : m_WorldMatrix(Math::m44fIDENTITY),
    m_ProjectionMatrix(Math::m44fIDENTITY),
    m_ViewMatrix(Math::m44fIDENTITY),
    m_ViewProjectionMatrix(Math::m44fIDENTITY),
    m_LightViewMatrix(Math::m44fIDENTITY),
    m_ShadowProjectionMatrix(Math::m44fIDENTITY),
    m_CameraEye(Math::v3fZERO),
    m_StaticMeshTechnique(0),
    m_AnimatedModelTechnique(0),
    m_Filename("")
{
}

CEffectManager::~CEffectManager()
{
    CHECKED_DELETE(m_StaticMeshTechnique);
    CHECKED_DELETE(m_AnimatedModelTechnique);
    CleanUp();
    Destroy();
}

const Math::Mat44f & CEffectManager::GetWorldMatrix() const
{
    return m_WorldMatrix;
}

const Math::Mat44f & CEffectManager::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

const Math::Mat44f & CEffectManager::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const Math::Mat44f & CEffectManager::GetViewProjectionMatrix()
{
    return m_ViewProjectionMatrix;
}
const Math::Vect3f & CEffectManager::GetCameraEye()
{
    return m_CameraEye;
}
const Math::Mat44f & CEffectManager::GetLightViewMatrix() const
{
    return m_LightViewMatrix;
}

const Math::Mat44f & CEffectManager::GetShadowProjectionMatrix()
{
    return m_ShadowProjectionMatrix;
}

void CEffectManager::SetWorldMatrix(const Math::Mat44f &Matrix)
{
    m_WorldMatrix = Matrix;
}

void CEffectManager::SetProjectionMatrix(const Math::Mat44f &Matrix)
{
    m_ProjectionMatrix = Matrix;
}

void CEffectManager::SetViewMatrix(const Math::Mat44f &Matrix)
{
    m_ViewMatrix = Matrix;
}

void CEffectManager::SetViewProjectionMatrix(const Math::Mat44f &ViewProjectionMatrix)
{
    m_ViewProjectionMatrix = ViewProjectionMatrix;
}

void CEffectManager::SetLightViewMatrix(const Math::Mat44f &Matrix)
{
    m_LightViewMatrix = Matrix;
}

void CEffectManager::SetShadowProjectionMatrix(const Math::Mat44f &Matrix)
{
    m_ShadowProjectionMatrix = Matrix;
}

void CEffectManager::SetCameraEye(const Math::Vect3f &CameraEye)
{
    m_CameraEye = CameraEye;
}

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short VertexType)
{
    return m_DefaultTechniqueEffectMap[VertexType];
}

size_t CEffectManager::GetMaxLights() const
{
    return 1; // Ask Jordi
}

CEffect * CEffectManager::GetEffect(const std::string &Name)
{
    return m_Effects.GetResource(Name);
}

CEffectTechnique * CEffectManager::GetEffectTechnique(const std::string &Name)
{
    return GetResource(Name);
}

CEffectTechnique * CEffectManager::GetStaticMeshTechnique() const
{
    return m_StaticMeshTechnique;
}

void CEffectManager::SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique)
{
    m_StaticMeshTechnique = StaticMeshTechnique;
}

CEffectTechnique * CEffectManager::GetAnimatedModelTechnique() const
{
    return m_AnimatedModelTechnique;
}

void CEffectManager::SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique)
{
    m_AnimatedModelTechnique = AnimatedModelTechnique;
}

void CEffectManager::CleanUp()
{
    m_DefaultTechniqueEffectMap.clear();
    m_Effects.Destroy();
}

void CEffectManager::ActivateCamera(const Math::Mat44f &ViewMatrix, const Math::Mat44f &ProjectionMatrix, const Math::Vect3f &CameraEye)
{
    SetViewMatrix(ViewMatrix);
    SetProjectionMatrix(ProjectionMatrix);
    SetCameraEye(CameraEye);
}

void CEffectManager::Load(const std::string &FileName)
{
    // Obtain the filename
    m_Filename = FileName;
    
    // Check if the file exist
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_Filename.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CEffectManager::Load No se puede abrir \"%s\"!", m_Filename.c_str());
        return;
    }

    // Parse the file and search for the key's
    CXMLTreeNode l_Node = newFile["effects"];
    if(!l_Node.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CCameraKeyController::Load Tag \"%s\" no existe", "camera_key_controller");
        return;
    }

    for(int i = 0; i < l_Node.GetNumChildren(); i++)
    {
        CXMLTreeNode &l_CurrentNode =l_Node(i);
        const std::string &l_TagName = l_CurrentNode.GetName();

        if( l_TagName == "effect" )
        {
            const std::string &l_EffectName = l_CurrentNode.GetPszProperty("name");
            CEffect* l_pEffect = new CEffect(l_EffectName);
            if(!l_pEffect->Load(l_CurrentNode.GetPszProperty("file")))
            {
              std::string msg_error = "EffectManager::Load->Error al intentar cargar el efecto: " + l_EffectName;
              CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, msg_error.c_str());
              CHECKED_DELETE(l_pEffect);
            }
            m_Effects.AddResource(l_EffectName, l_pEffect);
        }
        else if( l_TagName == "technique" )
        {
            CEffectTechnique* l_pEffectTechninque = new CEffectTechnique(l_CurrentNode);
            const std::string &l_EffectTechniqueName = l_CurrentNode.GetPszProperty("name");
            AddResource(l_EffectTechniqueName, l_pEffectTechninque);
        }
        else if( l_TagName == "default_technique" )
        {
            int l_VertexType = l_CurrentNode.GetIntProperty("vertex_type");
            const std::string &l_EffectTechniqueName = l_CurrentNode.GetPszProperty("technique");

            if(m_DefaultTechniqueEffectMap.find(l_VertexType)==m_DefaultTechniqueEffectMap.end())
            {
                m_DefaultTechniqueEffectMap[l_VertexType] = l_EffectTechniqueName;
            }
            else
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "Redefinición de vertice");
            }
        }
    }
}

void CEffectManager::Reload()
{
    CleanUp();
    Destroy();
    Load(m_Filename);
}