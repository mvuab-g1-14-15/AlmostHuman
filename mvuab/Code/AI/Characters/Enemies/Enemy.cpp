#include "Enemy.h"
#include "Utils\Defines.h"
#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "ScriptManager.h"
#include "Core.h"
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

CEnemy::CEnemy( CXMLTreeNode& Node, CStateMachine* aStateMachine )
  : CCharacter( Node.GetPszProperty( "name", "no_name" ) )
  , m_CurrentState( "inicial" )
  , m_NextState( "inicial" )
  , m_OnEnter( true )
  , m_OnExit( false )
  , m_pStateMachine( aStateMachine )
  , m_pRenderableObject( NULL )
{
  CCharacter::Init( Node );
}

CEnemy::~CEnemy()
{
  CRenderableObjectsManager* l_ROM = CRenderableObjectsLayersManager::GetSingletonPtr()->GetResource( "characters" );
  l_ROM->RemoveResource( m_Name );
  CPhysicsManager::GetSingletonPtr()->ReleasePhysicController( m_Controller );
}

void CEnemy::Update()
{
  CScriptManager* l_SM = CCore::GetSingletonPtr()->GetScriptManager();

  if (m_CurrentState != m_NextState)
    m_OnExit = true;

  CState* l_State = m_pStateMachine->GetResource( m_CurrentState );
  std::vector<CAction*> l_Actions;

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

  std::vector<CAction*>::iterator it = l_Actions.begin(),
                                  it_end = l_Actions.end();

  for ( ; it != it_end; ++it )
    l_SM->RunCode( ( *it )->GetLuaFunction() + "()" );

  m_Position = m_Controller->GetPosition();
  m_Position.y -=  m_Controller->GetHeight() / 2.0f;
  m_pRenderableObject->SetPosition( m_Position );
  m_pRenderableObject->SetYaw( m_fYaw - Math::pi32 * 0.5f );
  m_pRenderableObject->SetPitch( m_fPitch );
  m_pRenderableObject->SetRoll( m_fRoll );

  m_pRenderableObject->MakeTransform();

  Math::Vect3f l_Pos = m_Position;
  l_Pos.y += m_Controller->GetHeight();

  CGizmosManager* l_GizmosManager = CCore::GetSingletonPtr()->GetGizmosManager();
  std::ostringstream ss;
  ss << GetName() << "HeadGizmo";
  std::string l_GizmoName( ss.str() );
  CGizmo* l_Gizmo = l_GizmosManager->GetResource(l_GizmoName);
  if (l_Gizmo)
  {
    l_Gizmo->SetPosition(l_Pos);
    l_Gizmo->SetYaw(m_fYaw);
    l_Gizmo->SetPitch(m_fPitch);
  }
  else
  {
    l_Gizmo = l_GizmosManager->CreateGizmo(l_GizmoName, l_Pos, m_fYaw, m_fPitch);
    CGizmoElement* l_Element = l_GizmosManager->CreateGizmoElement(CGizmoElement::eCube, 0.2f, Math::Vect3f(0.0f), 0.0f, 0.0f);
    l_Gizmo->AddElement(l_Element);
    l_GizmosManager->AddResource(l_GizmoName, l_Gizmo);
  }
}

void CEnemy::ChangeState( std::string NewState )
{
  m_NextState = NewState;
}

void CEnemy::Init()
{
}

void CEnemy::Render()
{
}

void CEnemy::AddMesh( std::string MeshName )
{
  CRenderableObjectsManager* l_ROM = CRenderableObjectsLayersManager::GetSingletonPtr()->GetResource( "characters" );

  m_pRenderableObject = new CInstanceMesh( m_Name, MeshName );
  l_ROM->AddResource( m_Name, m_pRenderableObject );
  m_Position = m_Controller->GetPosition();
  m_Position.y -=  m_Controller->GetHeight() / 2.0f;
  m_pRenderableObject->SetPosition( m_Position );
  m_pRenderableObject->SetYaw( m_fYaw - Math::pi32 * 0.5f );
  m_pRenderableObject->SetPitch( m_fPitch );
  m_pRenderableObject->SetRoll( m_fRoll );

  m_pRenderableObject->MakeTransform();
}