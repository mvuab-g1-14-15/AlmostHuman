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
}

bool CStateMachine::Load(const std::string& FileName)
{
	m_FileName = FileName;

	CXMLTreeNode l_File;

  if ( !l_File.LoadFile( m_FileName.c_str() ) )
  {
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR, "CStateMachine::Load()->Reading the file %s", FileName.c_str() );
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
        CState* l_State = new CState(l_CurrentNode.GetPszProperty("name", "no_name"));
        if(!l_State->Load(l_CurrentNode))
        {
          CLogger::GetSingletonPtr()->AddNewLog( ELL_WARNING, "CStateMachine::Error loading state %s", l_State->GetName().c_str() );
          CHECKED_DELETE(l_State);
        }
      }
    }
  }
  else
  {
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
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