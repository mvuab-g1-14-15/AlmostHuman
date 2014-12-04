#pragma once

#include <string>
#include "Defines.h"

class CName
{
public:
	CName();
	~CName() {}
private:
	std::string m_Name;
	GET_SET(std::string,Name);
};


