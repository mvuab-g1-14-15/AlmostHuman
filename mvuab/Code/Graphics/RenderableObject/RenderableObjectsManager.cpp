#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "StaticMeshes\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Math\MathTypes.h"

CRenderableObjectsManager::CRenderableObjectsManager() : m_NumFaces(0), m_NumVertexs(0), m_NumDraws(0)
{
}

CRenderableObjectsManager::~CRenderableObjectsManager()
{
    Destroy();
}

//<MeshInstance name="Boli Azul" core="" pos="" yaw="" pitch="" roll="" scale=""/>
bool CRenderableObjectsManager::Load(const std::string &FileName)
{
    CXMLTreeNode newFile;
    CXMLTreeNode m;

    if (!newFile.LoadFile(FileName.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CStaticMeshManager::Load --> Error loading XML %s.",FileName.c_str());
        return false;
    }

    m = newFile["RenderableObjects"];

    if (!m.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CStaticMeshManager::Load --> Error reading %s, static_mesh_manager no existeix.",FileName.c_str());
        return false;
    }

    for(int i=0;i<m.GetNumChildren();++i)
    {
        const std::string &l_TagName=m(i).GetName();
        if(l_TagName=="MeshInstance")
        {
            std::string l_Name=m(i).GetPszProperty("name","");
            std::string l_Core=m(i).GetPszProperty("core","");
            Math::Vect3f l_Pos=m(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
            float32 l_Yaw=m(i).GetFloatProperty("yaw",0.0f);
            float32 l_Pitch=m(i).GetFloatProperty("pitch",0.0f);
            float32 l_Roll=m(i).GetFloatProperty("roll",0.0f);
            Math::Vect3f l_Scale=m(i).GetVect3fProperty("scale",Math::Vect3f(1.0f,1.0f,1.0f));
            CInstanceMesh* l_InstanceMesh = new CInstanceMesh(l_Name, l_Core);
            l_InstanceMesh->SetPosition(l_Pos);
            l_InstanceMesh->SetYaw(l_Yaw * Math::pi32/180.0f);
            l_InstanceMesh->SetPitch(l_Pitch * Math::pi32/180.0f);
            l_InstanceMesh->SetRoll(l_Roll * Math::pi32/180.0f);
            l_InstanceMesh->SetScale(l_Scale);
            AddResource(l_Name,l_InstanceMesh);
        }
        else if(l_TagName=="AnimatedInstance")
        {
          const std::string& l_Name=m(i).GetPszProperty("name","");
          const std::string& l_Core=m(i).GetPszProperty("core","");
          const Math::Vect3f& l_Pos=m(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
          float32 l_Yaw=m(i).GetFloatProperty("yaw",0.0f);
          float32 l_Pitch=m(i).GetFloatProperty("pitch",0.0f);
          float32 l_Roll=m(i).GetFloatProperty("roll",0.0f);
          Math::Vect3f l_Scale=m(i).GetVect3fProperty("scale",Math::Vect3f(1.0f,1.0f,1.0f));
          CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel(l_Name, l_Core);
          l_AnimatedInstance->SetPosition(l_Pos);
          l_AnimatedInstance->SetYaw(l_Yaw * Math::pi32/180.0f);
          l_AnimatedInstance->SetPitch(l_Pitch * Math::pi32/180.0f);
          l_AnimatedInstance->SetRoll(l_Roll * Math::pi32/180.0f);
          l_AnimatedInstance->SetScale(l_Scale);
          AddResource(l_Name,l_AnimatedInstance);
        }
    }

    return true;
}

void CRenderableObjectsManager::CleanUp()
{
    Destroy();
}

void CRenderableObjectsManager::Render()
{
    m_NumVertexs = m_NumFaces = m_NumDraws = 0;

    for(unsigned int i = 0; i < m_ResourcesVector.size(); ++i)
    {
        m_ResourcesVector[i]->Render();

        m_NumVertexs += ((CInstanceMesh *) m_ResourcesVector[i])->GetNumVertex();
        m_NumFaces += ((CInstanceMesh *) m_ResourcesVector[i])->GetNumFaces();
        m_NumDraws += ((CInstanceMesh *) m_ResourcesVector[i])->GetNumDraws();
    }
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
    for(unsigned int i = 0; i < m_ResourcesVector.size(); ++i)
        m_ResourcesVector[i]->Update(ElapsedTime);
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Math::Vect3f &Position)
{
	return NULL;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Math::Vect3f &Position)
{
	return NULL;
}
