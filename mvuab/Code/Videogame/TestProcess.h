#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H
#pragma once

#include "Process.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CTestProcess : public CProcess
{
private:
	float32             m_Speed;
	float32             m_Amount;
	float32             m_Angle;
	float32             m_AngleMoon;
	Vect2i              m_LastMousePoint;
	CCamera*            m_pFPSCamera;
	CCamera*            m_pTPSCamera;
	bool	            m_PaintAll;

public:
	CTestProcess();
	virtual ~CTestProcess();

	virtual void Init();
	virtual void Update(float32 deltaTime);
	virtual void Render();
	virtual void RenderDebugInfo();
};

#endif // TEST_PROCESS_H
