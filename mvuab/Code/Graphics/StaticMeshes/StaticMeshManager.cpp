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
	TIMER_STOP( "CStaticMeshManager::Load." );
}