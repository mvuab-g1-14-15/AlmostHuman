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
        void AddInstance( CParticleInstance* aInstance );
        void AddEmitter( CParticleEmitter* aEmitter );
        void SetFixedDirection( const Math::Vect3f& aDirection );
        void ResetEmitters();
        void Update();
        void Render();
        void Refresh();
    private:
        typedef std::vector< CParticleInstance * > TParticleInstanceVector;
        typedef std::vector< CParticleEmitter * > TParticleEmittersVector;
        TParticleInstanceVector mInstances;
        TParticleEmittersVector mEmitters;

        float mDelayIn;
        float mCurrentTime;
};

#endif //_PARTICLE_SYSTEM_CORE_H