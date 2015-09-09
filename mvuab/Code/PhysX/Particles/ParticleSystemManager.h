#ifndef _PARTICLE_SYSTEM_MANAGER_H
#define _PARTICLE_SYSTEM_MANAGER_H

#include "XML\XMLTreeNode.h"
#include "ParticleSystemCore.h"
#include "Utils\Manager.h"
#include "Utils\TemplatedVectorMapManager.h"

#include <string>
#include <vector>

class CEmitterFactory;

class CParticleSystemManager: public CManager, public CTemplatedVectorMapManager<CParticleSystemCore>
{
    public:
        CParticleSystemManager();
        CParticleSystemManager( CXMLTreeNode& atts );
        ~CParticleSystemManager();

        void Init();
        void Update();
        void Render() {};
        void Refresh();
    private:
        CEmitterFactory* mEmitterFactory;
};

#endif //_PARTICLE_SYSTEM_MANAGER_H