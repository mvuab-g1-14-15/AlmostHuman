#pragma once
#ifndef INC_DEBUG_WINDOW_MANAGER_H_
#define INC_DEBUG_WINDOW_MANAGER_H_

#include <vector>
#include "Utils\SingletonPattern.h"
#include "Utils\Types.h"

class CDebugWindow;

class CDebugWindowManager : public CSingleton<CDebugWindowManager>
{
public:
    CDebugWindowManager();
    ~CDebugWindowManager();

    void Render();
    void Update();
    bool GetVisible() const {return m_bIsVisible;}
    void SetVisible(bool flag){ m_bIsVisible = flag;}
    void ToggleVisibility();

protected:
    bool                                m_bIsVisible;
    uint32                                m_CurrentDebugWindow;
    std::vector<CDebugWindow*>            m_vDebugWindows;
};

#endif //INC_DEBUG_WINDOW_MANAGER_H_