#include "EnemyManager.h"
#include "Enemy.h"
#include "EasyEnemy.h"
#include "PatrolEnemy.h"
#include "BossEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include <map>
#include "Utils\Types.h"
#include "Timer\Timer.h"
#include "Core.h"
#include "StateMachine\StateMachine.h"
#include "Utils\Defines.h"

typedef CEnemy * (*CreateEnemyFn)(CXMLTreeNode &);

CEnemyManager::CEnemyManager()
{
}

CEnemyManager::~CEnemyManager()
{
    Destroy();
}

void CEnemyManager::Destroy()
{
  Destroy();
}

void CEnemyManager::Update()
{
}

void CEnemyManager::Render()
{
}

void CEnemyManager::Init(const std::string &Filename)
{
    std::map<std::string, CreateEnemyFn> l_CreateEnemiesFnsMap;

    /*l_CreateEnemiesFnsMap["easy"]=CreateTemplatedEnemy<CEasyEnemy>;
    l_CreateEnemiesFnsMap["boss"]=CreateTemplatedEnemy<CBossEnemy>;
    l_CreateEnemiesFnsMap["patrol"]=CreateTemplatedEnemy<CPatrolEnemy>;

    std::map<std::string, CreateEnemyFn>::iterator it=l_CreateEnemiesFnsMap.find(l_Type);
    if(it!=l_CreateEnemiesFnsMap.end())
    */

    m_Filename=Filename;
    CXMLTreeNode l_File;
    if (!l_File.LoadFile(m_Filename.c_str()))
    {
        printf("File '%s' not correctly loaded", Filename.c_str());
    }
    CXMLTreeNode  m=l_File["enemies"];
    if(m.Exists())
    {
        int count=m.GetNumChildren();
        for (int i = 0; i < count; ++i)
        {
            const std::string& l_TagName = m(i).GetName();
            if(l_TagName=="enemy")
            {
              AddNewEnemy(m(i));
            } 
            else if(l_TagName=="core_enemy")
            {
              AddNewCoreEnemy(m(i));
              //<core_enemy type="easy" life="50" time_to_respawn="2.0" time_to_shoot="5.0" shoot_accuracy="0.35" state_machine="AI.xml"/>
            }
        }
    }
}

void CEnemyManager::Reload()
{
    Destroy();
    Init(m_Filename);
}

template<class T>
CEnemy * CEnemyManager::CreateTemplatedEnemy(CXMLTreeNode &XMLTreeNode)
{
    return new T(XMLTreeNode);
}

void CEnemyManager::AddNewStateMachine( const std::string &Name, const std::string &SMFileName )
{
  CStateMachine* lStateMachine = new CStateMachine(Name);

  if( lStateMachine->Load(SMFileName) )
  {
    m_StateMachines.AddResource( Name,  lStateMachine );
  }
  else
  {
    CHECKED_DELETE(lStateMachine);
    LOG_ERROR_APPLICATION("Adding a new state machine");
  }
}

void CEnemyManager::AddNewCoreEnemy( CXMLTreeNode& Node )
{
  CCoreEnemy* lCoreEnemy = new CCoreEnemy();

  // Read the type
  const std::string lType = Node.GetPszProperty("type", "no_type");
  if(lType == "easy")
  {
    lCoreEnemy->m_EnemyType = CEnemy::eEasy;
  }
  else if (lType == "patroll")
  {
    lCoreEnemy->m_EnemyType = CEnemy::ePatroll;
  }
  else if (lType == "boss")
  {
    lCoreEnemy->m_EnemyType = CEnemy::eBoss;
  }

  lCoreEnemy->m_Life = Node.GetFloatProperty("life", 0.0f);
  lCoreEnemy->m_RespawnTime = Node.GetFloatProperty("time_to_respawn", 0.0f);
  lCoreEnemy->m_TimeToShoot= Node.GetFloatProperty("time_to_shoot", 0.0f);
  lCoreEnemy->m_ShootAccuracy= Node.GetFloatProperty("shoot_accuracy", 0.0f);
  lCoreEnemy->m_StateMachineName = Node.GetPszProperty("state_machine_name", "no_name");
  lCoreEnemy->m_StateMachineFileName = Node.GetPszProperty("state_machine_file", "no_name");
  // Now add the new state machine
  AddNewStateMachine( lCoreEnemy->m_StateMachineName, lCoreEnemy->m_StateMachineFileName );

  m_CoreEnemies.AddResource(lType, lCoreEnemy);
}

void CEnemyManager::AddNewEnemy( CXMLTreeNode& Node )
{
  const std::string &lType=Node.GetPszProperty("type");

  CCoreEnemy* lCoreEnemy = m_CoreEnemies.GetResource(lType);

  if(lCoreEnemy)
  {
    CEnemy* lEnemy = 0;
    if(lType == "easy")
    {
      //lEnemy = new C
    }
    else if (lType == "patroll")
    {
      //lCoreEnemy->m_EnemyType = CEnemy::ePatroll;
    }
    else if (lType == "boss")
    {
      //lCoreEnemy->m_EnemyType = CEnemy::eBoss;
    }
  }
  else
  {
    LOG_ERROR_APPLICATION(( "Core '%s' not found", lType.c_str()));
  }
}