#pragma once

#ifndef _PATROL_ENEMY_H_
#define _PATROL_ENEMY_H_

#include <vector>
#include "Math\Vector3.h"
#include "Enemy.h"

class CPatrolEnemy : public CEnemy
{
private:
    std::vector<Math::Vect3f>                    m_Waypoints;
public:
    CPatrolEnemy(CXMLTreeNode &Node);
    ~CPatrolEnemy();
	void Init();
    void Update();
    void Render();
};

#endif
