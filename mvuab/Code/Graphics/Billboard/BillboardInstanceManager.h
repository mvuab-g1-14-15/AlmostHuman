#ifndef INC_BillboardInstanceManager_H_
#define INC_BillboardInstanceManager_H_

#include "Utils/Defines.h"
#include "Utils\Manager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "XML/XMLTreeNode.h"

class CBillboardInstance;

class CBillboardInstanceManager : public CTemplatedVectorMapManager<CBillboardInstance>, public CManager
{
public:
  CBillboardInstanceManager( const CXMLTreeNode& atts );
  virtual ~CBillboardInstanceManager();

  void Init();
  void Render();
  void Update();
};

#endif // INC_CAMERAMANAGER_H_