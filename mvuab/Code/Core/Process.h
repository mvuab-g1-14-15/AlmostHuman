#ifndef PROCESS_H
#define PROCESS_H
#pragma once

#include "Utils\Defines.h"

class CCamera;

class CProcess
{
protected:
  CCamera*    m_pCamera;
  float32        m_FPS;
  uint8        m_LastLineInDebug;
public:
  CProcess();
  CProcess( CCamera* ap_Camera );
  virtual ~CProcess() {}

  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void RenderDebugInfo();

  GET_SET_PTR( CCamera, Camera );
  GET_SET( float32, FPS );


};

#endif // PROCESS_H
