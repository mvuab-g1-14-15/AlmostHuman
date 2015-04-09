#include "Action.h"
#include "ScriptManager.h"
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
  ScriptMInstance->RunCode( m_LuaFunction );
}