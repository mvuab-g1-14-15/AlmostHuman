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

void CStaticMeshManager::Load( const std::string& aFilePath, const std::string& aBasePath )
{
    TIMER_START();
	mMeshesPath = aBasePath + "meshes/";
    CXMLTreeNode newFile, node;
    if (newFile.LoadAndFindNode(aFilePath.c_str(), "static_meshes", node ))
    {
      for( int i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
      {
        const std::string &lName = node(i).GetAttribute<std::string>("name", "no_name");
        const std::string &file = aBasePath + std::string( node(i).GetAttribute<std::string>("filename", "no_file") );

        CStaticMesh *l_StaticMesh = new CStaticMesh();
        bool lLoadOk = l_StaticMesh->Load(file);
        ASSERT( lLoadOk, "Could not load static mesh %s", lName.c_str() );
        if(!lLoadOk || !AddResource( lName, l_StaticMesh ) )
        {
          CHECKED_DELETE(l_StaticMesh);
        }
      }
    }
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

CMaterial* CStaticMeshManager::GetMaterial( const std::string& aMaterialName )
{
	CMaterial* lMaterial = mMaterials.GetResource( aMaterialName );
	if( !lMaterial )
	{
		lMaterial = new CMaterial( mMeshesPath + aMaterialName + ".mat" );
		mMaterials.AddResource(aMaterialName, lMaterial);
	}
	return mMaterials.GetResource( aMaterialName );
}