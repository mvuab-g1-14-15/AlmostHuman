#ifndef ACTION_H
#define ACTION_H
#pragma once

#include "XML\XMLTreeNode.h"
#include <string>

class CAction
{
public:
	CAction(CXMLTreeNode &Node);
	~CAction();
	virtual void Execute();
protected:
	std::string m_LuaFunction;
private: // Members
private: // Methods
};

#endif // ACTION_H
