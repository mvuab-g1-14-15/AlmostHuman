#include "ActionManager.h"
#include "XML\XMLTreeNode.h"

#include <Windows.h>

CActionManager::CActionManager()
{
}

CActionManager::~CActionManager()
{
}

void CActionManager::Init( const std::string &xmlFile, CInputManager *a_pInputManager )
{
	m_pInputManager = a_pInputManager;
	m_ActionsPath = xmlFile;
	LoadXML();
}

bool CActionManager::DoAction(const std::string &action)
{
	float32 amount = 0;
	return(DoAction(action, amount));
}

bool CActionManager::DoAction(const std::string &action, float32 &amount)
{
	std::map<std::string, std::vector<S_INPUT_ACTION>>::iterator it1 = m_mActions.find(action);
	if(it1 == m_mActions.end())
		return(false);

	std::vector<S_INPUT_ACTION> vector = m_mActions[action];
	std::vector<S_INPUT_ACTION>::iterator itb = vector.begin(), ite = vector.end();

	bool doIt = false;
	for(; itb != ite; ++itb)
	{
		S_INPUT_ACTION current_action = *itb;

		if(current_action.m_EventType == EVENT_DOWN)
			doIt = doIt || m_pInputManager->IsDown(current_action.m_DeviceType, current_action.m_Code);
		if(current_action.m_EventType == EVENT_DOWN_UP)
			doIt = doIt || m_pInputManager->IsDownUp(current_action.m_DeviceType, current_action.m_Code);
		if(current_action.m_EventType == EVENT_UP_DOWN)
			doIt = doIt || m_pInputManager->IsUpDown(current_action.m_DeviceType, current_action.m_Code);
	}

	return(doIt);
}

