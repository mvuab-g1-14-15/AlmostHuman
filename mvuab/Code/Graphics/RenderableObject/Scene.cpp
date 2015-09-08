#include "Scene.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

#include "Utils\Defines.h"

#include "StaticMeshes\StaticMeshManager.h"
#include "Room.h"
#include "EngineManagers.h"

#include "Lights/LightManager.h"
#include "Particles/ParticleManager.h"

#include <string>


CScene::CScene( const CXMLTreeNode& atts )
    : CManager( atts )
    , mCurrentRoom( 0 )
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
    // Update the core room, in order to update all the characters
    GetResource( "core" )->GetLayers()->GetResource( "characters" )->Update();

    // Update the current room
    if ( mCurrentRoom )
        mCurrentRoom->GetLayers()->Update();
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
    bool lOk = false;

    if ( l_FilePath != "" )
    {
        CXMLTreeNode l_Root, l_Node;

        if ( l_Root.LoadAndFindNode( l_FilePath.c_str(), "scene", l_Node ) )
        {
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

                lRoom->LoadLightProbe();

                if ( !AddResource( l_Level, lRoom ) )
                    CHECKED_DELETE( lRoom );
            }

            // This will read the characters, all the characters are inside the core room, it is mandatory
            ActivateRoom( "core" );

            lOk = true;
        }
    }

    return lOk;
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

        if ( lSMPath.find( ".xml" ) != std::string::npos )
            SMeshMInstance->Load( lSMPath, lBasePath );

        if ( lROPath.find( ".xml" ) != std::string::npos )
            lROLM->LoadLayers( lROPath, aRoomName );

        LightMInstance->Load( lRoom->GetBasePath() + "lights.xml" );

        PSMan->SetConfigPath( lRoom->GetBasePath() + "particles.xml");
        PSMan->Init();

        lRoom->SetLayers( lROLM );
    }
}

void CScene::ActivateRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );

    if ( !lRoom->GetLayers() )
        LoadRoom( aRoomName );

    lRoom->SetActive( true );
    mCurrentRoom = lRoom;
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

std::vector<CLightProbe*> CScene::GetClosedLightProbes( std::string aRoomName, Math::Vect3f aPos )
{
    CRoom* lRoom = GetResource( aRoomName );
    //CRoom* lRoom = GetResource( "room2" ); // Hardcoded for test

    if ( lRoom )
        return lRoom->GetClosedLightProbes( aPos );

    return std::vector<CLightProbe*>();
}

const std::string& CScene::GetActivateRoom()
{
    TMapResource::iterator it = GetResourcesMap().begin(),
                           it_end = GetResourcesMap().end();

    while ( it != it_end )
    {
        if ( it->second == mCurrentRoom )
            return it->first;

        ++it;
    }

    const std::string& lDontExist( "" );
    return lDontExist;
}