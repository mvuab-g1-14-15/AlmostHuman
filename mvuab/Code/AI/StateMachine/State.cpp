#include "State.h"

CState::CState( const std::string& Name ) : CName( Name )
{

}

CState::~CState()
{

}

bool CState::Load( CXMLTreeNode& Node)
{
	return true;
}