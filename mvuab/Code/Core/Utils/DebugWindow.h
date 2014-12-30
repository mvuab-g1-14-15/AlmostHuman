#pragma once
#ifndef INC_DEBUG_WINDOW_H_
#define INC_DEBUG_WINDOW_H_

#include "Math\Color.h"
#include "Math\Vector2.h"
#include "Utils\Defines.h"
#include <vector>

class CDebugWindow
{
public:
    CDebugWindow(Math::Vect2i aWindowPosition);
    ~CDebugWindow();

    virtual void Render();
    virtual void Update() = 0;
    bool GetVisible() const {return m_bIsVisible;}
    void SetVisible(bool flag){ m_bIsVisible = flag;}
    void ToggleVisibility();

    GET_SET(uint32, WindowWidth);
    GET_SET(uint32, WindowHeight);

protected:
    virtual void RenderInfo() = 0;
protected:
    bool                                m_bIsVisible;
    uint32                                m_WindowWidth;
    uint32                                m_WindowHeight;
    Math::Vect2i                                m_WindowPosition;
    Math::CColor                                m_FontColor_debug;
    Math::CColor                                m_FontColor_release;
};

#endif //INC_DEBUG_WINDOW_H_