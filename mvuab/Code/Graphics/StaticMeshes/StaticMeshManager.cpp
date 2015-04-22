#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"

#include "EngineManagers.h"
#include "EngineConfig.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"


CStaticMeshManager::CStaticMeshManager() : CManager()
{
}


CStaticMeshManager::CStaticMeshManager(CXMLTreeNode& atts) : CManager(atts)
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
    { l_AllocatorManger->m_pFreeListAllocator->MakeDelete(it->second); }

    m_Resources.clear();
}

void CStaticMeshManager::Init()
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(mConfigPath.c_str()))
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load No se puede abrir \"%s\"!", mConfigPath.c_str());
        return;
    }

    CXMLTreeNode node = newFile["static_meshes"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
        return;
    }

    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(int i = 0; i < node.GetNumChildren(); i++)
    {
        //TODO - CHECK THE TAG NAME
        std::string name = node(i).GetPszProperty("name", "no_name");
        std::string file = node(i).GetPszProperty("filename", "no_file");
        file = "Data" + file;

        CStaticMesh *l_StaticMesh = (CStaticMesh *) l_AllocatorManger->m_pFreeListAllocator->MakeNew<CStaticMesh>();
        if(!l_StaticMesh->Load(file)) { l_AllocatorManger->m_pFreeListAllocator->Deallocate(l_StaticMesh); }
        else { AddResource(name, l_StaticMesh); }
    }
}

void CStaticMeshManager::Reload()
{
    Destroy();
    Init();
}
