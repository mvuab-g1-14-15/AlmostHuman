#ifndef _PARTICLE_SYSTEM_CORE_H
#define _PARTICLE_SYSTEM_CORE_H
#include "ParticleEmitter.h"

#include "XML\XMLTreeNode.h"
#include "Utils\TemplatedVectorMapManager.h"

class CParticleEmitter;
class CEmitterFactory;

class CParticleSystemCore: public CTemplatedVectorMapManager<CParticleEmitter>
{
    public:
        CParticleSystemCore();
        CParticleSystemCore( const CXMLTreeNode& atts, CEmitterFactory* aEmitterFactory );
        ~CParticleSystemCore();

        void Update();
        void Render();
        void Refresh();
};

#endif //_PARTICLE_SYSTEM_CORE_H