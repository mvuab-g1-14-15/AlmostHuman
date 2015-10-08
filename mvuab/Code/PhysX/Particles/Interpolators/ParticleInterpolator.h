#ifndef _H_PARTICLE_INTERPOLATOR_H
#define _H_PARTICLE_INTERPOLATOR_H
#pragma once

#include "XML/XMLTreeNode.h"
#include <vector>

#include "Math\SplineInterpolator.h"

class CParticleInterpolator
{
public:
  CParticleInterpolator() {}
  CParticleInterpolator( const CXMLTreeNode& aNode );
  virtual ~CParticleInterpolator();

  void Interpolate( float aPercentageOfLive, float& aSize, float& aRed, float& aGreen, float& aBlue, float& aAlpha );

private:
  Math::CSplineInterpolator mAlpha;
  Math::CSplineInterpolator mSize;
  Math::CSplineInterpolator mRed;
  Math::CSplineInterpolator mBlue;
  Math::CSplineInterpolator mGreen;
};

#endif // _H_PARTICLE_INTERPOLATOR_H