#include "PhysXFunctions.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Actor\PhysicController.h"
#include "PhysicsManager.h"
#include "Utils\Name.h"
#include "PhysicsDefs.h"
#include "Utils\Defines.h"
#include "Utils/Manager.h"

#include "Engine.h"

#include <sstream>

#include "Particles\ParticleEmitter.h"

#include "EngineManagers.h"

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

#include "Characters/Enemies/Enemy.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Triggers/Trigger.h"
#include "Triggers/TriggerManager.h"

#include "Particles/InstanceParticle.h"
#include "Particles/ParticleSystemCore.h"
#include "Particles/ParticleSystemManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

#include "Joints\PhysicRevoluteJoint.h"

#define REGISTER_LUA_FUNCTION(LuaState, AddrFunction) {luabind::module(LuaState) [ luabind::def(#AddrFunction,AddrFunction) ];}

using namespace luabind;

CParticleSystemManager* GetParticleSystemManager()
{
  return PSManager;
}

CTrigger* CreateTrigger( std::string name, Math::Vect3f position, Math::Vect3f size, bool aEnter, bool aStay, bool aLeave, std::string aEnterEvent ,std::string aStayEvent, std::string aLeaveEvent) 
{
  return new CTrigger( name, position, size, Math::colBLUE, 0, false, aEnter, aStay, aLeave, aEnterEvent, aStayEvent, aLeaveEvent);
}

bool Add_PhysicController( CPhysicsManager* PhysicManager, CPhysicController* PhysicController )
{
  return PhysicManager->AddPhysicController( PhysicController );
}

void Move_PhysicController( CPhysicController* PhysicController, const Math::Vect3f& Direction, float Dt )
{
  const Math::Vect3f& DirectionConst = Direction;
  PhysicController->Move( DirectionConst, Dt );
}

float RaycastDistance( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction, uint32 impactMask,
                       float maxDist )
{
  SCollisionInfo hit_info;

  if ( !PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info, maxDist ) )
    return 0.0f;

  return hit_info.m_fDistance;
}

SCollisionInfo RaycastCollisionGroup( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction, uint32 impactMask,
                                      float maxDist )
{
  SCollisionInfo hit_info;
  PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info, maxDist );
  return hit_info;
}

std::string RaycastName( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction, uint32 impactMask,
                         float maxDist )
{
    SCollisionInfo hit_info;
    CPhysicUserData* lPUD = PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info, maxDist );

    if ( lPUD )
    {
        return lPUD->GetName();
    }

    return "";
}

ECollisionGroup RaycastType( CPhysicsManager* PhysicManager, Math::Vect3f position, Math::Vect3f direction,
                             uint32 impactMask )
{
  SCollisionInfo hit_info;
  CPhysicUserData* l_PUD = PhysicManager->RaycastClosestActor( position, direction, impactMask, hit_info );

  if ( l_PUD )
    if ( l_PUD->GetController() )
      return l_PUD->GetController()->GetColisionGroup();
    else
      return l_PUD->GetMyCollisionGroup();
  else
    return ( ECollisionGroup ) - 1;
}

template<class T> size_t set_getIdByResource( std::set<T>& vec, T val )
{
  std::set<T>::iterator it = vec.begin() , it_end = vec.end();
  size_t i = 0;

  for ( ; it != it_end ; ++it )
  {
    if ( *it == val )
      return i;

    ++i;
  }

  return 0;
}

template<class T> void removeResource( T val )
{
  CHECKED_DELETE( val );
}

template<class T> T vector_get( std::vector<T>& vec, size_t i )
{
  return vec[i];
}

