#ifndef ACTION_H
#define ACTION_H
#pragma once

#include "Utils\Name.h"
#include "XML\XMLTreeNode.h"
#include <string>

class CAction: public CName
{
public:
	CAction(const std::string& Name);
	~CAction();
	virtual bool Load(CXMLTreeNode &Node);
	virtual void Execute();
protected:
	std::string m_LuaFuntion;
private: // Members
private: // Methods
};

#endif // ACTION_H
