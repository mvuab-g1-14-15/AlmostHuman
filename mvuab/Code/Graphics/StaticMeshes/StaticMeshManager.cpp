#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"
#include "EngineConfig.h"
#include "Material.h"

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

void CStaticMeshManager::LoadMaterials(const std::string& aMaterialsPath )
{
  TIMER_START();

  CXMLTreeNode lFile, lMaterials;
  if( lFile.LoadAndFindNode( aMaterialsPath.c_str(), "materials", lMaterials ) )
  {
    int32 lCount = lMaterials.GetNumChildren();
    //#pragma omp parallel for
    for( int32 i = 0; i < lCount; ++i )
    {
      const CXMLTreeNode& lNode = lMaterials(i);
      const std::string& lName = lNode.GetName();
      if( lName == "object_material" )
      {
        CMaterial* lNewMaterial = new CMaterial( lNode );
        if( !mMaterials.AddResource(lNewMaterial->GetName(), lNewMaterial ) )
        {
          LOG_ERROR_APPLICATION("Could not add %s material", lNewMaterial->GetName() );
          CHECKED_DELETE( lNewMaterial );
        }
      }
    }
  }

  TIMER_STOP("Load materials.");
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

    LoadMaterials( aBasePath + "meshes/materials.xml" );

    TIMER_START();
    int lCount( node.GetNumChildren() );
    std::vector<HANDLE> l_ThreadHandler;

    l_ThreadHandler.reserve(lCount);
    SYSTEM_INFO sysinfo;
    int lIntents = 0;

    GetSystemInfo( &sysinfo );
    int numCPU = sysinfo.dwNumberOfProcessors;

    //#pragma omp parallel for shared(lCount)
    for( int i = 0; i < lCount ; ++i )
    {
        const std::string &lName = node(i).GetAttribute<std::string>("name", "no_name");
        const std::string &file = aBasePath + std::string( node(i).GetAttribute<std::string>("filename", "no_file") );

        /*MESH_THREAD_INFO *l_ThreadInfo = (MESH_THREAD_INFO *) malloc (sizeof(MESH_THREAD_INFO));
        new (l_ThreadInfo) MESH_THREAD_INFO;
        int lSleep = 200;

        l_ThreadInfo->iD = i;
        l_ThreadInfo->MeshManager = this;

        l_ThreadInfo->FileName.assign(file);
        l_ThreadInfo->ResourceName.assign(lName);

        HANDLE h = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) runMeshLoad, l_ThreadInfo, NULL, NULL);
        while(h == NULL)
        {
            char s[256] = { 0 }; 
            
            sprintf_s(s, 256, "CreateThread -> Intent: %d -> Error: %d\n", ++lIntents, GetLastError());
            OutputDebugStringA(s);

            Sleep(lIntents * lSleep);
            h = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) runMeshLoad, l_ThreadInfo, NULL, NULL);
        }

        l_ThreadHandler.push_back(h);*/

        CStaticMesh *l_StaticMesh = new CStaticMesh();
        bool lLoadOk = l_StaticMesh->Load(file);

        ASSERT( lLoadOk, "Could not load static mesh %s", lName.c_str() );

        // Default TODO Delete
        int lState = 0;
        //#pragma omp atomic
        lState += AddResource( lName, l_StaticMesh );
        if(!lLoadOk || !lState )
        {
            CHECKED_DELETE(l_StaticMesh);
        }
    }

    /*WaitForMultipleObjects(l_ThreadHandler.size(), &l_ThreadHandler[0], true, INFINITE);
    for(unsigned int i = 0; i < l_ThreadHandler.size(); i++)
    {
        CloseHandle(l_ThreadHandler[i]);
    }*/

    TIMER_STOP( "CStaticMeshManager::Load." );
}

bool CStaticMeshManager::threadMeshLoad(std::string &l_File, std::string &l_Name, unsigned int iD)
{
    CStaticMesh *l_StaticMesh = new CStaticMesh();

    if(!l_StaticMesh->Load(l_File))
    {
        char s[256] = { 0 }; sprintf_s(s, 256, "CreateThread l_StaticMesh->Load() ERROR\n");
        OutputDebugStringA(s);

        CHECKED_DELETE(l_StaticMesh);
        return false;
    }

    if(!AddResource(l_Name, l_StaticMesh))
    {
        char s[256] = { 0 }; sprintf_s(s, 256, "CreateThread AddResource ERROR\n");
        OutputDebugStringA(s);

        CHECKED_DELETE(l_StaticMesh);
        return false;
    }

    return true;
}

void runMeshLoad(MESH_THREAD_INFO *l_ThreadInfo)
{
    bool lLoadOk = l_ThreadInfo->MeshManager->threadMeshLoad(l_ThreadInfo->FileName, l_ThreadInfo->ResourceName, l_ThreadInfo->iD);
    ASSERT( lLoadOk, "Could not load static mesh %s", l_ThreadInfo->FileName.c_str() );
    
    free(l_ThreadInfo);
    l_ThreadInfo = NULL;

    ExitThread(0);
}
