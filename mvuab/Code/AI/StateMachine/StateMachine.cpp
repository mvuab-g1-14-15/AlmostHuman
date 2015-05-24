#include "StateMachine.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "XML\XMLDocument.h"
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
    bool lOk(true);
    m_FileName = FileName;
    CXMLDocument lXMLDoc( m_FileName );
    if( lXMLDoc.Load() )
    {
        CXMLTreeNode lRootNode = lXMLDoc.GetNode("state_machine");
        for( CXMLTreeNode lChildNode = lRootNode.FirstChild(); lChildNode.More(); lChildNode = lChildNode.NextChild() )
        {
            const std::string & lTagName = lChildNode.GetName();
            if ( lTagName == "state" )
            {
                std::string lName;
                lOk = lOk && lChildNode.GetAttribute<std::string>("name", lName );
                CState* l_State = new CState(lName);
                if(!l_State->Load(lChildNode))
                {
                    LOG_WARNING_APPLICATION( "CStateMachine::Error loading state %s", l_State->GetName().c_str() );
                    CHECKED_DELETE(l_State);
                }
                if(!AddResource(lName, l_State))
                {
                    LOG_WARNING_APPLICATION( "CStateMachine::state is already loaded %s", l_State->GetName().c_str() );
                    CHECKED_DELETE(l_State);
                }
            }
        }
    }

    ASSERT(lOk, "Error loading the state machines");
}

bool CStateMachine::ReLoad()
{
    CleanUp();
    return Load(m_FileName);
}

void CStateMachine::CleanUp()
{

}