bool CActionManager::LoadXML()
{
	CXMLTreeNode l_File;
	if(false == l_File.LoadFile(m_ActionsPath.c_str()))
	{
		std::string err = "ERROR reading the file " + m_ActionsPath;

		MessageBox(NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(EXIT_FAILURE);
	}

	CXMLTreeNode  actions_TreeNode =l_File["Actions"];
	if(actions_TreeNode.Exists())
	{
		int count = actions_TreeNode.GetNumChildren();
		for( int i = 0; i < count; ++i )
		{
			std::string TagName = actions_TreeNode(i).GetName();
			if( TagName == "action" )
			{
				std::string StrActionName = std::string( actions_TreeNode(i).GetPszProperty("name", "") );
				CXMLTreeNode  action_TreeNode = actions_TreeNode(i);
				if(action_TreeNode.Exists())
				{
					int iSubActions = action_TreeNode.GetNumChildren();
					for( int j = 0; j < iSubActions; ++j )
					{
						std::string TagName = action_TreeNode(j).GetName();
						if( TagName == "input" )
						{
							S_INPUT_ACTION new_action;
							std::string deviceTypeStr(action_TreeNode(j).GetPszProperty("deviceType", "") );
							std::string EventTypeStr(action_TreeNode(j).GetPszProperty("EventType", "") );
							std::string CodeStr(action_TreeNode(j).GetPszProperty("Code", "") );

							new_action.m_Code = strKeyToCode(CodeStr);
							new_action.m_DeviceType = strDeviceToCode(deviceTypeStr);
							new_action.m_EventType = strEventToCode(EventTypeStr);

							std::vector<S_INPUT_ACTION> vector;
							if( m_mActions.find(TagName) != m_mActions.end())
							{
								vector = m_mActions[TagName];
							}
							vector.push_back(new_action);

							m_mActions[StrActionName] = vector;
						}
					}
				}
			}
		}
	}
	return true;
}

INPUT_DEVICE_TYPE CActionManager::strDeviceToCode(const std::string &strAction)
{
	if("IDV_MOUSE" == strAction) return(IDV_MOUSE);
	else if("IDV_KEYBOARD" == strAction) return(IDV_KEYBOARD);
	else if("IDV_GAMEPAD1" == strAction) return(IDV_GAMEPAD1);
	else if("IDV_GAMEPAD2" == strAction) return(IDV_GAMEPAD2);
	else if("IDV_GAMEPAD3" == strAction) return(IDV_GAMEPAD3);
	else if("IDV_GAMEPAD4" == strAction) return(IDV_GAMEPAD4);

	return(IDV_NOTHING);
}

INPUT_EVENT_TYPE CActionManager::strEventToCode(const std::string &strEvent)
{
	if("EVENT_DOWN" == strEvent) return(EVENT_DOWN);
	else if("EVENT_DOWN_UP" == strEvent) return(EVENT_DOWN_UP);
	else if("EVENT_UP_DOWN" == strEvent) return(EVENT_UP_DOWN);

	return(EVENT_NOTHING);
}

unsigned int CActionManager::strKeyToCode(const std::string &strKey)
{
	if("KEY_TAB" == strKey) return(KEY_TAB);
	else if("KEY_BACK" == strKey) return(KEY_BACK);
	else if("KEY_SPACE" == strKey) return(KEY_SPACE);
	else if("KEY_ESCAPE" == strKey) return(KEY_ESCAPE);
	else if("KEY_RETURN" == strKey) return(KEY_RETURN);
	else if("KEY_CAPITAL" == strKey) return(KEY_CAPITAL);

	else if("KEY_MINUS" == strKey) return(KEY_MINUS);
	else if("KEY_EQUALS" == strKey) return(KEY_EQUALS);
	else if("KEY_LBRACKET" == strKey) return(KEY_LBRACKET);
	else if("KEY_RBRACKET" == strKey) return(KEY_RBRACKET);
	else if("KEY_SEMICOLON" == strKey) return(KEY_SEMICOLON);
	else if("KEY_APOSTROPHE" == strKey) return(KEY_APOSTROPHE);

	else if("KEY_BACKSLASH" == strKey) return(KEY_BACKSLASH);
	else if("KEY_PERIOD" == strKey) return(KEY_PERIOD);
	else if("KEY_SLASH" == strKey) return(KEY_SLASH);
	else if("KEY_COMMA" == strKey) return(KEY_COMMA);
	else if("KEY_GRAVE" == strKey) return(KEY_GRAVE);
	
	else if("KEY_A" == strKey) return(KEY_A);
	else if("KEY_S" == strKey) return(KEY_S);
	else if("KEY_D" == strKey) return(KEY_D);
	else if("KEY_F" == strKey) return(KEY_F);
	else if("KEY_G" == strKey) return(KEY_G);
	else if("KEY_H" == strKey) return(KEY_H);
	else if("KEY_J" == strKey) return(KEY_J);
	else if("KEY_K" == strKey) return(KEY_K);
	else if("KEY_L" == strKey) return(KEY_L);
	else if("KEY_X" == strKey) return(KEY_X);
	else if("KEY_C" == strKey) return(KEY_C);
	else if("KEY_B" == strKey) return(KEY_B);
	else if("KEY_N" == strKey) return(KEY_N);
	else if("KEY_M" == strKey) return(KEY_M);
	else if("KEY_Q" == strKey) return(KEY_Q);
	else if("KEY_W" == strKey) return(KEY_W);
	else if("KEY_E" == strKey) return(KEY_E);
	else if("KEY_R" == strKey) return(KEY_R);
	else if("KEY_T" == strKey) return(KEY_T);
	else if("KEY_Y" == strKey) return(KEY_Y);
	else if("KEY_U" == strKey) return(KEY_U);
	else if("KEY_I" == strKey) return(KEY_I);
	else if("KEY_O" == strKey) return(KEY_O);
	else if("KEY_P" == strKey) return(KEY_P);
	
	else if("KEY_1" == strKey) return(KEY_1);
	else if("KEY_2" == strKey) return(KEY_2);
	else if("KEY_3" == strKey) return(KEY_3);
	else if("KEY_4" == strKey) return(KEY_4);
	else if("KEY_5" == strKey) return(KEY_5);
	else if("KEY_6" == strKey) return(KEY_6);
	else if("KEY_7" == strKey) return(KEY_7);
	else if("KEY_8" == strKey) return(KEY_8);
	else if("KEY_9" == strKey) return(KEY_9);
	else if("KEY_0" == strKey) return(KEY_0);
	
	else if("KEY_F1" == strKey) return(KEY_F1);
	else if("KEY_F2" == strKey) return(KEY_F2);
	else if("KEY_F3" == strKey) return(KEY_F3);
	else if("KEY_F4" == strKey) return(KEY_F4);
	else if("KEY_F5" == strKey) return(KEY_F5);
	else if("KEY_F6" == strKey) return(KEY_F6);
	else if("KEY_F7" == strKey) return(KEY_F7);
	else if("KEY_F8" == strKey) return(KEY_F8);
	else if("KEY_F9" == strKey) return(KEY_F9);
	else if("KEY_F10" == strKey) return(KEY_F10);
	else if("KEY_F11" == strKey) return(KEY_F11);
	else if("KEY_F12" == strKey) return(KEY_F12);
	
	else if("KEY_NPPLUS" == strKey) return(KEY_NPPLUS);
	else if("KEY_NPMINUS" == strKey) return(KEY_NPMINUS);
	else if("KEY_NPENTER" == strKey) return(KEY_NPENTER);
	else if("KEY_NUMLOCK" == strKey) return(KEY_NUMLOCK);
	else if("KEY_NPCOMMA" == strKey) return(KEY_NPCOMMA);
	else if("KEY_NPDIVIDE" == strKey) return(KEY_NPDIVIDE);
	else if("KEY_NPDECIMAL" == strKey) return(KEY_NPDECIMAL);
	else if("KEY_NPMULTIPLY" == strKey) return(KEY_NPMULTIPLY);
	
	else if("KEY_NP1" == strKey) return(KEY_NP1);
	else if("KEY_NP2" == strKey) return(KEY_NP2);
	else if("KEY_NP3" == strKey) return(KEY_NP3);
	else if("KEY_NP4" == strKey) return(KEY_NP4);
	else if("KEY_NP5" == strKey) return(KEY_NP5);
	else if("KEY_NP6" == strKey) return(KEY_NP6);
	else if("KEY_NP7" == strKey) return(KEY_NP7);
	else if("KEY_NP8" == strKey) return(KEY_NP8);
	else if("KEY_NP9" == strKey) return(KEY_NP9);
	else if("KEY_NP0" == strKey) return(KEY_NP0);
	
	else if("KEY_RSHIFT" == strKey) return(KEY_RSHIFT);
	else if("KEY_LSHIFT" == strKey) return(KEY_LSHIFT);
	else if("KEY_RCTRL" == strKey) return(KEY_RCTRL);
	else if("KEY_LCTRL" == strKey) return(KEY_LCTRL);
	else if("KEY_RALT" == strKey) return(KEY_RALT);
	else if("KEY_LALT" == strKey) return(KEY_LALT);
	else if("KEY_LWIN" == strKey) return(KEY_LWIN);
	else if("KEY_RWIN" == strKey) return(KEY_RWIN);
	
	else if("KEY_UP" == strKey) return(KEY_UP);
	else if("KEY_DOWN" == strKey) return(KEY_DOWN);
	else if("KEY_LEFT" == strKey) return(KEY_LEFT);
	else if("KEY_RIGHT" == strKey) return(KEY_RIGHT);
	
	else if("KEY_INSERT" == strKey) return(KEY_INSERT);
	else if("KEY_DELETE" == strKey) return(KEY_DELETE);
	else if("KEY_HOME" == strKey) return(KEY_HOME);
	else if("KEY_END" == strKey) return(KEY_END);
	
	else if("KEY_PGDOWN" == strKey) return(KEY_PGDOWN);
	else if("KEY_PGUP" == strKey) return(KEY_PGUP);
	
	else if("KEY_PAUSE" == strKey) return(KEY_PAUSE);
	else if("KEY_SCROLL" == strKey) return(KEY_SCROLL);  

	return(0);
}

bool CActionManager::Reload()
{
	m_mActions.clear();
	LoadXML();

	return true;
}