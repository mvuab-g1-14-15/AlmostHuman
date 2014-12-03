#pragma once
#ifndef INC_FPS_DEBUG_WINDOW_H_
#define INC_FPS_DEBUG_WINDOW_H_
#include "DebugWindow.h"

class CFPSDebugWindow : public CDebugWindow
{
public:
	CFPSDebugWindow(Vect2i aWindowPosition);
	~CFPSDebugWindow();
	virtual void Update(float32 deltaTime);

protected:
   virtual void RenderInfo();

private:
	uint32								m_NumberFPSCount;
	float32								m_NumberFPSSum;
	uint32								m_FPS;
	uint32								m_MinFps;
	uint32								m_MaxFps;
};

#endif //INC_LOG_RENDER_H_