#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#pragma once

#include "Utils\Defines.h"
#include "Utils\Name.h"
#include "Utils\MapManager.h"
#include "State.h"
#include <string>

class CStateMachine : public CName, public CMapManager<CState>
{
public:
	CStateMachine(const std::string& Name);
	~CStateMachine();
	bool Load(const std::string& FileName);
	bool ReLoad();
	void CleanUp();
protected: // Members
protected: // Methods
private: // Members
	std::string m_FileName;
  
private: // Methods
};

#endif // STATE_MACHINE_H
