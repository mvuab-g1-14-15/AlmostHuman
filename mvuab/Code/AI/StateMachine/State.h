#ifndef STATE_H
#define STATE_H
#pragma once

#include "Utils\Name.h"
#include <string>
#include "XML\XMLTreeNode.h"

class CState : public CName
{
public:
	CState(const std::string& Name);
	~CState();
	virtual bool Load( CXMLTreeNode& Node);
protected:
private: // Members
private: // Methods
};

#endif // CHARACTER_H
