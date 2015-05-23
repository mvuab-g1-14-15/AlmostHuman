#include "ParticleEmitter.h"
#include "Utils/BaseUtils.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "EngineManagers.h"
#include "Effects/EffectManager.h"

CParticleEmitter::CParticleEmitter()
    : CName()
    , mIsLoop( false )
    , mIsActive( false )
    , mMaxParticles(0)
    , mAliveParticles(0)
    , mTimeToEmit(Math::Vect2f(1.0f, 1.0f))
    , mTimeToLive(Math::Vect2f(1.0f, 1.0f))
    , mActualTime(0.0f)
    , mSize(Math::Vect2f(1.0f, 1.0f))
    , mAcceleration(Math::Vect2f(1.0f, 1.0f))
    , mDirection(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mPosition(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mVelocity(Math::Vect2f(1.0f, 1.0f))
    , mTechniqueName( "" )
    , mIsInmortal( 0 )
    , mParticlesXEmission(Math::Vect2f(1.0f, 1.0f))
{
}

CParticleEmitter::~CParticleEmitter()
{
    ASSERT( mAliveParticles == GetParticleCount(), "Different number of particles alive than in the vector" );
    for( uint32 i = 0, lParticles = mAliveParticles; i < lParticles; ++i)
    {
        CParticle* lParticle = GetParticle( i );
        CHECKED_DELETE( lParticle );
    }

    for( uint32 i = 0, lParticles = mMaxParticles - mAliveParticles; i < lParticles; ++i)
    {
        CParticle* lParticle = mDeadParticles[i];
        CHECKED_DELETE( lParticle );
    }
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
    mIsLoop             = atts.GetBoolProperty("loop", false );
    mIsInmortal         = !mIsLoop;
    mIsActive           = atts.GetBoolProperty("active", false );
    mMaxParticles       = atts.GetIntProperty("max_particles", 0);
    mTimeToEmit         = atts.GetVect2fProperty("time_to_emit", Math::Vect2f(1.0f, 1.0f));
    mTimeToLive         = atts.GetVect2fProperty("time_to_live", Math::Vect2f(1.0f, 1.0f));
    mSize               = atts.GetVect2fProperty("particle_size_range", 0.0f );
    mAcceleration       = atts.GetVect2fProperty("acceleration", Math::Vect2f(1.0f, 1.0f));
    mDirection          = atts.GetVect3fProperty("direction", Math::Vect3f());
    mPosition           = atts.GetVect3fProperty("position", Math::Vect3f());
    mVelocity           = atts.GetVect2fProperty("velocity", Math::Vect2f(1.0f, 1.0f));
    mTechniqueName      = atts.GetPszProperty("technique", "none");
    mParticlesXEmission = atts.GetVect2fProperty("particles_per_emission", Math::Vect2f(1.0f, 1.0f) );

    // Get textures
    for( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const std::string& lTagName = atts(i).GetName();
        if( lTagName == "texture" )
        {
            mTextures.push_back( atts(i).GetPszProperty("name") );
        }
    }

    // Reserve the memory for the particles
    mParticles.reserve( mMaxParticles );
    mDeadParticles.reserve( mMaxParticles );

    // Allocate all the particles
    for( uint32 i = 0; i < mMaxParticles; ++i )
    {
        mDeadParticles.push_back( new CParticle() );
    }

    return true;
}

void CParticleEmitter::Update( float dt )
{
    KillParticles();
    EmitParticles();
    for( uint32 i = 0, lParticles = GetParticleCount(); i < lParticles; ++i)
    {
        CParticle* lParticle = GetParticle( i );
        lParticle->Update( 1 / 30.0f );
    }
}

void CParticleEmitter::Render()
{
    for( uint32 i = 0, lParticles = GetParticleCount(); i < lParticles; ++i)
    {
        CParticle* lParticle = GetParticle( i );
        lParticle->Render();
    }
}

void CParticleEmitter::EmitParticles()
{
    const uint32 lParticlesToEmit = (uint32)baseUtils::RandRange( mParticlesXEmission.x, mParticlesXEmission.y );
    for( uint32 i = 0; i < lParticlesToEmit && (GetParticleCount() < mMaxParticles); ++i )
    {
        CParticle* lNewParticle = mDeadParticles.back();
        mDeadParticles.pop_back();

        bool lOk = lNewParticle->Init(
                       baseUtils::RandRange( mTimeToLive.x, mTimeToLive.y),
                       baseUtils::RandRange( Math::Vect3f(0.0f, 0.0f, 0.0f) , Math::Vect3f(1.0f, 1.0f, 1.0f) ),
                       GetSpawnPosition(),
                       baseUtils::RandRange( mVelocity.x, mVelocity.y ),
                       baseUtils::RandRange( mAcceleration.x, mAcceleration.x ),
                       mDirection,
                       baseUtils::RandRange( mSize.x, mSize.y ),
                       1.0f,
                       3.14f,
                       mTextures[ baseUtils::RandRange( (unsigned int)(0), (unsigned int)(mTextures.size() - 1) ) ],
                       mTechniqueName
                   );

        ASSERT( lOk, "Error initing the particle" );

        if( lOk )
        {
            mParticles.push_back( lNewParticle );
            ++mAliveParticles;
        }
    }
}

void CParticleEmitter::KillParticles()
{
    for( uint32 i = 0, lParticlesCount = GetParticleCount(); i < lParticlesCount; )
    {
        CParticle* lParticle = GetParticle( i );
        if( !lParticle->IsAlive() )
        {
            std::swap( mParticles[i], mParticles[mAliveParticles - 1] );
            mDeadParticles.push_back( mParticles.back() ) ;
            mParticles.pop_back();

            --mAliveParticles;
            lParticlesCount = GetParticleCount();
        }
        else
        {
            // Is ok, then next iteration
            ++i;
        }
    }
}