#include "EnemyManager.h"
#include "Enemy.h"
#include "EasyEnemy.h"
#include "PatrolEnemy.h"
#include "BossEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include <map>
#include "Utils\Types.h"
#include "Timer\Timer.h"

#include "StateMachine\StateMachine.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "EngineConfig.h"

#include "AnimatedModels\AnimatedInstanceModel.h"

typedef CEnemy* ( *CreateEnemyFn )( CXMLTreeNode& );

CEnemyManager::CEnemyManager()
    : CManager()
{
}

CEnemyManager::CEnemyManager(CXMLTreeNode& atts)
    : CManager(atts)
{
    /*  TODO RAUL
        PONER LECTURA XML
    */
}

CEnemyManager::~CEnemyManager()
{
    Destroy();
}

void CEnemyManager::Destroy()
{
    m_CoreEnemies.Destroy();
    CMapManager::Destroy();
    m_StateMachines.Destroy();
}

void CEnemyManager::Update()
{
    TMapResource::iterator itr = m_Resources.begin();

    while ( itr != m_Resources.end() )
    {
        m_ActualEnemy = itr->second;
		
		if ( m_ActualEnemy->GetLife() > 0 )
		{
			m_ActualEnemy->Update();
			++itr;
		}
		else
		{
			CAnimatedInstanceModel* l_AnimatedModel = m_ActualEnemy->GetAnimationModel();
			l_AnimatedModel->ChangeAnimationAction("morir", 0.2f, 0.2f);
			if ( l_AnimatedModel->IsActionAnimationActive("morir") )
			{
				CHECKED_DELETE( m_ActualEnemy );
			    m_Resources.erase( itr++ );
			}
			else
				++itr;
		}
    }
}

void CEnemyManager::Render()
{
    TMapResource::iterator it = m_Resources.begin();

    for ( ; it != m_Resources.end(); ++it )
    { it->second->Render(); }
}

void CEnemyManager::Init()
{
    //mConfigPath = EngineConfigInstance->GetEnemiesPath();
    RegisterEnemies();
    CXMLTreeNode l_File;

    if ( !l_File.LoadFile( mConfigPath.c_str() ) )
    { printf( "File '%s' not correctly loaded", mConfigPath.c_str() ); }

    CXMLTreeNode  m = l_File["enemies"];

    if ( m.Exists() )
    {
        int count = m.GetNumChildren();

        for ( int i = 0; i < count; ++i )
        {
            const std::string& l_TagName = m( i ).GetName();

            if ( l_TagName == "enemy" )
            { AddNewEnemy( m( i ) ); }
            else if ( l_TagName == "core_enemy" )
            {
                AddNewCoreEnemy( m( i ) );
                //<core_enemy type="easy" life="50" time_to_respawn="2.0" time_to_shoot="5.0" shoot_accuracy="0.35" state_machine="AI.xml"/>
            }
            else if ( l_TagName == "route" )
            { AddNewRoute( m( i ) ); }

        }
    }
}

void CEnemyManager::Reload()
{
    Destroy();
    Init();
}

template<class T>
CEnemy* CEnemyManager::CreateTemplatedEnemy( CXMLTreeNode& XMLTreeNode )
{
    return new T( XMLTreeNode );
}

void CEnemyManager::AddNewStateMachine( const std::string& Name, const std::string& SMFileName )
{
    CStateMachine* lStateMachine = new CStateMachine( Name );

    if ( lStateMachine->Load( SMFileName ) )
    { m_StateMachines.AddResource( Name,  lStateMachine ); }
    else
    {
        CHECKED_DELETE( lStateMachine );
        LOG_ERROR_APPLICATION( "Adding a new state machine" );
    }
}

