#include "Enemy.h"
#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "ScriptManager\ScriptManager.h"
#include "Core.h"
#include "StateMachine\Action.h"

CEnemy::CEnemy(CXMLTreeNode &Node)
	: CCharacter(Node.GetPszProperty("name", "no_name") )
{
	CCharacter::Init(Node);
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update()
{
  CScriptManager* l_SM = CCore::GetSingletonPtr()->GetScriptManager();
	CState* l_State = m_pStateMachine->GetResource(m_CurrentState);
  std::vector<CAction*> l_Actions = l_State->GetUpdateActions();
  std::vector<CAction*>::iterator it = l_Actions.begin(),
                                  it_end = l_Actions.end();
  for(; it != it_end; ++it)
    l_SM->RunCode((*it)->GetLuaFunction());
}

void CEnemy::ChangeState(std::string NewState)
{
  CScriptManager* l_SM = CCore::GetSingletonPtr()->GetScriptManager();
	CState* l_State = m_pStateMachine->GetResource(m_CurrentState);
  std::vector<CAction*> l_Actions = l_State->GetOnExitActions();
  std::vector<CAction*>::iterator it = l_Actions.begin(),
                                  it_end = l_Actions.end();
  for(; it != it_end; ++it)
    l_SM->RunCode((*it)->GetLuaFunction());

  m_CurrentState = NewState;

  l_State = m_pStateMachine->GetResource(m_CurrentState);
  l_Actions = l_State->GetOnEnterActions();
  it = l_Actions.begin();
  it_end = l_Actions.end();
  for(; it != it_end; ++it)
    l_SM->RunCode((*it)->GetLuaFunction());
}

void CEnemy::Init()
{
}

void CEnemy::Render()
{
}