bool PlayerInSight( CPhysicsManager* PhysicManager, float _Distance, float _Angle, const Math::Vect3f& _Position,
                    const Math::Vect3f& _Direction )
{
  const std::vector<CPhysicUserData*>& l_UserDatas = PhysicManager->OverlapConeActor( _Distance, _Angle, _Position,
      _Direction, 0xffffffff );
  std::vector<CPhysicUserData*>::const_iterator it = l_UserDatas.begin(),
                                                it_end = l_UserDatas.end();

  for ( ; it != it_end; ++it )
  {
    CPhysicUserData* l_UserData = *it;
    CPhysicController* l_Controller = l_UserData->GetController();
    std::string name = l_Controller ? l_Controller->GetUserData()->GetName() : l_UserData->GetName();

    if ( name == "Player" )
    {
      SCollisionInfo hit;
      CPhysicUserData* lRayCollision = PhysicManager->RaycastClosestActor( _Position, _Direction, 0xffffff, hit, _Distance );
      CPhysicController* lRayController = lRayCollision->GetController();

      if ( lRayController )
      {
        std::string lName( lRayController->GetUserData()->GetName() );

        if ( name == "Player" )
          return true;
        else
          return false;
      }
      else
        return false;
    }
  }

  return false;
}

bool AddGrenade( CPhysicsManager* PhysicManager, const std::string& name, const std::string& group,
                 const Math::Vect3f& dimensions,
                 const Math::Vect3f& position, unsigned int mask )
{
  return PhysicManager->AddActor
         (
           name, group, dimensions, Math::colWHITE, false,
           position, Math::Vect3f(), Math::Vect3f(),
           0, mask
         );
  return true;
}

void registerParticles( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Billboard" );
  
  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_DERIVED_CLASS2( CParticleInstance, CObject3D, CName )
    LUA_DECLARE_CTOR_3( const std::string&, const std::string&, const std::string& )
    LUA_DECLARE_METHOD( CParticleInstance, Reset )
    LUA_DECLARE_METHOD( CParticleInstance, SetDirection )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CParticleSystemManager, CManager )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( GetParticleSystemManager )
  LUA_END_DECLARATION
}

