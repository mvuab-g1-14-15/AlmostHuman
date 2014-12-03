#pragma once
#include <string>
#include <vector>
#include <map>

#include "InputDefs.h"
#include "InputManager.h"
#include "Utils\SingletonPattern.h"

typedef struct S_INPUT_ACTION
{
	INPUT_DEVICE_TYPE       m_DeviceType;
	INPUT_EVENT_TYPE        m_EventType;
	unsigned int			m_Code;
} S_INPUT_ACTION;

class CActionManager : public CSingleton<CActionManager>
{
public:
	CActionManager	();
	~CActionManager	();

	void Init( const std::string &xmlFile, CInputManager *a_pInputManager );
	bool LoadXML();
	bool Reload();

	bool DoAction	(const std::string &action);
	bool DoAction	(const std::string &action, float32 &amount);
private:
	std::map<std::string, std::vector<S_INPUT_ACTION>> m_mActions;
	CInputManager *m_pInputManager;
	std::string  m_ActionsPath;

	INPUT_DEVICE_TYPE	strDeviceToCode	(const std::string &strAction);
	INPUT_EVENT_TYPE	strEventToCode	(const std::string &strEvent);
	unsigned int		strKeyToCode	(const std::string &strKey);
};
