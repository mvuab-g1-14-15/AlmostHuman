#pragma once
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Utils/BaseUtils.h"
#include "Texture/Texture.h"

#include <string>

class CParticleEmitter;
class CTexture;
class CEffectTechnique;

namespace ps
{

enum TEmitterType
{
  eET_Circle = 0,
  eET_Sphere,
  eET_Line,
  eET_Point,
  eET_Box,
  eET_Radial
};

enum TSpeedType
{
  eST_Linear = 0,
  eST_Radial,
  eST_Ondulative
};

typedef Math::Vect3f( *TSpawnFunction )( CParticleEmitter* );
}