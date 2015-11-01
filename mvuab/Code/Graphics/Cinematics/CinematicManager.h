#ifndef CINEMATIC_MANAGER_
#define CINEMATIC_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "CinematicsElement\CinematicsElement.h"
#include "Cameras\CameraKeyController.h"
#include "Cinematics\Cinematic.h"
#include <string>
#include "Utils/Manager.h"

class CPlayAnimationElement;

class CCinematicManager : public CManager
{
public:
  CCinematicManager();
  CCinematicManager( CXMLTreeNode& atts );
  ~ CCinematicManager();

  void Init();
  void LoadCinematics( const std::string& NameCinematic );
  void ReLoad();
  void Execute(const std::string& NameCinematic);
  void Update();
  void Render();
  void PlayCinematic( const std::string& aName );
  void StopCinematic( const std::string& aName );
  void ReloadCinematic( const std::string& aName );

  GET_SET(bool, CinematicActive);
  GET_SET(std::string, CurrentCamera); 

private:
  class CCinematicsItems
  {
  public:
    CTemplatedVectorMapManager<CCinematicsElement> m_CinematicsItems;
    CCinematicsItems( const std::string&  Node );
    void Execute();
    std::string GetNextName();
  };

  CTemplatedVectorMapManager<CCinematicsItems>     m_vCinematicsElement;
  CTemplatedVectorMapManager<CCinematic>		   m_vCinematicLoop;

  CCinematicsItems*								   m_CurrentCinematicsElement;
  CCinematicsElement*							   m_CurrentElement;

  //CCameraCinematical*							   m_CurrentCameraCinematical;
  std::string							           m_CurrentCamera;
  bool											   m_CinematicActive;

  bool											   m_CheckBlock;
  size_t										   m_CurrentElementId;
  bool											   m_FirstFrame;

  void CleanUp();
  std::string GetNextName(); 
};
#endif //CINEMATIC_MANAGER_