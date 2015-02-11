#ifndef _ECS_MANAGER_H
#define _ECS_MANAGER_H

#include "Component.h"

#include "Utils\Defines.h"
#include "Entity.h"
#include "System.h"

#include <string>
#include <vector>
#include <map>

class CECSManager
{
public:
	CECSManager(void);
	virtual ~CECSManager(void);

  void Init(const std::string &xmlFile);
  void Update(void);
  void Render(void);

  void AddEntity(CEntity*);
  void AddSystem(CSystem*);

private:
  std::map<int, std::vector<CEntity*> > m_MapEntities;
  std::map<int, CSystem*> m_MapSystems;
};

#endif //_ECS_MANAGER_H