#ifndef _CUBE_EMITTER_H
#define _CUBE_EMITTER_H

#include "Particle.h"
#include "ParticleEmitter.h"

class CCubeEmitter : public CParticleEmitter
{
    private:
        Math::Vect3f mMinPnt;
        Math::Vect3f mMaxPnt;
    public:
        CCubeEmitter            ();
        virtual ~CCubeEmitter   ();

        virtual bool Init( const CXMLTreeNode& atts );
        void Update             (float dt);
        void Render             ();
        virtual Math::Vect3f GetSpawnPosition();
};

#endif
