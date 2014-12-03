#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

class CEnemy
{
protected:
	std::string					m_Name;
	float32						m_Life;
	float32						m_Yaw;
	Vect3f						m_Position;
public:
	CEnemy(CXMLTreeNode &Node);
	virtual ~CEnemy();
	
	GET_SET_REF(std::string, Name);
	GET_SET(float32, Life);
	GET_SET(float32, Yaw);
	GET_SET_REF(Vect3f, Position);
	
	virtual void Update(float32 deltaTime) = 0;
	virtual void Render() = 0;
};

#endif

