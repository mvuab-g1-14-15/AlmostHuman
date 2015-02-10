//----------------------------------------------------------------------------------
// CPhysicActor class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un material físico.
//----------------------------------------------------------------------------------
#pragma once
#ifndef __PHYSIC_MATERIAL_CLASS_H__
#define __PHYSIC_MATERIAL_CLASS_H__

#include "Utils\Defines.h"

//---Forward Declarations---
class NxMaterial;

//--------------------------

class CPhysicMaterial
{
public:
	//--- Init and End protocols------------------------------------------
	CPhysicMaterial	(float restitution, float staticFriction, float dynamicFriction); 
	~CPhysicMaterial	();


private:
  NxMaterial* m_Material;

};

#endif //__PHYSIC_MATERIAL_CLASS_H__