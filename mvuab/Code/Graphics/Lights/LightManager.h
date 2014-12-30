#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils/TemplatedVectorMapManager.h"
#include "Light.h"

#include <string>

class CGraphicsManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>
{
public:
	CLightManager();
	~CLightManager();
	void Load(const std::string &FileName);
	void Render(CGraphicsManager *gm);
};

#endif // LIGHT_MANAGER_H
