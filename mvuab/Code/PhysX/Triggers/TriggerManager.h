//----------------------------------------------------------------------------------
// CTriggerManager class
//
// Description:
// Clase Manager de Triggers de PhysX.
//----------------------------------------------------------------------------------

#pragma once

#ifndef __TRIGGER_MANAGER_H__
#define __TRIGGER_MANAGER_H__

#include <string>
#include <vector>
#include <map>
#include "Reports\PhysicTriggerReport.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Manager.h"
#include "Actor\PhysicActor.h"
#include "Trigger.h"

// Forward declarations
class CPhysicUserData;
//class CXMLTreeNode;
//class ItemCore;

class CTriggerManager : public CTemplatedVectorMapManager<CTrigger>, public CManager
{
private: // Members
private: // Methods

public:
  CTriggerManager();
  CTriggerManager(CXMLTreeNode& atts);
  ~CTriggerManager();

  void								  Init();
  void								  Update(){}
  void								  Render();
  bool                                LoadXML( const std::string& FileName );
  bool                                Reload();
  bool                                ReloadXML( const std::string& FileName );
  std::vector<CTrigger*>              GetTriggersVector();
  CTrigger*                           GetTriggerByName( std::string Name );
  std::string                         GetLUAByName( std::string Name,
      u_int Type );
  void                                Clear();
  void                                Release();
  void                                ReleaseTrigger( const std::string& _trigger );

  void                                AddTrigger                  (CTrigger* lTrigger );

  // Métodos para Luabind
  int                                 GetValue() const;
  void                                SetValue( const unsigned int& v );
};

#endif __TRIGGER_MANAGER_H__
