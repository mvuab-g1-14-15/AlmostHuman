#pragma once
#ifndef INC_FPS_DEBUG_WINDOW_H_
#define INC_FPS_DEBUG_WINDOW_H_
#include "DebugWindow.h"

class CFPSDebugWindow : public CDebugWindow
{
public:
    CFPSDebugWindow(Math::Vect2i aWindowPosition);
    ~CFPSDebugWindow();
    virtual void Update();

protected:
   virtual void RenderInfo();

private:
};

#endif //INC_LOG_RENDER_H_