#ifndef _ASTAR_PROCESS_H
#define _ASTAR_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

class CAStar;
class CPhysicActor;
class CPhysicUserData;
class CPhysicController;

class CAStarProcess : public CProcess
{
private:
  CAStar*                        m_pAStarScene;
  Math::Vect3f                    m_PointInicial;
  Math::Vect3f                    m_PointFinal;
  std::vector<Math::Vect3f>        m_Path;
  std::vector<CPhysicActor*>    m_vPA;
  std::vector<CPhysicUserData*> m_vPUD;
  CPhysicController*            m_PhysicController;

public:
  CAStarProcess();
  virtual ~CAStarProcess();
  GET_SET_PTR(CAStar, AStarScene);
  GET_SET(Math::Vect3f, PointInicial);
  GET_SET(Math::Vect3f, PointFinal);
  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();

  void InitSceneCharacterController();
};

#endif // _ASTAR_PROCESS_H
