#ifndef STATE_H
#define STATE_H
#pragma once

#include "Utils\Name.h"
#include <string>
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"

#include <vector>

class CAction;

class CState : public CName
{
public:
	CState(const std::string& Name);
	~CState();
	virtual bool Load( CXMLTreeNode& Node);

  GET_SET(std::vector<CAction*>, OnEnterActions);
  GET_SET(std::vector<CAction*>, OnExitActions);
  GET_SET(std::vector<CAction*>, UpdateActions);

protected:
private: // Members
	std::vector<CAction*> m_OnEnterActions;
	std::vector<CAction*> m_OnExitActions;
	std::vector<CAction*> m_UpdateActions;
private: // Methods
};

#endif // CHARACTER_H
