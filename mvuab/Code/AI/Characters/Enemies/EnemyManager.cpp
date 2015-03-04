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
    for(size_t i=0;i<m_Enemies.size();++i)
    {
        CHECKED_DELETE(m_Enemies[i]);
    }
    m_Enemies.clear();
}

void CEnemyManager::Update()
{

    for(size_t i=0;i<m_Enemies.size();++i)
        m_Enemies[i]->Update();
}

void CEnemyManager::Render()
{
    for(size_t i=0;i<m_Enemies.size();++i)
        m_Enemies[i]->Render();
}

void CEnemyManager::Init(const std::string &Filename)
{
    std::map<std::string, CreateEnemyFn> l_CreateEnemiesFnsMap;

    l_CreateEnemiesFnsMap["easy"]=CreateTemplatedEnemy<CEasyEnemy>;
    l_CreateEnemiesFnsMap["boss"]=CreateTemplatedEnemy<CBossEnemy>;
    l_CreateEnemiesFnsMap["patrol"]=CreateTemplatedEnemy<CPatrolEnemy>;

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
            std::string l_Name=m(i).GetName();
            if(l_Name=="enemy")
            {
                const std::string &l_Type=m(i).GetPszProperty("type");
                std::map<std::string, CreateEnemyFn>::iterator it=l_CreateEnemiesFnsMap.find(l_Type);
                if(it!=l_CreateEnemiesFnsMap.end())
                    m_Enemies.push_back(it->second(m(i)));
                else
                    printf("Enemy with type '%s' not correctly created", l_Type.c_str());
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