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

CStaticMeshManager::CStaticMeshManager( CXMLTreeNode& atts) : CManager(atts)
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
    { l_AllocatorManger->m_pFreeListAllocator->MakeDelete(it->second); }
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

    for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
    {
        const std::string &lName = node(i).GetAttribute<std::string>("name", "no_name");
        const std::string &file = "Data" + std::string( node(i).GetAttribute<std::string>("filename", "no_file") );

        CStaticMesh *l_StaticMesh = (CStaticMesh *) l_AllocatorManger->m_pFreeListAllocator->MakeNew<CStaticMesh>();
        bool lLoadOk = l_StaticMesh->Load(file);

        ASSERT( lLoadOk, "Could not load static mesh %s", lName.c_str() );

        // Default TODO Delete
        if( !AddResource( lName, l_StaticMesh ) )
        {
            l_AllocatorManger->m_pFreeListAllocator->MakeDelete(l_StaticMesh);
        }
    }
}

void CStaticMeshManager::Reload()
{
    Destroy();
    Init();
}
