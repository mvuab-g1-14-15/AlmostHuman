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
  bool Execute();
  void Update() {}
  void Render() {}


private:
  CTemplatedVectorMapManager<CCinematicsElement> m_CinematicsElement;

  void CleanUp();
  std::string GetNextName();
};
#endif //CINEMATIC_MANAGER_