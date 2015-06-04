#ifndef CINEMATIC_MANAGER_
#define CINEMATIC_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "CinematicsElement\CinematicsElement.h"
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
  void ReLoad();
  void Execute(const std::string& NameCinematic);
  void Update();
  void Render();
  
  GET_SET(bool, CinematicActive);

private:
  class CCinematicsItems
  {
  public:
    CTemplatedVectorMapManager<CCinematicsElement> m_CinematicsItems;
    CCinematicsItems( const std::string&  Node );
    void Execute();
    std::string GetNextName();
  };
  CTemplatedVectorMapManager<CCinematicsItems>    m_vCinematicsElement;
  CCinematicsItems*                               m_CurrentCinematicsElement;
  CCinematicsElement*                             m_CurrentElement;
  bool                                            m_CinematicActive;
  bool                                            m_CheckBlock;
  size_t                                          m_CurrentElementId;
  bool                                            m_FirstFrame;
  void CleanUp();
  std::string GetNextName(); 
};
#endif //CINEMATIC_MANAGER_