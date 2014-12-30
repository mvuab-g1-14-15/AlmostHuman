#ifndef INC_CAMERAMANAGER_H_
#define INC_CAMERAMANAGER_H_

#include <map>
#include <string>
#include "Utils/Defines.h"
#include "Utils\SingletonPattern.h"
#include "Utils\MapManager.h"
#include "Camera.h"
#include "Math\Vector3.h"

class CTimer;
class CCamera;
class CFrustum;

class CCameraManager : public CMapManager<CCamera>, public CSingleton<CCameraManager>
{

public:

    CCameraManager(); 
    ~CCameraManager();
    
    void Update();

    CCamera* GetCamera( const std::string &name );
    GET_SET_PTR( CCamera, CurrentCamera);
    void SetCurrentCamera( const std::string &name );
    std::string GetCurrentCameraName();

    // Create & Destroy Cameras
    void NewCamera( CCamera::Type id_cam, const std::string &name, const Math::Vect3f &Eye, const Math::Vect3f &LookAt);
    void DeleteCamera( const std::string &name );
    void RenderCameras();

private:

    bool        m_RenderCameras;
    CCamera    *m_pCurrentCamera;

};

#endif // INC_CAMERAMANAGER_H_