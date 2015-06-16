#include "Enemy.h"
#include "Utils\Defines.h"
#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "ScriptManager.h"

#include "StateMachine\Action.h"
#include "Utils\MapManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObject.h"
#include "Actor\PhysicController.h"
#include "PhysicsManager.h"

#include <sstream>
#include "Gizmos\Gizmo.h"
#include "Gizmos\GizmoElement.h"
#include "Gizmos\GizmosManager.h"
#include "EngineManagers.h"

#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Shoot.h"
#include "Pathfinding\AStar.h"

#include "Characters/Enemies/EnemyManager.h"
#include "Timer/Timer.h"

CEnemy::CEnemy( CXMLTreeNode& Node, CStateMachine *aStateMachine )
    : CCharacter( Node.GetPszProperty( "name", "no_name" ) )
    , m_CurrentState( "inicial" )
    , m_NextState( "inicial" )
    , m_OnEnter( true )
    , m_OnExit( false )
    , m_pStateMachine( aStateMachine )
    , m_pRenderableObject( NULL )
	, mPathCalculated( false )
{
    CCharacter::Init( Node );
}

CEnemy::~CEnemy()
{
    CRenderableObjectsManager *l_ROM = ROLMInstance->GetResource( "characters" );
    PhysXMInstance->ReleasePhysicController( m_Controller );
    l_ROM->RemoveResource( m_Name );

    std::ostringstream ss;
    ss << GetName() << "HeadGizmo";
    std::string l_GizmoName( ss.str() );
    GizmosMInstance->RemoveResource( l_GizmoName );
}

void CEnemy::Update()
{
    CScriptManager *l_SM = ScriptMInstance;

    if ( m_CurrentState != m_NextState )
        m_OnExit = true;

    CState *l_State = m_pStateMachine->GetResource( m_CurrentState );
    std::vector<CAction *> l_Actions;

    if ( m_OnEnter )
    {
        l_Actions = l_State->GetOnEnterActions();
        m_OnEnter = false;
    }
    else if ( m_OnExit )
    {
        l_Actions = l_State->GetOnExitActions();
        m_OnEnter = true;
        m_OnExit = false;
        m_CurrentState = m_NextState;
    }
    else
        l_Actions = l_State->GetUpdateActions();

    std::vector<CAction *>::iterator it = l_Actions.begin(),
                                     it_end = l_Actions.end();

    for ( ; it != it_end; ++it )
        l_SM->RunCode( ( *it )->GetLuaFunction() + "()" );

    m_Position = m_Controller->GetPosition();
    m_Position.y -=  m_Controller->GetHeight() / 2.0f;
	SetYaw( m_Controller->GetYaw() );
    SetPitch( m_Controller->GetPitch() );
    SetRoll( m_Controller->GetRoll() );

    MakeTransform();

    m_pRenderableObject->SetPosition( m_Position );
    m_pRenderableObject->SetYaw( m_fYaw );
    m_pRenderableObject->SetPitch( m_fPitch );
    m_pRenderableObject->SetRoll( m_fRoll );

    m_pRenderableObject->MakeTransform();

    Math::Vect3f l_Pos = m_Position;
    l_Pos.y += m_Controller->GetHeight();

    std::vector<CShoot *>::iterator it_shoot = mShoots.begin();

    while ( it_shoot != mShoots.end() )
    {
        CShoot *lShoot = *it_shoot;

        if ( lShoot->Impacted() )
            it_shoot = mShoots.erase( it_shoot );
        else
            ++it_shoot;
    }

    it_shoot = mShoots.begin();
    std::vector<CShoot *>::iterator it_end_shoot = mShoots.end();

    for ( ; it_shoot != it_end_shoot; ++it_shoot )
    {
        CShoot *lShoot = *it_shoot;
        lShoot->Update();
    }

    std::ostringstream ss;
    ss << GetName() << "HeadGizmo";
    std::string l_GizmoName( ss.str() );

    CGizmosManager *l_GizmosManager = GizmosMInstance;
    CGizmo *l_Gizmo = l_GizmosManager->GetResource( l_GizmoName );

    if ( l_Gizmo )
    {
        l_Gizmo->SetPosition( l_Pos );
        l_Gizmo->SetYaw( m_fYaw );
        l_Gizmo->SetPitch( m_fPitch );
    }
    else
    {
        l_Gizmo = l_GizmosManager->CreateGizmo( l_GizmoName, l_Pos, m_fYaw, m_fPitch );
        l_GizmosManager->AddResource( l_GizmoName, l_Gizmo );

        CGizmoElement *l_Element = l_GizmosManager->CreateGizmoElement( CGizmoElement::eCube, 0.2f, Math::Vect3f( 0.4f, 0.0f, 0.0f ), 0.0f, 0.0f, Math::colRED );
        l_Gizmo->AddElement( l_Element );

        l_Element = l_GizmosManager->CreateGizmoElement( CGizmoElement::eCube, 0.2f, Math::Vect3f( 0.0f, 0.4f, 0.0f ), 0.0f, 0.0f, Math::colGREEN );
        l_Gizmo->AddElement( l_Element );

        l_Element = l_GizmosManager->CreateGizmoElement( CGizmoElement::eCube, 0.2f, Math::Vect3f( 0.0f, 0.0f, 0.4f ), 0.0f, 0.0f, Math::colBLUE );
        l_Gizmo->AddElement( l_Element );

        l_Element = l_GizmosManager->CreateGizmoElement( CGizmoElement::eSphere, 0.2f, Math::Vect3f( 0.0f ), 0.0f, 0.0f );
        l_Gizmo->AddElement( l_Element );
    }

    {
        std::ostringstream ss;
        ss << GetName() << "TargetPosGizmo";
        std::string l_GizmoName( ss.str() );

        CGizmosManager *l_GizmosManager = GizmosMInstance;
        CGizmo *l_Gizmo = l_GizmosManager->GetResource( l_GizmoName );

        if ( l_Gizmo )
            l_Gizmo->SetPosition( GetTargetPosition() );
        else
        {
            l_Gizmo = l_GizmosManager->CreateGizmo( l_GizmoName, GetTargetPosition(), 0.0f, 0.0f );
            l_GizmosManager->AddResource( l_GizmoName, l_Gizmo );

            CGizmoElement *l_Element = l_GizmosManager->CreateGizmoElement( CGizmoElement::eCube, 0.4f, Math::Vect3f( 0.0f ), 0.0f, 0.0f, Math::colRED );
            l_Gizmo->AddElement( l_Element );
        }
    }
}

