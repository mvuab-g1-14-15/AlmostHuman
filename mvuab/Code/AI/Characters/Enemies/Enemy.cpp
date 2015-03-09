#include "Enemy.h"
#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "ScriptManager\ScriptManager.h"
#include "Core.h"
#include "StateMachine\Action.h"
#include "Utils\MapManager.h"

CEnemy::CEnemy( CXMLTreeNode& Node )
  : CCharacter( Node.GetPszProperty( "name", "no_name" ) ), m_CurrentState( "inicial" )
  , m_OnEnter(true)
  , m_OnExit(false)
{
  CCharacter::Init( Node );
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update()
{
  CScriptManager* l_SM = CCore::GetSingletonPtr()->GetScriptManager();
  CState* l_State = m_pStateMachine->GetResource( m_CurrentState );
  std::vector<CAction*> l_Actions;
  
  if(m_OnEnter)
  {
      m_OnExit = false;
      l_Actions = l_State->GetOnEnterActions();
      m_OnEnter = false;
  }
  else if(m_OnExit)
  {
      l_Actions = l_State->GetOnExitActions();
      m_OnEnter = true;
  }
  else
  {
      l_Actions = l_State->GetUpdateActions();
  }
  std::vector<CAction*>::iterator it = l_Actions.begin(),
                                  it_end = l_Actions.end();

  for ( ; it != it_end; ++it )
    l_SM->RunCode( ( *it )->GetLuaFunction() + "()" );
}

void CEnemy::ChangeState( std::string NewState )
{
  m_CurrentState = NewState;
}

void CEnemy::Init()
{
}

void CEnemy::Render()
{
}