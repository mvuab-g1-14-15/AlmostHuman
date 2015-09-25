#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"
#include "EngineConfig.h"

#include "Utils\BaseUtils.h"

CStaticMeshManager::CStaticMeshManager() : CManager()
{
}

CStaticMeshManager::CStaticMeshManager( CXMLTreeNode& atts) : CManager(atts)
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    Destroy();
}

void CStaticMeshManager::Init()
{
}

void CStaticMeshManager::Load( std::string aFilePath, std::string aBasePath )
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(aFilePath.c_str()))
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load No se puede abrir \"%s\"!", aFilePath.c_str());
        return;
    }

    CXMLTreeNode node = newFile["static_meshes"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
        return;
    }

    TIMER_START();
    int lCount( node.GetNumChildren() );
    //#pragma omp parallel for shared(lCount)
    for( int i = 0; i < lCount ; ++i )
    {
        const std::string &lName = node(i).GetAttribute<std::string>("name", "no_name");
        const std::string &file = aBasePath + std::string( node(i).GetAttribute<std::string>("filename", "no_file") );

        MESH_THREAD_INFO *l_ThreadInfo = (MESH_THREAD_INFO *) malloc (sizeof(MESH_THREAD_INFO));
        new (l_ThreadInfo) MESH_THREAD_INFO;

        l_ThreadInfo->MeshManager = this;
        l_ThreadInfo->FileName.assign(file);
        l_ThreadInfo->ResourceName.assign(lName);

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) runMeshLoad, l_ThreadInfo, NULL, NULL);

        /*CStaticMesh *l_StaticMesh = new CStaticMesh();
        bool lLoadOk = l_StaticMesh->Load(file);

        ASSERT( lLoadOk, "Could not load static mesh %s", lName.c_str() );

        // Default TODO Delete
        int lState = 0;
        //#pragma omp atomic
        lState += AddResource( lName, l_StaticMesh );
        if(!lLoadOk || !lState )
        {
            CHECKED_DELETE(l_StaticMesh);
        }*/
    }
    TIMER_STOP( "CStaticMeshManager::Load." );
}

bool CStaticMeshManager::threadMeshLoad(std::string &l_File, std::string &l_Name)
{
    CStaticMesh *l_StaticMesh = new CStaticMesh();

    if(!l_StaticMesh->Load(l_File))
    {
        CHECKED_DELETE(l_StaticMesh);
        return false;
    }

    if(!AddResource(l_Name, l_StaticMesh))
    {
        CHECKED_DELETE(l_StaticMesh);
        return false;
    }

    return true;
}

void runMeshLoad(MESH_THREAD_INFO *l_ThreadInfo)
{
    bool lLoadOk = l_ThreadInfo->MeshManager->threadMeshLoad(l_ThreadInfo->FileName, l_ThreadInfo->ResourceName);
    ASSERT( lLoadOk, "Could not load static mesh %s", l_ThreadInfo->FileName.c_str() );
    
    free(l_ThreadInfo);
    l_ThreadInfo = NULL;
}
