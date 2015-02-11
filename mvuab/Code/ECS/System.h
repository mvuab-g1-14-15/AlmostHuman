#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Component.h"

#include "Utils\Defines.h"

#include <vector>

class CEntity;

class CSystem
{
public:
	CSystem(void);
	virtual ~CSystem(void);

  virtual void Init() = 0;
  virtual void Update(std::vector<CEntity*>) = 0;
  virtual void Render(std::vector<CEntity*>) = 0;

  GET_SET(int, SystemType);

private:
  int m_SystemType;
};

#endif //_SYSTEM_H