#include "Entity.h"

#include "Utils\Defines.h"

CEntity::CEntity(void)
  : m_EntityType( 0 )
{
}


CEntity::~CEntity(void)
{
	std::map<ComponentType, CComponent*>::iterator it = m_Components.begin(),
													                       it_end = m_Components.end();
  for(; it != it_end; ++it)
  {
    CHECKED_DELETE( it->second );
  }
  m_Components.clear();
}

void CEntity::AddComponent(CComponent* a_Component)
{
  ComponentType l_ComponentType = a_Component->GetComponentType();
  std::map<ComponentType, CComponent*>::iterator it = m_Components.find(l_ComponentType);
  if(it != m_Components.end())
  {
    CHECKED_DELETE(it->second);
    it->second = a_Component;
  }
  else
  {
    m_Components[l_ComponentType] = a_Component;
    m_EntityType |= l_ComponentType;
  }
}

void CEntity::RemoveComponent(const ComponentType a_ComponentType)
{
  std::map<ComponentType, CComponent*>::iterator it = m_Components.find(a_ComponentType);
  if(it != m_Components.end())
  {
    CHECKED_DELETE(it->second);
    m_Components.erase(it);
    m_EntityType &= ~a_ComponentType;
  }
}

CComponent* CEntity::GetComponent(const ComponentType a_ComponentType)
{
  std::map<ComponentType, CComponent*>::iterator it = m_Components.find(a_ComponentType);
  if(it != m_Components.end())
  {
    return it->second;
  }
  return 0;
}

int CEntity::GetEntityType() const
{
  return m_EntityType;
}