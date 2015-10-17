#include "Scene.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

#include "Utils\Defines.h"
#include "EngineManagers.h"

#include "Room.h"

#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "CookingMesh\PhysicCookingMesh.h"

#include <string>


CScene::CScene( const CXMLTreeNode& atts )
    : CManager( atts )
    , mCurrentRoom( 0 )
    , mToDelete( 0 )
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
    GetResource( "core" )->Update();

    for( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end(); lItb != lIte; ++lItb )
    {
        if( (*lItb)->IsActive() )
            (*lItb)->Update();
    }
}

void CScene::Destroy()
{
    CTemplatedVectorMapManager::Destroy();
}

void CScene::Init()
{
    Load( mConfigPath );
	LoadAllRoom();
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
                CRoom *lRoom = new CRoom( l_Node( i ) );
                if ( !AddResource( lRoom->GetName(), lRoom ) )
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
    CTemplatedVectorMapManager::Destroy();
    return Load( mConfigPath );
}

void CScene::ActivateRoom( const std::string& aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );
    if( lRoom )
    {
        if( !lRoom->IsLoaded() )
            lRoom->Load();

        lRoom->Activate();
        mCurrentRoom = lRoom;
    }
}

void CScene::UnloadRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );
    if ( lRoom )
    {
    }
}

void CScene::DesactivateRoom( std::string aRoomName )
{
    CRoom* lRoom = GetResource( aRoomName );
    lRoom->Deactivate();
}

std::vector<CLightProbe*> CScene::GetClosedLightProbes( std::string aRoomName, Math::Vect3f aPos )
{
    CRoom* lRoom = GetResource( aRoomName );
    //CRoom* lRoom = GetResource( "room2" ); // Hardcoded for test

    if ( lRoom )
        return lRoom->GetClosedLightProbes( aPos );

    return std::vector<CLightProbe*>();
}

void CScene::RenderLayer( const std::string& aLayerName )
{
    for( TVectorResources::iterator lItb = m_ResourcesVector.begin(), lIte = m_ResourcesVector.end(); lItb != lIte; ++lItb )
    {
        if( (*lItb)->IsActive() )
            (*lItb)->RenderLayer( aLayerName );
    }
}

void CScene::LoadRoom( const std::string& aRoomName )
{
    ActivateRoom( aRoomName );
}

const std::string CScene::GetActivateRoom()
{
    TVectorResources::iterator it = m_ResourcesVector.begin(), it_end = m_ResourcesVector.end();

    while ( it != it_end )
    {
        if ( *it == mCurrentRoom )
            return (*it)->GetName();

        ++it;
    }

    const std::string& lDontExist( "" );
    return lDontExist;
}

void CScene::LoadAllRoom()
{
	TMapResources::iterator it = m_ResourcesMap.begin(), it_end = m_ResourcesMap.end();
	for(;it!=it_end;++it)
		ActivateRoom(it->first);
}