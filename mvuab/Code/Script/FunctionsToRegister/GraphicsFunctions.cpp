#include "GraphicsFunctions.h"
#include "luabind_macros.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "RenderableObject\RenderableObject.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject/RenderableObjectsLayersManager.h"
#include "Utils\Name.h"
#include "Math\Color.h"

#include "Object3D.h"

#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraInfo.h"

#include "Cinematics/CinematicManager.h"
#include "Cinematics/Cinematic.h"
#include "Cinematics/CinematicPlayer.h"
#include "Cinematics/CinematicObjectKeyFrame.h"
#include "Cinematics/CinematicObject.h"

#include "Lights\LightManager.h"
#include "Lights\OmniLight.h"

#include "AnimatedModels\AnimatedInstanceModel.h"

#include "PhysicsDefs.h"
#include "Math\Matrix44.h"

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
#include "StaticMeshes/InstanceMesh.h"
#include "Gizmos/GizmoElement.h"
#include "Gizmos/Gizmo.h"
#include "Gizmos/GizmosManager.h"

using namespace luabind;

CInstanceMesh* CreateInstanceMesh( const std::string& Name, const std::string& CoreName )
{
  return new CInstanceMesh( Name, CoreName );
}

CAnimatedInstanceModel* CreateAnimatedInstanceModel( const std::string& Name, const std::string& CoreName )
{
  return new CAnimatedInstanceModel( Name, CoreName );
}

/*
CGizmoElement* CreateGizmoElement( int type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color)
{
  return new CGizmoElement( (CGizmoElement::EGizmoElementType) type, size, position, yaw, pitch, color );
}
*/

void registerCameras( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Cameras" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CAMERA MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CCameraManager, CManager )
  LUA_DECLARE_DEFAULT_CTOR
  LUA_DECLARE_METHOD( CCameraManager, GetCamera )
  LUA_DECLARE_METHOD( CCameraManager, SetCurrentCamera )
  LUA_DECLARE_METHOD( CCameraManager, GetCurrentCamera )
  LUA_DECLARE_METHOD( CCameraManager, GetCurrentCameraName )
  LUA_DECLARE_METHOD( CCameraManager, NewCamera )
  LUA_DECLARE_METHOD( CCameraManager, DeleteCamera )
  LUA_END_DECLARATION
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CAMERA
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS2( CCamera, CName, CObject3D )
  LUA_DECLARE_DEFAULT_CTOR
  LUA_DECLARE_METHOD_PROTO( CCamera, GetDirection, Math::Vect3f( CCamera::* )( void ) )
  LUA_DECLARE_METHOD( CCamera, GetLookAt )
  LUA_DECLARE_METHOD( CCamera, GetVecUp )
  LUA_DECLARE_METHOD( CCamera, GetZFar )
  LUA_DECLARE_METHOD( CCamera, GetDirection )
  LUA_DECLARE_METHOD( CCamera, GetZNear )
  LUA_DECLARE_METHOD( CCamera, GetAspectRatio )
  LUA_DECLARE_METHOD( CCamera, GetEnable )
  LUA_DECLARE_METHOD( CCamera, SetCameraType )
  LUA_DECLARE_METHOD( CCamera, GetCameraType )
  LUA_DECLARE_METHOD( CCamera, SetZNear )
  LUA_DECLARE_METHOD( CCamera, SetZFar )
  LUA_DECLARE_METHOD( CCamera, SetAspectRatio )
  LUA_DECLARE_METHOD( CCamera, SetEnable )
  LUA_DECLARE_METHOD( CCamera, AddYaw )
  LUA_DECLARE_METHOD( CCamera, AddPitch )
  LUA_END_DECLARATION

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CINEMATIC MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CCinematicManager, CManager )
  LUA_DECLARE_METHOD( CCinematicManager, Execute )
  LUA_DECLARE_METHOD( CCinematicManager, GetCinematicActive )
  LUA_END_DECLARATION
}

void registerObject3D( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Cameras" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // OBJECT 3D
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CObject3D )
  LUA_DECLARE_DEFAULT_CTOR
  LUA_DECLARE_CTOR_4( const Math::Vect3f&, float, float, float )
  LUA_DECLARE_METHOD( CObject3D, GetYaw )
  LUA_DECLARE_METHOD( CObject3D, SetYaw )
  LUA_DECLARE_METHOD( CObject3D, GetPitch )
  LUA_DECLARE_METHOD( CObject3D, SetPitch )
  LUA_DECLARE_METHOD( CObject3D, GetRoll )
  LUA_DECLARE_METHOD( CObject3D, SetRoll )
  LUA_DECLARE_METHOD( CObject3D, GetPosition )
  LUA_DECLARE_METHOD( CObject3D, SetPosition )
  LUA_DECLARE_METHOD( CObject3D, GetTransform )
  LUA_DECLARE_METHOD( CObject3D, MakeTransform )
  LUA_DECLARE_METHOD( CObject3D, SetScale )
  LUA_DECLARE_METHOD( CObject3D, GetScale )
  LUA_DECLARE_METHOD( CObject3D, GetDirection )
  LUA_END_DECLARATION
}

