#ifndef _EASY_ENEMY_H
#define _EASY_ENEMY_H

#include "Enemy.h"

#pragma once
class CStateMachine;

class CEasyEnemy : public CEnemy
{
public:
    CEasyEnemy(CXMLTreeNode &Node, CStateMachine* aStateMachine);
    ~CEasyEnemy();
	void Init(){};
    void Update();
    void Render();
};
#endif
