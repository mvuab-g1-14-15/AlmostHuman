#ifndef _BOSS_ENEMY_H
#define _BOSS_ENEMY_H

#include "Enemy.h"

#pragma once

class CBossEnemy : public CEnemy
{
public:
    CBossEnemy(CXMLTreeNode &Node);
    ~CBossEnemy();

    void Update(float32 deltaTime);
    void Render();
};


#endif

