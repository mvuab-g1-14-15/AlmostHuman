#include "Trigger.h"
#include "EngineManagers.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "PhysicsManager.h"
#include "Utils\Defines.h"

#include "Logger\Logger.h"
#include "EngineManagers.h"

#include "GraphicsManager.h"
#include "Effects/EffectManager.h"
#include "Shapes/Shapes.h"

CTrigger::CTrigger( const CXMLTreeNode& Node )
    : CName( Node.GetAttribute<std::string>( "name", "unknown" ) )
    , m_Position( Node.GetAttribute<Math::Vect3f>( "position", Math::Vect3f( 0, 0, 0 ) ) )
    , m_Size( Node.GetAttribute<Math::Vect3f>( "size", Math::Vect3f( 0, 0, 0 ) ) )
    , m_Color( Node.GetAttribute<Math::CColor>( "color", Math::CColor( 0, 0, 0, 1 ) ) )
    , m_Group( Node.GetAttribute<int32>( "group", 0 ) )
    , m_Paint( Node.GetAttribute<bool>( "paint", false ) )
    , m_PhysicUserData( new CPhysicUserData( Node.GetAttribute<std::string>( "name", "unknown" ) ) )
    , mTechnique( Node.GetAttribute<CEffectTechnique>("technique") )
    , mShape( 0 )
	, mActive( true )
{
    m_bEnter = Node.GetAttribute<bool>( "enter_event", false );

    if ( m_bEnter )
        m_Enter = ( std::make_pair( ENTER, Node.GetAttribute<std::string>( "enter_script",
                                    "unknown" ) ) );

    m_bLeave = Node.GetAttribute<bool>( "leave_event", false );

    if ( m_bLeave )
        m_Leave = ( std::make_pair( LEAVE, Node.GetAttribute<std::string>( "leave_script",
                                    "unknown" ) ) );

    m_bStay = Node.GetAttribute<bool>( "stay_event", false );

    if ( m_bStay )
        m_Stay = ( std::make_pair( STAY, Node.GetAttribute<std::string>( "stay_script",
                                   "unknown" ) ) );

    m_PhysicUserData->SetPaint( m_Paint );
    m_PhysicUserData->SetColor( m_Color );
    m_PhysicUserData->SetGroup( ECG_TRIGGERS );
    m_PhysicActor = new CPhysicActor( m_PhysicUserData );
    const std::string& l_sType = Node.GetAttribute<std::string>( "shape", "" );

    if ( l_sType == "Box" )
    {
        m_PhysicActor->CreateBoxTrigger( m_Position, m_Size, m_Group );
        m_PhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_PhysicActor );
        mShape = new CBoxShape();
    }
    else if ( l_sType == "Sphere" )
    {
        m_PhysicActor->CreateSphereTrigger( m_Position, m_Radius, m_Group );
        m_PhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_PhysicActor );
    }

    ASSERT( mShape, "Null trigger shape" );
    ASSERT( mTechnique, "Null technique" );
    mShape->SetColor( m_Color );

    if ( l_sType == "" )
    {
        CHECKED_DELETE( m_PhysicActor );
    }
}

CTrigger::CTrigger
(
    std::string name,
    Math::Vect3f position,
    Math::Vect3f size,
    Math::CColor color,
    int group,
    bool paint,
    bool bEnter,
    bool bStay,
    bool bLeave,
    std::string enterScript,
    std::string stayScript,
    std::string leaveScript
)
    : CName( name )
    , m_Position( position )
    , m_Size( size )
    , m_Color( color )
    , m_Group( group )
    , m_Paint( paint )
    , m_PhysicUserData( new CPhysicUserData( name ) )
    , mTechnique( EffectManagerInstance->GetEffectTechnique("RenderForwardDebugShapeTechnique") )
    , mShape( 0 )
	, mActive( true )
{
    m_bEnter = bEnter;

    if ( m_bEnter )
    {
        m_Enter = ( std::make_pair( ENTER, enterScript ) );
    }

    m_bLeave = bLeave;

    if ( m_bLeave )
    {
        m_Leave = ( std::make_pair( LEAVE, leaveScript ) );
    }

    m_bStay = bStay;

    if ( m_bStay )
    {
        m_Stay = ( std::make_pair( STAY, stayScript ) );
    }

    m_PhysicUserData->SetPaint( m_Paint );
    m_PhysicUserData->SetColor( m_Color );
    m_PhysicUserData->SetGroup( ECG_TRIGGERS );
    m_PhysicActor = new CPhysicActor( m_PhysicUserData );
    const std::string& l_sType = "box";

    if ( l_sType == "box" )
    {
        m_PhysicActor->CreateBoxTrigger( m_Position, m_Size, m_Group );
        m_PhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_PhysicActor );
        mShape = new CBoxShape();
    }
    else if ( l_sType == "sphere" )
    {
        m_PhysicActor->CreateSphereTrigger( m_Position, m_Radius, m_Group );
        m_PhysicActor->ActivateAllTriggers();
        CPhysicsManager* l_PM = PhysXMInstance;
        l_PM->AddPhysicActor( m_PhysicActor );
    }

    ASSERT( mShape, "Null trigger shape" );
    ASSERT( mTechnique, "Null technique" );
    mShape->SetColor( m_Color );

    if ( l_sType == "" )
    {
        CHECKED_DELETE( m_PhysicActor );
    }
}

CTrigger::~CTrigger()
{
    //TODO JAUME BORRAR EL ACTOR DE LA SCENE DE PHYSICMANAGER (DIRIA YO QUE FALTABA ESTO)
    Destroy();
}

void CTrigger::Release()
{
    CHECKED_DELETE( m_PhysicActor );
    CHECKED_DELETE( m_PhysicUserData );
}

void CTrigger::Destroy()
{
    // CPhysicsManager* l_PM = PhysXMInstance;
    // l_PM->ReleasePhysicActor( m_PhysicUserData->GetActor() );
    CHECKED_DELETE( m_PhysicActor );
    CHECKED_DELETE( m_PhysicUserData );
    CHECKED_DELETE( mShape);
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
}

void CTrigger::SetLUAByName( unsigned int Type, std::string aScript )
{
    switch ( Type )
    {
        case ENTER:
            m_Enter.second = aScript;
            break;

        case LEAVE:
            m_Leave.second = aScript;
            break;

        case STAY:
            m_Stay.second = aScript;
            break;

        default:
            LOG_ERROR_APPLICATION( "SetLUABYName (Trigger) Type error" );
    }
}

void CTrigger::Render()
{
    if ( m_Paint )
    {
        ASSERT( mShape, "Null shape" );
        mShape->SetPosition( m_Position );
        mShape->SetScale( m_Size );
        mShape->MakeTransform();
        mShape->Render( mTechnique );
    }
}
