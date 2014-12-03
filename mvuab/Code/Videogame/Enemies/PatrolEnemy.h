#pragma once

#ifndef _PATROL_ENEMY_H_
#define _PATROL_ENEMY_H_

#include <vector>
#include "Math\Vector3.h"
#include "Enemy.h"

class CPatrolEnemy : public CEnemy
{
private:
	std::vector<Vect3f>					m_Waypoints;
public:
	CPatrolEnemy(CXMLTreeNode &Node);
	~CPatrolEnemy();

	void Update(float32 deltaTime);
	void Render();
};

#endif
