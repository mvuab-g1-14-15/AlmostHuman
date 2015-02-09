//----------------------------------------------------------------------------------
// CTriggerManager class
// Author: Xavier Macipe Arias
// 
// Description:
// Clase Manager de Triggers de PhysX.
//----------------------------------------------------------------------------------

#include "TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Logger\Logger.h"

CTriggerManager::CTriggerManager(): 
m_Value(16),
m_UserData()
{
        m_UserData.clear();
}
                                
CTriggerManager::~CTriggerManager()
{
        Clear();
}

bool CTriggerManager::LoadXML( const std::string &FileName )
{
        m_FileName = FileName;
        CXMLTreeNode newFile;
        
        if (!newFile.LoadFile( FileName.c_str() ))
        {
				//->AddNewLog(ELL_ERROR, "TRIGERMANAGER::LOADXML: ERROR loading the file %s.\n", FileName.c_str());
                return false;
        }

        CXMLTreeNode  m = newFile["triggers"];
        if (m.Exists())
        {
                int count = m.GetNumChildren();
                std::string sname;
                for (int i = 0; i < count; ++i)
                {
                        sname= m(i).GetName();
                        if (sname=="trigger")
                        {
                                // UserData
                                std::string l_sTriggerName = m(i).GetPszProperty("name","");
                                CPhysicUserData * l_pUserData = new CPhysicUserData(l_sTriggerName);
                                l_pUserData->SetName(l_sTriggerName);
                                l_pUserData->SetPaint(m(i).GetBoolProperty("paint",true));
                                CColor col(m(i).GetVect4fProperty("color",Vect4f(0.0f,1.0f,0.0f,1.0f)));
                                l_pUserData->SetColor(col);
                                l_pUserData->SetGroup(ECG_TRIGGERS);
                                m_UserData.push_back( l_pUserData );
                                // Actor
                                CPhysicActor *l_pActorTrigger = new CPhysicActor(l_pUserData);
                                std::string l_sType = m(i).GetPszProperty("shape","");
                                if (l_sType == "Box")
                                {
                                        Vect3f l_Pos = m(i).GetVect3fProperty("position",v3fZERO);
                                        Vect3f l_Size = m(i).GetVect3fProperty("size",v3fZERO);
                                        int l_Group = m(i).GetIntProperty("group",1);

                                        l_pActorTrigger->CreateBoxTrigger(l_Pos,l_Size,l_Group);
                                }
                                else if (l_sType == "Sphere")
                                        l_pActorTrigger->CreateSphereTrigger(m(i).GetVect3fProperty("position",v3fZERO),m(i).GetFloatProperty("radius",0.f),m(i).GetIntProperty("group",1));
                                if (l_sType!="")
                                {
                                        int triggerGroup = m(i).GetIntProperty("group",1);
                                        ECollisionGroup group = CORE->GetPhysicsManager()->GetCollisionGroup(triggerGroup);
                                        l_pActorTrigger->SetTriggerGroup(group);
                                        CORE->GetPhysicsManager()->AddPhysicActor(l_pActorTrigger);
                                        //llevar esto a physicsManager
                                        AddResource(l_sTriggerName, l_pActorTrigger);
                                        // Trigger
                                        sTrigger l_trigger;
                                        l_trigger.enter = m(i).GetPszProperty("onEnter","",false);
                                        l_trigger.exit = m(i).GetPszProperty("onExit","",false);
                                        l_trigger.stay = m(i).GetPszProperty("onStay","",false);
                                        m_LUAMap.insert( std::pair<std::string, sTrigger>(l_sTriggerName,l_trigger) );
                                }
                                else
                                {
                                        CHECKED_DELETE(l_pActorTrigger);
                                }
                        }
                }
        }
        return true;
}

bool CTriggerManager::Reload()
{
        if(m_FileName!="")
        {
                Clear();
                return LoadXML(m_FileName);
        }
        return false;
}

bool CTriggerManager::ReloadXML(const std::string &FileName)
{
        Clear();
        return LoadXML(FileName);
}
