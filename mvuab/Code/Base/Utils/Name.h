#pragma once
#ifndef INC_NAME_H_
#define INC_NAME_H_

#include <string>
#include "Defines.h"

class CName
{
public:
	CName(){}
	~CName() {}
protected:
	std::string m_Name;
	GET_SET(std::string,Name);
};

#endif // INC_NAME_H_