#ifndef CINEMATIC_MANAGER_
#define CINEMATIC_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "CinematicsElement\CinematicsElement.h"
#include <string>
#include "Utils/Manager.h"

class CCinematicManager : public CManager
{
public:
  CCinematicManager();
  CCinematicManager( CXMLTreeNode& atts );
  ~ CCinematicManager();

  void Init();
  void ReLoad();
  void Execute(const std::string& NameCinematic);
  void Update() {}
  void Render() {}


private:
  class CCinematicsItems
  {
  public:
    std::vector<CCinematicsElement*> m_CinematicsItems;
    CCinematicsItems( const std::string&  Node );
    void Execute();
  };
  CTemplatedVectorMapManager<CCinematicsItems>  m_vCinematicsElement;
  void CleanUp();
  std::string GetNextName();
};
#endif //CINEMATIC_MANAGER_