void CEnemy::ChangeState( std::string NewState )
{
    m_NextState = NewState;
}

void CEnemy::Init()
{
    CCharacter::Init();
}

void CEnemy::Render()
{
}

void CEnemy::AddMesh( std::string MeshName )
{
    CRenderableObjectsManager *l_ROM = ROLMInstance->GetResource( "characters" );

    //m_pRenderableObject = new CInstanceMesh( m_Name, MeshName );
    m_pRenderableObject = new CAnimatedInstanceModel( m_Name, MeshName );
    l_ROM->AddResource( m_Name, m_pRenderableObject );
    m_Position = m_Controller->GetPosition();
    m_Position.y -=  m_Controller->GetHeight() / 2.0f;
    m_pRenderableObject->SetPosition( m_Position );
    m_pRenderableObject->SetYaw( m_fYaw );
    m_pRenderableObject->SetPitch( m_fPitch );
    m_pRenderableObject->SetRoll( m_fRoll );

    m_pRenderableObject->MakeTransform();
}

CAnimatedInstanceModel *CEnemy::GetAnimationModel()
{
    return dynamic_cast<CAnimatedInstanceModel *>( m_pRenderableObject );
}

void CEnemy::MakeShoot( Math::Vect3f aDirection )
{
    Math::Vect3f lPosition = GetPosition() + aDirection * 0.4f;
    lPosition.y += GetHeight() / 2.0f;

    CShoot *lShoot = new CShoot( 5.0f, aDirection, lPosition, 5.0f );
    lShoot->Init();

    mShoots.push_back( lShoot );
}

void CEnemy::MoveAStar( Math::Vect3f aTargetPos )
{
	Math::Vect3f lPos = GetPosition();

    CAStar *lAStar = EnemyMInstance->GetAStar();

	if (!mPathCalculated)
    {
		mPath = lAStar->GetPath( lPos, aTargetPos );
		mPathCalculated = true;
	}
    std::vector<Math::Vect3f>::iterator it = mPath.begin(),
                                        it_end = mPath.end();

    for ( ; it != it_end; ++it )
        it->y = 0.0f;

    Math::Vect3f lTargetPos = lPos;
	Math::Vect3f lPosAux = lPos;
	lPosAux.y = 0.0f;
    float lDist = lPosAux.Distance( mPath[1] );

	if (mPath.size() > 2)
	{
		if ( lDist < 0.6f )
			mPath.erase(mPath.begin()+1);
		lTargetPos = mPath[1];
	}
	lTargetPos.y = lPos.y;
	SetTargetPosition(lTargetPos);

    Math::Vect3f lDir = lTargetPos - lPos;
	lDir.y = 0.0;
    lDir.Normalize();

    Move( lDir, deltaTimeMacro );

	aTargetPos.y = 0.0;
	if (mPath[mPath.size()-1].Distance(aTargetPos) > 5.0f)
		mPathCalculated = false;
}

Math::Vect3f CEnemy::GetDirectionEnemy()
{
	float lYawRotate = m_fYaw + Math::half_pi32;
	return Math::Vect3f( Math::Utils::Cos( lYawRotate ) * Math::Utils::Cos( m_fPitch ), Math::Utils::Sin( lYawRotate ), Math::Utils::Sin( lYawRotate ) * Math::Utils::Cos( m_fPitch ) );
}