#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"
#include "EngineConfig.h"

CStaticMeshManager::CStaticMeshManager() : CManager()
{
}

CStaticMeshManager::CStaticMeshManager( CXMLTreeNode& atts) : CManager(atts)
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    //Destroy();
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

    for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
    {
        const std::string &lName = node(i).GetPszProperty("name", "no_name");
        const std::string &file = "Data" + std::string( node(i).GetPszProperty("filename", "no_file") );

        CStaticMesh *l_StaticMesh = new CStaticMesh();
        bool lLoadOk = l_StaticMesh->Load(file);

        ASSERT( lLoadOk, "Could not load static mesh %s", lName.c_str() );

        l_StaticMesh->SetType( node(i).GetPszProperty("type", "static") );

        // Default TODO Delete
        if( !AddResource( lName, l_StaticMesh ) )
            CHECKED_DELETE(l_StaticMesh);
    }
}

void CStaticMeshManager::Reload()
{
    Destroy();
    Init();
}
