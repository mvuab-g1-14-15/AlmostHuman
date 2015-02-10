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
#include "Actor\PhysicActor.h"

//---Forward Declarations---//
class CPhysicUserData;
class CXMLTreeNode;
class ItemCore;
//--------------------------

struct sTrigger
{
        std::string             enter;
        std::string             exit;   
        std::string             stay;   
};

class CTriggerManager : public CTemplatedVectorMapManager<CPhysicActor>
{
public:
        enum TTriggerType
        {
                ENTER=0,
                LEAVE,
                STAY
        };

private:
        std::string                                     m_FileName;
        std::vector<CPhysicUserData *>					m_UserData;
        std::map<std::string, sTrigger>					m_LUAMap;
        
        // Luabind value
        int                                             m_Value;
public:
        CTriggerManager ();
        ~CTriggerManager();

        bool                                LoadXML                     ( const std::string &FileName );
        bool                                Reload                      ();
        bool                                ReloadXML                   ( const std::string &FileName );
        std::vector<CPhysicActor*>          GetTriggersVector           ();
        CPhysicActor*                       GetTriggerByName            ( std::string Name );
        std::string                         GetLUAByName                ( std::string Name, u_int Type );
        TTriggerType                        GetTypeByName               ( std::string Name );
        void                                Clear                       ();
        void                                Release                     ();
        void                                ReleaseTrigger              (const std::string &_trigger);
        
		//void                                AddTrigger                  (CXMLTreeNode XMLData, ItemCore *Core, Vect3f Pos,sTrigger TriggerLua);
        //void                                AddTriggerWithoutXML        (std::string Name, ItemCore *Core, Vect3f Pos);

		void                                AddTrigger                  (CXMLTreeNode XMLData,Vect3f Size, Vect3f Pos, int Group,sTrigger TriggerLua);
		void                                AddTriggerWithoutXML        (std::string Name, Vect3f Size, Vect3f Pos, int Group, sTrigger TriggerLua);

        // Métodos para Luabind
        int                                 GetValue                    () const;
        void                                SetValue                    ( const unsigned int& v ); 
};

#endif __TRIGGER_MANAGER_H__
