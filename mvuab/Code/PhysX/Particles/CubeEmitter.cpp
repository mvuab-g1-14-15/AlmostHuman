#include "CubeEmitter.h"
#include "Utils/BaseUtils.h"
#include "XML\XMLTreeNode.h"

CCubeEmitter::CCubeEmitter()
    : CParticleEmitter()
    , mMinPnt( Math::Vect3f(1.0f, 1.0f, 1.0f) )
    , mMaxPnt( Math::Vect3f(1.0f, 1.0f, 1.0f) )
{
}

CCubeEmitter::~CCubeEmitter()
{
}

bool CCubeEmitter::Init( const CXMLTreeNode& atts )
{
    bool lOk( CParticleEmitter::Init( atts ) );

    if( lOk )
    {
        mSize = atts.GetAttribute<Math::Vect3f>("cubic_size", Math::Vect3f());
        mMinPnt = mPosition - mSize;
        mMaxPnt = mPosition + mSize;
    }

    return lOk;
}

Math::Vect3f CCubeEmitter::GetSpawnPosition()
{
    return baseUtils::RandRange( mMinPnt, mMaxPnt );
}

/*
    void CCubeEmitter::Update(float dt)
    {
    if(m_Particles.size() == 0) return;
    if(!m_Active) return;

    m_EmitterLifeTime -= dt;
    if(m_EmitterLifeTime < 0.0f)
        m_Active = false;

    int lNumParticles = m_Particles.size();
    CParticle *p = &m_Particles[0];

    omp_set_num_threads(2);
    #pragma omp parallel for
    for(int i = 0; i < lNumParticles; ++i)
    {
        (p + i)->Update(dt);
    }

    m_ActualTime += dt;
    if(m_ActualTime >= m_TimeToEmit)
    {
        m_Rand = baseUtils::RandRange(m_RandMin, m_RandMax);
        m_ActualTime = 0.0f;
    }

    CParticle *l_Particle = &m_Particles[0];
    int l_NewRand = (int) std::ceilf(m_Rand * dt);

    for(int i = 0; i < lNumParticles && l_NewRand > 0; ++i)
    {
        if((l_Particle + i)->GetIsAlive()) continue;

        NewParticle(l_Particle + i);
        l_NewRand--; m_Rand--;
    }
    }
*/

//TODO ALEXITO EL NIÑO MÁS BONITO
void CCubeEmitter::SetPositionParticle(Math::Vect3f& lPosition)
{
	mMinPnt = mPosition - mSize;
    mMaxPnt = mPosition + mSize;
}