void registerGizmos( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Gizmos" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // GIZMOS
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CGizmoElement, CObject3D )
  LUA_DECLARE_CTOR_5( CGizmoElement::EGizmoElementType, float, Math::Vect3f, float, float )
  LUA_END_DECLARATION
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS2( CGizmo, CObject3D, CName )
  LUA_DECLARE_CTOR_4( std::string, Math::Vect3f, float, float )
  LUA_DECLARE_METHOD( CGizmo, AddElement )
  LUA_END_DECLARATION
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CTemplatedVectorMapManager<CGizmo> )
  LUA_DECLARE_METHOD( CTemplatedVectorMapManager<CGizmo>, GetResource )
  LUA_DECLARE_METHOD( CTemplatedVectorMapManager<CGizmo>, AddResource )
  LUA_END_DECLARATION
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CGizmosManager, CTemplatedVectorMapManager<CGizmo> )
  LUA_DECLARE_METHOD( CGizmosManager, CreateGizmoElement )
  LUA_DECLARE_METHOD( CGizmosManager, CreateGizmo )
  LUA_END_DECLARATION
}

void registerRenderableObject( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register RenderableObjects" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // RENDERABLE OBJECT
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS2( CRenderableObject, CObject3D, CName )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CTemplatedVectorMapManager<CRenderableObject> )
  LUA_END_DECLARATION

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // RENDERABLE OBJECTS MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CRenderableObjectsManager, CTemplatedVectorMapManager<CRenderableObject> )
  LUA_DECLARE_DEFAULT_CTOR
  LUA_DECLARE_METHOD( CRenderableObjectsManager, GetResource )
  LUA_DECLARE_METHOD( CRenderableObjectsManager, AddResource )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CTemplatedVectorMapManager<CRenderableObjectsManager> )
  LUA_END_DECLARATION

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // RENDERABLE OBJECTS MANAGER LAYERS
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CRenderableObjectsLayersManager, CTemplatedVectorMapManager<CRenderableObjectsManager> )
  LUA_DECLARE_METHOD( CRenderableObjectsLayersManager, GetResource )
  LUA_END_DECLARATION
}

void registerStaticMesh( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register StaticMesh" );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // INSTANCE MESH
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CInstanceMesh, CRenderableObject )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( CreateInstanceMesh )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_METHOD_WITHOUT_CLASS( CreateAnimatedInstanceModel )
  LUA_END_DECLARATION
}

void registerAnimatedModels( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register StaticMesh" );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // INSTANCE MESH
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_DERIVED_CLASS( CAnimatedInstanceModel, CRenderableObject )
  LUA_DECLARE_METHOD( CAnimatedInstanceModel, ChangeAnimation )
  LUA_DECLARE_METHOD( CAnimatedInstanceModel, ChangeAnimationAction )
  LUA_END_DECLARATION
}

void registerCinematics( lua_State* aLuaState )
{
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CCinematicPlayer )
  LUA_DECLARE_METHOD( CCinematicPlayer, Init )
  LUA_DECLARE_METHOD( CCinematicPlayer, Update )
  LUA_DECLARE_METHOD( CCinematicPlayer, Stop )
  LUA_DECLARE_METHOD( CCinematicPlayer, Play )
  LUA_DECLARE_METHOD( CCinematicPlayer, Pause )
  LUA_DECLARE_METHOD( CCinematicPlayer, IsFinished )
  LUA_DECLARE_METHOD( CCinematicPlayer, GetDuration )
  LUA_DECLARE_METHOD( CCinematicPlayer, GetTimeCurrent )
  LUA_DECLARE_METHOD( CCinematicPlayer, OnRestartCycle )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CCinematicObject )
  LUA_DECLARE_METHOD( CCinematicObject, IsOk )
  LUA_DECLARE_METHOD( CCinematicObject, AddCinematicObjectKeyFrame )
  LUA_DECLARE_METHOD( CCinematicObject, Update )
  LUA_DECLARE_METHOD( CCinematicObject, Stop )
  LUA_DECLARE_METHOD( CCinematicObject, OnRestartCycle )
  LUA_DECLARE_METHOD( CCinematicObject, Render )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CCinematicObjectKeyFrame )
  LUA_DECLARE_METHOD( CCinematicObjectKeyFrame, GetKeyFrameTime )
  LUA_DECLARE_METHOD( CCinematicObjectKeyFrame, SetKeyFrameTime )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CCinematic )
  LUA_DECLARE_METHOD( CCinematic, Stop )
  LUA_DECLARE_METHOD( CCinematic, Play )
  LUA_DECLARE_METHOD( CCinematic, Pause )
  LUA_DECLARE_METHOD( CCinematic, LoadXML )
  LUA_DECLARE_METHOD( CCinematic, AddCinematicObject )
  LUA_DECLARE_METHOD( CCinematic, Update )
  LUA_DECLARE_METHOD( CCinematic, Render )
  LUA_END_DECLARATION
}

void registerGraphics( lua_State* aLuaState )
{
  registerObject3D( aLuaState );
  registerCameras( aLuaState );
  registerGizmos( aLuaState );
  registerRenderableObject( aLuaState );
  registerStaticMesh( aLuaState );
  registerAnimatedModels( aLuaState );
  registerCinematics( aLuaState );
}