#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Utils\MapManager.h"
#include "Utils\Manager.h"
#include "Enemy.h"
#include "Utils\ObjectFactory.h"

class CEnemy;
class CXMLTreeNode;
class CStateMachine;
class CAStar;

class CEnemyManager : public CMapManager<CEnemy>, public CManager
{
private:

    class CCoreEnemy
    {
    public:
        CCoreEnemy() {}
        CEnemy::EEnemyType m_EnemyType;
        float32 m_Life;
        float32 m_RespawnTime;
        float32 m_TimeToShoot;
        float32 m_ShootAccuracy;
        std::string m_StateMachineName;
        std::string m_StateMachineFileName;
    };

    CMapManager<CStateMachine>   m_StateMachines;
    CMapManager<CCoreEnemy>      m_CoreEnemies;
    std::map<unsigned int, std::vector<Math::Vect3f>> m_Routes;

    CEnemy                  *m_ActualEnemy;

    void Destroy();
    void AddNewStateMachine( const std::string& Name, const std::string& SMFileName );
    void AddNewCoreEnemy( CXMLTreeNode& Node );
    void AddNewEnemy( CXMLTreeNode& Node );
    void AddNewRoute( CXMLTreeNode& Node );

    ObjectFactory2<CEnemy, CXMLTreeNode, CStateMachine *, std::string > EnemyFactory;
    void RegisterEnemies();

    CAStar *mAStar;

public:
    CEnemyManager();
    CEnemyManager( CXMLTreeNode& atts );
    ~CEnemyManager();

    void Update();
    void Render();
    void Init();
    void Reload();
    template<class T>
    static CEnemy *CreateTemplatedEnemy( CXMLTreeNode& XMLTreeNode );
    CEnemy *GetActualEnemy();

    CAStar *GetAStar()
    {
        return mAStar;
    }
};

#endif
