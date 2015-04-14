#include "PhysXFunctions.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Actor\PhysicController.h"
#include "PhysicsManager.h"
#include "Utils\Name.h"
#include "PhysicsDefs.h"
#include "Utils\Defines.h"

#include "Particles\ParticleManager.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\CubeEmitter.h"

#include "luabind_macros.h"

#include <set>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

bool Add_PhysicController( CPhysicsManager* PhysicManager, CPhysicController* PhysicController )
{
  return PhysicManager->AddPhysicController( PhysicController );
}

void Move_PhysicController( CPhysicController* PhysicController, const Math::Vect3f& Direction, float Dt )
{
  const Math::Vect3f& DirectionConst = Direction;
  PhysicController->Move( DirectionConst, Dt );
}

float RaycastDistance( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction, uint32 impactMask, float maxDist )
{
  SCollisionInfo hit_info;
  if (!PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info, maxDist ))
  {
    return 0.0f;
  }
  return hit_info.m_fDistance;
}

ECollisionGroup RaycastType( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction, uint32 impactMask )
{
  SCollisionInfo hit_info;
  CPhysicUserData* l_PUD = PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info);
  if (l_PUD)
	  if (l_PUD->GetController())
		return l_PUD->GetController()->GetColisionGroup();
	  else
		  return l_PUD->GetMyCollisionGroup();
  else
    return (ECollisionGroup)-1;
}

template<class T>
size_t set_getIdByResource( std::set<T>& vec, T val )
{
  std::set<T>::iterator it = vec.begin()
                             , it_end = vec.end();

  size_t i = 0;

  for ( ; it != it_end ; ++it )
  {
    if ( *it == val )
      return i;

    ++i;
  }

  return 0;
}

template<class T>
void removeResource( T val )
{
  CHECKED_DELETE( val );
}

