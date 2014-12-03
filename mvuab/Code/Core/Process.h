#ifndef PROCESS_H
#define PROCESS_H
#pragma once

#include <vector>
#include <string>
#include "Utils\SingletonPattern.h"
#include "Utils\Types.h"
#include "Utils\Defines.h"

class CCamera;

class CProcess
{
public:
	CProcess();
	CProcess( CCamera* ap_Camera );
	virtual ~CProcess(){}

	virtual void Init() = 0;
	virtual void Update(float32 deltaTime) = 0;
	virtual void Render() = 0;
	virtual void RenderDebugInfo();

	GET_SET_PTR(CCamera, Camera);
	GET_SET(float32, FPS);

protected:
	CCamera*	m_pCamera;
	float32		m_FPS;
	uint8		m_LastLineInDebug;
};

#endif // PROCESS_H
