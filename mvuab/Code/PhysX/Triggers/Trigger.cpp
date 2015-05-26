#include "Trigger.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "PhysicsManager.h"
#include "Utils\Defines.h"

#include "Logger\Logger.h"
#include "EngineManagers.h"

CTrigger::CTrigger( const CXMLTreeNode& Node )
    : CName( Node.GetPszProperty( "name", "unknown" ) )
    , m_Position( Node.GetVect3fProperty( "position", Math::Vect3f( 0, 0, 0 ) ) )
    , m_Size( Node.GetVect3fProperty( "size", Math::Vect3f( 0, 0, 0 ) ) )
    , m_Color( Node.GetCColorProperty( "color", Math::CColor( 0, 0, 0, 1 ) ) )
    , m_Group( Node.GetIntProperty( "group", 0 ) )
    , m_Paint( Node.GetBoolProperty( "paint", false ) )
    , m_pPhysicUserData( new CPhysicUserData( Node.GetPszProperty( "name",
                        "unknown" ) ) )
{
	m_bEnter = Node.GetBoolProperty( "enter_event", false );
    if ( m_bEnter )
        m_Enter = ( std::make_pair( ENTER, Node.GetPszProperty( "enter_script",
                                    "unknown" ) ) );
	m_bLeave = Node.GetBoolProperty( "leave_event", false );
    if ( m_bLeave )
        m_Leave = ( std::make_pair( LEAVE, Node.GetPszProperty( "leave_script",
                                    "unknown" ) ) );
	m_bStay = Node.GetBoolProperty( "stay_event", false );
    if ( m_bStay )
        m_Stay = ( std::make_pair( STAY, Node.GetPszProperty( "stay_script",
                                   "unknown" ) ) );

    m_pPhysicUserData->SetPaint( m_Paint );
    m_pPhysicUserData->SetColor( m_Color );
    m_pPhysicUserData->SetGroup( ECG_TRIGGERS );
    m_pPhysicActor = new CPhysicActor( m_pPhysicUserData );
    const std::string& l_sType = Node.GetPszProperty( "shape", "" );

    if ( l_sType == "box" )
    {
        m_pPhysicActor->CreateBoxTrigger( m_Position, m_Size, m_Group );
        m_pPhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_pPhysicActor );
    }
    else if ( l_sType == "sphere" )
    {
        m_pPhysicActor->CreateSphereTrigger( m_Position, m_Radius, m_Group );
        m_pPhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_pPhysicActor );
    }

    if ( l_sType == "" )
    { CHECKED_DELETE( m_pPhysicActor ); }
}

CTrigger::~CTrigger()
{
    //TODO JAUME BORRAR EL ACTOR DE LA SCENE DE PHYSICMANAGER (DIRIA YO QUE FALTABA ESTO)
    Destroy();
}

void CTrigger::Release()
{
    CHECKED_DELETE( m_pPhysicActor );
    CHECKED_DELETE( m_pPhysicUserData );
}

void CTrigger::Destroy()
{
    // CPhysicsManager* l_PM = PhysXMInstance;
    // l_PM->ReleasePhysicActor( m_PhysicUserData->GetActor() );
    CHECKED_DELETE( m_pPhysicActor );
    CHECKED_DELETE( m_pPhysicUserData );
}

std::string CTrigger::GetLUAByName( unsigned int Type )
{
    std::string l_Return;

    switch ( Type )
    {
        case ENTER:
            l_Return = m_Enter.second;
            break;

        case LEAVE:
            l_Return = m_Leave.second;
            break;

        case STAY:
            l_Return = m_Stay.second;
            break;

        default:
            l_Return = "";
            LOG_ERROR_APPLICATION( "GetLUABYName (Trigger) Type error" );
    }

    return l_Return;
};