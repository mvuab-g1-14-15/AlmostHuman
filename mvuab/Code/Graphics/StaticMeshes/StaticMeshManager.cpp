#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"

CStaticMeshManager::CStaticMeshManager() : m_FileName("")
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
    m_FileName = FileName;
    
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_FileName.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load No se puede abrir \"%s\"!", m_FileName.c_str());
        return false;
    }

    CXMLTreeNode node = newFile["static_meshes"];
    if(!node.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
        return false;
    }

    for(int i = 0; i < node.GetNumChildren(); i++)
    {
        std::string name = node(i).GetPszProperty("name", "no_name");
        std::string file = node(i).GetPszProperty("filename", "no_file");
        file = "Data" + file;

        CStaticMesh *l_StaticMesh = new CStaticMesh();
        if(!l_StaticMesh->Load(file))
        {
            CHECKED_DELETE(l_StaticMesh);
        }
        else
            AddResource(name, l_StaticMesh);
    }
    
    return true;
}

bool CStaticMeshManager::Reload()
{
    Destroy();
    return Load(m_FileName);
}

void CStaticMeshManager::Update( float32 deltaTime )
{
  CActionManager* l_pActionManager = CActionManager::GetSingletonPtr();

  if( l_pActionManager->DoAction("ReloadStaticMesh") )
    Reload();
}