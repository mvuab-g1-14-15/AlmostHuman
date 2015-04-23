#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils/TemplatedVectorMapManager.h"
#include "Utils/Manager.h"
#include "Light.h"

#include <string>

class CGraphicsManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>, public CManager
{
public:
  CLightManager();
  CLightManager(CXMLTreeNode& atts);
  ~CLightManager();
  void	  Init();
  bool    Load( const std::string& FileName );
  size_t  GetLightCount();
  CLight* GetLight( size_t at );
  void	  Update(){}
  void    Render();
  void    GenerateShadowMap( CGraphicsManager* GM );
  bool    ReLoad();
};

#endif // LIGHT_MANAGER_H
