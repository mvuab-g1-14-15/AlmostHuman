#ifndef INC_CAMERAMANAGER_H_
#define INC_CAMERAMANAGER_H_

#include <map>
#include <string>
#include "Utils/Defines.h"
#include "Utils\Manager.h"
#include "Utils\MapManager.h"
#include "Camera.h"
#include "Math\Vector3.h"

class CTimer;
class CCamera;
class CFrustum;

class CCameraManager : public CMapManager<CCamera>, public CManager
{

public:

  CCameraManager();
  CCameraManager( CXMLTreeNode &atts);
  ~CCameraManager();

  void Init();

  void Update();

  CCamera* GetCamera( const std::string& name );
  CCamera* GetCurrentCamera( )
  {
    return m_pCurrentCamera;
  }
  void SetCurrentCamera( const std::string& name );
  std::string GetCurrentCameraName();

  // Create & Destroy Cameras
  void NewCamera( CCamera::Type id_cam, const std::string& name, const Math::Vect3f& Eye,
                  const Math::Vect3f& LookAt );
  void DeleteCamera( const std::string& name );
  void AddCamera( const std::string& name, CCamera* aCamera );
  void Render();

private:

  bool        m_RenderCameras;
  CCamera*    m_pCurrentCamera;

};

#endif // INC_CAMERAMANAGER_H_