#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils/TemplatedVectorMapManager.h"
#include "Utils/SingletonPattern.h"
#include "Light.h"

#include <string>

class CGraphicsManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>, public CSingleton<CLightManager>
{
  public:
    CLightManager();
    ~CLightManager();
    void Load(const std::string &FileName);
    CLight* GetLight( size_t at );
    void Render();
};

#endif // LIGHT_MANAGER_H
