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

#include "Cinematics/Cinematic.h"
#include "Cinematics/CinematicPlayer.h"

#include "Lights\LightManager.h"
#include "Lights\OmniLight.h"

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

CGizmoElement* CreateGizmoElement( int type, float size, Math::Vect3f position, float yaw, float pitch, Math::CColor color)
{
  return new CGizmoElement( (CGizmoElement::EGizmoElementType) type, size, position, yaw, pitch, color );
}

void registerCameras( lua_State* aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Cameras" );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CAMERA MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_CLASS( CCameraManager )
    LUA_DECLARE_DEFAULT_CTOR
    LUA_DECLARE_METHOD( CCameraManager, GetCamera )
    LUA_DECLARE_METHOD( CCameraManager, SetCurrentCamera )
    LUA_DECLARE_METHOD( CCameraManager, GetCurrentCamera )
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
}

void registerObject3D( lua_State * aLuaState )
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
  LUA_END_DECLARATION
}

void registerGizmos( lua_State * aLuaState )
{
  ASSERT( aLuaState, "LuaState error in Register Gizmos" );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // OBJECT 3D
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_DERIVED_CLASS( CGizmoElement, CObject3D )
    LUA_DECLARE_CTOR_5(CGizmoElement::EGizmoElementType, float, Math::Vect3f, float, float)
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_DERIVED_CLASS2( CGizmo, CObject3D, CName )
    LUA_DECLARE_CTOR_4(std::string, Math::Vect3f, float, float)
    LUA_DECLARE_METHOD( CGizmo, AddElement )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_CLASS( CTemplatedVectorMapManager<CGizmo> )
    LUA_DECLARE_METHOD( CTemplatedVectorMapManager<CGizmo>, GetResource )
    LUA_DECLARE_METHOD( CTemplatedVectorMapManager<CGizmo>, AddResource )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
    LUA_DECLARE_DERIVED_CLASS( CGizmosManager, CTemplatedVectorMapManager<CGizmo> )
    LUA_DECLARE_METHOD(CGizmosManager, CreateGizmoElement)
    //.def( "CreateGizmoElement", &CreateGizmoElement )
    LUA_DECLARE_METHOD( CGizmosManager, CreateGizmo)
  LUA_END_DECLARATION
}

void registerGraphics( lua_State* aLuaState )
{
  registerObject3D( aLuaState );
  registerCameras( aLuaState );
  registerGizmos( aLuaState );

  module( aLuaState )
  [
    def( "CreateInstanceMesh", &CreateInstanceMesh )
  ];

  module( aLuaState )
  [
    class_<CRenderableObject, bases<CObject3D, CName>>( "CRenderableObject" )
  ];
  module( aLuaState )
  [
    class_<CInstanceMesh, CRenderableObject>( "CInstanceMesh" )
  ];
  module( aLuaState )
  [
    class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>( "CTemplatedVectorMapManagerCRenderableObjectManager" )
  ];
  module( aLuaState )
  [
    class_<CRenderableObjectsLayersManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>( "CRenderableObjectsManager" )
    .def( "GetResource", &CRenderableObjectsLayersManager::GetResource )
  ];
  module( aLuaState )
  [
    class_<CTemplatedVectorMapManager<CRenderableObject>>( "CTemplatedVectorMapManagerCRenderableObject" )
  ];
  module( aLuaState )
  [
    class_<CRenderableObjectsManager, CTemplatedVectorMapManager<CRenderableObject>>( "CRenderableObjectsManager" )
    .def( constructor<>() )
    .def( "Update", &CRenderableObjectsManager::Update )
    .def( "Render", &CRenderableObjectsManager::Render )
    .def( "CleanUp", &CRenderableObjectsManager::CleanUp )
    .def( "Load", &CRenderableObjectsManager::Load )
    .def( "AddResource", &CRenderableObjectsManager::AddResource )
    .def( "CreateCinematic", &CRenderableObjectsManager::CreateCinematic )
    .def( "GetResource", &CRenderableObjectsManager::GetResource )
  ];
  module( aLuaState )
  [
    class_<CGraphicsManager>( "CGraphicsManager" )
    .def( constructor<>() )
    .def( "BeginRender", &CGraphicsManager::BeginRender )
    .def( "DisableAlphaBlend", &CGraphicsManager::DisableAlphaBlend )
    .def( "DisableZBuffering", &CGraphicsManager::DisableZBuffering )
    .def( "DrawAxis", &CGraphicsManager::DrawAxis )
    .def( "DrawBox", &CGraphicsManager::DrawBox )
    //.def("DrawCamera", &CGraphicsManager::DrawCamera)
    .def( "DrawCapsule", &CGraphicsManager::DrawCapsule )
    .def( "DrawCircle", &CGraphicsManager::DrawCircle )
    .def( "DrawCube", ( void( CGraphicsManager::* )( float32 ) ) &CGraphicsManager::DrawCube )
    .def( "DrawCylinder", &CGraphicsManager::DrawCylinder )
    .def( "DrawGrid", &CGraphicsManager::DrawGrid )
    .def( "DrawIcoSphere", &CGraphicsManager::DrawIcoSphere )
    .def( "DrawLine", &CGraphicsManager::DrawLine )
    .def( "DrawPlane", &CGraphicsManager::DrawPlane )
    .def( "DrawQuad2D", &CGraphicsManager::DrawQuad2D )
    //.def("DrawQuad3D", &CGraphicsManager::DrawQuad3D)
    .def( "DrawRectangle2D", &CGraphicsManager::DrawRectangle2D )
    .def( "DrawSphere", &CGraphicsManager::DrawSphere )
    .def( "EnableAlphaBlend", &CGraphicsManager::EnableAlphaBlend )
    .def( "EnableZBuffering", &CGraphicsManager::EnableZBuffering )
    .def( "EndRender", &CGraphicsManager::EndRender )
    //.def("GetCurrentCamera", &CGraphicsManager::GetCurrentCamera)
    .def( "GetDevice", &CGraphicsManager::GetDevice )
    .def( "GetRay", &CGraphicsManager::GetRay )
    .def( "GetWidthAndHeight", &CGraphicsManager::GetWidthAndHeight )
    .def( "Init", &CGraphicsManager::Init )
    .def( "Release", &CGraphicsManager::Release )
    .def( "Render", &CGraphicsManager::Render )
    .def( "RenderCursor", &CGraphicsManager::RenderCursor )
    //.def( "SetTransform", ( void( CGraphicsManager::* )(const Math::Mat44f& ) ) &CGraphicsManager::SetTransform )
    .def( "SetupMatrices", &CGraphicsManager::SetupMatrices )
    .def( "Update", &CGraphicsManager::Update )
  ];
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  module( aLuaState )
  [
    class_<CCinematicPlayer>( "CCinematicPlayer" )
  ];
  module( aLuaState )
  [
    class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>( "CCinematic" )
    .def( constructor<const std::string&>() )
    .def( "Stop", &CCinematic::Stop )
    .def( "Play", &CCinematic::Play )
    .def( "Pause", &CCinematic::Pause )
  ];
}