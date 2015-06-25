#include "State.h"
#include "Logger\Logger.h"
#include "Action.h"

CState::CState( const std::string& Name ) : CName( Name )
{
}

CState::~CState()
{
    for ( size_t i = 0; i < m_OnEnterActions.size(); ++i )
    {
        CHECKED_DELETE( m_OnEnterActions[i] );
    }

    m_OnEnterActions.clear();

    for ( size_t i = 0; i < m_OnExitActions.size(); ++i )
    {
        CHECKED_DELETE( m_OnExitActions[i] );
    }

    m_OnExitActions.clear();

    for ( size_t i = 0; i < m_UpdateActions.size(); ++i )
    {
        CHECKED_DELETE( m_UpdateActions[i] );
    }

    m_UpdateActions.clear();
}

void ReadTriggerAction(const CXMLTreeNode& aNode,  std::vector<CAction*>& aActions )
{
    for ( uint32 j = 0, lCount = aNode.GetNumChildren() ; j < lCount ; ++j )
    {
        CXMLTreeNode& l_CurrentSubNode = aNode( j );
        const std::string& ActualTagName = l_CurrentSubNode.GetName();

        if ( ActualTagName == "action" )
        {
            CAction* l_Action = new CAction(l_CurrentSubNode);
            aActions.push_back(l_Action);
        }
    }
}

bool CState::Load( CXMLTreeNode& Node)
{
    for ( uint32 i = 0, lCount = Node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& l_CurrentNode = Node( i );
        const std::string& TagName = l_CurrentNode.GetName();
        if ( TagName == "on_enter" )
        {
            ReadTriggerAction( l_CurrentNode, m_OnEnterActions );
        }
        else if ( TagName == "on_exit" )
        {
            ReadTriggerAction( l_CurrentNode, m_OnExitActions );
        }
        else if ( TagName == "update" )
        {
            ReadTriggerAction( l_CurrentNode, m_UpdateActions );
        }
    }
    return true;
}