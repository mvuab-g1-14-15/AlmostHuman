#include "PhysXFunctions.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Actor\PhysicController.h"
#include "PhysicsManager.h"
#include "Utils\Name.h"


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
    .def( "AddSphereShape", &CPhysicActor::AddSphereShape )
    .def( "AddBoxShape", &CPhysicActor::AddBoxShapeHardcoded )
    .def( "AddPlaneShape", &CPhysicActor::AddPlaneShape )
    .def( "AddMeshShape", &CPhysicActor::AddMeshShape )
    .def( "AddCapsuleShape", &CPhysicActor::AddCapsuleShape )
    .def( "AddImpulseAtPos", &CPhysicActor::AddImpulseAtPos )
    .def( "AddVelocityAtPos", &CPhysicActor::AddVelocityAtPos )
    .def( "AddAcelerationAtPos", &CPhysicActor::AddAcelerationAtPos )
    //.def( "AddForceAtPos", &CPhysicActor::AddForceAtPos )
    .def( "AddTorque", &CPhysicActor::AddTorque )
  ];
  module( m_LS ) [
    class_<CPhysicsManager>( "CPhysicsManager" )
    .def( "RaycastClosestActor", &CPhysicsManager::RaycastClosestActor )
    //Adds
    .def( "AddPhysicActor", &CPhysicsManager::AddPhysicActor )
    .def( "AddPhysicRevoluteJoint", &CPhysicsManager::AddPhysicRevoluteJoint )
    .def( "AddPhysicSphericalJoint", &CPhysicsManager::AddPhysicSphericalJoint )
    //Release
    .def( "ReleasePhysicActor", &CPhysicsManager::ReleasePhysicActor )
    .def( "Load", &CPhysicsManager::Load )
    .def( "Init", &CPhysicsManager::Init )
    .def( "Done", &CPhysicsManager::Done )
    .def( "Reload", &CPhysicsManager::Reload )
    .def( "ReloadXML", &CPhysicsManager::ReloadXML )
    //Gets
    .def( "GetActor", &CPhysicsManager::GetActor )
    .def( "GetUserData", ( CPhysicUserData * ( CPhysicsManager::* )( const std::string& ) )
          &CPhysicsManager::GetUserData )
  ];
  module( m_LS ) [
    class_<CPhysicController>( "CPhysicController" )
	.def( constructor<float, float, float, float, float, ECollisionGroup, CPhysicUserData*, const Math::Vect3f&, float>() )
    .def( "Move", &CPhysicController::Move )
    .def( "Jump", &CPhysicController::Jump )
    .def( "GetPosition", &CPhysicController::GetPosition )
  ];
}