void registerPhysX( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CPhysicUserData, CName>( "CPhysicUserData" )
    .def( constructor<const std::string&>() )
    .def( "SetColor", ( void ( CPhysicUserData::* )( const float, const float, const float,
                        const float ) ) &CPhysicUserData::SetColor )
    .def( "GetColor", &CPhysicUserData::GetColor )
    .def( "SetPaint", &CPhysicUserData::SetPaint )
    .def( "SetName", &CPhysicUserData::SetName )
    .def( "GetActor", &CPhysicUserData::GetActor )
    .def( "GetName", &CPhysicUserData::GetName )
    .def( "GetController", &CPhysicUserData::GetController )
  ];
  module( m_LS )
  [
    class_<CPhysicActor>( "CPhysicActor" )
    .def( constructor<CPhysicUserData*>() )
    //.def( "GetUserData", &CPhysicActor::GetUserData )
    .def( "CreateBody", &CPhysicActor::CreateBody )
    .def( "AddSphereShape", &CPhysicActor::AddSphereShapeHardcoded )
    .def( "AddBoxShape", &CPhysicActor::AddBoxShapeHardcoded )
    .def( "AddPlaneShape", &CPhysicActor::AddPlaneShape )
    .def( "AddMeshShape", &CPhysicActor::AddMeshShape )
    .def( "AddCapsuleShape", &CPhysicActor::AddCapsuleShape )
    .def( "AddImpulseAtPos", &CPhysicActor::AddImpulseAtPos )
    .def( "AddVelocityAtPos", &CPhysicActor::AddVelocityAtPos )
    .def( "AddAcelerationAtPos", &CPhysicActor::AddAcelerationAtPos )
    //.def( "AddForceAtPos", &CPhysicActor::AddForceAtPos )
    .def( "AddTorque", &CPhysicActor::AddTorque )
    .def( "SetGlobalPosition", &CPhysicActor::SetGlobalPosition )
  ];
  module( m_LS ) [
    class_<CPhysicsManager>( "CPhysicsManager" )
    .def( "RaycastClosestActor", &CPhysicsManager::RaycastClosestActor )
    .def( "RaycastClosestActorName", &CPhysicsManager::RaycastClosestActorName )
    //Adds
    .def( "AddPhysicActor", &CPhysicsManager::AddPhysicActor )
    .def( "AddPhysicRevoluteJoint", &CPhysicsManager::AddPhysicRevoluteJoint )
    .def( "AddPhysicSphericalJoint", &CPhysicsManager::AddPhysicSphericalJoint )
    .def( "AddPhysicController", &Add_PhysicController )
    .def( "AddActor", ( bool ( CPhysicsManager::* )( const std::string&, std::string&, const Math::Vect3f&, const Math::CColor&,
                        bool, const Math::Vect3f&, const Math::Vect3f&, const Math::Vect3f&,
                        NxCCDSkeleton*, uint32 ) )&CPhysicsManager::AddActor )
    .def( "AddController", &CPhysicsManager::AddController )
    .def( "AddMesh", &CPhysicsManager::AddMesh )
    //Release
    .def( "ReleasePhysicActor", &CPhysicsManager::ReleasePhysicActor )
    .def( "ReleasePhysicController", &CPhysicsManager::ReleasePhysicController )
    .def( "RemoveUserData", &removeResource<CPhysicUserData*> )
    .def( "Load", &CPhysicsManager::Load )
    .def( "Init", &CPhysicsManager::Init )
    .def( "Done", &CPhysicsManager::Done )
    .def( "Reload", &CPhysicsManager::Reload )
    .def( "ReloadXML", &CPhysicsManager::ReloadXML )
    //Gets
    .def( "GetUserData", ( CPhysicUserData * ( CPhysicsManager::* )( const std::string& ) )
          &CPhysicsManager::GetUserData )

    .def( "OverlapSphere", &CPhysicsManager::OverlapSphereHardcoded )
    .def( "OverlapSphereActor", &CPhysicsManager::OverlapSphereActor )
    .def( "GetController", &CPhysicsManager::CMapManager<CPhysicController>::GetResource )
    .def( "GetActor", &CPhysicsManager::CMapManager<CPhysicActor>::GetResource )
    .def( "RaycastDistance", &RaycastDistance )
    .def( "RaycastType", &RaycastType )
  ];
  module( m_LS ) [
    class_<CPhysicController, CObject3D>( "CPhysicController" )
    .def( constructor<float, float, float, float, float, ECollisionGroup, CPhysicUserData*, const Math::Vect3f&, float>() )
    .def( "Move", ( void ( CPhysicController::* )( const Math::Vect3f&, float ) )&CPhysicController::Move )
    //.def( "Move", &Move_PhysicController )
    .def( "Jump", &CPhysicController::Jump )
    .def( "GetPosition", &CPhysicController::GetPosition )
    .def( "SetPosition", &CPhysicController::SetPosition )
    .def( "SetHeight", &CPhysicController::SetHeight )
    .def( "GetHeight", &CPhysicController::GetHeight )
    .def( "UpdateCharacterExtents", &CPhysicController::UpdateCharacterExtents )
  ];

  module( m_LS ) [
    class_<std::set<CPhysicUserData*>>( "ListaPUD" )
    .def( constructor<std::set<CPhysicUserData*>>() )
    .def( "size", &std::set<CPhysicUserData*>::size )
    .def( "getIdByResource", &set_getIdByResource<CPhysicUserData*> )
  ];

  module( m_LS ) [
    class_<std::vector<CPhysicUserData*>>( "vPUD" )
    .def( constructor<std::vector<CPhysicUserData*>>() )
    .def( "size", &std::vector<CPhysicUserData*>::size )
  ];

  LUA_BEGIN_DECLARATION( m_LS )
    LUA_DECLARE_CLASS( CParticleManager )
    LUA_DECLARE_METHOD( CParticleManager, AddEmitter )
    LUA_DECLARE_METHOD( CParticleManager, CreateCubeEmitter )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( m_LS )
    LUA_DECLARE_CLASS( CParticleEmitter )
    LUA_DECLARE_METHOD( CParticleEmitter, SetActive )
    LUA_DECLARE_METHOD( CParticleEmitter, SetAcceleration )
    LUA_DECLARE_METHOD( CParticleEmitter, SetDirection )
    LUA_DECLARE_METHOD( CParticleEmitter, SetPosition )
    LUA_DECLARE_METHOD( CParticleEmitter, SetVelocity )
    LUA_DECLARE_METHOD( CParticleEmitter, SetLifeTime )
    LUA_DECLARE_METHOD( CParticleEmitter, SetEmitterLifeTime )
	LUA_DECLARE_METHOD( CParticleEmitter, SetSize )
    LUA_DECLARE_METHOD( CParticleEmitter, SetTextureName )
	LUA_DECLARE_METHOD( CParticleEmitter, SetTimeToEmit )
    LUA_DECLARE_METHOD( CParticleEmitter, Generate )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( m_LS )
    LUA_DECLARE_DERIVED_CLASS( CCubeEmitter, CParticleEmitter )
    LUA_DECLARE_DEFAULT_CTOR
    LUA_DECLARE_METHOD( CCubeEmitter, SetDepth )
    LUA_DECLARE_METHOD( CCubeEmitter, SetWidth )
    LUA_DECLARE_METHOD( CCubeEmitter, SetHeight )
    LUA_DECLARE_METHOD( CCubeEmitter, SetRandom )
    LUA_DECLARE_METHOD( CCubeEmitter, SetTextureName )
  LUA_END_DECLARATION
}