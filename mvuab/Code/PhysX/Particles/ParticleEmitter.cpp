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
    , mAliveParticlesCount(0)
    , mEmissionTime(Math::Vect2f(1.0f, 1.0f))
    , mAliveParticles( Math::Vect2u( 0, 0 ) )
    , mTimeToLive(Math::Vect2f(1.0f, 1.0f))
    , mActualTime(0.0f)
    , mSize(Math::Vect2f(1.0f, 1.0f))
    , mSpeed(Math::Vect2f(1.0f, 1.0f))
    , mAcceleration(Math::Vect2f(1.0f, 1.0f))
    , mDirection(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mPosition(Math::Vect3f(0.0f, 0.0f, 0.0f))
    , mVelocity(Math::Vect2f(1.0f, 1.0f))
    , mTechniqueName( "" )
    , mIsImmortal( 0 )
    , mParticlesXEmission(Math::Vect2f(1.0f, 1.0f))
    , mGravity(0.f)
{
}

CParticleEmitter::~CParticleEmitter()
{
}

bool CParticleEmitter::Init( const CXMLTreeNode& atts )
{
    SetName(atts.GetAttribute<std::string>("name", "no_name"));
    mIsLoop               = atts.GetAttribute<bool>("loop", false );
    mIsImmortal           = atts.GetAttribute<bool>("immortal", false );
    mIsActive             = atts.GetAttribute<bool>("active", false );
    mAliveParticles       = atts.GetAttribute<Math::Vect2u>("alive_particles", 0);
    mAliveParticles       = atts.GetAttribute<Math::Vect2u>("speed", 0);
    mEmissionTime         = atts.GetAttribute<Math::Vect2f>("emission_time", Math::Vect2f(1.0f, 1.0f));
    mTimeToLive           = atts.GetAttribute<Math::Vect2f>("time_to_live", Math::Vect2f(1.0f, 1.0f));
    mSize                 = atts.GetAttribute<Math::Vect2f>("particle_size_range", 0.0f );
    mAcceleration         = atts.GetAttribute<Math::Vect2f>("acceleration", Math::Vect2f(1.0f, 1.0f));
    mDirection            = atts.GetAttribute<Math::Vect3f>("direction", Math::Vect3f());
    mPosition             = atts.GetAttribute<Math::Vect3f>("position", Math::Vect3f());
    mVelocity             = atts.GetAttribute<Math::Vect2f>("velocity", Math::Vect2f(1.0f, 1.0f));
    mTechniqueName        = atts.GetAttribute<std::string>("technique", "none");
    mParticlesXEmission   = atts.GetAttribute<Math::Vect2f>("particles_per_emission", Math::Vect2f(1.0f, 1.0f) );
    mColorMin             = atts.GetAttribute<Math::CColor>("min_color", Math::colWHITE );
    mColorMax             = atts.GetAttribute<Math::CColor>("max_color", mColorMin );
    mInitialDirectionMin  = atts.GetAttribute<Math::Vect3f>("min_initial_direction", Math::Vect3f());
    mInitialDirectionMax  = atts.GetAttribute<Math::Vect3f>("max_initial_direction", mInitialDirectionMin );
    mGravity              = atts.GetAttribute<float32>("gravity", 1.0f );
    mRadialSpeed          = atts.GetAttribute<Math::Vect2f>("radial_speed", 1.0f );
    mOndSpeed             = atts.GetAttribute<Math::Vect2f>("ondulative_speed", 1.0f );
    mOndSpeedDirectionMin = atts.GetAttribute<Math::Vect3f>("min_ondulative_direction", Math::Vect3f());
    mOndSpeedDirectionMax = atts.GetAttribute<Math::Vect3f>("max_ondulative_direction", mOndSpeedDirectionMin );

    // Get textures
    for( uint32 i = 0, lCount = atts.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lSubNode = atts(i);
        const std::string& lTagName = lSubNode.GetName();
        if( lTagName == "texture" )
        {
            CTexture* lTexture = lSubNode.GetAttribute<CTexture>("name");
            if( lTexture )
            {
                mTextures.push_back(lTexture);
            }
        }
    }

    // Reserve the memory for the particles
    mParticles.reserve( mAliveParticles.y );
    mDeadParticles.reserve( mAliveParticles.y );

    // Allocate all the particles
    for( uint32 i = 0; i < mAliveParticles.y; ++i )
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
    /*
        const uint32 lParticlesToEmit = (uint32)baseUtils::RandRange( mParticlesXEmission.x, mParticlesXEmission.y );
        for( uint32 i = 0; i < lParticlesToEmit && (GetParticleCount() < mAliveParticles.y); ++i )
        {
          CParticle* lNewParticle = mDeadParticles.back();
          mDeadParticles.pop_back();
          /*
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
              ++mAliveParticlesCount;
          }
        }*/
}

void CParticleEmitter::KillParticles()
{
    for( uint32 i = 0, lParticlesCount = GetParticleCount(); i < lParticlesCount; )
    {
        CParticle* lParticle = GetParticle( i );
        if( !lParticle->IsAlive() )
        {
            std::swap( mParticles[i], mParticles[mAliveParticlesCount - 1] );
            mDeadParticles.push_back( mParticles.back() ) ;
            mParticles.pop_back();

            --mAliveParticlesCount;
            lParticlesCount = GetParticleCount();
        }
        else
        {
            // Is ok, then next iteration
            ++i;
        }
    }
}