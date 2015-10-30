#ifndef _PARTICLE_SYSTEM_CORE_H
#define _PARTICLE_SYSTEM_CORE_H
#include "ParticleEmitter.h"

#include "XML\XMLTreeNode.h"
#include <vector>

class CParticleEmitter;
class CEmitterFactory;
class CParticleInstance;

class CParticleSystemCore
{
    public:
        CParticleSystemCore();
        CParticleSystemCore( const CXMLTreeNode& atts, CEmitterFactory* aEmitterFactory );
        ~CParticleSystemCore();
        void AddEmitter( CParticleEmitter* aEmitter );
        void SetFixedDirection( const Math::Vect3f& aDirection );
        void ResetEmitters();
        void Update( float dt );
        void Render();
        void Refresh();
        CParticleSystemCore* Clone() const;
    private:
        typedef std::vector< CParticleEmitter * > TParticleEmittersVector;
        TParticleEmittersVector mEmitters;

        float mDelayIn;
        float mCurrentTime;
};

#endif //_PARTICLE_SYSTEM_CORE_H