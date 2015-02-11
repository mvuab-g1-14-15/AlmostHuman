#include "ECSManager.h"


CECSManager::CECSManager(void)
{
}


CECSManager::~CECSManager(void)
{
}

void CECSManager::Init(const std::string &xmlFile)
{
  //Parse the XML file of System
}

void CECSManager::Update(void)
{
  std::map<int, CSystem* >::iterator it = m_MapSystems.begin(),
                                     it_end = m_MapSystems.end();
  std::map<int, std::vector<CEntity*> >::iterator it_out;
  for(; it != it_end; ++it)
  {
    int l_Type = it->first;
    CSystem* l_System = it->second;
    it_out = m_MapEntities.find(l_Type);
    if(it_out != m_MapEntities.end())
      l_System->Update(m_MapEntities[l_Type]);
  }
}

void CECSManager::Render(void)
{
  std::map<int, CSystem* >::iterator it = m_MapSystems.begin(),
                                     it_end = m_MapSystems.end();
  std::map<int, std::vector<CEntity*> >::iterator it_out;
  for(; it != it_end; ++it)
  {
    int l_Type = it->first;
    CSystem* l_System = it->second;
    it_out = m_MapEntities.find(l_Type);
    if(it_out != m_MapEntities.end())
      l_System->Render(m_MapEntities[l_Type]);
  }
}

void CECSManager::AddEntity(CEntity* a_Entity)
{
  int l_EntityType = a_Entity->GetEntityType();
  std::map<int, std::vector<CEntity*> >::iterator it = m_MapEntities.find(l_EntityType);
  if( it != m_MapEntities.end() )
  {
    it->second.push_back(a_Entity);
  }
  else
  {
    std::vector<CEntity*> l_NewVector;
    l_NewVector.push_back(a_Entity);
    m_MapEntities[l_EntityType] = l_NewVector;
  }
}

void CECSManager::AddSystem(CSystem* a_System)
{
  int l_SystemType = a_System->GetSystemType();
  std::map<int, CSystem* >::iterator it = m_MapSystems.find(l_SystemType);
  if( it != m_MapSystems.end() )
  {
    CHECKED_DELETE(it->second);
    it->second = a_System;
  }
  else
  {
    m_MapSystems[l_SystemType] = a_System;
  }
}