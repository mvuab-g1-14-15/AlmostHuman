#pragma once
#ifndef INC_CAMERA_DEBUG_WINDOW_H_
#define INC_CAMERA_DEBUG_WINDOW_H_
#include "DebugWindow.h"
#include "Utils\Defines.h"

class CCamera;

class CCameraDebugWindow : public CDebugWindow
{
public:
    CCameraDebugWindow(Vect2i aWindowPosition);
    ~CCameraDebugWindow();
    virtual void Update(float32 deltaTime);

protected:
   virtual void RenderInfo();

private:
    CCamera*        m_pCurrentCamera;
};

#endif //INC_CAMERA_DEBUG_WINDOW_H_