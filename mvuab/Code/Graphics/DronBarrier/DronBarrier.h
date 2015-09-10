#pragma once
#ifndef _DRON_BARRIER_H
#define _DRON_BARRIER_H

#include "Object3D.h"
#include "Math/Color.h"

class CRenderableVertexs;
class CEffectTechnique;

class CDronBarrier : public CObject3D
{
public:
  CDronBarrier();
  virtual ~CDronBarrier();
  void SetColor( Math::CColor aColor );
  virtual void Render();
  static void CreateGeometry();
  static void DestroyGeometry();
protected:
  Math::CColor    mColor;
  CEffectTechnique* mTechnique;
  static CRenderableVertexs* sRV;
};

#endif // _DRON_BARRIER_H