void registerPhysX( lua_State* m_LS )
{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // PHYSIC USER DATA
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_DERIVED_CLASS( CPhysicUserData, CName )
  LUA_DECLARE_CTOR_1( const std::string& )
  LUA_DECLARE_METHOD_PROTO( CPhysicUserData, SetColor, void ( CPhysicUserData::* )( const float, const float, const float,
                            const float ) )
  LUA_DECLARE_METHOD( CPhysicUserData, SetPaint )
  LUA_DECLARE_METHOD( CPhysicUserData, GetName )
  LUA_DECLARE_METHOD( CPhysicUserData, GetController )
  LUA_END_DECLARATION


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // PHYSIC ACTOR
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS( CPhysicActor )
  LUA_DECLARE_CTOR_1( CPhysicUserData* )
  //LUA_DECLARE_METHOD( CPhysicActor, GetUserData )
  LUA_DECLARE_METHOD( CPhysicActor, CreateBody )
  LUA_DECLARE_METHOD( CPhysicActor, AddImpulseAtPos )
  //LUA_DECLARE_METHOD( CPhysicActor, AddForceAtPos )
  LUA_DECLARE_METHOD( CPhysicActor, SetPosition )
  LUA_DECLARE_METHOD( CPhysicActor, Activate )
  LUA_DECLARE_METHOD( CPhysicActor, GetPosition )
  LUA_DECLARE_METHOD( CPhysicActor, MoveGlobalPosition)
  LUA_END_DECLARATION


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // PHYSICS MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS( CPhysicsManager )
  LUA_DECLARE_METHOD( CPhysicsManager, RaycastClosestActor )
  LUA_DECLARE_METHOD( CPhysicsManager, RaycastClosestActorName )
  LUA_DECLARE_METHOD( CPhysicsManager, AddPhysicActor )
  LUA_DECLARE_METHOD( CPhysicsManager, AddActorCapsule )
  LUA_DECLARE_METHOD( CPhysicsManager, AddController )
  LUA_DECLARE_METHOD( CPhysicsManager, ReleasePhysicActor )
  LUA_DECLARE_METHOD( CPhysicsManager, ReleasePhysicController )
  LUA_DECLARE_METHOD_PROTO( CPhysicsManager, GetUserData, CPhysicUserData * ( CPhysicsManager::* )( const std::string& ) )
  LUA_DECLARE_METHOD( CPhysicsManager, OverlapSphereController )
  .def( "OverlapSphere", &CPhysicsManager::OverlapSphereHardcoded )
  .def( "AddPhysicController", &Add_PhysicController )
  .def( "GetController", &CPhysicsManager::CMapManager<CPhysicController>::GetResource )
  .def( "GetJoint", &CPhysicsManager::CMapManager<CPhysicRevoluteJoint>::GetResource )
  .def( "GetActor", &CPhysicsManager::CMapManager<CPhysicActor>::GetResource )
  .def( "RemoveActor", &CPhysicsManager::CMapManager<CPhysicActor>::RemoveResource )
  .def( "RaycastType", &RaycastType )
  .def( "PlayerInSight", &PlayerInSight )
  //  .def( "GetClosestEnemy", &GetClosestEnemy )
  .def( "RaycastCollisionGroup", &RaycastCollisionGroup )
  .def( "RaycastName", &RaycastName )
  LUA_END_DECLARATION

  REGISTER_LUA_FUNCTION( m_LS, AddGrenade );
  /*  LUA_BEGIN_DECLARATION( aLuaState )
      LUA_DECLARE_METHOD_WITHOUT_CLASS( CreateInstanceMesh )
      LUA_END_DECLARATION*/

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // PHYSICCONTROLLER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_DERIVED_CLASS( CPhysicController, CObject3D )
  LUA_DECLARE_CTOR_9( float, float, float, float, float, ECollisionGroup, CPhysicUserData*, const Math::Vect3f&, float )
  LUA_DECLARE_METHOD_PROTO( CPhysicController, Move, void ( CPhysicController::* )( const Math::Vect3f&, float ) )
  LUA_DECLARE_METHOD_PROTO( CPhysicController, Jump, void( CPhysicController::* )( float ) )
  LUA_DECLARE_METHOD( CPhysicController, GetPosition )
  LUA_DECLARE_METHOD( CPhysicController, SetPosition )
  LUA_DECLARE_METHOD( CPhysicController, SetHeight )
  LUA_DECLARE_METHOD( CPhysicController, GetHeight )
  LUA_DECLARE_METHOD( CPhysicController, UpdateCharacterExtents )
  LUA_DECLARE_METHOD( CPhysicController, SetbCrouch )
  LUA_DECLARE_METHOD( CPhysicController, IsJumping )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS( SCollisionInfo )
  LUA_DECLARE_DEFAULT_CTOR
  .def_readwrite( "CollisionPoint", &SCollisionInfo::m_CollisionPoint )
  .def_readwrite( "Distance", &SCollisionInfo::m_fDistance )
  .def_readwrite( "Normal", &SCollisionInfo::m_Normal )
  .def_readwrite( "Name", &SCollisionInfo::m_Name )
  LUA_END_DECLARATION

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
    .def( "GetResource", &vector_get<CPhysicUserData*> )
  ];
  
  /* TRIGGER */
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS(CTriggerManager)
  LUA_DECLARE_METHOD( CTriggerManager, ReleaseTrigger )
  LUA_DECLARE_METHOD( CTriggerManager, AddTrigger )
  LUA_DECLARE_METHOD( CTriggerManager, GetTriggerByName )
  LUA_DECLARE_METHOD( CTriggerManager, ModifyTrigger )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_DERIVED_CLASS(CTrigger, CObject3D)
  LUA_DECLARE_METHOD( CTrigger, SetActive )
  LUA_DECLARE_METHOD( CTrigger, GetActive )
  LUA_DECLARE_METHOD( CTrigger, SetPosition )
  LUA_DECLARE_METHOD( CTrigger, GetPosition )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_CLASS(CPhysicRevoluteJoint)
  LUA_DECLARE_METHOD( CPhysicRevoluteJoint, ActiveMotor )
  LUA_END_DECLARATION
  
  LUA_BEGIN_DECLARATION( m_LS )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( CreateTrigger )
  LUA_END_DECLARATION

  registerParticles(m_LS);
}