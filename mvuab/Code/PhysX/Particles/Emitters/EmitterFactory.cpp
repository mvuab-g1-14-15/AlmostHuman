#include "EmitterFactory.h"

#include "Particles/ParticleEmitter.h"
#include "Utils/BaseUtils.h"

using namespace Math;

Vect3f GetSphereSpawnPosition( CParticleEmitter* aEmitter )
{
    ASSERT(aEmitter, "Null Emitter");
    return Vect3f( 0.0f, 0.0f, 0.0f );
}

Vect3f GetBoxSpawnPosition( CParticleEmitter* aEmitter )
{
    ASSERT(aEmitter, "Null Emitter");
    return baseUtils::RandRange( aEmitter->GetMinPoint(), aEmitter->GetMaxPoint() );
}

Vect3f GetCircleSpawnPosition( CParticleEmitter* aEmitter )
{
    ASSERT(aEmitter, "Null Emitter");
    return Vect3f( 0.0f, 0.0f, 0.0f );
}

Vect3f GetLineSpawnPosition( CParticleEmitter* aEmitter )
{
    ASSERT(aEmitter, "Null Emitter");
    return Vect3f( 0.0f, 0.0f, 0.0f );
}

Vect3f GetPointSpawnPosition( CParticleEmitter* aEmitter )
{
    return Vect3f( 0.0f, 0.0f, 0.0f );
}

ps::TEmitterType StrToEmitterType( const std::string & aEmitterStr )
{
    if( aEmitterStr == "sphere") return ps::eET_Sphere;
    if( aEmitterStr == "line"  ) return ps::eET_Line;
    if( aEmitterStr == "point" ) return ps::eET_Point;
    if( aEmitterStr == "circle") return ps::eET_Circle;

    return ps::eET_Box;
}

void CEmitterFactory::InitFactory()
{
    LOG_INFO_APPLICATION("Initializing emitter factory");

    mMap[ps::eET_Box   ] = GetBoxSpawnPosition;
    mMap[ps::eET_Sphere] = GetSphereSpawnPosition;
    mMap[ps::eET_Circle] = GetCircleSpawnPosition;
    mMap[ps::eET_Line  ] = GetLineSpawnPosition;
    mMap[ps::eET_Point ] = GetPointSpawnPosition;
}

CParticleEmitter* CEmitterFactory::CreateEmitter( const std::string& aEmitterType )
{
    ps::TEmitterType lType = StrToEmitterType(aEmitterType);
    return ( new CParticleEmitter(lType, mMap[lType] ) );
}
