#include "EnumDefinitions.h"

#include "LuaEnumBinder.h"

#include "Cameras\Camera.h"
#include "PhysicsDefs.h"

using namespace luabind;

void registerEnum( lua_State* m_LS )
{
  add_enum_to_lua( m_LS, "CameraType",
                   "Free", CCamera::Free,
                   "TPS", CCamera::ThirdPerson,
                   "FPS", CCamera::FirstPerson,
                   "Spherical", CCamera::Spherical,
                   "Cinematical", CCamera::Cinematical, 0 );

  add_enum_to_lua( m_LS, "CollisionGroup",
                   "ECG_ESCENE", ECG_ESCENE,
                   "ECG_PLAYER", ECG_PLAYER,
                   "ECG_ENEMY", ECG_ENEMY,
                   "ECG_TRIGGERS", ECG_TRIGGERS,
                   "ECG_MALGLOBAL", ECG_MALGLOBAL,
                   "ECG_COBERTURES", ECG_COBERTURES,
                   "ECG_DYNAMIC_OBJECTS", ECG_DYNAMIC_OBJECTS,
                   "ECG_STATIC_OBJECTS", ECG_STATIC_OBJECTS,
                   "ECG_EXPLOSIONS", ECG_EXPLOSIONS,
                   "ECG_RAY_SHOOT", ECG_RAY_SHOOT,
                   "ECG_RAY_SHOOT_PLAYER", ECG_RAY_SHOOT_PLAYER,
                   "ECG_FORCE", ECG_FORCE,
                   "ECG_RAY_IA_GRAPH", ECG_RAY_IA_GRAPH,
                   "ECG_CAMERA", ECG_CAMERA,
                   "ECG_RAGDOLL", ECG_RAGDOLL,
                   "ECG_RAGDOLL_PLAYER", ECG_RAGDOLL_PLAYER,
                   "ECG_VIGIA", ECG_VIGIA,
                   "ECG_LIMITS", ECG_LIMITS,
                   "ECG_LAST_GROUP", ECG_LAST_GROUP, 0 );
}