#include "Scene.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

#include "Utils\Defines.h"

#include "StaticMeshes\StaticMeshManager.h"
#include "Room.h"
#include "EngineManagers.h"

#include <string>


CScene::CScene( const CXMLTreeNode& atts )
    : CManager( atts )
{

}

CScene::~CScene()
{
    Destroy();
}

void CScene::Render()
{

}

void CScene::Update()
{

}

void CScene::Destroy()
{
    CMapManager::Destroy();
}

void CScene::Init()
{
    Load( mConfigPath );
}

bool CScene::Load( const std::string& l_FilePath )
{
    if ( l_FilePath == "" )
        return false;

    CXMLTreeNode l_Root, l_Node;

    if ( !l_Root.LoadAndFindNode( l_FilePath.c_str(), "scene", l_Node ) )
        return false;

    for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
    {
        CXMLTreeNode& l_CurrentNode = l_Node( i );

        const std::string& l_Path = l_CurrentNode.GetAttribute<std::string>( "path", "no_path" );
        const std::string& l_ROFile = l_CurrentNode.GetAttribute<std::string>( "renderable_objects_file", "no_file" );
        const std::string& l_SMFile = l_CurrentNode.GetAttribute<std::string>( "static_meshes_file", "no_file" );
        const std::string& l_Level = l_CurrentNode.GetAttribute<std::string>( "level", "no_level" );

        CRoom* lRoom = new CRoom();

        lRoom->SetName( l_Level );
        lRoom->SetRenderableObjectsPath( l_Path + "/" + l_ROFile );
        lRoom->SetStaticMeshesPath( l_Path + "/" + l_SMFile );
		lRoom->SetBasePath( l_Path + "/" );

        if ( !AddResource( l_Level, lRoom ) )
            CHECKED_DELETE( lRoom );
    }

	LoadRoom("core");

    return true;
}

bool CScene::Reload()
{
    CMapManager::Destroy();
    return Load( mConfigPath );
}

void CScene::LoadRoom( std::string aRoomName )
{
    CRenderableObjectsLayersManager* lROLM = new CRenderableObjectsLayersManager();

    CRoom* lRoom = GetResource( aRoomName );

    if ( lRoom )
    {
		std::string lSMPath = lRoom->GetStaticMeshesPath();
		std::string lROPath = lRoom->GetRenderableObjectsPath();
		std::string lBasePath = lRoom->GetBasePath();

		if (lSMPath.find(".xml") != std::string::npos)
			SMeshMInstance->Load( lSMPath, lBasePath );
		if (lROPath.find(".xml") != std::string::npos)
			lROLM->LoadLayers( lROPath );
        lRoom->SetLayers( lROLM );
    }
}

void CScene::ActivateRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );

    if ( !lRoom->GetLayers() )
        LoadRoom( aRoomName );

    lRoom->SetActive( true );
}

void CScene::UnloadRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );

    if ( lRoom )
    {
        CRenderableObjectsLayersManager* lROLM = lRoom->GetLayers();
        CHECKED_DELETE( lROLM );
        lRoom->SetActive( false );
    }
}

void CScene::DesactivateRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );
    lRoom->SetActive( false );
}