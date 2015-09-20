#ifndef _H_PARTICLE_INTERPOLATOR_H
#define _H_PARTICLE_INTERPOLATOR_H
#pragma once

#include "XML/XMLTreeNode.h"
#include <vector>

class CParticleInterpolator
{
public:
  CParticleInterpolator() {}
  CParticleInterpolator( const CXMLTreeNode& aNode );
  virtual ~CParticleInterpolator();

  void Interpolate( float aPercentageOfLive, float& aSize, float& aRed, float& aGreen, float& aBlue, float& aAlpha );

private:
  struct SInterpolablePropery
  {
    float         mAlpha;
    float         mSize;
    Math::CColor  mColor;
  };

  typedef std::vector< SInterpolablePropery > TStepsVector;
  TStepsVector       mSteps;
};

#endif // _H_PARTICLE_INTERPOLATOR_H