#ifndef INC_CAMERA_KEY_CONTROLLER_H_
#define INC_CAMERA_KEY_CONTROLLER_H_

#include "Utils\Types.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include <vector>
#include <string.h>
#include "Utils\Name.h"

class CCameraKey;

class CCameraKeyController : public CName
{
public:
    CCameraKeyController(CXMLTreeNode &atts);
    virtual ~CCameraKeyController();
    void Update(float32 deltaTime);
    void SetCurrentTime(float32 CurrentTime);
    void ResetTime();
    float32 GetTotalTime();
    bool IsCycle() const;
    void SetCycle(bool Cycle);
    bool IsReverse() const;
    void SetReverse(bool Reverse);

private: // Members
    std::string                 m_FileName;
    std::vector<CCameraKey *>   m_Keys;
    size_t                      m_CurrentKey;
    size_t                      m_NextKey;
    float32                     m_CurrentTime;
    float32                     m_TotalTime;
    bool                        m_PlayingForward;
    bool                        m_PlayingBackward;
    bool                        m_Cycle;
    bool                        m_Reverse;
    bool                        m_Finish;

private: // Methods
    void LoadXML(const std::string &FileName);
    void GetCurrentKey();
    void GetCurrentKeyForward();
    void GetCurrentKeyBackward();
};

#endif // INC_CAMERA_KEY_CONTROLLER_H_