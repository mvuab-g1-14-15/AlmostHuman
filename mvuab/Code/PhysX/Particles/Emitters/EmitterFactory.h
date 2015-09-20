#ifndef _EMITTER_FACTORY_H
#define _EMITTER_FACTORY_H
#pragma once

#include "Utils/SingletonPattern.h"
#include "Particles/ps.h"
#include "XML\XMLTreeNode.h"

#include <map>
#include <string>

class CParticleEmitter;

class CEmitterFactory : public CSingleton< CEmitterFactory >
{
    public:
        void InitFactory();
        CParticleEmitter* CreateEmitter( const std::string& aEmitterType, const CXMLTreeNode& aNode );

		void SetEmittersPath( const std::string & aEmittersPath ) { mEmittersPath = aEmittersPath; }

    private:
        std::map< ps::TEmitterType, ps::TSpawnFunction > mMap;
		std::string mEmittersPath;
};

#endif