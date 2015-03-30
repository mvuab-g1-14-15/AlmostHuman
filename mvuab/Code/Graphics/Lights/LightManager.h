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
  bool    Load( const std::string& FileName );
  size_t  GetLightCount();
  CLight* GetLight( size_t at );
  void    Render();
  void    GenerateShadowMap( CGraphicsManager* GM );
  bool    ReLoad();
private:
  std::string m_Filename;
};

#endif // LIGHT_MANAGER_H
