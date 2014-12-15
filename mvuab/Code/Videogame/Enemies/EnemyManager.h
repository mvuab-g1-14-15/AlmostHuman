#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include "Utils\Types.h"

class CEnemy;
class CXMLTreeNode;

class CEnemyManager
{
private:
    std::vector<CEnemy*>                m_Enemies;
    std::string                            m_Filename;
    
    void Destroy();
public:
    CEnemyManager();
    ~CEnemyManager();

    void Update(float32 deltaTime);
    void Render();
    void Init(const std::string &Filename);
    void Reload();

    /*static CEnemy * CreateEasyEnemy(CXMLTreeNode &XMLTreeNode);
    static CEnemy * CreateBossEnemy(CXMLTreeNode &XMLTreeNode);
    static CEnemy * CreatePatrolEnemy(CXMLTreeNode &XMLTreeNode);*/
    template<class T>
    static CEnemy * CreateTemplatedEnemy(CXMLTreeNode &XMLTreeNode);
};

#endif
