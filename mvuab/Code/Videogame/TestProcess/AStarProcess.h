#ifndef _ASTAR_PROCESS_H
#define _ASTAR_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

class CAStar;

class CAStarProcess : public CProcess
{
private:
  CAStar* m_AStarScene;

  std::vector<Math::Vect3f> m_Path;

public:
  CAStarProcess();
  virtual ~CAStarProcess();

  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();

  void InitSceneCharacterController();
};

#endif // _ASTAR_PROCESS_H
