#pragma once
#ifndef INC_NAME_H_
#define INC_NAME_H_

#include <string>
#include "Defines.h"

class CName
{
public:
    CName(){}
    virtual ~CName() {}
	
	GET_SET(std::string,Name);

protected:
    std::string m_Name;
};

#endif // INC_NAME_H_