#include "Action.h"

CAction::CAction( const std::string& Name ) : CName( Name )
{
}

CAction::~CAction()
{
}

bool CAction::Load(CXMLTreeNode &Node)
{
	return true;
}

void CAction::Execute()
{
}