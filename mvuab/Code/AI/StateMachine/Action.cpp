#include "Action.h"
#include "ScriptManager/ScriptManager.h"
#include "Core.h"

CAction::CAction( CXMLTreeNode& Node )
  : m_LuaFunction( Node.GetPszProperty( "function", "no_function" ) )
{
}

CAction::~CAction()
{
}

void CAction::Execute()
{
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( m_LuaFunction );
}