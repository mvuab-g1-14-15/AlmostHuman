#pragma once
#ifndef INC_VISIBLE_H_
#define INC_VISIBLE_H_

class CVisible
{
public:
	CVisible(){}
    ~CVisible() {}
	
	GET_SET(bool,Visible);

protected:
    bool m_Visible;
};

#endif 