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

void CState::FillActions(  CXMLTreeNode& Node, std::vector<CAction*>& aActions )
{
    for( CXMLTreeNode lChildNode = Node.FirstChild(); lChildNode.More(); lChildNode = lChildNode.NextChild() )
    {
        if ( lChildNode.GetName() == "action" )
        {
            CAction* l_Action = new CAction(lChildNode);
            aActions.push_back(l_Action);
        }
    }
}

bool CState::Load( CXMLTreeNode& Node)
{
    bool lOk(Node.IsOk());
    if( lOk )
    {
        for( CXMLTreeNode lChildNode = Node.FirstChild(); lChildNode.More(); lChildNode = lChildNode.NextChild() )
        {
            const std::string & lTagName = lChildNode.GetName();
            if ( lTagName == "on_enter" )
            {
                FillActions(lChildNode, m_OnEnterActions);
            }
            if ( lTagName == "on_exit" )
            {
                FillActions(lChildNode, m_OnExitActions);
            }
            if ( lTagName == "update" )
            {
                FillActions(lChildNode, m_UpdateActions);
            }
        }
    }
    return lOk;
}