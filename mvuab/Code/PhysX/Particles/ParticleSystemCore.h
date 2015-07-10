#ifndef _PARTICLE_SYSTEM_CORE_H
#define _PARTICLE_SYSTEM_CORE_H

#include "XML\XMLTreeNode.h"
#include "ParticleEmitter.h"
#include "Utils\TemplatedVectorMapManager.h"

#include <string>
#include <vector>

class CParticleEmitter;
class CCubeEmitter;
class CSphereEmitter;

class CParticleSystemCore: public CTemplatedVectorMapManager<CParticleEmitter>
{
public:
  CParticleSystemCore();
  CParticleSystemCore( const CXMLTreeNode& atts );
  ~CParticleSystemCore();

  void Update();
  void Render();
  void Refresh();

};

#endif //_PARTICLE_SYSTEM_CORE_H