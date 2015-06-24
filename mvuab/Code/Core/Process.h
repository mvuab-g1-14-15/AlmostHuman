#ifndef PROCESS_H
#define PROCESS_H
#pragma once

#include "Utils\Defines.h"

class CCamera;
class CAStar;

typedef enum EMenuOption
{
  eOptionScene = 1,
  eOptionMesh,
  eOptionParticles,
  eOptionLights,
  eOptionEffect,
  eOptionAnimatedModels,
  eOptionExit
};

class CProcess
{
protected:
  CCamera*     m_pCamera;
  uint8        m_LastLineInDebug;

public:
  CProcess();
  CProcess( CCamera* ap_Camera );
  virtual ~CProcess() {}

  virtual void Init() = 0;
  void ProcessReloads();
  void ProcessKey();
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void RenderDebugInfo();

  GET_SET_PTR( CCamera, Camera );

  void OnClickedMenuOption( EMenuOption aMenuOption );
};

#endif // PROCESS_H
