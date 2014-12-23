#ifndef INC_CAMERA_KEY_CONTROLLER_H_
#define INC_CAMERA_KEY_CONTROLLER_H_

#include "Utils\Types.h"
#include "XML\XMLTreeNode.h"
#include <vector>
#include <string.h>

class CCameraKey;
class CCameraKeyController
{
private:
    std::string                 m_FileName;
    std::vector<CCameraKey *>   m_Keys;
    size_t                      m_CurrentKey, m_NextKey;
    float32                     m_CurrentTime, m_TotalTime;
    bool                        m_Cycle;
    bool                        m_Reverse;
    void LoadXML(const std::string &FileName);
    void GetCurrentKey();
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
};

#endif // INC_CAMERA_KEY_CONTROLLER_H_