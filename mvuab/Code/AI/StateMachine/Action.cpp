#include "Action.h"
#include "ScriptManager.h"

#include "EngineManagers.h"

CAction::CAction( CXMLTreeNode& Node )
    : m_LuaFunction( Node.GetAttribute<std::string>( "function", "no_function" ) )
{
}

CAction::~CAction()
{
}

void CAction::Execute()
{
    ScriptMInstance->RunCode( m_LuaFunction );
}