#include "State.h"
#include "Logger\Logger.h"
#include "Action.h"

CState::CState( const std::string& Name ) : CName( Name )
{
}

CState::~CState()
{
}

bool CState::Load( CXMLTreeNode& Node)
{
  int count = Node.GetNumChildren();

  for ( int i = 0; i < count; ++i )
  {
    CXMLTreeNode& l_CurrentNode = Node( i );
    const std::string& TagName = l_CurrentNode.GetName();
    
    if ( TagName == "on_enter" )
    {
      for ( int j = 0; j < l_CurrentNode.GetNumChildren(); ++j )
			{
        CXMLTreeNode& l_CurrentSubNode = l_CurrentNode( j );
		    const std::string& ActualTagName = l_CurrentSubNode.GetName();
        
        if ( ActualTagName == "action" )
        {
		      CAction* l_Action = new CAction(l_CurrentSubNode);
		      m_OnEnterActions.push_back(l_Action);
        }
      }
    }
	  if ( TagName == "on_exit" )
    {
      for ( int j = 0; j < l_CurrentNode.GetNumChildren(); ++j )
			{
        CXMLTreeNode& l_CurrentSubNode = l_CurrentNode( j );
		    const std::string& ActualTagName = l_CurrentSubNode.GetName();

        if ( ActualTagName == "action" )
        {
		      CAction* l_Action = new CAction(l_CurrentSubNode);
		      m_OnExitActions.push_back(l_Action);
        }
      }
    }
	  if ( TagName == "update" )
    {
		  for ( int j = 0; j < l_CurrentNode.GetNumChildren(); ++j )
      {
        CXMLTreeNode& l_CurrentSubNode = l_CurrentNode( j );
		    const std::string& ActualTagName = l_CurrentSubNode.GetName();

        if ( ActualTagName == "action" )
        {
		      CAction* l_Action = new CAction(l_CurrentSubNode);
		      m_UpdateActions.push_back(l_Action);
        }
      }
    }
  }
	return true;
}