#include "ParticleInterpolator.h"
#include "Utils/Defines.h"

#define NUM_INTERPOLATORS 4
#define MAX_UPPER_LIMIT   3

CParticleInterpolator::CParticleInterpolator( const CXMLTreeNode& aNode )
{
  for ( uint32 i = 0, lCount = aNode.GetNumChildren(); i < lCount; ++i )
  {
    const CXMLTreeNode& lNode = aNode( i );
    const std::string& lTagName = lNode.GetName();

    if ( lTagName == "interpolator" )
    {
      SInterpolablePropery lInterpolableProp =
      {
        lNode.GetAttribute<float>( "alpha", 1.0f ),
        lNode.GetAttribute<float>( "size", 0.0f ),
        lNode.GetAttribute<Math::CColor>( "color", Math::colORANGE )
      };

      mSteps.push_back( lInterpolableProp );
    }
  }

  ASSERT( mSteps.size() == NUM_INTERPOLATORS, "The numer of steps of the interpolator must be 4" );
}

CParticleInterpolator::~CParticleInterpolator()
{
}

void  CParticleInterpolator::Interpolate( float aPercentageOfLive, float& aSize, float& aRed, float& aGreen, float& aBlue, float& aAlpha )
{
  float32 lAux = Math::Utils::Abs( ( aPercentageOfLive * NUM_INTERPOLATORS ) );
  uint32 lIdx = ( uint32 )Math::Utils::Floor( lAux );

  uint32 lUpperRange = lIdx, lLowerRange = lIdx - 1;

  if ( lIdx == 0 )
  {
    lLowerRange = 0;
    lUpperRange = 1;
  }
  else if ( lIdx > MAX_UPPER_LIMIT )
  {
    lLowerRange = MAX_UPPER_LIMIT - 1;
    lUpperRange = MAX_UPPER_LIMIT;
  }

  aSize  = Math::Utils::Lerp( mSteps[lLowerRange].mSize,    mSteps[lUpperRange].mSize, aPercentageOfLive );
  aRed   = Math::Utils::Lerp( mSteps[lLowerRange].mColor.r, mSteps[lUpperRange].mColor.r, aPercentageOfLive );
  aGreen = Math::Utils::Lerp( mSteps[lLowerRange].mColor.g, mSteps[lUpperRange].mColor.g, aPercentageOfLive );
  aBlue  = Math::Utils::Lerp( mSteps[lLowerRange].mColor.r, mSteps[lUpperRange].mColor.r, aPercentageOfLive );
  aAlpha = Math::Utils::Lerp( mSteps[lLowerRange].mAlpha,   mSteps[lUpperRange].mAlpha, aPercentageOfLive );
}