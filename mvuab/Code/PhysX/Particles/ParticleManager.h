#ifndef _PARTICLE_MANAGER_H
#define _PARTICLE_MANAGER_H

#include "XML\XMLTreeNode.h"
#include "ParticleEmitter.h"

#include <string>
#include <vector>

class CParticleManager
{
    private:
        std::vector<CParticleEmitter> m_Emitters;

    public:
        CParticleManager    ();
        ~CParticleManager   ();

        bool Init           (const std::string &path);
        void Update         (float dt);
        void Render         ();

};

#endif