#ifndef _SPHERE_EMITTER_H
#define _SPHERE_EMITTER_H

#include "Particle.h"
#include "ParticleEmitter.h"

class CSphereEmitter : public CParticleEmitter
{
    public:
        CSphereEmitter();
        virtual ~CSphereEmitter();

        virtual bool Init( const CXMLTreeNode& atts );
        void Update             (float dt);
        void Render             ();
        virtual Math::Vect3f GetSpawnPosition();
		virtual void SetPositionParticle(Math::Vect3f& lPosition);
};

#endif
