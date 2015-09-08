#pragma once
#include "Math/Vector3.h"

#include <string>

class CParticleEmitter;

namespace ps {

enum TEmitterType
{
    eET_Circle = 0,
    eET_Sphere,
    eET_Line,
    eET_Point,
    eET_Box
};

typedef Math::Vect3f(*TSpawnFunction)(CParticleEmitter*);
}