#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H
#pragma once
#include <string>
#include <vector>
#include <map>

#include "Utils\Manager.h"
#include "Utils\Mutex.h"

#include "InputDefs.h"
#include "InputManager.h"
#include "Utils\SingletonPattern.h"

struct SInputActions
{
    EInputDeviceType        m_DeviceType;
    EInputEventType         m_EventType;
    EInputAxisType          m_AxisType;

    uint32                  m_Code;
    float32                 m_fDelta;

    std::string             m_sDeviceType;
    std::string             m_sAxisType;
    std::string             m_sEventType;
    std::string             m_sCode;

};

struct SActionDone
{
    bool mIsActionDone;
    float32 mActionAmount;
    EInputDeviceType        m_DeviceType;
};

typedef std::vector<SInputActions> TInputsVector;

class CActionManager : public CManager
{
    public:
        CActionManager  ();
        CActionManager  ( CXMLTreeNode& atts );
        ~CActionManager ();

        void Init         ();
        void ProcessInputs();

        bool Reload       ();
        bool LoadXML      ();
        bool SaveXML      (const std::string &xmlFile);

        bool DoAction     (const std::string &action);
        bool DoAction     (const std::string &action, float32 &amount);
        bool AnyKey();
        void SetAction    (const std::string &action, TInputsVector &a_vInputs);

        void Update       ();
        void Render       () {};
        void Destroy      ();
    private:

        typedef std::map<std::string, std::vector<SInputActions>> MapActions;
        typedef std::map< std::string, SActionDone > MapActionsDone;

        MapActionsDone      mDoActions;
        MapActions          mActions;

        CMutex              mMutex;
        CInputManager*      mInputManager;

        EInputDeviceType   strDeviceToCode    (const std::string &strAction);
        EInputAxisType     strAxisToCode      (const std::string &strAxis);
        EInputEventType    strEventToCode     (const std::string &strEvent);
        unsigned int        strKeyToCode       (const std::string &strKey);

};

#endif //ACTION_MANAGER_H