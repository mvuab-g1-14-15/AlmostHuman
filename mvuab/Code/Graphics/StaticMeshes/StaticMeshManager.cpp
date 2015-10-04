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
	mMeshesPath = aBasePath + "meshes/";
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
	CMaterial* lMaterial = NULL;
	if( !mMaterials.Exist(aMaterialName) )
	{
		lMaterial = new CMaterial( mMeshesPath + aMaterialName + ".mat" );
		mMaterials.AddResource(aMaterialName, lMaterial);
	}
	else
	{
		lMaterial = mMaterials.GetResource(aMaterialName);
	}
	return lMaterial;
}

CStaticMesh* CStaticMeshManager::GetStaticMesh( const std::string& aStaticMesh )
{
	CStaticMesh* lStaticMesh = NULL;
	if( !Exist(aStaticMesh) )
	{
		lStaticMesh = new CStaticMesh();
		bool lLoadOk = lStaticMesh ->Load( mMeshesPath + aStaticMesh + ".mesh");
		ASSERT( lLoadOk, "Could not load static mesh %s", aStaticMesh.c_str() );
		if(!lLoadOk || !AddResource( aStaticMesh, lStaticMesh ) )
		{
			CHECKED_DELETE(lStaticMesh);
		}
	}
	else
	{
		lStaticMesh = GetResource(aStaticMesh);
	}
	return lStaticMesh;
}