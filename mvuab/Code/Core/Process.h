#ifndef PROCESS_H
#define PROCESS_H
#pragma once

#include "Utils\Defines.h"

class CCamera;
class CAStar;

class CProcess
{
protected:
  CCamera*    m_pCamera;
  uint8        m_LastLineInDebug;
  CAStar*		m_AStar;
public:
  CProcess();
  CProcess( CCamera* ap_Camera );
  virtual ~CProcess() {}

  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void RenderDebugInfo();

  GET_SET_PTR( CCamera, Camera );
  CAStar* GetAStar() const
  {
	  return m_AStar;
  }
};

#endif // PROCESS_H
