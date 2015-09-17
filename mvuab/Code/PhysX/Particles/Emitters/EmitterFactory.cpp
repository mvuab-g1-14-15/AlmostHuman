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
	float lAngle = baseUtils::RandRange(0.0f, 360.0f);
	Vect3f lVector( Math::Utils::Cos( lAngle ), 0.0f, Math::Utils::Sin( lAngle ) );
    return lVector * baseUtils::RandRange(aEmitter->GetMinRadius(), aEmitter->GetRadius() );
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

Vect3f GetRadialSpawnPosition( CParticleEmitter* aEmitter )
{
	float lAngle = aEmitter->GetRadialAngle();

	Vect3f lVector( Math::Utils::Cos( lAngle ), 0.0f, Math::Utils::Sin( lAngle ) );

	aEmitter->SetRadialAngle( lAngle + aEmitter->GetRadiusSpace() );

	return lVector * aEmitter->GetRadius();
}

ps::TEmitterType StrToEmitterType( const std::string & aEmitterStr )
{
    if( aEmitterStr == "sphere_emitter") return ps::eET_Sphere;
    if( aEmitterStr == "line_emitter"  ) return ps::eET_Line;
    if( aEmitterStr == "point_emitter" ) return ps::eET_Point;
    if( aEmitterStr == "circle_emitter") return ps::eET_Circle;
	if( aEmitterStr == "radial_emitter") return ps::eET_Radial;

    return ps::eET_Box;
}

void CEmitterFactory::InitFactory()
{
    LOG_INFO_APPLICATION("Initializing emitter factory");

    mMap[ps::eET_Box    ] = GetBoxSpawnPosition;
    mMap[ps::eET_Sphere ] = GetSphereSpawnPosition;
    mMap[ps::eET_Circle ] = GetCircleSpawnPosition;
    mMap[ps::eET_Line   ] = GetLineSpawnPosition;
    mMap[ps::eET_Point  ] = GetPointSpawnPosition;
	mMap[ps::eET_Radial ] = GetRadialSpawnPosition;
}

CParticleEmitter* CEmitterFactory::CreateEmitter( const std::string& aEmitterType )
{
    ps::TEmitterType lType = StrToEmitterType(aEmitterType);
    return ( new CParticleEmitter(lType, mMap[lType] ) );
}
