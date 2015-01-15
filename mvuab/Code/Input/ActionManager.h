#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H
#pragma once
#include <string>
#include <vector>
#include <map>

#include "InputDefs.h"
#include "InputManager.h"
#include "Utils\SingletonPattern.h"

typedef struct S_INPUT_ACTION
{
    S_INPUT_ACTION::S_INPUT_ACTION(): m_DeviceType(IDV_NOTHING), m_EventType(EVENT_NOTHING),
                            m_AxisType(AXIS_NOTHING), m_Code(MOUSE_BUTTON_NOTHING), m_fDelta(1.f),
                            m_sDeviceType(""), m_sEventType(""), m_sAxisType(""), m_sCode("") {}

    INPUT_DEVICE_TYPE       m_DeviceType;
    INPUT_AXIS_TYPE            m_AxisType;
    INPUT_EVENT_TYPE        m_EventType;
    unsigned int            m_Code;

    float                    m_fDelta;

    std::string             m_sDeviceType;
    std::string             m_sAxisType;
    std::string             m_sEventType;
    std::string             m_sCode;

} S_INPUT_ACTION;

typedef std::vector<S_INPUT_ACTION> VecInputs;

class CActionManager : public CSingleton<CActionManager>
{
public:
    CActionManager    ();
    ~CActionManager    ();

    void Init( const std::string &xmlFile, CInputManager *a_pInputManager );
    bool LoadXML();
    bool Reload();
    bool SaveXML(const std::string &xmlFile);

    bool DoAction    (const std::string &action);
    bool DoAction    (const std::string &action, float32 &amount);
    void SetAction  (const std::string &action, VecInputs &a_vInputs);
    void Update();
    void Destroy();
private:
    typedef std::map<std::string, std::vector<S_INPUT_ACTION>> MapActions;
    MapActions m_mActions;
    CInputManager *m_pInputManager;
    std::string  m_ActionsPath;

    INPUT_DEVICE_TYPE    strDeviceToCode    (const std::string &strAction);
    INPUT_AXIS_TYPE        strAxisToCode    (const std::string &strAxis);
    INPUT_EVENT_TYPE    strEventToCode    (const std::string &strEvent);
    unsigned int        strKeyToCode    (const std::string &strKey);

};

#endif //ACTION_MANAGER_H