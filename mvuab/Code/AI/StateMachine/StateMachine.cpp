#include "StateMachine.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "State.h"

CStateMachine::CStateMachine( const std::string& Name ) : CName( Name )
{
  Load(Name);
}

CStateMachine::~CStateMachine()
{
  Destroy();
}

bool CStateMachine::Load(const std::string& FileName)
{
	m_FileName = FileName;

	CXMLTreeNode l_File;

  if ( !l_File.LoadFile( m_FileName.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "CStateMachine::Load()->Reading the file %s", FileName.c_str() );
    return false;
  }

  CXMLTreeNode TreeNode = l_File["state_machine"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      CXMLTreeNode& l_CurrentNode = TreeNode( i );
      const std::string& TagName = l_CurrentNode.GetName();

      if ( TagName == "state" )
      {
        const std::string& l_Name = l_CurrentNode.GetPszProperty("name", "no_name");
        CState* l_State = new CState(l_Name);
        if(!l_State->Load(l_CurrentNode))
        {
          LOG_WARNING_APPLICATION( "CStateMachine::Error loading state %s", l_State->GetName().c_str() );
          CHECKED_DELETE(l_State);
        }
        if(!AddResource(l_Name, l_State))
        {
          LOG_WARNING_APPLICATION( "CStateMachine::state is already loaded %s", l_State->GetName().c_str() );
          CHECKED_DELETE(l_State);
        }
        else
            m_StateName.push_back(l_Name);
      }
    }
  }
  else
  {
    LOG_ERROR_APPLICATION(
                                           "RenderableObjectTechniqueManager::Load->Error trying to read the file: %s", FileName.c_str() );
  }

	return true;
}

bool CStateMachine::ReLoad()
{
	CleanUp();
	return Load(m_FileName);
}

void CStateMachine::CleanUp()
{

}