#include "ParticleInterpolator.h"
#include "Utils/Defines.h"
#include "Texture/Texture.h"

#define NUM_INTERPOLATORS 4
#define MAX_UPPER_LIMIT   3

struct SInterpolableProperty
{
float         mAlpha;
float         mSize;
Math::CColor  mColor;
};

CParticleInterpolator::CParticleInterpolator( const CXMLTreeNode& aNode )
{
	std::vector<SInterpolableProperty> lSteps;

  for ( uint32 i = 0, lCount = aNode.GetNumChildren(); i < lCount; ++i )
  {
    const CXMLTreeNode& lNode = aNode( i );
    const std::string& lTagName = lNode.GetName();

    if ( lTagName == "interpolator" )
    {
      SInterpolableProperty lInterpolableProp =
      {
        lNode.GetAttribute<float>( "alpha", 1.0f ),
        lNode.GetAttribute<float>( "size", 0.0f ),
        lNode.GetAttribute<Math::CColor>( "color", Math::colORANGE )
      };

      lSteps.push_back( lInterpolableProp );
    }
  }

  ASSERT( lSteps.size() <= NUM_INTERPOLATORS && lSteps.size() > 2, "The number of steps of the interpolator must be between 2 and 4" );

  int lStepCount = lSteps.size();

  std::vector<double> X(lStepCount), YAlpha(lStepCount), YSize(lStepCount), YRed(lStepCount), YBlue(lStepCount), YGreen(lStepCount);

  float lPercentage = 1.0f / lStepCount;
  float lCurrentPercentage = 0.0f;

  for (int i = 0; i < lStepCount; ++i)
  {
    X[i] = lCurrentPercentage;
    lCurrentPercentage += lPercentage;
    YAlpha[i] = lSteps[i].mAlpha;
    YSize[i] = lSteps[i].mSize;
    YRed[i] = lSteps[i].mColor.r;
    YBlue[i] = lSteps[i].mColor.b;
    YGreen[i] = lSteps[i].mColor.g;
  }

  mAlpha.set_points(X, YAlpha);
  mSize.set_points(X, YSize);
  mRed.set_points(X, YRed);
  mBlue.set_points(X, YBlue);
  mGreen.set_points(X, YGreen);
}

CParticleInterpolator::~CParticleInterpolator()
{
}

void  CParticleInterpolator::Interpolate( float aPercentageOfLive, float& aSize, float& aRed, float& aGreen, float& aBlue, float& aAlpha )
{
  aSize  = (float)mSize(aPercentageOfLive);
  aRed   = (float)mRed(aPercentageOfLive);
  aGreen = (float)mGreen(aPercentageOfLive);
  aBlue  = (float)mBlue(aPercentageOfLive);
  aAlpha = (float)mAlpha(aPercentageOfLive);
}