void CEnemyManager::AddNewCoreEnemy( CXMLTreeNode& Node )
{
    CCoreEnemy* lCoreEnemy = new CCoreEnemy();
    // Read the type
    const std::string lType = Node.GetPszProperty( "type", "no_type" );

    if ( lType == "easy" )
    { lCoreEnemy->m_EnemyType = CEnemy::eEasy; }
    else if ( lType == "patroll" )
    { lCoreEnemy->m_EnemyType = CEnemy::ePatroll; }
    else if ( lType == "boss" )
    { lCoreEnemy->m_EnemyType = CEnemy::eBoss; }

    lCoreEnemy->m_Life = Node.GetFloatProperty( "life", 0.0f );
    lCoreEnemy->m_RespawnTime = Node.GetFloatProperty( "time_to_respawn", 0.0f );
    lCoreEnemy->m_TimeToShoot = Node.GetFloatProperty( "time_to_shoot", 0.0f );
    lCoreEnemy->m_ShootAccuracy = Node.GetFloatProperty( "shoot_accuracy", 0.0f );
    lCoreEnemy->m_StateMachineName = Node.GetPszProperty( "state_machine_name", "no_name" );
    lCoreEnemy->m_StateMachineFileName = Node.GetPszProperty( "state_machine_file", "no_name" );
    // Now add the new state machine
    AddNewStateMachine( lCoreEnemy->m_StateMachineName, lCoreEnemy->m_StateMachineFileName );
    m_CoreEnemies.AddResource( lType, lCoreEnemy );
}

void CEnemyManager::AddNewEnemy( CXMLTreeNode& Node )
{
    const std::string& lType = Node.GetPszProperty( "type" );
    CCoreEnemy* lCoreEnemy = m_CoreEnemies.GetResource( lType );

    if ( !lCoreEnemy )
    { LOG_ERROR_APPLICATION( ( "Core '%s' not found", lType.c_str() ) ); }

    CStateMachine* lStateMachine = m_StateMachines.GetResource( m_CoreEnemies.GetResource( lType )->m_StateMachineName );

    if ( !lStateMachine )
    { LOG_ERROR_APPLICATION( ( "State machine for '%s' not found", lType.c_str() ) ); }

    CEnemy* lEnemy = EnemyFactory.Create( lType.c_str(), Node, lStateMachine );

    lEnemy->AddMesh( Node.GetPszProperty( "mesh", "default_mesh" ) );
    lEnemy->SetLife( m_CoreEnemies.GetResource( lType )->m_Life );

    CPatrolEnemy* lPatrolEnemy = dynamic_cast<CPatrolEnemy*>( lEnemy );

    if ( lPatrolEnemy )
    { lPatrolEnemy->SetWaypoints( m_Routes[lPatrolEnemy->GetRouteId()] ); }

    if ( !AddResource( Node.GetPszProperty( "name", "no_name" ), lEnemy ) )
    {
        LOG_ERROR_APPLICATION( ( "Error adding '%s' not found", Node.GetPszProperty( "name", "no_name" ) ) );
        CHECKED_DELETE( lEnemy );
    }
}

void CEnemyManager::AddNewRoute( CXMLTreeNode& Node )
{
    const size_t l_Id = Node.GetIntProperty( "id", -1 );
    int count = Node.GetNumChildren();
    std::vector<Math::Vect3f> l_Route;

    for ( int i = 0; i < count; ++i )
    {
        const Math::Vect3f& l_Point = Node( i ).GetVect3fProperty( "value", Math::Vect3f( 0.0f, -99999999.0f, 0.0f ) );

        if ( l_Point != Math::Vect3f( 0.0f, -99999999.0f, 0.0f ) )
        { l_Route.push_back( l_Point ); }
        else
        { LOG_ERROR_APPLICATION( "Point in the route '%d' not correctly loaded.", l_Id ); }
    }

    m_Routes[l_Id] = l_Route;
}

void CEnemyManager::RegisterEnemies()
{
    EnemyFactory.Register( "easy", Type2Type<CEasyEnemy>( ) );
    EnemyFactory.Register( "patroll", Type2Type<CPatrolEnemy>( ) );
    EnemyFactory.Register( "boss", Type2Type<CBossEnemy>( ) );
}