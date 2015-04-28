#ifndef INC_BILLBOARDMANAGER_H_
#define INC_BILLBOARDMANAGER_H_

#include "Utils/Defines.h"
#include "Utils\Manager.h"
#include "Utils\MapManager.h"
#include "XML/XMLTreeNode.h"

class CBillboard;

class CBillboardManager : public CMapManager<CBillboard>, public CManager
{
public:

  CBillboardManager( const CXMLTreeNode& atts );
  virtual ~CBillboardManager();

  void Init();
  void Render();
  void Update();

private:

};

#endif // INC_CAMERAMANAGER_H_