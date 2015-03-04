#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include "Utils\Types.h"
#include "Utils\MapManager.h"
#include "Enemy.h"

class CEnemy;
class CXMLTreeNode;
class CStateMachine;

class CEnemyManager : public CMapManager<CEnemy>
{
private:
    
    class CCoreEnemy
    {
      public:
      CCoreEnemy(){}
      CEnemy::EEnemyType m_EnemyType;
      float32 m_Life;
      float32 m_RespawnTime;
      float32 m_TimeToShoot;
      float32 m_ShootAccuracy;
      std::string m_StateMachineName;
      std::string m_StateMachineFileName;
    };

    std::string                  m_Filename;
    CMapManager<CStateMachine>   m_StateMachines;
    CMapManager<CCoreEnemy>      m_CoreEnemies;
    void Destroy();
    void AddNewStateMachine( const std::string &Name, const std::string &SMFileName );
    void AddNewCoreEnemy( CXMLTreeNode& Node );
    void AddNewEnemy( CXMLTreeNode& Node );
public:
    CEnemyManager();
    ~CEnemyManager();

    void Update();
    void Render();
    void Init(const std::string &Filename);
    void Reload();
    template<class T>
    static CEnemy * CreateTemplatedEnemy(CXMLTreeNode &XMLTreeNode);
};

#endif
