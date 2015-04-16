#ifndef _PARTICLE_MANAGER_H
#define _PARTICLE_MANAGER_H

#include "XML\XMLTreeNode.h"
#include "ParticleEmitter.h"
#include "Utils\Manager.h"

#include <string>
#include <vector>

class CCubeEmitter;
class CTrailEmitter;

class CParticleManager: public CManager
{
    private:
        std::vector<CParticleEmitter*> m_Emitters;

    public:
        CParticleManager();
        CParticleManager(CXMLTreeNode& atts);
        ~CParticleManager();

        void Init();
        void Update();
        void Render();
        void AddEmitter(CParticleEmitter *l_Emitter);

        CCubeEmitter *CreateCubeEmitter();
        CTrailEmitter *CreateTrailEmitter();
};

#endif