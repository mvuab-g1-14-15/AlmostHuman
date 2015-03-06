#ifndef PHYSIC_MATERIAL_H
#define PHYSIC_MATERIAL_H
#pragma once

#include "Process.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CPhysicMaterial : public CProcess
{
private:
  float32             m_Speed;
  float32             m_Amount;
  float32             m_Angle;
  float32             m_AngleMoon;
  Math::Vect2i        m_LastMousePoint;
  bool                m_PaintAll;

public:
  CPhysicMaterial();
  virtual ~CPhysicMaterial();

  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();
};

#endif // PHYSIC_MATERIAL_H
