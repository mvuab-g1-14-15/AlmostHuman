#ifndef _EMITTER_FACTORY_H
#define _EMITTER_FACTORY_H
#pragma once

#include "Utils/SingletonPattern.h"
#include "Particles/ps.h"

#include <map>

class CParticleEmitter;

class CEmitterFactory : public CSingleton< CEmitterFactory >
{
    public:
        void InitFactory();
        CParticleEmitter* CreateEmitter( const std::string& aEmitterType );

    private:
        std::map< ps::TEmitterType, ps::TSpawnFunction > mMap;
};

#endif