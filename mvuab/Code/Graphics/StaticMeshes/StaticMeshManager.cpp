#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "ActionManager.h"
#include "EngineConfig.h"

CStaticMeshManager::CStaticMeshManager() : CManager()
{
}


CStaticMeshManager::CStaticMeshManager( CXMLTreeNode& atts)
    : CManager(atts)
{
}

CStaticMeshManager::~CStaticMeshManager()
{
    Destroy();
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

    for(int i = 0; i < node.GetNumChildren(); i++)
    {
        //TODO - CHECK THE TAG NAME
        std::string name = node(i).GetPszProperty("name", "no_name");
        std::string file = node(i).GetPszProperty("filename", "no_file");
        file = "Data" + file;

        CStaticMesh *l_StaticMesh = new CStaticMesh();
        if(!l_StaticMesh->Load(file))
        { CHECKED_DELETE(l_StaticMesh); }
        else
        { AddResource(name, l_StaticMesh); }
    }
}

void CStaticMeshManager::Reload()
{
    Destroy();
    Init();
}
