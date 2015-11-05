#ifndef INC_CAMERA_KEY_CONTROLLER_H_
#define INC_CAMERA_KEY_CONTROLLER_H_

#include "Utils\Types.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include <vector>
#include <string.h>
#include "Utils\Name.h"

class CCameraKey;
class CCameraInfo;

typedef struct S_LUACodePerKey
{
    std::string LuaCode;
    bool Executed;
} S_LUACodePerKey, *PS_LUACodePerKey;

class CCameraKeyController : public CName
{
    public:
        CCameraKeyController();
        CCameraKeyController(CXMLTreeNode &atts);
        virtual ~CCameraKeyController();
        void Update();
        void SetCurrentTime(float32 CurrentTime);
        void ResetTime();
        float32 GetTotalTime();
        bool IsCycle() const;
        void SetCycle(bool Cycle);
        bool IsReverse() const;
        void SetReverse(bool Reverse);
        bool LoadXML(const std::string &FileName);
        bool LoadXML(CXMLTreeNode &aNode);
        CCameraInfo* GetCurrentInfo() {
            return  m_pCameraInfo;
        }

    private: // Members
        std::string                 m_FileName;
        std::string                 m_LuaCode;

        std::vector<CCameraKey *>    m_Keys;
        std::vector<S_LUACodePerKey> m_LuaCodePerKey;

        size_t                      m_CurrentKey;
        size_t                      m_NextKey;

        float32                     m_CurrentTime;
        float32                     m_KeyAction;
        float32                     m_TotalTime;

        bool                        m_PlayingForward;
        bool                        m_PlayingBackward;
        bool                        m_Cycle;
        bool                        m_Reverse;
        bool                        m_Finish;
        bool                        m_bLuaEnable;

        CCameraInfo*          m_pCameraInfo;

    private: // Methods
        void GetCurrentKeyForward();
        void GetCurrentKeyBackward();
        void PlayFoward();
        void PlayBackward();
        void InterpolateKeys(float32 Percentage, CCameraInfo A, CCameraInfo B);
};

#endif // INC_CAMERA_KEY_CONTROLLER_H_